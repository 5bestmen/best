/*! @file node_scd_mem_cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  node_scd_mem_cpp
文件实现功能 :  内存数据库
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   scada内存数据库
*  @author  xingzhibing
*  @version 1.0
*  @date    2017.06.02
*******************************************************************************/
#ifndef _NODE_SCD_MEM_CPP
#define _NODE_SCD_MEM_CPP

#include "db_svc.h"
#include "data_def.h"
#include "fes_db.h"
#include "server_db.h"
#include "client_db.h"
#include <QString>
#include <QDebug>


/*! \fn bool CDbSvc::BuildScdMem(const char* pszFilePath)
********************************************************************************************************* 
** \brief CDbSvc::BuildScdMem 
** \details 变量内存分为三块 第一块为magic，第二块为各节点大概内存信息 ，第三块为server和workstation的内存信息
** \param pszFilePath 
** \return bool 
** \author xingzhibing
** \date 2017年6月4日 
** \note server 和workstation的内存放置在一块内存中，因其内存类似，先server后workstation
********************************************************************************************************/

bool CDbSvc::BuildScdMem(const char* pszFilePath)
{
	int nCount = 0;
	for (auto iter:m_mapApps)
	{
		nCount += iter.second.size();
	}
	//magic
	size_t nSize = sizeof NODE_SCD_MAGIC +
		sizeof NODE_MEM * m_mapScdConfig.size() +
		sizeof NODE_MEM * m_mapWorkStationConfig.size() +
		sizeof NODE_APP_MAGIC *(1+ m_mapApps.size()) +
		sizeof SAPP * nCount;

	//server
	std::shared_ptr<CServerDB> pSvr = Q_NULLPTR;

	std::map<int32u, QString>::iterator it = m_mapScdConfig.begin();

	for (; it != m_mapScdConfig.end(); ++it)
	{
		pSvr = std::make_shared<CServerDB>();

		pSvr->Initialize((it->second).toStdString().c_str(), 0, it->first);

		auto iter = m_mapApps.find(it->first);
		if (iter!=m_mapApps.end())
		{
			pSvr->InitMailBoxInfo(iter->second);
		}
		
		nSize += pSvr->EstimateMemSize();

		m_mapNodeSize.insert(std::make_pair(it->first, pSvr->EstimateMemSize()));

		m_mapServer.insert(std::make_pair(it->first, pSvr));

		m_arrServerDB.push_back(pSvr);
	}

	//client
	std::shared_ptr<CClientDB> pClient = Q_NULLPTR;

	std::map<int32u, QString>::iterator it1 = m_mapWorkStationConfig.begin();

	for (; it1 != m_mapWorkStationConfig.end(); ++it1)
	{
		pClient = std::make_shared<CClientDB>();

		pClient->Initialize((it1->second).toStdString().c_str(), 0, it1->first);

		nSize += pClient->EstimateMemSize();

		m_mapNodeSize.insert(std::make_pair(it1->first, pClient->EstimateMemSize()));

		m_mapClient.insert(std::make_pair(it1->first, pClient));

		m_arrClientDB.push_back(pClient);
	}


	//预留
	nSize = static_cast<size_t>(nSize*1.1);

	QString szName = QString(pszFilePath) + "-scada-variable";

	char* pData = (char*)m_pScadaMem->CreateShareMem(szName.toStdString().c_str(), nSize);

	Q_ASSERT(pData);

	if (!pData)
	{
		return false;
	}

	m_pScadaMem->Lock();

	//HEAD
	NODE_SCD_MAGIC *pHead = (NODE_SCD_MAGIC*)(pData);
	pHead->MagicNum1 = MAGIC_HEAD;
	pHead->MagicNum2 = MAGIC_HEAD;
	pHead->NodeServerCount = m_pNodeGrpMgr->GetGroup(NODE_SVR)->GetTotalNodeSize();
	pHead->NodeClientCount = m_pNodeGrpMgr->GetGroup(NODE_CLIENT)->GetTotalNodeSize();
	pHead->AllNodeCount = m_arrTempNodes.size();
	pHead->ShmSize = nSize;
	//GI
	size_t nLen = 0;
	pData += sizeof NODE_SCD_MAGIC;

	nLen = BuildNodeGIInfo(pData,NODE_SVR);
	pData += nLen;

	nLen = BuildNodeGIInfo(pData, NODE_CLIENT);
	pData += nLen;

	nLen = BuildNodeTotalInfo(pData,NODE_SVR);
	pData += nLen;

	nLen = BuildNodeTotalInfo(pData, NODE_CLIENT);
	pData += nLen;

	NODE_APP_MAGIC* pAppHead = (NODE_APP_MAGIC*)(pData);
	pAppHead->MagicNum1 = MAGIC_HEAD;
	pAppHead->MagicNum2 = MAGIC_HEAD;
	pAppHead->nCount = m_mapApps.size();
	pAppHead->ShmSize = sizeof NODE_APP_MAGIC * m_mapApps.size() + sizeof SAPP * nCount;

	pData += sizeof NODE_APP_MAGIC;

	nLen=BuildAppDB(pData);
	pData += nLen;

	m_pScadaMem->UnLock();

	return true;
}

#endif