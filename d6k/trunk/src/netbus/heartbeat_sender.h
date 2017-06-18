
#ifndef _HEARTBEAT_SENDER_H_
#define _HEARTBEAT_SENDER_H_

#include "ace/OS.h"

#include <string>
#include <atomic>

#include <QUdpSocket>
#include <QString>
#include <QTimer>

#include "datatypes.h"
#include "netbus/nbdef.h"

class CUdpSender
{

public:
	CUdpSender(const QString& strIP, const unsigned short nPort, NET_CONFIG* pNodeCfg, int32u nMyNodeIdx, int32s nAnotherIndex, bool bCheckTime = false, bool isExistMainSlave = false);
	~CUdpSender();

	void InitSocket();

	void Send();

private:
	std::atomic<bool> m_isInit;

	QString m_strIP;
	unsigned short m_nPort;
	NET_CONFIG* m_pNetCfg;
	bool m_bCheckTime;

	int32u m_nMyNodeIdx;
	int32s m_nAnotherIndex;

	// 是否存在主备节点
	std::atomic<bool> m_isExistMainSlave;

	//套接字对象
	QUdpSocket* m_pUdpSocket;
};


#endif
