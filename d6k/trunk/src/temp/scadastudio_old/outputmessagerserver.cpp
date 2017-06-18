/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ��������
�ļ���       :  outputmessagerserver.cpp
�ļ�ʵ�ֹ��� :  ϵͳ��������
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
#include "outputmessagerserver.h"

/*! \fn COutputMessagerServer::COutputMessagerServer(QObject *parent)
********************************************************************************************************* 
** \brief COutputMessagerServer::COutputMessagerServer 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
COutputMessagerServer::COutputMessagerServer(QObject *parent)
	: QObject(parent)
{
	m_tcpServer = new QTcpServer(this);

	m_tcpServer->listen(QHostAddress::Any,19999); //�����κ�����19999�˿ڵ�ip  

	connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection())); //�������źŴ���������newConnect()�ۺ�����������źź���һ������ʵ��������ȡ��
}

/*! \fn COutputMessagerServer::~COutputMessagerServer()
********************************************************************************************************* 
** \brief COutputMessagerServer::~COutputMessagerServer 
** \details 
** \return  
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
COutputMessagerServer::~COutputMessagerServer()
{

}

/*! \fn void COutputMessagerServer::newConnection()
********************************************************************************************************* 
** \brief COutputMessagerServer::newConnection 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void COutputMessagerServer::newConnection()
{
	QTcpSocket *newsocket = m_tcpServer->nextPendingConnection(); 

	connect((QObject *)newsocket,SIGNAL(readyRead()),this,SLOT(readyRead()));
}

/*! \fn void COutputMessagerServer::readyRead()
********************************************************************************************************* 
** \brief COutputMessagerServer::readyRead 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void COutputMessagerServer::readyRead()
{
	QTcpSocket *local = (QTcpSocket *)(sender()); 

	QByteArray qba= local->readAll(); //��ȡ  

	qDebug()<<qba;  

	QString ss=QVariant(qba).toString(); 
}