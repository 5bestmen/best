
#ifndef ASIO_TCP_SOCKET_H_
#define ASIO_TCP_SOCKET_H_

#include "asio_socket.h"
#include "asio_unpacker.h"

#ifndef ASIO_GRACEFUL_CLOSE_MAX_DURATION
#define ASIO_GRACEFUL_CLOSE_MAX_DURATION	5 //seconds, maximum waiting seconds while graceful closing
#endif

#ifndef ASIO_DEFAULT_UNPACKER
#define ASIO_DEFAULT_UNPACKER CUnpacker
#endif

namespace ASIO
{
	namespace ASIO_TCP
	{
		// tcp�׽����࣬ʵ��tcp���ݵ��շ�
		template <typename Socket, typename Packer, typename Unpacker>
		class CTcpSocketBase : public CAsioSocket<Socket, Packer, Unpacker>
		{
		public:
			typedef typename Packer::msg_type in_msg_type;
			typedef typename Packer::msg_ctype in_msg_ctype;
			using out_msg_type = typename Unpacker::msg_type;
			typedef typename Unpacker::msg_ctype out_msg_ctype;

		protected:
			using CAsioSocket<Socket, Packer, Unpacker>::TIMER_BEGIN;
			using CAsioSocket<Socket, Packer, Unpacker>::TIMER_END;

			CTcpSocketBase(boost::asio::io_service& io_svc) : CAsioSocket<Socket, Packer, Unpacker>(io_svc), m_Unpacker(boost::make_shared<Unpacker>()), m_nCloseState(0)
			{				 
			}

			template<typename Arg>
			CTcpSocketBase(boost::asio::io_service& io_svc, Arg& arg) : CAsioSocket<Socket, Packer, Unpacker>(io_svc, arg), m_Unpacker(boost::make_shared<Unpacker>()), m_nCloseState(0)
			{
			}

		public:
			// �ڵ��ø���ͬ�������Ļ����ϣ������˶��Ƿ����ڹر����ӵ��жϣ�����ǣ��򷵻ؼ١�
			virtual bool obsoleted()
			{
				return !IsClosing() && CAsioSocket<Socket, Packer, Unpacker>::IsObsoleted();
			}

			//Reset all, be ensure that there's no any operations performed on this st_tcp_socket_base when invoke it
			void Reset()
			{
				reset_state();
				m_nCloseState = 0;
				CAsioSocket<Socket, Packer, Unpacker>::Reset();
			}
			void reset_state()
			{
				m_Unpacker->reset_state();
				CAsioSocket<Socket, Packer, Unpacker>::ResetState();
			}
			// �Ƿ��������Źر��׽��֣���ʱ���ٷ�����Ϣ��������ͻ������Ϊ�Ѿ��ر����Լ������ݷ��ͣ���������������Ϣ��
			bool IsClosing() const
			{
				return 0 != m_nCloseState;
			}
			//��ȡ���޸Ľ������
			//ע�⣬����ʱ�޸Ľ�����Ƿ��̰߳�ȫ�ģ�����ֻ���ڹ��캯���������reset����������Ǹ�����on_msg�����޸ġ���֧�ֶ��߳�һ��Ϊ��
			//Ч�ʣ�����֧����Ҳ����Ҫ��ǰ��˵���������ط��޸ģ����������ط������ж��߳����⣬������������õú��١�
			//get or change the CUnpacker at runtime
			//changing CUnpacker at runtime is not thread-safe, this operation can only be done in OnMsg(), Reset() or constructor, please pay special attention
			//we can resolve this defect via mutex, but i think it's not worth, because this feature is not frequently used
			boost::shared_ptr<IUnpacker<out_msg_type>> inner_unpacker()
			{
				return m_Unpacker;
			}
			boost::shared_ptr<const IUnpacker<out_msg_type>> inner_unpacker() const
			{
				return m_Unpacker;
			}
			void inner_unpacker(const boost::shared_ptr<IUnpacker<out_msg_type>>& _unpacker_)
			{
				m_Unpacker = _unpacker_;
			}

			using CAsioSocket<Socket, Packer, Unpacker>::SendMsg;

			//������Ϣ����������helper���������һ�����������ķ�����Ϣ��������Ϣ���ͻ��棩����һ�����õڶ������ڶ������õ�������
			bool send_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
			{
				boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
				return (can_overflow || send_msg_buffer.size() < ASIO_MAX_MSG_NUM) ? do_direct_send_msg(m_pPacker->pack_msg(pstr, len, num, false)) : false;
			}
			bool send_msg(const char* pstr, size_t len, bool can_overflow = false)
			{
				return send_msg(&pstr, &len, 1, can_overflow);
			}
			bool send_msg(const std::string& str, bool can_overflow = false)
			{
				return send_msg(str.data(), str.size(), can_overflow);
			}
			//ͬ�ϣ�ֻ����nativeΪtrue����i_packer::pack_msg�ӿڡ�
			bool send_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
			{
				boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
				return (can_overflow || send_msg_buffer.size() < ASIO_MAX_MSG_NUM) ? do_direct_send_msg(m_pPacker->pack_msg(pstr, len, num, true)) : false;
			}
			bool send_native_msg(const char* pstr, size_t len, bool can_overflow = false)
			{
				return send_native_msg(&pstr, &len, 1, can_overflow);
			}
			bool send_native_msg(const std::string& str, bool can_overflow = false)
			{
				return send_native_msg(str.data(), str.size(), can_overflow);
			}

			//ͬsend_msg��ֻ������Ϣ���ͻ��������ʱ���ȴ�ֱ��������ã����is_send_allowed����false����io_service�Ѿ�ֹͣ�������Ϸ����ȴ�����ʧ�ܡ�
			//safeϵ�к�������on_msg��om_msg_handle�������ʱ��Ҫ�ر��������Ϊ��������service�̡߳�
			bool safe_send_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
			{
				while (!send_msg(pstr, len, num, can_overflow))
				{
					if (!is_send_allowed() || GetIoService().stopped())
						return false;
					boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(50));
				}
				return true;
			}

			bool safe_send_msg(const char* pstr, size_t len, bool can_overflow = false)
			{
				return safe_send_msg(&pstr, &len, 1, can_overflow);
			}
			bool safe_send_msg(const std::string& str, bool can_overflow = false)
			{
				return safe_send_msg(str.data(), str.size(), can_overflow);
			}
			//ͬsend_msg��ֻ������Զ�����ϳɹ���
			//	ע�⣺�����Ϣ���ͻ�������ˣ���Ȼcan_overflow��Ϊfalse�����򲻼�⻺���Ƿ������������ͣ��Ϣ�ɷ�������Ҳ��ֹͣ��Ϣ���գ�
			//	��Ϊ���ɷ�ֻ������Ϣ�����������Ϣ���ջ���������ͣ��Ϣ���գ������ʹ����Ϣ���ջ��棬��������ͣ��Ϣ���գ���
			//	post_msg��send_msg�������뿴st_socket�����post_msg_buffer��send_msg_buffer��
			bool post_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
			{
				return  DirectPostMsg(m_pPacker->pack_msg(pstr, len, num, false), can_overflow);
			}
			bool post_msg(const char* pstr, size_t len, bool can_overflow = false)
			{
				return post_msg(&pstr, &len, 1, can_overflow);
			}
			bool post_msg(const std::string& str, bool can_overflow = false)
			{
				return post_msg(str.data(), str.size(), can_overflow);
			}
			
			bool post_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
			{
				return  DirectPostMsg(m_pPacker->pack_msg(pstr, len, num, true), can_overflow);
			}
			bool post_native_msg(const char* pstr, size_t len, bool can_overflow = false)
			{
				return post_native_msg(&pstr, &len, 1, can_overflow);
			}
			bool post_native_msg(const std::string& str, bool can_overflow = false)
			{
				return post_native_msg(str.data(), str.size(), can_overflow);
			}
			///////////////////////////////////////////////////
			//msg sending interface
			//TCP_SEND_MSG(send_msg, false) //use the packer with native = false to pack the msgs
			//	TCP_SEND_MSG(send_native_msg, true) //use the packer with native = true to pack the msgs
			//guarantee send msg successfully even if can_overflow equal to false
			//success at here just means put the msg into st_tcp_socket_base's send buffer
			//	TCP_SAFE_SEND_MSG(safe_send_msg, send_msg)
			//	TCP_SAFE_SEND_MSG(safe_send_native_msg, send_native_msg)
			//like safe_send_msg and safe_send_native_msg, but non-block
			//	TCP_POST_MSG(post_msg, false)
			//	TCP_POST_MSG(post_native_msg, true)
			//msg sending interface
			///////////////////////////////////////////////////

		protected:
			void force_close()
			{
				if (1 != m_nCloseState)
					do_close();
			}
			bool graceful_close(bool bSync = true) //will block until closing success or time out if bSync equal to true
			{
				if (IsClosing())
					return false;
				else
					m_nCloseState = 2;

				boost::system::error_code ec;
				GetLowestLayer().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
				if (ec) //graceful closing is impossible
				{
					do_close();
					return false;
				}

				if (bSync)
				{
					auto loop_num = ASIO_GRACEFUL_CLOSE_MAX_DURATION * 100; //seconds to 10 milliseconds
					while (--loop_num >= 0 && IsClosing())
						boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(10));
					if (loop_num < 0) //graceful closing is impossible
					{
						//	unified_out::info_out("failed to graceful close within %d seconds", ASIO_GRACEFUL_CLOSE_MAX_DURATION);
						do_close();
					}
				}

				return !bSync;
			}

			//must mutex send_msg_buffer before invoke this function
			virtual bool DoSendMsg()
			{
				if (!is_send_allowed() || IsStopped())
					m_bIsSending = false;
				else if (!m_bIsSending && !send_msg_buffer.empty())
				{
					m_bIsSending = true;
					last_send_msg.swap(send_msg_buffer.front());
					send_msg_buffer.pop_front();
// 					boost::asio::async_write(GetNextLayer(), boost::asio::buffer(last_send_msg.data(), last_send_msg.size()),
// 						boost::bind(&CTcpSocketBase::SendHandler, this, 
// 						boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
					boost::asio::async_write(GetNextLayer(), boost::asio::buffer(last_send_msg.data(), last_send_msg.size()),
						make_handler_error_size(boost::bind(&CTcpSocketBase::SendHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
				}

				return  m_bIsSending;
			}
			//start the asynchronous read
			//it's child's responsibility to invoke this properly, because tcp_socket_base doesn't know any of the connection status
			void DoRecvMsg()
			{
				auto recv_buff = m_Unpacker->prepare_next_recv();
				assert(boost::asio::buffer_size(recv_buff) > 0);

				boost::asio::async_read(GetNextLayer(), recv_buff,
					boost::bind(&IUnpacker<out_msg_type>::completion_condition, m_Unpacker, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
					make_handler_error_size(boost::bind(&CTcpSocketBase::RecvHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));

// 					boost::asio::async_read(GetNextLayer(), recv_buff,
// 					boost::bind(&IUnpacker<out_msg_type>::completion_condition, m_Unpacker, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred),
// 					boost::bind(&CTcpSocketBase::RecvHandler, this,
// 					boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			
			}
			virtual bool is_send_allowed() const
			{
				return !IsClosing() && CAsioSocket<Socket, Packer, Unpacker>::is_send_allowed();
			}
			//can send data or not(just put into send buffer)

			//msg can not be unpacked
			//the link can continue to use, so don't need to close the st_tcp_socket_base at both client and server endpoint
			virtual void OnUnpackError() = 0;

#ifndef ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
			virtual bool OnMsg(out_msg_type& msg)
			{
				std::ignore = msg;
				//	unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
				return true;
			}
#endif

			virtual bool OnMsgHandle(out_msg_type& msg, bool link_down)
			{
				std::ignore = msg;
				std::ignore = link_down;
				//unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
				return true;
			}

			void do_close()
			{
				m_nCloseState = 1;
				StopAllTimer();
				m_bIsStarted = false;
			//	reset_state();

				if (GetLowestLayer().is_open())
				{
					boost::system::error_code ec;
					GetLowestLayer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
				//	GetLowestLayer().close(ec);
				}
			}

		private:
			void RecvHandler(const boost::system::error_code& ec, size_t bytes_transferred)
			{
				if (!ec && bytes_transferred > 0)
				{
					bool bUnpackOk = m_Unpacker->parse_msg(bytes_transferred, m_lstTempMsgBuffer);
					dispatch_msg();

					if (!bUnpackOk)
					{
						OnUnpackError();
						//Reset CUnpacker's state after on_unpack_error(), so user can get the left half-baked msg in on_unpack_error()
						m_Unpacker->reset_state();
					}
				}
				else
					OnRecvError(ec);
			}
			//�ɹ�������Ϣ��д��ײ��׽��֣�����asio�ص���
			void SendHandler(const boost::system::error_code& ec, size_t bytes_transferred)
			{
				if (!ec)
				{
					assert(bytes_transferred > 0);
#ifdef ASIO_WANT_MSG_SEND_NOTIFY
					OnMsgSend( last_send_msg);
#endif
				}
				else
					OnSendError(ec);

				boost::unique_lock<boost::shared_mutex> lock(send_msg_buffer_mutex);
				m_bIsSending = false;

				//send msg sequentially, that means second send only after first send success
				if (!ec)
#ifdef ASIO_WANT_ALL_MSG_SEND_NOTIFY
					if (!DoSendMsg())
						on_all_msg_send( last_send_msg);
#else
					DoSendMsg();
#endif

				if (!m_bIsSending)
					last_send_msg.clear();
			}

		protected:
			boost::shared_ptr<IUnpacker<out_msg_type>> m_Unpacker;
			int m_nCloseState; //2-the first step of graceful close, 1-force close, 0-normal state
		};

	} //namespace tcp
} //namespace asio_wrapper

//using namespace st_asio_wrapper::st_tcp; //compatible with old version which doesn't have st_tcp namespace.

#endif /* ASIO_WRAPPER_TCP_SOCKET_H_ */
