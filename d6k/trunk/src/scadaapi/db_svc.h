/*! @file db_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  db_svc.h
文件实现功能 :  内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  注意：与前置结构不同的是，node由db_svc这一层来管理
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.01
*******************************************************************************/
#ifndef DBSVC_MODULE_H
#define DBSVC_MODULE_H

#include "base_module.h"
#include "data_def.h"
#include "sharemem.h"
#include "datatypes.h"
#include <memory>
#include <vector> 
#include <map>
#include <ace/Event.h>
#include <QString>

struct NODE_MEM;
struct NODE;
class CFesDB;
class CServerDB;
class CClientDB;

class CDbSvc : public CBaseModule
{
public:
	CDbSvc(CScadaSvc* pServer, const std::string &szMailBoxName, int &MailID);
	virtual ~CDbSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
public:
	bool IsDBAlive(unsigned  int nTimeout);

	int32u GetMyNodeOccNo()
	{
		return m_nMyNodeOccNo;
	}
	size_t GetNodeCount()
	{
		return m_nNodeCount;
	}
	std::pair<unsigned int, NODE*> GetNodeData()
	{
		return std::make_pair(m_nNodeCount, m_pNodes);
	}
public:
	bool GetAinValue(int32u nNodeOccNo, int32u nOccNo, fp64 & nVal)const;
	bool GetDinValue(int32u nNodeOccNo, int32u nOccNo, int8u & nVal)const;
protected:
	//根据OccNo获取前置内存结构
	std::shared_ptr<CFesDB> GetFesDbByOccNo(int nOccNo);
	//获取每个节点内存大小
	size_t GetNodeSizeByOccNO(int nOccNo);
	//创建节点信息
	size_t BuildNodeDB(char* pAddr);
	//创建节点内存概述信息
	size_t BulidNodeGIInfo(char* pAddr);
private:
	//节点内存开辟模块
	std::shared_ptr<CShareMem> m_pMem;

	std::shared_ptr<ACE_Event> m_pDBAliveFlag;

	ACE_Event m_DBAliveFlag;
private:
	std::vector<std::shared_ptr<CFesDB>> m_arrFesDB;
	std::vector<std::shared_ptr<CServerDB>> m_arrServerDB;   //! server 只有一个
	std::vector<std::shared_ptr<CClientDB>> m_arrClientDB;
private:
	NODE_MEM* m_pNodeInfo;
	CFesDB* m_pFesDB;
	int32u m_nMyNodeOccNo;
	// 共享内存中的各数据的排布
	NODE*  m_pNodes;
	unsigned int m_nNodeCount;
private:
	//节点层次管理，可以获取所有节点映射关系
//	NODE_GRP_MGR_DEF m_pNodeGrpMgr;                           
	//OccNo映射到fes配置文件
	std::map < int32u, QString >m_mapFesConfig;
	//通过OccNo找到对应的节点大小
	std::map<int32u, size_t> m_mapNodeSize;
	//通过OccNo找到对应的前置关系
	std::map < int32u, std::shared_ptr<CFesDB> > m_mapFes;
};

#endif // _DBGSVR_MODULE_H


/** @}*/

