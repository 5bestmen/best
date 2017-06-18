

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif

#include <thread>
#include<sstream> 

#if 0
#if _MSC_VER >= 1600
#include <vld.h>
#endif
#endif

#include "relaynet.h"

#include "service_pump.h"
#include "asio_timer.h"
#include "asio_client.h"
#include "object_pool.h"
#include "asio_client.h"
#include "asio_buffer.h"
#include "asio_packer.h"
#include "asio_unpacker.h"
#include "asio_socket.h"
#include "asio_connector.h" 
#include "asio_tcp_client.h"

bool CNet103Socket::OnTimer(unsigned int nID)
{
	std::ignore = nID;
//	LogString("ttttttttttt....", 2);
	if (nID == 100)
	{
		if (m_p103Impl->IsReqestedCatalog() == false)
		{

		}
	}

	return true;
}

void CNet103Socket::HandleMsg(ASIO::CAsioConnectorBase< ASIO::CPacker, CRly103Unpacker, boost::asio::ip::tcp::socket>::out_msg_type & msg)
{
	LogString("HandleMsg....", 2);
	if (msg.length() > 0)
	{
		CommMsgLog(msg.c_str(), msg.length(), IMainModule::CMSG_TCP_RECV, "RECV", "192.168.0.221", "local");

		if (m_p103Impl)
		{
			bool bRet = m_p103Impl->HandleMsg(msg.c_str(), msg.length());
			if (bRet)
			{
				m_TaskTimer.SetTimer(100, 1000, std::bind(&CNet103Socket::OnTimer, this, std::placeholders::_1));			
				
			}
	 
		}
	}
}

std::string CNet103Socket::GetRemoteEndPoint()
{
	std::string szRet;

	boost::asio::ip::tcp::endpoint ep =  GetNextLayer().remote_endpoint();

	std::stringstream strmPort;
	strmPort << ep.port();

	szRet = ep.address().to_string();

	if (strmPort.str().empty() == false)
	{
		szRet = szRet + ":" + strmPort.str();
	}
	return szRet;
}
 /*! \fn  void CNet103Socket::OnConnect()
 ********************************************************************************************************* 
 ** \brief CNet103Socket::OnConnect 
 ** \details 连接成功
 ** \return void 
 ** \author LiJin 
 ** \date 2016年7月5日 
 ** \note 
 ********************************************************************************************************/
void CNet103Socket::OnConnect()
{
	LogString( "connecting success.", 1);
	// 下发启动帧
	unsigned char pStartFrm[15] = { 0 };
	m_p103Impl->ISendStartDT(pStartFrm, 15);
	send_native_msg(reinterpret_cast< const char* const> (pStartFrm), 15, 1);
} 
/*! \fn int CNet103Socket::PrepareReconnect(const boost::system::error_code& ec)
********************************************************************************************************* 
** \brief CNet103Socket::PrepareReconnect 
** \details 连接失败时回调，返回大于等于零则继续在相应毫秒之后重试，返回负数则放弃。
** \param ec 
** \return int 
** \author LiJin 
** \date 2016年7月15日 
** \note 
********************************************************************************************************/
int CNet103Socket::PrepareReconnect(const boost::system::error_code& ec)
{
	std::ignore = ec;

	LogString("connecting failed.waiting 1s reconnect", 1);

	// 等待1秒后重新连接
	return 1000;
}
/*! \fn CNet103Socket::OnRecvError 
********************************************************************************************************* 
** \brief CNet103Socket::OnRecvError 
** \details 接收错误，默认的行为是关闭连接，可以重写以自定义行为。
** \param ec 
** \return void  
** \author LiJin 
** \date 2016年7月15日 
** \note 
********************************************************************************************************/
void CNet103Socket::OnRecvError(const boost::system::error_code& ec)
{
	LogString("recv failed.waiting 1s reconnect", 1);
	ASIO::CAsioConnectorBase< ASIO::CPacker, CRly103Unpacker, boost::asio::ip::tcp::socket>::OnRecvError(ec);
}

void CNet103Clients::Start()
{
	SetTaskStartFlag();

	m_pSocket = this->add_client(6000, "192.168.0.221");
	m_ServicePumpRef.StartService();

//	m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	if (m_pMainTask!=nullptr)
	{
	 	m_pMainTask.reset(new (std::thread)(&CNet103Clients::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
	 	m_pMainTask = std::make_shared<std::thread>(&CNet103Clients::MainTask, this, nullptr);
	}	
}


void CNet103Clients::Stop()
{	
//	StopAllTaskTimer();
	if (m_pSocket)
	{
		m_pSocket->StopAllTaskTimer();
	}

	SetTaskStopFlag();
	// 退出线程
	Q_ASSERT(m_pMainTask!=nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{	 
		m_pMainTask->join();
	}

	m_ServicePumpRef.StopService();
}

void CNet103Clients::SetTaskStartFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = false;
}

void CNet103Clients::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}

void CNet103Clients::MainTask(void *pImpl)
{
	LogString( "start main task....", 2);
	
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CNet103Clients::IsStopped,this));
		if (m_bStopFlag)
		{
			LogString( "exit main task1....", 2);
			return;
		}
		
// 		if (bRet != std::cv_status::timeout)
// 		{
// 			LogString("rly103", "exit main task....", 2);
// 			return;
// 		}
//		LogString("rly103", "in main task....", 2);

		//DoSth();
	}
}

CNet103::CNet103()
{
	using namespace ASIO;
	m_pSvcPump = std::make_shared<CServicePump>();
}

CNet103::~CNet103()
{
	Stop();
}

bool CNet103::Start()
{
	std::size_t nCpuCount = std::thread::hardware_concurrency();
	std::ignore = nCpuCount;

	m_pNet103Client = std::make_shared<CNet103Clients>(*m_pSvcPump);

	m_pNet103Client->Start();
	return true;
}

void CNet103::Stop()
{
	if (m_pNet103Client)
	{
		m_pNet103Client->Stop();
	}

}
 