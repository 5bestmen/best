
#ifndef _HEARTBEAT_RECVIVER_H_
#define _HEARTBEAT_RECVIVER_H_

#include "ace/OS.h"

#include <string>
#include <atomic>

#include <QObject>
#include <QUdpSocket>
#include <QString>
#include <QTimer>
#include <QThread>
#include <QHostAddress>
#include <QMutex>

#include "datatypes.h"
#include "netbus/nbdef.h"

class CUdpRecver : public QObject
{
	Q_OBJECT
public:
	CUdpRecver(const unsigned short nPort, NET_CONFIG* pNetCfg, const int32u nMyNodeIndex, const int32u nAnotherIndex, const int nABNet);
	~CUdpRecver();

	void InitSocket();
private:

	
	void HandleMsg(const int8u* pData);

private slots:
	void Recv();

private:
	QMutex m_pMutex;

	std::atomic<bool> m_isInit;

	QString m_strIP;
	unsigned short m_nPort;

	// 当前节点索引
	int32u m_nMyNodeIndex;
	// 对侧节点索引
	int32s m_nAnotherIndex;

	// 全网节点信息
	NET_CONFIG* m_pNetCfg;

	// AB网
	int m_nABNet;

	//套接字对象
	QUdpSocket* m_pUdpSocket;

	// 是否存在主备
	std::atomic<bool> m_bHostSlave;

	// 循环多少次后，修改系统时间
	unsigned int m_nTimeLoop;
};

#endif
