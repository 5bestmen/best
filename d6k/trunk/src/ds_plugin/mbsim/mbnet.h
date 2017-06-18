#ifndef MODBUS_NET_H
#define MODBUS_NET_H 

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
#include "asio_server.h"
#include "asio_server_socket.h"
#include "mb_unpacker.h"
 
#include "devicestudio/main_module.h"
#include "iosvc_timer.h"

#include "memdata.h"

extern bool LogString(const char *szLogTxt, int nLevel);
extern bool CommMsgLog(const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);

namespace ASIO
{
	class CServicePump;
} 

class  CModbusImpl;

class CModbusSocket :public ASIO::CServerSocketBase<  ASIO::CPacker, CMBEUnpacker >
{
public:
	CModbusSocket(ASIO::CAsioServerInterface & svr);
	~CModbusSocket();
public:
	//because we don't use objects pool(we don't defined ASIO_REUSE_OBJECT), so this virtual function will
	//not be invoked, and can be omitted, but we keep it for possibly future using
	virtual void Reset();
	void StopAllTaskTimer()
	{
		m_TaskTimer.StopAllTimer();
	}
	std::shared_ptr<CModbusImpl> GetModbusImpl()
	{
		return m_pModbusImpl;
	}

	std::string GetRemoteEndPoint();
	std::string GetLocalEndPoint();

protected:
	//msg handling
//#ifndef ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
	//we can handle msg very fast, so we don't use recv buffer
	virtual bool OnMsg(out_msg_type& msg);
//#endif
	virtual bool OnMsgHandle(out_msg_type& msg, bool link_down);
	//msg handling end

#ifdef ASIO_WANT_MSG_SEND_NOTIFY
	virtual void OnMsgSend(in_msg_type& msg);
#endif

	virtual void OnUnpackError();
private:
	void HandleMsg(out_msg_type& msg);	
	bool OnTimer(unsigned int nID);
private:
	std::shared_ptr<CModbusImpl> m_pModbusImpl;
	CIoSvcTimer m_TaskTimer;

	unsigned char m_pAduBuff[512];

//	std::shared_ptr<CIoSvcTimer> m_pTaskTimer;
};
 
class CModbusServer : public ASIO::CServerBase<CModbusSocket, ASIO::CObjectPool<CModbusSocket>, ASIO::CAsioServerInterface>
{
public:
	CModbusServer(ASIO::CServicePump& svc_pump, CMemData & memData)
		: ASIO::CServerBase<CModbusSocket, ASIO::CObjectPool<CModbusSocket>, ASIO::CAsioServerInterface>(svc_pump), m_MemDataRef(memData)
	{
	 
	}
public:
	virtual bool OnAccept(ASIO::CObjectPool<CModbusSocket>::object_ctype& pClient);
public:
	void Stop();
private:
	CMemData  &m_MemDataRef;
//	CMemData  *m_pMemDataRef;

};


class CModbusSvc
{
public:
	CModbusSvc();
	~CModbusSvc();
public:
	bool Start();

	void Stop();

	CMemData & GetMemData()
	{
		return m_MemData;
	}
private:
	CMemData  m_MemData;
	std::shared_ptr<ASIO::CServicePump> m_pSvcPump;
	std::shared_ptr<CModbusServer> m_pServer;
};



#endif // MODBUS_NET_H
