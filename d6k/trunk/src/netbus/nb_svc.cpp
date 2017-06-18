/*! @file nb_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  nb_svc.cpp
文件实现功能 :  网络总线
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   网络总线
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "nb_svc.h"

#include "ace/INET_Addr.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"
#include "ace/Init_ACE.h"

#include <QObject> 
#include <QString> 
#include <QHostAddress>

#include "netbus/nbdef.h"
#include "node_svc.h"
#include "fesnode_svc.h"
#include "server_node_svc.h"
#include "client_node_svc.h"

#include "fesapi/fescnst.h"
#include "fesapi/fesdatadef.h"
#include "log/log.h" 


CNetbusSvc::CNetbusSvc(const char * pszPredicateName)  :m_pNetConfig(nullptr)
{
	Q_ASSERT(pszPredicateName);
	Q_ASSERT(strlen(pszPredicateName) > 0);

	m_pNodeSvc = nullptr;

	if (pszPredicateName == nullptr || strlen(pszPredicateName) == 0)
	{
		return;
	}

	m_szPredicateName = pszPredicateName;

	m_bShutdown = false;

	m_isExistMainSlave = false;

	m_nMyNodeIdx = -1;
	m_nAnotherIndex = -1;
	m_pCheckNetNodeState = nullptr;
	m_pCheckHostSlaveState = nullptr;

	// 发送心跳报文定时器
	m_pUdbSender = nullptr;
	m_pHeartbeatTimer = new QTimer();
	m_pHeartbeatTimer->setInterval(HEARTBEAT_TIMER);
	connect(m_pHeartbeatTimer, SIGNAL(timeout()), this, SLOT(Slot_SendHeartbeatMsg()));

	m_pCheckAnotherNodeState = nullptr;	

	m_pCheckAllNodeState = new QTimer();
	m_pCheckAllNodeState->setInterval(HEARTBEAT_TIMER);
	connect(m_pCheckAllNodeState, SIGNAL(timeout()), this, SLOT(Slot_CheckAllNodeState()));
}

CNetbusSvc::CNetbusSvc() :m_pNetConfig(nullptr)
{
	m_pNodeSvc = nullptr;

	m_bShutdown = false;
	m_isExistMainSlave = false;

	m_nMyNodeIdx = -1;
	m_nAnotherIndex = -1;
	m_pCheckNetNodeState = nullptr;
	m_pCheckHostSlaveState = nullptr;
	m_pCheckAnotherNodeState = nullptr;
	m_pCheckAllNodeState = nullptr;
}

CNetbusSvc::~CNetbusSvc( )
{
	//ACE_Guard<ACE_Thread_Mutex> guard(m_mutex);
	if (m_bShutdown)
	{
		return;
	}

	m_bShutdown = true;

	if (m_pCheckNetNodeState != nullptr)
	{
		delete m_pCheckNetNodeState;
		m_pCheckNetNodeState = nullptr;
	}

	if (m_pCheckHostSlaveState != nullptr)
	{
		delete m_pCheckHostSlaveState;
		m_pCheckHostSlaveState = nullptr;
	}

	if (m_pHeartbeatTimer != nullptr)
	{
		m_pHeartbeatTimer->stop();

		delete m_pHeartbeatTimer;
		m_pHeartbeatTimer = nullptr;
	}

	if (m_pCheckAnotherNodeState != nullptr)
	{
		m_pCheckAnotherNodeState->stop();

		delete m_pCheckAnotherNodeState;
		m_pCheckAnotherNodeState = nullptr;
	}

	if (m_pCheckAllNodeState != nullptr)
	{
		m_pCheckAllNodeState->stop();

		delete m_pCheckAllNodeState;
		m_pCheckAllNodeState = nullptr;
	}

	if (m_pUdbSender != nullptr)
	{
		delete m_pUdbSender;
		m_pUdbSender = nullptr;
	}

	if (m_pNodeSvc != nullptr)
	{
		delete m_pNodeSvc;
		m_pNodeSvc = nullptr;
	}
}

void CNetbusSvc::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("FES", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CNetbusSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CNetbusSvc::Initialize(NET_CONFIG * pConf,const char *pszDataPath, int32u nNodeType, unsigned int nMode)
{
	//
	ACE_Time_Value vTmp = ACE_OS::gettimeofday();
	ACE_UINT64 msec = vTmp.get_msec();
	// 1484033638461
	//
	if (pszDataPath && strlen(pszDataPath)>0)
	{
		m_szDataPathName = pszDataPath;
	}

	Q_ASSERT(pConf->NodeCount > 0 && pConf->NodeCount <= MAX_NODE_COUNT);
	if (pConf->NodeCount == 0 || pConf->NodeCount > MAX_NODE_COUNT)
	{
		// log
		return false;
	}

	Q_ASSERT(pConf->pNodeConfig);
	if (pConf->pNodeConfig == nullptr)
		return false;

	m_pNetConfig = pConf;

	struct NODE_CONFIG * pMyNode = GetMyNodeConfig(m_pNetConfig);
	Q_ASSERT(pMyNode);
	if (pMyNode == nullptr)
	{
		return false;
	}

	Q_ASSERT(pMyNode->NodeType == nNodeType);
	if (pMyNode->NodeType != nNodeType)
	{
		return false;
	}

	bool bRet = false;
	bRet = GetAllLocalIpV4Addr(m_setMyIpAddr);
	Q_ASSERT(bRet && m_setMyIpAddr.size());

	// 找到本机所在节点
	int nIdx = -1;
//	bRet = GetMyNodeIndex(*pConf, nNodeType,nIdx);
	bRet = CheckMyNodeIpAddr(pMyNode);
	if (bRet)
	{
		bRet = GetMyNodeIndex(pConf, nNodeType, nIdx);
		Q_ASSERT(bRet);
		if (bRet)
		{
			Q_ASSERT(nIdx >= 0);
			m_nMyNodeIdx = nIdx;
			bRet = StartNodeSvc(pConf, nIdx);
		}	
		// 还要找到从节点		
	}
	else
	{
		// Log 节点与表不一致
		return false;
	}

	return bRet;
}


void CNetbusSvc::Run()
{
	Q_ASSERT(m_pNodeSvc);

	m_pNodeSvc->Run();
}

void CNetbusSvc::Shutdown()
{
	Q_ASSERT(m_pNodeSvc);
	if (m_pNodeSvc)
	{
		m_pNodeSvc->Shutdown();
	}
}

/*! \fn struct NODE_CONFIG *CNetbusSvc::GetMyNodeConfig(struct NET_CONFIG *pConf)const
*********************************************************************************************************
** \brief CNetbusSvc::GetMyNodeConfig
** \details 获取本机的配置情况
** \param pConf
** \return struct NODE_CONFIG *
** \author LiJin
** \date 2016年11月28日
** \note
********************************************************************************************************/
struct NODE_CONFIG *CNetbusSvc::GetMyNodeConfig(struct NET_CONFIG *pConf)const
{
	Q_ASSERT(pConf);
	if (pConf == nullptr)
		return nullptr;

	Q_ASSERT(pConf->MyNodeOccNo > INVALID_OCCNO && pConf->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (pConf->MyNodeOccNo == INVALID_OCCNO || pConf->MyNodeOccNo > MAX_NODE_OCCNO)
		return nullptr;

	int32u i = 0;
	for (i = 0; i < pConf->NodeCount; i++)
	{
		if (pConf->pNodeConfig[i].OccNo == pConf->MyNodeOccNo)
		{
			return &pConf->pNodeConfig[i];
		}
	}
	return nullptr;
}
/*! \fn struct NODE_CONFIG *CNetbusSvc::GetSlaveNodeConfig(struct NET_CONFIG *pConf) const
*********************************************************************************************************
** \brief CNetbusSvc::GetSlaveNodeConfig
** \details 获取备机的配置情况
** \param pConf
** \return struct NODE_CONFIG *
** \author LiJin
** \date 2016年11月28日
** \note
********************************************************************************************************/
struct NODE_CONFIG *CNetbusSvc::GetSlaveNodeConfig(struct NET_CONFIG *pConf) const
{
	Q_ASSERT(pConf);
	if (pConf == nullptr)
		return nullptr;

	Q_ASSERT(pConf->MyNodeOccNo > INVALID_OCCNO && pConf->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (pConf->MyNodeOccNo == INVALID_OCCNO || pConf->MyNodeOccNo > MAX_NODE_OCCNO)
		return nullptr;

	int32u i = 0;
 	NODE_CONFIG *pMyNode = nullptr;
	for (i = 0; i < pConf->NodeCount; i++)
	{
		// 确保不是同一节点
		if (pConf->pNodeConfig[i].OccNo == pConf->MyNodeOccNo)
		{
			Q_ASSERT(pConf->pNodeConfig[i].OccNo >INVALID_OCCNO);
			pMyNode = &pConf->pNodeConfig[i];
			break;
		}
	}

	Q_ASSERT(pMyNode);
	if (pMyNode == nullptr)
		return nullptr;

	for (i = 0; i < pConf->NodeCount; i++)
	{
		// 确保不是同一节点
		if (pConf->pNodeConfig[i].SlaveOccNo == pConf->MyNodeOccNo && pConf->pNodeConfig[i].SlaveOccNo != pConf->pNodeConfig[i].OccNo)
		{
			if (  pMyNode !=  &(pConf->pNodeConfig[i]) && pMyNode->NodeType == pConf->pNodeConfig[i].NodeType)
			{
				Q_ASSERT(pConf->pNodeConfig[i].OccNo >INVALID_OCCNO);
				return &pConf->pNodeConfig[i];
			}
		}
	}

	return nullptr;
}
/*! \fn bool CNodeSvc::GetAllLocalIpV4Addr(std::set<unsigned long> & arrIpAddr) const
*********************************************************************************************************
** \brief CNodeSvc::GetAllLocalIpV4Addr
** \details 获取本机所有IP地址
** \param arrIpAddr
** \return bool
** \author LiJin
** \date 2016年11月5日
** \note
********************************************************************************************************/
bool CNetbusSvc::GetAllLocalIpV4Addr(std::set<std::string> & arrIpAddr) const
{
	arrIpAddr.clear();

	ACE_INET_Addr* arrAddr = nullptr;
	size_t count = 0;

	if (ACE::get_ip_interfaces(count, arrAddr) != 0)
	{
		return false;
	}

	if (count <= 0)
	{
		return false;
	}

	for (size_t i = 0; i < count; i++)
	{
		char addr[256] = { 0 };

		arrAddr[i].get_host_addr(addr, sizeof(addr));

		unsigned long ulAddr = QHostAddress(QString(addr)).toIPv4Address();// ACE_OS::inet_addr(addr);
		ACE_ASSERT(ulAddr != 0);
		ACE_DEBUG((LM_INFO, ACE_TEXT("%D:%N:%l:[INFO   ]:ADDR：%s.\n"), addr));
		//		m_setIP.insert(lAddr);
		auto ret = arrIpAddr.insert(addr);
		ACE_ASSERT(ret.second == true);
	}
	return true;
}
/*! \fn bool CNetbusSvc::IsMyIpV4Addr(unsigned long ulAddr)const
********************************************************************************************************* 
** \brief CNetbusSvc::IsMyIpV4Addr 
** \details 判断指定IP是否属于本机IP
** \param ulAddr 
** \return bool true 是本机的IP，false 不是本机IP
** \author LiJin 
** \date 2016年11月7日 
** \note 
********************************************************************************************************/
bool CNetbusSvc::IsMyIpV4Addr(const char* addr)const
{
	std::set<std::string>::const_iterator it_find = m_setMyIpAddr.find(addr);

	if (it_find == m_setMyIpAddr.cend())
	{// 没有找到
		return false;
	}
	return true;
}
/*! \fn bool CNetbusSvc::CheckMyNodeIpAddr(struct NODE_CONFIG *pMyNode)const
********************************************************************************************************* 
** \brief CNetbusSvc::CheckMyNodeIpAddr 
** \details 检查配置的ip地址，在本节点上是否有配置
** \param pMyNode 
** \return bool 只要有一个ip是属于本机的即返回 true
** \author LiJin 
** \date 2016年11月28日 
** \note 
********************************************************************************************************/
bool CNetbusSvc::CheckMyNodeIpAddr(struct NODE_CONFIG *pMyNode)const
{
	Q_ASSERT(pMyNode);
	if (pMyNode == nullptr)
		return false;

	if (IsMyIpV4Addr(pMyNode->chIP[0]) || IsMyIpV4Addr(pMyNode->chIP[1]))
	{

		return true;
	}

	return false;
}

bool CNetbusSvc::GetMyNodeIndex(const NET_CONFIG & conf, int32u nNodeType,int & nIdx) const
{
	bool bRet = false;
	int32u i = 0;
	for (i = 0; i < conf.NodeCount; i++)
	{
		unsigned long ulAddr1 = conf.pNodeConfig[i].NetAIPAddr;
		unsigned long ulAddr2 = conf.pNodeConfig[i].NetBIPAddr;
		if (conf.pNodeConfig[i].NodeType == nNodeType 
			&& (IsMyIpV4Addr(conf.pNodeConfig[i].chIP[0]) || IsMyIpV4Addr(conf.pNodeConfig[i].chIP[0]))
			)
		{// 找到
			nIdx = i;
			bRet = true;			
			break;
		}
	}

	return bRet;
}

bool  CNetbusSvc::GetMyNodeIndex(struct NET_CONFIG *pConf, int32u nNodeType, int & nIdx) const
{
	bool bRet = false;
	int32u i = 0;
	for (i = 0; i < pConf->NodeCount; i++)
	{	 
		if (pConf->pNodeConfig[i].NodeType == nNodeType
			&& pConf->MyNodeOccNo == pConf->pNodeConfig[i].OccNo )			
		{// 找到
			nIdx = i;
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool  CNetbusSvc::StartNodeSvc(NET_CONFIG *pNetConf, int32u nMyNodeIdx)
{
	Q_ASSERT(pNetConf);
	
	Q_ASSERT(pNetConf->pNodeConfig);

	Q_ASSERT(nMyNodeIdx >= 0 && nMyNodeIdx < pNetConf->NodeCount);

	if (nMyNodeIdx < 0 || nMyNodeIdx >= pNetConf->NodeCount)
	{
		return false;
	}

	if (pNetConf->pNodeConfig[nMyNodeIdx].NodeType == NODE_FES)
	{// 本机是前置
		m_pNodeSvc = new CFesNodeSvc(pNetConf, nMyNodeIdx);

		m_pNodeSvc->Initialize("", NODE_FES, 0);
	}
	else if (pNetConf->pNodeConfig[nMyNodeIdx].NodeType == NODE_SVR)
	{
		m_pNodeSvc = new CServerNodeSvc(pNetConf, nMyNodeIdx);

		m_pNodeSvc->Initialize("", NODE_SVR, 0);
	}
	else if (pNetConf->pNodeConfig[nMyNodeIdx].NodeType == NODE_CLIENT)
	{
		m_pNodeSvc = new CClientNodeSvc(pNetConf, nMyNodeIdx);

		m_pNodeSvc->Initialize("", NODE_CLIENT, 0);
	}
	else
	{
		Q_ASSERT(false);
		return false;
	}

#ifdef NB_HEARTBEAT	

	m_nAnotherIndex = m_pNodeSvc->GetAnotherNodeIndex();

	if (!StartHeartbeat(pNetConf, nMyNodeIdx, m_nAnotherIndex))
	{
		return false;
	}

#endif

	m_pNodeSvc->Run();

	return true;
}

// 启动心跳
bool CNetbusSvc::StartHeartbeat(NET_CONFIG *pNetConf, const int32u nMyNodeIdx, const int32s nAnotherIndex)
{
	Q_ASSERT(pNetConf);
	Q_ASSERT(pNetConf->pNodeConfig);
	Q_ASSERT(nMyNodeIdx >= 0 && nMyNodeIdx < pNetConf->NodeCount);
	if (nMyNodeIdx < 0 || nMyNodeIdx >= pNetConf->NodeCount)
	{
		return false;
	}

	// 判断是否存在主备
	if (m_nAnotherIndex >= 0 && m_nAnotherIndex < m_pNetConfig->NodeCount)
	{ // 存在主备
		m_isExistMainSlave = true;
	}

	if (m_isExistMainSlave)
	{
		// 时间初始化
		pNetConf->pNodeConfig[m_nAnotherIndex].SwitchTime = ACE_OS::gettimeofday().get_msec();
	}

	// 创建心跳发送线程（A网）
	// 是否是对时节点
	bool bCheckTimeNode = false;
	if (pNetConf->bCheckTime && pNetConf->pNodeConfig[nMyNodeIdx].OccNo == pNetConf->CheckTimeNodeOccNo)
	{
		bCheckTimeNode = true;
	}
	const char* ip = pNetConf->pNodeConfig[nMyNodeIdx].chIP[0];
	ACE_INET_Addr addrHeartbeatSend_A(DGRAM_PORT_A, ip);
	QString qStrIP(ip);
	std::string strIP = qStrIP.toStdString();
	size_t index = strIP.find_last_of('.');
	char chUdpIp[64] = {0};
	strncpy(chUdpIp, strIP.c_str(), index);
	QString strUdpIP;
	strUdpIP.append(chUdpIp);
	strUdpIP.append(".255");
	const unsigned short port = DGRAM_PORT_A;

	// 心跳接收
	m_pUdbRecver = new CUdpRecver(port, pNetConf, m_nMyNodeIdx, m_nAnotherIndex, NET_A);
	m_pUdbRecver->InitSocket();

	// 心跳发送
	m_pUdbSender = new CUdpSender(strUdpIP, port, m_pNetConfig, nMyNodeIdx, m_nAnotherIndex, bCheckTimeNode);
	m_pHeartbeatTimer->start();

	// 创建心跳接收线程（A网）
	/*ACE_INET_Addr addrHeartbeatRecv_A(DGRAM_PORT_A);
	m_pHeartbeatRecv_A = new CHeartbeatReceiver(addrHeartbeatRecv_A, pNetConf, nMyNodeIdx, nAnotherIndex, NET_A);
	m_createRecviverThread_A = new CThreadCreate(m_pHeartbeatRecv_A);
	if (0 != m_createRecviverThread_A->ThreadCreate())
	{
		return false;
	}

	// 创建心跳发送线程（A网）
	// 是否是对时节点
	bool bCheckTimeNode = false;
	if (pNetConf->bCheckTime && pNetConf->pNodeConfig[nMyNodeIdx].OccNo == pNetConf->CheckTimeNodeOccNo)
	{
		bCheckTimeNode = true;
	}
	const char* ip = pNetConf->pNodeConfig[nMyNodeIdx].chIP[0];
	ACE_INET_Addr addrHeartbeatSend_A(DGRAM_PORT_A, ip);
	m_pHeartbeatSender_A = new CHeartbeatSender(addrHeartbeatSend_A, &pNetConf->pNodeConfig[nMyNodeIdx], bCheckTimeNode);
	m_createSenderThread_A = new CThreadCreate(m_pHeartbeatSender_A);
	if (0 != m_createSenderThread_A->ThreadCreate())
	{
		return false;
	}

	// 初始化A网时间
	pNetConf->pNodeConfig[nMyNodeIdx].TimeNetA = ACE_OS::gettimeofday().get_msec();

	// 是否双网判断
	if (pNetConf->IsDoubleNet)
	{
		// 初始化A网时间
		pNetConf->pNodeConfig[nMyNodeIdx].TimeNetB = ACE_OS::gettimeofday().get_msec();

		// 创建心跳接收线程（B网）
		ACE_INET_Addr addrHeartbeatRecv_B(DGRAM_PORT_B);
		m_pHeartbeatRecv_B = new CHeartbeatReceiver(addrHeartbeatRecv_B, pNetConf, nMyNodeIdx, nAnotherIndex, NET_B);
		m_createRecviverThread_B = new CThreadCreate(m_pHeartbeatRecv_B);
		if (0 != m_createRecviverThread_B->ThreadCreate())
		{
			return false;
		}

		// 创建心跳发送线程（B网）
		const char* ip = pNetConf->pNodeConfig[nMyNodeIdx].chIP[1];
		ACE_INET_Addr addrHeartbeatSend_B(DGRAM_PORT_B, ip);
		m_pHeartbeatSender_B = new CHeartbeatSender(addrHeartbeatSend_B, &pNetConf->pNodeConfig[nMyNodeIdx], bCheckTimeNode);
		m_createSenderThread_B = new CThreadCreate(m_pHeartbeatSender_B);
		if (0 != m_createSenderThread_B->ThreadCreate())
		{
			return false;
		}
	}*/

	// 如果存在准备则启动定时器检测对侧节点状态和主备信息
	if (m_isExistMainSlave)
	{ // 存在主备
		m_pCheckAnotherNodeState = new QTimer();
		m_pCheckAnotherNodeState->setInterval(HEARTBEAT_TIMER);
		connect(m_pCheckAnotherNodeState, SIGNAL(timeout()), this, SLOT(Slot_CheckAnotherNodeState()));

		m_pCheckAnotherNodeState->start();
	}

	// 检查整个网络节点状态，超过指定时间收不到心跳报文则表示节点下线
	m_pCheckAllNodeState->start();
	
	return true;
}

void CNetbusSvc::Slot_SendHeartbeatMsg()
{
	Q_ASSERT(m_pUdbSender != NULL);
	if (m_pUdbSender == NULL)
	{
		return;
	}

	static std::atomic<bool> bCheckAnotherNode = false;

	if (!bCheckAnotherNode)
	{
		if (m_nAnotherIndex >= 0 && m_nAnotherIndex < m_pNetConfig->NodeCount)
		{ // 存在主备
		  // 如果存在主备,则检测对侧节点类型
			ACE_OS::sleep(START_WAIT_TIME);

			if (!CheckNodeTypeOnTheSide(m_pNetConfig, m_nMyNodeIdx, m_nAnotherIndex))
			{
				return;
			}
		}

		bCheckAnotherNode = true;
	}	

	m_pUdbSender->InitSocket();

	m_pUdbSender->Send();
}

void CNetbusSvc::Slot_CheckAnotherNodeState()
{
	Q_ASSERT(m_pNetConfig != nullptr && m_pNetConfig->pNodeConfig != nullptr);
	Q_ASSERT(m_nAnotherIndex >= 0 && m_nAnotherIndex < m_pNetConfig->NodeCount);
	Q_ASSERT(m_nMyNodeIdx >= 0 && m_nMyNodeIdx < m_pNetConfig->NodeCount);
	Q_ASSERT(m_nAnotherIndex != m_nMyNodeIdx);

	if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr ||
		m_nAnotherIndex < 0 || m_nAnotherIndex >= m_pNetConfig->NodeCount || m_nAnotherIndex == m_nMyNodeIdx)
	{
		return;
	}

	ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
	ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[m_nAnotherIndex].SwitchTime;
	ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
	// 5秒收到对侧心跳则进行主备切换
	if (subTime > HEARTBEAT_TIMEOUT)
	{
		if (m_pNetConfig->pNodeConfig[m_nMyNodeIdx].HostSlave != MAIN)
		{
			m_pNetConfig->pNodeConfig[m_nMyNodeIdx].HostSlave = MAIN;
		}

		if (m_pNetConfig->pNodeConfig[m_nAnotherIndex].HostSlave != SLAVE)
		{
			m_pNetConfig->pNodeConfig[m_nAnotherIndex].HostSlave = SLAVE;
		}
	}
}

void CNetbusSvc::Slot_CheckAllNodeState()
{
	Q_ASSERT(m_pNetConfig != nullptr && m_pNetConfig->pNodeConfig != nullptr);
	if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr)
	{
		return;
	}

	int32u nCnt = m_pNetConfig->NodeCount;
	for (int32u i = 0; i < nCnt; i++)
	{
		ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
		ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[i].TimeNetA;
		ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
		// 5秒收不到对侧心跳则认为该节点下线
		if (subTime > NODE_ALIVE_TIME)
		{
			m_pNetConfig->pNodeConfig[i].CardStatus[0] = 0;
		}
		else
		{
			m_pNetConfig->pNodeConfig[i].CardStatus[0] = 1;
		}

		// 双网
		if (m_pNetConfig->IsDoubleNet)
		{
			ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
			ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[i].TimeNetB;
			ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
			// 5秒收不到对侧心跳则认为该节点下线
			if (subTime > NODE_ALIVE_TIME)
			{
				m_pNetConfig->pNodeConfig[i].CardStatus[1] = 0;
			}
			else
			{
				m_pNetConfig->pNodeConfig[i].CardStatus[1] = 1;
			}
		}
	}
}

bool CNetbusSvc::CheckNodeTypeOnTheSide(NET_CONFIG *&pNetConf, const int32u nMyNodeIdx, const int32u nAnotherIndex)
{
	Q_ASSERT(pNetConf);
	Q_ASSERT(pNetConf->pNodeConfig);
	Q_ASSERT(nMyNodeIdx >= 0 && nAnotherIndex >= 0 && nMyNodeIdx < pNetConf->NodeCount &&
		nAnotherIndex < pNetConf->NodeCount && nAnotherIndex < pNetConf->NodeCount);

	if (pNetConf == nullptr || pNetConf->pNodeConfig == nullptr ||
		nMyNodeIdx < 0 || nAnotherIndex < 0 || nMyNodeIdx >= pNetConf->NodeCount ||
		nAnotherIndex >= pNetConf->NodeCount || nAnotherIndex >= pNetConf->NodeCount)
	{
		return false;
	}

	if (pNetConf->pNodeConfig[nAnotherIndex].HostSlave == MAIN)
	{
		pNetConf->pNodeConfig[nMyNodeIdx].HostSlave = SLAVE;
	}
	else
	{
		pNetConf->pNodeConfig[nMyNodeIdx].HostSlave = MAIN;
	}

	return true;
}

/*! \fn int CNetbusSvc::SendData(const int8u * pMsg, int32u nLen)
********************************************************************************************************* 
** \brief CNetbusSvc::SendData 
** \details 发送数据，只是发到内部的缓存队列
** \param pMsg 
** \param nLen 
** \return int 
** \author LiJin 
** \date 2016年12月14日 
** \note 
********************************************************************************************************/
int CNetbusSvc::SendData(const int8u * pMsg, int32u nLen)
{
	Q_ASSERT(m_pNodeSvc && nLen > EMSG_BUF_HEAD_SIZE);
	if (m_pNodeSvc == nullptr || nLen <= EMSG_BUF_HEAD_SIZE)
	{
		return 0;
	}	

	return m_pNodeSvc->SendData(pMsg, nLen);
}

bool  CNetbusSvc::RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout)
{
	Q_ASSERT(m_pNodeSvc && nMsgLen > EMSG_BUF_HEAD_SIZE);
	if (m_pNodeSvc == nullptr || nMsgLen <= EMSG_BUF_HEAD_SIZE)
	{
		return 0;
	}

	return m_pNodeSvc->RecvData(pMsg, nMsgLen, nCopySize, ulTimeout);
}

bool CNetbusSvc::ManualSwitchNode()
{
	Q_ASSERT(m_pNodeSvc);
	if (m_pNodeSvc == nullptr)
	{
		return false;
	}

	m_nAnotherIndex = m_pNodeSvc->GetAnotherNodeIndex();

	// 判断是否存在主备
	if (m_nAnotherIndex < 0 || m_nAnotherIndex >= m_pNetConfig->NodeCount)
	{
		// 不存在主备
		return false;
	}

	EMSG_BUF msgBuf;

	int32u nNodeType = m_pNetConfig->pNodeConfig[m_nMyNodeIdx].NodeType;
	switch (nNodeType)
	{
	case NODE_SVR:
		msgBuf.MsgPath = EMSG_PATH::TO_SERVER;
		break;
	case NODE_FES:
		msgBuf.MsgPath = EMSG_PATH::TO_FES;
		break;
	default:
		return false;
	}

	int32u destOccNo = m_pNetConfig->pNodeConfig[m_nAnotherIndex].OccNo;
	msgBuf.DestOccNo = destOccNo;
	// 发送切换报文
	msgBuf.MsgType = SEND_SWITCH_CMD_REQ;
	const char* data = "switch req";
	int nLen = strlen(data);
	msgBuf.MsgDataSize = EMSG_BUF_HEAD_SIZE + nLen;
	memset(msgBuf.BuffData, 0, nLen + 1);

	memcpy(msgBuf.BuffData, &data, nLen);

	// 发送切换命令到对侧节点
	m_pNodeSvc->SendData(reinterpret_cast<int8u*>(&msgBuf), msgBuf.MsgDataSize);

	return true;
}

/** @}*/
