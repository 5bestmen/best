
#ifndef ASIO_SOCKET_H_
#define ASIO_SOCKET_H_

#include <boost/container/list.hpp>
 

#include "asio_packer.h"
#include "asio_timer.h"

//默认的打包器，也可以通过模板参数控制，提供这个宏可以让使用者省略相应的模板参数。
#ifndef ASIO_DEFAULT_PACKER
#define ASIO_DEFAULT_PACKER CPacker
#endif

namespace ASIO
{
	template<typename Socket, typename Packer, typename Unpacker, typename InMsgType = typename Packer::msg_type, typename OutMsgType = typename Unpacker::msg_type>
	class CAsioSocket : public CAsioTimer
	{
	public:
		//keep size() constant time would better, because we invoke it frequently, so don't use std::list(gcc)
		using in_container_type = boost::container::list<InMsgType>;
	//	typedef typename Unpacker::container_type out_container_type;
		using out_container_type = typename Unpacker::container_type;

	protected:
		static const unsigned int TIMER_BEGIN = CAsioTimer::TIMER_END;
		static const unsigned int TIMER_DISPATCH_MSG = TIMER_BEGIN;
		static const unsigned int TIMER_SUSPEND_DISPATCH_MSG = TIMER_BEGIN + 1;
		static const unsigned int TIMER_HANDLE_POST_BUFFER = TIMER_BEGIN + 2;
		static const unsigned int TIMER_RE_DISPATCH_MSG = TIMER_BEGIN + 3;
		static const unsigned int TIMER_END = TIMER_BEGIN + 10;

		CAsioSocket(boost::asio::io_service& io_svc) : CAsioTimer(io_svc), m_nID(static_cast<unsigned int>(-1)), m_NextLayer(io_svc), m_pPacker(boost::make_shared<Packer>()), m_bIsStarted(false)
		{
			ResetState();
		}

		template<typename Arg>
		CAsioSocket(boost::asio::io_service& io_svc, Arg& arg) : CAsioTimer(io_svc), m_nID(static_cast<unsigned int>(-1)), m_NextLayer(io_svc, arg), m_pPacker(boost::make_shared<Packer>()), m_bIsStarted(false)
		{ 
			ResetState();
		}

		void Reset()
		{
			close();
			ResetState();
			ClearBuffer();
			m_tmRecvIdle = boost::posix_time::time_duration();
			CAsioTimer::Reset();
		}
		//重置所有状态。
		void ResetState()
		{
			m_bIsPosting = false;
			m_bIsSending = m_bSuspendSendMsg = false;
			m_bIsDispatching = m_bSuspendDispatchMsg = false;
		//	m_bIsStarted = false;
		//	m_tmRecvIdle = boost::posix_time::time_duration();
		}
		//清空所有buffer。
		void ClearBuffer()
		{
			post_msg_buffer.clear();
			send_msg_buffer.clear();
			m_lstRecvMsgBuffer.clear();
			m_lstTempMsgBuffer.clear();

			last_send_msg.clear();
			last_dispatch_msg.clear();
		}
	public:
		//please do not change SetID at runtime via the following function, except this st_socket is not managed by st_object_pool,
		//it should only be used by st_object_pool when this st_socket being reused or creating new st_socket.
		//id的设置与获取，注意使用者不可设置id，只有st_socket的创建者（st_object_pool或者其继承者）才可设置id，
		//除非这个st_socket没有被任何对象池管理。
		void SetID(uint_fast64_t id)
		{ 
			assert(!m_bIsStarted); 
			if (m_bIsStarted)
				assert(false);// unified_out::error_out("id is unchangeable!");
			else
				m_nID = id;
		}
		uint_fast64_t GetID() const 
		{
			return m_nID; 
		}
		//底层对象，它应该是一个boost::asio::ip::tcp::socket，boost::asio::ip::udp::socket或者
		//boost::asio::ssl::stream<boost::asio::ip::tcp::socket>对象及其从它们继承的对象。
		//最底层对象其实就是调用底层对象的lowest_layer()，我们真正要读写的其实就是最底层对象。
		Socket& GetNextLayer() 
		{ 
			return m_NextLayer; 
		}
		const Socket& GetNextLayer() const 
		{
			return m_NextLayer; 
		}
		typename Socket::lowest_layer_type& GetLowestLayer() 
		{ 
			return GetNextLayer().lowest_layer();
		}
		const typename Socket::lowest_layer_type& GetLowestLayer() const
		{ 
			return GetNextLayer().lowest_layer(); 
		}
		//判断本对象是否可以被重用或者被释放。
		virtual bool IsObsoleted()
		{
			if (IsStarted() || IsAsyncCalling())
				return false;

			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex, boost::try_to_lock);
			return lock.owns_lock() && m_lstRecvMsgBuffer.empty(); //if successfully locked, means this st_socket is idle
		}
		//是否已经开始，已经开始意思是已经调用过start()了，多次调用start()会有严重的包乱顺问题，好在我在某个版本
		//增加了防止多次调用start()的功能，之前靠用户保证，现在st_asio_wrapper库可以保证，即使用户多次调用也没问题。
		bool IsStarted() const
		{
			return m_bIsStarted;
		}
		//开始，开始的动作由子类实现，本函数只是简单的判断start是否已经被调用过了，如果没有，则调用do_start函数（纯虚）。
		void Start()
		{
			boost::unique_lock<boost::shared_mutex> lock(start_mutex);
			if (!m_bIsStarted)
				m_bIsStarted = DoStart();
		}
		//return false not means failure, but means already closed.
		bool close()
		{
			if (!GetLowestLayer().is_open())
				return false;

			boost::system::error_code ec;
			GetLowestLayer().close(ec);
			return true;
		}
		//发送缓存里面的消息，如果当前已经在发送消息了，则调用本函数无任何作用。
		//st_socket内部有消息发送缓存，当连接未建立的时候，用户仍然可以发送消息（注意缓存满问题），这些消息会缓存起来，
		//	当连接建立之后，会自动开始真正的发送消息，这个功能就是调用这个函数实现的。
		bool SendMsg() //return false if send buffer is empty or sending not allowed or io_service stopped
		{
			boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			return DoSendMsg();
		}
		//暂停／恢复消息发送，这里指暂停／恢复真正的消息发送，所以就算暂停了消息发送，在发送缓存可用的情况下，send_msg和send_safe_msg
		//仍然可以成功调用。
	    //注意，如果消息发送处于暂停状态，则safe_send_msg在发送缓存溢出的情况下，马上返回失败，而不是等待发送缓存直到可用为止。
		//post_msg不受这个属性的影响，所以post_msg一定只能在on_msg和on_msg_handle里面调用，再次强调。
		void SuspendSendMsg(bool bSuspend) 
		{ 
			if (!(m_bSuspendSendMsg = bSuspend)) 
				SendMsg(); 
		}
		bool SuspendSendMsg() const
		{ 
			return m_bSuspendSendMsg; 
		}
		//暂停／恢复消息派发，这里的消息派发，是指当st_socket收到消息的时候，调用on_msg或者on_msg_handle，这个功能有什么用呢？
		//当你在处理一个耗时消息时，不管你是在on_msg还是on_msg_handle里面处理，处理过程中，都将消耗掉一个service线程，那么必将对自己和
		//其它st_socket的数据收发造成一定的影响（当所有service线程耗尽时，就不是一定的影响了，而是所有的st_socket在这期间都将无法发送和
		//接收数据），为了解决这个问题，你可以开启一个线程来做这个耗时业务，但记得在启动线程之前，先暂停消息派发，线程结束之前，再恢复
		//消息派发，这样消息就不会乱序，否则由于你开启线程之后，马上退出了on_msg或者on_msg_handle，那么下一条消息（如果有的话）将马上被
		//派发，这样就出现了乱序问题（前一条消息还未处理完，后一条消息就被派发了）。
		void SuspendDispatchMsg(bool bSuspend)
		{
			m_bSuspendDispatchMsg = bSuspend;
			StopTimer(TIMER_SUSPEND_DISPATCH_MSG);
			do_dispatch_msg(true);
		}
		bool SuspendDispatchMsg() const
		{
			return m_bSuspendDispatchMsg;
		}
		boost::posix_time::time_duration recv_idle_time() const 
		{
			return m_tmRecvIdle; 
		}
		//get or change the packer at runtime
		//changing packer at runtime is not thread-safe, please pay special attention
		//we can resolve this defect via mutex, but i think it's not worth, because this feature is not frequently used
		boost::shared_ptr<IPacker<typename Packer::msg_type>> inner_packer()
		{
			return m_pPacker; 
		}
		boost::shared_ptr<const IPacker<typename Packer::msg_type>> inner_packer() const
		{ 
			return m_pPacker;
		}
		void inner_packer(const boost::shared_ptr<IPacker<typename Packer::msg_type>>& _packer_) 
		{ 
			m_pPacker = _packer_; 
		}
		//判断消息发送缓存是否可用，即里面的消息数量是否小于MAX_MSG_NUM条，如果以can_overflow为true调用任何消息发送函数（如send_msg），
		//将马上成功而无论消息发送缓存是否可用，所以可能会造成消息发送缓存大小不可控。
		//if you use can_overflow = true to invoke send_msg or send_native_msg, it will always succeed no matter whether the send buffer is available or not,
		//this can exhaust all virtual memory, please pay special attentions.
		bool IsSendBufferAvailable()
		{
			boost::shared_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			return send_msg_buffer.size() < ASIO_MAX_MSG_NUM;
		}
		//直接发送消息（放入消息发送缓存）而不再调用i_packer::pack_msg函数，其实st_socket内部在发送消息时也是调用这个函数，只是在调用
		//之前先调用了i_packer::pack_msg而已。
		//don't use the packer but insert into send buffer directly
		bool DirectSendMsg(const InMsgType& msg, bool can_overflow = false) 
		{
			return DirectSendMsg(InMsgType(msg), can_overflow);
		}

		bool DirectSendMsg(InMsgType&& msg, bool can_overflow = false)
		{
			boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			return can_overflow || send_msg_buffer.size() < ASIO_MAX_MSG_NUM ? do_direct_send_msg(std::move(msg)) : false;
		}

		bool DirectPostMsg(const InMsgType& msg, bool can_overflow = false)
		{ 
			return DirectPostMsg(InMsgType(msg), can_overflow);
		}

		bool DirectPostMsg(InMsgType&& msg, bool can_overflow = false)
		{
			if (DirectSendMsg(std::move(msg), can_overflow))
				return true;

			boost::unique_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
			return do_direct_post_msg(std::move(msg));
		}
//#define GET_PENDING_MSG_NUM(FUNNAME, CAN, MUTEX) size_t FUNNAME() {boost::shared_lock<boost::shared_mutex> lock(MUTEX); return CAN.size();}
		//获取缓存里面的消息数量，其中post和send缓存里面的消息是打包过的；recv缓存里面的消息是解包过后的，下同。
		size_t GetPendingPostMsgNum()
		{
			boost::shared_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
			return post_msg_buffer.size();
		}

		size_t GetPendingSendMsgNum()
		{
			boost::shared_lock<boost::shared_mutex> lock(send_msg_buffer_mutex); 
			return send_msg_buffer.size();
		}

		size_t GetPendingRecvMsgNum()
		{
			boost::shared_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
			return m_lstRecvMsgBuffer.size();
		} 
		//偷看一下缓存中第一个包，如果得到一个空包（msg.empty()等于true），则说明缓存里面没有消息。
		void PeekFirstPendingPostMsg(InMsgType& msg)
		{
			msg.clear();
			boost::shared_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
			if (!post_msg_buffer.empty())
				msg = post_msg_buffer.front();
		}

		void PeekFirstPendingSendMsg(InMsgType& msg)
		{
			msg.clear();
			boost::shared_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			if (!send_msg_buffer.empty())
				msg = send_msg_buffer.front();
		}

		void PeekFirstPendingRecvMsg(OutMsgType& msg)
		{
			msg.clear();
			boost::shared_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
			if (!m_lstRecvMsgBuffer.empty())
				msg = m_lstRecvMsgBuffer.front();
		}
		//弹出缓存中第一个包，如果得到一个空包（msg.empty()等于true），则说明缓存里面没有消息。
		void PopFirstPendingPostMsg(InMsgType& msg)
		{
			msg.clear();
			boost::unique_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
			if (!post_msg_buffer.empty())
			{
				msg.swap(post_msg_buffer.front());
				post_msg_buffer.pop_front();
			}
		}

		void PopFirstPendingSendMsg(InMsgType& msg)
		{
			msg.clear();
			boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			if (!send_msg_buffer.empty())
			{
				msg.swap(send_msg_buffer.front());
				send_msg_buffer.pop_front();
			}
		}

		void PopFirstPendingRecvMsg(OutMsgType& msg)
		{
			msg.clear();
			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
			if (!m_lstRecvMsgBuffer.empty())
			{
				msg.swap(m_lstRecvMsgBuffer.front());
				m_lstRecvMsgBuffer.pop_front();
			}
		}
		//弹出缓存中所有包，相当于清空了缓存。
		void PopAllPendingPostMsg(in_container_type & msg_list)
		{
			boost::unique_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
			msg_list.splice(msg_list.end(), post_msg_buffer);
		}

		void PopAllPendingSendMsg(in_container_type & msg_list)
		{
			boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			msg_list.splice(msg_list.end(), send_msg_buffer);
		}

		void PopAllPendingRecvMsg(out_container_type & msg_list)
		{
			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
			msg_list.splice(msg_list.end(), m_lstRecvMsgBuffer);
		}

	protected:
		//子类重写，请看st_server_socket、st_connector、st_udp_socket和st_ssl_connector的实现。
		virtual bool DoStart() = 0;
		//真正的消息发送（调用asio函数），具体怎么发请看st_tcp_socket和st_udp_socket的实现。
		virtual bool DoSendMsg() = 0; //must mutex send_msg_buffer before invoke this function

		virtual void DoRecvMsg() = 0;

		//can send msg or not(just put into send buffer)
		virtual bool is_send_allowed() const
		{ 
			return !m_bSuspendSendMsg; 
		} 
		// 发送接收失败时回调，对于st_tcp_socket，如果需要连接断开事件，建议重写on_recv_error。
		//generally, you don't have to rewrite this to maintain the status of connections(TCP)
		virtual void OnSendError(const boost::system::error_code& ec) 
		{
			std::ignore = ec;
			//unified_out::error_out("send msg error (%d %s)", ec.value(), ec.message().data());
		}
		//receiving error or peer endpoint quit(false ec means ok)
		virtual void OnRecvError(const boost::system::error_code& ec) = 0;

#ifndef ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
		//if you want to use your own receive buffer, you can move the msg to your own receive buffer, then handle them as your own strategy(may be you'll need a msg dispatch thread),
		//or you can handle the msg at here, but this will reduce efficiency because this msg handling will block the next msg receiving on the same st_socket,
		//but if you can handle the msg very fast, you are recommended to handle them at here, which will inversely more efficient,
		//because msg receive buffer and msg dispatching are not needed any more.
		//
		//return true means msg been handled, st_socket will not maintain it anymore, return false means msg cannot be handled right now, you must handle it in on_msg_handle()
		//notice: on_msg_handle() will not be invoked from within this function
		//
		//notice: the msg is unpacked, using inconstant is for the convenience of swapping
		virtual bool OnMsg(OutMsgType& msg) = 0;
#endif

		//handling msg in om_msg_handle() will not block msg receiving on the same st_socket
		//return true means msg been handled, false means msg cannot be handled right now, and st_socket will re-dispatch it asynchronously
		//if link_down is true, no matter return true or false, st_socket will not maintain this msg anymore, and continue dispatch the next msg continuously
		//
		//notice: the msg is unpacked, using inconstant is for the convenience of swapping
		virtual bool OnMsgHandle(OutMsgType& msg, bool link_down) = 0;

#ifdef ASIO_WANT_MSG_SEND_NOTIFY
		//one msg has sent to the kernel buffer, msg is the right msg
		//notice: the msg is packed, using inconstant is for the convenience of swapping
		virtual void OnMsgSend(InMsgType& msg) { std::ignore = msg; }
#endif
#ifdef ASIO_WANT_ALL_MSG_SEND_NOTIFY
		//send buffer goes empty
		//notice: the msg is packed, using inconstant is for the convenience of swapping
		virtual void on_all_msg_send(InMsgType& msg) {}
#endif
		//派发消息，它要么直接调用on_msg，要么把消息放入消息接收缓存，最后调用do_dispatch_msg，如果消息处理完毕（调用on_msg）
		//或者都放入了消息接收缓存，则调用do_start以继续接收数据。
		void dispatch_msg()
		{
#ifndef ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
			auto bDispatch = false;
			for (auto iter = std::begin(m_lstTempMsgBuffer); !m_bSuspendDispatchMsg && !m_bIsPosting && iter != std::end(m_lstTempMsgBuffer);)
			{
				if (OnMsg(*iter))
					m_lstTempMsgBuffer.erase(iter++);
				else
				{
					boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
					if (m_lstRecvMsgBuffer.size() < ASIO_MAX_MSG_NUM) //msg receive buffer available
					{
						bDispatch = true;
						m_lstRecvMsgBuffer.splice(std::end(m_lstRecvMsgBuffer), m_lstTempMsgBuffer, iter++);
					}
					else
						++iter;
				}
			}

			if (bDispatch)
				do_dispatch_msg(true);
#else
			if (!m_lstTempMsgBuffer.empty())
			{
				boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
				if (splice_helper(m_lstRecvMsgBuffer, m_lstTempMsgBuffer))
					do_dispatch_msg(false);
			}
#endif

			if (m_lstTempMsgBuffer.empty())
				DoRecvMsg(); //receive msg sequentially, which means second receiving only after first receiving success
			else
				SetTimer(TIMER_DISPATCH_MSG, 50, [this](unsigned int id)->bool {return  TimerHandler(id); });
		}

		//must mutex m_lstRecvMsgBuffer before invoke this function
		void do_dispatch_msg(bool bNeedLock)
		{
			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex, boost::defer_lock);
			if (bNeedLock) 
				lock.lock();

			if (m_bSuspendDispatchMsg)
			{
				if (!m_bIsDispatching && !m_lstRecvMsgBuffer.empty())
					SetTimer(TIMER_SUSPEND_DISPATCH_MSG, 24 * 60 * 60 * 1000, [this](unsigned int id)->bool {return  TimerHandler(id); }); //one day
			}
			else if (!m_bIsPosting)
			{
			//	auto& io_svc = GetIoService();
				auto dispatch_all = false;
				if (IsStopped())
					dispatch_all = !(m_bIsDispatching = false);
				else if (!m_bIsDispatching)
				{
					if (! IsStarted()  )
						dispatch_all = true;
					else if (!m_lstRecvMsgBuffer.empty())
					{
						m_bIsDispatching = true;
						last_dispatch_msg.swap(m_lstRecvMsgBuffer.front());
						post([this]() { MsgHandler(); });   //io_svc.post([this]() { MsgHandler(); });
						m_lstRecvMsgBuffer.pop_front();
					}
				}

				if (dispatch_all)
				{
#ifdef ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
					//the msgs in m_lstTempMsgBuffer are discarded if we don't used msg receive buffer, it's very hard to resolve this defect,
					//so, please be very carefully if you decide to resolve this issue, the biggest problem is calling force_close in OnMsg.
					m_lstRecvMsgBuffer.splice(std::end(m_lstRecvMsgBuffer), m_lstTempMsgBuffer);
#endif
#ifndef ST_ASIO_DISCARD_MSG_WHEN_LINK_DOWN
					ASIO::do_something_to_all(m_lstRecvMsgBuffer, [this](OutMsgType& msg) { OnMsgHandle(msg, true); });
#endif
					m_lstRecvMsgBuffer.clear();
				}
			}
		}

		//must mutex send_msg_buffer before invoke this function
		bool do_direct_send_msg(InMsgType&& msg)
		{
			if (!msg.empty())
			{
				send_msg_buffer.resize(send_msg_buffer.size() + 1);
				send_msg_buffer.back().swap(msg);
				DoSendMsg();
			}

			return true;
		}

		//must mutex post_msg_buffer before invoke this function
		bool do_direct_post_msg(InMsgType&& msg)
		{
			if (!msg.empty())
			{
				post_msg_buffer.resize(post_msg_buffer.size() + 1);
				post_msg_buffer.back().swap(msg);
				if (!m_bIsPosting)
				{
					m_bIsPosting = true;
					SetTimer(TIMER_HANDLE_POST_BUFFER, 50, [this](unsigned int id)->bool {return  TimerHandler(id); });
				}
			}
			return true;
		}

	private:
		bool TimerHandler(unsigned int id)
		{
			switch (id)
			{
			case TIMER_DISPATCH_MSG: //delay putting msgs into receive buffer cause of receive buffer overflow
				m_tmRecvIdle += boost::posix_time::milliseconds(50);
				dispatch_msg();
				break;
			case TIMER_SUSPEND_DISPATCH_MSG: //suspend dispatching msgs
				do_dispatch_msg(true);
				break;
			case TIMER_HANDLE_POST_BUFFER:
			{
				boost::unique_lock<boost::shared_mutex> lock(post_msg_buffer_mutex);
				{
					boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
					if (splice_helper(send_msg_buffer, post_msg_buffer))
						DoSendMsg();
				}

				bool bEmpty = post_msg_buffer.empty();
				m_bIsPosting = !bEmpty;
				lock.unlock();

				if (bEmpty)
					do_dispatch_msg(true);

				return !bEmpty; //continue the timer if some msgs still left behind
			}
			break;
			case TIMER_RE_DISPATCH_MSG: //re-dispatch
				m_tmRecvIdle += boost::posix_time::milliseconds(50);
				do_dispatch_msg(true);
				break;
			default:
				assert(false);
				break;
			}

			return false;
		}

		void MsgHandler()
		{
			bool re = OnMsgHandle(last_dispatch_msg, false); //must before next msg dispatching to keep sequence
			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex);
			m_bIsDispatching = false;
			if (!re) //dispatch failed, re-dispatch
			{
				m_lstRecvMsgBuffer.push_front(OutMsgType());
				m_lstRecvMsgBuffer.front().swap(last_dispatch_msg);
				SetTimer(TIMER_RE_DISPATCH_MSG, 50, [this](unsigned int id)->bool {return TimerHandler(id); });
			}
			else //dispatch msg sequentially, which means second dispatching only after first dispatching success
				do_dispatch_msg(false);

			if (!m_bIsDispatching)
				last_dispatch_msg.clear();
		}

	protected:
		uint_fast64_t m_nID;
		Socket m_NextLayer;
		boost::shared_mutex close_mutex;

		InMsgType last_send_msg;
		OutMsgType last_dispatch_msg;
		boost::shared_ptr<IPacker<typename Packer::msg_type>> m_pPacker;

		in_container_type post_msg_buffer, send_msg_buffer;
		out_container_type m_lstRecvMsgBuffer, m_lstTempMsgBuffer;
		//st_socket will invoke dispatch_msg() when got some msgs. if these msgs can't push into m_lstRecvMsgBuffer cause of receive buffer overflow,
		//st_socket will delay 50 milliseconds(non-blocking) to invoke dispatch_msg() again, and now, as you known, m_lstTempMsgBuffer is used to hold these msgs temporarily.
		boost::shared_mutex post_msg_buffer_mutex, send_msg_buffer_mutex;
		boost::shared_mutex recv_msg_buffer_mutex;

		bool m_bIsPosting;
		bool m_bIsSending;
		bool m_bSuspendSendMsg;
		bool m_bIsDispatching;
		bool m_bSuspendDispatchMsg;

		bool m_bIsStarted; //has IsStarted or not
		boost::shared_mutex start_mutex;
		
		//during this duration, st_socket suspended msg reception because of receiving buffer was full.
		boost::posix_time::time_duration m_tmRecvIdle;
	};

} //namespace

#endif /* ASIO_SOCKET_H_ */
