/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出服务端
文件名       :  outputmessagerserver.cpp
文件实现功能 :  系统输出服务端
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
#include "outputmessagerserver.h"

/*! \fn COutputMessagerServer::COutputMessagerServer(QObject *parent)
********************************************************************************************************* 
** \brief COutputMessagerServer::COutputMessagerServer 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
COutputMessagerServer::COutputMessagerServer(QObject *parent)
	: QObject(parent)
{
	m_tcpServer = new QTcpServer(this);

	m_tcpServer->listen(QHostAddress::Any,19999); //监听任何连上19999端口的ip  

	connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(newConnection())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样，其实你可以随便取。
}

/*! \fn COutputMessagerServer::~COutputMessagerServer()
********************************************************************************************************* 
** \brief COutputMessagerServer::~COutputMessagerServer 
** \details 
** \return  
** \author gw
** \date 2015年11月30日 
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
** \date 2015年11月30日 
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
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void COutputMessagerServer::readyRead()
{
	QTcpSocket *local = (QTcpSocket *)(sender()); 

	QByteArray qba= local->readAll(); //读取  

	qDebug()<<qba;  

	QString ss=QVariant(qba).toString(); 
}