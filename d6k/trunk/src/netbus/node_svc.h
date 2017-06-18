/*! @file node_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  node_svc.h
文件实现功能 :  节点基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   节点基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.24
*******************************************************************************/
#ifndef NODE_SVC_MODULE_H
#define NODE_SVC_MODULE_H  

#include "datatypes.h"

#include <string>
#include <atomic>
#include <vector>

#include "netbus/nbdef.h"
#include "node_manager.h"
 
class CNodeSvc  
{
public:  
	CNodeSvc(NET_CONFIG* pNetConfig, int32u nCurrentNodeIndex);
	virtual ~CNodeSvc();
public:
	virtual bool Initialize(const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
public:
	virtual bool SendData(const int8u * pMsg, int32u nLen);
	virtual bool RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout = 0);

	void SetNodeState(int nState);
	int GetNodeState() const
	{
		return m_nNodeState;
	}

	virtual int GetAnotherNodeIndex() = 0;

	// 节点分类(从网络节点表中将前置机和工作站分类)
	bool NodeSplit();

	// 网络配置信息
	NET_CONFIG* m_pNetConfig;

	int32u m_nCurrentNodeIndex;

	struct NODE_INFO
	{
		CONNECTION_IP addr;
		int32u nDestOccNo;
	};

	// 存放前置节点IP
	std::vector<NODE_INFO> m_vecFesNodeIP;

	// 存放工作站节点IP
	std::vector<NODE_INFO> m_vecWorkStationNodeIP;

protected:
	void LogMsg(const char *szLogTxt, int nLevel);
protected:
	std::atomic <int> m_nNodeState;  //! 节点状态

};

#endif // NODE_SVC_MODULE_H


/** @}*/

