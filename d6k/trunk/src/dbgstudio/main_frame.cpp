/*! @file
<PRE>
********************************************************************************
模块名       :  主窗口模块
文件名       :  ccore.cpp
文件实现功能 :  UI管理模块 工程管理模块 模块管理模块
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
*  @date    2015.11.26
*/
#include "main_frame.h"
#include "dbgstudio/treewidget.h"
#include "dbgstudio/tabwidget.h"

#include "main_module_impl.h" 
#include "pluginloadwgt.h"
#include "modulemgr.h"
#include "outputdock.h"
#include "capturemsgwgt.h"
#include "scadalog_view.h"

#include <QtGlobal>
#include <QDockWidget>
#include <QStandardItemModel>
#include <QAction> 
#include <QMenuBar>
#include <QMessageBox>
#include <QDebug>

CMainFrm::CMainFrm( )
	:m_pTreeWidget(nullptr),
	m_pTabWidget(nullptr),
	m_pLeftTreeDock(nullptr),
	m_pPluginWgt(new CPluginLoadWgt)
{	 
// 	m_pTabWidget = new CTabWidget(this);
// 	setCentralWidget(m_pTabWidget);
//	setWindowTitle(tr("Maintenance Tool"));

	qApp->setWindowIcon(QIcon(":/icons/dbgstudio.png"));
	CreateMenus();
	CreateLeftTree();
	 
	CreateTab();

	CreateDock();

	setWindowState(Qt::WindowMaximized);
	connect(m_pPluginWgt, SIGNAL(Signal_NeedLoadInfo(QString,QString)),this,SLOT(Slot_LoadPlugin(QString,QString)));
	//m_pUi->setParent(this);	
}

CMainFrm::~CMainFrm()
{
	m_pPluginWgt->deleteLater();
	delete CCaptureMsgWgt::GetInstance();
}

CTreeWidget *CMainFrm::GetLeftTree()
{
	return m_pTreeWidget;
}

CTabWidget *CMainFrm::GetTabWidget()
{
	return m_pTabWidget;
}

void CMainFrm::CreateTab()
{
	m_pTabWidget = new CTabWidget(this);
	setCentralWidget(m_pTabWidget);
}

void CMainFrm::CreateLeftTree()
{
	m_pTreeWidget = new CTreeWidget(this);

 	m_pTreeModel = new QStandardItemModel(this);

  	m_pTreeWidget->setModel(m_pTreeModel);
  
  	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << tr("List"));
 
	m_pTopTreeItem = new QStandardItem(tr("Project Manager"));
	m_pTopTreeItem->setIcon(QIcon(":/icons/gongcheng.png"));
 
// 	m_pTopProjectItem->setData(290, Qt::UserRole);
// 
// 	m_pTopProjectItem->setIcon(QIcon(":images//project.png"));
  
	m_pTreeModel->appendRow(m_pTopTreeItem);

}
/*! \fn void CMainFrm::CreateDock()
*********************************************************************************************************
** \brief CMainFrm::CreateDock
** \details
** \return void
** \author gw
** \date 2015年11月16日
** \note
********************************************************************************************************/
void CMainFrm::CreateDock()
{
	//左边树形dock
	m_pLeftTreeDock = new QDockWidget(this);
	m_pLeftTreeDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_pLeftTreeDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	Q_ASSERT(m_pTreeWidget);
	if (m_pTreeWidget)
	{
		m_pLeftTreeDock->setWidget(m_pTreeWidget);
	}
	addDockWidget(Qt::LeftDockWidgetArea, m_pLeftTreeDock);

	//
	m_pOutDock = new COutputDock(this);
	m_pOutDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_pOutDock->setAllowedAreas(Qt::AllDockWidgetAreas);


	addDockWidget(Qt::BottomDockWidgetArea, m_pOutDock);
}

QTableView *CMainFrm::GetOutputTableView()
{
	if (m_pOutDock != NULL)
	{
		return m_pOutDock->GetTableView();
	}
	else
	{
		return NULL;
	}
}

void  CMainFrm::CreateMenus()
{
  	QMenu *pFileMenu = menuBar()->addMenu(tr("File(&F)"));
	QAction *pOpenAct = new QAction(tr("Open(&O)"), this);
	pOpenAct->setIcon(QIcon(":/icons/Open.png"));
	pFileMenu->addAction(pOpenAct);
	connect(pOpenAct, SIGNAL(triggered()), this, SLOT(Slot_ClickOpenDllWgt()));

	QAction *pAboutAct = new QAction(tr("About(&A)"),this);
	pFileMenu->addAction(pAboutAct);
	connect(pAboutAct, SIGNAL(triggered()), this, SLOT(Slot_ClickAboutThisTool()));

	// 日志
//	QMenu *pLogMenu = menuBar()->addMenu(tr("ViewLog(&M)"));
// 	QAction *pLogAct = new QAction(tr("ViewLog(&V)"), this);
// 	pLogAct->setIcon(QIcon(":/icons/viewlog.png"));
// 	pLogMenu->addAction(pLogAct);
// 	connect(pLogAct, SIGNAL(triggered()), this, SLOT(Slot_ShowMsgMonitorWgt()));

	//视图
	QMenu *pViewMenu = menuBar()->addMenu(tr("View(&M)"));
	
	// 日志
	QAction *pLogAct = new QAction(tr("ViewLog(&V)"), this);
	pLogAct->setIcon(QIcon(":/icons/viewlog.png"));
	pViewMenu->addAction(pLogAct);
	connect(pLogAct, SIGNAL(triggered()), this, SLOT(SlotViewLog()));

	QAction *pMonitorAct = new QAction(tr("PacketTrace(&P)"), this);
	pMonitorAct->setIcon(QIcon(":/icons/jianshi.png"));
	pViewMenu->addAction(pMonitorAct);
	connect(pMonitorAct, SIGNAL(triggered()), this, SLOT(Slot_ShowMsgMonitorWgt()));
	
	
}


void CMainFrm::Slot_ClickAboutThisTool()
{
	QMessageBox::information(this, tr("About"), tr("DBGStuidio, V1.0"));
}


//打开插件
void CMainFrm::Slot_ClickOpenDllWgt()
{
	//加载当前的插件
	m_pPluginWgt->show();
}

//加载插件
void CMainFrm::Slot_LoadPlugin(const QString &strPluginName, const QString &strDeviceName)
{
	emit Signal_NeedLoadInfo(strPluginName,strDeviceName);
}
//查看实时报文
void CMainFrm::Slot_ShowMsgMonitorWgt()
{
	//CCaptureMsgWgt::GetInstance()->show();
	m_pTabWidget->AddTab(CCaptureMsgWgt::GetInstance(), tr("Monitor"), "Monitor");
	m_pTabWidget->setCurrentWidget(CCaptureMsgWgt::GetInstance());
}

void CMainFrm::SlotViewLog()
{
	m_pTabWidget->AddTab(CScadaLogView::GetInstance(), tr("Log"), "Log");
	m_pTabWidget->setCurrentWidget(CScadaLogView::GetInstance());

}
