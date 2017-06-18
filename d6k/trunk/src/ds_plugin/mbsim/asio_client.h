
#ifndef  ASIO_CLIENT_H_
#define  ASIO_CLIENT_H_

#include "object_pool.h"

namespace ASIO
{
	// ֻ֧��һ�����ӵĿͻ��ˣ�����˵һ���׽��֣���tcp��udpͨ��
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
		//ʵ��CServiceInterface�Ĵ���ӿڣ���st_service_pumpʹ�ã�start_serviceʱ����init��stop_serviceʱ����uninit�����������ӿ���ʵ
		//����ʵ���˿�ʼ�ͽ����߼�����ʼ��ζ��ʲô��Socket���������������ǰ�潲st_socket��st_tcp_socket��st_udp_socket��
		//st_connector��st_server_socket��ʱ���Ѿ�˵�������
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

	//֧�ֶ������ӵĿͻ��ˣ�����˵����׽��֣���tcp��udpͨ�ã���ʵ������һ��������ֻ����object_pool����չ��һЩhelper����
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
		//ʵ��CServiceInterface�Ĵ���ӿڣ���st_service_pumpʹ�ã�start_serviceʱ���ã�����st_sclient::init����һ����ֻ�Ƕ����пͻ��˶���һ������ʼ��������
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
		//���һ�����ӵ�����أ�������client_ptr->start()�����resetΪtrue����ô����֮ǰ�����ȵ���client_ptr->reset��
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
