/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出dock窗口
文件名       :  sysoutdock.cpp
文件实现功能 :  系统输出dock窗口
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
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CSysOutDock::CSysOutDock(QWidget *parent)
	: QDockWidget(parent)
{	
	this->setWindowTitle(QStringLiteral("系统输出"));
	
	m_messsageTab = new QTabWidget(this);

	m_messsageTab->setTabPosition(QTabWidget::South);

	setWidget(m_messsageTab);

	m_tcpServer = new QTcpServer(this);

	m_tcpServer->listen(QHostAddress::Any, 19999); //监听任何连上19999端口的ip  

	connect(m_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样，其实你可以随便取。

	createCommandView();

	createLogCommandView();
}

/*! \fn CSysOutDock::~CSysOutDock()
********************************************************************************************************* 
** \brief CSysOutDock::~CSysOutDock 
** \details 
** \return  
** \author gw
** \date 2015年11月16日 
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
** \date 2015年11月30日 
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
** \date 2015年11月30日 
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
** \date 2015年11月30日 
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
** \date 2015年11月30日 
** \note 
********************************************************************************************************/
void CSysOutDock::readyRead()
{
	QTcpSocket *local = (QTcpSocket *)(sender());

	QByteArray qba = local->readAll(); //读取  

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