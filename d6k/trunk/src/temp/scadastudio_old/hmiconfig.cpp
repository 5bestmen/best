/*! @file
<PRE>
********************************************************************************
模块名       :	组态框架
文件名       :	hmiconfi.cpp
文件实现功能 :  组态框架
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
*  @date    2015.11.13
*/

#include <QMenuBar>
#include <QToolBar>
#include "hmiconfig.h"
#include "splitter.h"
#include "leftdock.h"
#include "rightdock.h"
#include "sysoutdock.h"
#include "tabwidget.h"
#include "treewidget.h"
#include "quoteItem.h"
#include "fesmodel.h"
#include "equipmentmodel.h"
#include "graphmodel.h"
#include "reportmodel.h"
#include "usermodel.h"
#include "sysmodel.h"
#include "nodeparamodel.h"
#include "seqscriptmodel.h"

/*! \fn CHMIConfig::CHMIConfig(QWidget *parent, Qt::WFlags flags)
********************************************************************************************************* 
** \brief CHMIConfig::CHMIConfig 
** \details 组态构造函数
** \param parent 
** \param flags 
** \return  
** \author gw
** \date 2015年11月13日 
** \note 
********************************************************************************************************/
CHMIConfig::CHMIConfig(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{	
	CreateMenus();

	CreateToolBar();

	Init();

	CreateLeftTree();

	CreateDock();

	CreateModel();

	CreateConnect();
}

/*! \fn 
********************************************************************************************************* 
** \brief CHMIConfig::~CHMIConfig 
** \details 组态析构函数
** \return  
** \author gw
** \date 2015年11月13日 
** \note 
********************************************************************************************************/
CHMIConfig::~CHMIConfig()
{

}

/*! \fn 
********************************************************************************************************* 
** \brief CHMIConfig::CreateMenus 
** \details 创建菜单
** \return void 
** \author gw
** \date 2015年11月13日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateMenus()
{
	m_pFileMenu = menuBar()->addMenu(QStringLiteral("&文件"));

	m_pSaveAct = new QAction(QIcon(":images//save.png"), tr("&Save"), this);
	m_pSaveAct->setShortcuts(QKeySequence::Save);
	m_pSaveAct->setStatusTip(tr("Save the document to disk"));
	connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(Save()));

	m_pFileMenu->addAction(m_pSaveAct);

	m_pDatabaseMenu = menuBar()->addMenu(QStringLiteral("&数据库"));
}

/*! \fn void CHMIConfig::CreateToolBar()
********************************************************************************************************* 
** \brief CHMIConfig::CreateToolBar 
** \details 创建工具栏
** \return void 
** \author gw
** \date 2015年11月13日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateToolBar()
{
	m_pFileToolBar = addToolBar(tr("File"));

	m_pFileToolBar->addAction(m_pSaveAct);
}

/*! \fn void CHMIConfig::Init()
********************************************************************************************************* 
** \brief CHMIConfig::Init 
** \details 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CHMIConfig::Init()
{
	m_pSplitter = new CSplitter(this);

	//中心窗口分离器
	setCentralWidget(m_pSplitter);

	m_pTabWidget = new CTabWidget(this);

	m_pSplitter->addWidget(m_pTabWidget);
}

/*! \fn void CHMIConfig::CreateDock()
********************************************************************************************************* 
** \brief CHMIConfig::CreateDock 
** \details 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateDock()
{
	//左边树形dock
	m_pLeftTreeDock = new CLeftDock(this);
	m_pLeftTreeDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_pLeftTreeDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	m_pLeftTreeDock->setWidget(m_pTreeWidget);
	addDockWidget(Qt::LeftDockWidgetArea, m_pLeftTreeDock);

	//
	m_pRightDock = new CRightDock(this);
	m_pRightDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_pRightDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	addDockWidget(Qt::RightDockWidgetArea, m_pRightDock);

	//
	m_pSysOutDock = new CSysOutDock(this);
		
	addDockWidget(Qt::BottomDockWidgetArea, m_pSysOutDock);
}

/*! \fn void CHMIConfig::CreateLeftTree()
********************************************************************************************************* 
** \brief CHMIConfig::CreateLeftTree 
** \details 创建左边树
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateLeftTree()
{
	m_pTreeWidget = new CTreeWidget(this);

	m_pTreeModel = new QStandardItemModel(this);

	m_pTreeWidget->setModel(m_pTreeModel);

	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("组态配置"));

	m_pTopProjectItem = new CQuoteItem(QStringLiteral("工程管理"));

	m_pTopProjectItem->setIcon(QIcon(":images//project.png"));

	m_pTreeModel->appendRow(m_pTopProjectItem);
}

/*! \fn void CHMIConfig::CreateModel()
********************************************************************************************************* 
** \brief CHMIConfig::CreateModel 
** \details 创建model
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateModel()
{
	//前置
	m_pFesModel = new CFesModel(this, m_pTopProjectItem);

	//对象
	m_pEquiModel = new CEquipmentModel(this, m_pTopProjectItem);

	m_pGraphModel = new CGraphModel(this, m_pTopProjectItem);

	m_pReportModel = new CReportModel(this, m_pTopProjectItem);

	m_pUserModel = new CUserModel(this, m_pTopProjectItem);

	m_pSysModel = new CSysModel(this, m_pTopProjectItem);

	m_pNodeParaModel = new CNodeParaModel(this, m_pTopProjectItem);

	m_pSeqScriptModel = new CSeqscriptModel(this, m_pTopProjectItem);
}

/*! \fn CTreeWidget *CHMIConfig::GetLeftTree()
********************************************************************************************************* 
** \brief CHMIConfig::GetLeftTree 
** \details 
** \return CTreeWidget * 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTreeWidget *CHMIConfig::GetLeftTree()
{
	return m_pTreeWidget;
}

/*! \fn QStandardItemModel *CHMIConfig::GetLeftTreeModel()
********************************************************************************************************* 
** \brief CHMIConfig::GetLeftTreeModel 
** \details 
** \return QStandardItemModel * 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
QStandardItemModel *CHMIConfig::GetLeftTreeModel()
{
	return m_pTreeModel;
}

/*! \fn CTabWidget *CHMIConfig::GetTabWidget()
********************************************************************************************************* 
** \brief CHMIConfig::GetTabWidget 
** \details 
** \return CTabWidget * 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTabWidget *CHMIConfig::GetTabWidget()
{
	return m_pTabWidget;
}

/*! \fn void CHMIConfig::CreateConnect()
********************************************************************************************************* 
** \brief CHMIConfig::CreateConnect 
** \details 创建信号槽连接
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CHMIConfig::CreateConnect()
{
	//前置模块
	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pFesModel, SLOT(GetFesType(int,QModelIndex &,int)));

	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pEquiModel, SLOT(GetFesType(int,QModelIndex &,int)));

	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pGraphModel, SLOT(GetFesType(int,QModelIndex &,int)));

	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pReportModel, SLOT(GetFesType(int,QModelIndex &,int)));

	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pUserModel, SLOT(GetFesType(int,QModelIndex &,int)));

	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pSysModel, SLOT(GetFesType(int,QModelIndex &,int)));

	//节点
	connect(m_pTreeWidget, SIGNAL(SendFesType(int,QModelIndex &,int)), m_pNodeParaModel, SLOT(GetFesType(int,QModelIndex &,int)));
}

#include "base.h"
#include "devicedialog.h"

void CHMIConfig::Save()
{
	if(m_pTabWidget == NULL)
	{
		return;
	}

	IBase *pBase = dynamic_cast<IBase *>(m_pTabWidget->currentWidget());

	//CDeviceDialog* pDialog = (CDeviceDialog*)(m_pTabWidget->currentWidget());

	//IBase *pBase = (IBase *)pDialog;

	pBase->Save();
}