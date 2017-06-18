/*! @file nb_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  nb_svc.h
�ļ�ʵ�ֹ��� :  ��������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��������
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

	// ��������
	bool StartHeartbeat(NET_CONFIG *pNetConf, const int32u nMyNodeIdx, const int32s nAnotherIndex);

public Q_SLOTS:
	// ������������
	void Slot_SendHeartbeatMsg();

	// ���Բ�ڵ�״̬
	void Slot_CheckAnotherNodeState();

	// �����������ڵ�״̬
	void Slot_CheckAllNodeState();
private:

	int32u m_nMyNodeIdx;
	int32s m_nAnotherIndex;

	CCheckNodeState* m_pCheckNetNodeState;

	CCheckNodeState* m_pCheckHostSlaveState;

	bool CheckNodeTypeOnTheSide(NET_CONFIG *&pNetConf, const int32u nMyNodeIdx, const int32u nAnotherIndex);

protected:

	CNodeSvc * m_pNodeSvc; // �ڵ����ָ��

	// �����������Ķ�ʱ��
	QTimer* m_pHeartbeatTimer;

	// ���Բ�ڵ�״̬��������Ϣ��ʱ��
	QTimer* m_pCheckAnotherNodeState;

	// �����������ڵ㶨ʱ��
	QTimer* m_pCheckAllNodeState;

	// UDP
	CUdpSender* m_pUdbSender;

	CUdpRecver* m_pUdbRecver;
	// A��


	// B��

	std::set<std::string> m_setMyIpAddr; //! ����������IP

 	NET_CONFIG * m_pNetConfig;      //! ����ϵͳ�������������

	std::string  m_szPredicateName; //! ����ģ�����ƣ�FES,SCADA
	std::string  m_szDataPathName;  //! ����·��

									// �Ƿ��������
	std::atomic<bool> m_isExistMainSlave;

	//! �Ƿ�shutdown
	std::atomic<bool> m_bShutdown;

	// �̻߳�����
	ACE_Thread_Mutex m_mutex;
};

#endif // NB_SVC_MODULE_H


/** @}*/

