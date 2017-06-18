/*! @file net_config.h
<PRE>
********************************************************************************
模块名       :
文件名       :  net_config.h
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
*  @date   2016.11.26
*******************************************************************************/
#ifndef NET_CONFIG_H
#define NET_CONFIG_H  

#include "datatypes.h"
#include "netbus/nbdef.h"

#include <string>
#include <atomic>
#include <map>
#include <memory>

#if 0
struct NODE_CONFIG
{
	int32u OccNo;							//!  节点在数据库中的ID
	int32u SlaveOccNo;
	int32u NodeType;						//!  节点类型
	int32u NetAIPAddr;                      //! IP address of net1, filled by cfg. 
	int32u NetBIPAddr;                      //! IP address of net2, filled by cfg. 

	int32u NetAIpV6Addr[4];                 //! IPV6 的A网地址
	int32u NetBIpV6Addr[4];                 //! IPV6 的B网地址

	int8u IsCheckTime;						//！ 是否对时
	int8u IsDefaultHost;					//！ 是否默认为主

	char HostName[NAME_SIZE];		    	//！ 主机名称
	char TagName[NAME_SIZE];			    //！ 别名
	// 实时值
	int32u HostSlave;						//！ 主从状态
	int8u CardStatus[2];					//！ AB网状态
};

typedef struct NODE_CONFIG CNodeConfig; 
/*! \struct  NET_CONFIG
*   \brief 网络节点定义 */
struct NET_CONFIG
{
	int32u IsDoubleNet;					//! 是否双网
	char CheckTime[NAME_SIZE];	        //! 对时节点别名
	int32u NodeCount;					//! 节点数
	CNodeConfig* pNodeConfig;			//! 节点表指针
};

#endif


struct NET_CONFIG;

class CNodeInfo : protected CNodeConfig
{
public:
	CNodeInfo();
	CNodeInfo(NODE_CONFIG *pNode);

	~CNodeInfo();

	enum CardIdx:int32u
	{
		NET_A = 0,
		NET_B = 1,
	};

public:
	int32u GetOccNo() const
	{
		return OccNo;
	}
	// 获取网卡状态
	int32u GetCardState(CardIdx nIdx) const
	{
		return m_nCardState[nIdx];
	}
	// 设置网卡状态
	void SetCardStarte(CardIdx nIdx,int32u nState)
	{
		m_nCardState[nIdx] = nState;

		this->CardStatus[nIdx] = static_cast<int8u> (nState);
	}
	// 获取节点状态
	int32u GetNodeState() const
	{
		return m_nHostState;
	}
	// 设置节点状态
	void SetNodeState(int32u nState)
	{
		m_nHostState = nState;

		this->HostSlave = nState;
	}

private:
	//! 双网的
	std::atomic<int32u> m_nCardState[2];
	//! 主从状态
	std::atomic<int32u> m_nHostState;

};

class CNetState
{
public:
	CNetState();
	CNetState(NET_CONFIG *pConfig);
	~CNetState();
public:


protected:
	NET_CONFIG *m_pNetConfig;

	std::map <int32u, std::shared_ptr<CNodeInfo>> m_mapNetConf;
};

#endif // NET_CONFIG_H


/** @}*/

