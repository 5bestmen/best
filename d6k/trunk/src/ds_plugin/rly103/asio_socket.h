
#ifndef ASIO_SOCKET_H_
#define ASIO_SOCKET_H_

#include <boost/container/list.hpp>
 

#include "asio_packer.h"
#include "asio_timer.h"

//Ĭ�ϵĴ������Ҳ����ͨ��ģ��������ƣ��ṩ����������ʹ����ʡ����Ӧ��ģ�������
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
		//��������״̬��
		void ResetState()
		{
			m_bIsPosting = false;
			m_bIsSending = m_bSuspendSendMsg = false;
			m_bIsDispatching = m_bSuspendDispatchMsg = false;
		//	m_bIsStarted = false;
		//	m_tmRecvIdle = boost::posix_time::time_duration();
		}
		//�������buffer��
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
		//id���������ȡ��ע��ʹ���߲�������id��ֻ��st_socket�Ĵ����ߣ�st_object_pool������̳��ߣ��ſ�����id��
		//�������st_socketû�б��κζ���ع���
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
		//�ײ������Ӧ����һ��boost::asio::ip::tcp::socket��boost::asio::ip::udp::socket����
		//boost::asio::ssl::stream<boost::asio::ip::tcp::socket>����������Ǽ̳еĶ���
		//��ײ������ʵ���ǵ��õײ�����lowest_layer()����������Ҫ��д����ʵ������ײ����
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
		//�жϱ������Ƿ���Ա����û��߱��ͷš�
		virtual bool IsObsoleted()
		{
			if (IsStarted() || IsAsyncCalling())
				return false;

			boost::unique_lock<boost::shared_mutex> lock(recv_msg_buffer_mutex, boost::try_to_lock);
			return lock.owns_lock() && m_lstRecvMsgBuffer.empty(); //if successfully locked, means this st_socket is idle
		}
		//�Ƿ��Ѿ���ʼ���Ѿ���ʼ��˼���Ѿ����ù�start()�ˣ���ε���start()�������صİ���˳���⣬��������ĳ���汾
		//�����˷�ֹ��ε���start()�Ĺ��ܣ�֮ǰ���û���֤������st_asio_wrapper����Ա�֤����ʹ�û���ε���Ҳû���⡣
		bool IsStarted() const
		{
			return m_bIsStarted;
		}
		//��ʼ����ʼ�Ķ���������ʵ�֣�������ֻ�Ǽ򵥵��ж�start�Ƿ��Ѿ������ù��ˣ����û�У������do_start���������飩��
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
		//���ͻ����������Ϣ�������ǰ�Ѿ��ڷ�����Ϣ�ˣ�����ñ��������κ����á�
		//st_socket�ڲ�����Ϣ���ͻ��棬������δ������ʱ���û���Ȼ���Է�����Ϣ��ע�⻺�������⣩����Щ��Ϣ�Ỻ��������
		//	�����ӽ���֮�󣬻��Զ���ʼ�����ķ�����Ϣ��������ܾ��ǵ����������ʵ�ֵġ�
		bool SendMsg() //return false if send buffer is empty or sending not allowed or io_service stopped
		{
			boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			return DoSendMsg();
		}
		//��ͣ���ָ���Ϣ���ͣ�����ָ��ͣ���ָ���������Ϣ���ͣ����Ծ�����ͣ����Ϣ���ͣ��ڷ��ͻ�����õ�����£�send_msg��send_safe_msg
		//��Ȼ���Գɹ����á�
	    //ע�⣬�����Ϣ���ʹ�����ͣ״̬����safe_send_msg�ڷ��ͻ������������£����Ϸ���ʧ�ܣ������ǵȴ����ͻ���ֱ������Ϊֹ��
		//post_msg����������Ե�Ӱ�죬����post_msgһ��ֻ����on_msg��on_msg_handle������ã��ٴ�ǿ����
		void SuspendSendMsg(bool bSuspend) 
		{ 
			if (!(m_bSuspendSendMsg = bSuspend)) 
				SendMsg(); 
		}
		bool SuspendSendMsg() const
		{ 
			return m_bSuspendSendMsg; 
		}
		//��ͣ���ָ���Ϣ�ɷ����������Ϣ�ɷ�����ָ��st_socket�յ���Ϣ��ʱ�򣬵���on_msg����on_msg_handle�����������ʲô���أ�
		//�����ڴ���һ����ʱ��Ϣʱ������������on_msg����on_msg_handle���洦����������У��������ĵ�һ��service�̣߳���ô�ؽ����Լ���
		//����st_socket�������շ����һ����Ӱ�죨������service�̺߳ľ�ʱ���Ͳ���һ����Ӱ���ˣ��������е�st_socket�����ڼ䶼���޷����ͺ�
		//�������ݣ���Ϊ�˽��������⣬����Կ���һ���߳����������ʱҵ�񣬵��ǵ��������߳�֮ǰ������ͣ��Ϣ�ɷ����߳̽���֮ǰ���ٻָ�
		//��Ϣ�ɷ���������Ϣ�Ͳ������򣬷��������㿪���߳�֮�������˳���on_msg����on_msg_handle����ô��һ����Ϣ������еĻ��������ϱ�
		//�ɷ��������ͳ������������⣨ǰһ����Ϣ��δ�����꣬��һ����Ϣ�ͱ��ɷ��ˣ���
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
		//�ж���Ϣ���ͻ����Ƿ���ã����������Ϣ�����Ƿ�С��MAX_MSG_NUM���������can_overflowΪtrue�����κ���Ϣ���ͺ�������send_msg����
		//�����ϳɹ���������Ϣ���ͻ����Ƿ���ã����Կ��ܻ������Ϣ���ͻ����С���ɿء�
		//if you use can_overflow = true to invoke send_msg or send_native_msg, it will always succeed no matter whether the send buffer is available or not,
		//this can exhaust all virtual memory, please pay special attentions.
		bool IsSendBufferAvailable()
		{
			boost::shared_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
			return send_msg_buffer.size() < ASIO_MAX_MSG_NUM;
		}
		//ֱ�ӷ�����Ϣ��������Ϣ���ͻ��棩�����ٵ���i_packer::pack_msg��������ʵst_socket�ڲ��ڷ�����ϢʱҲ�ǵ������������ֻ���ڵ���
		//֮ǰ�ȵ�����i_packer::pack_msg���ѡ�
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
		//��ȡ�����������Ϣ����������post��send�����������Ϣ�Ǵ�����ģ�recv�����������Ϣ�ǽ������ģ���ͬ��
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
		//͵��һ�»����е�һ����������õ�һ���հ���msg.empty()����true������˵����������û����Ϣ��
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
		//���������е�һ����������õ�һ���հ���msg.empty()����true������˵����������û����Ϣ��
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
		//�������������а����൱������˻��档
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
		//������д���뿴st_server_socket��st_connector��st_udp_socket��st_ssl_connector��ʵ�֡�
		virtual bool DoStart() = 0;
		//��������Ϣ���ͣ�����asio��������������ô���뿴st_tcp_socket��st_udp_socket��ʵ�֡�
		virtual bool DoSendMsg() = 0; //must mutex send_msg_buffer before invoke this function

		virtual void DoRecvMsg() = 0;

		//can send msg or not(just put into send buffer)
		virtual bool is_send_allowed() const
		{ 
			return !m_bSuspendSendMsg; 
		} 
		// ���ͽ���ʧ��ʱ�ص�������st_tcp_socket�������Ҫ���ӶϿ��¼���������дon_recv_error��
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
		//�ɷ���Ϣ����Ҫôֱ�ӵ���on_msg��Ҫô����Ϣ������Ϣ���ջ��棬������do_dispatch_msg�������Ϣ������ϣ�����on_msg��
		//���߶���������Ϣ���ջ��棬�����do_start�Լ����������ݡ�
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
