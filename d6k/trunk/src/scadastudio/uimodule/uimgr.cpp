#include <QMenuBar>
#include <QToolBar>
#include "scadastudio/uimgr.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/quoteItem.h"
#include "leftdock.h"
#include "rightdock.h"
#include "sysoutdock.h"
#include "attriwindow/attriwindow.h"
#include "scadastudio/tabwidget.h"
#include "scadastudiodefine.h"
#include "fesmodule.h"

IMainWindow *GetGlobalCoreUI()
{
	return new CUIMgr();
}


CUIMgr::CUIMgr()
	:m_pTopProjectItem(nullptr), m_pTreeModel(nullptr), m_pTreeWidget(nullptr), m_pTabWidget(nullptr), m_pAttri(nullptr), m_pExtendServiceTopItem(nullptr)
{
	m_pTabWidget = new CTabWidget(this);

	setCentralWidget(m_pTabWidget);

	CreateMenus();

	CreateToolBar();
	
	CreateLeftTree();

	CreateRightAttri();

	CreateDock();

	CreateModel();

	setWindowState(Qt::WindowMaximized);

	setWindowIcon(QIcon(GRAPH_PNG));
}

CUIMgr::~CUIMgr()
{
	//delete m_pTopProjectItem;

	this->deleteLater();
}

/*! \fn void CUIMgr::CreateLeftTree()
*********************************************************************************************************
** \brief CUIMgr::CreateLeftTree
** \details 创建左边树
** \return void
** \author gw
** \date 2015年11月16日
** \note
********************************************************************************************************/
void CUIMgr::CreateLeftTree()
{
	m_pTreeWidget = new CTreeWidget(this);

	m_pTreeModel = new QStandardItemModel(this);

	m_pTreeWidget->setModel(m_pTreeModel);

	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << tr("Config"));

	m_pTopProjectItem = new CQuoteItem(QObject::tr("ProjectManagement"));

	m_pTopProjectItem->setData(290, Qt::UserRole);

	m_pTopProjectItem->setIcon(QIcon(":images//project.png"));

	m_pTreeModel->appendRow(m_pTopProjectItem);
}

/*! \fn void CUIMgr::CreateDock()
*********************************************************************************************************
** \brief CScadastudio::CreateDock
** \details
** \return void
** \author gw
** \date 2015年11月16日
** \note
********************************************************************************************************/
void CUIMgr::CreateDock()
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

	m_pRightDock->setWidget((QWidget *)m_pAttri);

	addDockWidget(Qt::RightDockWidgetArea, m_pRightDock);

	//
	m_pSysOutDock = new CSysOutDock(this);
	m_pSysOutDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable);
	m_pSysOutDock->setAllowedAreas(Qt::AllDockWidgetAreas);


	addDockWidget(Qt::BottomDockWidgetArea, m_pSysOutDock);
}

/*! \fn void  CUIMgr::CreateRightAttri()
*********************************************************************************************************
** \brief CUIMgr::CreateRightAttri
** \details 创建属性栏
** \return void
** \author gw
** \date 2015年11月29日
** \note
********************************************************************************************************/
void CUIMgr::CreateRightAttri()
{
	m_pAttri = new CAttriwindow(this);
}

/*! \fn void CScadastudio::CreateMenus()
*********************************************************************************************************
** \brief CScadastudio::CreateMenus
** \details 创建菜单
** \return void
** \author gw
** \date 2015年11月13日
** \note
********************************************************************************************************/
void CUIMgr::CreateMenus()
{
	//m_pFileMenu = menuBar()->addMenu(QStringLiteral("&文件"));

	//m_pSaveAct = new QAction(QIcon(":images//save.png"), tr("&Save"), this);
	//m_pSaveAct->setShortcuts(QKeySequence::Save);
	//m_pSaveAct->setStatusTip(tr("Save the document to disk"));
	//connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(Save()));

	//m_pFileMenu->addAction(m_pSaveAct);

	//m_pDatabaseMenu = menuBar()->addMenu(QStringLiteral("&数据库"));
}

/*! \fn void CScadastudio::CreateToolBar()
*********************************************************************************************************
** \brief CScadastudio::CreateToolBar
** \details 创建工具栏
** \return void
** \author gw
** \date 2015年11月13日
** \note
********************************************************************************************************/
#include <QComboBox>
void CUIMgr::CreateToolBar()
{
	//m_pFileToolBar = addToolBar(tr("File"));

	//m_pFileToolBar->addAction(m_pSaveAct);

	//QComboBox *pCombox = new QComboBox((QWidget*)this);

	//pCombox->addItem("test");

	//m_pFileToolBar->addWidget(pCombox);
}

/*! \fn CTreeWidget *CUIMgr::GetLeftTree()
********************************************************************************************************* 
** \brief CUIMgr::GetLeftTree 
** \details 给外部模块提供左边树形接口
** \return CTreeWidget * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
CTreeWidget *CUIMgr::GetLeftTree()
{
	return m_pTreeWidget;
}


/*! \fn QStandardItemModel *CUIMgr::GetLeftTreeModel()
********************************************************************************************************* 
** \brief CUIMgr::GetLeftTreeModel 
** \details 给外部模块提供树形model接口
** \return QStandardItemModel * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
QStandardItemModel *CUIMgr::GetLeftTreeModel()
{
	return m_pTreeModel;
}

/*! \fn CTabWidget *CUIMgr::GetTabWidget()
********************************************************************************************************* 
** \brief CUIMgr::GetTabWidget 
** \details 给外部模块提供树tab接口
** \return CTabWidget * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
CTabWidget *CUIMgr::GetTabWidget()
{
	return m_pTabWidget;
}

/*! \fn CQuoteItem *CUIMgr::GetTopItem()
********************************************************************************************************* 
** \brief CUIMgr::GetTopItem 
** \details 给外部模块提供树形顶级item接口
** \return CQuoteItem * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
CQuoteItem *CUIMgr::GetTopItem()
{
	return m_pTopProjectItem;
}

bool CUIMgr::SetExtendServiceTopItem(CQuoteItem *pItem)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}
	
	m_pExtendServiceTopItem = pItem;

	return true;
}

CQuoteItem * CUIMgr::GetExtendServiceTopItem()
{
	return m_pExtendServiceTopItem;
}

/*! \fn CAtrribute *CUIMgr::GetAtrri()
********************************************************************************************************* 
** \brief CUIMgr::GetAtrri 
** \details 
** \return CAtrribute * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
CAttriwindow *CUIMgr::GetAtrri()
{
	return m_pAttri;
}

/*! \fn QMenuBar*CUIMgr::GetMainMenuBar()
********************************************************************************************************* 
** \brief CUIMgr::GetMainMenuBar 
** \details  主菜单
** \return QMenuBar* 
** \author LiJin
** \date 2016年2月27日 
** \note 
********************************************************************************************************/
QMenuBar*CUIMgr::GetMainMenuBar()
{
	return menuBar();
}

/*! \fn QToolBar *CUIMgr::CreateToolBarInModel(QString strTitle)
********************************************************************************************************* 
** \brief CUIMgr::CreateToolBarInModel 
** \details 
** \param strTitle 
** \return QToolBar * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
QToolBar *CUIMgr::CreateToolBarInModel(QString strTitle)
{
	return addToolBar(strTitle);
}

/*! \fn QAction *CUIMgr::CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon)
********************************************************************************************************* 
** \brief CUIMgr::CreateActionInModel 
** \details 在model里创建action
** \param strTitle Action名
** \param pToolBar 图标
** \param strIcon 工具栏
** \return QAction * 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
QAction *CUIMgr::CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon)
{
	QAction *pAct = new QAction(QIcon(strIcon), strTitle, pToolBar);

	pToolBar->addAction(pAct);

	return pAct;
}

/*! \fn bool CUIMgr::PutToolBarIntoMap(QString strName, QToolBar *pToolBar)
********************************************************************************************************* 
** \brief CUIMgr::PutToolBarIntoMap 
** \details 工具栏加入map
** \param strName 工具栏名
** \param pToolBar 
** \return bool 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
bool CUIMgr::PutToolBarIntoMap(QString strName, QToolBar *pToolBar)
{
	if (m_toolBarMap.contains(strName))
	{
		return false;
	}

	m_toolBarMap[strName] = pToolBar;

	return true;
}

/*! \fn bool CUIMgr::RemoveToolBarFromMap(QString strName)
********************************************************************************************************* 
** \brief CUIMgr::RemoveToolBarFromMap 
** \details 工具栏从map里移除
** \param strName 
** \return bool 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
bool CUIMgr::RemoveToolBarFromMap(QString strName)
{
	if (m_toolBarMap.contains(strName))
	{
		m_toolBarMap[strName]->deleteLater();

		m_toolBarMap.remove(strName);

		return true;
	}

	return false;
}

/*! \fn bool CUIMgr::FindToolBar(QString strName)
********************************************************************************************************* 
** \brief CUIMgr::FindToolBar 
** \details 查找工具栏
** \param strName 
** \return bool 
** \author gw
** \date 2015年12月3日 
** \note 
********************************************************************************************************/
bool CUIMgr::FindToolBar(QString strName)
{
	if (m_toolBarMap.contains(strName))
	{
		m_toolBarMap[strName]->deleteLater();

		m_toolBarMap.remove(strName);

		return true;
	}

	return false;
}

/*! \fn void CUIMgr::InitFesUi()
********************************************************************************************************* 
** \brief CUIMgr::InitFesUi 
** \details 初始化前置
** \return void 
** \author gw
** \date 2015年12月7日 
** \note 
********************************************************************************************************/
void CUIMgr::InitFesUi()
{
	//前置
	CQuoteItem *pItem = new CQuoteItem(tr("fes"));
	pItem->setData(FES_CONFIG, Qt::UserRole);
	pItem->setIcon(QIcon(FES_PNG));
	m_pTopProjectItem->appendRow(pItem);

	//通道
	CQuoteItem *itemChannel = new CQuoteItem(tr("channel"));
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	//报警
	CQuoteItem *itemAlarm = new CQuoteItem(tr("alarm"));
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	pItem->appendRow(itemAlarm);

	//转换关系
	CQuoteItem *itemScale = new CQuoteItem(tr("scale"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(":images/scale.png"));
	itemScale->setEditable(false);

	pItem->appendRow(itemScale);

	//变量
	CQuoteItem *itemVariableList = new CQuoteItem(tr("virable"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemVariableList->setEditable(false);

	pItem->appendRow(itemVariableList);

	//系统变量
	CQuoteItem *itemSysVariableList = new CQuoteItem(tr("system virable"));
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);

	itemVariableList->appendRow(itemSysVariableList);

	//用户变量
	CQuoteItem *itemUserVariableList = new CQuoteItem(tr("user vairable"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	//用户root
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(tr("root"));
	itemUserVariableRoot->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
	//itemUserVariableRoot->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableRoot->setEditable(false);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//历史存储
	CQuoteItem *itemHis = new CQuoteItem(tr("historicaldatastorage"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	//模拟量报表数据保存
	CQuoteItem *itemHisTmp = new CQuoteItem(tr("analogreportstorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量曲线数据保存
	itemHisTmp = new CQuoteItem(tr("analogcurvestorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量最值统计
	itemHisTmp = new CQuoteItem(tr("analogmaxminstatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量越限值统计
	itemHisTmp = new CQuoteItem(tr("analoglimitstatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//模拟量累计值统计
	itemHisTmp = new CQuoteItem(tr("analogcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//开关量累计值统计
	itemHisTmp = new CQuoteItem(tr("digitalcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//电度量累计值统计
	itemHisTmp = new CQuoteItem(tr("kwhcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////

	pItem->appendRow(itemHis);

	//事故追忆
	CQuoteItem *itemPdr = new CQuoteItem(tr("pdr"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(PDR_PNG));
	itemPdr->setEditable(false);

	pItem->appendRow(itemPdr);
}

void CUIMgr::CreateModel()
{
	//前置
	//m_pFesModel = new CFesModel(this, m_pTopProjectItem);
}

CRightDock *CUIMgr::GetRightDock()
{
	Q_ASSERT(m_pRightDock);

	return m_pRightDock;
}

bool CUIMgr::AddSysOutItem(MSG_LOG & msg, QColor color)
{
	m_pSysOutDock->AddItem(msg, color);

	return true;
}
