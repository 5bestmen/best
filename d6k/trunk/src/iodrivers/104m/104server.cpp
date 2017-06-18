#include "104server.h"
#include "log/log.h"
#include "fesapi/fesapi.h"
#include <QDateTime>
#include <QAbstractSocket>

C104Server::C104Server(QObject *parent,int32u nOccNo, std::shared_ptr<C104Channel> pKernel) : QObject(parent)
{
	m_nOccNo = nOccNo;
	m_pKernel = pKernel;

	m_bIsAllowConnect = true;
	m_bIsEnding = false;
	m_nIsConnected = false;
	m_bLoopFlag = false;

	m_szIP = "127.0.0.1";
	if (nOccNo==1)
	{
		m_nPort = 2304;
	}
	else
	{
		m_nPort = 2404;
	}
	

	m_pTimer = new QTimer;
	QObject::connect(m_pTimer, &QTimer::timeout, this, &C104Server::slot_onTimer);
	m_pTimer->start(1000);

	m_pSocket = new QTcpSocket;
	QObject::connect(m_pSocket, &QTcpSocket::connected, this, &C104Server::Handle_On_Connected);
	QObject::connect(m_pSocket, &QTcpSocket::disconnected, this, &C104Server::Handle_On_Disconnected);
	QObject::connect(m_pSocket, &QTcpSocket::readyRead, this, &C104Server::Handle_On_Recv);
	QObject::connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Handle_On_DisplayError(QAbstractSocket::SocketError)));

}

C104Server::~C104Server()
{
	QMapIterator<int, TIMER* > i(m_mapTimers);
	while (i.hasNext())
	{
		i.next();
		TIMER *pTimer = i.value();
		if (pTimer)
		{
			delete pTimer;
			pTimer = NULL;
		}
		m_mapTimers.remove(i.key());
	}
}

void C104Server::start()
{
	ConnectToHost();
}

void C104Server::stop()
{

}

void C104Server::ConnectToHost()
{
	m_pSocket->close();
	if (!m_bIsEnding && m_bIsAllowConnect)
	{
		m_pSocket->connectToHost(m_szIP, m_nPort, QIODevice::ReadWrite);

		char buf[256];
		sprintf(buf, "[TIME:%s][CONNECTING]--->ip:%s,port:%d", QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss").toStdString().c_str(), m_szIP.toStdString().c_str(), m_nPort);

		qDebug() << "socket connect:" << m_szIP;
	}
}

void C104Server::LogMsg(char* pszText, int nLevel)
{
	/*Q_ASSERT(pszText && strlen(pszText )>0);
	if (!pszText || strlen( pszText)==0 )
	{
		return;
	}
	
	::LogMsg("C104Server", pszText, nLevel, nullptr);*/
}

void C104Server::Handle_On_Connected()
{
	m_nIsConnected = true;
	OnOpen();
	char buf[256];
	sprintf(buf, "[TIME:%s][CONNECTED]--->ip:%s,port:%d", 
		QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss").toStdString().c_str(), m_szIP.toStdString().c_str(), m_nPort);
	
	IoDiagAlarm(m_nOccNo, 1, "104装置连接成功", nullptr);

	LogMsg(buf, 0);
}

void C104Server::Handle_On_Disconnected()
{
	OnClose();
	RemoveAllTimer();
	m_nIsConnected = false;
	char buf[256];
	sprintf(buf, "[TIME:%s][DISCONNECTED]--->ip:%s,port:%d",
		QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss").toStdString().c_str(), m_szIP.toStdString().c_str(), m_nPort);
	
	IoDiagAlarm(m_nOccNo, 1, "104装置断开连接", nullptr);
	
	LogMsg(buf, 0);
}

void C104Server::Handle_On_ReadData(char *pBuf, int nLen)
{
	Q_ASSERT(pBuf && nLen);

	if (!pBuf ||nLen <=0)
	{
		return;
	}

	if (!m_bIsEnding)
	{
		m_pSocket->read(pBuf, nLen);
	}
	else
		return;
}

void C104Server::Handle_On_WriteData(char *pBuf, int nLen)
{
	Q_ASSERT(pBuf && nLen);

	if (!pBuf || nLen <= 0)
	{
		return;
	}

	int m_r = m_pSocket->write(pBuf, nLen);

	Q_ASSERT(m_r);

	return;
}

/*! \fn void C104Server::Handle_On_Recv()
********************************************************************************************************* 
** \brief C104Server::Handle_On_Recv 
** \details 基本数据入口
** \return void 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
void C104Server::Handle_On_Recv()
{
	int nCount = m_pSocket->bytesAvailable();

	if (nCount < 6)
	{
		return;
	}

	QByteArray arrayData = m_pSocket->readAll();

	int nRecv = arrayData.size();

	Q_ASSERT(nRecv >= 6 );

	bool bRet=OnReceive(arrayData.data(), nRecv);

	Q_ASSERT(bRet);

	char kbuf[512];

	sprintf(kbuf, "[TIME:%s][RECV_DATA][Ip:%s:Port:%d],recvData:%d", QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss").toStdString().c_str(), m_szIP.toStdString().c_str(), m_nPort, nRecv);
	
	LogMsg(kbuf, 0);
	QString str;

	qDebug() << kbuf << str.prepend(arrayData);
}

/*! \fn void C104Server::Handle_On_DisplayError(QAbstractSocket::SocketError error)
********************************************************************************************************* 
** \brief C104Server::slot_onTimer 
** \details 定时处理所有事件，超时事件
** \return void 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
void C104Server::slot_onTimer()
{
	static unsigned int cnts = 1;
	//定时检查链路状态
	if (true)
	{
		cnts++;
		if (!(cnts % 5))
		{
			qDebug() << QDateTime::currentDateTime().time().toString("yyyy-MM-dd:hh:mm:ss");
			if (!m_nIsConnected)
			{
				ConnectToHost();
			}
		}
	}
	if (!m_nIsConnected)
	{
		SETVAL_MSG pMsg;
		ReadHostCmd(m_nOccNo, &pMsg, 0);
		return;
	}

	m_mutex.tryLock();
	//判断是否超时
	QMapIterator<int, TIMER* > it(m_mapTimers);
	while (it.hasNext())
	{
		it.next();
		qDebug() << it.key() << it.value()->m_nTimer << it.value()->m_nCounter;
		if (it.value()->m_nCounter == 0 && it.value()->m_bIsUse)
		{
			if (!OnTimer(it.key()))
			{
				m_pSocket->close();
				m_nIsConnected = false;
				break;
			}
			it.value()->m_nCounter = it.value()->m_nTimer;
		}
		else  if (it.value()->m_nCounter == 0)
		{
			it.value()->m_nCounter = it.value()->m_nTimer;
		}
	}
	QMapIterator<int, TIMER* > itt(m_mapTimers);
	while (itt.hasNext())
	{
		itt.next();
		if (itt.value()->m_nCounter > 0 && itt.value()->m_bIsUse)
		{
			itt.value()->m_nCounter--;
		}
	}
}

/*! \fn void C104Server::Handle_On_DisplayError(QAbstractSocket::SocketError error)
********************************************************************************************************* 
** \brief C104Server::Handle_On_DisplayError 
** \details 查看出现某个错误
** \param error 
** \return void 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
void C104Server::Handle_On_DisplayError(QAbstractSocket::SocketError error)
{
	if (error != QAbstractSocket::SocketTimeoutError)
	{
		qDebug() << "socket error:" << error;

		char buf[512];
		sprintf(buf, "[TIME:%s][CONNECT_ERROR]--->ip:%s,port:%d,error:%d", 
			QDateTime::currentDateTime().toString("yyyy-MM-dd:hh:mm:ss").toStdString().c_str(), m_szIP.toStdString().c_str(), m_nPort, error);

		LogMsg(buf, 0);
	}
}

/*! \fn bool C104Server::StartTimer(int nTimerID, int nSeconds)
********************************************************************************************************* 
** \brief C104Server::StartTimer 
** \details 启动某个定时器
** \param nTimerID 
** \param nSeconds 
** \return bool 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
bool C104Server::StartTimer(int nTimerID, int nSeconds)
{
	m_mutex.tryLock();

	if (m_mapTimers.contains(nTimerID))
	{
		TIMER *pTimer = m_mapTimers[nTimerID];
		if (pTimer == NULL)
		{
			return false;
		}
		pTimer->m_nTimer = nSeconds;
		pTimer->m_nCounter = nSeconds;
		pTimer->m_bIsUse = true;
	}
	else
	{
		TIMER *pTimer = new TIMER;

		if (pTimer == NULL)
		{
			return false;
		}
		pTimer->m_nTimer = nSeconds;
		pTimer->m_nCounter = nSeconds;
		pTimer->m_bIsUse = true;

		m_mapTimers[nTimerID] = pTimer;
	}
	return true;
}

/*! \fn void C104Server::KillTimer(int nTimerID)
********************************************************************************************************* 
** \brief C104Server::KillTimer 
** \details 取消某个定时器
** \param nTimerID 
** \return void 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
void C104Server::KillTimer(int nTimerID)
{
	m_mutex.tryLock();

	QMapIterator<int, TIMER* > i(m_mapTimers);

	while (i.hasNext())
	{
		i.next();
		if (i.key() == nTimerID)
		{
			TIMER* pTimer = *&i.value();
			pTimer->m_nCounter = pTimer->m_nTimer;
			pTimer->m_bIsUse = false;
			return;
		}
	}
}

/*! \fn void C104Server::RemoveAllTimer()
********************************************************************************************************* 
** \brief C104Server::RemoveAllTimer 
** \details 取消所有定时器
** \return void 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
void C104Server::RemoveAllTimer()
{
	m_mutex.tryLock();
	QMapIterator<int, TIMER* > i(m_mapTimers);
	while (i.hasNext())
	{
		i.next();
		KillTimer(i.key());
	}
}


/*! \fn bool C104Server::Send(char* pBuf, int len)
********************************************************************************************************* 
** \brief C104Server::Send 
** \details 基本发送接口
** \param pBuf 
** \param len 
** \return bool 
** \author xingzhibing
** \date 2017年3月3日 
** \note 
********************************************************************************************************/
bool C104Server::Send(char* pBuf, int len)
{
	Q_ASSERT(pBuf && len );
	if (!pBuf || len <=0)
	{
		return false;
	}
	return m_pSocket->write(pBuf, len);
}
