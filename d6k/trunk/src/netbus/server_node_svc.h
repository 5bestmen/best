/*! @file server_node_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  server_node_svc.h
文件实现功能 :  前置节点的网络服务
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  1.作为client去连接server
                2.如果在从机状态下作为client去连接主机
				3.如果在主机状态下，等待从机的连接
				4.作为server，给部分应用提供服务
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

	// 节点分类(从网络节点表中将前置机和工作站分类)
	//bool NodeSplit();

	/*struct NODE_INFO
	{
		CONNECTION_IP
	};

	// 存放前置节点IP
	std::vector<CONNECTION_IP> m_vecFesNodeIP;

	// 存放工作站节点IP
	std::vector<CONNECTION_IP> m_vecWorkStationNodeIP;*/

private:
	//! 作为客户端连接对侧
	std::shared_ptr<CConnectorMgr> m_pConnectPeer;

	//! 主从冗余时，从机主动连接主机，
	//std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! 供其他节点的远程数据服务
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr;

	//! 是否双主服务器
	bool m_bDoubleSvr;

	//! 本节点是否冗余节点
	bool m_bIsRedNode;

	// 网络配置信息
	NET_CONFIG* m_pNetConfig;

	// 当前节点索引
	int m_nCurrentNodeIndex;

	// 当前节点的对侧节点索引
	int m_nAnotherNodeIndex;
};

#endif // SERVER_NODE_SVC_MODULE_H


/** @}*/

