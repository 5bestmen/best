/*! @file fesnode_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fesnode_svc.cpp
�ļ�ʵ�ֹ��� :  ǰ�ýڵ���������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
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
#include "fesnode_svc.h"

#include <QObject> 
#include <QString> 
#include <QDebug>

#include "netbus/nbdef.h"
#include "log/log.h" 

#include "connector_mgr.h"
#include "nb_svc.h"

#include "fesapi/fescnst.h"

#include "acceptor_mgr.h"

CFesNodeSvc::~CFesNodeSvc( )
{

}

CFesNodeSvc::CFesNodeSvc(NET_CONFIG* pNodeConfig, int nCrtNodeIdx) : 
	m_pNetConfig(pNodeConfig), m_nCurrentNodeIndex(nCrtNodeIdx), CNodeSvc(pNodeConfig, nCrtNodeIdx)
{
	Q_ASSERT(pNodeConfig);
	Q_ASSERT(m_pNetConfig->pNodeConfig);
	Q_ASSERT(m_nCurrentNodeIndex >= 0);

	m_nSvrIndexA = -1;
	m_nSvrIndexB = -1;
	m_nAnotherNodeIndex = -1;
	m_nCurWorkingNet = -1;
	m_pConnectSvrA = nullptr;
	m_pConnectSvrB = nullptr;
	m_pConnectAnother = nullptr;
	m_pMySvr = nullptr;
	m_pRemoteSvr = nullptr;

	m_bIsDoubleSvr = false;
	m_bIsRedNode = false;
}

/*! \fn bool CFesNodeSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CFesNodeSvc::Initialize 
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
** \note  
********************************************************************************************************/
bool CFesNodeSvc::Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	Q_ASSERT(m_pNetConfig);
	Q_ASSERT(m_nCurrentNodeIndex >= 0 && m_nCurrentNodeIndex < m_pNetConfig->NodeCount);
	if (m_pNetConfig == nullptr || m_nCurrentNodeIndex < 0 || m_nCurrentNodeIndex >= m_pNetConfig->NodeCount)
	{
		return false;
	}

	ACE_UNUSED_ARG(pszDataPath);
	ACE_UNUSED_ARG(nMode);

	bool bExistSvrNode = false;

	// ���Ȳ��ҷ���ڵ�
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_SVR) // ����ڵ�
		{
			bExistSvrNode = true;

			// log ��ӡ����ڵ���Ϣ

			if (m_nSvrIndexA == -1)
			{
				m_nSvrIndexA = i;
			}
			else
			{
				m_nSvrIndexB = i;

				// ����˫��������
				m_bIsDoubleSvr = true;

				break;
			}
		}
	}

	if (!bExistSvrNode)
	{
		// log �������󣬲����ڷ���ڵ�
		return false;
	}


	// �Ƿ���� �Բ� �ڵ�
	int32u slaveOccNo = m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex].SlaveOccNo;
	if (slaveOccNo <= 0 || slaveOccNo > m_pNetConfig->NodeCount)
	{
		m_bIsRedNode = false;
	}
	else
	{
		for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
		{
			if (m_pNetConfig->pNodeConfig[i].OccNo == slaveOccNo && m_pNetConfig->pNodeConfig[i].NodeType == NODE_FES)
			{
				// ���ڶԲ�
				m_bIsRedNode = true;

				m_nAnotherNodeIndex = i;

				break;
			}
		}
	}

	if (m_bIsDoubleSvr)
	{ // ˫��������
		if (m_pNetConfig->IsDoubleNet)
		{ // AB��
			// ������A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);
			ACE_INET_Addr addrB_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[1]);
			vecAddressA.push_back(addrA_A);
			vecAddressA.push_back(addrB_A);
			
			m_pConnectSvrA = std::make_shared<CConnectorMgr>(true, vecAddressA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);

			// ������B
			std::vector<ACE_INET_Addr> vecAddressB;
			ACE_INET_Addr addrA_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[0]);
			ACE_INET_Addr addrB_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[1]);
			vecAddressB.push_back(addrA_B);
			vecAddressB.push_back(addrB_B);

			m_pConnectSvrB = std::make_shared<CConnectorMgr>(true, vecAddressB, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A��
			// ������A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(addrA_A, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);

			// ������B
			std::vector<ACE_INET_Addr> vecAddressB;
			ACE_INET_Addr addrA_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[0]);

			m_pConnectSvrB = std::make_shared<CConnectorMgr>(addrA_B, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}		
	}
	else
	{ // ������������

		if (m_pNetConfig->IsDoubleNet)
		{ // AB��
			// ������A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);
			ACE_INET_Addr addrB_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[1]);
			vecAddressA.push_back(addrA_A);
			vecAddressA.push_back(addrB_A);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(true, vecAddressA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A��
			// ������A
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}		
	}

	// �Բ�
	if (m_bIsRedNode)
	{
		if (m_pNetConfig->IsDoubleNet)
		{ // AB��

			std::vector<ACE_INET_Addr> vecAddr;
			ACE_INET_Addr addrA(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);
			ACE_INET_Addr addrB(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetBIPAddr);
			vecAddr.push_back(addrA);
			vecAddr.push_back(addrB);

			m_pConnectAnother = std::make_shared<CConnectorMgr>(true, vecAddr, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A��
			ACE_INET_Addr addrA(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);

			m_pConnectAnother = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}

		// ���ڶԲ�ʱ�������������񣬵ȴ��Բ�����
		// ��������ָ��IP���м���,fes��client����INADDR_ANY ���м���
		ACE_INET_Addr addr;
		addr.set(FES_PORT, (ACE_UINT32)INADDR_ANY);

		m_pMySvr = std::make_shared<CAcceptorMgr>(addr);
	}

	return true;
}

void CFesNodeSvc::Run()
{
	Q_ASSERT(m_pConnectSvrA);

	// ������������
	if (m_bIsDoubleSvr)
	{ // ˫��������
		Q_ASSERT(m_pConnectSvrB);

		if (!m_pConnectSvrA->Initialize())
		{
			if (!m_pConnectSvrB->Initialize())
			{
				// log ��ʼ������A B��������ʧ��
				return;
			}
			else
			{
				m_nCurWorkingNet = NET_B;
			}
		}
		else
		{ // ��������A start�ɹ�
			m_nCurWorkingNet = NET_A;

			m_pConnectSvrB->Initialize();
		}
	}
	else
	{ // ����������
		if (!m_pConnectSvrA->Initialize())
		{
			return;
		}

		m_nCurWorkingNet = NET_A;
	}	

	// ����ڵ��ж�
	if (!m_bIsRedNode)
	{
		return;
	}

	// ���ӶԲ�
	Q_ASSERT(m_pConnectAnother);
	m_pConnectAnother->Initialize();

	// �ȴ��Բ�����
	Q_ASSERT(m_pMySvr);
	m_pMySvr->Initialize();
}

void CFesNodeSvc::Shutdown()
{ 
	if (m_pConnectSvrA != nullptr)
	{
		m_pConnectSvrA->Shutdown();
	}

	if (m_pConnectSvrB != nullptr)
	{
		m_pConnectSvrB->Shutdown();
	}

	if (m_pConnectAnother != nullptr)
	{
		m_pConnectAnother->Shutdown();
	}

	if (m_pMySvr != nullptr)
	{
		m_pMySvr->Shutdown();
	}
}
/*! \fn int CFesNodeSvc::SendDataToServer(const int8u *pMsg, int32u nLen)
********************************************************************************************************* 
** \brief CFesNodeSvc::SendDataToServer 
** \details ��ǰ�÷������ݵ���������
** \param pMsg 
** \param nLen 
** \return int 
** \author   
** \date 2017��1��12�� 
** \note 
********************************************************************************************************/
bool CFesNodeSvc::SendDataToServer(const int8u *pMsg, size_t nLen, int32u msgLevel/* = GENERAL_MSG*/)
{
	Q_ASSERT(pMsg && nLen > EMSG_BUF_HEAD_SIZE);
	if (pMsg == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}

	bool bRet = false;

	Q_ASSERT(m_pConnectSvrA);
	if (m_pConnectSvrA)
	{
		bRet = m_pConnectSvrA->SendData(pMsg, nLen, msgLevel);
	}

	return bRet;
#if 0
	// todo:�Զ��л����ֶ��л�˼����
	if (m_bIsDoubleSvr)
	{
		if (m_nCurWorkingNet > NET_B)
		{
			Q_ASSERT(false);
			m_nCurWorkingNet = NET_A;
		}
		// ����
		switch (m_nCurWorkingNet)
		{
		case NET_A:
			return m_pConnectSvrA->SendData(pMsg, nLen);
		case NET_B:
			return m_pConnectSvrB->SendData(pMsg, nLen);
		default:
			break;
		}
	}
	else
	{
		Q_ASSERT(m_pConnectSvrA != nullptr);
		if (m_pConnectSvrA)
		{
			return m_pConnectSvrA->SendData(pMsg, nLen);
		}
	}
#endif
	return 0;
}

bool CFesNodeSvc::SendDataToSlave(const int8u *pMsg, size_t nLen, int32u msgLevel/* = GENERAL_MSG*/)
{
	/*Q_ASSERT(pMsg && nLen > EMSG_BUF_SIZE);
	if (pMsg == nullptr || nLen <= EMSG_BUF_SIZE)
	{
		return 0;
	}

	if (m_bIsDoubleSvr)
	{
		return m_pConnectAnother->SendData(pMsg, nLen);
	}

	return 0;*/

	return true;
}

/*! \fn int CFesNodeSvc::SendData(const int8u * pMsg, int32u nLen)
********************************************************************************************************* 
** \brief CFesNodeSvc::SendData 
** \details �������ݣ�ֻ��д���ڲ��������
** \param pMsg 
** \param nLen 
** \return int 
** \author LiJin 
** \date 2016��11��28�� 
** \note 
********************************************************************************************************/
bool CFesNodeSvc::SendData(const int8u * pMsg, int32u nLen)
{
	Q_ASSERT(pMsg && nLen > EMSG_BUF_HEAD_SIZE);
	if (pMsg == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}
	// ע����Ҫ������Ŀ�ĵأ���ͬ��Ŀ�ĵأ����Ķ��в�һ��
	EMSG_BUF *pBuf = reinterpret_cast<EMSG_BUF*>(const_cast <int8u *>(pMsg));

	// test
	if (0)
	{
		char* temp = new char[pBuf->MsgDataSize];

		ACE_OS::memcpy(temp, pBuf->BuffData, pBuf->MsgDataSize);
	}

	bool bRet = false;
	switch (pBuf->MsgPath)
	{
	case TO_SERVER:
		// todo:��Ϣ������Ϊ����(��ֵԽ����Ϣ����Խ�ߣ�������Ҫ����)
		bRet = SendDataToServer(pMsg, nLen, pBuf->MsgType);
		break;
	case TO_FES:
		bRet = SendDataToSlave(pMsg, nLen, pBuf->MsgType);
		break;
	default:
		break;
	}

	//return SendDataToServer(pMsg, nLen);
	return bRet;
}

/*! \fn std::pair<std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>>  CFesNodeSvc::GetCurServerList()
********************************************************************************************************* 
** \brief CFesNodeSvc::GetCurServerList 
** \details ��ȡ��ǰ���ӷ������б���һ������ֵ�� ��������ָ�룬�ڶ����Ǵӷ�������
** \return std::pair<std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>> 
** \author LiJin 
** \date 2017��1��12�� 
** \note 
********************************************************************************************************/
std::pair<std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>>  CFesNodeSvc::GetCurServerList()
{
	return  std::make_pair(m_pConnectSvrA, m_pConnectSvrB);
}

/*! \fn bool CFesNodeSvc::RecvData(const int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout)
********************************************************************************************************* 
** \brief CFesNodeSvc::RecvData 
** \details 
** \param pMsg ���뻺����
** \param nMsgLen ��Ҫ�ĳ���
** \param nCopySize ���صĲ���---ʵ�ʽ��յĳ���
** \param ulTimeout ��ʱ����λ����
** \return bool 
** \author LiJin 
** \date 2017��1��12�� 
** \note 
********************************************************************************************************/
bool CFesNodeSvc::RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout)
{
	// ��Ҫ�������е�socket����
	Q_ASSERT(pMsg && nMsgLen);
	if (pMsg == nullptr || nMsgLen < EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}
	// �������ȼ� �Ƚ����������������ݣ��ӷ�����������

	size_t nRet = 0;
	if (m_bIsDoubleSvr)
	{
		// �жϵ�ǰ������������A����B��
		auto pCurSvr = GetCurServerList();
		if (pCurSvr.first)
		{ 
			nRet = pCurSvr.first->RecvData(pMsg, nMsgLen, ulTimeout);
			nCopySize = nRet;
			if (nRet > EMSG_BUF_HEAD_SIZE)
			{
				return true;
			}
			else
			{
				nRet = pCurSvr.second->RecvData(pMsg, nMsgLen, ulTimeout);
				nCopySize = nRet;
				if (nRet > EMSG_BUF_HEAD_SIZE)
				{
					return true;
				}
			}
		}
	}
	else
	{// ������������ֻ������A
		if (m_pConnectSvrA)
		{
			nRet = m_pConnectSvrA->RecvData(pMsg, nMsgLen, ulTimeout);
			nCopySize = nRet;
			if (nRet > EMSG_BUF_HEAD_SIZE)
			{
				return true;
			}
		}
	}
	// ���� ����ͬ������


 
	return false;
}

 
/** @}*/
