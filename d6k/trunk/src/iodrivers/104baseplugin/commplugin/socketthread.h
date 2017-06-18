/********************************************************************************
模块名       :  socket通信管理
文件名       :  socketthread.h
文件实现功能 :  实现socket通信 连接 接收数据  发送数据
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef CSOCKETTHREAD_H
#define CSOCKETTHREAD_H
#include <QThread>
#include <QAbstractSocket>

class QTcpSocket;
class QMutex;
class CApduRecver;

class CSocketThread : public QObject
{
    Q_OBJECT
public:
    CSocketThread(QObject* parent=0);
    ~CSocketThread();
    //初始化socket
    void  ConnectSocket(const QString &strIP, int iPort);
    //关闭socket
    void CloseSocket();
    //获取数据
    QByteArray GetNetworkData();
    //写入数据
    bool WriteSocketData(const char *pSendData, int iLength);
	void setApduRecver(CApduRecver *pRecver);

signals:
    //错误信号
    void Signal_SocketError(QString strError);
    //连接成功信号
    void Signal_ConnectSuccess(QString strLocalInfo);
	//发送出去的数据
	void Signal_Write16Data(QByteArray btbuff);

private slots:
    void Slot_recvNewData();
    void Slot_SocketError(QAbstractSocket::SocketError error);
	//连接成功
	void Slot_ConnectSuccess();

private:
    QTcpSocket *m_pTcpScoket;
    //接收到的数据
    QByteArray m_BtayRecvData;
    //锁
    QMutex *m_pMutex;
	//处理类
	CApduRecver *m_pApduRecver;
};

#endif // CSOCKETTHREAD_H
