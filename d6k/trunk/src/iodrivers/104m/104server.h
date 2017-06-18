#ifndef _104SERVER_H
#define _104SERVER_H
#include "datatypes.h"
#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QTimerEvent>
#include <QTcpSocket>
#include <QThread>
#include <QEventLoop>
#include <memory>

typedef struct
{
	int m_nTimer;
	int m_nCounter;
	bool m_bIsUse;

}TIMER;

class C104Channel;
class C104Master;

class C104Server : public QObject
{
	Q_OBJECT

public:
	C104Server(QObject *parent, int32u nOccNo, std::shared_ptr<C104Channel> pKernel);
	//C104Server(int32u nOccNo, std::shared_ptr<C104Channel> pKernel);
	~C104Server();

public:
	void start();
	void stop();
public:
	void ConnectToHost();
	void LogMsg(char* pszText,int nLevel);
private Q_SLOTS:
	void Handle_On_Connected();
	void Handle_On_Disconnected();
	void Handle_On_ReadData(char *pBuf, int nLen);
	void Handle_On_WriteData(char *pBuf, int nLen);
	void Handle_On_Recv();
	void slot_onTimer();
	void Handle_On_DisplayError(QAbstractSocket::SocketError error);
public:

	virtual bool OnOpen() { return true; }
	virtual int  OnClose() { return 0; }
	virtual bool OnReceive(char* pBuff, int nLen) { return true; }
	virtual bool OnTimer(int nTimerID) { return true; }
public:
	bool StartTimer(int nTimerID, int nSeconds);
	void KillTimer(int nTimerID);
	void RemoveAllTimer();
	bool Send(char* buf, int len);
private:
	QMutex m_mutex;
	QMap< int, TIMER* > m_mapTimers;
private:
	QTcpSocket *m_pSocket;
	QTimer *m_pTimer;
	bool m_bIsEnding;                  
	bool m_bIsAllowConnect;            
	bool m_nIsConnected;               
	int m_nTimerAlive;                 	
private:
	int32u m_nOccNo;
	std::shared_ptr<C104Channel> m_pKernel;
	int m_nGroupID;
	int m_nChannelID;
private:                                
	QString m_szIP;
	int m_nPort;
private:                               
//	QEventLoop m_eLoop;
	bool m_bLoopFlag;
	
};

#endif // _104SERVER_H
