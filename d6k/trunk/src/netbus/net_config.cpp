/*! @file net_config.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  net_config.cpp
文件实现功能 :  系统网络配置及状态
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   系统网络配置及状态
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.26
*******************************************************************************/
#include "net_config.h"
#include "log/log.h" 
#include "nb_svc.h"

#include <fesapi/fescnst.h>

#include <QObject> 
#include <QString> 
  
CNodeInfo::CNodeInfo()
{

}

CNodeInfo::CNodeInfo(NODE_CONFIG *pNode)
{
	Q_ASSERT(pNode);
	if (pNode == nullptr)
		return;

	this->OccNo = pNode->OccNo;
	Q_ASSERT(this->OccNo);
	this->SlaveOccNo = pNode->SlaveOccNo;

	this->NodeType = pNode->NodeType;
	this->NetAIPAddr = pNode->NetAIPAddr;
	this->NetBIPAddr = pNode->NetBIPAddr;

	std::memcpy(this->NetAIpV6Addr, pNode->NetAIpV6Addr, sizeof(pNode->NetAIpV6Addr));
	std::memcpy(this->NetBIpV6Addr, pNode->NetBIpV6Addr, sizeof(pNode->NetBIpV6Addr));
	this->IsCheckTime = pNode->IsCheckTime;
	this->IsDefaultHost = pNode->IsDefaultHost;
	std::memcpy(this->HostName, pNode->HostName, sizeof(pNode->HostName));
	std::memcpy(this->TagName, pNode->TagName, sizeof(pNode->TagName));

	this->HostSlave = pNode->HostSlave;  
	std::memcpy(this->CardStatus, pNode->CardStatus, sizeof(pNode->CardStatus));

}

CNodeInfo::~CNodeInfo()
{

}


CNetState::CNetState( ) 
{
	 

}

CNetState::~CNetState( )
{

}

CNetState::CNetState(NET_CONFIG *pConfig)
{
	Q_ASSERT(pConfig);
	if (pConfig == nullptr)
		return;

	Q_ASSERT(pConfig->pNodeConfig && pConfig->NodeCount > 0);
	if (pConfig->pNodeConfig == nullptr || pConfig->NodeCount == 0)
		return;

	int32u i = 0;

	for (i = 0; i < pConfig->NodeCount;i++)
	{
		std::shared_ptr<CNodeInfo> pNode = std::make_shared<CNodeInfo>(& pConfig->pNodeConfig[i]);

		auto ret = m_mapNetConf.insert(std::make_pair(pConfig->pNodeConfig[i].OccNo, pNode));
		Q_ASSERT(ret.second);
	}

}

/** @}*/
