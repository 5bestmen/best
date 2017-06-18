/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ����ͻ���
�ļ���       :  outputmessagerclient.cpp
�ļ�ʵ�ֹ��� :  ϵͳ����ͻ���
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \date 2015��11��30�� 
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
** \date 2015��11��30�� 
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
** \date 2015��11��30�� 
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
** \date 2015��11��30�� 
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
** \date 2015��11��30�� 
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