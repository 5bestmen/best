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
	//���ӱ�־��ʼ��Ϊfalse
	m_bServerConnected = false;
	//���÷��ͺͽ��ջ�������С
	m_pTcpSocket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, MAX_SOCKETMSG_BUF);
	m_pTcpSocket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, MAX_SOCKETMSG_BUF);

	connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Slot_SocketError(QAbstractSocket::SocketError)),Qt::DirectConnection);
	//���ӳɹ�
	connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(Slot_ConnectSuccess()));
	//����������
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
** \details socket����
** \param  strIP IP��ַ
** \param  nPort �˿ں�
** \return void
** \author HuangYuqing
** \date   2016��8��18��
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

//�ر�socket����
void CSocketClient::CloseSocket()
{
	m_pTcpSocket->close();
}

//���ַ��������ĸ�ʽ���
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
** \details д��socket����
** \param  pSendData  д�������
** \param  nLength д�����ݵĳ���
** \return bool
** \author HuangYuqing
** \date   2016��8��18��
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

	qDebug() << QStringLiteral("�������ݣ�") << strData << QStringLiteral("���ͳ���:") << sendLength;

	if (sendLength < nLength)
	{
		return false;
	}

	return true;
}
/*! \fn 
********************************************************************************************************* 
** \brief  CSocketClient::Slot_recvNewData
** \details �������ݲۺ���
** \return void
** \author HuangYuqing
** \date   2016��8��18��
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
** \details socket����ۺ���
** \param  error
** \return void
** \author HuangYuqing
** \date   2016��8��18��
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
** \details socket���ӳɹ��ۺ���
** \return void
** \author HuangYuqing
** \date   2016��8��18��
** \note 
********************************************************************************************************/
void CSocketClient::Slot_ConnectSuccess()
{
	emit Signal_ConnectSuccess();
	qDebug() << "Connect success";
	m_bServerConnected = true;

}

//�ֶ������ۺ���
void CSocketClient::Slot_ReconSocket(int nDeviceID, QString strIP, int nPort)
{
	m_pTcpSocket->connectToHost(strIP, nPort);
}

//�Ͽ����Ӳۺ���
void CSocketClient::Slot_CloseSocket()
{
	m_pTcpSocket->close();
}

//�Զ�����
void CSocketClient::Slot_ConnectServer()
{
	QString strIP = m_strIp;
	int nPort = m_nPort;
	//���ӱ�־Ϊfalseʱ��������
	if (!m_bServerConnected)
	{
		m_pTcpSocket->connectToHost(strIP, nPort);
	}
}