/*! @file connector.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  connector.h
�ļ�ʵ�ֹ��� :  ��Ϊ�ͻ���ȥ���������ڵ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��Ϊ�ͻ���ȥ���������ڵ�����״̬�ж�
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.08
*******************************************************************************/
#ifndef CONNECTOR_MODULE_H
#define CONNECTOR_MODULE_H

#include <atomic>
  
#include "datatypes.h"

#include "ace/INET_Addr.h"
#include "ace/Task.h"
#include "ace/Proactor.h"
#include "ace/Connector.h"
// #include "ace/Asynch_Acceptor.h"
// #include "ace/Asynch_Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"

#include "ace/Reactor_Notification_Strategy.h"
#include "ace/CDR_Base.h"
#include "netbus/nbdef.h"
#include "connector_mgr.h"
#include "msg_queue.h"
#include "msg_cache.h"

template <typename SVC_HANDLER>
class CConnector;
class CPeerHandler;
class CConnectorMgr;

class CConnectorTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CConnectorTask(ACE_INET_Addr& addr, CConnectorMgr* pConnMgr, unsigned int nCurWorkingNet = NET_A);

	~CConnectorTask();

	// ����reactorģʽ
	bool Start(int numThreads = 1);

	// ֹͣproactorģʽ
	bool Stop(void);

	virtual int close(u_long flags = 0);

	virtual int svc();

	int SendData(const unsigned char *pBuf, unsigned int nLen);

	bool PushMsg(const int8u *pBuf, unsigned int nLen, int32u msgLevel = GENERAL_MSG);

	bool PushMsg(ACE_Message_Block* pBlock, int32u msgLevel = GENERAL_MSG, ACE_Time_Value* pTimeout = nullptr);

	 CConnectorMgr * GetConnMgr()
	{
		return m_pConnMgr;
	}
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
	void LogDebugMsg(const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc);


private:

	// ��ʱ��ID
	long m_nTimerID;

	// ����״̬
	//std::atomic<bool> m_bReconnState;

	// ������ʱ��,delay:�ӳ�ʱ��, interval����ʱ����, arg:���
	// ���� -1 ʧ��
	long StartTimer(int delay, int interval, const void* arg = 0);

	// ��ʱ���ص�����
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);

private:
	// ɾ��reactorģʽ
	int DeleteReactor(void);

private:

	ACE_INET_Addr m_addr;

	CConnector<CPeerHandler>* m_pConnector;

	ACE_Reactor* m_pReactor;
	ACE_SYNCH_RECURSIVE_MUTEX m_lock;

	std::atomic<bool>  m_nStopState;   //! stop״̬

	CConnectorMgr *m_pConnMgr;
	//ACE_Thread_Semaphore m_sem;

	std::atomic<int>  m_nCurWorkingNet;   //! ��ǰ����������

};

class CPeerHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_SYNCH>
{
	enum
	{
		HIGH_WATER = 1024 * 1024 * ACE_IOV_MAX, // ��ˮλ
		LOW_WATER = 1024 * ACE_IOV_MAX			// ��ˮλ
	};

	// ��������
	static const int m_reconnCnt = 3;

	const int32u MAX_RECV_LEN = ACE_DEFAULT_MAX_SOCKET_BUFSIZ - ACE_CDR::MAX_ALIGNMENT - 8;

public:

	CPeerHandler();
	void InitConnMgr(ACE_Reactor* pReactor, CConnectorMgr * pConnMgr, unsigned int nCurWorkingNet = NET_A);

	~CPeerHandler();

	/*! \fn int PushMsg(const unsigned char *pBuf, unsigned int nLen, bool bReconn = true);
	*********************************************************************************************************
	** \brief PushMsg
	** \details ����Ϣ�ŵ�������Ϣ������
	** \param pBuf ��Ϣ����
	** \param nLen ��Ϣ����
	** \author ZhuYongjin
	** \date 2016��12��16��
	** \note
	********************************************************************************************************/
	bool PushMsg(const int8u *pBuf, unsigned int nLen, int32u msgLevel = GENERAL_MSG);

	/*! \fn int PushMsg(ACE_Message_Block* pBlock, ACE_Time_Value* pTimeout = nullptr);
	*********************************************************************************************************
	** \brief PushMsg
	** \details ����Ϣ�ŵ�������Ϣ������
	** \param pBlock ��Ϣ����
	** \param pTimeout ��ʱʱ��
	** \author ZhuYongjin
	** \date 2016��12��16��
	** \note
	********************************************************************************************************/
	bool PushMsg(ACE_Message_Block* pBlock, int32u msgLevel = GENERAL_MSG, ACE_Time_Value* pTimeout = nullptr);

	// Activate the handler when connection is established.
	virtual int open(void *connector = 0);

	// �߳�ִ�к���
	virtual int svc();

	// Called on failed connection attempt.
	virtual int close(u_long flags = 0);

	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);

protected:
	void LogMsg(const char *szLogTxt, int nLevel);
	void LogDebugMsg(const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc);

	int GetFromMsgQueue(ACE_Message_Block *&mb, ACE_Time_Value *timeout = 0);

	 CConnectorMgr * GetConnectorMgr();
private:

	virtual int handle_input(ACE_HANDLE  fd /*= ACE_INVALID_HANDLE*/);	

	// ���͵�����Ϣ
	int32s send(const void *mb, size_t length, bool bReconn = true);

	// ���͵�����Ϣ
	int send(ACE_Message_Block *mb, bool bReconn = true);

	// ���Ϳ����ACE_IOV_MAX��64���飩
	int send(ACE_Message_Block *chunk[], size_t &count, bool bReconn = true);

	// ������Ϣ������
	int32s SendMsgAndReconn(const void *mb, size_t length, bool bReconn = true);

	// ������Ϣ
	bool HandleMsg(char* pData, size_t nLen);

protected:

	// = These methods implement the State pattern.
	int uninitialized(void)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("uninitialized!\n")));
		return 0;
	}

	int connected(void)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("in connected\n")));
		return 0;
	}

	int stdio(void)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("in stdio\n")));
		return 0;
	}

public:

	// ������յ�����Ϣ
	bool SetRecvMsg(ACE_Message_Block *mb, int priority = GENERAL_MSG);

	// ��ȡ���յ�����Ϣ
	bool GetRecvMsg(ACE_Message_Block *mb);

public:
	// ����ģʽ
	std::atomic<bool> m_bSendMode;   // true����������,false:��������
	CConnector<CPeerHandler>* m_pConnector;

private:

	// ��socket�յ�����֮���ӵ�������
	ACE_Message_Queue<ACE_MT_SYNCH> m_RecvQueue;

	// ��Ҫ���ͳ�ȥ�������ݴ����������
	//ACE_Message_Queue<ACE_MT_SYNCH> m_SendQueue;

	// ���ӹ���ָ��
	 CConnectorMgr*  m_pConnMgr;

	 // ��Ϣ���ദ����
	 CMsgCache m_msgCache;
	
	 //! ��ǰ����������
	std::atomic<int>  m_nCurWorkingNet;   

	// ���յ���������
	size_t m_cnt;

	// �߳�ID
	ACE_thread_t  m_threadsID[0x01]; 

	// ��ʱ��ID
	long m_nTimerID;

	// ��ʱ��Reactor
	ACE_Reactor* m_pReactor;

	// �Ƿ�����
	std::atomic<bool> m_isRunning;
};


template <typename SVC_HANDLER>
class CConnector : public ACE_Connector<SVC_HANDLER, ACE_SOCK_CONNECTOR>
{

public:

	CConnector(ACE_INET_Addr addr, ACE_Reactor* pReactor,  CConnectorMgr * pConnMgr, unsigned int nCurWorkingNet)
		: m_remoteAddr(addr), m_pReactor(pReactor), m_pConnMgr(pConnMgr), m_pHandler(nullptr)
	{
		ACE_ASSERT(m_pReactor);

		if (nCurWorkingNet > NET_B)
		{
			m_nCurWorkingNet = NET_A;
		}

		m_nCurWorkingNet = nCurWorkingNet;

	}

	~CConnector(void)
	{
		Shutdown();
	}

	void Shutdown()
	{
		Fini();

		if (m_pHandler != nullptr)
		{
			delete m_pHandler;
			m_pHandler = nullptr;
		}

		//m_pHandler->shutdown();
	}

	// Initialize the IPC client.
	virtual int Init()
	{
		// optionĬ��5�볬ʱ
		//ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);		
		//this->m_options.set(ACE_Synch_Options::USE_REACTOR, timeout);

		// û�г�ʱ
		//m_options.set(ACE_Synch_Options::USE_REACTOR);
	
		//m_pConnMgr->GetMsgQueue()->GetMsgQueue().is_empty();// .GetMsgQueue().is_empty();

		//ACE_NEW_RETURN(m_handler, SVC_HANDLER(m_pReactor), -1);
		ACE_NEW_RETURN(m_pHandler, SVC_HANDLER(), -1);
		m_pHandler->InitConnMgr(m_pReactor, m_pConnMgr, m_nCurWorkingNet);
		//m_handler = std::make_shared(m_pReactor);

		const char* ip = m_remoteAddr.get_host_addr();
		// Connect to the peer.

		// ʹ��sap_any���ӣ�����Ҫ����AB��IP
		// todo:���������Ҫָ������IP���������޸�
		int res = this->connect(m_pHandler, m_remoteAddr, m_options);
		//  && errno != EWOULDBLOCK
		if (res == -1)
		{
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("connect")), -1);
		}

		return 0;
	}
	
	// Destroy the client.
	virtual int Fini(void)
	{
		this->close();

		return 0;
	}

	bool ReConnect()
	{
		//ACE_ASSERT(m_pHandler);

		if (m_pHandler == nullptr)
		{
			return false;
		}

		m_pHandler->shutdown();

		int res = this->connect(m_pHandler, m_remoteAddr);
		if (res == -1)
		{				
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("����")));
			return false;
		}

		return true;
	}

	virtual	int make_svc_handler(SVC_HANDLER *&sh)
	{
		ACE_TRACE("ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>::make_svc_handler");

		if (sh == 0)
		{
			ACE_NEW_RETURN(sh, SVC_HANDLER, -1);
		}

		// Set the reactor of the newly created <SVC_HANDLER> to the same
		// reactor that this <Connector> is using.
		sh->reactor(this->reactor());
		return 0;
	}

	bool IsShutdown() const 
	{
		return m_isShutdown;
	}

public:
	SVC_HANDLER* m_pHandler;

private:

	// ���ӹ���ָ��
	 CConnectorMgr * m_pConnMgr;

	// Options for the active connection factory.
	ACE_Synch_Options m_options;

	ACE_INET_Addr m_remoteAddr;

	ACE_Reactor* m_pReactor;

	std::atomic<int>  m_nCurWorkingNet;   //! ��ǰ����������

	std::atomic<bool> m_isShutdown;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/
