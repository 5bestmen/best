
#ifndef  __HANDLER_H_
#define __HANDLER_H_

#include "ace/Synch_Traits.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"


#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"

#include <vector>
using namespace std;


class CPeer;

class CHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
	typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> super;

public:
	CHandler()
	{
		static int nHandlerID = 0;
		m_nConnectID = nHandlerID++;
		m_pPeer = NULL;
	}
	CHandler(int nHandlerID)
	{
		m_nConnectID = nHandlerID;
		m_pPeer = NULL;
	}
	~CHandler()
	{
		int hello = 0;
	}

	virtual int open(void * = 0);

	virtual int handle_input(ACE_HANDLE handle = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);

	//连接ID
public:
	//void SetConnectID(int nConnectID) { m_nConnectID = nConnectID; }
	int   GetConnectID() { return m_nConnectID; }
private:
	int m_nConnectID;

	//发送消息
public:
	bool SendMessage(char* pBuff, int nMsgLen);

	//对端地址
public:
	ACE_INET_Addr GetRemoteAddr() { return m_addrRemote; }
private:
	ACE_INET_Addr m_addrRemote;

	//宿主对象
public:
	CPeer* GetSocketPeer() { return m_pPeer; }
	void SetSocketPeer(CPeer* pPeer) { m_pPeer = pPeer; }
private:
	CPeer* m_pPeer;

	//接收发送缓存
private:
	enum
	{
		MAX_HANDLER_BUFF_SIZE = 2048,
	};
	char m_buff_recv[MAX_HANDLER_BUFF_SIZE];
};


#endif /* __HANDLER_H_ */
