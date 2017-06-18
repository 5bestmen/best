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
  
#include "datatypes.h"

#include "ace/Task.h"
#include "ace/Proactor.h"
#include "ace/Connector.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"

#include "ace/Reactor_Notification_Strategy.h"

template <typename PEER_STREAM>
class CPeerHandler : public ACE_Svc_Handler<PEER_STREAM, ACE_SYNCH>
{
	// = TITLE
	//   Handles communication with the server.
	// = DESCRIPTION
	//   This class uses a very clever state machine pattern to keep
	//   track of how it interacts with the user and the server.
public:
	CPeerHandler(ACE_Reactor *pReactor = 0) :m_pAction((&CPeerHandler<PEER_STREAM>::uninitialized))
	{
		this->reactor(pReactor);
	}
	// Activate the handler when connection is established.
	virtual int open(void *p = 0)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("activating %d\n"), 	this->peer().get_handle()));

	//	this->m_pAction = &CPeerHandler<PEER_STREAM>::connected;

		return 0;
	}
	// Called on failed connection attempt.
	virtual int close(u_long flags = 0)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("Connect not successful: ending reactor event loop\n")));
		this->reactor()->end_reactor_event_loop();
		return 0;
	}
	
	// = Demultiplexing hooks.
	virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("in handle_output\n")));

		ACE_Message_Block *mb = 0;
		ACE_Time_Value nowait(ACE_OS::gettimeofday());
		while (-1 != this->getq(mb, &nowait))
		{
			ssize_t send_cnt =	this->peer().send(mb->rd_ptr(), mb->length());
			if (send_cnt == -1)
				ACE_ERROR((LM_ERROR,	ACE_TEXT("(%P|%t) %p\n"),	ACE_TEXT("send")));
			else
				mb->rd_ptr(static_cast<size_t> (send_cnt));

			if (mb->length() > 0)
			{
				this->ungetq(mb);
				break;
			}
			mb->release();
		}
		if (this->msg_queue()->is_empty())
			this->reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);
		else
			this->reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);

		return 0;

		return (this->*m_pAction) ();
	}

	virtual int handle_input(ACE_HANDLE  fd = ACE_INVALID_HANDLE)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("in handle_input\n")));

		char buf[64] = { 0 };
		ssize_t recv_cnt = this->peer().recv(buf, sizeof(buf) - 1);
		if (recv_cnt > 0)
		{
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("%.*C"),	static_cast<int> (recv_cnt),buf));
			return 0;
		}

// 		if (recv_cnt == 0 || ACE_OS::last_error() != EWOULDBLOCK)
// 		{
// 			this->reactor()->end_reactor_event_loop();
// 			return -1;
// 		}
		return 0;

		return (this->*m_pAction) ();
	}
	virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("closing down handle %d with mask %d\n"), h,mask));

		if (this->m_pAction == &CPeerHandler<PEER_STREAM>::stdio)
		{
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("moving to closed state\n")));
			this->reactor()->end_reactor_event_loop();
		}
		else
		{
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("moving to stdio state\n")));
			this->m_pAction = &CPeerHandler<PEER_STREAM>::stdio;
			this->peer().close();
			
			if (this->reactor())
			{

			}
			else
				delete this;
		}

		return 0;
	}
	virtual int handle_signal(int signum,siginfo_t * = 0,	ucontext_t * = 0)
	{
		// @@ Note that this code is not portable to all OS platforms since
		// it uses print statements within signal handler context.
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("in handle_signal\n")));

		return (this->*m_pAction) ();
	}
	virtual int handle_timeout(const ACE_Time_Value &time, const void *)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("Connect timedout. ")));
		return this->close();
	}
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

	int (CPeerHandler<PEER_STREAM>::*m_pAction) (void);
	// Keeps track of which state we are in.
};

template <typename SVC_HANDLER, typename PEER_CONNECTOR>
class CConnector : public ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>
{
	// = TITLE
	//   This class illustrates how the <ACE_Connector> works.
public:
	// = Initialization and termination methods.
	// Constructor.
	CConnector(void) //: done_handler_(ACE_Sig_Handler_Ex(ACE_Reactor::end_event_loop))
	{

	}
	// Destructor.
	~CConnector(void)
	{

	}

	// = Dynamic linking hooks.
	// Initialize the IPC client.
	virtual int init(int argc, ACE_TCHAR *argv[])
	{
		this->inherited::open(ACE_Reactor::instance());

		ACE_Reactor *pTempReactor1 = ACE_Reactor::instance();
		ACE_Reactor *pTempReactor2 = this->reactor();


		const ACE_TCHAR *r_addr = argc > 1 ? argv[1] :	ACE_SERVER_ADDRESS(ACE_DEFAULT_SERVER_HOST, "20002");
		ACE_Time_Value timeout(argc > 2 ? ACE_OS::atoi(argv[2]) : ACE_DEFAULT_TIMEOUT);

		typename PEER_CONNECTOR::PEER_ADDR remote_addr(r_addr);
		
		this->m_options.set(ACE_Synch_Options::USE_REACTOR, timeout);
	
		SVC_HANDLER *pSvcHandler = nullptr;
		ACE_NEW_RETURN(pSvcHandler,  SVC_HANDLER(this->reactor()),	-1);

		// Connect to the peer.
		if (this->connect(pSvcHandler, remote_addr, this->m_options) == -1	&& errno != EWOULDBLOCK)
		{
			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("connect")), -1);
		}
	//	pSvcHandler->peer();

// 		else
// 			return 0;
		ACE_Reactor::instance()->register_handler(pSvcHandler, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::CONNECT_MASK);

// 		if (ACE_Reactor::instance()->register_handler(pSvcHandler, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK | ACE_Event_Handler::CONNECT_MASK) == -1)
// 			ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p\n"), ACE_TEXT("register_handler")), -1);

	}
	
	// Destroy the client.
	virtual int fini(void)
	{
		return 0;
	}
#if 0
	// Run the svc.
	virtual int svc(void)
	{
		if (this->reactor())
			this->reactor()->run_reactor_event_loop();

		return 0;
	}
#endif

private:
	typedef ACE_Connector<SVC_HANDLER, PEER_CONNECTOR>	inherited;

	// Options for the active connection factory.
	ACE_Synch_Options m_options;
};


#if 0
/*! \struct  CConnectorMgr
*   \brief 客户端连接器，一个客户端一个线程 */
class CConnectorMgr : public ACE_Task_Base
{
public:
	virtual int svc(void)
	{
		return 0;
	}

	~CConnector()
	{
	}

	virtual int close(u_long flags = 0)
	{
		if (ACE_OS::thr_equal(ACE_Thread::self(), this->last_thread()))
		{
			// 释放对象
			delete this;
		}

		return 0;
	}
protected:

public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
public:

protected:
	 
	CClient<CPeerHandler<ACE_SOCK_Stream>, ACE_SOCK_CONNECTOR> peer_connector;

	ACE_SOCK_Connector m_connector;
	ACE_SOCK_Stream m_peer;
};
#endif

#endif // FESNODE_SVC_MODULE_H


/** @}*/