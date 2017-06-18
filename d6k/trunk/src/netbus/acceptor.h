/*! @file acceptor.h
<PRE>
********************************************************************************
模块名       :
文件名       :  acceptor.h
文件实现功能 :  作为服务端等待客户端的连接
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   作为服务端等待客户端的连接
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.25
*******************************************************************************/
#ifndef ACCEPTOR_MODULE_H
#define ACCEPTOR_MODULE_H

#include <atomic>
 
#include "ace/Task.h"
#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/Asynch_IO.h"
#include "ace/Service_Object.h"
#include "ace/Asynch_IO.h"
#include "ace/Thread_Semaphore.h"

#include "netbus/nbdef.h"

#include "msg_cache.h"

#include "node_manager.h"

const int32u MAX_SERVERS = 1000;

static int DisableSignal(int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
	sigset_t signal_set;
	if (ACE_OS::sigemptyset(&signal_set) == -1)
		ACE_ERROR((LM_ERROR,
		ACE_TEXT("Error: (%P|%t):%p\n"),
		ACE_TEXT("sigemptyset failed")));

	for (int i = sigmin; i <= sigmax; i++)
		ACE_OS::sigaddset(&signal_set, i);

	// Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
	// In multi-threaded application this is not POSIX compliant
	// but let's leave it just in case.
	if (ACE_OS::sigprocmask(SIG_BLOCK, &signal_set, 0) != 0)
# else
	if (ACE_OS::thr_sigsetmask(SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
		ACE_ERROR_RETURN((LM_ERROR,
		ACE_TEXT("Error: (%P|%t): %p\n"),
		ACE_TEXT("SIG_BLOCK failed")),
		-1);
#else
	ACE_UNUSED_ARG(sigmin);
	ACE_UNUSED_ARG(sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

	return 0;
}


class CProactiveService : public ACE_Service_Handler
{
public:
	CProactiveService();

	CProactiveService(int nID);

	virtual ~CProactiveService();

	// 当有客户端连接上来，连接建立成功后Proactor会调用这个方法
	virtual void open(ACE_HANDLE h, ACE_Message_Block&);

	// This method will be called when an asynchronous read  completes on a stream.
	virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result &result);

	// This method will be called when an asynchronous write completes on a stream.
	virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result &result);

protected:

	// 保存连接上来的节点信息
	void BindNodeInfo(const char* ip);

	void UnbindNodeInfo(const char* ip);

private:
	int Initiate_Read_Stream(void);

	int Initiate_Write_Stream(ACE_Message_Block &mb, size_t nbytes);

	// 处理消息
	int HandleMsg(ACE_Message_Block* mb);

	// 解析消息
	bool HandleMsg(char* pData, size_t nLen);

	// 处理主备切换命令
	bool HandleSwitch(char* pData, size_t nLen);

private:
	ACE_Asynch_Read_Stream  m_asynchReader;
	ACE_Asynch_Write_Stream m_asynchWriter;

	ACE_INET_Addr m_addr;

	// 计数
	long m_cnt;

	std::string m_strRemainFlag;
	//char m_bufRemainData[2];

	// 消息冗余处理类
	CMsgCache m_msgCache;

	// ACE句柄
	ACE_HANDLE m_handle;

	size_t m_size;
	// 服务ID
	int m_nServerID;
	ACE_SYNCH_MUTEX m_synchMutex;
};

class CAcceptorTask : public ACE_Task<ACE_MT_SYNCH>
{
public:

	CAcceptorTask(ACE_INET_Addr& addr);

	~CAcceptorTask();

	// 启动proactor模式
	int Start(int numThreads = 1);

	// 停止proactor模式
	int Stop(void);

	virtual int close(u_long flags = 0);

	virtual int svc();

private:

	// 创建proactor模式
	int CreateProactor(void);

	// 删除proactor模式
	int DeleteProactor(void);

private:

	//! stop状态
	std::atomic<bool>  m_nStopState;

	ACE_SYNCH_RECURSIVE_MUTEX m_lock;
	ACE_Thread_Semaphore m_sem;

	// ACE proactor模式
	ACE_Proactor* m_pProactor;

	ACE_INET_Addr m_addrAcceptor;
	ACE_Asynch_Acceptor<CProactiveService> m_acceptor;
}; 


// todo:后续尝试多个服务
/*class CProactiveAcceptor : public ACE_Asynch_Acceptor<CProactiveService>
{
public:
	CProactiveAcceptor();
	virtual ~CProactiveAcceptor(void);

	// Virtual from ACE_Asynch_Acceptor  
	CProactiveService* make_handler(void);

private:

	ACE_SYNCH_MUTEX m_synchMutex;
	CProactiveService* m_serverList[100];	
};*/



#endif

/** @}*/
