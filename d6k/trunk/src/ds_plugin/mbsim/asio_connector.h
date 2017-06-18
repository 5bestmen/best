

#ifndef ASIO_CONNECTOR_H_
#define ASIO_CONNECTOR_H_

#include "asio_tcp_socket.h"

#ifndef ST_ASIO_SERVER_IP
#define ASIO_SERVER_IP			"127.0.0.1"
#endif
#ifndef ASIO_SERVER_PORT
#define ASIO_SERVER_PORT			5050
#endif

// �첽���ӣ�asio����ʧ��֮����ͣһ��ʱ��������ԣ����ٴε���asnc_connect����
#ifndef ASIO_RECONNECT_INTERVAL
#define ASIO_RECONNECT_INTERVAL	500 //millisecond(s)
#endif

namespace ASIO
{
	// 	�����ӹ��ܵ� tcp_socket������һ�������Ŀͻ�����
	using namespace ASIO::ASIO_TCP;
	template <typename Packer = ASIO_DEFAULT_PACKER, typename Unpacker = ASIO_DEFAULT_UNPACKER, typename Socket = boost::asio::ip::tcp::socket>
	class CAsioConnectorBase : public CTcpSocketBase<Socket, Packer, Unpacker>
	{
	public:
		static const unsigned int TIMER_BEGIN = CTcpSocketBase<Socket, Packer, Unpacker>::TIMER_END;
		static const unsigned int TIMER_CONNECT = TIMER_BEGIN;
		static const unsigned int TIMER_ASYNC_CLOSE = TIMER_BEGIN + 1;
		static const unsigned int TIMER_END = TIMER_BEGIN + 10;

		CAsioConnectorBase(boost::asio::io_service& io_svc) : CTcpSocketBase<Socket, Packer, Unpacker>(io_svc), m_bIsConnected(false), m_bIsReConnecting(true)
		{
			set_server_addr(ASIO_SERVER_PORT, ASIO_SERVER_IP);
		}

		template<typename Arg>
		CAsioConnectorBase(boost::asio::io_service& io_svc, Arg& arg) : CTcpSocketBase<Socket, Packer, Unpacker>(io_svc, arg), m_bIsConnected(false), m_bIsReConnecting(true)
		{
			set_server_addr(ASIO_SERVER_PORT, ASIO_SERVER_IP);
		}
	public:
		//reset all, be ensure that there's no any operations performed on this st_connector_base when invoke it
		//notice, when reusing this st_connector_base, st_object_pool will invoke reset(), child must re-write this to initialize
		//all member variables, and then do not forget to invoke st_connector_base::reset() to initialize father's member variables
		// �������У�st_object_pool������ʱ����á�st_connector���������д���������Լ���״̬���ǵ������Ҫ���ñ����reset��
		virtual void Reset()
		{
			m_bIsConnected = false;
			m_bIsReConnecting = true;
			CTcpSocketBase<Socket, Packer, Unpacker>::Reset();
		//	m_nCloseState = 0;
		}
		// �ڵ��ø���ͬ�������Ļ����ϣ������˶��Ƿ������ӵ��жϣ������Ҫ�������򷵻ؼ١�
		virtual bool obsoleted()
		{
			return !m_bIsReConnecting && CTcpSocketBase<Socket, Packer, Unpacker>::obsoleted();
		}
		// ���÷���˵�ַ��������֮�ã���Ҫ��do_start֮ǰ�����á�
		bool set_server_addr(unsigned short port, const std::string& ip = ASIO_SERVER_IP)
		{
			boost::system::error_code ec;
			auto addr = boost::asio::ip::address::from_string(ip, ec);
			if (ec)
				return false;

			m_ServerAddr = boost::asio::ip::tcp::endpoint(addr, port);
			return true;
		}
		const boost::asio::ip::tcp::endpoint& get_server_addr() const
		{
			return m_ServerAddr;
		}
		// �Ƿ��Ѿ����ӳɹ���
		bool IsConnected() const
		{
			return m_bIsConnected;
		}
		// ֱ�ӵ���force_close��
		//if the connection is broken unexpectedly, st_connector will try to reconnect to the server automatically.
		void disconnect(bool bReconnect = false)
		{
			force_close(bReconnect);
		}

		void force_close(bool reconnect = false)
		{
			if (1 != m_nCloseState)
			{
				show_info("client link:", "been closed.");
				m_bIsReConnecting = reconnect;
				m_bIsConnected = false;
			}

			CTcpSocketBase<Socket, Packer, Unpacker>::force_close();
		}

		void graceful_close(bool reconnect = false, bool sync = true)
		{
			if (IsClosing())
				return;
			else if (!IsConnected())
				return force_close(reconnect);

			//	show_info("client link:", "been closing gracefully.");
			m_bIsReConnecting = reconnect;
			m_bIsConnected = false;

			if (CTcpSocketBase<Socket, Packer, Unpacker>::graceful_close(sync))
				SetTimer(TIMER_ASYNC_CLOSE, 10, boost::bind(&CAsioConnectorBase::AsyncCloseHandler, this, _1, ASIO_GRACEFUL_CLOSE_MAX_DURATION * 100));
		}

		void show_info(const char* head, const char* tail) const
		{
			std::ignore = tail;
			std::ignore = head;
			boost::system::error_code ec;
			auto ep = GetLowestLayer().local_endpoint(ec);
			//	if (!ec)
			//		unified_out::info_out("%s %s:%hu %s", head, ep.address().to_string().c_str(), ep.port(), tail);
		}

		void show_info(const char* head, const char* tail, const boost::system::error_code& ec) const
		{
			boost::system::error_code ec2;
			auto ep = GetLowestLayer().local_endpoint(ec2);
			//	if (!ec2)
			//		unified_out::info_out("%s %s:%hu %s (%d %s)", head, ep.address().to_string().c_str(), ep.port(), tail, ec.value(), ec.message().data());
		}

	protected:
		//��ʼ�����￪ʼ�������ǣ��������δ�ɹ�����ʼ���ӷ���������������Ѿ��ɹ�����ʼ�������ݡ�
		virtual bool DoStart() //connect or receive
		{
			if (!IsStopped())
			{
				if (m_bIsReConnecting && !IsConnected())
					GetLowestLayer().async_connect(m_ServerAddr,
					make_handler_error(boost::bind(&CAsioConnectorBase::ConnectHandler, this, boost::asio::placeholders::error))
					);
					//boost::bind(&CAsioConnectorBase::ConnectHandler, this, boost::asio::placeholders::error));
				else
					DoRecvMsg();

				return true;
			}
			return false;
		}
		// ����ʧ��ʱ�ص������ش��ڵ��������������Ӧ����֮�����ԣ����ظ����������
		//after how much time(ms), st_connector will try to reconnect to the server, negative means give up.
		virtual int PrepareReconnect(const boost::system::error_code& ec)
		{
			std::ignore = ec;
			return ASIO_RECONNECT_INTERVAL;
		}
		// ���ӳɹ�ʱ�ص����û���д���Եõ����ӳɹ��¼���
		virtual void OnConnect()
		{
			//	unified_out::info_out("connecting success."); 
		}
		// �Ƿ�ɷ������ݣ�is_connected���ϸ����is_send_allowedΪ���յ��ж������
		virtual bool is_send_allowed() const
		{
			return IsConnected() && CTcpSocketBase<Socket, Packer, Unpacker>::is_send_allowed();
		}
		// �������Ĭ�ϵ���Ϊ�ǹر����ӣ�������д���Զ�����Ϊ��
		virtual void on_unpack_error()
		{
			//unified_out::info_out("can not unpack msg."); 
			force_close();
		}
		// ���մ���Ĭ�ϵ���Ϊ�ǹر����ӣ�������д���Զ�����Ϊ��
		virtual void OnRecvError(const boost::system::error_code& ec)
		{
			//show_info("client link:", "broken/closed", ec);
			force_close(IsClosing() ? m_bIsReConnecting : PrepareReconnect(ec) >= 0);
			m_nCloseState = 0;

			if (m_bIsReConnecting)
				Start();
		}
		// �������io_service��Ȼ��������prepare_reconnect���ش��ڵ���0����������ʱ������ʱһС��ʱ��֮���������ӷ�������
		bool PrepareNextReconnect(const boost::system::error_code& ec)
		{
			if ((boost::asio::error::operation_aborted != ec || m_bIsReConnecting) && !IsStopped())
			{
#ifdef _WIN32
				if (boost::asio::error::connection_refused != ec && boost::asio::error::network_unreachable != ec && boost::asio::error::timed_out != ec)
#endif
			//	if (boost::asio::error::connection_refused != ec && boost::asio::error::timed_out != ec)
				{
					boost::system::error_code ec;
					GetLowestLayer().close(ec);
				}

				auto delay = PrepareReconnect(ec);
				if (delay >= 0)
				{
					SetTimer(TIMER_CONNECT, delay, [this](unsigned char id)->bool {assert(TIMER_CONNECT == id); DoStart(); return false; });
					return true;
				}
			}

			return false;
		}

	private:
		// �첽���Źرճ�ʱ��ʱ����
		bool AsyncCloseHandler(unsigned int id, size_t loop_num)
		{
			assert(TIMER_ASYNC_CLOSE == id);

			if (2 == m_nCloseState)
			{
				--loop_num;
				if (loop_num > 0)
				{
					UpdateTimerInfo(id, 10, boost::bind(&CAsioConnectorBase::AsyncCloseHandler, this, _1, loop_num));
					return true;
				}
				else
				{
				//	unified_out::info_out("failed to graceful close within %d seconds", ASIO_GRACEFUL_CLOSE_MAX_DURATION);
					force_close(m_bIsReConnecting);
				}
			}
			return false;
		}
		// ���ӳɹ�����ʧ�ܺ���asio�ص���
		void ConnectHandler(const boost::system::error_code& ec)
		{
			if (!ec)
			{
				m_bIsConnected = m_bIsReConnecting = true;
				OnConnect();
				reset_state();
				SendMsg(); //send buffer may have msgs, send them
				DoStart();
			}
			else
				PrepareNextReconnect(ec);
		}

	protected:
		// ��������ַ��
		boost::asio::ip::tcp::endpoint m_ServerAddr;
		// �Ƿ��Ѿ����ӳɹ���
		bool m_bIsConnected;
		// �Ƿ������������ӡ�
		bool m_bIsReConnecting;
	};
	using CAsioConnector = CAsioConnectorBase<>;

} //namespace

#endif /* ASIO_CONNECTOR_H_ */
