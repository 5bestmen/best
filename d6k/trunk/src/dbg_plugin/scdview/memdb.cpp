#include "memdb.h"
#include "log/log.h"
#include "fesdb.h"

CViewMemDB::CViewMemDB()
{
	m_pMem = std::make_shared<CShareMem>();
	m_pNodeGrpMgr = std::make_shared<CNodeGroupMgr>();
}


CViewMemDB::~CViewMemDB()
{
	m_pMem->ReleaseMem();
	m_pMem.reset();
	m_pNodeGrpMgr.reset();
}
bool CViewMemDB::Initialize(const char *pszDataPath, unsigned int nMode)
{
	if (!BuildMemDB(pszDataPath))
	{
		return false;
	}
	return true;
}
void CViewMemDB::Run()
{
	
}

void CViewMemDB::Shutdown()
{

}

bool CViewMemDB::BuildMemDB(const char* pszDataPath)
{
	Q_ASSERT(pszDataPath && strlen(pszDataPath) > 0);
	if (!pszDataPath || strlen(pszDataPath) <= 0)
	{
		return false;
	}
	QString szName = QString(pszDataPath) + "-scada";
	char* pData = (char*)(m_pMem->GetShareMem(szName.toStdString().c_str()));

	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}
	m_pMagicHead = (NODE_SCD_MAGIC*)pData;
	Q_ASSERT(m_pMagicHead->MagicNum1 == MAGIC_HEAD && m_pMagicHead->MagicNum2 == MAGIC_HEAD);

	m_nNodeCount = m_pMagicHead->NodeFesCount;
	m_nAllNodeCount = m_pMagicHead->AllNodeCount;
	Q_ASSERT(m_nNodeCount > 0 && m_nAllNodeCount >0 );

	if (m_nNodeCount <=0 || m_nNodeCount <=0 || m_nNodeCount > m_nAllNodeCount)
	{
		return false;
	}

	pData = (char* )pData + sizeof NODE_SCD_MAGIC;

	size_t nRet = 0;

	nRet = CreateNodeDB((unsigned char*)pData);
	pData += nRet;

	nRet = CreateNodeMem((unsigned char*)pData);
	pData += nRet;


	nRet = CreateFesMem((unsigned char*)pData);
	pData += nRet;

	return true;
}

size_t CViewMemDB::CreateNodeMem(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		LogMsg(QObject::tr("read mem error,due to error head address").toStdString().c_str(), 0);
		return 0;
	}

	QString szLog;
	m_pNodeGIHead = reinterpret_cast<NODE_MEM*>(pHead);

	for (int i= 0; i < m_nNodeCount;++i)
	{
		Q_ASSERT(m_pNodeGIHead[i].OccNo != INVALID_OCCNO && m_pNodeGIHead[i].OccNo < MAX_NODE_OCCNO);
		if (m_pNodeGIHead[i].OccNo == INVALID_OCCNO || m_pNodeGIHead[i].OccNo > MAX_NODE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodeGIHead[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrTempNodes.push_back(&m_pNodeGIHead[i]);
		m_MapNode.insert(std::make_pair(m_pNodeGIHead[i].OccNo, &m_pNodeGIHead[i]));
	}
	return sizeof NODE_MEM * m_nNodeCount;
}

size_t CViewMemDB::CreateFesMem(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		return 0;
	}
	m_pFesHead = (CFesDBR *)(pHead);

	CFesDBR* pFesDB = Q_NULLPTR;

	for (auto i:m_arrTempNodes)
	{
		pFesDB = new CFesDBR;

		if (pFesDB)
		{
			pFesDB->LoadFesMemDB((char*)((char*)m_pFesHead + i->NodeOffSet), i->NodeSize);
			m_arrFesDbs.push_back(pFesDB);
			m_mapFesDBs.insert(std::make_pair(i->OccNo, pFesDB));
		}		
	}
	return 0;
}

void CViewMemDB::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("scdview", pszText, nLevel,nullptr);
}

NODE_MEM* CViewMemDB::GetNodeByOccNo(int nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<MAX_NODE_OCCNO);

	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_NODE_OCCNO)
	{
		return nullptr;
	}

	auto it = m_MapNode.find(nOccNo);

	if (it==m_MapNode.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

CFesDBR * CViewMemDB::GetFesByOccNo(int nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_NODE_OCCNO);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}

	auto it = m_mapFesDBs.find(nOccNo);

	if (it==m_mapFesDBs.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

void CViewMemDB::CloseMemDB()
{
	m_pMem->ReleaseMem();
	ClearTempResources();
}

void CViewMemDB::ClearTempResources()
{
	STLClearObject(&m_arrFesDbs);
	STLClearObject(&m_arrTempNodes);
}

size_t CViewMemDB::CreateNodeDB(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		return 0;
	}
	m_pNodes = reinterpret_cast<NODE*>(pHead);

	QString szLog;

	int32u i = 0;
	// 创建节点表
	for (i = 0; i < m_nAllNodeCount; i++)
	{
		Q_ASSERT(m_pNodes[i].OccNo != INVALID_OCCNO && m_pNodes[i].OccNo <= MAX_NODE_OCCNO);
		if (m_pNodes[i].OccNo == INVALID_OCCNO || m_pNodes[i].OccNo > MAX_NODE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodes[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrNodes.push_back(&m_pNodes[i]);
	}

	return sizeof(NODE)* m_nAllNodeCount;
}
