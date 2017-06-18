/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出客户端
文件名       :  outputmessagerclient.cpp
文件实现功能 :  系统输出客户端
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.30
*/

#include <QTcpSocket>
#include "outputmessagerclient.h"

/*! \fn COutputMessagerClient::COutputMessagerClient(QObject *parent)
********************************************************************************************************* 
** \brief COutputMessagerClient::COutputMessagerClient 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
COutputMessagerClient::COutputMessagerClient(QObject *parent)
	: QObject(parent)
{
	m_tcpSocket = new QTcpSocket(this);

	m_tcpSocket->connectToHost("127.0.0.1", 19999, QIODevice::ReadWrite);

	if(m_tcpSocket->state() == QAbstractSocket::UnconnectedState)
	{
		
	}
	else
	{

	}
}

/*! \fn COutputMessagerClient::~COutputMessagerClient()
********************************************************************************************************* 
** \brief COutputMessagerClient::~COutputMessagerClient 
** \details 
** \return  
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
COutputMessagerClient::~COutputMessagerClient()
{
	m_tcpSocket->deleteLater();
}


/*! \fn bool COutputMessagerClient::checkConnect()
********************************************************************************************************* 
** \brief COutputMessagerClient::checkConnect 
** \details 
** \return bool 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
bool COutputMessagerClient::checkConnect()
{
	return m_tcpSocket->isOpen();
}

/*! \fn bool COutputMessagerClient::connectHost(QString host, int port)
********************************************************************************************************* 
** \brief COutputMessagerClient::connectHost 
** \details 
** \param host 
** \param port 
** \return bool 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
bool COutputMessagerClient::connectHost(QString host/* ="127.0.0.1" */, int port /* = 19999 */)
{
	m_tcpSocket->connectToHost(host, port, QIODevice::ReadWrite);

	return true;
}

/*! \fn void COutputMessagerClient::sendData(char *data, int length)
********************************************************************************************************* 
** \brief COutputMessagerClient::sendData 
** \details 
** \param data 
** \param length 
** \return void 
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void COutputMessagerClient::sendData(char *data, int length)
{
	if(m_tcpSocket->waitForConnected())
	{
		m_tcpSocket->write(data, length);

		//m_tcpSocket->waitForBytesWritten();

		m_tcpSocket->flush();
	}
}