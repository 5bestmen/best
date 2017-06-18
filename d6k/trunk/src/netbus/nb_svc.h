/*! @file nb_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  nb_svc.h
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
*  @date    2016.10.22
*******************************************************************************/
#ifndef NB_SVC_MODULE_H
#define NB_SVC_MODULE_H
 

#include "datatypes.h"
#include <string>
#include <set>
#include <atomic>

#include <QObject>
#include <QTimer>

#include "thread_manager.h"
#include "heartbeat_sender.h"
#include "heartbeat_receiver.h"

#include "task_timer.h"

struct NET_CONFIG;
struct NODE_CONFIG;
class CNodeSvc;

class CNetbusSvc : public QObject
{
	Q_OBJECT
public:
	explicit CNetbusSvc(const char * pszPredicateName);
	~CNetbusSvc( );
protected:
	CNetbusSvc();
public:
	virtual bool Initialize(NET_CONFIG * pConf,const char *pszDataPath, int32u nNodeType, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown(); 
public:
	std::string &GetName() 
	{
		return m_szPredicateName;
	}

	int SendData(const int8u * pMsg, int32u nLen);
	bool RecvData(int8u * pMsg, int32u nMsgLen, int32u &nCopySize, int32u ulTimeout);

	bool ManualSwitchNode();

	bool GetShutdownFlag()
	{
		return m_bShutdown;
	}

protected:
	struct NODE_CONFIG *GetMyNodeConfig(struct NET_CONFIG *pConf)const;
	struct NODE_CONFIG *GetSlaveNodeConfig(struct NET_CONFIG *pConf) const;
	bool CheckMyNodeIpAddr(struct NODE_CONFIG *pMyNode)const;
	bool GetAllLocalIpV4Addr(std::set<std::string> & arrIpAddr) const;
	bool IsMyIpV4Addr(const char* addr)const;

	void LogMsg(const char *szLogTxt, int nLevel);

	bool GetMyNodeIndex(const NET_CONFIG & conf, int32u nNodeType,int & nIdx) const;
	bool GetMyNodeIndex(struct NET_CONFIG *pConf, int32u nNodeType, int & nIdx) const;

	bool StartNodeSvc(NET_CONFIG *pNetConf, int32u nMyNodeIdx);

	// 启动心跳
	bool StartHeartbeat(NET_CONFIG *pNetConf, const int32u nMyNodeIdx, const int32s nAnotherIndex);

public Q_SLOTS:
	// 发送心跳报文
	void Slot_SendHeartbeatMsg();

	// 检查对侧节点状态
	void Slot_CheckAnotherNodeState();

	// 检查整个网络节点状态
	void Slot_CheckAllNodeState();
private:

	int32u m_nMyNodeIdx;
	int32s m_nAnotherIndex;

	CCheckNodeState* m_pCheckNetNodeState;

	CCheckNodeState* m_pCheckHostSlaveState;

	bool CheckNodeTypeOnTheSide(NET_CONFIG *&pNetConf, const int32u nMyNodeIdx, const int32u nAnotherIndex);

protected:

	CNodeSvc * m_pNodeSvc; // 节点基类指针

	// 发送心跳报文定时器
	QTimer* m_pHeartbeatTimer;

	// 检测对侧节点状态和主备信息定时器
	QTimer* m_pCheckAnotherNodeState;

	// 检测整个网络节点定时器
	QTimer* m_pCheckAllNodeState;

	// UDP
	CUdpSender* m_pUdbSender;

	CUdpRecver* m_pUdbRecver;
	// A网


	// B网

	std::set<std::string> m_setMyIpAddr; //! 本机的所有IP

 	NET_CONFIG * m_pNetConfig;      //! 整个系统的网络配置情况

	std::string  m_szPredicateName; //! 调用模块名称，FES,SCADA
	std::string  m_szDataPathName;  //! 工程路径

									// 是否存在主备
	std::atomic<bool> m_isExistMainSlave;

	//! 是否shutdown
	std::atomic<bool> m_bShutdown;

	// 线程互斥锁
	ACE_Thread_Mutex m_mutex;
};

#endif // NB_SVC_MODULE_H


/** @}*/

