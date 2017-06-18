#ifndef SCADAAPI_H
#define SCADAAPI_H

#include "scadaapi_global.h"
#include "datatypes.h"
#include "sharemem.h"
#include "tag_att.h"
#include "fes_db.h"

#include <memory>
#include <map>
#include <unordered_map>

class CMemDB;
class CTagNamePool;
class CServerDB;
class CClientDB;
struct NODE_SCD_MAGIC;
struct TAG_OCCNO;
struct NODE_MEM;
struct SAPP;

class CScadaApi
{
public:
    CScadaApi();
	~CScadaApi();
public:
	bool Initialize(const char *pszDataPath, unsigned int nMode);
	void Run();
	void Shutdown();
public:
	bool GetOccNoByTagName(const char*pszTagName, int32u *pNodeOccNo, int32u *pIddType, int32u *pOccNo, int32u *pFiledID);
	// 测点类型、节点索引、测点索引、测点属性
	bool GetRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData);
	bool PutRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc);
protected:
	bool   GetNodeTagNameByOccNo(int32u nOccNo, std::string& tagName);
	int32u GetNodeOccNoByTagName(const std::string& tagName);
	bool   GetIOOccNoByTagName(int32u nNodeOccNo, const std::string& tagName, const std::string& szAttr, int32u* pOccNo, int32u* pIddType, int32u *pFiledID);
	
	CFesDB* GetFesDBByOccNO(int32u nOccNo);

	void LogMsg(const char * pszText, int nLevel);
private:
	int m_nRefCount;
protected:
	bool BuildTagNamePool(const char* pszFilePath);
protected:
	std::shared_ptr<CShareMem> m_pMem;
	
	std::map< std::string, int32u> m_MapString2ID;
	//节点OccNo对应的内存中的tagname池
	std::map <int32u, std::shared_ptr<CTagNamePool> > m_MapTagName;
	//节点的tagname occno映射
	std::vector < TAG_OCCNO* >m_arrNodeTagNames;
protected:
	bool   BuildFesDB(const char* pszDataPath);
protected:
	std::shared_ptr<CShareMem> m_pDataMem;
	size_t CreateNodeDB(unsigned char* pHead);
	size_t CreateNodeMem(unsigned char* pHead);
	size_t CreateFesMem(unsigned char* pHead);
private:
	//tagname 的内存结构
	NODE_SCD_MAGIC* m_pMagicHead;
	
	TAG_OCCNO*  m_pNodeTag;
private:
	int m_nNodeFesCount;
	int m_nNodeScdCount;
	int m_nNodeClientCount;

	int m_nTotalNodeCount;
private:
	std::shared_ptr<CTagAttMgr > m_pTagAttr;
	std::map<int32u, NODE_MEM*> m_MapNode;
	std::map<int32u, CFesDB* > m_mapFesDBs;
	std::vector<CFesDB* > m_arrFesDbs;
	std::vector<NODE_MEM* >  m_arrTempNodes;
	std::vector<NODE* > m_arrNodes;
private:
	NODE_SCD_MAGIC* m_pNodeMagicHead;
	NODE_MEM       * m_pNodeGIHead;
	CFesDB         * m_pFesHead;
	NODE* m_pNodes;

protected:
	bool   CreateScdAndClientDB(const char* pszFilePath);
	size_t CreateMemDB(unsigned char* pAddr);
	size_t CreateScadaDB(unsigned char* pAddr);
	size_t CreateClientDB(unsigned char* pAddr);
	size_t CreateAppDB(unsigned char* pAddr);
	size_t CreateNodeAppDB(unsigned char* pAddr);
private:
	std::shared_ptr<CShareMem> m_pScdMem;

	NODE_SCD_MAGIC* m_pScdHead;
	NODE_MEM       * m_pNodeScdMemHead;
	std::vector<CServerDB*> m_arrScdDBs;
	std::vector<NODE_MEM* > m_arrScdMemDbs;
	std::map<int32u, CServerDB*> m_mapScdDbs;

	NODE_MEM       * m_pNodeClientMemHead;
	std::vector<CClientDB*> m_arrClientDBs;
	std::vector<NODE_MEM* >m_arrClientMemDbs;
	std::map<int32u, CClientDB*> m_mapClientDBs;
	
	CServerDB* m_pNodeScdDBHead;
	CClientDB* m_pNodeClientHead;

	std::map<int32u, SAPP*> m_mapAppAddr;
	std::map < int32u, std::vector <SAPP* > > m_arrAppInfos;
};

#endif // SCADAAPI_H
