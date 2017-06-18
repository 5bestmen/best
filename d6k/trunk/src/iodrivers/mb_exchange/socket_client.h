/*! @file
<PRE>
********************************************************************************
模块名       :  socket客户端
文件名       :
文件实现功能 :
作者         :  HuangYuqing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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

	//初始化socket
	void ConnectSocket(const QString &strIP, int nPort);
	//关闭socket
	void CloseSocket();
	//写入数据
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
	//自动重连槽函数
	void Slot_ConnectServer();

private:
	QTcpSocket *m_pTcpSocket;
	QByteArray m_ByteRecvData;
	int m_nPort;
	QString m_strIp;
	//连接标志 false需要重连，true不需要重连
	bool m_bServerConnected;
	
public:
	QTimer m_timer;
	

};

#endif // SOCKET_CLIENT_H