/*! @file mail.h
<PRE>
********************************************************************************
模块名       :  邮件模块
文件名       :  mail.h
文件实现功能 :  邮件模块接口定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  需要注意今后，SCADA可能与FES运行在同一进程空间内
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   邮件模块接口定义
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/

#include "mail_group.h" 

#include <QDebug>
//#include <bitset>
#include <algorithm> 

 

CMailBoxGrp::CMailBoxGrp()
{
	
}

CMailBoxGrp::~CMailBoxGrp()
{
	m_mapMailQueue.clear();
	using namespace boost::interprocess;

	if (m_szPrediateName.empty() == false)
	{
		named_mutex::remove(m_szPrediateName.c_str());
	}
}

CMailBoxGrp::CMailBoxGrp(const char * szPredicate)   
{
	using namespace boost::interprocess;

	if (szPredicate && strlen(szPredicate))
	{
		m_szPrediateName = szPredicate;
		
		std::string szMutexName = m_szPrediateName + "_QUEUE";

		try
		{
			message_queue::remove(szMutexName.c_str());
			m_pQueMux = std::make_shared<boost::interprocess::named_mutex>(create_only, szMutexName.c_str());
		}
		catch (interprocess_exception &ex)
		{
			qDebug() << ex.what();
			Q_ASSERT(false);
		}		
	}
}
/*! \fn bool  CMailBoxGrp::Create(int nMailBoxID, size_t nNum, size_t nSize)
*********************************************************************************************************
** \brief CMailBoxGrp::Create
** \details
** \param nMailBoxID
** \param nNum
** \param nSize
** \return bool
** \author LiJin
** \date 2016年9月24日
** \note
********************************************************************************************************/
bool  CMailBoxGrp::Create(int nMailBoxID, size_t nNum, size_t nSize)
{
	// 0 用于广播
	Q_ASSERT(nMailBoxID != 0);
	if (nMailBoxID == 0)
		return false;

	Q_ASSERT(m_szPrediateName.empty() == false);
	if (m_szPrediateName.empty() == true)
		return false;

	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// 找到了，说明已经创建过
		Q_ASSERT(false);
		return false;
	}

	std::shared_ptr<CQueue> pQueue = std::make_shared<CQueue>(nMailBoxID);
	QString szName = m_szPrediateName.c_str() + QString("_%1").arg(nMailBoxID);

	bool bRet = pQueue->Create(szName.toStdString().c_str(), nNum, nSize);
	if (bRet == false)
	{
		Q_ASSERT(false);
		return false;
	}

	m_mapMailQueue.insert(std::make_pair(nMailBoxID, pQueue));
	return true;
}
/*! \fn bool CMailBoxGrp::Destroy(int nMailBoxID)
********************************************************************************************************* 
** \brief CMailBoxGrp::Destroy 
** \details 主程序端调用
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
bool CMailBoxGrp::Destroy(int nMailBoxID)
{
	Q_ASSERT(nMailBoxID != 0);
	if (nMailBoxID == 0)
		return false;

	Q_ASSERT(m_szPrediateName.empty() == false);
	if (m_szPrediateName.empty() == true)
		return false;

	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// 找到了 
		it->second->Destroy( );

		m_mapMailQueue.erase(it);
		return true;
	}
	Q_ASSERT(false);
	return false;
}
/*! \fn bool CMailBoxGrp::Open(int nMailBoxID)
*********************************************************************************************************
** \brief CMailBoxGrp::Open
** \details 子模块打开邮箱
** \param nMailBoxID
** \return bool
** \author LiJin
** \date 2016年9月24日
** \note 需要注意主程序与子模块是否在同一进程空间内
********************************************************************************************************/
bool CMailBoxGrp::Open(int nMailBoxID)
{
	using namespace boost::interprocess;

	// 0 用于广播
	Q_ASSERT(nMailBoxID != 0);
	if (nMailBoxID == 0)
		return false;

	Q_ASSERT(m_szPrediateName.empty() == false);
	if (m_szPrediateName.empty() == true)
		return false;

	Q_ASSERT(m_pQueMux);
	if (m_pQueMux == nullptr)
		return false;

	//QString szName;
	QString szName = m_szPrediateName.c_str() + QString("_%1").arg(nMailBoxID);

	scoped_lock<named_mutex> lck(*m_pQueMux);

	// 如果此前有已经打开过，则没有必要再打开
	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// 找到
		return it->second->Open(szName.toStdString().c_str());
	}
	//  没有打开过
	std::shared_ptr<CQueue> pQueue = std::make_shared<CQueue>();

	bool bRet = pQueue->Open(szName.toStdString().c_str());
	if (bRet == false)
	{
		Q_ASSERT(false);
		return false;
	}

	m_mapMailQueue.insert(std::make_pair(nMailBoxID, pQueue));
	return true;
}

bool  CMailBoxGrp::Close(int nMailBoxID)
{
	using namespace boost::interprocess;

	Q_ASSERT(nMailBoxID != 0);
	if (nMailBoxID == 0)
		return false;

	Q_ASSERT(m_szPrediateName.empty() == false);
	if (m_szPrediateName.empty() == true)
		return false;

	Q_ASSERT(m_pQueMux);
	if (m_pQueMux == nullptr)
		return false;

	scoped_lock<named_mutex> lck(*m_pQueMux);

	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// 找到了 
		it->second->Close();
		// 在同一进程空间内，删除可能有问题，待调试
		//	m_mapMailQueue.erase(it);
		return true;
	}
	Q_ASSERT(false);
	return false;
}

bool  CMailBoxGrp::SendMail(DMSG *pMail, unsigned int nWaitTime)
{
	Q_ASSERT(pMail);
	if (pMail == nullptr)
		return false;
	int nRecvMailBoxID = pMail->RecverID;
	
	auto it_find = m_mapMailQueue.find(nRecvMailBoxID);
	if (it_find == m_mapMailQueue.end())
	{// 如果没有找到，可能是没有打开过
		bool bRet = false;
		bRet = Open(nRecvMailBoxID);
		if (bRet == false)
		{
			Q_ASSERT(false);
			return false;
		}
		return SendMail(pMail, nWaitTime);
	}

	LETTER msg;
	memset(&msg, 0, sizeof(LETTER));

	msg.MagicHead = MAIL_MAGIC_CODE;
	msg.MagicTail = MAIL_MAGIC_CODE;

	std::memcpy(&msg.Body, pMail, sizeof(pMail->Buff));
	msg.Id = it_find->second->GetMailID();

	return it_find->second->Send(reinterpret_cast<const unsigned char *> (&msg), sizeof(msg), 0, nWaitTime);
}

bool CMailBoxGrp::SendMail(int nMailBoxID, const DMSG* pMail)
{
	Q_ASSERT(pMail);
	if (pMail == nullptr)
		return false;

	auto it_find = m_mapMailQueue.find(nMailBoxID);
	if (it_find == m_mapMailQueue.end())
	{ // 如果没有找到，可能是没有打开过
		bool bRet = false;
		bRet = Open(nMailBoxID);
		if (bRet == false)
		{
			Q_ASSERT(false);
			return false;
		}
		return SendMail(nMailBoxID, pMail);		 
	}

	Q_ASSERT(it_find->second);
	if (it_find->second == nullptr)
		return false;

	LETTER msg;
	msg.MagicHead = MAIL_MAGIC_CODE;
	msg.MagicTail = MAIL_MAGIC_CODE;
	std::memcpy(&msg.Body, pMail, sizeof(DMSG));
	msg.Id = it_find->second->GetMailID();

	return it_find->second->Send(reinterpret_cast<const unsigned char *> (&msg), sizeof(msg), 0);
}

bool CMailBoxGrp::TrySendMail(int nMailBoxID, const DMSG* pMail)
{
	Q_ASSERT(pMail);
	if (pMail == nullptr)
		return false;

	auto it_find = m_mapMailQueue.find(nMailBoxID);
	if (it_find == m_mapMailQueue.end())
	{// 如果没有找到，可能是没有打开过
		bool bRet = false;
		bRet = Open(nMailBoxID);
		if (bRet == false)
		{
			Q_ASSERT(false);
			return false;
		}
		return TrySendMail(nMailBoxID, pMail);
	}

	Q_ASSERT(it_find->second);
	if (it_find->second == nullptr)
		return false;

	LETTER msg;
	msg.MagicHead = MAIL_MAGIC_CODE;
	msg.MagicTail = MAIL_MAGIC_CODE;
	std::memcpy(&msg.Body, pMail, sizeof(DMSG));
	msg.Id = it_find->second->GetMailID();

	return it_find->second->TrySend(reinterpret_cast<const unsigned char *> (&msg), sizeof(msg), 0);
}

bool CMailBoxGrp::RecvMail(int nMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen, unsigned int nWaitTime)
{
	Q_ASSERT(pMail != nullptr);
	if (pMail == nullptr)
		return false;
	Q_ASSERT(nBufLen > 0 && nBufLen <= MAIL_MAX_SIZE);
	if (nBufLen == 0 || nBufLen > MAIL_MAX_SIZE)
		return false;

	auto it_find = m_mapMailQueue.find(nMailBoxID);
	if (it_find == m_mapMailQueue.end())
	{
		Q_ASSERT(false);
		return false;
	}

	Q_ASSERT(it_find->second);
	if (it_find->second == nullptr)
		return false;

	unsigned int nPriority = 0;
	return it_find->second->Receive(pMail, nBufLen, (nRecvdLen), nPriority, nWaitTime);
}

/*! \fn bool CMailBoxGrp::RecvMail(DMSG *pMsg, unsigned int nWaitTime )
********************************************************************************************************* 
** \brief CMailBoxGrp::RecvMail 
** \details 接收邮件，
** \param pMsg 注意，该结构中有接收者的ID，
** \param nWaitTime 
** \return bool 
** \author LiJin 
** \date 2016年10月21日 
** \note 
********************************************************************************************************/
bool CMailBoxGrp::RecvMail(DMSG *pMsg, unsigned int nWaitTime )
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;
	
	int nRecvMailBoxID = pMsg->RecverID;
	
	Q_ASSERT(nRecvMailBoxID);

	auto it_find = m_mapMailQueue.find(nRecvMailBoxID);
	if (it_find == m_mapMailQueue.end())
	{
	//	Q_ASSERT(false);
		return false;
	}

	Q_ASSERT(it_find->second);
	if (it_find->second == nullptr)
		return false;

	unsigned int nPriority = 0;
	LETTER ltr;
	memset(&ltr, 0, sizeof(ltr));

	size_t nLen = 0;
//	unsigned char pTempBuff[1600] = { 0 };

	size_t nRecvdLen = 0;
	bool bRet = it_find->second->Receive(reinterpret_cast<unsigned char *> (&ltr), sizeof(LETTER), (nRecvdLen), nPriority, nWaitTime);
//	bool bRet = it_find->second->Receive(pTempBuff, sizeof(LETTER), nLen, nPriority, nWaitTime);
	if (bRet && nRecvdLen >0)
	{
		Q_ASSERT(ltr.MagicHead == MAIL_MAGIC_CODE);
		Q_ASSERT(ltr.MagicTail == MAIL_MAGIC_CODE);
		Q_ASSERT(ltr.Body.RecverID == nRecvMailBoxID || ltr.Body.SubID==nRecvMailBoxID);
		Q_ASSERT(sizeof(DMSG) == nRecvdLen-8);
//		std::memcpy(pMsg, &msg.Body, sizeof(DMSG));
		qDebug() << "Mail Id = " << ltr.Id ;
		

//		LETTER *pLetter = reinterpret_cast<LETTER*>(pTempBuff) ;
//		std::memcpy(pMsg, &pLetter->Body, sizeof(DMSG));
		std::memcpy(pMsg, &ltr.Body, std::min(sizeof(DMSG), nRecvdLen));
		Q_ASSERT(pMsg->RecverID == nRecvMailBoxID || pMsg->SubID==nRecvMailBoxID);
	}
	return bRet;
}
/** @}*/
