/*! @file server_node_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  server_node_svc.cpp
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
#include "server_node_svc.h"

#include "log/log.h" 
#include "nb_svc.h"
#include <QObject> 
#include <QString> 

#include "ace/config-macros.h"
#include "ace/INET_Addr.h"
#include "ace/Assert.h"

#include "netbus/nbdef.h"
#include "fesapi/fesapi.h"

CMsgQueue& g_msgQueue = CMsgQueue::GetInstance();

CServerNodeSvc::CServerNodeSvc(NET_CONFIG *pNetConfig, int nCrtNodeIdx) : 
	m_pNetConfig(pNetConfig), m_nCurrentNodeIndex(nCrtNodeIdx), CNodeSvc(pNetConfig, nCrtNodeIdx)
{
	Q_ASSERT(m_pNetConfig);
	Q_ASSERT(m_nCurrentNodeIndex >= 0);

	m_pConnectPeer = nullptr;
	//m_pMySvr = nullptr;
	m_pRemoteSvr = nullptr;
	m_bDoubleSvr = false;
	m_bIsRedNode = false;

	m_nAnotherNodeIndex = -1;
}

CServerNodeSvc::~CServerNodeSvc( )
{
	//if (m_pRemoteSvr != nullptr)
	//{
	//	m_pRemoteSvr.reset();
	//}
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
bool CServerNodeSvc::Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	ACE_ASSERT(m_pNetConfig);
	Q_ASSERT(m_nCurrentNodeIndex >= 0 && m_nCurrentNodeIndex < m_pNetConfig->NodeCount);
	if (m_pNetConfig == nullptr || m_nCurrentNodeIndex < 0 || m_nCurrentNodeIndex >= m_pNetConfig->NodeCount)
	{
		return false;
	}

	// AB��������socket����
	ACE_UNUSED_ARG(pszDataPath);
	ACE_UNUSED_ARG(nMode);

	// �Ƿ���� �Բ� �ڵ�
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (i == m_nCurrentNodeIndex)
		{
			continue;
		}

		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_SVR) // SCADA�ڵ�
		{
			// log ���ڶԲ�ڵ�
			m_nAnotherNodeIndex = i;

			if (m_nCurrentNodeIndex < m_nAnotherNodeIndex)
			{
				// todo ˼�����������жϽڵ���ǲ����Ѿ�����������.
				m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex].HostSlave = 1; // ��
				m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].HostSlave = 0; // ��
			}

			// ���ڶԲ�
			m_bIsRedNode = true;

			break;
		}
	}

	// �Բ��ж�
	if (m_bIsRedNode)
	{
		if (m_pNetConfig->IsDoubleNet)
		{ // AB��

			std::vector<ACE_INET_Addr> vecAddr;
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);
			ACE_INET_Addr addrB(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetBIPAddr);
			vecAddr.push_back(addrA);
			vecAddr.push_back(addrB);

			m_pConnectPeer = std::make_shared<CConnectorMgr>(true, vecAddr, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A��
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);

			m_pConnectPeer = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}

		// ���ڶԲ�ʱ�������������񣬵ȴ��Բ�����
		// ��������ָ��IP���м���,fes��client����INADDR_ANY ���м���
		/*ACE_INET_Addr addr;
		addr.set((u_short)SERVER_PORT, (ACE_UINT32)INADDR_ANY);

		m_pMySvr = std::make_shared<CAcceptorMgr>(addr);*/
	}

	// �����ڵ��Զ�����ݷ���
	ACE_INET_Addr addr;
	addr.set((u_short)SERVER_PORT, (ACE_UINT32)INADDR_ANY);

	m_pRemoteSvr = std::make_shared<CAcceptorMgr>(addr);

	// �ڵ����
	if (!NodeSplit())
	{
		return false;
	}

	return true;
}

void CServerNodeSvc::Run()
{
	ACE_ASSERT(m_pRemoteSvr);
	if (!m_pRemoteSvr->Initialize())
	{
		// log: Զ�����ݷ�������ʧ��
		return;
	}

	// ����ڵ��ж�
	if (!m_bIsRedNode)
	{
		return;
	}

	// ���ӶԲ�
	ACE_ASSERT(m_pConnectPeer);
	if (!m_pConnectPeer->Initialize())
	{
		// log: ���ӿͻ���ʧ��
		return;
	}

	// �ȴ��Բ�����
	/*ACE_ASSERT(m_pMySvr);
	if (!m_pMySvr->Initialize())
	{
		// log: �ȴ��Բ����ӳ�ʼ��ʧ��
		return;
	}*/
}

void CServerNodeSvc::Shutdown()
{ 
	if (m_pRemoteSvr != nullptr)
	{
		m_pRemoteSvr->Shutdown();
	}

	/*if (m_pMySvr != nullptr)
	{
		m_pMySvr->Shutdown();
	}*/

	if (m_pConnectPeer != nullptr)
	{
		m_pConnectPeer->Shutdown();
	}
}

bool CServerNodeSvc::SendData(const int8u * pMsg, int32u nLen)
{
	Q_ASSERT(pMsg && nLen > EMSG_BUF_HEAD_SIZE);
	if (pMsg == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}

	size_t nodeSize = M_SINGLETON::instance()->m_connMap.current_size();
	if (nodeSize <= 0)
	{
		// log ��ǰû�����ӵ�������Ľڵ�
		return false;
	}

	/*CONNECTION_MAP_ITERATOR iter(M_SINGLETON::instance()->m_connMap);

	for (CONNECTION_MAP_ENTRY * i = 0; iter.next(i) != 0; iter.advance())
	{
		CNodeManager* pNode = i->int_id_;
	}*/

	// ע����Ҫ������Ŀ�ĵأ���ͬ��Ŀ�ĵأ����Ķ��в�һ��
	EMSG_BUF *pBuf = reinterpret_cast<EMSG_BUF*>(const_cast <int8u *>(pMsg));

	// test
	if (0)
	{
		char* temp = new char[pBuf->MsgDataSize];

		ACE_OS::memcpy(temp, pBuf->BuffData, pBuf->MsgDataSize);
	}
	int nRet = 0;
	switch (pBuf->MsgPath)
	{
		// �����������ݵ�ǰ��
		case TO_FES:
		{
			if (m_vecFesNodeIP.size() == 0)
			{
				// log:ǰ�ýڵ���ʱδ����
				// todo:����ȷ����������Ƿ񱣴�
				return false;
			}

			std::vector<NODE_INFO>::iterator iter = m_vecFesNodeIP.begin();
			for (; iter != m_vecFesNodeIP.end(); iter++)
			{
				CNodeManager* pNodeManager = nullptr;
				if (-1 == M_SINGLETON::instance()->m_connMap.find((*iter).addr, pNodeManager))
				{
					continue;
				}

				// ��ǰ�ýڵ�����
				// ��������
				char* data = new char[nLen + 1];
				data[0] = ORIGIN_FLAG;
				ACE_OS::memcpy(data + 1, pMsg, nLen);
				nLen += 1;
				if (0 == AsynchSend(pNodeManager->Get_Asynch_Write_Stream(), (const int8u*)(data), nLen))
				{
					nRet = true;
				}

				delete [] data;
			}

			break;
		}
		// �����������ݵ�����վ
		case TO_CLIENT:
		{
			if (m_vecWorkStationNodeIP.size() == 0)
			{
				// log:����վ�ڵ���ʱδ����
				// todo:����ȷ����������Ƿ񱣴�
				return false;
			}

			std::vector<NODE_INFO>::iterator iter = m_vecWorkStationNodeIP.begin();
			for (; iter != m_vecWorkStationNodeIP.end(); iter++)
			{
				CNodeManager* pNodeManager = nullptr;
				if (-1 == M_SINGLETON::instance()->m_connMap.find((*iter).addr, pNodeManager))
				{
					continue;
				}

				// �й���վ�ڵ�����
				// ��������
				if(0 == AsynchSend(pNodeManager->Get_Asynch_Write_Stream(), pMsg, nLen))
				{
					nRet = true;
				}
			}

			break;
		}
		// server������Ϣ��server��Ŀǰֻ��׼���л�����
		case TO_SERVER:
		{
			if (m_nAnotherNodeIndex < 0 || m_nAnotherNodeIndex >= m_pNetConfig->NodeCount || !m_bIsRedNode)
			{
				return false;
			}

			const char* ip = m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].chIP[0];
			ACE_UINT64 addr = ACE_OS::inet_addr(ip);
			CNodeManager* pNodeManager = nullptr;
			if (-1 == M_SINGLETON::instance()->m_connMap.find(addr, pNodeManager))
			{
				return false;
			}

			// �жԶ˷���������
			// ��������
			if (0 == AsynchSend(pNodeManager->Get_Asynch_Write_Stream(), pMsg, nLen))
			{
				nRet = true;
			}

			break;
		}
		default:
		{
			// log:��Ϣ·������
			break;
		}
	}

	//return SendDataToServer(pMsg, nLen);
	return nRet;
}

int CServerNodeSvc::AsynchSend(ACE_Asynch_Write_Stream& asynchWrite, const int8u * pMsg, int32u nLen)
{
	ACE_ASSERT(pMsg && nLen <= MAX_EMSG_L - 1);
	if (pMsg == nullptr || nLen > MAX_EMSG_L - 1)
	{
		return -1;
	}

	ACE_Message_Block *mb = nullptr;
	ACE_NEW_RETURN(mb, ACE_Message_Block(nLen + 1), -1);
	mb->wr_ptr((char*)pMsg);
	ACE_OS::memcpy(mb->base(), (char*)pMsg, nLen);

	//�����ϵͳ��������
	if (asynchWrite.write(*mb, nLen) == -1)
	{
		return -1;
	}
	return 0;
}

// �ڵ����(������ڵ���н�ǰ�û��͹���վ����)
/*bool CServerNodeSvc::NodeSplit()
{
	ACE_ASSERT(m_pNetConfig && m_pNetConfig->pNodeConfig && m_nCurrentNodeIndex >= 0);
	if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr)
	{
		return false;
	}
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (i == m_nCurrentNodeIndex)
		{
			continue;
		}

		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_FES)
		{
			// todo:��ʱֻ����A�������
			ACE_UINT64 addr = ACE_OS::inet_addr(m_pNetConfig->pNodeConfig[i].chIP[0]);

			m_vecFesNodeIP.push_back(addr);
		}
		else if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_CLIENT)
		{
			// todo:��ʱֻ����A�������
			ACE_UINT64 addr = ACE_OS::inet_addr(m_pNetConfig->pNodeConfig[i].chIP[0]);

			m_vecWorkStationNodeIP.push_back(addr);
		}
		else
		{
			continue;
		}
	}

	return true;
}*/

bool CServerNodeSvc::RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout/* = 0*/)
{
#if 1
	Q_ASSERT(pMsg && nMsgLen > 0);
	if (pMsg == nullptr || nMsgLen <= 0)
	{
		return false;
	}

	size_t len = 0;
	ACE_Time_Value timeout(ulTimeout);// = ACE_OS::gettimeofday();

	if (!g_msgQueue.GetUrgentQueue().is_empty())
	{
		len = g_msgQueue.DequeuMsgFromUrgentMQ((void*)pMsg, nMsgLen, &timeout);

	}
	else if (!g_msgQueue.GetNormalQueue().is_empty())
	{
		len = g_msgQueue.DequeuMsgFromNormalMQ((void*)pMsg, nMsgLen, &timeout);
	}

	if (len == 0)
	{
		return false;
	}

	nCopySize = static_cast<int32u>(len);
#endif

	return true;
}





/** @}*/
