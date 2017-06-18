/*! @file client_node_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  client_node_svc.h
�ļ�ʵ�ֹ��� :  ����վ�ڵ���������
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
*  @brief   ����վ�ڵ���������
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef CLIENT_NODE_SVC_MODULE_H
#define CLIENT_NODE_SVC_MODULE_H  

#include "node_svc.h"

#include <string>
#include "datatypes.h"

#include <memory>

class CConnectorMgr;
class CAcceptorMgr;
struct NET_CONFIG;
class CClientNodeSvc  : public CNodeSvc
{
public:  
	CClientNodeSvc(NET_CONFIG* &pNetConfig, int nCrtNodeIdx);
	virtual ~CClientNodeSvc();
protected:

public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
 
	virtual int GetAnotherNodeIndex()
	{
		return m_nAnotherNodeIndex;
	}

protected:
	std::shared_ptr<CConnectorMgr> m_pConnectSvrA;  //! ��Ϊ�ͻ���������������A
	std::shared_ptr<CConnectorMgr> m_pConnectSvrB;  //! ��Ϊ�ͻ���������������B

	std::shared_ptr<CConnectorMgr> m_pConnectPeer;  //! ��Ϊ�ͻ������ӶԲ�

	//! ��������ʱ���ӻ���������������
	std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! �������ڵ��Զ�����ݷ���
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr; 
	//! �Ƿ�˫��������
	bool m_bDoubleSvr;
	//! ���ڵ��Ƿ�����ڵ�
	bool m_bIsRedNode;

private:
	NET_CONFIG* m_pNetCfgMain;
	NET_CONFIG* m_pNetCfgA;

	// ����ڵ��
	NET_CONFIG* m_pNetCfg;

	// ��ǰ�ڵ�ĶԲ�ڵ�����
	int m_nAnotherNodeIndex;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/


