/*! @file db_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  db_svc.h
�ļ�ʵ�ֹ��� :  �ڴ����ݿ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ע�⣺��ǰ�ýṹ��ͬ���ǣ�node��db_svc��һ��������
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ڴ����ݿ�
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

	// ��ȡ�����Ľڵ�
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
	//�������������ļ�
	bool LoadProject(const char *pszFilePath);
	//���ؽڵ������ļ�
	bool LoadNodeFile(const char *pszFilePath);
	//����scada�ڴ��ļ�
	bool BuildScdMem(const char* pszFilePath);
	//�����ڵ��ڴ��ļ�
	bool BuildNodeMem(const char *pszFilePath);
	//�������ڵ�tagname
	bool BuildTagNameMen(const char* pszFilePath);
	//����OccNo��ȡǰ���ڴ�ṹ
	std::shared_ptr<CFesDB> GetFesDbByOccNo(int nOccNo);
	//��ȡÿ���ڵ��ڴ��С
	size_t GetNodeSizeByOccNO(int nOccNo);
	//�����ڵ���Ϣ
	size_t BuildNodeDB(char* pAddr);
	//�����ڵ��ڴ������Ϣ
	size_t BuildNodeGIInfo(char* pAddr,NODE_TYPE nType);
	//�����ڵ��ڴ���峧վ�豸����Ϣ
	size_t BuildNodeTotalInfo(char* pAddr,NODE_TYPE nType);

	size_t BuildAppDB(char* pAddr);

	size_t BuildAppNodeDB(char* pAddr,int32u nOccNo,std::vector<std::shared_ptr<SAPP>>& arrAppNodes);

	size_t BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec);
private:
	//�ڵ��ڴ濪��ģ��
	std::shared_ptr<CShareMem> m_pMem;
	//tagname�ڴ濪��
	std::shared_ptr<CShareMem> m_pTagNameMem;
	//������վ���������scada��������
	std::shared_ptr<CShareMem> m_pScadaMem;
	//scada�����
	std::shared_ptr<ACE_Event> m_pDBAliveFlag;

	ACE_Event m_DBAliveFlag;
private:
	std::vector<std::shared_ptr<CFesDB>> m_arrFesDB;
	std::vector<std::shared_ptr<CServerDB>> m_arrServerDB;   //! server ֻ��һ��
	std::vector<std::shared_ptr<CClientDB>> m_arrClientDB;
	std::vector<std::shared_ptr<NODE>>    m_arrTempNodes;
private:
	NODE_MEM* m_pNodeInfo;
	CFesDB* m_pFesDB;
	CServerDB* m_pSvrDB;
	CClientDB* m_pClientDB;
	int32u m_nMyNodeOccNo;
	// �����ڴ��еĸ����ݵ��Ų�
	NODE*  m_pNodes;
	unsigned int m_nNodeCount;
private:
	//�ڵ��ι������Ի�ȡ���нڵ�ӳ���ϵ
	NODE_GRP_MGR_DEF m_pNodeGrpMgr;                           
	//�ӹ����ļ��ж�ȡ����ǰ���ļ��б�����֤ʹ��
	std::vector < std ::string > m_arrTempFesTagNames;
	//OccNoӳ�䵽fes�����ļ�
	std::map < int32u, QString >m_mapFesConfig;	
	//OccNoӳ�䵽scada�����ļ�
	std::map<int32u, QString> m_mapScdConfig;
	//OccNOӳ�䵽workstationswen�ļ�
	std::map<int32u, QString> m_mapWorkStationConfig;
	//ͨ��OccNo�ҵ���Ӧ�Ľڵ��С
	std::map<int32u, size_t> m_mapNodeSize;
	//ͨ��OccNo�ҵ���Ӧ��ǰ�ù�ϵ
	std::map < int32u, std::shared_ptr<CFesDB> > m_mapFes;
	//ͨ��occno�ҵ���Ӧ��scada
	std::map < int32u, std::shared_ptr<CServerDB>>m_mapServer;
	//ͨ��occno�ҵ���Ӧworkstation
	std::map < int32u, std::shared_ptr<CClientDB>>m_mapClient;
	//occno   :: tagname
	std::map<int32u, std::string> m_MapID2String;
	//tagname :: occno
	std::map<std::string, int32u> m_MapString2ID;
	//node tag_occno
	std::vector<TAG_OCCNO_DEF> m_arrNodeTagName;

	std::map < int32u, std::vector<std::shared_ptr< SAPP > > > m_mapApps;
	//�ڴ���ӳ���ַ
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

