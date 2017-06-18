
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

	// ��ǰ�ڵ�����
	int32u m_nMyNodeIndex;
	// �Բ�ڵ�����
	int32s m_nAnotherIndex;

	// ȫ���ڵ���Ϣ
	NET_CONFIG* m_pNetCfg;

	// AB��
	int m_nABNet;

	//�׽��ֶ���
	QUdpSocket* m_pUdpSocket;

	// �Ƿ��������
	std::atomic<bool> m_bHostSlave;

	// ѭ�����ٴκ��޸�ϵͳʱ��
	unsigned int m_nTimeLoop;
};

#endif
