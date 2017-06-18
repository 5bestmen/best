/*! @file node_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  node_svc.h
�ļ�ʵ�ֹ��� :  �ڵ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ڵ����
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef NODE_SVC_MODULE_H
#define NODE_SVC_MODULE_H  

#include "datatypes.h"

#include <string>
#include <atomic>
#include <vector>

#include "netbus/nbdef.h"
#include "node_manager.h"
 
class CNodeSvc  
{
public:  
	CNodeSvc(NET_CONFIG* pNetConfig, int32u nCurrentNodeIndex);
	virtual ~CNodeSvc();
public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
public:
	virtual bool SendData(const int8u * pMsg, int32u nLen);
	virtual bool RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout = 0);

	void SetNodeState(int nState);
	int GetNodeState() const
	{
		return m_nNodeState;
	}

	virtual int GetAnotherNodeIndex() = 0;

	// �ڵ����(������ڵ���н�ǰ�û��͹���վ����)
	bool NodeSplit();

	// ����������Ϣ
	NET_CONFIG* m_pNetConfig;

	int32u m_nCurrentNodeIndex;

	struct NODE_INFO
	{
		CONNECTION_IP addr;
		int32u nDestOccNo;
	};

	// ���ǰ�ýڵ�IP
	std::vector<NODE_INFO> m_vecFesNodeIP;

	// ��Ź���վ�ڵ�IP
	std::vector<NODE_INFO> m_vecWorkStationNodeIP;

protected:
	void LogMsg(const char *szLogTxt, int nLevel);
protected:
	std::atomic <int> m_nNodeState;  //! �ڵ�״̬

};

#endif // NODE_SVC_MODULE_H


/** @}*/

