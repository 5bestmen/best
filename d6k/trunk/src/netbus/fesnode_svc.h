/*! @file fesnode_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  fesnode_svc.h
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
	// 获取当前主、从服务器
	std::pair< std::shared_ptr<CConnectorMgr>, std::shared_ptr<CConnectorMgr>>  GetCurServerList();
	// 发送数据到主服务器
	bool SendDataToServer(const int8u *pMsg, size_t nLen, int32u msgLevel = GENERAL_MSG);

	// 发送数据到从机
	bool SendDataToSlave(const int8u *pMsg, size_t nLen, int32u msgLevel = GENERAL_MSG);

protected:
	std::atomic<unsigned int>  m_nCurWorkingNet;   //! 当前连接服务

	std::shared_ptr<CConnectorMgr> m_pConnectSvrA;  //! 作为客户端连接主服务器A
	std::shared_ptr<CConnectorMgr> m_pConnectSvrB;  //! 作为客户端连接主服务器B

	std::shared_ptr<CConnectorMgr> m_pConnectAnother;  //! 作为客户端连接对侧

	//! 前置节点，主从冗余时，从机主动连接主机，
	std::shared_ptr<CAcceptorMgr>  m_pMySvr;

	//! 供其他节点的远程数据服务
	std::shared_ptr<CAcceptorMgr> m_pRemoteSvr;

	//! 是否双主服务器
	bool m_bIsDoubleSvr;

	// 主服务器A索引
	int m_nSvrIndexA;

	// 主服务器B索引
	int m_nSvrIndexB;

	//! 本节点是否冗余节点
	bool m_bIsRedNode;

	// 网络配置信息
	NET_CONFIG* m_pNetConfig;

	// 当前节点索引
	int32u m_nCurrentNodeIndex;

	// 当前节点的对侧节点索引
	int m_nAnotherNodeIndex;
};

#endif // FESNODE_SVC_MODULE_H


/** @}*/

