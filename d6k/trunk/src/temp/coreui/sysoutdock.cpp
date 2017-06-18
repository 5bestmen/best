/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ���dock����
�ļ���       :  sysoutdock.cpp
�ļ�ʵ�ֹ��� :  ϵͳ���dock����
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
*  @date    2015.11.15
*/

#include <QTcpServer>
#include <QTcpSocket>
#include <QTabWidget>
#include "sysoutdock.h"
#include "systemoutputview.h"
#include "systemoutputmodel.h"
#include "icore/define.h"


/*! \fn CSysOutDock::CSysOutDock(QWidget *parent)
********************************************************************************************************* 
** \brief CSysOutDock::CSysOutDock 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CSysOutDock::CSysOutDock(QWidget *parent)
	: QDockWidget(parent)
{	
	this->setWindowTitle(QStringLiteral("ϵͳ���"));
	
	m_messsageTab = new QTabWidget(this);

	m_messsageTab->setTabPosition(QTabWidget::South);

	setWidget(m_messsageTab);

	m_tcpServer = new QTcpServer(this);

	m_tcpServer->listen(QHostAddress::Any, 19999); //�����κ�����19999�˿ڵ�ip  

	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection())); //�������źŴ���������newConnect()�ۺ�����������źź���һ������ʵ��������ȡ��

	createCommandView();

	createLogCommandView();
}

/*! \fn CSysOutDock::~CSysOutDock()
********************************************************************************************************* 
** \brief CSysOutDock::~CSysOutDock 
** \details 
** \return  
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CSysOutDock::~CSysOutDock()
{

}

/*! \fn void CSysOutDock::createCommandView()
********************************************************************************************************* 
** \brief CSysOutDock::createCommandView 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void CSysOutDock::createCommandView()
{
	m_commandView = new CSystemOutputView(this);

	m_commandModel = new CSystemOutputModel(this);

	m_commandView->setModel(m_commandModel);

	m_commandView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_messsageTab->addTab(m_commandView, QIcon(":/images/new.png"), "CommandT");
}

/*! \fn void CSysOutDock::createLogCommandView()
********************************************************************************************************* 
** \brief CSysOutDock::createLogCommandView 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void CSysOutDock::createLogCommandView()
{
	m_logView = new CSystemOutputView(this);

	m_logModel = new CSystemOutputModel(m_logView);


	QVector<QString> vec;

	vec.push_back("test");

	//m_logModel->AddItem(vec);

	m_logView->setModel(m_logModel);

	m_logView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_messsageTab->addTab(m_logView, QIcon(":/images/new.png"), "LogT");
}

/*! \fn void CSysOutDock::newConnection()
********************************************************************************************************* 
** \brief CSysOutDock::newConnection 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void CSysOutDock::newConnection()
{
	QTcpSocket *newsocket = m_tcpServer->nextPendingConnection();

	connect((QObject *)newsocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

/*! \fn void CSysOutDock::readyRead()
********************************************************************************************************* 
** \brief CSysOutDock::readyRead 
** \details 
** \return void 
** \author gw
** \date 2015��11��30�� 
** \note 
********************************************************************************************************/
void CSysOutDock::readyRead()
{
	QTcpSocket *local = (QTcpSocket *)(sender());

	QByteArray qba = local->readAll(); //��ȡ  

	int test = qba.count();

	char *pTmpT = (char *)qba.toStdString().c_str();

	QByteArray arr;
	SYSTEMOUTINFO* m = reinterpret_cast<SYSTEMOUTINFO*>(qba.data());

	SYSTEMOUTINFO *pTmp = (SYSTEMOUTINFO *)(char *)qba.toStdString().c_str();

	qDebug() << qba;

	QString ss = QVariant(qba).toString();

	QString strTmp = qba.data();

	QVector<QString> vec;

	vec.push_back(strTmp);

	m_logModel->AddItem(m->type, m->msg, QColor());

	//m_logModel->AddItem(vec);
}