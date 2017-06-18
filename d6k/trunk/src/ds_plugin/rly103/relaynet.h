#ifndef RELAY_NET_103_H
#define RELAY_NET_103_H 

#include <QDebug>

#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  


#ifdef _MSC_VER
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif // !_WIN32_WINNT
#endif


#include <boost/timer/timer.hpp>

#define ASIO_WANT_MSG_SEND_NOTIFY

#include "asio_tcp_client.h"
#include "asio_connector.h"
#include "asio_packer.h"
//#include "asio_unpacker.h"
#include "rly103_unpacker.h"

#include "relay103.h"
#include "devicestudio/main_module.h"
#include "iosvc_timer.h"

extern bool LogString(const char *szLogTxt, int nLevel);
extern bool CommMsgLog(const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);

namespace ASIO
{
	class CServicePump;
}

class CNet103Socket : public ASIO::CAsioConnectorBase< ASIO::CPacker, CRly103Unpacker, boost::asio::ip::tcp::socket>
{
public:
	CNet103Socket(boost::asio::io_service& io_svc) : ASIO::CAsioConnectorBase< ASIO::CPacker, CRly103Unpacker, boost::asio::ip::tcp::socket>(io_svc), m_TaskTimer(io_svc)
	{
		m_p103Impl = std::make_shared<C103Impl>();
		m_p103Impl->m_fnSendData = std::bind(&CNet103Socket::SendData, this,
			std::placeholders::_1,std::placeholders::_2,std::placeholders::_3,false);
		m_p103Impl->m_fnGetRemoteEndPoint = std::bind(&CNet103Socket::GetRemoteEndPoint, this);
	} 
	virtual ~CNet103Socket()
	{

	}

	bool SendData(const char* const pstr[], const size_t len[], size_t num, bool can_overflow = false)
	{
		return send_native_msg(pstr, len, num, can_overflow);
	}

	virtual void Reset()
	{
		ASIO::CAsioConnectorBase< ASIO::CPacker, CRly103Unpacker, boost::asio::ip::tcp::socket>::Reset();
	}

	std::string GetRemoteEndPoint();

	bool OnTimer(unsigned int nID);
	void StopAllTaskTimer()
	{
		m_TaskTimer.StopAllTimer();
	}
protected:
	virtual bool OnMsg(out_msg_type& msg)
	{
		LogString("OnMsg....", 2);

		HandleMsg(msg); 
		return true;
	}
	//we should handle msg in on_msg_handle for time-consuming task like this:
	virtual bool OnMsgHandle(out_msg_type& msg, bool link_down)
	{
		std::ignore = link_down;
		LogString("on_msg_handle....", 2);

		HandleMsg(msg);
		return true;
	}
	 
	void HandleMsg(out_msg_type& msg);
	// 解包出错处理
	virtual void OnUnpackError()
	{
		LogString("on_unpack_error....", 2);
	}
	virtual void OnMsgSend(in_msg_type& msg)
	{
	//	LogString("on_msg_send....", 2);
		if (msg.length()>0)
		{
			CommMsgLog(msg.c_str(), msg.length(), IMainModule::CMSG_TCP_SEND, "SEND", "local", "192.168.0.221");
		}
	}
	virtual void OnConnect(); 
	//after how much time(ms), st_connector will try to reconnect to the server, negative means give up.
	virtual int PrepareReconnect(const boost::system::error_code& ec);

	virtual void OnRecvError(const boost::system::error_code& ec);
private:
	std::size_t m_nRecvBytes;
	std::shared_ptr<C103Impl> m_p103Impl;
	CIoSvcTimer  m_TaskTimer;
};

class CNet103Clients : public  ASIO::CTcpClientBase<CNet103Socket>
{
public:
	CNet103Clients(ASIO::CServicePump & svc_pump) :CTcpClientBase<CNet103Socket>(svc_pump), m_ServicePumpRef(svc_pump)
	{
		m_bStopFlag = false;	
		m_pSocket = nullptr;
	}

	void Start();

	void Stop();

protected:
	void MainTask(void *pImpl);

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}
	//! 主任务
	std::shared_ptr< std::thread > m_pMainTask;	 

	//! 客户端
	boost::shared_ptr<CNet103Socket> m_pSocket;
 
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	bool m_bStopFlag;

//	std::shared_ptr<ASIO::CAsioTcpSingleClient> m_pClient;
	ASIO::CServicePump & m_ServicePumpRef; 
};


class CNet103
{
public:
	CNet103();
	~CNet103();
public:
	bool Start();

	void Stop();

private:
	std::shared_ptr<ASIO::CServicePump> m_pSvcPump;

	std::shared_ptr<CNet103Clients>m_pNet103Client;
};



#endif // RELAY_NET_103_H
