#include <QTcpSocket>
#include <QMutex>
#include <QHostAddress>
#include <QDebug>
#include "socket_client.h"
#include "ui_mb_exchange.h"

#define MAX_SOCKETMSG_BUF 256*256

CSocketClient::CSocketClient()
{
	m_pTcpSocket = new QTcpSocket;
	//连接标志初始化为false
	m_bServerConnected = false;
	//设置发送和接收缓冲区大小
	m_pTcpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, MAX_SOCKETMSG_BUF);
	m_pTcpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, MAX_SOCKETMSG_BUF);

	connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Slot_SocketError(QAbstractSocket::SocketError)),Qt::DirectConnection);
	//连接成功
	connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(Slot_ConnectSuccess()));
	//接收新数据
	connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(Slot_recvNewData()));

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(Slot_ConnectServer()));
	m_timer.setInterval(3000);
	m_timer.start();
}

CSocketClient::~CSocketClient()
{
	m_pTcpSocket->close();
	delete m_pTcpSocket;
	m_pTcpSocket = NULL;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::ConnectSocket
** \details socket连接
** \param  strIP IP地址
** \param  nPort 端口号
** \return void
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
void CSocketClient::ConnectSocket(const QString &strIP, int nPort)
{
	m_pTcpSocket->abort();
	m_ByteRecvData.clear();
	m_nPort = nPort;
	m_strIp = strIP;
	m_pTcpSocket->connectToHost(strIP, nPort);
}

//关闭socket连接
void CSocketClient::CloseSocket()
{
	m_pTcpSocket->close();
}

//将字符串按报文格式输出
QString FormatBuffer(const char* pBuff, int nMsgLen)
{
	QString strDebugData("");
	char szByte[10] = { '\0' };

	for (int i = 0; i < nMsgLen; i++)
	{
		sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
		strDebugData += szByte;
	}

	return strDebugData;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::WriteSocketData
** \details 写入socket数据
** \param  pSendData  写入的数据
** \param  nLength 写入数据的长度
** \return bool
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
bool CSocketClient::WriteSocketData(const char *pSendData, int nLength)
{
	if ((pSendData ==  NULL) || (nLength < 0))
	{
		return false;
	}
	qint64 sendLength = m_pTcpSocket->write(pSendData, nLength);

	QString strData = FormatBuffer(pSendData, nLength);

	qDebug() << QStringLiteral("发送数据：") << strData << QStringLiteral("发送长度:") << sendLength;

	if (sendLength < nLength)
	{
		return false;
	}

	return true;
}
/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::Slot_recvNewData
** \details 接收数据槽函数
** \return void
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
void CSocketClient::Slot_recvNewData()
{
	while (m_pTcpSocket->bytesAvailable())
	{

		QByteArray btArrayRecvData = m_pTcpSocket->readAll();
		//qDebug() << btArrayRecvData;
		emit Signal_RecvNewData(btArrayRecvData);
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::Slot_SocketError
** \details socket错误槽函数
** \param  error
** \return void
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
void CSocketClient::Slot_SocketError(QAbstractSocket::SocketError error)
{
	qDebug() << "error:" << error;
	emit Signal_SocketError(m_pTcpSocket->errorString());
	m_bServerConnected = false;
}

/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::Slot_ConnectSuccess
** \details socket连接成功槽函数
** \return void
** \author HuangYuqing
** \date   2016年8月18日
** \note 
********************************************************************************************************/
void CSocketClient::Slot_ConnectSuccess()
{
	emit Signal_ConnectSuccess();
	qDebug() << "Connect success";
	m_bServerConnected = true;

}

//手动重连槽函数
void CSocketClient::Slot_ReconSocket(int nDeviceID, QString strIP, int nPort)
{
	m_pTcpSocket->connectToHost(strIP, nPort);
}

//断开连接槽函数
void CSocketClient::Slot_CloseSocket()
{
	m_pTcpSocket->close();
}

//自动重连
void CSocketClient::Slot_ConnectServer()
{
	QString strIP = m_strIp;
	int nPort = m_nPort;
	//连接标志为false时启动重连
	if (!m_bServerConnected)
	{
		m_pTcpSocket->connectToHost(strIP, nPort);
	}
}