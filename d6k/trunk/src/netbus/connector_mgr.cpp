/*! @file connector_mgr.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  connector_mgr.cpp
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

#include "connector_mgr.h" 

#include <QObject>


CConnectorMgr::CConnectorMgr(ACE_INET_Addr addr, CNodeConfig* pNodeConfig) : m_addrA(addr), m_pNodeConfig(pNodeConfig)
{
	ACE_ASSERT(m_pNodeConfig);

	m_bIsDoubleNet = false;
	m_bShutdown = false;
	m_pConnTaskA = nullptr;
	m_pConnTaskB = nullptr;

	m_nCurWorkingNet = NET_A;

	m_pRecvMsgQueue = new CMsgQueueLite();

	m_pSendMsgQueue = new CMsgQueueLite();
}

CConnectorMgr::CConnectorMgr(bool bDoubleNet, std::vector <ACE_INET_Addr> & vecAddress, CNodeConfig* pNodeConfig)
	: m_arrAddress(vecAddress), m_pNodeConfig(pNodeConfig)
{
	ACE_ASSERT(m_arrAddress.size() >= 2);
	ACE_ASSERT(m_pNodeConfig);

	m_bIsDoubleNet = bDoubleNet;

	m_pConnTaskA = nullptr;
	m_pConnTaskB = nullptr;

	m_nCurWorkingNet = NET_A;

	if (m_arrAddress.size()>=1)
	{
		m_addrA = m_arrAddress[0];
	}
	if (m_arrAddress.size()>=2)
	{
		m_addrB = m_arrAddress[1];
	}

	m_pRecvMsgQueue = new CMsgQueueLite();

	m_pSendMsgQueue = new CMsgQueueLite();
}

CConnectorMgr::~CConnectorMgr()
{
	if (!m_bShutdown)
	{
		m_bShutdown = true;
		Shutdown();
	}

	if (m_pRecvMsgQueue != nullptr)
	{
		delete m_pRecvMsgQueue;
		m_pRecvMsgQueue = nullptr;
	}

	if (m_pSendMsgQueue != nullptr)
	{
		delete m_pSendMsgQueue;
		m_pSendMsgQueue = nullptr;
	}

	if (m_pConnTaskA != nullptr)
	{
		delete m_pConnTaskA;
		m_pConnTaskA = nullptr;
	}

	if (m_pConnTaskB != nullptr)
	{
		delete m_pConnTaskB;
		m_pConnTaskB = nullptr;
	}
}

bool CConnectorMgr::Initialize()
{
	if (m_arrAddress.size() == 0)
	{
		m_bIsDoubleNet = false;
	}

	if (m_bIsDoubleNet)
	{ // 双网
		if (m_arrAddress.size() > 1)
		{
			m_pConnTaskA = new CConnectorTask(m_arrAddress[0], this, NET_A);

			m_pConnTaskB = new CConnectorTask(m_arrAddress[1], this, NET_B);
		}
		else
		{
			m_pConnTaskA = new CConnectorTask(m_arrAddress[0], this, NET_A);

			m_bIsDoubleNet = false;
		}
	}
	else
	{
		m_pConnTaskA = new CConnectorTask(m_addrA, this, NET_A);
	}

	// 初始化connector
	return Run();
}

bool CConnectorMgr::Run()
{
	if (m_bIsDoubleNet)
	{
		if (!m_pConnTaskA->Start())
		{
			if (!m_pConnTaskB->Start())
			{
				return false;
			}

			m_nCurWorkingNet = NET_B;

			return true;
		}
		
		// log A网task启动成功
		m_nCurWorkingNet = NET_A;

		if (!m_pConnTaskB->Start())
		{
			// log B网task启动失败
		}
		else
		{
			// log B网task启动成功
		}
	}
	else
	{
		if (!m_pConnTaskA->Start())
		{
			return false;
		}

		m_nCurWorkingNet = NET_A;
	}	

	return true;
}

void CConnectorMgr::Shutdown()
{
	if (m_pConnTaskA != nullptr)
	{
		m_pConnTaskA->Stop();
	}

	if (m_pConnTaskB != nullptr)
	{
		m_pConnTaskB->Stop();
	}

	m_bShutdown = true;
}

void CConnectorMgr::LogMsg(const char *szLogTxt, int nLevel)
{
	//::LogMsg("FES", szLogTxt, nLevel);
}

void CConnectorMgr::LogDebugMsg(const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc)
{
	//::LogDebugMsg("FES", szLogTxt, nLevel, szFile, nLine, szFunc);
}

bool CConnectorMgr::PutMsgToSendQueue(const int8u *pBuf, size_t nLen, int32u msgLevel/* = GENERAL_MSG*/)
{
	ACE_ASSERT(pBuf && nLen > 0 && m_pSendMsgQueue != nullptr);
	if (m_pSendMsgQueue == nullptr)
	{
		return false;
	}

	if (1 != m_pSendMsgQueue->EnqueuMsgToMQ(pBuf, nLen, msgLevel))
	{
		return false;
	}

	return true;
}

bool CConnectorMgr::GetMsgFromSendQueue(const int8u * pBuf, size_t nBufLen, size_t *pCopySize, int32u ulTimeout)
{
	ACE_ASSERT(pBuf && nBufLen > 0 && m_pSendMsgQueue != nullptr);

	if (pBuf == nullptr || nBufLen <= 0 || m_pSendMsgQueue == nullptr)
	{
		return false;
	}

	ACE_Time_Value timeout(ulTimeout);

	size_t size = m_pSendMsgQueue->DequeuMsgFromMQ((void*)pBuf, nBufLen, &timeout);
	if (size == 0)
	{
		return false;
	}

	*pCopySize = size;

	return true;
}

int CConnectorMgr::GetMsgFromSendQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout/* = 0*/)
{
	ACE_ASSERT(m_pSendMsgQueue != nullptr);

	if (m_bShutdown || m_pSendMsgQueue == nullptr)
	{
		return -2;
	}

	return m_pSendMsgQueue->DequeuMsgFromMQ(mb, timeout);
}

bool CConnectorMgr::SendData(const unsigned char *pBuf, size_t nLen, int32u msgLevel/* = GENERAL_MSG*/)
{
	Q_ASSERT(pBuf && nLen > EMSG_BUF_HEAD_SIZE);
	if (pBuf == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return 0;
	}

	return PutMsgToSendQueue(pBuf, nLen, msgLevel);
}

size_t CConnectorMgr::RecvData(unsigned char *pBuf, size_t nBufLen, int32u nTimeout/* = 0*/)
{
	Q_ASSERT(pBuf && nBufLen > EMSG_BUF_HEAD_SIZE);
	if (pBuf == nullptr || nBufLen <= EMSG_BUF_HEAD_SIZE)
	{
		return 0;
	}

	ACE_Message_Block *mblk = 0;
	ACE_Time_Value timeout = ACE_OS::gettimeofday();
	timeout += ACE_Time_Value(nTimeout); // 1s

	// 从【接收】消息队列中取数据
	if (-1 == GetMsgFromRecvQueue(mblk, &timeout))
	{
		return 0;
	}

	size_t llMsgLen = mblk->length();
	size_t llBufLen = nBufLen;
	if (llMsgLen > llBufLen)
	{
		llMsgLen = llBufLen;
	}

	ACE_OS::memcpy(pBuf, mblk->base(), llMsgLen);

	return llMsgLen;
}

bool CConnectorMgr::SwitchWorkingNet()
{
	ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);

	if (!m_bIsDoubleNet)
	{
		// log 单网模式，切换失败
		return false;
	}
	
	if (m_nCurWorkingNet == NET_A)
	{
		if (m_pNodeConfig->CardStatus[NET_B] != 1)
		{
			// log 切换失败，B网不通
			return false;
		}
		else
		{
			m_nCurWorkingNet = NET_B;
			// todo: log 切换到B网
		}
		
	}
	else if (m_nCurWorkingNet == NET_B)
	{
		if (m_pNodeConfig->CardStatus[NET_A] != 1)
		{
			// log 切换失败，A网不通
			return false;
		}
		else
		{
			m_nCurWorkingNet = NET_A;
			// todo: log 切换到A网
		}
	}
	else
	{
		// log 当前工作网络错误
		return false;
	}

	return true;
}

bool CConnectorMgr::SwitchWorkingNet(int nDestWorkingNet)
{
	if (nDestWorkingNet == NET_A)
	{
		if (m_nCurWorkingNet != NET_A)
		{
			m_nCurWorkingNet = NET_A;
		}
	}
	else if (nDestWorkingNet == NET_B)
	{
		if (m_nCurWorkingNet != NET_B)
		{
			m_nCurWorkingNet = NET_B;
		}
	}
	else
	{
		// log 设置工作网络错误

		return false;
	}

	return true;
}

bool CConnectorMgr::PutMsgToRecvQueue(const int8u *pBuf, size_t nLen, int32u msgLevel /*= GENERAL_MSG*/)
{
	ACE_ASSERT(pBuf && nLen > 0 && m_pRecvMsgQueue != nullptr);
	if (m_pRecvMsgQueue == nullptr)
	{
		return false;
	}

	if (1 != m_pRecvMsgQueue->EnqueuMsgToMQ(pBuf, nLen, msgLevel))
	{
		return false;
	}

	return true;
}

int CConnectorMgr::GetMsgFromRecvQueue(ACE_Message_Block *&mb, ACE_Time_Value* timeout /*= 0*/)
{
	ACE_ASSERT(m_pRecvMsgQueue != nullptr);

	if (m_bShutdown || m_pRecvMsgQueue == nullptr)
	{
		return -1;
	}

	return m_pRecvMsgQueue->DequeuMsgFromMQ(mb, timeout);
}

/** @}*/
