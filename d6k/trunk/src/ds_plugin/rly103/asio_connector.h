

#ifndef ASIO_CONNECTOR_H_
#define ASIO_CONNECTOR_H_

#include "asio_tcp_socket.h"

#ifndef ST_ASIO_SERVER_IP
#define ASIO_SERVER_IP			"127.0.0.1"
#endif
#ifndef ASIO_SERVER_PORT
#define ASIO_SERVER_PORT			5050
#endif

// 异步连接，asio返回失败之后，暂停一定时间继续重试（即再次调用asnc_connect）。
#ifndef ASIO_RECONNECT_INTERVAL
#define ASIO_RECONNECT_INTERVAL	500 //millisecond(s)
#endif

namespace ASIO
{
	// 	带连接功能的 tcp_socket，算是一个真正的客户端了
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
		// 重置所有，st_object_pool在重用时会调用。st_connector的子类可重写它以重置自己的状态，记得最后需要调用本类的reset。
		virtual void Reset()
		{
			m_bIsConnected = false;
			m_bIsReConnecting = true;
			CTcpSocketBase<Socket, Packer, Unpacker>::Reset();
		//	m_nCloseState = 0;
		}
		// 在调用父类同名函数的基础上，增加了对是否重连接的判断，如果需要重连接则返回假。
		virtual bool obsoleted()
		{
			return !m_bIsReConnecting && CTcpSocketBase<Socket, Packer, Unpacker>::obsoleted();
		}
		// 设置服务端地址用于连接之用，需要在do_start之前被调用。
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
		// 是否已经连接成功。
		bool IsConnected() const
		{
			return m_bIsConnected;
		}
		// 直接调用force_close。
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
		//开始，这里开始的意义是，如果连接未成功，则开始连接服务器，如果连接已经成功，则开始接收数据。
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
		// 连接失败时回调，返回大于等于零则继续在相应毫秒之后重试，返回负数则放弃。
		//after how much time(ms), st_connector will try to reconnect to the server, negative means give up.
		virtual int PrepareReconnect(const boost::system::error_code& ec)
		{
			std::ignore = ec;
			return ASIO_RECONNECT_INTERVAL;
		}
		// 连接成功时回调，用户重写它以得到连接成功事件。
		virtual void OnConnect()
		{
			//	unified_out::info_out("connecting success."); 
		}
		// 是否可发送数据，is_connected加上父类的is_send_allowed为最终的判定结果。
		virtual bool is_send_allowed() const
		{
			return IsConnected() && CTcpSocketBase<Socket, Packer, Unpacker>::is_send_allowed();
		}
		// 解包错误，默认的行为是关闭连接，可以重写以自定义行为。
		virtual void on_unpack_error()
		{
			//unified_out::info_out("can not unpack msg."); 
			force_close();
		}
		// 接收错误，默认的行为是关闭连接，可以重写以自定义行为。
		virtual void OnRecvError(const boost::system::error_code& ec)
		{
			//show_info("client link:", "broken/closed", ec);
			force_close(IsClosing() ? m_bIsReConnecting : PrepareReconnect(ec) >= 0);
			m_nCloseState = 0;

			if (m_bIsReConnecting)
				Start();
		}
		// 如果允许（io_service仍然在运行且prepare_reconnect返回大于等于0），启动定时器以延时一小段时间之后重新连接服务器。
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
		// 异步优雅关闭超时定时器。
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
		// 连接成功或者失败后由asio回调。
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
		// 服务器地址。
		boost::asio::ip::tcp::endpoint m_ServerAddr;
		// 是否已经连接成功。
		bool m_bIsConnected;
		// 是否正在重新连接。
		bool m_bIsReConnecting;
	};
	using CAsioConnector = CAsioConnectorBase<>;

} //namespace

#endif /* ASIO_CONNECTOR_H_ */
