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

#include "mailimpl.h"
#include "mail_group.h"
#include "sharemem.h"

#include <QDebug>
#include <algorithm> 

CMailImpl g_MailImpl;

CMailBoxSlotGrp::CMailBoxSlotGrp()
{	
	m_mapSlotsID.clear();
	m_mapSlotsName.clear();
}

CMailBoxSlotGrp::CMailBoxSlotGrp(const char *szPredicateName, size_t nSlotNum, int nStartID)
{
	m_nStartID = nStartID;
	Q_ASSERT(nSlotNum > 0 && nSlotNum <= MAX_MAILBOX_NUM);
	if (nSlotNum == 0 || nSlotNum > MAX_MAILBOX_NUM)
	{
		nSlotNum = GRP_MAILBOX_NUM;
	}
	m_nSlotNum = nSlotNum;

	Q_ASSERT(szPredicateName && strlen(szPredicateName)>0) ;
	if (szPredicateName == nullptr || strlen(szPredicateName) == 0)
		return;

	m_szPredicate = szPredicateName;
	
	m_pShareMem = std::make_shared<CShareMem>();

	int nSize = static_cast<int>(sizeof(MB_SLOT) * nSlotNum + sizeof(SLOT_HEAD_MEM));
	void *pMem = m_pShareMem->CreateShareMem(szPredicateName, nSize);
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return;

	memset(pMem, 0, nSize);

	SLOT_HEAD_MEM *pHead = reinterpret_cast<SLOT_HEAD_MEM*>(pMem);
	pHead->MagicHead1 = MAGIC_HEAD;
	pHead->MagicHead2 = MAGIC_HEAD;

	pHead->ShmLength = nSize;
	pHead->SlotCount = static_cast<int32u> (nSlotNum);


	MB_SLOT *pSlots = reinterpret_cast<MB_SLOT*>(pHead + sizeof(SLOT_HEAD_MEM));

	size_t i = 0;
	for (i = 0; i < nSlotNum;i ++)
	{
		pSlots[i].MailBoxID = static_cast<int> (nStartID+i);

		auto ret = m_mapSlotsID.insert(std::make_pair(pSlots[i].MailBoxID, &pSlots[i]));
		Q_ASSERT(ret.second == true);
	}
}

CMailBoxSlotGrp::~CMailBoxSlotGrp()
{
	m_mapSlotsID.clear();
	m_mapSlotsName.clear();
}
/*! \fn void CMailBoxSlotGrp::Create(const char *szPredicateName, size_t nSlotNum, int nStartID)
********************************************************************************************************* 
** \brief CMailBoxSlotGrp::Create 
** \details 主模块调用，在系统启动之初把所有邮箱队列全部建立OK
** \param szPredicateName 
** \param nSlotNum 
** \param nStartID 
** \return void 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
void CMailBoxSlotGrp::Create(const char *szPredicateName, size_t nSlotNum, int nStartID)
{
	m_nStartID = nStartID;
	Q_ASSERT(nSlotNum > 0 && nSlotNum <= MAX_MAILBOX_NUM);
	if (nSlotNum == 0 || nSlotNum > MAX_MAILBOX_NUM)
	{
		nSlotNum = GRP_MAILBOX_NUM;
	}
	m_nSlotNum = nSlotNum;

	Q_ASSERT(szPredicateName && strlen(szPredicateName) > 0);
	if (szPredicateName == nullptr || strlen(szPredicateName) == 0)
		return;

	m_szPredicate = szPredicateName;

	m_pShareMem = std::make_shared<CShareMem>();

	int nSize = static_cast<int>(sizeof(MB_SLOT) * nSlotNum + sizeof(SLOT_HEAD_MEM));
	unsigned char *pMem = reinterpret_cast<unsigned char*> (m_pShareMem->CreateShareMem(szPredicateName, nSize));
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return;

	memset(pMem, 0, nSize);

	SLOT_HEAD_MEM *pHead = reinterpret_cast<SLOT_HEAD_MEM*>(pMem);
	pHead->MagicHead1 = MAGIC_HEAD;
	pHead->MagicHead2 = MAGIC_HEAD;

	pHead->ShmLength = nSize;
	pHead->SlotCount = static_cast<int32u> (nSlotNum);	 

	MB_SLOT *pSlots = reinterpret_cast<MB_SLOT*>(pMem + sizeof(SLOT_HEAD_MEM));

	size_t i = 0;
	for (i = 0; i < nSlotNum; i++)
	{
		pSlots[i].MailBoxID = static_cast<int> (nStartID + i);

		auto ret = m_mapSlotsID.insert(std::make_pair(pSlots[i].MailBoxID, &pSlots[i]));
		Q_ASSERT(ret.second == true);
	}
}
/*! \fn size_t CMailBoxSlotGrp::GetSubscriptionList(int32u nType, std::vector<int> & arrSub)
********************************************************************************************************* 
** \brief CMailBoxSlotGrp::GetSubscriptionList 
** \details 计算对这几个消息感兴趣的队列
** \param nType 
** \param arrSub 
** \return size_t 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
size_t CMailBoxSlotGrp::GetSubscriptionList(int32u nType, std::vector<int> & arrSub)
{
	arrSub.clear();
	if (nType == 0)
		return 0;

	for (auto i : m_mapSlotsID)
	{
		Q_ASSERT(i.second->MailBoxID);
		if (nType & i.second->Subscription)
		{
			arrSub.push_back(i.first);
			Q_ASSERT(i.first == i.second->MailBoxID);
		}
	}
	return arrSub.size();
}
/*! \fn void CMailBoxSlotGrp::Open(const char *szPredicateName)
********************************************************************************************************* 
** \brief CMailBoxSlotGrp::Open 
** \details 子模块调用，在调用前，所有的邮箱ID均已经确定
** \param szPredicateName 
** \return bool 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
bool CMailBoxSlotGrp::Open(const char *szPredicateName)
{
	Q_ASSERT(szPredicateName && strlen(szPredicateName) > 0);
	if (szPredicateName == nullptr || strlen(szPredicateName) == 0)
		return false;

	m_szPredicate = szPredicateName;

	m_pShareMem = std::make_shared<CShareMem>();

	unsigned char *pMem = reinterpret_cast<unsigned char*> (m_pShareMem->GetShareMem(szPredicateName));
	Q_ASSERT(pMem);
	if (pMem == nullptr)
		return false;

	SLOT_HEAD_MEM *pHead = reinterpret_cast<SLOT_HEAD_MEM*>(pMem);
	Q_ASSERT(pHead->MagicHead1 == MAGIC_HEAD);
	Q_ASSERT(pHead->MagicHead2 == MAGIC_HEAD);

	int nSlotNum = pHead->SlotCount;
	Q_ASSERT(nSlotNum > 0 && nSlotNum < MAX_MAILBOX_NUM);
		
	MB_SLOT *pSlots = reinterpret_cast<MB_SLOT*>(pHead + sizeof SLOT_HEAD_MEM);

	size_t i = 0;
	for (i = 0; i < nSlotNum; i++)
	{
		Q_ASSERT(pSlots[i].MailBoxID > 0);
		if (pSlots[i].MailBoxID > 0)
		{
			Q_ASSERT(strlen(pSlots[i].MailBoxName) > 0);
			auto ret1 = m_mapSlotsID.insert(std::make_pair(pSlots[i].MailBoxID, &pSlots[i]));
			Q_ASSERT(ret1.second == true);

			auto ret2 = m_mapSlotsName.insert(std::make_pair(pSlots[i].MailBoxName, pSlots[i].MailBoxID));
			Q_ASSERT(ret2.second == true);
		}	 
	}
	return true;
}

void CMailBoxSlotGrp::Close()
{
	m_pShareMem.reset();
}

/*! \fn int CMailBoxSlotGrp::QueryMailBoxID(const char *pszMailBoxName)
********************************************************************************************************* 
** \brief CMailBoxSlotGrp::QueryMailBoxID 
** \details 根据名字获取ID
** \param pszMailBoxName 
** \return int 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
int CMailBoxSlotGrp::QueryMailBoxID(const char *pszMailBoxName)
{
	Q_ASSERT(pszMailBoxName && strlen(pszMailBoxName) > 0);
	if (pszMailBoxName == nullptr || strlen(pszMailBoxName) == 0)
		return 0;

	auto it_find = m_mapSlotsName.find(pszMailBoxName);
	Q_ASSERT(it_find != m_mapSlotsName.end());
	if (it_find != m_mapSlotsName.end())
	{
		return it_find->second;
	}
	return 0;
}
/*! \fn int CMailBoxSlotGrp::RegisterMailBox(const char * pszMailBoxName)
********************************************************************************************************* 
** \brief CMailBoxSlotGrp::RegisterMailBox 
** \details 注册一个邮箱
** \param pszMailBoxName 
** \return int 
** \author LiJin 
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
int CMailBoxSlotGrp::RegisterMailBox(const char * pszMailBoxName)
{
	Q_ASSERT(pszMailBoxName && strlen(pszMailBoxName) > 0);
	if (pszMailBoxName == nullptr || strlen(pszMailBoxName) == 0)
		return 0;

	size_t nLen = 0;
	QString szMailBoxName(pszMailBoxName);

	// 先查找下，看看有没有重名（已经创建过）
	for (auto i : m_mapSlotsID)
	{
		nLen = strlen(i.second->MailBoxName);
		if (nLen > 0 && nLen <= MAX_NAME_LENGTH)
		{
			if (szMailBoxName.compare((i.second->MailBoxName), Qt::CaseSensitive) == 0)
			{	// 找到，重名
				return  i.second->MailBoxID;				 
			}
		}
	}
//	int nStartID = m_nStartID;

	// 没有找到，注册邮箱
	for (auto i : m_mapSlotsID)
	{
		nLen = strlen(i.second->MailBoxName);
		if (nLen ==0)
		{
			strncpy(i.second->MailBoxName, szMailBoxName.toStdString().c_str(), std::min(static_cast<int>(szMailBoxName.length()), static_cast<int>(MAX_NAME_LENGTH)));
			//i.second->MailBoxID = nStartID;
			Q_ASSERT(i.first == i.second->MailBoxID);
			Q_ASSERT(i.second->MailBoxID != 0);

			auto ret = m_mapSlotsName.insert(std::make_pair(i.second->MailBoxName, i.second->MailBoxID));
			Q_ASSERT(ret.second == true);

			return i.second->MailBoxID;
		}
	//	nStartID++;
	}
	// 没有足够的槽位可以用
	return 0;
}

bool CMailBoxSlotGrp::SubscribeMail(int nID, int32u nType)
{
	auto it_find = m_mapSlotsID.find(nID); 
	Q_ASSERT(it_find != m_mapSlotsID.end());
	if (it_find != m_mapSlotsID.end())
	{
		it_find->second->Subscription = nType;
		return true;
	}
	return false;
}

CMailImpl::CMailImpl(void)
{
	m_nStartID = 0;
}

CMailImpl::~CMailImpl(void)
{ 
	m_arrMasterMailBoxs.clear();
	m_arrSlaveMailBoxs.clear();
	m_arrSlots.clear();
}
/*! \fn bool CMailImpl::CreateMailBoxGrp(const char*pszPredicate, size_t nMailBoxNum)
********************************************************************************************************* 
** \brief CMailImpl::CreateMailBoxGrp 
** \details 主程序调用，创建管理槽
** \param pszPredicate 前缀名称，通常是FES或者SCADA
** \param nMailBoxNum 邮箱数量
** \return bool 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
bool CMailImpl::CreatePostOffice(const char*pszPredicate, size_t nMailBoxNum)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	if (m_nStartID == 0)
	{
		m_nStartID += GRP_MAILBOX_NUM;
	}

	using namespace boost::interprocess;

#if 1
	std::string szMutexName = "MAIL_SLOT";
	try
	{
		named_mutex::remove(szMutexName.c_str());
		m_pSlotMux = std::make_shared<boost::interprocess::named_mutex>(open_or_create, szMutexName.c_str());
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
	// std::cout << ex.what() << std::endl;	 
	}
#endif

	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	if (pSlotGrp == nullptr)
	{// 没有创建过
		pSlotGrp = std::make_shared<CMailBoxSlotGrp>(pszPredicate, nMailBoxNum, m_nStartID);
		m_arrSlots.push_back(pSlotGrp);
		m_nStartID += GRP_MAILBOX_NUM; 
	}

	auto pMailBoxGrp = GetMasterMailBoxGrp(pszPredicate);
	if (pMailBoxGrp == nullptr)
	{// 没有创建过
		pMailBoxGrp = std::make_shared<CMailBoxGrp>(pszPredicate);
		m_arrMasterMailBoxs.push_back(pMailBoxGrp);
	}
	return true;
}

void CMailImpl::DestroyPostOffice(const char *pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return ;

	for (auto it = m_arrMasterMailBoxs.begin(); it != m_arrMasterMailBoxs.end(); ++ it)
	{
		if ((*it)->GetName().compare(pszPredicate) == 0)
		{//				 
			m_arrMasterMailBoxs.erase(it);
			return;
		}
	}

	Q_ASSERT(false);
} 
/*! \fn bool CMailImpl::OpenPostOffice(const char*pszPredicate)
********************************************************************************************************* 
** \brief CMailImpl::OpenPostOffice 
** \details 打开邮局 主要用于进程外的模块
** \param pszPredicate 
** \return bool 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
bool CMailImpl::OpenPostOffice(const char*pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	using namespace boost::interprocess;
	if (!m_pSlotMux)
	{
		std::string szMutexName = "MAIL_SLOT";
		try
		{
			m_pSlotMux = std::make_shared<boost::interprocess::named_mutex>(open_or_create, szMutexName.c_str());
		}
		catch (interprocess_exception &ex)
		{
			qDebug() << ex.what();
			Q_ASSERT(false);
		}
	}
	scoped_lock<named_mutex> lck(*m_pSlotMux);

	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	if (pSlotGrp == nullptr)
	{// 没有创建过
		pSlotGrp = std::make_shared<CMailBoxSlotGrp>();
		if (pSlotGrp->Open(pszPredicate))
		{
			m_arrSlots.push_back(pSlotGrp);	
		}
		else
		{
			Q_ASSERT(false);
			return false;
		}
	}
	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	if (pMailBoxGrp == nullptr)
	{
		pMailBoxGrp = std::make_shared<CMailBoxGrp>(pszPredicate);
		m_arrSlaveMailBoxs.push_back(pMailBoxGrp);
	}	
	return true;
} 
/*! \fn void CMailImpl::ClosePostOffice(const char *pszPredicate)
********************************************************************************************************* 
** \brief CMailImpl::ClosePostOffice 
** \details 关闭邮寄 主要用于进程外的模块
** \param pszPredicate 
** \return void 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
void CMailImpl::ClosePostOffice(const char *pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return  ;
	
	using namespace boost::interprocess;
	scoped_lock<named_mutex> lck(*m_pSlotMux);

	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotGrp);
	if (pSlotGrp != nullptr)
	{//  
		pSlotGrp->Close( );
		return;
	}
	Q_ASSERT(false);
}
/*! \fn int CMailImpl::RegisterMailBox(const char * pszPredicate, const char * pszMailBoxName)
*********************************************************************************************************
** \brief CMailImpl::RegisterMailBox
** \details 主模块向系统申请一个mail box id
** \param pszPredicate
** \param pszMailBoxName
** \return int
** \author LiJin
** \date 2016年9月27日
** \note
********************************************************************************************************/
int CMailImpl::RegisterMailBox(const char * pszPredicate, const char * pszMailBoxName)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return 0;

	Q_ASSERT(pszMailBoxName && strlen(pszMailBoxName));
	if (pszMailBoxName == nullptr || strlen(pszMailBoxName) <= 0)
		return 0;
 
	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotGrp);
	if (pSlotGrp == nullptr)
		return 0;
	return pSlotGrp->RegisterMailBox(pszMailBoxName);
}
/*! \fn int  CMailImpl::QueryMailBoxID(const char * pszPredicate, const char * szTagName)
*********************************************************************************************************
** \brief CMailImpl::QueryMailBoxID
** \details 获取邮箱地址
** \param pszPredicate
** \param szTagName
** \return int
** \author LiJin
** \date 2016年9月26日
** \note 创建之前，是无法获取到ID的
********************************************************************************************************/
int  CMailImpl::QueryMailBoxID(const char * pszPredicate, const char * szTagName)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return 0;

	Q_ASSERT(szTagName && strlen(szTagName));
	if (szTagName == nullptr || strlen(szTagName) <= 0)
		return 0;

	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotGrp);
	if (pSlotGrp == nullptr)
		return 0;
	return pSlotGrp->QueryMailBoxID(szTagName); 
}
/*! \fn bool CMailImpl::CreateMailBox(const char *pszPredicate, int nMailBoxID)
********************************************************************************************************* 
** \brief CMailImpl::CreateMailBox 
** \details 主程序创建邮箱（SCADA\FES）
** \param pszPredicate 
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::CreateMailBox(const char *pszPredicate, int nMailBoxID, size_t nNum, size_t nSize)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	auto pMailBoxGrp = GetMasterMailBoxGrp(pszPredicate);
	
	// 找到了，说明已经创建过
	if (pMailBoxGrp )
		return pMailBoxGrp->Create(nMailBoxID, nNum, nSize); 

	// 这个组还没有创建
	pMailBoxGrp = std::make_shared<CMailBoxGrp>(pszPredicate);
	m_arrMasterMailBoxs.push_back(pMailBoxGrp);
	pMailBoxGrp->Create(nMailBoxID, nNum, nSize);

	return true;
}
/*! \fn bool CMailImpl::DestroyMailBox(const char *pszPredicate, int nMailBoxID)
********************************************************************************************************* 
** \brief CMailImpl::DestroyMailBox 
** \details 主程序销毁邮箱（SCADA\FES）
** \param pszPredicate 
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::DestroyMailBox(const char *pszPredicate, int nMailBoxID)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	auto pMailBoxGrp = GetMasterMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp == nullptr)
		return false;

	pMailBoxGrp->Destroy(nMailBoxID);

	return false;
} 
/*! \fn bool CMailImpl::CreateMailBox(const char *pszPredicate, const char *pszMailName,size_t nNum, size_t nSize, int & nRetID)
********************************************************************************************************* 
** \brief CMailImpl::CreateMailBox 
** \details 创建 邮箱
** \param pszPredicate 
** \param pszMailName 
** \param nRetID 
** \return bool 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
bool CMailImpl::CreateMailBox(const char *pszPredicate, const char *pszMailName, size_t nNum, size_t nSize, int & nRetID)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pszMailName && strlen(pszMailName));
	if (pszMailName == nullptr || strlen(pszMailName) <= 0)
		return false;

	auto pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotGrp);
	if (pSlotGrp == nullptr)
		return false;

	nRetID = pSlotGrp->RegisterMailBox(pszMailName);
	Q_ASSERT(nRetID != 0);
	if (nRetID == 0)
		return false;

	auto pMailBoxGrp = GetMasterMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp == nullptr)
		return false;

	return pMailBoxGrp->Create(nRetID,nNum,nSize);
}
/*! \fn bool  CMailImpl::OpenMailBox(const char *pszPredicate, const char *pszMailName, int & nRetID)
********************************************************************************************************* 
** \brief CMailImpl::OpenMailBox 
** \details 子模块打开队列
** \param pszPredicate 
** \param pszMailName 
** \param nRetID 
** \return bool 
** \author LiJin 
** \date 2016年9月26日 
** \note 
********************************************************************************************************/
bool  CMailImpl::OpenMailBox(const char *pszPredicate, const char *pszMailName, int & nRetID)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pszMailName && strlen(pszMailName));
	if (pszMailName == nullptr || strlen(pszMailName) <= 0)
		return false;

	int nMailBoxId = QueryMailBoxID(pszPredicate, pszMailName);
	nRetID = nMailBoxId;
	Q_ASSERT(nMailBoxId > 0);

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		return pMailBoxGrp->Open(nRetID);
	}

	Q_ASSERT(false);
	return false;
}
/*! \fn bool CMailImpl::OpenMailBox(const char *pszPredicate, int nMailBoxID)
********************************************************************************************************* 
** \brief CMailImpl::OpenMailBox 
** \details 子模块打开已经创建好的邮箱 （前置驱动、报警、前置转发等等）
** \param pszPredicate 
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::OpenMailBox(const char *pszPredicate, int nMailBoxID)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		return pMailBoxGrp->Open(nMailBoxID);
	}
 
	Q_ASSERT(false);
	return false;
}
/*! \fn bool CMailImpl::CloseMailBox(const char *pszPredicate, int nMailBoxID)
********************************************************************************************************* 
** \brief CMailImpl::CloseMailBox 
** \details 子模块关闭邮箱（前置驱动、报警、前置转发等等）
** \param pszPredicate 
** \param nMailBoxID 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::CloseMailBox(const char *pszPredicate, int nMailBoxID)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		return pMailBoxGrp->Close(nMailBoxID);
	}
 
	Q_ASSERT(false);
	return false;
}
/*! \fn bool CMailImpl::SubScribeMail(const char *pszPredicate, int nSelfMailBoxID, int32u nType)
********************************************************************************************************* 
** \brief CMailImpl::SubScribeMail 
** \details 
** \param pszPredicate 
** \param nSelfMailBoxID 
** \param nType 
** \return bool 
** \author LiJin 
** \date 2016年9月27日 
** \note 
********************************************************************************************************/
bool CMailImpl::SubscribeMail(const char *pszPredicate, int nSelfMailBoxID, int32u nType)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	std::shared_ptr<CMailBoxSlotGrp> pSlotGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotGrp);
	if (pSlotGrp == nullptr)
		return false;

	return pSlotGrp->SubscribeMail(nSelfMailBoxID, nType);
}
 /*! \fn bool CMailImpl::SendMail(const char *pszPredicate, DMSG *pMsg)
********************************************************************************************************* 
** \brief CMailImpl::SendMail 
** \details 发送消息
** \param pszPredicate 
** \param pMsg 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::SendMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	// 先查看下，有哪些队列注册了这个消息
	auto pSlotsGrp = GetMailBoxSlotGrp(pszPredicate);
	Q_ASSERT(pSlotsGrp);
	if (pSlotsGrp == nullptr)
		return false;

	int32u nSubscription = pMsg->Type;
	std::vector<int> arrSub;
	size_t nRet = pSlotsGrp->GetSubscriptionList(nSubscription, arrSub);

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp == nullptr)
		return false;

	if (nRet == 0)
	{// 没有人感兴趣，则直接发送
		return pMailBoxGrp->SendMail(pMsg,nWaitTime);
	}
	else
	{// 有那么几个人对这几个消息（nSubscription 按位计算）感兴趣，
		for (auto i : arrSub)
		{
			pMsg->SubID = i;

			if (i != pMsg->SenderID)
			{// 不能发给自己
				pMailBoxGrp->TrySendMail(i, pMsg);
			}
		}

		return pMailBoxGrp->SendMail(pMsg, nWaitTime);		 
	}

	Q_ASSERT(false);
	return false;
}
/*! \fn bool CMailImpl::SendMail(const char *pszPredicate, int nSendMailBoxID, int nRecvMailBoxID, const unsigned  char* pMail, int nLength)
********************************************************************************************************* 
** \brief CMailImpl::SendMail 
** \details 与上类似，但是因为内存拷贝，效率会略低，常用于发送裸报文
** \param pszPredicate 
** \param nSendMailBoxID 
** \param nRecvMailBoxID 
** \param pMail 
** \param nLength 
** \return bool 
** \author LiJin 
** \date 2016年9月24日 
** \note 
********************************************************************************************************/
bool CMailImpl::SendMail(const char *pszPredicate, int nSendMailBoxID, int nRecvMailBoxID, const unsigned  char* pMail, int nLength, unsigned int nWaitTime)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pMail);
	if (pMail == nullptr)
		return false;

	Q_ASSERT(nLength > 0 && nLength <= MAIL_MAX_SIZE);
	if (nLength == 0 || nLength > MAIL_MAX_SIZE)
		return false;

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		DMSG mail;
		memset(&mail, 0, sizeof(mail));

		mail.SenderID = nSendMailBoxID;
		mail.RecverID = nRecvMailBoxID;
		mail.Type = 0;
		mail.Size = std::min(nLength, static_cast<int>(MAIL_MAX_SIZE));
		memcpy(mail.Buff, pMail, mail.Size);

		return pMailBoxGrp->SendMail(&mail,nWaitTime);		 
	}

	Q_ASSERT(false);
	return false;
}

bool CMailImpl::RecvMail(const char *pszPredicate, int nRecvMailBoxID, unsigned char* pMail, size_t nBufLen, size_t&nRecvdLen, unsigned int nWaitTime)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pMail);
	if (pMail == nullptr)
		return false;

	Q_ASSERT(nBufLen > 0 && nBufLen <= MAIL_MAX_SIZE);
	if (nBufLen == 0 || nBufLen > MAIL_MAX_SIZE)
		return false;

	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		return pMailBoxGrp->RecvMail(nRecvMailBoxID, pMail, nBufLen, nRecvdLen, nWaitTime);		
	}

	Q_ASSERT(false);
	return false;
}

bool  CMailImpl::RecvMail(const char *pszPredicate, DMSG *pMsg, unsigned int nWaitTime )
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return false;

	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	Q_ASSERT(pMsg->RecverID != 0);
	if (pMsg->RecverID == 0)
		return false;
	
	auto pMailBoxGrp = GetSlaveMailBoxGrp(pszPredicate);
	Q_ASSERT(pMailBoxGrp);
	if (pMailBoxGrp)
	{
		return pMailBoxGrp->RecvMail(pMsg, nWaitTime);
	}
	
	Q_ASSERT(false);
	return false;
}

std::shared_ptr<CMailBoxSlotGrp>  CMailImpl::GetMailBoxSlotGrp(const char * pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return nullptr;

	for (auto it : m_arrSlots)
	{
		if (it->GetName().compare(pszPredicate) == 0)
		{//				 
			return it;
		}
	}
	//Q_ASSERT(false);
	return nullptr;
}

std::shared_ptr<CMailBoxGrp>  CMailImpl::GetMasterMailBoxGrp(const char *pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return nullptr;

	for (auto it : m_arrMasterMailBoxs)
	{
		if (it->GetName().compare(pszPredicate) == 0)
		{//				 
			return it;
		}
	}
//	Q_ASSERT(false);
	return nullptr;
}

std::shared_ptr<CMailBoxGrp> CMailImpl::GetSlaveMailBoxGrp(const char *pszPredicate)
{
	Q_ASSERT(pszPredicate && strlen(pszPredicate));
	if (pszPredicate == nullptr || strlen(pszPredicate) <= 0)
		return nullptr;

	for (auto it : m_arrSlaveMailBoxs)
	{
		if (it->GetName().compare(pszPredicate) == 0)
		{//				 
			return it;
		}
	}
	//Q_ASSERT(false);
	return nullptr;
}
/** @}*/
