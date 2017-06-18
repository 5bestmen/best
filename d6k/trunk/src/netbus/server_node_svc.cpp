/*! @file server_node_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  server_node_svc.cpp
文件实现功能 :  前置节点的网络服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   前置节点的网络服务
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
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
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

	// AB网都建立socket连接
	ACE_UNUSED_ARG(pszDataPath);
	ACE_UNUSED_ARG(nMode);

	// 是否存在 对侧 节点
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (i == m_nCurrentNodeIndex)
		{
			continue;
		}

		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_SVR) // SCADA节点
		{
			// log 存在对侧节点
			m_nAnotherNodeIndex = i;

			if (m_nCurrentNodeIndex < m_nAnotherNodeIndex)
			{
				// todo 思考，刚启动判断节点表是不是已经有主在运行.
				m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex].HostSlave = 1; // 主
				m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].HostSlave = 0; // 从
			}

			// 存在对侧
			m_bIsRedNode = true;

			break;
		}
	}

	// 对侧判断
	if (m_bIsRedNode)
	{
		if (m_pNetConfig->IsDoubleNet)
		{ // AB网

			std::vector<ACE_INET_Addr> vecAddr;
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);
			ACE_INET_Addr addrB(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetBIPAddr);
			vecAddr.push_back(addrA);
			vecAddr.push_back(addrB);

			m_pConnectPeer = std::make_shared<CConnectorMgr>(true, vecAddr, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A网
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);

			m_pConnectPeer = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}

		// 存在对侧时，建立接收任务，等待对侧连接
		// 主服务器指定IP进行监听,fes和client采用INADDR_ANY 进行监听
		/*ACE_INET_Addr addr;
		addr.set((u_short)SERVER_PORT, (ACE_UINT32)INADDR_ANY);

		m_pMySvr = std::make_shared<CAcceptorMgr>(addr);*/
	}

	// 其他节点的远程数据服务
	ACE_INET_Addr addr;
	addr.set((u_short)SERVER_PORT, (ACE_UINT32)INADDR_ANY);

	m_pRemoteSvr = std::make_shared<CAcceptorMgr>(addr);

	// 节点分类
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
		// log: 远程数据服务启动失败
		return;
	}

	// 冗余节点判断
	if (!m_bIsRedNode)
	{
		return;
	}

	// 连接对侧
	ACE_ASSERT(m_pConnectPeer);
	if (!m_pConnectPeer->Initialize())
	{
		// log: 连接客户端失败
		return;
	}

	// 等待对侧连接
	/*ACE_ASSERT(m_pMySvr);
	if (!m_pMySvr->Initialize())
	{
		// log: 等待对侧连接初始化失败
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
		// log 当前没有连接到主服务的节点
		return false;
	}

	/*CONNECTION_MAP_ITERATOR iter(M_SINGLETON::instance()->m_connMap);

	for (CONNECTION_MAP_ENTRY * i = 0; iter.next(i) != 0; iter.advance())
	{
		CNodeManager* pNode = i->int_id_;
	}*/

	// 注意需要区分下目的地，不同的目的地，他的队列不一样
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
		// 主服务发送数据到前置
		case TO_FES:
		{
			if (m_vecFesNodeIP.size() == 0)
			{
				// log:前置节点暂时未连接
				// todo:后续确认这个数据是否保存
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

				// 有前置节点连接
				// 发送数据
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
		// 主服务发送数据到工作站
		case TO_CLIENT:
		{
			if (m_vecWorkStationNodeIP.size() == 0)
			{
				// log:工作站节点暂时未连接
				// todo:后续确认这个数据是否保存
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

				// 有工作站节点连接
				// 发送数据
				if(0 == AsynchSend(pNodeManager->Get_Asynch_Write_Stream(), pMsg, nLen))
				{
					nRet = true;
				}
			}

			break;
		}
		// server发送消息给server，目前只是准备切换命令
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

			// 有对端服务来连接
			// 发送数据
			if (0 == AsynchSend(pNodeManager->Get_Asynch_Write_Stream(), pMsg, nLen))
			{
				nRet = true;
			}

			break;
		}
		default:
		{
			// log:消息路线有误
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

	//向操作系统发送数据
	if (asynchWrite.write(*mb, nLen) == -1)
	{
		return -1;
	}
	return 0;
}

// 节点分类(从网络节点表中将前置机和工作站分类)
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
			// todo:暂时只考虑A网的情况
			ACE_UINT64 addr = ACE_OS::inet_addr(m_pNetConfig->pNodeConfig[i].chIP[0]);

			m_vecFesNodeIP.push_back(addr);
		}
		else if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_CLIENT)
		{
			// todo:暂时只考虑A网的情况
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
