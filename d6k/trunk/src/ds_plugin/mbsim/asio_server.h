

#ifndef ASIO_SERVER_H
#define ASIO_SERVER_H

#include "asio_server_socket.h"
#include "object_pool.h"

#ifndef ASIO_SERVER_PORT
#define ASIO_SERVER_PORT			5050
#endif
static_assert(ASIO_SERVER_PORT > 0, "server port must be bigger than zero.");

#ifndef ASIO_ASYNC_ACCEPT_NUM
#define ASIO_ASYNC_ACCEPT_NUM	1 //how many async_accept delivery concurrently
#endif
static_assert(ASIO_ASYNC_ACCEPT_NUM > 0, "async accept number must be bigger than zero.");

//in set_server_addr, if the IP is empty, ST_ASIO_TCP_DEFAULT_IP_VERSION will define the IP version, or the IP version will be deduced by the IP address.
//boost::asio::ip::tcp::v4() means ipv4 and boost::asio::ip::tcp::v6() means ipv6.
#ifndef ASIO_TCP_DEFAULT_IP_VERSION
#define ASIO_TCP_DEFAULT_IP_VERSION boost::asio::ip::tcp::v4()
#endif

namespace ASIO
{
	//服务端服务器类，主要功能是监听和接受连接，连接管理继承于Pool
	template<typename Socket = CServerSocket, typename Pool = CObjectPool<Socket>, typename ServerInterface = CAsioServerInterface>
	class CServerBase : public ServerInterface, public Pool
	{
	public:
		using Pool::TIMER_BEGIN;
		using Pool::TIMER_END;

		CServerBase(CServicePump& svc_pump) : Pool(svc_pump), m_acceptor(svc_pump)
		{
			set_server_addr(ASIO_SERVER_PORT);
		}
		template<typename Arg>
		CServerBase(CServicePump& svc_pump, Arg arg) : Pool(svc_pump, arg), m_acceptor(svc_pump)
		{ 
			set_server_addr(ASIO_SERVER_PORT);
		}
		//设置获取监听地址。
		bool set_server_addr(unsigned short nPort, const std::string& szIp = std::string())
		{
			if (szIp.empty())
				m_epServerAddr = boost::asio::ip::tcp::endpoint(ASIO_TCP_DEFAULT_IP_VERSION, nPort);
			else
			{
				boost::system::error_code ec;
				auto addr = boost::asio::ip::address::from_string(szIp, ec);
				if (ec)
					return false;

				m_epServerAddr = boost::asio::ip::tcp::endpoint(addr, nPort);
			}

			return true;
		}
	
		const boost::asio::ip::tcp::endpoint& get_server_addr() const 
		{
			return m_epServerAddr; 
		}

		void stop_listen() 
		{ 
			boost::system::error_code ec; 
			m_acceptor.cancel(ec);
			m_acceptor.close(ec);
		}
		bool is_listening() const
		{
			return m_acceptor.is_open(); 
		}
		//实现i_server中的三个纯虚接口。del_client的功能主要是调用st_object_pool的del_object函数，并输出一些信息，主要用于连接断开时调用，
		//也可用于主动断开连接，但是用st_socket的force_close或者graceful_close更理合理一些。

		//implement CAsioServerInterface's pure virtual functions
		virtual CServicePump& GetServicePump()
		{
			return Pool::GetServicePump();
		}
		virtual const CServicePump& GetServicePump() const
		{ 
			return Pool::GetServicePump();
		}
		virtual bool del_client(const boost::shared_ptr<CAsioTimer>& client_ptr)
		{
			auto raw_client_ptr(boost::dynamic_pointer_cast<Socket>(client_ptr));
			return raw_client_ptr && del_object(raw_client_ptr) ? raw_client_ptr->force_close(), true : false;
		}

		//do not use graceful_close() as client does, because in this function, object_can_mutex has been locked, graceful_close will wait until on_recv_error() been invoked,
		//but in on_recv_error(), we need to lock object_can_mutex too(in del_object()), this will cause dead lock
		void close_all_client() 
		{
			do_something_to_all([](typename Pool::object_ctype& item) {item->force_close(); }); 
		}

		void broadcast_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->send_msg(pstr, len, num, can_overflow); });
		}
		void broadcast_msg(const char* pstr, size_t len, bool can_overflow = false) 
		{
			return broadcast_msg(&pstr, &len, 1, can_overflow);
		}
		void broadcast_msg(const std::string& str, bool can_overflow = false)
		{
			return broadcast_msg(str.data(), str.size(), can_overflow);
		}

		void broadcast_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->send_native_msg(pstr, len, num, can_overflow); });
		}
		void broadcast_native_msg(const char* pstr, size_t len, bool can_overflow = false)
		{
			return broadcast_native_msg(&pstr, &len, 1, can_overflow);
		}
		void broadcast_native_msg(const std::string& str, bool can_overflow = false)
		{
			return broadcast_native_msg(str.data(), str.size(), can_overflow);
		}
		//guarantee send msg successfully even if can_overflow equal to false, success at here just means putting the msg into send buffer successfully
		void safe_broadcast_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->safe_send_msg(pstr, len, num, can_overflow); });
		}
		void safe_broadcast_msg(const char* pstr, size_t len, bool can_overflow = false)
		{
			return safe_broadcast_msg(&pstr, &len, 1, can_overflow);
		}
		void safe_broadcast_msg(const std::string& str, bool can_overflow = false)
		{
			return safe_broadcast_msg(str.data(), str.size(), can_overflow);
		}

		void safe_broadcast_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->safe_send_native_msg(pstr, len, num, can_overflow); });
		}
		void safe_broadcast_native_msg(const char* pstr, size_t len, bool can_overflow = false)
		{
			return safe_broadcast_native_msg(&pstr, &len, 1, can_overflow);
		}
		void safe_broadcast_native_msg(const std::string& str, bool can_overflow = false)
		{
			return safe_broadcast_native_msg(str.data(), str.size(), can_overflow);
		}

		///////////////////////////////////////////////////
		//msg sending interface
// 		TCP_BROADCAST_MSG(broadcast_msg, send_msg)
// 		TCP_BROADCAST_MSG(broadcast_native_msg, send_native_msg)
// 		//guarantee send msg successfully even if can_overflow equal to false, success at here just means putting the msg into send buffer successfully
// 		TCP_BROADCAST_MSG(safe_broadcast_msg, safe_send_msg)
// 		TCP_BROADCAST_MSG(safe_broadcast_native_msg, safe_send_native_msg)
		//msg sending interface
		///////////////////////////////////////////////////

		void disconnect(typename Pool::object_ctype& client_ptr)
		{
			del_object(client_ptr);
			client_ptr->disconnect();
		}
		void force_close(typename Pool::object_ctype& client_ptr)
		{
			del_object(client_ptr);
			client_ptr->force_close();
		}
		void graceful_close(typename Pool::object_ctype& client_ptr, bool sync = true)
		{
			del_object(client_ptr);
			client_ptr->graceful_close(sync);
		}

	protected:
		//实现 纯虚接口，在启动和关闭service的时候，由CServicePump调用。
		virtual bool Init()
		{
			boost::system::error_code ec;
			m_acceptor.open(m_epServerAddr.protocol(), ec); 
			assert(!ec);
#ifndef ASIO_NOT_REUSE_ADDRESS
			m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec); assert(!ec);
#endif
			m_acceptor.bind(m_epServerAddr, ec); 
			assert(!ec);
			if (ec) 
			{ 
				GetServicePump().stop();
				unified_out::error_out("bind failed."); 
				return false; 
			}
			m_acceptor.listen(boost::asio::ip::tcp::acceptor::max_connections, ec); 
			assert(!ec);
			if (ec)
			{ 
				GetServicePump().stop();
				unified_out::error_out("listen failed.");
				return false; 
			}
			
			Start();

			for (auto i = 0; i < ASIO_ASYNC_ACCEPT_NUM; ++i)
				start_next_accept();

			return true;
		}
		virtual void Uninit()
		{  
			Stop(); 
			stop_listen(); 
			close_all_client();
		}

		virtual bool OnAccept(typename Pool::object_ctype& client_ptr)
		{ 
			std::ignore = client_ptr;
			return true;
		}
		//开始下一次异步接受连接，抽象成一个方法是因为两个地方需要调用，一是init内，一accept_handler内。
		//使用者也可以随时调用这个方法，如果你想增加一些异步accept的话（参看ASIO_ASYNC_ACCEPT_NUM宏）。
		virtual void start_next_accept()
		{
			auto client_ptr = create_object(boost::ref(*this));
			m_acceptor.async_accept(client_ptr->GetLowestLayer(), boost::bind(&CServerBase::accept_handler, this, boost::asio::placeholders::error, client_ptr));
		}
	protected:
		bool add_client(typename Pool::object_ctype& client_ptr)
		{
			if (add_object(client_ptr))
			{
				client_ptr->show_info("client:", "arrive.");
				return true;
			}

			client_ptr->show_info("client:", "been refused because of too many clients.");
			client_ptr->force_close();
			return false;
		}

		void accept_handler(const boost::system::error_code& ec, typename Pool::object_ctype& client_ptr)
		{
			if (!ec)
			{
				if (OnAccept(client_ptr) && add_client(client_ptr))
					client_ptr->Start();

				start_next_accept();
			}
			else
				stop_listen();
		}
	protected:
		boost::asio::ip::tcp::endpoint m_epServerAddr;
		boost::asio::ip::tcp::acceptor m_acceptor;
	};
	using CServer = CServerBase< >;


} //namespace

#endif /* ASIO_SERVER_H */
