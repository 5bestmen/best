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

#include "sysoutdock.h"
#include "systemoutputview.h"
#include "systemoutputmodel.h"
#include "scadastudiodefine.h"
#include "logdefine.h"

#include <qscrollbar.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTabWidget>
#include <QHeaderView>
#include <QMenu>
#include <QClipboard>

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
	
	setFeatures(DockWidgetFeature::AllDockWidgetFeatures);

	m_pMessageTab = new QTabWidget(this);

	m_pMessageTab->setTabPosition(QTabWidget::South);

	CreateLogCommandView();

	setWidget(m_pMessageTab);

	m_pTcpServer = new QTcpServer(this);

	//!todo 此处需要注意，如果程序多开的话，可能会导致监听失败  LiJin 2017.6.14
	bool bRet = m_pTcpServer->listen(QHostAddress::Any, 19999); //监听任何连上19999端口的ip  
	Q_ASSERT(bRet);

	connect(m_pTcpServer, SIGNAL(newConnection()), this, SLOT(newConnection())); //新连接信号触发，调用newConnect()槽函数，这个跟信号函数一样，其实你可以随便取。

	//createCommandView();
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

void CSysOutDock::AddItem(MSG_LOG &log, QColor &color /*= QColor()*/)
{
	m_pLogModel->AddItem(log, color);
}

void CSysOutDock::resizeEvent(QResizeEvent *event)
{
	m_pLogView->setColumnWidth(0, m_pLogView->horizontalHeader()->width() * 3 / 30);
	m_pLogView->setColumnWidth(1, m_pLogView->horizontalHeader()->width() * 3 / 30);
	m_pLogView->setColumnWidth(2, m_pLogView->horizontalHeader()->width() * 15 / 30);
	m_pLogView->setColumnWidth(3, m_pLogView->horizontalHeader()->width() * 3 / 30);
	m_pLogView->setColumnWidth(4, m_pLogView->horizontalHeader()->width() * 3 / 30);
	m_pLogView->setColumnWidth(5, m_pLogView->horizontalHeader()->width() * 3 / 30);

	QDockWidget::resizeEvent(event);
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
void CSysOutDock::CreateCommandView()
{
	m_commandView = new CSystemOutputView(this);

	m_commandModel = new CSystemOutputModel(this);

	m_commandView->setModel(m_commandModel);

	m_commandView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_pMessageTab->addTab(m_commandView, QIcon(":/images/new.png"), "CommandT");
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
void CSysOutDock::CreateLogCommandView()
{
	m_pLogView = new CSystemOutputView(this);

	m_pLogModel = new CSystemOutputModel(m_pLogView);

	connect(m_pLogModel, SIGNAL(rowsInserted(const QModelIndex &,int,int)), this, SLOT(ModelRowsInserted(const QModelIndex &,int,int)));

	m_pLogView->setModel(m_pLogModel);

	m_pLogView->setSelectionBehavior(QAbstractItemView::SelectRows);

	m_pMessageTab->addTab(m_pLogView, QIcon(":/images/new.png"), tr("Log"));
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
	QTcpSocket *newsocket = m_pTcpServer->nextPendingConnection();

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

	QByteArray qba = local->read(sizeof(MSG_LOG)); //读取 

	while (qba.size() >= sizeof(MSG_LOG))
	{
		MSG_LOG* m = reinterpret_cast<MSG_LOG*>(qba.data());

		m_pLogModel->AddItem(*m, QColor());

		qba = local->read(sizeof(MSG_LOG));
	}	 
}

void CSysOutDock::ModelRowsInserted(const QModelIndex & parent, int start, int end)
{
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);
	
	if (m_bSrcollFlag)
	{
		m_pLogView->scrollToBottom();
	}
}

//右击菜单
void CSysOutDock::Slot_ContextMenuRequest(const QPoint &cPoint)
{
	Q_UNUSED(cPoint);
	QMenu *pMenu = new QMenu(m_pLogView);

	QAction *pCpyAct = new QAction(tr("Copy"), m_pLogView);
	QAction *pClear = new QAction(tr("Clear"), m_pLogView);
	QAction *pStart = new QAction(tr("Run"), m_pLogView);
	QAction *pStop = new QAction(tr("Stop"), m_pLogView);
	QAction *pExportAct = new QAction(tr("Export"), m_pLogView);

	pMenu->addAction(pCpyAct);
	pMenu->addAction(pClear);
	pMenu->addAction(pStart);
	pMenu->addAction(pStop);
	pMenu->addSeparator();
	pMenu->addAction(pExportAct);

	connect(pCpyAct, SIGNAL(triggered()), this, SLOT(Slot_CpyAction()));
	connect(pClear, SIGNAL(triggered()), this, SLOT(Slot_ClearItemAction()));
	connect(pStart, SIGNAL(triggered()), this, SLOT(Slot_StartRunAction()));
	connect(pStop, SIGNAL(triggered()), this, SLOT(Slot_StopAction()));
	connect(pExportAct, SIGNAL(triggered()), this, SLOT(Slot_ExportAction()));


	pMenu->exec(QCursor::pos());
	pMenu->deleteLater();
}
//copy
void CSysOutDock::Slot_CpyAction()
{
	QModelIndexList selected = m_pLogView->selectionModel()->selectedRows();
	//组装需要拷贝的数据
	QString strCpyData;
	//读取数据
	for each (QModelIndex mIndex in selected)
	{
//		strCpyData.append(m_pLogModel->CpyRowData(mIndex));
	}

//	QClipboard *board = QApplication::clipboard();

//	board->setText(strCpyData);
}

//clear
void CSysOutDock::Slot_ClearItemAction()
{
	m_pLogModel->ClearAll();
}
//run
void CSysOutDock::Slot_StartRunAction()
{
	//开始滚动
	m_bSrcollFlag = true;
}
//stop
void CSysOutDock::Slot_StopAction()
{
	//停止滚动
	m_bSrcollFlag = false;
}
//Export
void CSysOutDock::Slot_ExportAction()
{
	//延后开发
//	QMessageBox::warning(this, tr("warning"), tr("No Develop!"));
}
 