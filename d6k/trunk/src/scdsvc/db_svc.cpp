/*! @file db_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  db_svc.cpp
文件实现功能 :  内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "db_svc.h"
#include "log/log.h"
#include "fes_db.h"
#include "scada_db.h"
#include "server_db.h"
#include "client_db.h"
#include <QObject> 
#include <QString> 
#include <QHostInfo>

CDbSvc::CDbSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
	m_nMyNodeOccNo = INVALID_OCCNO;
	m_pNodeGrpMgr = std::make_shared<CNodeGroupMgr>();
	m_pMem = std::make_shared<CShareMem>();
	m_pTagNameMem = std::make_shared<CShareMem>();
	m_pScadaMem = std::make_shared<CShareMem>();
}

CDbSvc::~CDbSvc(void)
{ 
	if (m_pMem)
	{
		m_pMem->Destroy();
		m_pMem.reset();
		m_pMem = nullptr;
	}
	if (m_pTagNameMem)
	{
		m_pTagNameMem->Destroy();
		m_pTagNameMem.reset();
		m_pTagNameMem = nullptr;
	}
	if (m_pScadaMem)
	{
		m_pScadaMem->Destroy();
		m_pScadaMem.reset();
		m_pScadaMem = nullptr;
	}
	if (m_pDBAliveFlag)
	{
		m_pDBAliveFlag->reset();
		m_pDBAliveFlag.reset();
	}
}

/*! \fn bool CDbSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbSvc::Initialize 
** \details 初始化  加载proj文件，分别接线各个配置文件
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CDbSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{ 
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	//读取所有节点配置信息
	if (!LoadProject(pszDataPath))
	{
		LogMsg(QObject::tr("load project file error").toStdString().c_str(), 0);
		Q_ASSERT(false);
		return false;
	}

	for each (std::shared_ptr<NODE> pNode in  m_arrTempNodes)
	{
		if (pNode->HostName == QHostInfo::localHostName() && pNode->NodeType == NODE_SVR)
		{
			m_nMyNodeOccNo = pNode->OccNo;
		}
	}

	//创建scada变量内存
	if (!BuildScdMem(pszDataPath))
	{
		return false;
	}

	//构建节点内存
	if (!BuildNodeMem(pszDataPath))
	{
		return false;
	}

	//构建tagname内存
	if (!BuildTagNameMen(pszDataPath))
	{
		return false;
	}
	QString szDataBaseName = QString(pszDataPath) + "/scada";
	// EVENT 设置成 手动模式，初始化为 true，名字为内存库名字 第一参数为模式设定 ，第二参数为是否有信号 ，第三参数为进程内外 ，第四为全局信号名称
	m_pDBAliveFlag = std::make_shared<ACE_Event>(1, 0, USYNC_PROCESS, szDataBaseName.toStdString().c_str());
	// 设置成有信号状态
	m_pDBAliveFlag->signal();

	bool bRet = IsDBAlive(0);

	return true;
}

void CDbSvc::Run()
{

}

void CDbSvc::Shutdown()
{
	if (m_pDBAliveFlag)
	{
		m_pDBAliveFlag->reset();
		m_pDBAliveFlag.reset();
	}
}


bool CDbSvc::IsDBAlive(unsigned  int nTimeout)
{
	if (m_pDBAliveFlag == nullptr)
		return false;

	ACE_Time_Value tm_val(0, nTimeout * 1000);

	int nRet = m_pDBAliveFlag->wait(&tm_val, 0);

	return (nRet == 0) ? true : false;
}

/*! \fn bool  CDbSvc::GetNodeTagNameByOccNo(int32u nOccNo, std::string& tagName) const
********************************************************************************************************* 
** \brief CDbSvc::GetNodeTagNameByOccNo 
** \details 
** \param nOccNo 
** \return bool
** \author xingzhibing
** \date 2017年2月7日 
** \note 
********************************************************************************************************/
bool  CDbSvc::GetNodeTagNameByOccNo(int32u nOccNo, std::string& tagName) const
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<=MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo>MAX_NODE_OCCNO)
	{
		return false;
	}
	auto iter = m_MapID2String.find(nOccNo);
	if (iter==m_MapID2String.end())
	{
		return false;
	}
	else
	{
		tagName = iter->second;
	}
	return true;
}

/*! \fn int32u CDbSvc::GetNodeOccNoByTagName(std::string& tagName)
********************************************************************************************************* 
** \brief CDbSvc::GetNodeOccNoByTagName 
** \details 
** \param tagName 
** \return int32u 
** \author xingzhibing
** \date 2017年2月7日 
** \note 
********************************************************************************************************/
int32u CDbSvc::GetNodeOccNoByTagName(const std::string& tagName) const 
{
	Q_ASSERT(!tagName.empty()  && tagName.length()>0);
	if (tagName.empty() || tagName.length() <=0)
	{
		return INVALID_OCCNO;
	}
	auto iter = m_MapString2ID.find(tagName);
	if (iter== m_MapString2ID.end())
	{
		return INVALID_OCCNO;
	}
	else
	{
		return iter->second;
	}
}

bool CDbSvc::GetAinAlarmByOccNo(int32u nNodeOccNo, int32u nOccNo, AIN_ALARM** pAin)
{
	Q_ASSERT(nNodeOccNo!=INVALID_OCCNO && nNodeOccNo <=m_nNodeCount);	
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > m_nNodeCount)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo ==INVALID_OCCNO || nOccNo >MAX_ALARM_OCCNO)
	{
		return false;
	}
	auto pNodeIter = m_mapFes.find(nNodeOccNo);

	if (pNodeIter ==m_mapFes.end())
	{
		return false;
	}
	else
	{
		return pNodeIter->second->GetAinAlarmByOccNo(nOccNo,pAin);
	}

	return true;
}

bool CDbSvc::GetAinAlarmLimitByOccNo(int32u nNodeOccNo, int32u nOccNo, AIN_ALARM_LIMIT** pAinLimit)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= m_nNodeCount);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > m_nNodeCount)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
	{
		return false;
	}
	auto pNodeIter = m_mapFes.find(nNodeOccNo);

	if (pNodeIter == m_mapFes.end())
	{
		return false;
	}
	else
	{
		return pNodeIter->second->GetAinAlarmLimitByOcNo(nOccNo, pAinLimit);
	}

	return true;
}

bool CDbSvc::GetDinAlarmByOccNo(int32u nNodeOccNo, int32u nOccNo, DIN_ALARM** pDin)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= m_nNodeCount);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > m_nNodeCount)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
	{
		return false;
	}
	auto pNodeIter = m_mapFes.find(nNodeOccNo);

	if (pNodeIter == m_mapFes.end())
	{
		return false;
	}
	else
	{
		return pNodeIter->second->GetDinAlarmByOccNo(nOccNo, pDin);
	}

	return true;
}

bool CDbSvc::GetDinAlarmLimitByOccNo(int32u nNodeOccNo, int32u nOccNo, DIN_ALARM_LIMIT** pDinLimit)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= m_nNodeCount);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > m_nNodeCount)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
	{
		return false;
	}
	auto pNodeIter = m_mapFes.find(nNodeOccNo);

	if (pNodeIter == m_mapFes.end())
	{
		return false;
	}
	else
	{
		return pNodeIter->second->GetDinAlarmLimitByOccNo(nOccNo, pDinLimit);
	}

	return true;
}

int CDbSvc::GetNodeAppCount(int nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo==INVALID_OCCNO ||nOccNo>MAX_OCCNO)
	{
		return 0;
	}
	auto iter = m_arrAppNums.find(nOccNo);
	if (iter==m_arrAppNums.end())
	{
		return 0;
	}
	else
	{
		return iter->second;
	}
}

SAPP * CDbSvc::GetNodeAppAddr(int32u nOccNo, int nIndex)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return nullptr;
	}
	Q_ASSERT(nIndex >= 1 );
	auto iter = m_arrAppAddr.find(nOccNo);
	if (iter == m_arrAppAddr.end())
	{
		return nullptr;
	}
	return &iter->second[nIndex-1];
}

/*! \fn bool CDbSvc::UpdateAinValue(int32u nNodeOccNo, int nAinOccNo, fp64  nVal)
********************************************************************************************************* 
** \brief CDbSvc::UpdateAinValue 
** \details  用于内部更新AIN值
** \param nNodeOccNo 
** \param nAinOccNo 
** \param nVal 
** \return bool 
** \author xingzhibing
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
bool CDbSvc::UpdateAinValue(int32u nNodeOccNo, int32u nAinOccNo, fp64  dblVal)
{
	Q_ASSERT(nNodeOccNo !=INVALID_OCCNO  && nNodeOccNo <=MAX_NODE_OCCNO );
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo >MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nAinOccNo !=INVALID_OCCNO && nAinOccNo <=MAX_OCCNO);
	if (nAinOccNo ==INVALID_OCCNO || nAinOccNo > MAX_OCCNO)
	{
		return false;
	}

	std::shared_ptr<CFesDB> pFes = nullptr;

	auto it = m_mapFes.find(nNodeOccNo);
	if (it == m_mapFes.end())
	{
		Q_ASSERT(false);
		return false;
	}
	else
	{
		pFes = it->second;
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}
	}
	CVariant nVariant(dblVal,DT_DOUBLE);
// 	nVariant.Type = DT_FLOAT;
// 	memcpy(&nVariant.Value, &nVal,sizeof  fp64);
	return pFes->UpdateAinValue(nAinOccNo, nVariant, 1);
}
/*! \fn bool CDbSvc::UpdateDinValue(int32u nNodeOccNo, int nOccNo, int8u& nVal)
********************************************************************************************************* 
** \brief CDbSvc::UpdateDinValue 
** \details 用于内部更新DIN值
** \param nNodeOccNo 
** \param nOccNo 
** \param nVal 
** \return bool 
** \author xingzhibing
** \date 2017年1月18日 
** \note 
********************************************************************************************************/
bool CDbSvc::UpdateDinValue(int32u nNodeOccNo, int32u nOccNo, int8u nVal)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO  && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it = m_mapFes.find(nNodeOccNo);

	std::shared_ptr<CFesDB> pFes = nullptr;
	if (it == m_mapFes.end())
	{
		return false;
	}
	else
	{
		pFes = it->second;
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}
	}
	CVariant nVariant(nVal, DT_BOOLEAN);
// 	CVariant nVariant;
// 	nVariant.Type = DT_CHAR;
// 	memcpy(&nVariant.Value, &nVal, sizeof  int8u);
	return pFes->UpdateDinValue(nOccNo, nVariant, 1);
}



size_t CDbSvc::BuildAppDB(char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	size_t nSize = 0;
	for (auto iter:m_mapApps)
	{
		nSize += BuildAppNodeDB(pAddr, iter.first, iter.second);
		pAddr += nSize;
	}
}

size_t CDbSvc::BuildAppNodeDB(char* pAddr, int32u nOccNo, std::vector<std::shared_ptr<SAPP>>& arrAppNodes)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo==INVALID_OCCNO || nOccNo>MAX_OCCNO)
	{
		return 0;
	}
	if (arrAppNodes.size()==0)
	{
		return 0;
	}
	size_t nSize = 0;

	NODE_APP_MAGIC * pHead = reinterpret_cast<NODE_APP_MAGIC*>(pAddr);
	pHead->MagicNum1 = MAGIC_HEAD;
	pHead->MagicNum2 = MAGIC_HEAD;
	pHead->nCount = arrAppNodes.size();
	pHead->nOccNo = nOccNo;
	pHead->ShmSize = sizeof SAPP * arrAppNodes.size();

	pAddr += sizeof NODE_APP_MAGIC;

	int nIndex = 0;

	SAPP * pApp = nullptr;

	SAPP * pBaseApp = reinterpret_cast<SAPP*>(pAddr);
	
	m_arrAppAddr.insert(std::make_pair(nOccNo,pBaseApp));

	m_arrAppNums.insert(std::make_pair(nOccNo, arrAppNodes.size()));

	for (auto iter:arrAppNodes)
	{
		pApp = (SAPP*)((char*)pBaseApp + sizeof SAPP *nIndex);
		std::memcpy(pApp,iter.get(),sizeof SAPP);
		nIndex++;
	}

	return sizeof SAPP * arrAppNodes.size() + sizeof NODE_APP_MAGIC;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
}

void  CDbSvc::CreateMailBoxs()
{
	CServerDB *pScd = GetMyNodeDB();
	Q_ASSERT(pScd);
	if (pScd)
	{
		pScd->CreateMailBoxs();
	}
// 	std::string szTagName;
// 	int nID = 0;
// 
// 	for (auto i : m_arrIoChannels)
// 	{
// 		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
// 		Q_ASSERT(szTagName.empty() == false);
// 		if (szTagName.empty() == false)
// 		{
// 			CreateMailBox("FES", szTagName.c_str(), 100, &nID);
// 			Q_ASSERT(nID != 0);
// 			i->SetMailBoxID(nID);
// 		}
// 	}
// 
// 	for (auto i : m_arrFTChannels)
// 	{
// 		szTagName = GetTagName(i->GetOccNo(), IDD_FTCHANNEL);
// 		Q_ASSERT(szTagName.empty() == false);
// 		if (szTagName.empty() == false)
// 		{
// 			CreateMailBox("FES", szTagName.c_str(), 100, &nID);
// 			Q_ASSERT(nID != 0);
// 			i->SetMailBoxID(nID);
// 		}
// 	}
}

void  CDbSvc::DestroyMailBoxs()
{
	/*CScadaDB *pScd = GetMyNodeDB();
	Q_ASSERT(pScd);
	if (pScd)
	{
		pScd->DestroyMailBoxs();
	}*/
// 	std::string szTagName;
// 	for (auto i : m_arrIoChannels)
// 	{
// 		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
// 		Q_ASSERT(szTagName.empty() == false);
// 		if (szTagName.empty() == false)
// 		{
// 			DestroyMailBox("FES", szTagName.c_str());
// 		}
// 	}
// 
// 	for (auto i : m_arrFTChannels)
// 	{
// 		szTagName = GetTagName(i->GetOccNo(), IDD_CHANNEL);
// 		Q_ASSERT(szTagName.empty() == false);
// 		if (szTagName.empty() == false)
// 		{
// 			DestroyMailBox("FES", szTagName.c_str());
// 		}
// 	}
}

CServerDB * CDbSvc::GetMyNodeDB()
{
	Q_ASSERT(m_nMyNodeOccNo!=INVALID_OCCNO && m_nMyNodeOccNo<=MAX_NODE_OCCNO);
	if (m_nMyNodeOccNo==INVALID_OCCNO || m_nMyNodeOccNo >MAX_NODE_OCCNO)
	{
		return nullptr;
	}
	for (auto iter:m_arrServerDB)
	{
		if (iter->GetOccNo()== m_nMyNodeOccNo)
		{
			return iter.get();
		}
	}
	return nullptr;
}

/** @}*/
