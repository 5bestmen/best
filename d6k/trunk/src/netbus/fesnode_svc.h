/*! @file fesnode_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fesnode_svc.h
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
#ifndef FESNODE_SVC_MODULE_H
#define FESNODE_SVC_MODULE_H  

#include "node_svc.h"

#include <string>
#include <memory>

#include "datatypes.h"
#include "netbus/nbdef.h"

struct NET_CONFIG;
class CConnectorMgr;
class CAcceptorMgr;

class CFesNodeSvc  : public CNodeSvc
{
	enum :int
	{
		NET_A = 0,
		NET_B = 1,
	};

public:

	CFesNodeSvc(NET_CONFIG* pNetConfig, int nCrtNodeIdx);
	virtual ~CFesNodeSvc();

public:

	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 

public:

	virtual bool SendData(const int8u * pMsg, int32u nLen);
	virtual bool RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout/* = 0*/);

	virtual int GetAnotherNodeIndex()
	{
		return m_nAnotherNodeIndex;
	}
protected:
	// ��ȡ��ǰ�����ӷ�����
	std::pair< std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>>  GetCurServerList();
	// �������ݵ���������
	bool SendDataToServer(const int8u *pMsg, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// �������ݵ��ӻ�
	bool SendDataToSlave(const int8u *pMsg, size_t nLen, int32u msgLevel = GENERAL_MSG);

protected:
	std::atomic<unsigned int>  m_nCurWorkingNet;   //! ��ǰ���ӷ���

	std::shared_ptr<CConnectorMgr> m_pConnectSvrA;  //! ��Ϊ�ͻ���������������A
	std::shared_ptr<CConnectorMgr> m_pConnectSvrB;  //! ��Ϊ�ͻ���������������B

	std::shared_ptr<CConnectorMgr> m_pConnectAnother;  //! ��Ϊ�ͻ������ӶԲ�

	//! ǰ�ýڵ㣬��������ʱ���ӻ���������������
	std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! �������ڵ��Զ�����ݷ���
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr;

	//! �Ƿ�˫��������
	bool m_bIsDoubleSvr;

	// ��������A����
	int m_nSvrIndexA;

	// ��������B����
	int m_nSvrIndexB;

	//! ���ڵ��Ƿ�����ڵ�
	bool m_bIsRedNode;

	// ����������Ϣ
	NET_CONFIG* m_pNetConfig;

	// ��ǰ�ڵ�����
	int32u m_nCurrentNodeIndex;

	// ��ǰ�ڵ�ĶԲ�ڵ�����
	int m_nAnotherNodeIndex;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/

