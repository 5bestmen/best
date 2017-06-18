/*! @file mail.h
<PRE>
********************************************************************************
ģ����       :  �ʼ�ģ��
�ļ���       :  mail.h
�ļ�ʵ�ֹ��� :  �ʼ�ģ��ӿڶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ��Ҫע����SCADA������FES������ͬһ���̿ռ���
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ʼ�ģ��ӿڶ���
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
** \date 2016��9��24��
** \note
********************************************************************************************************/
bool  CMailBoxGrp::Create(int nMailBoxID, size_t nNum, size_t nSize)
{
	// 0 ���ڹ㲥
	Q_ASSERT(nMailBoxID != 0);
	if (nMailBoxID == 0)
		return false;

	Q_ASSERT(m_szPrediateName.empty() == false);
	if (m_szPrediateName.empty() == true)
		return false;

	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// �ҵ��ˣ�˵���Ѿ�������
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
** \details ������˵���
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016��9��26�� 
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
	{// �ҵ��� 
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
** \details ��ģ�������
** \param nMailBoxID
** \return bool
** \author LiJin
** \date 2016��9��24��
** \note ��Ҫע������������ģ���Ƿ���ͬһ���̿ռ���
********************************************************************************************************/
bool CMailBoxGrp::Open(int nMailBoxID)
{
	using namespace boost::interprocess;

	// 0 ���ڹ㲥
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

	// �����ǰ���Ѿ��򿪹�����û�б�Ҫ�ٴ�
	auto it = m_mapMailQueue.find(nMailBoxID);
	if (it != m_mapMailQueue.end())
	{// �ҵ�
		return it->second->Open(szName.toStdString().c_str());
	}
	//  û�д򿪹�
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
	{// �ҵ��� 
		it->second->Close();
		// ��ͬһ���̿ռ��ڣ�ɾ�����������⣬������
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
	{// ���û���ҵ���������û�д򿪹�
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
	{ // ���û���ҵ���������û�д򿪹�
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
	{// ���û���ҵ���������û�д򿪹�
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
** \details �����ʼ���
** \param pMsg ע�⣬�ýṹ���н����ߵ�ID��
** \param nWaitTime 
** \return bool 
** \author LiJin 
** \date 2016��10��21�� 
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
