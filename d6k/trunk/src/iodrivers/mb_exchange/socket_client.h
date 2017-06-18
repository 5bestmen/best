/*! @file
<PRE>
********************************************************************************
ģ����       :  socket�ͻ���
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  HuangYuqing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author  HuangYuqing
*  @version 1.0
*  @date    2016.8.18
*/
#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <QAbstractSocket>
#include <QThread>
#include <QTimer>

class QTcpSocket;
class CApduRecver;

class CSocketClient : public QObject
{
	Q_OBJECT
public:
	CSocketClient();
	~CSocketClient();

	//��ʼ��socket
	void ConnectSocket(const QString &strIP, int nPort);
	//�ر�socket
	void CloseSocket();
	//д������
	bool WriteSocketData(const char *pSendData, int nLength);

signals: 
	void Signal_RecvNewData(QByteArray bydata);
	void Signal_ConnectSuccess();
	void Signal_SocketError(const QString &strError);

private slots:
	void Slot_recvNewData();
	void Slot_SocketError(QAbstractSocket::SocketError error);
	void Slot_ConnectSuccess();
	void Slot_ReconSocket(int nDeviceID, QString strIP, int nPort);
	void Slot_CloseSocket();
	//�Զ������ۺ���
	void Slot_ConnectServer();

private:
	QTcpSocket *m_pTcpSocket;
	QByteArray m_ByteRecvData;
	int m_nPort;
	QString m_strIp;
	//���ӱ�־ false��Ҫ������true����Ҫ����
	bool m_bServerConnected;
	
public:
	QTimer m_timer;
	

};

#endif // SOCKET_CLIENT_H