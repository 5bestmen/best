/*! @file connector.h
<PRE>
********************************************************************************
模块名       :
文件名       :  connector.h
文件实现功能 :  作为客户端去连接其他节点
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   作为客户端去连接其他节点网络状态判定
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

	// 启动reactor模式
	bool Start(int numThreads = 1);

	// 停止proactor模式
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

	// 定时器ID
	long m_nTimerID;

	// 重连状态
	//std::atomic<bool> m_bReconnState;

	// 启动定时器,delay:延迟时间, interval：定时周期, arg:入参
	// 返回 -1 失败
	long StartTimer(int delay, int interval, const void* arg = 0);

	// 定时器回调函数
	virtual int handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */);

private:
	// 删除reactor模式
	int DeleteReactor(void);

private:

	ACE_INET_Addr m_addr;

	CConnector<CPeerHandler>* m_pConnector;

	ACE_Reactor* m_pReactor;
	ACE_SYNCH_RECURSIVE_MUTEX m_lock;

	std::atomic<bool>  m_nStopState;   //! stop状态

	CConnectorMgr *m_pConnMgr;
	//ACE_Thread_Semaphore m_sem;

	std::atomic<int>  m_nCurWorkingNet;   //! 当前工作的网络

};

class CPeerHandler : public ACE_Svc_Handler<ACE_SOCK_Stream, ACE_SYNCH>
{
	enum
	{
		HIGH_WATER = 1024 * 1024 * ACE_IOV_MAX, // 高水位
		LOW_WATER = 1024 * ACE_IOV_MAX			// 低水位
	};

	// 重连次数
	static const int m_reconnCnt = 3;

	const int32u MAX_RECV_LEN = ACE_DEFAULT_MAX_SOCKET_BUFSIZ - ACE_CDR::MAX_ALIGNMENT - 8;

public:

	CPeerHandler();
	void InitConnMgr(ACE_Reactor* pReactor, CConnectorMgr * pConnMgr, unsigned int nCurWorkingNet = NET_A);

	~CPeerHandler();

	/*! \fn int PushMsg(const unsigned char *pBuf, unsigned int nLen, bool bReconn = true);
	*********************************************************************************************************
	** \brief PushMsg
	** \details 把消息放到发送消息队列中
	** \param pBuf 消息内容
	** \param nLen 消息长度
	** \author ZhuYongjin
	** \date 2016年12月16日
	** \note
	********************************************************************************************************/
	bool PushMsg(const int8u *pBuf, unsigned int nLen, int32u msgLevel = GENERAL_MSG);

	/*! \fn int PushMsg(ACE_Message_Block* pBlock, ACE_Time_Value* pTimeout = nullptr);
	*********************************************************************************************************
	** \brief PushMsg
	** \details 把消息放到发送消息队列中
	** \param pBlock 消息内容
	** \param pTimeout 超时时间
	** \author ZhuYongjin
	** \date 2016年12月16日
	** \note
	********************************************************************************************************/
	bool PushMsg(ACE_Message_Block* pBlock, int32u msgLevel = GENERAL_MSG, ACE_Time_Value* pTimeout = nullptr);

	// Activate the handler when connection is established.
	virtual int open(void *connector = 0);

	// 线程执行函数
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

	// 发送单个消息
	int32s send(const void *mb, size_t length, bool bReconn = true);

	// 发送单个消息
	int send(ACE_Message_Block *mb, bool bReconn = true);

	// 发送块最多ACE_IOV_MAX（64个块）
	int send(ACE_Message_Block *chunk[], size_t &count, bool bReconn = true);

	// 发送消息和重连
	int32s SendMsgAndReconn(const void *mb, size_t length, bool bReconn = true);

	// 解析消息
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

	// 保存接收到的消息
	bool SetRecvMsg(ACE_Message_Block *mb, int priority = GENERAL_MSG);

	// 获取接收到的消息
	bool GetRecvMsg(ACE_Message_Block *mb);

public:
	// 发送模式
	std::atomic<bool> m_bSendMode;   // true：批量发送,false:单个发送
	CConnector<CPeerHandler>* m_pConnector;

private:

	// 从socket收到数据之后，扔到这里来
	ACE_Message_Queue<ACE_MT_SYNCH> m_RecvQueue;

	// 需要发送出去的数据暂存在这个队列
	//ACE_Message_Queue<ACE_MT_SYNCH> m_SendQueue;

	// 连接管理指针
	 CConnectorMgr*  m_pConnMgr;

	 // 消息冗余处理类
	 CMsgCache m_msgCache;
	
	 //! 当前工作的网络
	std::atomic<int>  m_nCurWorkingNet;   

	// 接收到包的数量
	size_t m_cnt;

	// 线程ID
	ACE_thread_t  m_threadsID[0x01]; 

	// 定时器ID
	long m_nTimerID;

	// 定时器Reactor
	ACE_Reactor* m_pReactor;

	// 是否运行
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
		// option默认5秒超时
		//ACE_Time_Value timeout(ACE_DEFAULT_TIMEOUT);		
		//this->m_options.set(ACE_Synch_Options::USE_REACTOR, timeout);

		// 没有超时
		//m_options.set(ACE_Synch_Options::USE_REACTOR);
	
		//m_pConnMgr->GetMsgQueue()->GetMsgQueue().is_empty();// .GetMsgQueue().is_empty();

		//ACE_NEW_RETURN(m_handler, SVC_HANDLER(m_pReactor), -1);
		ACE_NEW_RETURN(m_pHandler, SVC_HANDLER(), -1);
		m_pHandler->InitConnMgr(m_pReactor, m_pConnMgr, m_nCurWorkingNet);
		//m_handler = std::make_shared(m_pReactor);

		const char* ip = m_remoteAddr.get_host_addr();
		// Connect to the peer.

		// 使用sap_any连接，不需要本地AB网IP
		// todo:后续如果需要指定本地IP连接再做修改
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
			ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("重连")));
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

	// 连接管理指针
	 CConnectorMgr * m_pConnMgr;

	// Options for the active connection factory.
	ACE_Synch_Options m_options;

	ACE_INET_Addr m_remoteAddr;

	ACE_Reactor* m_pReactor;

	std::atomic<int>  m_nCurWorkingNet;   //! 当前工作的网络

	std::atomic<bool> m_isShutdown;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/
