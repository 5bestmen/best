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
#include <memory>
#include <vector>
#include <map>
#include <QDomElement>
#include <ace/Event.h>
struct NODE;
class CFesDB;
class CServerDB;
class CClientDB;
class CScadaDB;

struct TAG_OCCNO;
struct SAPP;

typedef std::shared_ptr<SAPP> APP_INFO_DEF;
typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;
using AIN_ALARM_DEF = std::shared_ptr<AIN_ALARM>;
using DIN_ALARM_DEF = std::shared_ptr<DIN_ALARM>;
using AIN_ALARM_LIMIT_DEF = std::shared_ptr<AIN_ALARM_LIMIT>;
using DIN_ALARM_LIMIT_DEF = std::shared_ptr<DIN_ALARM_LIMIT>;


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

	void CreateMailBoxs();
	void DestroyMailBoxs();

	// 获取本机的节点
	CServerDB * GetMyNodeDB();

	int32u GetMyNodeOccNo()const 
	{
		return m_nMyNodeOccNo;
	}
	size_t GetNodeCount() const
	{
		return m_nNodeCount;
	}
	std::pair<unsigned int, NODE*> GetNodeData()
	{
		return std::make_pair(m_nNodeCount, m_pNodes);
	}

	bool  GetNodeTagNameByOccNo(int32u nOccNo,std::string& tagName) const;
	int32u GetNodeOccNoByTagName(const std::string& tagName) const;
public:
	bool GetAinAlarmByOccNo(int32u nNodeOccNo,int32u nOccNo,AIN_ALARM** pAin);
	bool GetAinAlarmLimitByOccNo(int32u nNodeOccNo, int32u nOccNo,AIN_ALARM_LIMIT** pAinLimit);
	bool GetDinAlarmByOccNo(int32u nNodeOccNo, int32u nOccNo,DIN_ALARM** pDin);
	bool GetDinAlarmLimitByOccNo(int32u nNodeOccNo, int32u nOccNo,DIN_ALARM_LIMIT** pDinLimit);
	int GetNodeAppCount(int nOccNo);
	SAPP * GetNodeAppAddr(int32u nOccNo,int nIndex);
public:
	bool UpdateAinValue(int32u nNodeOccNo, int32u nAinOccNo, fp64  nVal);
	bool UpdateDinValue(int32u nNodeOccNo, int32u nOccNo, int8u  nVal);
protected:
	//加载整个工程文件
	bool LoadProject(const char *pszFilePath);
	//加载节点配置文件
	bool LoadNodeFile(const char *pszFilePath);
	//创建scada内存文件
	bool BuildScdMem(const char* pszFilePath);
	//创建节点内存文件
	bool BuildNodeMem(const char *pszFilePath);
	//创建各节点tagname
	bool BuildTagNameMen(const char* pszFilePath);
	//根据OccNo获取前置内存结构
	std::shared_ptr<CFesDB> GetFesDbByOccNo(int nOccNo);
	//获取每个节点内存大小
	size_t GetNodeSizeByOccNO(int nOccNo);
	//创建节点信息
	size_t BuildNodeDB(char* pAddr);
	//创建节点内存概述信息
	size_t BuildNodeGIInfo(char* pAddr,NODE_TYPE nType);
	//创建节点内存具体厂站设备点信息
	size_t BuildNodeTotalInfo(char* pAddr,NODE_TYPE nType);

	size_t BuildAppDB(char* pAddr);

	size_t BuildAppNodeDB(char* pAddr,int32u nOccNo,std::vector<std::shared_ptr<SAPP>>& arrAppNodes);

	size_t BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec);
private:
	//节点内存开辟模块
	std::shared_ptr<CShareMem> m_pMem;
	//tagname内存开辟
	std::shared_ptr<CShareMem> m_pTagNameMem;
	//各工作站变量缓存和scada变量缓存
	std::shared_ptr<CShareMem> m_pScadaMem;
	//scada存活标记
	std::shared_ptr<ACE_Event> m_pDBAliveFlag;

	ACE_Event m_DBAliveFlag;
private:
	std::vector<std::shared_ptr<CFesDB>> m_arrFesDB;
	std::vector<std::shared_ptr<CServerDB>> m_arrServerDB;   //! server 只有一个
	std::vector<std::shared_ptr<CClientDB>> m_arrClientDB;
	std::vector<std::shared_ptr<NODE>>    m_arrTempNodes;
private:
	NODE_MEM* m_pNodeInfo;
	CFesDB* m_pFesDB;
	CServerDB* m_pSvrDB;
	CClientDB* m_pClientDB;
	int32u m_nMyNodeOccNo;
	// 共享内存中的各数据的排布
	NODE*  m_pNodes;
	unsigned int m_nNodeCount;
private:
	//节点层次管理，可以获取所有节点映射关系
	NODE_GRP_MGR_DEF m_pNodeGrpMgr;                           
	//从工程文件中读取所有前置文件列表，供验证使用
	std::vector < std ::string > m_arrTempFesTagNames;
	//OccNo映射到fes配置文件
	std::map < int32u, QString >m_mapFesConfig;	
	//OccNo映射到scada配置文件
	std::map<int32u, QString> m_mapScdConfig;
	//OccNO映射到workstationswen文件
	std::map<int32u, QString> m_mapWorkStationConfig;
	//通过OccNo找到对应的节点大小
	std::map<int32u, size_t> m_mapNodeSize;
	//通过OccNo找到对应的前置关系
	std::map < int32u, std::shared_ptr<CFesDB> > m_mapFes;
	//通过occno找到对应的scada
	std::map < int32u, std::shared_ptr<CServerDB>>m_mapServer;
	//通过occno找到对应workstation
	std::map < int32u, std::shared_ptr<CClientDB>>m_mapClient;
	//occno   :: tagname
	std::map<int32u, std::string> m_MapID2String;
	//tagname :: occno
	std::map<std::string, int32u> m_MapString2ID;
	//node tag_occno
	std::vector<TAG_OCCNO_DEF> m_arrNodeTagName;

	std::map < int32u, std::vector<std::shared_ptr< SAPP > > > m_mapApps;
	//内存中映射地址
	std::map<int32u, SAPP*> m_arrAppAddr;

	std::map<int32u, int> m_arrAppNums;
private:
	bool LoadServer(QDomElement nEle, NODE_GROUP_DEF pGrp);
	bool LoadClient(QDomElement nEle, NODE_GROUP_DEF pGrp);
	bool LoadFes(QDomElement nEle, NODE_GROUP_DEF pGrp);
	bool LoadGroup(QDomElement nEle, NODE_GROUP_DEF pGrp);
	bool LoadNode(QDomElement nEle, NODE_GROUP_DEF pGrp);
	bool LoadApps(QDomElement nEle,int32u nOccNo);
	bool CheckOccNoExist(int32u nOccNo);
private:
	QDomElement m_DomServer;
	QDomElement m_DomClient;
	QDomElement m_DomFes;
};

#endif // _DBGSVR_MODULE_H


/** @}*/

