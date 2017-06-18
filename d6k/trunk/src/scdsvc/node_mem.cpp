/*! @file node_mem_cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  node_mem_cpp
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
*  @brief   内存数据库
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.12.26
*******************************************************************************/
#ifndef _NODE_MEM_CPP
#define _NODE_MEM_CPP

#include "db_svc.h"
#include "data_def.h"
#include "fes_db.h"
#include "server_db.h"
#include "client_db.h"
#include <QString>
#include <QDebug>


/*! \fn bool CDbSvc::BuildTagNameMen(const char* pszFilePath)
********************************************************************************************************* 
** \brief CDbSvc::BuildTagNameMen 
** \details 创建节点内tagname内存池
** \param pszFilePath 
** \return bool 
** \author xingzhibing
** \date 2017年2月7日 
** \note  内存结构分为三个部分 第一部分为magic
** \note  第二部分为各前置节点内部各点tagname
** \note  scada tagname 名称命名为全路径-scada/tagname "../../../xx.proj-scada-tagname"
********************************************************************************************************/
bool CDbSvc::BuildTagNameMen(const char* pszFilePath)
{
	//magic
	size_t nSize = sizeof NODE_SCD_MAGIC + sizeof TAG_OCCNO * (m_arrTempNodes.size());

	//fes	
	for (auto i:m_arrFesDB)
	{
		nSize += i->EstimateTagNameMemSize();
	}
	for (auto i:m_arrServerDB)
	{
		nSize += i->EstimateTagNameMemSize();
	}
	for (auto i:m_arrClientDB)
	{
		nSize += i->EstimateTagNameMemSize();
	}

	//预留
	nSize = static_cast<size_t>(nSize*1.1);

	QString szName = QString(pszFilePath) + "-scada-tagname";

	char* pData = (char*)m_pTagNameMem->CreateShareMem(szName.toStdString().c_str(), nSize);

	Q_ASSERT(pData);

	if (!pData)
	{
		return false;
	}

	m_pTagNameMem->Lock();

	//HEAD
	NODE_SCD_MAGIC *pHead = (NODE_SCD_MAGIC*)(pData);
	pHead->MagicNum1 = MAGIC_HEAD;
	pHead->MagicNum2 = MAGIC_HEAD;
	pHead->NodeFesCount = m_pNodeGrpMgr->GetGroup(NODE_FES)->GetTotalNodeSize();
	pHead->NodeServerCount = m_pNodeGrpMgr->GetGroup(NODE_SVR)->GetTotalNodeSize();
	pHead->NodeClientCount = m_pNodeGrpMgr->GetGroup(NODE_CLIENT)->GetTotalNodeSize();
	pHead->AllNodeCount = m_arrTempNodes.size();
	pHead->ShmSize = nSize;

	size_t nLen = 0;
	pData += sizeof NODE_SCD_MAGIC;

	//node tagname<--->occno
	nLen = BuildTagNameDB(pData, m_arrNodeTagName);
	pData += nLen;
	//fes
	for (auto i:m_arrFesDB)
	{
		i->BuilTagNamedDB(pData);
		pData += i->EstimateTagNameMemSize();
	}
	//scada
	for (auto i:m_arrServerDB)
	{
		i->BuildTagnamedDB(pData);
		pData += i->EstimateTagNameMemSize();
	}
	//client
	for (auto i:m_arrClientDB)
	{
		i->BuildTagnamedDB(pData);
		pData += i->EstimateTagNameMemSize();
	}
	m_pTagNameMem->UnLock();

	return true;
}

/*! \fn size_t CDbSvc::BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec)
********************************************************************************************************* 
** \brief CDbSvc::BuildTagNameDB 
** \details 构建tagname中node 对应的tagname-occno关系
** \param pAddress 
** \param vec 
** \return size_t 
** \author xingzhibing
** \date 2017年2月8日 
** \note 
********************************************************************************************************/
size_t CDbSvc::BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec)
{
	Q_ASSERT(pAddress && !vec.empty());

	if (pAddress == Q_NULLPTR || vec.empty())
	{
		return 0;
	}

	int  nIndex = 0;

	TAG_OCCNO* pTagOccNo = Q_NULLPTR;

	//基准地址
	TAG_OCCNO * pBaseTagOccNos = reinterpret_cast<TAG_OCCNO*>(pAddress);

	for (auto i : vec)
	{
		pTagOccNo = (TAG_OCCNO*)((char*)pBaseTagOccNos + sizeof TAG_OCCNO * nIndex);

		std::memcpy(pTagOccNo, i.get(), sizeof TAG_OCCNO);

		nIndex++;
	}

	return sizeof TAG_OCCNO * vec.size();
}

/*! \fn bool CDbSvc::BuildNodeMem(const char *pszFilePath)
********************************************************************************************************* 
** \brief CDbSvc::BuildNodeMem 
** \details 各节点内存文件映射
** \param pszFilePath 
** \return bool 
** \author xingzhibing
** \date 2016年12月27日 
** \note  内存结构分为三个部分 第一部分为magic 
** \note  第二部分为个节点概述 第三部分为各前置节点内部各点状态值
** \note  scada名称命名为全路径-scada "../../../xx.proj-scada"
********************************************************************************************************/
bool CDbSvc::BuildNodeMem(const char *pszFilePath)
{
	//magic
	size_t nSize = sizeof NODE_SCD_MAGIC +
				   sizeof NODE * m_arrTempNodes .size() +
				   sizeof NODE_MEM * m_mapFesConfig.size();

	//node
	std::shared_ptr<CFesDB> pFes = Q_NULLPTR;

	std::map<int32u, QString>::iterator it = m_mapFesConfig.begin();

	for (;it!=m_mapFesConfig.end();++it)
	{
		pFes = std::make_shared<CFesDB>();

		pFes->Initialize((it->second).toStdString().c_str(),0,it->first);

		nSize += pFes->EstimateMemSize();

		m_mapNodeSize.insert(std::make_pair(it->first,pFes->EstimateMemSize()));

		m_mapFes.insert(std::make_pair(it->first,pFes));

		m_arrFesDB.push_back(pFes);
	}	
	//预留
	nSize = static_cast<size_t>(nSize*1.1);

	QString szName=QString(pszFilePath) + "-scada" ;

	char* pData = (char*)m_pMem->CreateShareMem(szName.toStdString().c_str(),nSize);

	Q_ASSERT(pData);
	
	if (!pData)
	{
		return false;
	}

	m_pMem->Lock();
	
	//HEAD
	NODE_SCD_MAGIC *pHead = (NODE_SCD_MAGIC*)(pData);
	pHead->MagicNum1 = MAGIC_HEAD;
	pHead->MagicNum2 = MAGIC_HEAD;
	pHead->NodeFesCount = m_pNodeGrpMgr->GetGroup(NODE_FES)->GetTotalNodeSize();
	pHead->AllNodeCount = m_arrTempNodes.size();
	pHead->ShmSize = nSize;
	//GI
	size_t nLen = 0;
	pData += sizeof NODE_SCD_MAGIC;

	nLen = BuildNodeDB(pData);
	pData += nLen;

	nLen = BuildNodeGIInfo(pData,NODE_FES);
	pData += nLen;

	nLen = BuildNodeTotalInfo(pData,NODE_FES);
	//接口和QT 一致
	m_pMem->UnLock();



	return true;
}

size_t CDbSvc::BuildNodeDB(char* pAddress)
{
	Q_ASSERT(pAddress);
	if (!pAddress)
	{
		return 0;
	}
	int nIndex = 0;

	NODE* pNode = Q_NULLPTR;

	m_pNodes = (NODE*)((char*)pAddress);

	for (auto i : m_arrTempNodes)
	{
		pNode = (NODE*)((char*)m_pNodes + sizeof(NODE)*nIndex);
		std::memcpy(pNode, i.get(), sizeof(NODE));
		nIndex++;
	}

	m_nNodeCount = m_arrTempNodes.size();


	return sizeof NODE * m_arrTempNodes.size();
}

/*! \fn bool  CDbSvc::BulidNodeGIInfo(char* pAddr)
********************************************************************************************************* 
** \brief CDbSvc::BulidNodeGIInfo 
** \details  创建前置内存节点概述信息
** \param pAddr 
** \return bool 
** \author xingzhibing
** \date 2016年12月27日 
** \note 
********************************************************************************************************/
size_t  CDbSvc::BuildNodeGIInfo(char* pAddr, NODE_TYPE nType)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		//todo log invalid address
		return 0;
	}

	m_pNodeInfo = (NODE_MEM *)((unsigned char*)pAddr);

	NODE_MEM* pNodeInfo = Q_NULLPTR;

	size_t  nBaseSize = 0;

	std::vector < std::shared_ptr<NODE_MEM > > VecNodeMem;

	std::shared_ptr<NODE_MEM> pNodeMem = Q_NULLPTR;

	int nIndex = 0;

	switch (nType)
	{
	case NODE_SVR:
	{
		std::map<int32u, QString>::iterator it = m_mapScdConfig.begin();

		for (; it != m_mapScdConfig.end(); ++it)
		{
			NODE_DEF pNode = m_pNodeGrpMgr->GetGroup(NODE_SVR)->GetNodeByOccNo(it->first);

			Q_ASSERT(pNode);

			if (pNode)
			{
				pNodeMem = std::make_shared<NODE_MEM>();
				memset(pNodeMem->NodeTagName, 0, MAX_NAME_LEN);
				pNodeMem->Type = NODE_SVR;
				strncpy(pNodeMem->NodeTagName, pNode->TagName, qMin((size_t)MAX_NAME_LEN, (size_t)strlen(pNode->TagName)));

				pNodeMem->OccNo = pNode->OccNo;

				pNodeMem->NodeSize = GetNodeSizeByOccNO(pNodeMem->OccNo);

				pNodeMem->NodeOffSet = nBaseSize;
				nBaseSize += pNodeMem->NodeSize;

				VecNodeMem.push_back(pNodeMem);

				pNodeInfo = (NODE_MEM*)((unsigned char*)(m_pNodeInfo)+sizeof NODE_MEM * nIndex);

				memcpy(pNodeInfo, pNodeMem.get(), sizeof NODE_MEM);

				nIndex++;
			}
		}
		return sizeof NODE_MEM *  m_mapScdConfig.size();
	}
	case NODE_FES:
	{
		std::map<int32u, QString>::iterator it = m_mapFesConfig.begin();

		for (; it != m_mapFesConfig.end(); ++it)
		{
			NODE_DEF pNode = m_pNodeGrpMgr->GetGroup(NODE_FES)->GetNodeByOccNo(it->first);

			Q_ASSERT(pNode);

			if (pNode)
			{
				pNodeMem = std::make_shared<NODE_MEM>();
				memset(pNodeMem->NodeTagName, 0, MAX_NAME_LEN);
				pNodeMem->Type = NODE_FES;
				strncpy(pNodeMem->NodeTagName, pNode->TagName, qMin((size_t)MAX_NAME_LEN, (size_t)strlen(pNode->TagName)));

				pNodeMem->OccNo = pNode->OccNo;

				pNodeMem->NodeSize = GetNodeSizeByOccNO(pNodeMem->OccNo);

				pNodeMem->NodeOffSet = nBaseSize;
				nBaseSize += pNodeMem->NodeSize;

				VecNodeMem.push_back(pNodeMem);

				pNodeInfo = (NODE_MEM*)((unsigned char*)(m_pNodeInfo)+sizeof NODE_MEM * nIndex);

				memcpy(pNodeInfo, pNodeMem.get(), sizeof NODE_MEM);

				nIndex++;
			}
		}
		return sizeof NODE_MEM *  m_mapFesConfig.size();
	}
	case NODE_CLIENT:
	{
		std::map<int32u, QString>::iterator it = m_mapWorkStationConfig.begin();

		for (; it != m_mapWorkStationConfig.end(); ++it)
		{
			NODE_DEF pNode = m_pNodeGrpMgr->GetGroup(NODE_CLIENT)->GetNodeByOccNo(it->first);

			Q_ASSERT(pNode);

			if (pNode)
			{
				pNodeMem = std::make_shared<NODE_MEM>();
				memset(pNodeMem->NodeTagName, 0, MAX_NAME_LEN);
				pNodeMem->Type = NODE_CLIENT;
				strncpy(pNodeMem->NodeTagName, pNode->TagName, qMin((size_t)MAX_NAME_LEN, (size_t)strlen(pNode->TagName)));

				pNodeMem->OccNo = pNode->OccNo;

				pNodeMem->NodeSize = GetNodeSizeByOccNO(pNodeMem->OccNo);

				pNodeMem->NodeOffSet = nBaseSize;
				nBaseSize += pNodeMem->NodeSize;

				VecNodeMem.push_back(pNodeMem);

				pNodeInfo = (NODE_MEM*)((unsigned char*)(m_pNodeInfo)+sizeof NODE_MEM * nIndex);

				memcpy(pNodeInfo, pNodeMem.get(), sizeof NODE_MEM);

				nIndex++;
			}
		}
		return sizeof NODE_MEM *  m_mapWorkStationConfig.size();
	}
	default:
		return 0;
	}

	
}

size_t CDbSvc::BuildNodeTotalInfo(char* pAddr, NODE_TYPE nType)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	switch (nType)
	{
	case NODE_SVR:
	{
		m_pSvrDB = (CServerDB*)((unsigned char*)pAddr);

		CServerDB *pSvrDB = Q_NULLPTR;

		int nIndexOffset = 0;

		std::map<int32u, std::shared_ptr< CServerDB> > ::iterator it = m_mapServer.begin();

		for (; it != m_mapServer.end(); ++it)
		{
			qDebug() << it->first;

			std::shared_ptr<CServerDB> pSvr = it->second;
			if (pSvr)
			{
				pSvrDB =(CServerDB*)((unsigned char*)m_pSvrDB + nIndexOffset);
				pSvr->BuildDB((char*)pSvrDB);
				nIndexOffset += pSvr->EstimateMemSize();
			}
		}
		return nIndexOffset;
	}
	case NODE_FES:
	{
		m_pFesDB = (CFesDB*)((unsigned char*)pAddr);

		CFesDB *pFesDB = Q_NULLPTR;

		int nIndexOffset = 0;

		std::map<int32u, std::shared_ptr< CFesDB> > ::iterator it = m_mapFes.begin();

		for (; it != m_mapFes.end(); ++it)
		{
			qDebug() << it->first;

			std::shared_ptr<CFesDB> pFes = it->second;
			if (pFes)
			{
				pFesDB = (CFesDB*)((unsigned char*)m_pFesDB + nIndexOffset);
				pFes->BuildDB((char*)pFesDB);
				nIndexOffset += pFes->EstimateMemSize();
			}
		}
		return nIndexOffset;
	}
	case NODE_CLIENT:
	{
		m_pClientDB = (CClientDB*)((unsigned char*)pAddr);

		CClientDB *pClientDB = Q_NULLPTR;

		int nIndexOffset = 0;

		std::map<int32u, std::shared_ptr< CClientDB> > ::iterator it = m_mapClient.begin();

		for (; it != m_mapClient.end(); ++it)
		{
			qDebug() << it->first;

			std::shared_ptr<CClientDB> pClient = it->second;
			if (pClient)
			{
				pClientDB = (CClientDB*)((unsigned char*)m_pClientDB + nIndexOffset);
				pClient->BuildDB((char*)pClientDB);
				nIndexOffset += pClient->EstimateMemSize();
			}
		}
		return nIndexOffset;
	}
	default:
		break;
	}
	
	return 0;
}

size_t CDbSvc::GetNodeSizeByOccNO(int nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<=MAX_NODE_OCCNO);
	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_NODE_OCCNO)
	{
		return 0;
	}

	Q_ASSERT(m_mapNodeSize.size());
	if (!m_mapNodeSize.size())
	{
		return 0;
	}

	auto it = m_mapNodeSize.find(nOccNo);

	if (it==m_mapNodeSize.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}

std::shared_ptr<CFesDB> CDbSvc::GetFesDbByOccNo(int nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}

	Q_ASSERT(m_mapFes.size());
	if (!m_mapFes.size())
	{
		return nullptr;
	}

	auto it = m_mapFes.find(nOccNo);
	if (it==m_mapFes.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

#endif
