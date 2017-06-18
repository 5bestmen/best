/*! @file client_node_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  client_node_svc.h
文件实现功能 :  工作站节点的网络服务
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
*  @brief   工作站节点的网络服务
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
	std::shared_ptr<CConnectorMgr> m_pConnectSvrA;  //! 作为客户端连接主服务器A
	std::shared_ptr<CConnectorMgr> m_pConnectSvrB;  //! 作为客户端连接主服务器B

	std::shared_ptr<CConnectorMgr> m_pConnectPeer;  //! 作为客户端连接对侧

	//! 主从冗余时，从机主动连接主机，
	std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! 供其他节点的远程数据服务
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr; 
	//! 是否双主服务器
	bool m_bDoubleSvr;
	//! 本节点是否冗余节点
	bool m_bIsRedNode;

private:
	NET_CONFIG* m_pNetCfgMain;
	NET_CONFIG* m_pNetCfgA;

	// 网络节点表
	NET_CONFIG* m_pNetCfg;

	// 当前节点的对侧节点索引
	int m_nAnotherNodeIndex;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/


