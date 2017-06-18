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
	//����OccNo��ȡǰ���ڴ�ṹ
	std::shared_ptr<CFesDB> GetFesDbByOccNo(int nOccNo);
	//��ȡÿ���ڵ��ڴ��С
	size_t GetNodeSizeByOccNO(int nOccNo);
	//�����ڵ���Ϣ
	size_t BuildNodeDB(char* pAddr);
	//�����ڵ��ڴ������Ϣ
	size_t BulidNodeGIInfo(char* pAddr);
private:
	//�ڵ��ڴ濪��ģ��
	std::shared_ptr<CShareMem> m_pMem;

	std::shared_ptr<ACE_Event> m_pDBAliveFlag;

	ACE_Event m_DBAliveFlag;
private:
	std::vector<std::shared_ptr<CFesDB>> m_arrFesDB;
	std::vector<std::shared_ptr<CServerDB>> m_arrServerDB;   //! server ֻ��һ��
	std::vector<std::shared_ptr<CClientDB>> m_arrClientDB;
private:
	NODE_MEM* m_pNodeInfo;
	CFesDB* m_pFesDB;
	int32u m_nMyNodeOccNo;
	// �����ڴ��еĸ����ݵ��Ų�
	NODE*  m_pNodes;
	unsigned int m_nNodeCount;
private:
	//�ڵ��ι������Ի�ȡ���нڵ�ӳ���ϵ
//	NODE_GRP_MGR_DEF m_pNodeGrpMgr;                           
	//OccNoӳ�䵽fes�����ļ�
	std::map < int32u, QString >m_mapFesConfig;
	//ͨ��OccNo�ҵ���Ӧ�Ľڵ��С
	std::map<int32u, size_t> m_mapNodeSize;
	//ͨ��OccNo�ҵ���Ӧ��ǰ�ù�ϵ
	std::map < int32u, std::shared_ptr<CFesDB> > m_mapFes;
};

#endif // _DBGSVR_MODULE_H


/** @}*/

