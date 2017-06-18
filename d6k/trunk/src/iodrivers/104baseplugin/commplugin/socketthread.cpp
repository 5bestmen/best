#include <QTcpSocket>
#include <QMutex>
#include <QHostAddress>
#include "socketthread.h"
#include "apdurecver.h"
#include "define_104.h"

#define MAX_MSG_SOCKETBUFF    1024*1024    //SOCKET数据缓存大小

/*********************************************************************************************************
** \brief CSocketThread(QObject *parent)
** \details 构造函数
** \param argl  父类指针
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
**********************************************************************************************************/
CSocketThread::CSocketThread(QObject *parent):
    QObject(parent),
    m_pTcpScoket(new QTcpSocket),
    m_pMutex(new QMutex)
{
	m_pApduRecver = NULL;

	//设置发送和结构缓冲区大小
    m_pTcpScoket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption,MAX_MSG_SOCKETBUFF);
    m_pTcpScoket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption,MAX_MSG_SOCKETBUFF);

    connect(m_pTcpScoket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(Slot_SocketError(QAbstractSocket::SocketError)),Qt::DirectConnection);
    //连接成功
	connect(m_pTcpScoket, SIGNAL(connected()), this, SLOT(Slot_ConnectSuccess()));
    //接收新数据
    connect (m_pTcpScoket,SIGNAL(readyRead()),this,SLOT(Slot_recvNewData()));

}

CSocketThread::~CSocketThread()
{
    m_pTcpScoket->close();
    delete m_pTcpScoket;
    delete m_pMutex;
    m_pTcpScoket = NULL;
}

/*********************************************************************************************************
** \brief ConnectSocket(const QString &strIP, int iPort)
** \details socket连接
** \param argl  IP地址
** \param arg2  端口号
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::ConnectSocket(const QString &strIP, int iPort)
{
	m_pMutex->lock();
    m_pTcpScoket->abort();
    m_BtayRecvData.clear();
    m_pTcpScoket->connectToHost(strIP,iPort);
	//qDebug() << m_pTcpScoket->localAddress().toString() + ":" + QString::number(m_pTcpScoket->localPort());
	m_pMutex->unlock();
}

/*********************************************************************************************************
** \brief setApduRecver(CApduRecver *pRecver)
** \details recver类
** \param argl  
** \param arg2  
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/

void CSocketThread::setApduRecver(CApduRecver *pRecver)
{
	Q_ASSERT(pRecver);
	if (pRecver == NULL)
	{
		return;
	}
	m_pApduRecver = pRecver;
}


void CSocketThread::CloseSocket()
{
	m_pMutex->lock();
    m_pTcpScoket->close();
	m_pMutex->unlock();
}

/*********************************************************************************************************
** \brief GetNetworkData()
** \details 获取网络数据
** \param argl  
** \param arg2  
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
QByteArray CSocketThread::GetNetworkData()
{
//     m_pMutex->lock();
// 	if (m_BtayRecvData.isEmpty())
// 	{
// 		m_pMutex->unlock();
// 		return "";
// 	}
//     QByteArray btAryData = m_BtayRecvData;
//     m_BtayRecvData.clear();
//     m_pMutex->unlock();
// 
//     return btAryData;
	return "";
}

/*********************************************************************************************************
** \brief WriteSocketData(const char *pSendData, int iLength)
** \details 写入socket数据
** \param argl  需要写入的数据
** \param arg2  需要写入数据的长度
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
bool CSocketThread::WriteSocketData(const char *pSendData, int iLength)
{
    if ((pSendData == NULL) || (iLength < 0))
    {
        return false;
    }
    //qDebug() << "Senderdddddddddddddd:" << QByteArray(pSendData, iLength).toHex();
    m_pMutex->lock();
    qint64 sendLength =  m_pTcpScoket->write(pSendData,iLength);
    m_pMutex->unlock();

	//CommMsgLog("dddd", static_cast<unsigned char*> (pSendData), sendLength, CMSG_TCP_SEND, 0);

    if (sendLength < iLength)
    {
        return false;
    }

	emit Signal_Write16Data(QByteArray(pSendData,iLength));

    return true;
}

/*********************************************************************************************************
** \brief Slot_recvNewData()
** \details 接收网络数据槽函数
** \param argl  
** \param arg2  
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::Slot_recvNewData()
{
	while (m_pTcpScoket->bytesAvailable())
	{
		QByteArray btArray = m_pTcpScoket->readAll();
        //qDebug()<<"RecvXXX:" << btArray.toHex();
		if ((unsigned)btArray.length() < sizeof(APCI))
		{
			return;
		}
		m_pApduRecver->OnReceive(btArray.data(),btArray.length());
	}
}

/*********************************************************************************************************
** \brief Slot_SocketError(QAbstractSocket::SocketError error)
** \details socket错误槽函数
** \param argl   socket错误码
** \param arg2
** \return void
** \author ww
** \date 2016年5月27日
** \note
********************************************************************************************************/
void CSocketThread::Slot_SocketError(QAbstractSocket::SocketError error)
{
    qDebug()<<"error:"<<error;

    emit Signal_SocketError(m_pTcpScoket->errorString());
}

//连接成功
void CSocketThread::Slot_ConnectSuccess()
{
	QString strLocalInfo = m_pTcpScoket->localAddress().toString() + ":" + QString::number(m_pTcpScoket->localPort());
	emit Signal_ConnectSuccess(strLocalInfo);
}
