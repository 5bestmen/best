/*! @file server_node_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  server_node_svc.h
�ļ�ʵ�ֹ��� :  ǰ�ýڵ���������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  1.��Ϊclientȥ����server
                2.����ڴӻ�״̬����Ϊclientȥ��������
				3.���������״̬�£��ȴ��ӻ�������
				4.��Ϊserver��������Ӧ���ṩ����
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ǰ�ýڵ���������
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef SERVER_NODE_SVC_MODULE_H
#define SERVER_NODE_SVC_MODULE_H  

#include "node_svc.h"

#include <string>
#include <memory>

#include "datatypes.h"
#include "acceptor_mgr.h"
#include "connector_mgr.h"

#include "node_manager.h"

class CConnectorMgr;
class CAcceptorMgr;
struct NET_CONFIG;

class CServerNodeSvc  : public CNodeSvc
{

public:

	CServerNodeSvc(NET_CONFIG *pNetConfig, int nCrtNodeIdx);
	virtual ~CServerNodeSvc();

public:

	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
 
	virtual bool SendData(const int8u * pMsg, int32u nLen);
	virtual bool RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout/* = 0*/);

	virtual int GetAnotherNodeIndex()
	{
		return m_nAnotherNodeIndex;
	}
protected:

private:

	int AsynchSend(ACE_Asynch_Write_Stream& asynchWrite, const int8u * pMsg, int32u nLen);

private:

	// �ڵ����(������ڵ���н�ǰ�û��͹���վ����)
	//bool NodeSplit();

	/*struct NODE_INFO
	{
		CONNECTION_IP
	};

	// ���ǰ�ýڵ�IP
	std::vector<CONNECTION_IP> m_vecFesNodeIP;

	// ��Ź���վ�ڵ�IP
	std::vector<CONNECTION_IP> m_vecWorkStationNodeIP;*/

private:
	//! ��Ϊ�ͻ������ӶԲ�
	std::shared_ptr<CConnectorMgr> m_pConnectPeer;

	//! ��������ʱ���ӻ���������������
	//std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! �������ڵ��Զ�����ݷ���
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr;

	//! �Ƿ�˫��������
	bool m_bDoubleSvr;

	//! ���ڵ��Ƿ�����ڵ�
	bool m_bIsRedNode;

	// ����������Ϣ
	NET_CONFIG* m_pNetConfig;

	// ��ǰ�ڵ�����
	int m_nCurrentNodeIndex;

	// ��ǰ�ڵ�ĶԲ�ڵ�����
	int m_nAnotherNodeIndex;
};

#endif // SERVER_NODE_SVC_MODULE_H


/** @}*/

