#pragma once
#include "fesdb.h"
#include "fes_db.h"
#include "data_def.h"
#include "sharemem.h"
#include <vector>
#include <memory>

class CViewMemDB
{
public:
	CViewMemDB();
	virtual ~CViewMemDB();

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

	std::shared_ptr<CShareMem>& GetMem()
	{
		Q_ASSERT(m_pMem);
		return m_pMem;
	}
	int GetNodeSize()
	{
		return m_arrTempNodes.size();
	}

	std::vector<NODE_MEM* >& GetNodeArrs()
	{
		return m_arrTempNodes;
	}

	NODE_MEM* GetNodeByOccNo(int nOccNo);

	CFesDBR * GetFesByOccNo(int nOccNo);

	std::vector<CFesDBR* >& GetFesArrs()
	{
		return m_arrFesDbs;
	}
	void CloseMemDB();
	void ClearTempResources();
private:
	NODE_SCD_MAGIC* m_pMagicHead;
	NODE_MEM       * m_pNodeGIHead;
	CFesDBR         * m_pFesHead;
private:
	int m_nNodeCount;
	int m_nAllNodeCount;
	std::vector<CFesDBR* > m_arrFesDbs;

	std::map<int32u, CFesDBR* > m_mapFesDBs;

	std::vector<NODE_MEM* >  m_arrTempNodes;

	std::map<int32u,NODE_MEM*> m_MapNode ;

	//节点层次管理，可以获取所有节点映射关系
	NODE_GRP_MGR_DEF m_pNodeGrpMgr;

	std::shared_ptr<CShareMem> m_pMem;

	std::vector<NODE* > m_arrNodes;
	NODE* m_pNodes;
private:
	bool BuildMemDB(const char* pszPath);
	void LogMsg(const char * pszText, int nLevel);
	size_t CreateNodeDB(unsigned char* pHead);
	size_t CreateNodeMem(unsigned char* pHead);
	size_t CreateFesMem(unsigned char* pHead);
	
};

