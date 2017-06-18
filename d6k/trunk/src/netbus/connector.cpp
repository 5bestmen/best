/*! @file connector.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  connector.cpp
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
*  @brief   ��Ϊ�ͻ���ȥ���������ڵ�
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
		// �ͷŶ���
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
		// log ����ʧ��,��ʼ����

		// ������ʱ�����Զ�����.
		// ��ʱ1�룬����5��
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
		// �رն�ʱ��
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
	// ��Ϣ�������ó�deactivate()�Ժ���Ϣ����ȡ���ݵ�ʱ��error==ESHUTDOWN
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

	// ������Ϣ���ȼ�
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

	// ������Ϣ���ȼ�
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

	// ����Ϊ������
	if (peer().enable(ACE_NONBLOCK) == -1)
	{
		return -1;
	}

	// ���ӳɹ�֮�� LOG��־

	/*if (m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK |
		ACE_Event_Handler::CONNECT_MASK | ACE_Event_Handler::ACCEPT_MASK) == -1)
	{
		return -1;
	}*/

	//ע�����봦��ص�����  
	m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK);

	//m_pReactor->register_handler(this, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);

	/*if (msg_queue()->activate() == ACE_Message_Queue_Base::ACTIVATED)
	{
		msg_queue()->high_water_mark(HIGH_WATER);
		msg_queue()->low_water_mark(LOW_WATER);

		// �����߳�
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
		return 0; // ���ٶ���
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

	// ������Ϣʱ��ƴ���Ͽ�ʼλ1���ֽ�
	char* data = new char[length + 1];
	data[0] = ORIGIN_FLAG;
	ACE_OS::memcpy(data + 1, mb, length);
	length += 1;

	result = peer().send_n(data, length);
	if (result == -1 && bReconn)
	{
		// �������� 5 ��֮�󣬻����ɹ�����֪ͨMGR׼���л�����
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
		{ // ����ʧ��
			// todo:�ж���Ϣ�����Ƿ����������ͣ�������������;ͱ���,����ֵ�ȿ��Զ���
			// �л�����
			if (!m_pConnMgr->SwitchWorkingNet())
			{
				// log �����л�ʧ��
			}
		}
		else
		{ // �����ɹ�
			result = peer().send_n(data, length);
			if (result == -1)
			{
				// todo:�ж���Ϣ�����Ƿ����������ͣ�������������;ͱ���,����ֵ�ȿ��Զ���
				// todo:�ж��Ƿ�д����Ϣ����
			}
		}

		// ���ʹ��task�Դ�����Ϣ���У���ſ�ע��
		/*if (m_pConnector == nullptr || m_pConnector->m_pHandler == nullptr)
		{
		msg_queue()->deactivate();
		delete[] data;
		return 0;
		}*/
	}

	bool bCurState = false;
	bool bSendRes = (result == -1 ? false : true);
	// ��������״̬
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

	// MAX_ALIGNMENT����������ֵ����ֹ�����Խ��.
	ACE_OutputCDR head(ACE_CDR::MAX_ALIGNMENT + 8);
	head << ACE_CDR::ULong(ACE_CDR_BYTE_ORDER);
	head << length;

	// CDR ͷ
	iov[0].iov_base = head.begin()->rd_ptr();
	iov[0].iov_len = 8;

	// CDR ��
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

	// CDR ��
	ACE_OutputCDR body(mb->length() + ACE_CDR::MAX_ALIGNMENT);
	body << mb->base();

	ACE_CDR::ULong length = 0;
	length = ACE_Utils::truncate_cast<ACE_CDR::ULong>(body.total_length());

	// MAX_ALIGNMENT����������ֵ����ֹ�����Խ��.
	ACE_OutputCDR head(ACE_CDR::MAX_ALIGNMENT + 8);
	head << ACE_CDR::ULong(ACE_CDR_BYTE_ORDER);
	head << length;

	iovec iov[2];

	// CDR ͷ
	iov[0].iov_base = head.begin()->rd_ptr();
	iov[0].iov_len = 8;
	iov[1].iov_base = body.begin()->rd_ptr();
	iov[1].iov_len = length;
	
	int result = 0;

	while ((this->peer().sendv_n(iov, 2) == -1) && bReconn)
	{
		if (!m_pConnector->ReConnect())
		{
			// todo: SOE�¼����ܶ�. �������������ͻ������ӶϿ�����SOE��Ϣ�Ż���Ϣ�����С�
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

	// ������Ϣʱ��ƴ���Ͽ�ʼλ1���ֽ�
	result = peer().send_n(mb, length);
	if (result == -1 && bReconn)
	{
		// ���²��ɹ�����֪ͨMGR׼���л�����
		int i = 0;
		for (i = 0; i < m_reconnCnt; ++i)
		{
			if (m_pConnector->ReConnect())
			{
				break;
			}

			// 10����
			ACE_OS::sleep(ACE_Time_Value(0, 10000));
		}

		if (i == m_reconnCnt)
		{ // ����ʧ��
		  // �л�����
			if (!m_pConnMgr->SwitchWorkingNet())
			{
				// log �����л�ʧ��
			}
		}
		else
		{ // �����ɹ�
			result = peer().send_n(mb, length);
		}
	}

	bool bCurState = false;
	bool bSendRes = (result == -1 ? false : true);
	// ��������״̬
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

	// ������Ϣ��
	mb->msg_priority(priority);

	ACE_Time_Value nowait(ACE_OS::gettimeofday());

	// �� ACE_Message_Queue ����������ݿ�
	if (-1 == m_RecvQueue.enqueue_prio(mb, &nowait))
	{
		mb->release(); // �ͷ���Ϣ��

		return false;
	}

	return true;
}

bool CPeerHandler::GetRecvMsg(ACE_Message_Block *mb)
{
	// �� ACE_Message_Queue ��ȡ����Ϣ��
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
		// log (�߳�����ʧ��)
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
		// �жϵ�ǰ�����Ƿ����Լ�������
		// ������Լ������磬������Ч���ݣ���������Լ������磬����idle����
		// ������Լ������磬���mgr�Ķ����л�ȡ��Ч���ݣ����ͳ�ȥ
		if (nCurNet != m_nCurWorkingNet)
		{ // �����Լ�������
			// ��ǰ�����Լ������磬����idle���Ա���
			
			char* pData = "Test Msg";
			size_t len = strlen(pData);

			//this->send(pData, len);

			SendMsgAndReconn(pData, len);

			// 5��
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
				// �ж��Ƿ�shutdow
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

		// ����
		if (mblk != nullptr)
		{
			if (this->send(mblk->rd_ptr(), mblk->length()))
			{
				// log ����ʧ��
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
	{ // ������������

		// ������������ʱ���������ݵĳ��Ⱥ��ٴν��յĳ����Ƿ񳬹���ͷ
		if ((nLen + m_msgCache.GetCurByteLength()) < EMSG_BUF_HEAD_SIZE + 1)
		{ // û�г�����ͷ���ȣ������������

			// ��ʱ���յ��İ�С��������ͷ���������ݼ������ա�
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else
		{ // ������ͷ����

			if (m_msgCache.GetCurByteLength() < EMSG_BUF_HEAD_SIZE + 1)
			{
				// ���ư�ͷ
				// ��ͷ����ֵ
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

		if (nNeedBytes > nLen)//��Ҫ�ĳ��ȴ��ڽ��յ��ĳ���
		{
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else//���յ��ĳ��ȴ�����Ҫ�ĳ���
		{
			//������Ҫ�Ĳ���
			m_msgCache.CopyData(pData, nNeedBytes);
			pData += nNeedBytes;
			nLen -= nNeedBytes;

			m_cnt++;
			if (m_cnt % 10000 == 0)
			{
				std::cout << "����num:" << m_cnt << std::endl;
			}

			char* data = m_msgCache.GetDataBuff();
			size_t len = m_msgCache.GetDataLen();			

			EMSG_BUF* msg = (EMSG_BUF*)data;

			int nMsgType = static_cast<int>(msg->MsgType);
			if (nMsgType == SEND_SWITCH_CMD_REQ) // �յ��Զ˷��͵ģ������л����ģ�
			{
				ACE_ASSERT(m_pConnMgr != nullptr && m_pConnMgr->GetMsgQueue() != nullptr);
				if (m_pConnMgr != nullptr && m_pConnMgr->GetMsgQueue() != nullptr);
				{
					// ���յ����л���Ӧ���ķŵ���Ϣ����
					if (g_msgQueueConn.SetToSwitchMQ(data, len))
					{
						// ����ȷ��
						// �ظ���Ӧ
						EMSG_BUF msgBuf;

						// �����л�����
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
				// �����յ�����Ϣ�ŵ�������Ϣ����
				m_pConnMgr->PutMsgToRecvQueue(reinterpret_cast<int8u*>(data), len, nMsgType);
			}		

			m_msgCache.Clear();

			//����ʣ��Ĳ���
			return HandleMsg(pData, nLen);
		}
	}
	else
	{ // û����������

		//�ڽ�����Ѱ�������ַ�
		char* pNewBuff = std::find(pData, pData + nLen, ORIGIN_FLAG);
		if (pNewBuff == pData + nLen)
		{
			return false;//û���ҵ������ַ�
		}

		nLen = nLen - (pNewBuff - pData);
		pData = pNewBuff;

		//���������ַ���������
		m_msgCache.CopyData(pData, 1);
		pData += 1;
		nLen -= 1;

		return HandleMsg(pData, nLen);
	}

	return true;
}


/** @}*/


