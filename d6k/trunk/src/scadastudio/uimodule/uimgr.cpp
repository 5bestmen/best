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
** \details ���������
** \return void
** \author gw
** \date 2015��11��16��
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
** \date 2015��11��16��
** \note
********************************************************************************************************/
void CUIMgr::CreateDock()
{	
	//�������dock
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
** \details ����������
** \return void
** \author gw
** \date 2015��11��29��
** \note
********************************************************************************************************/
void CUIMgr::CreateRightAttri()
{
	m_pAttri = new CAttriwindow(this);
}

/*! \fn void CScadastudio::CreateMenus()
*********************************************************************************************************
** \brief CScadastudio::CreateMenus
** \details �����˵�
** \return void
** \author gw
** \date 2015��11��13��
** \note
********************************************************************************************************/
void CUIMgr::CreateMenus()
{
	//m_pFileMenu = menuBar()->addMenu(QStringLiteral("&�ļ�"));

	//m_pSaveAct = new QAction(QIcon(":images//save.png"), tr("&Save"), this);
	//m_pSaveAct->setShortcuts(QKeySequence::Save);
	//m_pSaveAct->setStatusTip(tr("Save the document to disk"));
	//connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(Save()));

	//m_pFileMenu->addAction(m_pSaveAct);

	//m_pDatabaseMenu = menuBar()->addMenu(QStringLiteral("&���ݿ�"));
}

/*! \fn void CScadastudio::CreateToolBar()
*********************************************************************************************************
** \brief CScadastudio::CreateToolBar
** \details ����������
** \return void
** \author gw
** \date 2015��11��13��
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
** \details ���ⲿģ���ṩ������νӿ�
** \return CTreeWidget * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CTreeWidget *CUIMgr::GetLeftTree()
{
	return m_pTreeWidget;
}


/*! \fn QStandardItemModel *CUIMgr::GetLeftTreeModel()
********************************************************************************************************* 
** \brief CUIMgr::GetLeftTreeModel 
** \details ���ⲿģ���ṩ����model�ӿ�
** \return QStandardItemModel * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
QStandardItemModel *CUIMgr::GetLeftTreeModel()
{
	return m_pTreeModel;
}

/*! \fn CTabWidget *CUIMgr::GetTabWidget()
********************************************************************************************************* 
** \brief CUIMgr::GetTabWidget 
** \details ���ⲿģ���ṩ��tab�ӿ�
** \return CTabWidget * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CTabWidget *CUIMgr::GetTabWidget()
{
	return m_pTabWidget;
}

/*! \fn CQuoteItem *CUIMgr::GetTopItem()
********************************************************************************************************* 
** \brief CUIMgr::GetTopItem 
** \details ���ⲿģ���ṩ���ζ���item�ӿ�
** \return CQuoteItem * 
** \author gw
** \date 2015��12��3�� 
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
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CAttriwindow *CUIMgr::GetAtrri()
{
	return m_pAttri;
}

/*! \fn QMenuBar*CUIMgr::GetMainMenuBar()
********************************************************************************************************* 
** \brief CUIMgr::GetMainMenuBar 
** \details  ���˵�
** \return QMenuBar* 
** \author LiJin
** \date 2016��2��27�� 
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
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
QToolBar *CUIMgr::CreateToolBarInModel(QString strTitle)
{
	return addToolBar(strTitle);
}

/*! \fn QAction *CUIMgr::CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon)
********************************************************************************************************* 
** \brief CUIMgr::CreateActionInModel 
** \details ��model�ﴴ��action
** \param strTitle Action��
** \param pToolBar ͼ��
** \param strIcon ������
** \return QAction * 
** \author gw
** \date 2015��12��3�� 
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
** \details ����������map
** \param strName ��������
** \param pToolBar 
** \return bool 
** \author gw
** \date 2015��12��3�� 
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
** \details ��������map���Ƴ�
** \param strName 
** \return bool 
** \author gw
** \date 2015��12��3�� 
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
** \details ���ҹ�����
** \param strName 
** \return bool 
** \author gw
** \date 2015��12��3�� 
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
** \details ��ʼ��ǰ��
** \return void 
** \author gw
** \date 2015��12��7�� 
** \note 
********************************************************************************************************/
void CUIMgr::InitFesUi()
{
	//ǰ��
	CQuoteItem *pItem = new CQuoteItem(tr("fes"));
	pItem->setData(FES_CONFIG, Qt::UserRole);
	pItem->setIcon(QIcon(FES_PNG));
	m_pTopProjectItem->appendRow(pItem);

	//ͨ��
	CQuoteItem *itemChannel = new CQuoteItem(tr("channel"));
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	//����
	CQuoteItem *itemAlarm = new CQuoteItem(tr("alarm"));
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	pItem->appendRow(itemAlarm);

	//ת����ϵ
	CQuoteItem *itemScale = new CQuoteItem(tr("scale"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(":images/scale.png"));
	itemScale->setEditable(false);

	pItem->appendRow(itemScale);

	//����
	CQuoteItem *itemVariableList = new CQuoteItem(tr("virable"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemVariableList->setEditable(false);

	pItem->appendRow(itemVariableList);

	//ϵͳ����
	CQuoteItem *itemSysVariableList = new CQuoteItem(tr("system virable"));
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);

	itemVariableList->appendRow(itemSysVariableList);

	//�û�����
	CQuoteItem *itemUserVariableList = new CQuoteItem(tr("user vairable"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	//�û�root
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(tr("root"));
	itemUserVariableRoot->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
	//itemUserVariableRoot->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableRoot->setEditable(false);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//��ʷ�洢
	CQuoteItem *itemHis = new CQuoteItem(tr("historicaldatastorage"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	//ģ�����������ݱ���
	CQuoteItem *itemHisTmp = new CQuoteItem(tr("analogreportstorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//ģ�����������ݱ���
	itemHisTmp = new CQuoteItem(tr("analogcurvestorage"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//ģ������ֵͳ��
	itemHisTmp = new CQuoteItem(tr("analogmaxminstatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//ģ����Խ��ֵͳ��
	itemHisTmp = new CQuoteItem(tr("analoglimitstatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//ģ�����ۼ�ֵͳ��
	itemHisTmp = new CQuoteItem(tr("analogcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//�������ۼ�ֵͳ��
	itemHisTmp = new CQuoteItem(tr("digitalcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	//������ۼ�ֵͳ��
	itemHisTmp = new CQuoteItem(tr("kwhcumulativestatistics"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////

	pItem->appendRow(itemHis);

	//�¹�׷��
	CQuoteItem *itemPdr = new CQuoteItem(tr("pdr"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(PDR_PNG));
	itemPdr->setEditable(false);

	pItem->appendRow(itemPdr);
}

void CUIMgr::CreateModel()
{
	//ǰ��
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
