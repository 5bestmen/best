
#ifndef  ASIO_CLIENT_H_
#define  ASIO_CLIENT_H_

#include "object_pool.h"

namespace ASIO
{
	// 只支持一条连接的客户端（或者说一个套接字），tcp和udp通用
	template<typename Socket>
	class CAsioSingleClient : public CServicePump::CServiceInterface, public Socket
	{
	public:
		using Socket::TIMER_BEGIN;
		using Socket::TIMER_END;

		CAsioSingleClient(CServicePump& svc_pump) : CServiceInterface(svc_pump), Socket(svc_pump) 
		{

		}
		template<typename Arg>
		CAsioSingleClient(CServicePump& svc_pump, Arg& arg) : CServiceInterface(svc_pump), Socket(svc_pump, arg) 
		{

		}
	protected:
		//实现CServiceInterface的纯虚接口，供st_service_pump使用（start_service时调用init，stop_service时调用uninit），这两个接口其实
		//就是实现了开始和结束逻辑，开始意味着什么由Socket决定，这个我们在前面讲st_socket、st_tcp_socket、st_udp_socket、
		//st_connector和st_server_socket的时候，已经说过多次了
		virtual bool Init()
		{
			Reset();
			Start();
			//SendMsg();
			return Socket::IsStarted();
		}
		virtual void Uninit()
		{
			graceful_close();
		}
	};

	//支持多条连接的客户端（或者说多个套接字），tcp和udp通用，其实它更像一个容器，只是在object_pool上扩展了一些helper函数
	template<typename Socket, typename Pool>
	class CAsioMultiClient : public Pool
	{
	protected:
		using Pool::TIMER_BEGIN;
		using Pool::TIMER_END;

		CAsioMultiClient(CServicePump& service_pump_) : Pool(service_pump_)
		{

		}

		template<typename Arg>
		CAsioMultiClient(CServicePump& service_pump_, Arg arg) : Pool(service_pump_, arg)
		{

		}
		//实现CServiceInterface的纯虚接口，供st_service_pump使用（start_service时调用），跟st_sclient::init功能一样，只是对所有客户端都做一个“开始”操作。
		virtual bool Init()
		{
			do_something_to_all([](typename Pool::object_ctype& item) {item->Reset(); });
			do_something_to_all([](typename Pool::object_ctype& item) {item->Start(); });
		//	do_something_to_all([](typename Pool::object_ctype& item) {item->SendMsg(); });
			Start();
			return true;
		}
	public:
		//parameter reset valid only if the service pump already started, or service pump will call object pool's init function before start service pump
		//添加一个连接到对象池，并调用client_ptr->start()，如果reset为true，那么在这之前还会先调用client_ptr->reset。
		bool add_client(typename Pool::object_ctype& client_ptr, bool bReset = true)
		{
			if (add_object(client_ptr))
			{
				if (GetServicePump().IsServiceStarted()) //service already started
				{
					if (bReset)
						client_ptr->Reset();
					client_ptr->Start();
				}
				return true;
			}
			return false;
		}
		//unseal object creation for client endpoint object pool.
		using Pool::create_object;
	};

} //namespace

#endif /* ASIO_CLIENT_H_ */
