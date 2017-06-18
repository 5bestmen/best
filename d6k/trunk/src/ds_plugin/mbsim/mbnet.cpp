

#if _MSC_VER >= 1600
#define _WIN32_WINNT  0x0500
#endif

#include <thread>
#include <sstream> 
#include <algorithm>

#include "mbnet.h"

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

#include "modbus.h"


CModbusSocket::CModbusSocket(ASIO::CAsioServerInterface & svr) :ASIO::CServerSocketBase<ASIO::CPacker, CMBEUnpacker>(svr), m_TaskTimer(svr.GetServicePump())
{
	m_pModbusImpl = std::make_shared<CModbusImpl>( );

	m_TaskTimer.SetTimer(2016, 2000, std::bind(&CModbusSocket::OnTimer, this, std::placeholders::_1));
}

CModbusSocket::~CModbusSocket()
{
	StopAllTaskTimer();
}

void CModbusSocket::Reset()
{
	StopAllTaskTimer();
	ASIO::CServerSocketBase<ASIO::CPacker, CMBEUnpacker>::Reset();
}
 
//we can handle msg very fast, so we don't use recv buffer
bool CModbusSocket::OnMsg(out_msg_type& msg)
{
	//handle_msg(msg); 
	HandleMsg(msg);
	return true; 
} 
 
bool CModbusSocket::OnMsgHandle(out_msg_type& msg, bool link_down)
{
//	HandleMsg(msg);
	return true; 
}

void  CModbusSocket::OnUnpackError()
{
	//unified_out::error_out("can not unpack msg.");
	force_close();

	auto ep = this->GetLowestLayer().remote_endpoint();

	std::stringstream strmPort;
	strmPort << ep.port();

	std::string szRet = ep.address().to_string();

	if (strmPort.str().empty() == false)
	{
		szRet = szRet + ":" + strmPort.str();
	}
	szRet += "  force disconnected!";
	LogString(szRet.c_str(), 1);
}

bool CModbusSocket::OnTimer(unsigned int nID)
{
	std::ignore = nID;
	if (nID == 2016)
	{

	}
#if 0
	auto id = std::this_thread::get_id();
	std::string szLog("i am in timer . my thread is :");

	std::stringstream strmPort;
	strmPort << id;

	szLog += strmPort.str();
	LogString(szLog.c_str(), 2);
#endif

 
	return true;
}
/*! \fn void CModbusSocket::HandleMsg(out_msg_type & msg)
********************************************************************************************************* 
** \brief CModbusSocket::HandleMsg 
** \details 处理收到的数据
** \param msg 
** \return void 
** \author LiJin 
** \date 2016年7月28日 
** \note 
********************************************************************************************************/
void CModbusSocket::HandleMsg(out_msg_type & msg)
{// 接收到数据
//	LogString("HandleMsg....", 2);
#if 0
	auto id = std::this_thread::get_id();
	std::string szLog("i am in recv thread . my thread is :");
	std::stringstream strmPort;
	strmPort << id;
	szLog += strmPort.str();
	LogString(szLog.c_str(), 2);
#endif

	if (msg.length() > 0)
	{
		std::string szSrc = GetLocalEndPoint();
		std::string szDest = GetRemoteEndPoint();
		CommMsgLog(msg.c_str(), msg.length(), IMainModule::CMSG_TCP_RECV, "RECV", szDest.c_str(), szSrc.c_str());

		if (msg.length() > MB_TCP_FUNC)
		{// 生成PDU报文出来给Impl去处理		
			unsigned char pAduBuff[512] = { 0 };
			assert(msg.size() < 512);
			msg.copy(reinterpret_cast<char*>(pAduBuff), std::min(msg.size(),(size_t)512)  );

			unsigned char *pPdu = & pAduBuff[MB_TCP_FUNC];
			unsigned short nLen = static_cast<unsigned short>(msg.length()) - MB_TCP_FUNC;

		//	m_pModbusImpl->MBPoll(pPdu, &nLen);

			pAduBuff[MB_TCP_LEN] = (nLen + 1) >> 8U;
			pAduBuff[MB_TCP_LEN + 1] = (nLen + 1) & 0xFF;

			unsigned short nTCPLength = nLen + MB_TCP_FUNC;

			post_native_msg(reinterpret_cast<const char* const> (pAduBuff), nTCPLength, 1);
		}
		else
		{// 报文非法
			std::string szLog("recv error msg");
			LogString(szLog.c_str(), 2);
		}
	}
}

#ifdef ASIO_WANT_MSG_SEND_NOTIFY
void CModbusSocket::OnMsgSend(in_msg_type& msg)
{
	if (msg.length() > 0)
	{
		std::string szSrc = GetLocalEndPoint();
		std::string szDest = GetRemoteEndPoint();
		CommMsgLog(msg.c_str(), msg.length(), IMainModule::CMSG_TCP_SEND, "SEND", szSrc.c_str(), szDest.c_str());
	}
}
#endif

std::string CModbusSocket::GetRemoteEndPoint()
{
	std::string szRet;

	boost::asio::ip::tcp::endpoint ep = GetNextLayer().remote_endpoint();

	std::stringstream strmPort;
	strmPort << ep.port();

	szRet = ep.address().to_string();

	if (strmPort.str().empty() == false)
	{
		szRet = szRet + ":" + strmPort.str();
	}
	return szRet;
}

std::string CModbusSocket::GetLocalEndPoint()
{
	std::string szRet;

	boost::asio::ip::tcp::endpoint ep = GetNextLayer().local_endpoint();

	std::stringstream strmPort;
	strmPort << ep.port();

	szRet = ep.address().to_string();

	if (strmPort.str().empty() == false)
	{
		szRet = szRet + ":" + strmPort.str();
	}
	return szRet;
}

bool CModbusServer::OnAccept(ASIO::CObjectPool<CModbusSocket>::object_ctype& pClient)
{
	auto ep = pClient->GetLowestLayer().remote_endpoint();
	std::shared_ptr<CModbusImpl> pModbus = pClient->GetModbusImpl();
	assert(pModbus);
	if (pModbus)
	{
		pModbus->m_fnGetAddress = std::bind(&CMemData::MBGetAddress, &m_MemDataRef);
		pModbus->m_fnRegCoils = std::bind(&CMemData::MBRegCoilsCB, &m_MemDataRef, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		pModbus->m_fnRegHolding = std::bind(&CMemData::MBRegHoldingCB, &m_MemDataRef, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		pModbus->m_fnRegDiscrete = std::bind(&CMemData::MBRegDiscreteCB, &m_MemDataRef, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		pModbus->m_fnRegInput = std::bind(&CMemData::MBRegInputCB, &m_MemDataRef, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		pModbus->m_fnGetRemoteEndPoint = std::bind(&CModbusSocket::GetRemoteEndPoint, pClient);
	}

	std::stringstream strmPort;
	strmPort << ep.port();

	std::string szRet = ep.address().to_string();

	if (strmPort.str().empty() == false)
	{
		szRet = szRet + ":" + strmPort.str();
	}
	szRet += "  connected!";
	LogString(szRet.c_str(), 1);

	return true;
}
void CModbusServer::Stop()
{
//	auto can = container();
	do_something_to_all([](object_ctype& item) {item->StopAllTaskTimer(); });

}

CModbusSvc::CModbusSvc()
{
	using namespace ASIO;
	m_pSvcPump = std::make_shared<CServicePump>();
	m_pServer = std::make_shared<CModbusServer>(*m_pSvcPump,m_MemData);
}

CModbusSvc::~CModbusSvc()
{
	Stop();
}

bool CModbusSvc::Start()
{
	std::size_t nCpuCount = std::thread::hardware_concurrency();
	std::ignore = nCpuCount;

	m_pServer->set_server_addr(502);
 
	m_pSvcPump->StartService();
		
	if (m_pSvcPump->IsServiceStarted()== false || m_pSvcPump->IsRunning() == false )
	{
		LogString("start service err!", 1);
		return false;
	}

	LogString("start service success!", 1);
	return true;
}

void CModbusSvc::Stop()
{
	if (m_pServer)
	{
		m_pServer->Stop();
	}

	m_pSvcPump->StopService();
}
 