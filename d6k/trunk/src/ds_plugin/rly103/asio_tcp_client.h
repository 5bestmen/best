

#ifndef ASIO_TCP_CLIENT_H
#define ASIO_TCP_CLIENT_H

#include "asio_connector.h"
#include "asio_client.h"

namespace ASIO
{
   //using st_tcp_sclient = CAsioSingleClient<CAsioConnector>;
	using CAsioTcpSingleClient = CAsioSingleClient<CAsioConnector>;

	template<typename Socket = CAsioConnector, typename Pool = CObjectPool<Socket>>
	class CTcpClientBase : public CAsioMultiClient<Socket, Pool>
	{
	public:
		using CAsioMultiClient<Socket, Pool>::TIMER_BEGIN;
		using CAsioMultiClient<Socket, Pool>::TIMER_END;

		CTcpClientBase(CServicePump& svc_pump) : CAsioMultiClient<Socket, Pool>(svc_pump)
		{
		}
		template<typename Arg>
		CTcpClientBase(CServicePump& svc_pump, Arg arg) : CAsioMultiClient<Socket, Pool>(svc_pump, arg)
		{
		}

		//connected link size, may smaller than total object size(st_object_pool::size)
		// 有效（已连接）的连接数量。
		size_t valid_size()
		{
			size_t size = 0;
			 do_something_to_all([&size](typename Pool::object_ctype& item) {
				 if (item->IsConnected())
					++size;
			});
			return size;
		}

		using CAsioMultiClient<Socket, Pool>::add_client;
		typename Pool::ObjectType add_client()
		{
			auto client_ptr( create_object());
			return  add_client(client_ptr,false) ? client_ptr : typename Pool::ObjectType();
		}
		typename Pool::ObjectType add_client(unsigned short port, const std::string& ip = ASIO_SERVER_IP)
		{
			auto client_ptr( create_object());
			client_ptr->set_server_addr(port, ip);
			return  add_client(client_ptr,false) ? client_ptr : typename Pool::ObjectType();
		}

		void broadcast_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->send_msg(pstr, len, num, can_overflow); });
		}
		void broadcast_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->send_native_msg(pstr, len, num, can_overflow); });
		}
		void safe_broadcast_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->safe_send_msg(pstr, len, num, can_overflow); });
		}
		void safe_broadcast_native_msg(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->safe_send_native_msg(pstr, len, num, can_overflow); });
		}
		///////////////////////////////////////////////////
// 		//msg sending interface
// 		TCP_BROADCAST_MSG(broadcast_msg, send_msg)
// 			TCP_BROADCAST_MSG(broadcast_native_msg, send_native_msg)
// 			//guarantee send msg successfully even if can_overflow equal to false
// 			//success at here just means put the msg into st_tcp_socket_base's send buffer
// 			TCP_BROADCAST_MSG(safe_broadcast_msg, safe_send_msg)
// 			TCP_BROADCAST_MSG(safe_broadcast_native_msg, safe_send_native_msg)
			//msg sending interface
			///////////////////////////////////////////////////

			//functions with a client_ptr parameter will remove the link from object pool first, then call corresponding function, if you want to reconnect to the server,
			//please call client_ptr's 'disconnect' 'force_close' or 'graceful_close' with true 'reconnect' directly.
		void disconnect(typename Pool::object_ctype& client_ptr)
		{
			del_object(client_ptr);
			client_ptr->disconnect(false);
		}
		void disconnect(bool bReConnect = false)
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->disconnect(bReConnect); });
		}
		void force_close(typename Pool::object_ctype& client_ptr)
		{
			del_object(client_ptr);
			client_ptr->force_close(false);
		}
		void force_close(bool reconnect = false) 
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->force_close(reconnect); });
		}
		void graceful_close(typename Pool::object_ctype& client_ptr, bool sync = true) 
		{
			del_object(client_ptr);
			client_ptr->graceful_close(false, sync);
		}
		void graceful_close(bool reconnect = false, bool sync = true) 
		{
			do_something_to_all([=](typename Pool::object_ctype& item) {item->graceful_close(reconnect, sync); }); 
		}
	protected:
		virtual void Uninit() 
		{
			Stop(); 
			graceful_close();
		}
	};
	using CAsioTcpClient = CTcpClientBase<>;
} //namespace

#endif /* ASIO_TCP_CLIENT_H */
