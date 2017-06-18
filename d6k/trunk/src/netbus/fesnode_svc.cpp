/*! @file fesnode_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  fesnode_svc.cpp
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
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
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

	// 优先查找服务节点
	for (int32u i = 0; i < m_pNetConfig->NodeCount; ++i)
	{
		if (m_pNetConfig->pNodeConfig[i].NodeType == NODE_SVR) // 服务节点
		{
			bExistSvrNode = true;

			// log 打印服务节点信息

			if (m_nSvrIndexA == -1)
			{
				m_nSvrIndexA = i;
			}
			else
			{
				m_nSvrIndexB = i;

				// 存在双主服务器
				m_bIsDoubleSvr = true;

				break;
			}
		}
	}

	if (!bExistSvrNode)
	{
		// log 配置有误，不存在服务节点
		return false;
	}


	// 是否存在 对侧 节点
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
				// 存在对侧
				m_bIsRedNode = true;

				m_nAnotherNodeIndex = i;

				break;
			}
		}
	}

	if (m_bIsDoubleSvr)
	{ // 双主服务器
		if (m_pNetConfig->IsDoubleNet)
		{ // AB网
			// 服务器A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);
			ACE_INET_Addr addrB_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[1]);
			vecAddressA.push_back(addrA_A);
			vecAddressA.push_back(addrB_A);
			
			m_pConnectSvrA = std::make_shared<CConnectorMgr>(true, vecAddressA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);

			// 服务器B
			std::vector<ACE_INET_Addr> vecAddressB;
			ACE_INET_Addr addrA_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[0]);
			ACE_INET_Addr addrB_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[1]);
			vecAddressB.push_back(addrA_B);
			vecAddressB.push_back(addrB_B);

			m_pConnectSvrB = std::make_shared<CConnectorMgr>(true, vecAddressB, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A网
			// 服务器A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(addrA_A, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);

			// 服务器B
			std::vector<ACE_INET_Addr> vecAddressB;
			ACE_INET_Addr addrA_B(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexB].chIP[0]);

			m_pConnectSvrB = std::make_shared<CConnectorMgr>(addrA_B, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}		
	}
	else
	{ // 单个主服务器

		if (m_pNetConfig->IsDoubleNet)
		{ // AB网
			// 服务器A
			std::vector<ACE_INET_Addr> vecAddressA;
			ACE_INET_Addr addrA_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);
			ACE_INET_Addr addrB_A(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[1]);
			vecAddressA.push_back(addrA_A);
			vecAddressA.push_back(addrB_A);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(true, vecAddressA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A网
			// 服务器A
			ACE_INET_Addr addrA(SERVER_PORT, m_pNetConfig->pNodeConfig[m_nSvrIndexA].chIP[0]);

			m_pConnectSvrA = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}		
	}

	// 对侧
	if (m_bIsRedNode)
	{
		if (m_pNetConfig->IsDoubleNet)
		{ // AB网

			std::vector<ACE_INET_Addr> vecAddr;
			ACE_INET_Addr addrA(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);
			ACE_INET_Addr addrB(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetBIPAddr);
			vecAddr.push_back(addrA);
			vecAddr.push_back(addrB);

			m_pConnectAnother = std::make_shared<CConnectorMgr>(true, vecAddr, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}
		else
		{ // A网
			ACE_INET_Addr addrA(FES_PORT, m_pNetConfig->pNodeConfig[m_nAnotherNodeIndex].NetAIPAddr);

			m_pConnectAnother = std::make_shared<CConnectorMgr>(addrA, &m_pNetConfig->pNodeConfig[m_nCurrentNodeIndex]);
		}

		// 存在对侧时，建立接收任务，等待对侧连接
		// 主服务器指定IP进行监听,fes和client采用INADDR_ANY 进行监听
		ACE_INET_Addr addr;
		addr.set(FES_PORT, (ACE_UINT32)INADDR_ANY);

		m_pMySvr = std::make_shared<CAcceptorMgr>(addr);
	}

	return true;
}

void CFesNodeSvc::Run()
{
	Q_ASSERT(m_pConnectSvrA);

	// 连接主服务器
	if (m_bIsDoubleSvr)
	{ // 双主服务器
		Q_ASSERT(m_pConnectSvrB);

		if (!m_pConnectSvrA->Initialize())
		{
			if (!m_pConnectSvrB->Initialize())
			{
				// log 初始化连接A B主服务器失败
				return;
			}
			else
			{
				m_nCurWorkingNet = NET_B;
			}
		}
		else
		{ // 主服务器A start成功
			m_nCurWorkingNet = NET_A;

			m_pConnectSvrB->Initialize();
		}
	}
	else
	{ // 单主服务器
		if (!m_pConnectSvrA->Initialize())
		{
			return;
		}

		m_nCurWorkingNet = NET_A;
	}	

	// 冗余节点判断
	if (!m_bIsRedNode)
	{
		return;
	}

	// 连接对侧
	Q_ASSERT(m_pConnectAnother);
	m_pConnectAnother->Initialize();

	// 等待对侧连接
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
** \details 从前置发送数据到主服务器
** \param pMsg 
** \param nLen 
** \return int 
** \author   
** \date 2017年1月12日 
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
	// todo:自动切换和手动切换思考。
	if (m_bIsDoubleSvr)
	{
		if (m_nCurWorkingNet > NET_B)
		{
			Q_ASSERT(false);
			m_nCurWorkingNet = NET_A;
		}
		// 主机
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
** \details 发送数据，只是写到内部缓冲队列
** \param pMsg 
** \param nLen 
** \return int 
** \author LiJin 
** \date 2016年11月28日 
** \note 
********************************************************************************************************/
bool CFesNodeSvc::SendData(const int8u * pMsg, int32u nLen)
{
	Q_ASSERT(pMsg && nLen > EMSG_BUF_HEAD_SIZE);
	if (pMsg == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}
	// 注意需要区分下目的地，不同的目的地，他的队列不一样
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
		// todo:消息类型作为级别(数值越大，消息级别越高，后续需要调整)
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
** \details 获取当前主从服务器列表（第一个返回值是 主服务器指针，第二个是从服务器）
** \return std::pair<std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>> 
** \author LiJin 
** \date 2017年1月12日 
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
** \param pMsg 输入缓冲区
** \param nMsgLen 需要的长度
** \param nCopySize 返回的参数---实际接收的长度
** \param ulTimeout 超时，单位毫秒
** \return bool 
** \author LiJin 
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
bool CFesNodeSvc::RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout)
{
	// 需要遍历所有的socket连接
	Q_ASSERT(pMsg && nMsgLen);
	if (pMsg == nullptr || nMsgLen < EMSG_BUF_HEAD_SIZE)
	{
		return false;
	}
	// 接收优先级 先接收主服务器的数据，从服务器的数据

	size_t nRet = 0;
	if (m_bIsDoubleSvr)
	{
		// 判断当前，主服务器是A还是B，
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
	{// 单主服务器，只能连接A
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
	// 接收 主从同步数据


 
	return false;
}

 
/** @}*/
