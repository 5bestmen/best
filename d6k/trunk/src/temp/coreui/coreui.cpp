#include <QMenuBar>
#include <QToolBar>
#include "icore/coreui.h"
#include "icore/treewidget.h"
#include "icore/quoteItem.h"
#include "leftdock.h"
#include "rightdock.h"
#include "sysoutdock.h"
#include "attriwindow/attriwindow.h"
#include "icore/tabwidget.h"
#include "icore/define.h"
#include "fesmodel.h"

ICoreui *GetGlobalCoreUI()
{
	return new CCoreui();
}


CCoreui::CCoreui()
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

CCoreui::~CCoreui()
{
	//delete m_pTopProjectItem;

	this->deleteLater();
}

/*! \fn void CCoreui::CreateLeftTree()
*********************************************************************************************************
** \brief CCoreui::CreateLeftTree
** \details ���������
** \return void
** \author gw
** \date 2015��11��16��
** \note
********************************************************************************************************/
void CCoreui::CreateLeftTree()
{
	m_pTreeWidget = new CTreeWidget(this);

	m_pTreeModel = new QStandardItemModel(this);

	m_pTreeWidget->setModel(m_pTreeModel);

	m_pTreeModel->setHorizontalHeaderLabels(QStringList() << QStringLiteral("��̬����"));

	m_pTopProjectItem = new CQuoteItem(QStringLiteral("���̹���"));

	m_pTopProjectItem->setData(290, Qt::UserRole);

	m_pTopProjectItem->setIcon(QIcon(":images//project.png"));

	m_pTreeModel->appendRow(m_pTopProjectItem);
}

/*! \fn void CCoreui::CreateDock()
*********************************************************************************************************
** \brief CScadastudio::CreateDock
** \details
** \return void
** \author gw
** \date 2015��11��16��
** \note
********************************************************************************************************/
void CCoreui::CreateDock()
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
	m_pSysOutDock->setFeatures(QDockWidget::DockWidgetMovable);
	m_pSysOutDock->setAllowedAreas(Qt::AllDockWidgetAreas);


	addDockWidget(Qt::BottomDockWidgetArea, m_pSysOutDock);
}

/*! \fn void  CCoreui::CreateRightAttri()
*********************************************************************************************************
** \brief CCoreui::CreateRightAttri
** \details ����������
** \return void
** \author gw
** \date 2015��11��29��
** \note
********************************************************************************************************/
void CCoreui::CreateRightAttri()
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
void CCoreui::CreateMenus()
{
	m_pFileMenu = menuBar()->addMenu(QStringLiteral("&�ļ�"));

	m_pSaveAct = new QAction(QIcon(":images//save.png"), tr("&Save"), this);
	m_pSaveAct->setShortcuts(QKeySequence::Save);
	m_pSaveAct->setStatusTip(tr("Save the document to disk"));
	connect(m_pSaveAct, SIGNAL(triggered()), this, SLOT(Save()));

	m_pFileMenu->addAction(m_pSaveAct);

	m_pDatabaseMenu = menuBar()->addMenu(QStringLiteral("&���ݿ�"));
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
void CCoreui::CreateToolBar()
{
	m_pFileToolBar = addToolBar(tr("File"));

	m_pFileToolBar->addAction(m_pSaveAct);

	QComboBox *pCombox = new QComboBox(this);

	pCombox->addItem("test");

	m_pFileToolBar->addWidget(pCombox);
}

/*! \fn CTreeWidget *CCoreui::GetLeftTree()
********************************************************************************************************* 
** \brief CCoreui::GetLeftTree 
** \details ���ⲿģ���ṩ������νӿ�
** \return CTreeWidget * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CTreeWidget *CCoreui::GetLeftTree()
{
	return m_pTreeWidget;
}


/*! \fn QStandardItemModel *CCoreui::GetLeftTreeModel()
********************************************************************************************************* 
** \brief CCoreui::GetLeftTreeModel 
** \details ���ⲿģ���ṩ����model�ӿ�
** \return QStandardItemModel * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
QStandardItemModel *CCoreui::GetLeftTreeModel()
{
	return m_pTreeModel;
}

/*! \fn CTabWidget *CCoreui::GetTabWidget()
********************************************************************************************************* 
** \brief CCoreui::GetTabWidget 
** \details ���ⲿģ���ṩ��tab�ӿ�
** \return CTabWidget * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CTabWidget *CCoreui::GetTabWidget()
{
	return m_pTabWidget;
}

/*! \fn CQuoteItem *CCoreui::GetTopItem()
********************************************************************************************************* 
** \brief CCoreui::GetTopItem 
** \details ���ⲿģ���ṩ���ζ���item�ӿ�
** \return CQuoteItem * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CQuoteItem *CCoreui::GetTopItem()
{
	return m_pTopProjectItem;
}

/*! \fn CAtrribute *CCoreui::GetAtrri()
********************************************************************************************************* 
** \brief CCoreui::GetAtrri 
** \details 
** \return CAtrribute * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
CAttriwindow *CCoreui::GetAtrri()
{
	return m_pAttri;
}

/*! \fn QMenuBar*CCoreui::GetMainMenuBar()
********************************************************************************************************* 
** \brief CCoreui::GetMainMenuBar 
** \details  ���˵�
** \return QMenuBar* 
** \author LiJin
** \date 2016��2��27�� 
** \note 
********************************************************************************************************/
QMenuBar*CCoreui::GetMainMenuBar()
{
	return menuBar();
}

/*! \fn QToolBar *CCoreui::CreateToolBarInModel(QString strTitle)
********************************************************************************************************* 
** \brief CCoreui::CreateToolBarInModel 
** \details 
** \param strTitle 
** \return QToolBar * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
QToolBar *CCoreui::CreateToolBarInModel(QString strTitle)
{
	return addToolBar(strTitle);
}

/*! \fn QAction *CCoreui::CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon)
********************************************************************************************************* 
** \brief CCoreui::CreateActionInModel 
** \details ��model�ﴴ��action
** \param strTitle Action��
** \param pToolBar ͼ��
** \param strIcon ������
** \return QAction * 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
QAction *CCoreui::CreateActionInModel(QString strTitle, QToolBar *pToolBar, QString strIcon)
{
	QAction *pAct = new QAction(QIcon(strIcon), strTitle, pToolBar);

	pToolBar->addAction(pAct);

	return pAct;
}

/*! \fn bool CCoreui::PutToolBarIntoMap(QString strName, QToolBar *pToolBar)
********************************************************************************************************* 
** \brief CCoreui::PutToolBarIntoMap 
** \details ����������map
** \param strName ��������
** \param pToolBar 
** \return bool 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
bool CCoreui::PutToolBarIntoMap(QString strName, QToolBar *pToolBar)
{
	if (m_toolBarMap.contains(strName))
	{
		return false;
	}

	m_toolBarMap[strName] = pToolBar;

	return true;
}

/*! \fn bool CCoreui::RemoveToolBarFromMap(QString strName)
********************************************************************************************************* 
** \brief CCoreui::RemoveToolBarFromMap 
** \details ��������map���Ƴ�
** \param strName 
** \return bool 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
bool CCoreui::RemoveToolBarFromMap(QString strName)
{
	if (m_toolBarMap.contains(strName))
	{
		m_toolBarMap[strName]->deleteLater();

		m_toolBarMap.remove(strName);

		return true;
	}

	return false;
}

/*! \fn bool CCoreui::FindToolBar(QString strName)
********************************************************************************************************* 
** \brief CCoreui::FindToolBar 
** \details ���ҹ�����
** \param strName 
** \return bool 
** \author gw
** \date 2015��12��3�� 
** \note 
********************************************************************************************************/
bool CCoreui::FindToolBar(QString strName)
{
	if (m_toolBarMap.contains(strName))
	{
		m_toolBarMap[strName]->deleteLater();

		m_toolBarMap.remove(strName);

		return true;
	}

	return false;
}

/*! \fn void CCoreui::InitFesUi()
********************************************************************************************************* 
** \brief CCoreui::InitFesUi 
** \details ��ʼ��ǰ��
** \return void 
** \author gw
** \date 2015��12��7�� 
** \note 
********************************************************************************************************/
void CCoreui::InitFesUi()
{
	//ǰ��
	CQuoteItem *pItem = new CQuoteItem(QStringLiteral("ǰ������"));
	pItem->setIcon(QIcon(FES_PNG));
	m_pTopProjectItem->appendRow(pItem);

	//ͨ��
	CQuoteItem *itemChannel = new CQuoteItem(QStringLiteral("ͨ��"));
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	//����
	CQuoteItem *itemAlarm = new CQuoteItem(QStringLiteral("����"));
	itemAlarm->setData(FES_ALARM_ITEM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);

	pItem->appendRow(itemAlarm);

	//ת����ϵ
	CQuoteItem *itemScale = new CQuoteItem(QStringLiteral("ת��"));
	itemScale->setData(FES_SCALE_ITEM, Qt::UserRole);
	itemScale->setIcon(QIcon(":images/scale.png"));
	itemScale->setEditable(false);

	pItem->appendRow(itemScale);

	//����
	CQuoteItem *itemVariableList = new CQuoteItem(QStringLiteral("����"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(":images/variablelist.png"));
	itemVariableList->setEditable(false);

	pItem->appendRow(itemVariableList);

	//ϵͳ����
	CQuoteItem *itemSysVariableList = new CQuoteItem(QStringLiteral("ϵͳ����"));
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);

	itemVariableList->appendRow(itemSysVariableList);

	//�û�����
	CQuoteItem *itemUserVariableList = new CQuoteItem(QStringLiteral("�û�����"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	//�û�root
	CQuoteItem *itemUserVariableRoot = new CQuoteItem(QStringLiteral("root"));
	itemUserVariableRoot->setData(FES_USER_VARIBALE_ROOT_ITEM, Qt::UserRole);
	//itemUserVariableRoot->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableRoot->setEditable(false);

	itemUserVariableList->appendRow(itemUserVariableRoot);

	itemVariableList->appendRow(itemUserVariableList);

	//��ʷ�洢
	CQuoteItem *itemHis = new CQuoteItem(QStringLiteral("��ʷ�洢"));
	itemHis->setData(FES_HIS_SAVE_ITEM, Qt::UserRole);
	itemHis->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	///////////////////////////////////////////////////////////////////
	CQuoteItem *itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����������ݱ���"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_REPORT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����������ݱ���"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CURVE_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ������ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_MAX_MIN_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ����Խ��ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_LIMIT_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("ģ�����ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_AI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("�������ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_DI_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHis->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	itemHisTmp = new CQuoteItem(QStringLiteral("������ۼ�ֵͳ��"));
	itemHisTmp->setData(FES_TYPE_HIS_KWH_CUM_ITEM, Qt::UserRole);
	itemHisTmp->setIcon(QIcon(HIS_SAVE_PNG));
	itemHisTmp->setEditable(false);

	itemHis->appendRow(itemHisTmp);

	///////////////////////////////////////////////////////////////////

	pItem->appendRow(itemHis);

	//�¹�׷��
	CQuoteItem *itemPdr = new CQuoteItem(QStringLiteral("�¹�׷��"));
	itemPdr->setData(FES_PDR_ITEM, Qt::UserRole);
	itemPdr->setIcon(QIcon(PDR_PNG));
	itemPdr->setEditable(false);

	pItem->appendRow(itemPdr);
}

void CCoreui::CreateModel()
{
	//ǰ��
	//m_pFesModel = new CFesModel(this, m_pTopProjectItem);
}

CRightDock *CCoreui::GetRightDock()
{
	Q_ASSERT(m_pRightDock);

	return m_pRightDock;
}