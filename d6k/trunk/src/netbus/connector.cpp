/*! @file connector.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  connector.cpp
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
*  @brief   作为客户端去连接其他节点
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.08
*******************************************************************************/

#include "connector.h"

#include <iostream>

#include "ace/CDR_Stream.h"
#include "ace/OS.h"
#include <QObject>

#include "netbus/nbdef.h"
#include "fesapi/fesdatadef.h"

CMsgQueue& g_msgQueueConn = CMsgQueue::GetInstance();

CConnectorTask::CConnectorTask(ACE_INET_Addr& addr, CConnectorMgr* pConnMgr, unsigned int nCurWorkingNet/* = NET_A*/) 
	: m_addr(addr), m_pConnMgr(pConnMgr), m_pConnector(nullptr), m_nTimerID(-1), m_nStopState(false), m_pReactor(new ACE_Reactor())
{
	if (nCurWorkingNet > NET_B)
	{
		m_nCurWorkingNet = NET_A;
	}

	m_nCurWorkingNet = nCurWorkingNet;
}

CConnectorTask::~CConnectorTask()
{
	if (!m_nStopState)
	{
		Stop();	
	}

	if (m_pReactor != nullptr)
	{
		DeleteReactor();
	}

	if (m_pConnector != nullptr)
	{
		delete m_pConnector;
		m_pConnector = nullptr;
	}

	if (this->wait() == -1)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("unable to stop thread pool")));

		m_nStopState = true;
	}
}

bool CConnectorTask::Start(int nThreadNumber /*= 1*/)
{
	ACE_ASSERT(nThreadNumber > 0);
	ACE_ASSERT(m_pReactor);
	ACE_ASSERT(m_pConnMgr);
	if (m_pReactor == nullptr || m_pConnMgr == nullptr)
	{
		return false;
	}

	if (nThreadNumber <= 0)
	{
		nThreadNumber = 1;
	}

	if (nThreadNumber > 4)
	{
		nThreadNumber = 4;
	}

	//m_pConnector = std::make_shared<CConnector<CPeerHandler>>(m_addr, m_pReactor);
	//m_pConnMgr->GetMsgQueue()->GetMsgQueue().is_empty();
	m_pConnector = new CConnector<CPeerHandler>(m_addr, m_pReactor, m_pConnMgr, m_nCurWorkingNet);

	if (this->activate(THR_NEW_LWP, nThreadNumber) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("unable to activate thread pool")), false);
	}

	return true;
}

bool CConnectorTask::Stop(void)
{
	if (m_nStopState)
	{
		return true;
	}
	//m_pConnMgr->GetMsgQueue()->GetMsgQueue().deactivate();

	if (m_pConnector != nullptr)
	{
		m_pConnector->Shutdown();
	}

	if (m_pReactor != nullptr)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Calling End Proactor event loop\n")));

		m_pReactor->cancel_timer(m_nTimerID);

		ACE_Reactor::instance()->end_reactor_event_loop();
		
	}

	m_nStopState = true;

	return true;
}

int CConnectorTask::close(u_long flags /*= 0*/)
{
	ACE_UNUSED_ARG(flags);

	if (ACE_OS::thr_equal(ACE_Thread::self(), this->last_thread()))
	{
		// 释放对象
		//delete this;
	}

	return 0;
}

int CConnectorTask::svc()
{
	ACE_ASSERT(m_pConnector);

	m_pReactor->owner(ACE_OS::thr_self());

	ACE_Reactor::instance(m_pReactor);

	if (m_pConnector->Init() != 0)
	{
		// log 连接失败,开始重连

		// 启动定时器，自动重连.
		// 延时1秒，周期5秒
		StartTimer(1, 5);
	}

	ACE_Reactor::instance()->run_reactor_event_loop();

	return 0;
}

bool CConnectorTask::PushMsg(const int8u *pBuf, unsigned int nLen, int32u msgLevel /*= GENERAL_MSG*/)
{
	ACE_ASSERT(m_pConnector);
	ACE_ASSERT(pBuf);
	ACE_ASSERT(m_pConnector->m_pHandler);
	ACE_ASSERT(nLen > 0 && nLen < 500 * 1024);
	
	if (m_pConnector == nullptr || pBuf == nullptr || m_pConnector->m_pHandler == nullptr)
	{
		return false;
	}

	return m_pConnector->m_pHandler->PushMsg(pBuf, nLen, msgLevel);
}

bool CConnectorTask::PushMsg(ACE_Message_Block* pBlock, int32u msgLevel /*= GENERAL_MSG*/, ACE_Time_Value* pTimeout /*= nullptr*/)
{
	ACE_ASSERT(m_pConnector);
	ACE_ASSERT(pBlock);
	ACE_ASSERT(m_pConnector->m_pHandler);
	if (m_pConnector == nullptr || pBlock == nullptr || m_pConnector->m_pHandler == nullptr)
	{
		return false;
	}

	return m_pConnector->m_pHandler->PushMsg(pBlock);
}

long CConnectorTask::StartTimer(int delay, int interval, const void* arg /*= 0*/)
{
	ACE_ASSERT(m_pReactor);

	ACE_Time_Value timeDelay(delay);

	ACE_Time_Value timeInterval(interval);

	m_nTimerID = m_pReactor->schedule_timer(this, arg, timeDelay, timeInterval);

	return m_nTimerID;
}

int CConnectorTask::handle_timeout(const ACE_Time_Value &current_time, const void *act /* = 0 */)
{
	ACE_ASSERT(m_pConnector);

	if (m_pConnector->ReConnect())
	{
		// 关闭定时器
		m_pReactor->cancel_timer(m_nTimerID);
	}

	return 0;
}


int CConnectorTask::DeleteReactor(void)
{
	m_pReactor->close_singleton();

	return 0;
}

CPeerHandler::CPeerHandler()
{
	m_bSendMode = false;
	m_pConnector = nullptr;
	m_pConnMgr = nullptr;
	m_pReactor = nullptr;
	m_cnt = 0;
	m_isRunning = false;
	//m_RecvQueue.high_water_mark(HIGH_WATER);
	//m_RecvQueue.low_water_mark(LOW_WATER);
}

void CPeerHandler::InitConnMgr(ACE_Reactor* pReactor, CConnectorMgr * pConnMgr, unsigned int nCurWorkingNet/* = NET_A*/)
{
	ACE_ASSERT(pConnMgr && pReactor);

	if (nCurWorkingNet > NET_B)
	{
		m_nCurWorkingNet = NET_A;
	}

	m_nCurWorkingNet = nCurWorkingNet;

	m_pConnMgr =  pConnMgr;

	m_pReactor = pReactor;

	ACE_ASSERT(m_pReactor);
}

CPeerHandler::~CPeerHandler()
{
	// 消息队列设置成deactivate()以后，消息队列取数据的时候error==ESHUTDOWN
	GetConnectorMgr()->GetMsgQueue()->GetMsgQueue().deactivate();

	m_isRunning = false;

	if (this->wait() == -1)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("stop thread")));
	}
}

bool CPeerHandler::PushMsg(const int8u *pBuf, unsigned int nLen, int32u msgLevel/* = GENERAL_MSG*/)
{
	ACE_ASSERT(this->thr_count());

	if (this->thr_count() <= 0)
	{
		return false;
	}

	const char* pData = reinterpret_cast<const char*> (pBuf);
	ACE_Message_Block* mb = new ACE_Message_Block(nLen + 1);
	mb->copy(pData, nLen);

	// 设置消息优先级
	mb->msg_priority(msgLevel);

	int nRet = -1;
	while ((nRet = putq(mb)) == -1)
	{
		if (msg_queue()->state() != ACE_Message_Queue_Base::PULSED)
		{
			break;
		}
	}

	return nRet == -1 ? false : true;
}

bool CPeerHandler::PushMsg(ACE_Message_Block* pBlock, int32u msgLevel/* = GENERAL_MSG*/, ACE_Time_Value* pTimeout/* = nullptr*/)
{
	ACE_ASSERT(this->thr_count());
	ACE_ASSERT(pBlock);

	if (this->thr_count() <= 0)
	{
		return false;
	}

	// 设置消息优先级
	pBlock->msg_priority(msgLevel);

	int nRet = -1;
	while ((nRet = putq(pBlock, pTimeout)) == -1)
	{
		if (msg_queue()->state() != ACE_Message_Queue_Base::PULSED)
		{
			break;
		}
	}

	return nRet == -1 ? false : true;
}

int CPeerHandler::open(void *connector /*= 0*/)
{
	ACE_ASSERT(m_pReactor);
	if (m_pReactor == nullptr)
	{
		return -1;
	}

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("activating %d\n"), this->peer().get_handle()));

	m_pConnector = static_cast<CConnector<CPeerHandler>*> (connector);
	int nBufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
	peer().set_option(SOL_SOCKET, SO_SNDBUF, &nBufsize, sizeof(nBufsize));

	// 设置为非阻塞
	if (peer().enable(ACE_NONBLOCK) == -1)
	{
		return -1;
	}

	// 连接成功之后 LOG日志

	/*if (m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK |
		ACE_Event_Handler::CONNECT_MASK | ACE_Event_Handler::ACCEPT_MASK) == -1)
	{
		return -1;
	}*/

	//注册输入处理回调函数  
	m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK);

	//m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);

	/*if (msg_queue()->activate() == ACE_Message_Queue_Base::ACTIVATED)
	{
		msg_queue()->high_water_mark(HIGH_WATER);
		msg_queue()->low_water_mark(LOW_WATER);

		// 启动线程
		return activate();
	}*/

	return activate();
}

// Called on failed connection attempt.
int CPeerHandler::close(u_long flags/* = 0*/)
{
	ACE_ERROR((LM_ERROR, ACE_TEXT("CPeerHandler::close.\n")));

	/*if (flags == -1)
	{
		this->destroy();
		return 0; // 销毁对象
	}
	else
	{
		this->shutdown();

		if (this->thr_count() <= 0)
		{
			msg_queue()->deactivate();
		}
	}*/
	peer().close();
	return 0;
}

// SetMsgToRecvQueue
int CPeerHandler::handle_input(ACE_HANDLE  fd /*= ACE_INVALID_HANDLE*/)
{
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("in handle_input\n")));

	//ACE_Message_Block* mb = new ACE_Message_Block(ACE_DEFAULT_CDR_BUFSIZE + 1);

	char m_recvData[ACE_DEFAULT_CDR_BUFSIZE + 1];
	ssize_t size = this->peer().recv(m_recvData, ACE_DEFAULT_CDR_BUFSIZE);
	if (size <= 0)
	{
		return 0;
	}

	m_recvData[size] = '\0';

	HandleMsg(m_recvData, size);

	return 0;
}

int CPeerHandler::handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask)
{
	return ACE_OS::closesocket(h);
}

int32s CPeerHandler::send(const void *mb, size_t length, bool bReconn /*= true*/)
{
	Q_ASSERT(mb);

	int32s result = 0;

	// 发送消息时，拼接上开始位1个字节
	char* data = new char[length + 1];
	data[0] = ORIGIN_FLAG;
	ACE_OS::memcpy(data + 1, mb, length);
	length += 1;

	result = peer().send_n(data, length);
	if (result == -1 && bReconn)
	{
		// 重新连接 5 次之后，还不成功，则通知MGR准备切换网络
		int i = 0;
		for (i = 0; i < m_reconnCnt; ++i)
		{
			if (m_pConnector->ReConnect())
			{
				break;
			}

			ACE_OS::sleep(1);
		}

		if (i == m_reconnCnt)
		{ // 重连失败
			// todo:判断消息类型是否是事项类型，如果是事项类型就保存,测量值等可以丢弃
			// 切换网络
			if (!m_pConnMgr->SwitchWorkingNet())
			{
				// log 网络切换失败
			}
		}
		else
		{ // 重连成功
			result = peer().send_n(data, length);
			if (result == -1)
			{
				// todo:判断消息类型是否是事项类型，如果是事项类型就保存,测量值等可以丢弃
				// todo:判断是否写回消息队列
			}
		}

		// 如果使用task自带的消息队列，则放开注释
		/*if (m_pConnector == nullptr || m_pConnector->m_pHandler == nullptr)
		{
		msg_queue()->deactivate();
		delete[] data;
		return 0;
		}*/
	}

	bool bCurState = false;
	bool bSendRes = (result == -1 ? false : true);
	// 设置网络状态
	switch (m_nCurWorkingNet)
	{
		case NET_A:
		{
			bCurState = m_pConnMgr->Get_A_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_A_NetState(bSendRes);
			}
			
			break;
		}
		case NET_B:
		{
			bCurState = m_pConnMgr->Get_B_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_B_NetState(bSendRes);
			}
			break;
		}
		default:
		{
			m_nCurWorkingNet = NET_A;
			m_pConnMgr->SwitchWorkingNet(NET_A);
			bCurState = m_pConnMgr->Get_A_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_A_NetState(bSendRes);
			}
			break;
		}
	}

	/*while (1)
	{
		for (int32s i = 0; i < 1000000; ++i)
		{
			if (m_pConnector == nullptr || m_pConnector->m_handler == nullptr)
			{
				msg_queue()->deactivate();
				delete[] data;
				return 0;
			}

			result = peer().send_n(data, length);
			if (result == -1 && bReconn)
			{
				if (m_pConnector == nullptr ||m_pConnector->m_handler == nullptr)
				{
					msg_queue()->deactivate();
					delete[] data;
					return 0;
				}

				while (m_pConnector->ReConnect())
				{
					result = peer().send_n(data, length);
					break;
				}
			}
		}

		break;
	}*/

	/*m_cnt++;
	if (m_cnt % 1000000 == 0)
	{
		m_cnt = 0;
	}*/

	delete[] data;

	return result;
}

int CPeerHandler::send(ACE_Message_Block *chunk[], size_t &count, bool bReconn/* = true*/)
{
	if (count >= ACE_IOV_MAX)
	{
		count = ACE_IOV_MAX - 1;
	}
	
	iovec iov[ACE_IOV_MAX];
	size_t iov_size;
	int result = 0;

	ACE_CDR::ULong length = 0;
	for (iov_size = 0; iov_size < count; ++iov_size)
	{
		if (nullptr == chunk[iov_size])
		{
			break;
		}

		length += ACE_Utils::truncate_cast<ACE_CDR::ULong>(chunk[iov_size]->length());
	}

	if (length <= 0)
	{
		return -1;
	}

	// MAX_ALIGNMENT：最大的冗余值，防止对齐后越界.
	ACE_OutputCDR head(ACE_CDR::MAX_ALIGNMENT + 8);
	head << ACE_CDR::ULong(ACE_CDR_BYTE_ORDER);
	head << length;

	// CDR 头
	iov[0].iov_base = head.begin()->rd_ptr();
	iov[0].iov_len = 8;

	// CDR 体
	for (iov_size = 1; iov_size <= count; ++iov_size)
	{
		if (nullptr == chunk[iov_size])
		{
			break;
		}

		iov[iov_size].iov_base = chunk[iov_size]->rd_ptr();
		iov[iov_size].iov_len = ACE_Utils::truncate_cast<ACE_CDR::ULong> (chunk[iov_size]->length());
	}

	while ((peer().sendv_n(iov, ACE_Utils::truncate_cast<int> (iov_size)) == -1) && bReconn)
	{
		if (!m_pConnector->ReConnect())
		{
			result = -1;
			break;
		}
	}

	while (iov_size > 1)
	{
		chunk[--iov_size]->release();
		chunk[iov_size] = 0;
	}

	count = iov_size + 1;

	return result;
}

int CPeerHandler::send(ACE_Message_Block *mb, bool bReconn /*= true*/)
{
	Q_ASSERT(mb);

	ACE_Time_Value nowait(ACE_OS::gettimeofday());	

	// CDR 体
	ACE_OutputCDR body(mb->length() + ACE_CDR::MAX_ALIGNMENT);
	body << mb->base();

	ACE_CDR::ULong length = 0;
	length = ACE_Utils::truncate_cast<ACE_CDR::ULong>(body.total_length());

	// MAX_ALIGNMENT：最大的冗余值，防止对齐后越界.
	ACE_OutputCDR head(ACE_CDR::MAX_ALIGNMENT + 8);
	head << ACE_CDR::ULong(ACE_CDR_BYTE_ORDER);
	head << length;

	iovec iov[2];

	// CDR 头
	iov[0].iov_base = head.begin()->rd_ptr();
	iov[0].iov_len = 8;
	iov[1].iov_base = body.begin()->rd_ptr();
	iov[1].iov_len = length;
	
	int result = 0;

	while ((this->peer().sendv_n(iov, 2) == -1) && bReconn)
	{
		if (!m_pConnector->ReConnect())
		{
			// todo: SOE事件不能丢. 后续新增，发送或者连接断开。把SOE消息放回消息队列中。
			//this->ungetq(mb);

			result = -1;
			break;
		}
	}	

	return result;
}

int32s CPeerHandler::SendMsgAndReconn(const void *mb, size_t length, bool bReconn/* = true*/)
{
	Q_ASSERT(mb && length > 0);

	int32s result = 0;

	// 发送消息时，拼接上开始位1个字节
	result = peer().send_n(mb, length);
	if (result == -1 && bReconn)
	{
		// 重新不成功，则通知MGR准备切换网络
		int i = 0;
		for (i = 0; i < m_reconnCnt; ++i)
		{
			if (m_pConnector->ReConnect())
			{
				break;
			}

			// 10毫秒
			ACE_OS::sleep(ACE_Time_Value(0, 10000));
		}

		if (i == m_reconnCnt)
		{ // 重连失败
		  // 切换网络
			if (!m_pConnMgr->SwitchWorkingNet())
			{
				// log 网络切换失败
			}
		}
		else
		{ // 重连成功
			result = peer().send_n(mb, length);
		}
	}

	bool bCurState = false;
	bool bSendRes = (result == -1 ? false : true);
	// 设置网络状态
	switch (m_nCurWorkingNet)
	{
		case NET_A:
		{
			bCurState = m_pConnMgr->Get_A_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_A_NetState(bSendRes);
			}

			break;
		}
		case NET_B:
		{
			bCurState = m_pConnMgr->Get_B_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_B_NetState(bSendRes);
			}
			break;
		}
		default:
		{
			m_nCurWorkingNet = NET_A;
			m_pConnMgr->SwitchWorkingNet(NET_A);
			bCurState = m_pConnMgr->Get_A_NetState();
			if (!(bCurState && bSendRes))
			{
				m_pConnMgr->Set_A_NetState(bSendRes);
			}
			break;
		}
	}

	return result;
}

bool CPeerHandler::SetRecvMsg(ACE_Message_Block *mb, int priority/* = GENERAL_MSG*/)
{
	Q_ASSERT(mb);

	if (m_RecvQueue.is_full())
	{
		ACE_DEBUG((LM_DEBUG, "SetRecvMsg is full !!!\n"));
		ACE_OS::sleep(1);

		return false;
	}

	// 构造消息块
	mb->msg_priority(priority);

	ACE_Time_Value nowait(ACE_OS::gettimeofday());

	// 向 ACE_Message_Queue 中添加新数据块
	if (-1 == m_RecvQueue.enqueue_prio(mb, &nowait))
	{
		mb->release(); // 释放消息块

		return false;
	}

	return true;
}

bool CPeerHandler::GetRecvMsg(ACE_Message_Block *mb)
{
	// 从 ACE_Message_Queue 中取出消息块
	if (m_RecvQueue.dequeue_head(mb) != -1)
	{
		mb->release();
		return true;
	}

	return false;
}

int CPeerHandler::GetFromMsgQueue(ACE_Message_Block *&mb, ACE_Time_Value *tv/* = 0*/)
{
	return (GetConnectorMgr())->GetMsgFromSendQueue(mb, tv);
}

 CConnectorMgr * CPeerHandler::GetConnectorMgr()
{
	return m_pConnMgr;
}

int CPeerHandler::svc()
{
	ACE_ASSERT(m_pConnMgr);
	if (m_pConnMgr == nullptr)
	{
		// log (线程启动失败)
		return -1;
	}
	ACE_Message_Block *chunk[ACE_IOV_MAX];
	size_t message_index = 0;
	ACE_Time_Value time_of_last_send(ACE_OS::gettimeofday());
	ACE_Time_Value timeout;
	ACE_Sig_Action no_sigpipe((ACE_SignalHandler)SIG_IGN);
	ACE_Sig_Action original_action;
	no_sigpipe.register_action(SIGPIPE, &original_action);

	//std::shared_ptr<CConnectorMgr>& pConnMgr = GetConnectorMgr();

	m_isRunning = true;
	int nCurNet = -1;
	while (1)
	{
		nCurNet = m_pConnMgr->GetCurrentWorkingNet();
		// 判断当前网络是否是自己的网络
		// 如果是自己的网络，则发送有效数据，如果不是自己的网络，则发送idle报文
		// 如果是自己的网络，则从mgr的队列中获取有效数据，发送出去
		if (nCurNet != m_nCurWorkingNet)
		{ // 不是自己的网络
			// 当前不是自己的网络，发送idle测试报文
			
			char* pData = "Test Msg";
			size_t len = strlen(pData);

			//this->send(pData, len);

			SendMsgAndReconn(pData, len);

			// 5秒
			ACE_OS::sleep(5);

			continue;
		}

		ACE_Message_Block *mblk = nullptr;
		ACE_Time_Value timeout = ACE_OS::gettimeofday();

		timeout += ACE_Time_Value(1); // 1s
		int nRes = GetFromMsgQueue(mblk, &timeout);
		switch (nRes)
		{
			case -2:
			{
				break;
			}
			case -1:
			{
				if (!m_isRunning)
				{
					return -3;
				}
				// 判断是否shutdow
				if (errno == ESHUTDOWN)
				{
					return 0;
				}
				else if (errno != EWOULDBLOCK)
				{
					ACE_ERROR((LM_ERROR, ACE_TEXT("(%P|%t) %p\n"), ACE_TEXT("EWOULDBLOCK")));
					if (mblk != nullptr)
					{
						mblk->release();
						mblk = nullptr;
					}
					
					continue;
				}
				else
				{
					continue;
				}
				break;
			}
			case 0:
			{
				if (mblk->size() == 0 && mblk->msg_type() == ACE_Message_Block::MB_STOP)
				{
					mblk->release();
					break;
				}
				break;
			}
			default:
			{
				continue;
			}
		}

		// 发送
		if (mblk != nullptr)
		{
			if (this->send(mblk->rd_ptr(), mblk->length()))
			{
				// log 发送失败
			}

			mblk->release();
		}		
	}

	no_sigpipe.restore_action(SIGPIPE, original_action);

	return 0;
}

bool CPeerHandler::HandleMsg(char* pData, size_t nLen)
{
	if (pData == nullptr || nLen <= 0)
	{
		return false;
	}

	if (!m_msgCache.IsEmpty())
	{ // 存在冗余数据

		// 存在冗余数据时，冗余数据的长度和再次接收的长度是否超过包头
		if ((nLen + m_msgCache.GetCurByteLength()) < EMSG_BUF_HEAD_SIZE + 1)
		{ // 没有超过包头长度，保存继续接收

			// 此时接收到的包小于整个包头，保存数据继续接收。
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else
		{ // 超过包头长度

			if (m_msgCache.GetCurByteLength() < EMSG_BUF_HEAD_SIZE + 1)
			{
				// 复制包头
				// 包头冗余值
				size_t nHeadSize = (EMSG_BUF_HEAD_SIZE + 1) - m_msgCache.GetCurByteLength();
				m_msgCache.CopyData(pData, nHeadSize);

				pData += nHeadSize;
				nLen -= nHeadSize;
			}
		}

		size_t nNeedBytes = m_msgCache.GetNeedByteLength();
		if (nNeedBytes <= 0 || nNeedBytes > MAX_EMSG_L)
		{
			m_msgCache.Clear();
			return false;
		}

		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_msgCache.CopyData(pData, nNeedBytes);
			pData += nNeedBytes;
			nLen -= nNeedBytes;

			m_cnt++;
			if (m_cnt % 10000 == 0)
			{
				std::cout << "接收num:" << m_cnt << std::endl;
			}

			char* data = m_msgCache.GetDataBuff();
			size_t len = m_msgCache.GetDataLen();			

			EMSG_BUF* msg = (EMSG_BUF*)data;

			int nMsgType = static_cast<int>(msg->MsgType);
			if (nMsgType == SEND_SWITCH_CMD_REQ) // 收到对端发送的（发送切换报文）
			{
				ACE_ASSERT(m_pConnMgr != nullptr && m_pConnMgr->GetMsgQueue() != nullptr);
				if (m_pConnMgr != nullptr && m_pConnMgr->GetMsgQueue() != nullptr);
				{
					// 将收到的切换响应报文放到消息队列
					if (g_msgQueueConn.SetToSwitchMQ(data, len))
					{
						// 发送确认
						// 回复响应
						EMSG_BUF msgBuf;

						// 发送切换报文
						msgBuf.MsgType = RECV_SWITCH_CMD_RES;

						const char* data = "switch res";
						int nLen = strlen(data);
						msgBuf.MsgDataSize = EMSG_BUF_HEAD_SIZE + nLen;

						memset(msgBuf.BuffData, 0, nLen + 1);

						memcpy(msgBuf.BuffData, &data, nLen);

						SendMsgAndReconn((void*)&msgBuf, msgBuf.MsgDataSize);
					}					
				}				
			}
			else
			{
				// 将接收到的消息放到接收消息队列
				m_pConnMgr->PutMsgToRecvQueue(reinterpret_cast<int8u*>(data), len, nMsgType);
			}		

			m_msgCache.Clear();

			//解析剩余的部分
			return HandleMsg(pData, nLen);
		}
	}
	else
	{ // 没有冗余数据

		//在接收区寻找启动字符
		char* pNewBuff = std::find(pData, pData + nLen, ORIGIN_FLAG);
		if (pNewBuff == pData + nLen)
		{
			return false;//没有找到启动字符
		}

		nLen = nLen - (pNewBuff - pData);
		pData = pNewBuff;

		//复制启动字符至缓冲区
		m_msgCache.CopyData(pData, 1);
		pData += 1;
		nLen -= 1;

		return HandleMsg(pData, nLen);
	}

	return true;
}


/** @}*/


