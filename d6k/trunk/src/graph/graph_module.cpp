#if _MSC_VER >= 1900
//#ifdef DEBUG
#include <vld.h>
//#endif // DEBUG
#endif

#include "scadastudio/uimgr.h"
#include "scadastudio/icore.h"
#include "graph_module.h"

#include "scadastudio/quoteItem.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"

#define MODULE_LIB
#include "scadastudio/module_export.h"

#include "graphview.h"
#include "graphscene.h"
#include "graphlayer_dockwidget.h"

#include "widget_factory.h"

#include "graphtabview.h"
#include "graph_container.h"
#include "graph_file.h"
#include "stl_util-inl.h"

#include "graphtoolswgt.h"
#include "../scadastudio/uimodule/rightdock.h"
#include "scadastudio/uimgr.h"
#include "attriwindow/attriwindow.h"
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "graph_folder.h"
#include "scadastudio/iprojmgr.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QDockWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QDomDocument> 
#include <QDir> 
#include <QTranslator> 
#include <QMessageBox>

//图形
#define GRAPH_ROOT_ITEM 22
#define GRAPH_GROUP_CHILD_ITEM 225
#define GRAPH_CHILD_ITEM 226
#define GRAPH_PNG ":images/graph.png"

//图形
#define GRAPH_GROUP_NAME QStringLiteral("新建组")
#define GRAPH_NAME QStringLiteral("新建")
#define GRAPH_PNG ":images/graph.png"

//组图标
#define CLOSE_GROUP_PNG ":images/close_group.png"
#define OPEN_GROUP_PNG ":images/open_group.png"
#define GROUP_NAME QStringLiteral("添加组")
//通道
#define CHANNEL_PNG ":images/channel.png"

#define NEW_TREE_ITEM  QStringLiteral("新建")
#define NEW_GROUP_TREE_ITEM  QStringLiteral("新建组")

#define DELETE_TREE_ITEM  QStringLiteral("删除")
#define COPY_TREE_ITEM    QStringLiteral("复制")
#define CUT_TREE_ITEM     QStringLiteral("剪切")
#define PASTE_TREE_ITEM   QStringLiteral("粘帖")
#define RENAME_TREE_ITEM  QStringLiteral("重命名")
#define ATTR_TREE_ITEM  QStringLiteral("属性")

#define MAX_GROUP_NUM 3
#define GROUP_MAX Qt::WhatsThisRole


//IModel *pModel = new CGraphApi();
//static CGraphApi s_Module;
//static std::shared_ptr <CGraphApi> s_ptrModule = std::make_shared <CGraphApi>();

static CGraphApi *s_ptrModule = nullptr;


/*! \fn extern "C" SCADA_EXPORT IModule *CreateModule()
********************************************************************************************************* 
** \brief CreateModule 
** \details 子模块入口函数
** \return extern " SCADA_EXPORT IModule * 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
extern "C" SCADA_EXPORT IModule *CreateModule()
{
	s_ptrModule = new CGraphApi;
	return s_ptrModule;
//	return  s_ptrModule.get();
}

/*! \fn extern "C" SCADA_EXPORT void DestroyModule()
********************************************************************************************************* 
** \brief DestroyModule 
** \details 回收资源
** \return extern " SCADA_EXPORT void 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
extern "C" SCADA_EXPORT void DestroyModule()
{
	if (s_ptrModule)
	{
		delete s_ptrModule;
		s_ptrModule = nullptr;
	}
}

CGraphApi *GetModuleApi()
{
	return s_ptrModule;
//	return  s_ptrModule.get();
}

IMainWindow *GetMainWindow()
{
//	return s_ptrModule->GetCoreUI();
	return s_ptrModule->GetMainWindow();
}

IMainModuleInterface *GetIMainInterface()
{
	return s_ptrModule->GetIMainInterface();
}

QIcon GetModuleIcon(unsigned int nIconIdx)
{
	Q_ASSERT(nIconIdx <= ICON_MAX);
	if (nIconIdx > ICON_MAX)
		return s_ptrModule->m_arrIconList[0];

	return s_ptrModule->m_arrIconList[nIconIdx];
}

CGraphApi::CGraphApi()
{
	m_pTran = std::make_shared<QTranslator>();

	QString strRunPath = qApp->applicationDirPath();

	if (m_pTran->load(strRunPath + "/locales/graph.qm"))
	{
		qApp->installTranslator(m_pTran.get());
	}
	else
	{
		qDebug() << QStringLiteral("加载graph语言文件失败！");
	}

	m_pMainModuleInterface = nullptr;
	m_pUi = nullptr;

	m_arrIconList[ICON_EYE_ON].addFile(":images/eye_on.png");
	m_arrIconList[ICON_EYE_OFF].addFile(":images/eye_off.png");
	m_arrIconList[ICON_LEFT].addFile(":images/left.png");
	m_arrIconList[ICON_LAYER].addFile(":images/layers.png");

	m_pGraphData = std::make_shared<CGraphContainer>();

	m_pGraphicsRoot = nullptr;
}

CGraphApi::~CGraphApi()
{
	
}

int CGraphApi::CallBackBeforeCloseTab(void *pData)
{
	qDebug() << "CallBackBeforeCloseTab";
	Q_UNUSED(pData);
	return 1;
}

void CGraphApi::Init(IMainModuleInterface *pCore)
{
	Q_ASSERT(pCore);
	if (pCore == nullptr)
		return;

	// these are defaults:
// 	qApp->setOrganizationName("scadastudio");
// 	qApp->setOrganizationDomain("scadastudio");
// 	qApp->setApplicationName("scadastudio");
//	qApp->setApplicationVersion(RSettings::getVersionString());
	m_pMainModuleInterface = pCore;

	m_pUi = pCore->GetUIMgr();

	m_pUi->GetRightDock()->setMinimumWidth(300);

	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	connect(pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_currentChanged(int )));

	connect(pTabWidget,SIGNAL(SendCloseWidget(QWidget*)),this,SLOT(Slot_CloseTabWidget(QWidget*)));

	m_pUi->GetTabWidget()->m_fnBeforeCloseTab = std::bind(&CGraphApi::CallBackBeforeCloseTab,this,std::placeholders::_1);
	
	CreateMenu();
	CreateDockWidgets(m_pUi);

	m_pUi->GetRightDock()->resize(QSize(500, 1000));
}

void CGraphApi::UnInit()
{

}
/*! \fn void  CGraphApi::InitGraphRootTreeItem()
********************************************************************************************************* 
** \brief CGraphApi::InitGraphRootTreeItem 
** \details 创建树叶（图形项），注册右键菜单
** \return void 
** \author LiJin
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
void  CGraphApi::InitGraphRootTreeItem()
{
	Q_ASSERT(m_pUi);
	if (m_pUi == nullptr)
		return;

	CQuoteItem *pItem = m_pUi->GetTopItem();

	CTreeWidget *pTree = m_pUi->GetLeftTree();
	Q_ASSERT(pTree);

	//CQuoteItem *pTopTreeItem = m_pUi->GetTopItem();

	QString strName = tr("Graphics");
	CQuoteItem *pNewItem = new CQuoteItem(strName);

	pNewItem->setData(GRAPH_ROOT_ITEM, Qt::UserRole+1);
	pNewItem->setIcon(QIcon(GRAPH_PNG));

	pItem->appendRow(pNewItem);

	m_pGraphicsRoot = pNewItem;

	connect(pTree, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	//	connect(pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(pTree, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slot_doubleClicked(const QModelIndex &)));
	connect(pTree, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_itemChanged(QStandardItem *)));
	//	connect(pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));
	//	connect(pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));

	connect((QStandardItemModel*)m_pUi->GetLeftTree()->model(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_TreeItemChange(QStandardItem *)));


}

void CGraphApi::slot_currentChanged(int nIdx)
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	//int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nIdx);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			//m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pView);
			if (pView)
			{
				pView->UpdateGraphLayerList(m_pLayerList);
			}
		}
		else
		{
			m_pDockWidget->setVisible(false);
		}
	}
	else
	{
		m_pDockWidget->setVisible(false);
	}

	ClearPropertyBrowser();
}

void CGraphApi::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole+1).toInt();

	if (nType == GRAPH_ROOT_ITEM || nType == GRAPH_GROUP_CHILD_ITEM || nType == GRAPH_CHILD_ITEM)
	{
		ShowMenu(indexSelect);
	}
}

void CGraphApi::ShowRootMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(nullptr);

	QAction *pActionGraph = new QAction(GRAPH_NAME, pMenu);
	pActionGraph->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pActionGraph);

	QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);
	pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pActionGroup);

	QAction *pActionRename = new QAction(RENAME_TREE_ITEM, pMenu);
	pMenu->addAction(pActionRename);

	
	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pActionGraph)
	{
		//添加图形
		AddGraph(index);
	}
	else if (selectedItem == pActionGroup)
	{
		//添加通道组
		AddGraphGroup(index);
	}
	else if (selectedItem == pActionRename)
	{
		//
		RenameTreeItem();
	}
	pMenu->deleteLater();
}

void  CGraphApi::ShowGraphChildMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(nullptr);
	
// 	QAction *pActionCut = new QAction(CUT_TREE_ITEM, pMenu);
// 	pMenu->addAction(pActionCut);

	QAction *pActionCopy = new QAction(COPY_TREE_ITEM, pMenu);
	pMenu->addAction(pActionCopy);

	QAction *pActionPaste = new QAction(PASTE_TREE_ITEM, pMenu);
	pMenu->addAction(pActionPaste);

	QAction *pActionDelete = new QAction(DELETE_TREE_ITEM, pMenu);
	pMenu->addAction(pActionDelete);

	QAction *pActionRename = new QAction(RENAME_TREE_ITEM, pMenu);
	pMenu->addAction(pActionRename);

	QAction *pActionAttr = new QAction(ATTR_TREE_ITEM, pMenu);
	pMenu->addAction(pActionAttr);

	QAction* pSel = pMenu->exec(QCursor::pos());

	if (pSel == pActionDelete)
	{
		DeleteGraph(index);
	}
	else if (pSel == pActionRename)
	{
		RenameTreeItem();
	}

// 	if (selectedItem == pChannelAction)
// 	{
// 		//添加图形
// 		AddGraph(index);
// 	}
// 	else if (selectedItem == pActionGroup)
// 	{
// 		//添加通道组
// 		AddGraphGroup(index);
// 	}
	
	pMenu->deleteLater();
}

void  CGraphApi::ShowGraphGrpChildMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(nullptr);

	//新建图形文件
	QAction *pActionGraph = new QAction(GRAPH_NAME, pMenu);
	pActionGraph->setIcon(QIcon(CHANNEL_PNG));
	pMenu->addAction(pActionGraph);

	//新建组
	QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);
	pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pActionGroup);

	QAction *pActionRename = new QAction(RENAME_TREE_ITEM, pMenu);
	pMenu->addAction(pActionRename);


	//删除组
	QAction *pDeleteGroup = new QAction(DELETE_TREE_ITEM, pMenu);
	pMenu->addAction(pDeleteGroup);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pActionGraph)
	{
		//添加图形
		AddGraph(index);
	}
	else if (selectedItem == pActionGroup)
	{
		//添加通道组
		AddGraphGroup(index);
	}
	else if (selectedItem == pActionRename)
	{
		//
		RenameTreeItem();
	}
	else if (selectedItem == pDeleteGroup)
	{
		//删除组
		DeleteGraphFolder(index);
	}

	pMenu->deleteLater();
	////////////////////////////////
}

void CGraphApi::ShowMenu(QModelIndex &index)
{
	int nRet = index.data(Qt::UserRole+1).toInt();

	if (nRet == GRAPH_ROOT_ITEM)
	{
		ShowRootMenu(index);
	}
	else if (nRet == GRAPH_GROUP_CHILD_ITEM)
	{
		ShowGraphGrpChildMenu(index);
	}
	else if (nRet == GRAPH_CHILD_ITEM)
	{
		ShowGraphChildMenu(index);
	}
	else
	{
		Q_ASSERT(false);
	}
}


void CGraphApi::slot_doubleClicked(const QModelIndex &index)
{
	int nRet = index.data(Qt::UserRole + 1).toInt();

    if (nRet != GRAPH_CHILD_ITEM)
    {
        m_pToolBoxDockWidget->setHidden(true);
        return;
    }
	

	long long llRet = index.data(USER_DATA_ROLE).toLongLong();

	CGraphFile *pFile = reinterpret_cast<CGraphFile*>(llRet);
	Q_ASSERT(pFile);
	if (pFile == nullptr)
		return;

	pFile->SetFlashFlag(true);

	// 查找右边的TAB中有没有属于该文件的tab 
	bool bFind = false;
	QWidget *pWidget = nullptr;
	CGraphTabView *pGraphView = nullptr;
	//TODO
	int nCount = m_pUi->GetTabWidget()->count();
	for (int i = 0; i < nCount;i++)
	{
		pWidget = m_pUi->GetTabWidget()->widget(i);
		pGraphView = dynamic_cast<CGraphTabView*>(pWidget);
		if (pGraphView)
		{
			if (pGraphView->GetGraphFile() == pFile)
			{
				bFind = true;
				
				break;
			}
		}
	}

	// 如果没有找到以前的窗口，说明已经被关闭了。
	// 如果找到，则激活	
	//m_pUi->GetTabWidget()->m_pBeforeCloseTabData = reinterpret_cast <void*>(m_pWindow);
	if (bFind)
	{
		Q_ASSERT(pGraphView);
		m_pUi->GetTabWidget()->setCurrentWidget(pGraphView);
		pGraphView->GetGraphView()->ChangeProperty();
		//ClearPropertyBrowser();
	}
	else
	{
		pFile->ClearLayer();
		// 如果没有找到，则创建
	     m_pGraphData->OpenFiles(index.data(USER_PATH_ROLE).toString(), pFile);

		AddNewTabView(pFile);
		//创建的话 清空
		ClearPropertyBrowser();
	}

    m_pToolBoxDockWidget->setVisible(true);
    m_pToolBoxDockWidget->raise();
	
	m_pDockWidget->setVisible(true);

}

void CGraphApi::AddNewTabView(CGraphFile *pFile)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
		return;

	CGraphTabView *pWindow = new CGraphTabView(nullptr,pFile);

	QString szName = QString::fromLocal8Bit(pFile->GetGraphName().c_str());
	m_pUi->GetTabWidget()->addTab(pWindow, szName);
	m_pUi->GetTabWidget()->setCurrentWidget(pWindow);

	pWindow->UpdateUI(pFile, m_pLayerList);
	
}

void CGraphApi::ClearPropertyBrowser()
{

	IMainWindow *pMainFrm = GetMainWindow();
	Q_ASSERT(pMainFrm);
	if (pMainFrm == nullptr)
		return;
	

	QtTreePropertyBrowser *pTreeBrowser = pMainFrm->GetAtrri()->GetTreePropertyBrowser();

	QList<QtProperty *> listProperty = pTreeBrowser->properties();


	if (listProperty.empty() == false)
	{
		for (auto it : listProperty)
		{
			pTreeBrowser->removeProperty(it);
		}
	}
	listProperty.clear();
}

void CGraphApi::DeleteTabView(CGraphFile *pFile)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
		return;

	CTabWidget *pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	QWidget *pWidget = nullptr;
	CGraphTabView *pGraphView = nullptr;
	int nCount = m_pUi->GetTabWidget()->count();
	for (int i = 0; i < nCount; i++)
	{
		pWidget = m_pUi->GetTabWidget()->widget(i);
		pGraphView = dynamic_cast<CGraphTabView*>(pWidget);
		if (pGraphView)
		{
			if (pGraphView->GetGraphFile() == pFile)
			{
				pTabWidget->removeTab(i);			
				return;
			}
		}
	}
}

void CGraphApi::CreateDockWidgets(IMainWindow *pMainWindow)
{
	Q_ASSERT(pMainWindow);
	if (pMainWindow == nullptr)
		return;

	m_pDockWidget = new CGraphLayerDockWidget(pMainWindow,0);
	m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //m_pDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	//m_pDockWidget->setFixedWidth(260);
	//pMainWindow->addDockWidget(Qt::RightDockWidgetArea, m_pDockWidget);
	m_pDockWidget->setWindowTitle(tr("Layer"));
	
	QTabWidget *pTab = new QTabWidget(m_pDockWidget);

	m_pDockWidget->setWidget(pTab);
	m_pLayerList = new QTableWidget;

	pTab->addTab(m_pLayerList, tr("Layer"));

	m_pLayerList->setColumnCount(3);
	m_pLayerList->setRowCount(16);
	m_pLayerList->horizontalHeader()->setSectionsClickable(false);
	m_pLayerList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_pLayerList->verticalHeader()->setVisible(false);
	m_pLayerList->horizontalHeader()->setVisible(false);
	m_pLayerList->setSelectionMode(QAbstractItemView::NoSelection);
//	m_pLayerList->setSelectionBehavior(QAbstractItemView::SelectItems);

	m_pLayerList->setColumnWidth(0, 40);
	m_pLayerList->setColumnWidth(1, 120);
	m_pLayerList->setColumnWidth(2, 60);

	// 创建时隐藏
	m_pDockWidget->setVisible(false);
//	connect(m_pLayerList, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slotItemClicked(QTableWidgetItem*)));

    m_pToolBoxDockWidget = new CGraphLayerDockWidget(pMainWindow, 0);
    m_pToolBoxDockWidget->setWindowTitle(tr("ToolBox"));
    

	pMainWindow->addDockWidget(Qt::RightDockWidgetArea, m_pToolBoxDockWidget);

	pMainWindow->tabifyDockWidget(m_pToolBoxDockWidget, pMainWindow->GetRightDock());

	pMainWindow->addDockWidget(Qt::RightDockWidgetArea, m_pDockWidget);
	pMainWindow->tabifyDockWidget(pMainWindow->GetRightDock(), m_pDockWidget);



     m_pToolBoxDockWidget->raise();
    //
	m_pToolWgt = new CGraphToolsWgt;
    m_pToolBoxDockWidget->setWidget(m_pToolWgt);

    pMainWindow->GetRightDock()->setWindowTitle(tr("Property"));

    m_pToolBoxDockWidget->setHidden(true);

	//读取模板
	LoadTemplateInfo();

//     pMainWindow->splitDockWidget(m_pToolBoxDockWidget, (QDockWidget*)pMainWindow->GetRightDock(), Qt::Horizontal);
//     pMainWindow->splitDockWidget((QDockWidget*)pMainWindow->GetRightDock(), m_pDockWidget, Qt::Horizontal);
    //
//     m_pToolBoxDockWidget->setVisible(false);
//     
//     pMainWindow->GetRightDock()->setVisible(false);
}

#if 0
void  CGraphApi::slotItemClicked(QTableWidgetItem* pItem)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return;
// 	Q_ASSERT(m_pGraphView);
// 	if (m_pGraphView == nullptr)
// 		return;

	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
 			CGraphTabView *pView = dynamic_cast<CGraphTabView*>(pWidget);
 			Q_ASSERT(pView);
 			if (pView)
 			{
				pView->slotItemClicked(pItem);
 			}
		}		
	}
}
#endif

void CGraphApi::slot_itemChanged(QStandardItem *item)
{
	QString strTmp = item->data(Qt::EditRole).toString();

	int nRet = item->data(Qt::UserRole + 1).toInt();
	if (nRet != GRAPH_GROUP_CHILD_ITEM || nRet != GRAPH_CHILD_ITEM)
		return;

#if 0
	if (item->data(Qt::UserRole + 1).toInt() == FES_CHANNEL_CHILD_ITEM)
	{
		QString strTag = item->data(TAGNAME).toString();

		QString strChannelName = item->data(Qt::EditRole).toString();

		Q_ASSERT(m_pArrChannel);

		bool bFlag = CheckTagNameAndChannelName(strTag, strChannelName);

		//判断数据库是否存在该通道
		if (bFlag)
		{
			//QMessageBox::warning(NULL, "warning", QStringLiteral("通道名重复!"));
		}
		else
		{

		}
	}
	else if (item->data(Qt::UserRole + 1).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		//分组名重复是前置所有分组不可以重复还是所有前置分组
		QString strTmp = item->data(TAGNAME).toString();

		QString strDisplay = item->data(Qt::EditRole).toString();

		QString strGroup;

		if (strTmp.compare(strDisplay) != 0)
		{
			QStandardItem *pItem = item;

			////找通道项
			//while (pItem->hasChildren())
			//{
			//	pItem = pItem->child(0);

			//	if (pItem->data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
			//	{

			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
			//
			//if (pItem != item)
			//{
			//	//拼组
			//	while (pItem->parent() != NULL && pItem->parent()->data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
			//	{
			//		strGroup += "." + item->parent()->data(Qt::EditRole).toString();
			//	}
			//}

		}
	}
#endif
}

void CGraphApi::AddGraph(QModelIndex &index)
{
	std::string szNewFileName = "";

	//添加图形
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	if (pItem == nullptr)
	{
		return;
	}

	CGraphFile* pGraph = nullptr;

	QString szText = GetRelativePath(pItem);

	QString strName = "";


	if (pItem == m_pGraphicsRoot)
	{
		szNewFileName = m_pGraphData->RecommendFileName();
		pGraph = m_pGraphData->CreateGraph(szNewFileName);
		strName = QString::fromLocal8Bit(szNewFileName.c_str());
	}
	else
	{
		CGraphBaseData* pParentData = reinterpret_cast<CGraphBaseData*>(pItem->data(USER_DATA_ROLE).toLongLong());
		if (pParentData != nullptr && pParentData->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			//上层为目录数据结构
			CGraphFolder *pFolderData = dynamic_cast<CGraphFolder *>(pParentData);
			if (pFolderData != nullptr)
			{
				szNewFileName = pFolderData->RecommendFileName();
				strName = QString::fromLocal8Bit(szNewFileName.c_str());

				pGraph = pFolderData->CreateGraph(szNewFileName);
			}
		}
	}
	
	if (pGraph == nullptr)
	{
		return;
	}

	CQuoteItem *pNewItem = new CQuoteItem(strName);

	pNewItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole + 1);
	pNewItem->setData((reinterpret_cast<long long>(pGraph)), USER_DATA_ROLE);

	pNewItem->setData(m_strGraphcsPath + "/" + szText + strName + ".pic", USER_PATH_ROLE);

	pGraph->SetFilePath(m_strGraphcsPath + "/" + szText + strName + ".pic");

	pNewItem->setIcon(QIcon(GRAPH_PNG));

	pItem->appendRow(pNewItem);
	m_pUi->GetLeftTree()->expand(index);

}

QString CGraphApi::GetRelativePath(CQuoteItem *pItem)
{
	QString tStrpath = "/";

	while (pItem != m_pGraphicsRoot)
	{
		tStrpath = pItem->text() + "/" + tStrpath;

		pItem = dynamic_cast<CQuoteItem *>(pItem->parent());

		if (pItem == nullptr)
		{
			return "";
		}
	}

	return tStrpath;
}


void CGraphApi::DeleteGraph(QModelIndex &index)
{
	// 删除树叶
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	if (pItem == nullptr)
		return; 
	long long nRet = pItem->data(USER_DATA_ROLE).toLongLong();
	Q_ASSERT(nRet);

	m_pUi->GetLeftTreeModel()->removeRow(pItem->row(),index.parent());
	// 关闭右边 tab
	if (nRet)
	{
		CGraphFile *pFile = reinterpret_cast<CGraphFile*>(nRet);
		Q_ASSERT(pFile);
		if (pFile)
		{
			
			DeleteTabView(pFile);
			//删除数据
			m_pGraphData->DeleteBaseData(pFile);
		}
	}
}

void  CGraphApi::DeleteGraphFolder(QModelIndex &index)
{
	// 删除树叶
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	if (pItem == nullptr)
	{
		return;
	}

	long long nRet = pItem->data(USER_DATA_ROLE).toLongLong();

	Q_ASSERT(nRet);

	if (nRet)
	{
		if (pItem->parent() == m_pGraphicsRoot)
		{
			//父节点为跟目录
			CGraphFolder *pFile = reinterpret_cast<CGraphFolder*>(nRet);
			Q_ASSERT(pFile);
			if (pFile)
			{
				//删除数据
				m_pGraphData->DeleteBaseData(pFile);
			}
		}
		else
		{
			//父节点不为跟目录
			CQuoteItem *pParent = dynamic_cast<CQuoteItem*>(pItem->parent());
			Q_ASSERT(pParent);

			if (pParent == nullptr)
			{
				return;
			}

			long long nParentRet = pParent->data(USER_DATA_ROLE).toLongLong();

			CGraphFolder *pParentFolder = reinterpret_cast<CGraphFolder*>(nParentRet);
			Q_ASSERT(pParentFolder);

			if (pParentFolder != nullptr)
			{
				pParentFolder->DeleteBaseData(reinterpret_cast<CGraphBaseData*>(nRet));
			}

		}
	}

	//遍历所有的子item
	DeleteAllChildItems(pItem);

	m_pUi->GetLeftTreeModel()->removeRow(pItem->row(), index.parent());
}

//删除所有的子item
void CGraphApi::DeleteAllChildItems(CQuoteItem *pParent)
{
	Q_ASSERT(pParent);

	for (int i=0; i<pParent->rowCount(); i++)
	{
		CQuoteItem *pChild = dynamic_cast<CQuoteItem*>(pParent->child(i));
		Q_ASSERT(pChild);
		if (pChild == nullptr)
		{
			return;
		}

		if (pChild->hasChildren())
		{
			//文件夹节点
			DeleteAllChildItems(pChild);
		}
		else
		{
			DeleteGraph(pChild->index());
			//文件节点
		}
	}
}


void CGraphApi::AddGraphGroup(QModelIndex &index)
{


	std::string szNewName = "";

	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//////////////////////////////////
	CGraphBaseData* pGraph = nullptr;

	if (item == m_pGraphicsRoot)
	{
		szNewName = m_pGraphData->RecommendFolderName();
		pGraph = m_pGraphData->CreateGraphFolder(szNewName);
	}
	else
	{
		CGraphBaseData* pParentData = reinterpret_cast<CGraphBaseData*>(item->data(USER_DATA_ROLE).toLongLong());

		if (pParentData != nullptr && pParentData->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			//上层为目录数据结构
			CGraphFolder *pFolderData = dynamic_cast<CGraphFolder *>(pParentData);

			if (pFolderData != nullptr)
			{
				szNewName = pFolderData->RecommendFolderName();
				pGraph = pFolderData->CreateGraphFolder(szNewName);


			}
		}
	}

	if (pGraph == nullptr)
	{
		return;
	}

	//////////////////////////////////

	QString strName = QString::fromLocal8Bit(szNewName.c_str());

	if (item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *pNewItem = new CQuoteItem(strName);

		pNewItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole + 1);

		pNewItem->setData((reinterpret_cast<std::size_t>(pGraph)), USER_DATA_ROLE);

		QString szText = GetRelativePath(item);

		pNewItem->setData(m_strGraphcsPath + "/" + szText + strName, USER_PATH_ROLE);


		pNewItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(pNewItem);
	}

	m_pUi->GetLeftTree()->expand(index);
}

void CGraphApi::CreateMenu()
{
	QMenuBar *pBar = m_pUi->GetMainMenuBar();
	Q_ASSERT(pBar);
	if (pBar)
	{
#if 0
		m_pFileMenu = pBar->addMenu(QStringLiteral("文件(&F)"));
		Q_ASSERT(m_pFileMenu);
		if (m_pFileMenu)
		{
			QAction *action = m_pFileMenu->addAction(QStringLiteral("打印(&P)"));
			connect(action, SIGNAL(triggered()), this, SLOT(PrintGraph()));

			action = m_pFileMenu->addAction(QStringLiteral("导出(&E)"));
			connect(action, SIGNAL(triggered()), this, SLOT(ExportImage()));

 			action = m_pFileMenu->addAction(QStringLiteral("退出(&Q)"));
 			connect(action, SIGNAL(triggered()), this, SLOT(close()));
		}
#endif
		m_pEditMenu = pBar->addMenu(QStringLiteral("编辑(&E)"));
		Q_ASSERT(m_pEditMenu);
		if (m_pEditMenu)
		{
			QAction *action = m_pEditMenu->addAction(QStringLiteral("剪切(&T)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnCut()));

			action = m_pEditMenu->addAction(QStringLiteral("复制(&C)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnCopy()));

			action = m_pEditMenu->addAction(QStringLiteral("粘帖(&P)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnPaste()));

			action = m_pEditMenu->addAction(QStringLiteral("删除(&D)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnDelete()));

			m_pEditMenu->addSeparator();

			action = m_pEditMenu->addAction(QStringLiteral("组合(&G)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsGroup()));

			action = m_pEditMenu->addAction(QStringLiteral("取消组合(&U)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsBreak()));

			action = m_pEditMenu->addAction(QStringLiteral("编辑组合(&E)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(OnWidgetsBreak()));
		}

// 		m_pElementMenu = pBar->addMenu(QStringLiteral("图元(&W)"));
// 		Q_ASSERT(m_pElementMenu);
// 		if (m_pElementMenu)
// 		{
// 			m_pDrawToolActGrp = new QActionGroup(this);
// 			m_pDrawToolActGrp->setExclusive(true);
// 
// 			QAction *pAction = m_pElementMenu->addAction(QStringLiteral("选择(&S)"));
// 			pAction->setCheckable(true);
// 		 	connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
// 			pAction->setActionGroup(m_pDrawToolActGrp);
// 			m_pDrawToolSel = pAction;
// 
// 			// 			QAction *pAction = new QAction(this);
// 			// 			m_pDrawToolActGrp->addAction(pAction);
// 			// 			connect(pAction, &QAction::triggered, this, CGraphApi::SelectDrawTool);
// 
// 			pAction = m_pElementMenu->addAction(QStringLiteral("直线(&L)"));
// 			pAction->setCheckable(true);
// 		 	connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
// 			pAction->setActionGroup(m_pDrawToolActGrp);
// 			m_pDrawToolLine = pAction;
// 
// 			pAction = m_pElementMenu->addAction(QStringLiteral("矩形(&R)"));
// 			pAction->setCheckable(true);
// 		 	connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
// 			pAction->setActionGroup(m_pDrawToolActGrp);
// 			m_pDrawToolRect = pAction;
// 
// 			pAction = m_pElementMenu->addAction(QStringLiteral("圆形(&C)"));
// 			pAction->setCheckable(true);
// 		 	connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
// 			pAction->setActionGroup(m_pDrawToolActGrp);
// 			m_pDrawToolRound = pAction;
// 
// 			pAction = m_pElementMenu->addAction(QStringLiteral("圆角矩形(&R)"));
// 			pAction->setCheckable(true);
// 		 	connect(pAction, SIGNAL(triggered()), this, SLOT(SelectDrawTool()));
// 			pAction->setActionGroup(m_pDrawToolActGrp);
// 			m_pDrawToolRoundRect = pAction;
// 
// 		}

		m_pArrangeMenu = pBar->addMenu(QStringLiteral("排列"));
		if (m_pArrangeMenu)
		{
			QAction *pAction = m_pArrangeMenu->addAction(QStringLiteral("自动对齐"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnAutoAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("左对齐"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnLeftAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("水平居中"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnCenterHorzAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("右对齐"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnRightAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("顶端对齐"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnTopAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("垂直居中"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnCenterVertAlign);

			pAction = m_pArrangeMenu->addAction(QStringLiteral("底端对齐"));
		 	connect(pAction, &QAction::triggered, this, &CGraphApi::OnButtomAlign);

		}

		m_pDebugMenu = pBar->addMenu(QStringLiteral("调试(&D)"));
		Q_ASSERT(m_pDebugMenu);
		if (m_pDebugMenu)
		{
			QAction *action = m_pDebugMenu->addAction(QStringLiteral("模拟(&S)"));
		 	connect(action, SIGNAL(triggered()), this, SLOT(Simulation()));
		}
	}
}

void CGraphApi::SelectDrawTool()
{
	CTabWidget *pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	QWidget *pWidget = pTabWidget->currentWidget();
	if (pWidget == nullptr)
		return;

	CGraphTabView *pGraphTabView = dynamic_cast<CGraphTabView*>(pWidget);
	if (pGraphTabView == nullptr)
		return;

	CGraphView *pGraphView = pGraphTabView->GetGraphView();
	Q_ASSERT(pGraphView);
	if (pGraphView == nullptr)
		return;
 
	Q_ASSERT(m_pDrawToolActGrp);
	if (m_pDrawToolActGrp)
	{
		QAction *pAct = m_pDrawToolActGrp->checkedAction();
		if (pAct)
		{
			//pAct->setChecked(true);
			if (pAct == m_pDrawToolSel)
			{
				pGraphView->m_nDrawTool = DRAW_TOOLS_SEL;
			}
			else if (pAct == m_pDrawToolLine)
			{
				pGraphView->m_nDrawTool = DRAW_TOOLS_LINE;
			}
			else if (pAct == m_pDrawToolRect)
			{
				pGraphView->m_nDrawTool = DRAW_TOOLS_RECTANGLE;
			}
			else if (pAct == m_pDrawToolRound)
			{
				pGraphView->m_nDrawTool = DRAW_TOOLS_ROUND;
			}
			else if (pAct == m_pDrawToolRoundRect)
			{
				pGraphView->m_nDrawTool = DRAW_TOOLS_ROUNDRECT;
			}
		}
	}
 
}


void CGraphApi::OnCopy()
{
#if 0
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCopy();
	}
#endif
}

void CGraphApi::OnPaste()
{
#if 0
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnPaste();
	}
#endif
}

void CGraphApi::OnDelete()
{
#if 0
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnDelete();
	}
#endif
}

void CGraphApi::OnCut()
{
#if 0
	Q_ASSERT(m_pGraphView);
	if (m_pGraphView)
	{
		m_pGraphView->OnCut();
	}
#endif
}
void CGraphApi::OnWidgetsGroup()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnWidgetsGroup();
			}
		}
	}
}

void CGraphApi::OnWidgetsBreak()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnWidgetsBreak();
			}
		}
	}
}

void CGraphApi::OnAutoAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnAutoAlign();
			}
		}
	}
}

void CGraphApi::OnLeftAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnLeftAlign();
			}
		}
	}

}

void CGraphApi::OnRightAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnRightAlign();
			}
		}
	}
}

void CGraphApi::OnCenterHorzAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnCenterHorzAlign();
			}
		}
	}
}

void CGraphApi::OnTopAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnTopAlign();
			}
		}
	}
}

void CGraphApi::OnCenterVertAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnCenterVertAlign();
			}
		}
	}
}

void CGraphApi::OnButtomAlign()
{
	CTabWidget *pTabWidget = nullptr;
	pTabWidget = m_pUi->GetTabWidget();
	Q_ASSERT(pTabWidget);
	if (pTabWidget == nullptr)
		return;

	int nCur = pTabWidget->currentIndex();
	QWidget *pWidget = pTabWidget->widget(nCur);
	if (pWidget)
	{
		if (pWidget->inherits("CGraphTabView"))
		{
			m_pDockWidget->setVisible(true);
			// 刷新 图层界面
			CGraphTabView *pTabView = dynamic_cast<CGraphTabView*>(pWidget);
			Q_ASSERT(pTabView);
			Q_ASSERT(pTabView->GetGraphView());
			if (pTabView && pTabView->GetGraphView())
			{
				pTabView->GetGraphView()->OnButtomAlign();
			}
		}
	}
}


void CGraphApi::Slot_CloseTabWidget(QWidget* pWidget)
{
	CGraphTabView* pGraphView = dynamic_cast<CGraphTabView*>(pWidget);

	if (pGraphView == nullptr)
	{
		return;
	}

	if (pGraphView != nullptr)
	{
		CGraphFile *pGraphFile = pGraphView->GetGraphFile();

		Q_ASSERT(pGraphView);

		if (pGraphView != nullptr)
		{
			pGraphFile->ClearLayer();
			pGraphFile->SetFlashFlag(false);
		}
	}
	
}

void CGraphApi::Slot_TreeItemChange(QStandardItem *pItem)
{
	if (!CheckRenameLegailty(pItem->parent(), pItem, pItem->text()))
	{
		pItem->setText(m_strOldName);
		return;
	}

	int nRet = pItem->data(Qt::UserRole + 1).toInt();

	if (nRet != GRAPH_GROUP_CHILD_ITEM && nRet != GRAPH_CHILD_ITEM)
	{
		return;
	}



	if (nRet == GRAPH_GROUP_CHILD_ITEM)
	{
		//组
		long long nRet = pItem->data(USER_DATA_ROLE).toLongLong();
		Q_ASSERT(nRet);

		// 关闭右边 tab
		if (nRet)
		{
			CGraphFolder *pFolder = reinterpret_cast<CGraphFolder*>(nRet);
			Q_ASSERT(pFolder);
			if (pFolder)
			{
				//重命名
				pFolder->SetFolderName(pItem->text().toLocal8Bit().data());
				pFolder->SetItemName(pItem->text().toLocal8Bit().data());

				QString strOldFileName = pItem->data(USER_PATH_ROLE).toString();

				QDir tDir(strOldFileName);

				int nLength = strOldFileName.split("/").last().length();
				QString strNewFileName = strOldFileName.remove(strOldFileName.length() - nLength, nLength) + pItem->text();

				tDir.rename(tDir.path(),strNewFileName);
				//遍历子item修改所有的名称
				RenameAllChildItems(pItem);
			}
		}
	}
	else if (nRet == GRAPH_CHILD_ITEM)
	{
		//文件
		long long nRet = pItem->data(USER_DATA_ROLE).toLongLong();
		Q_ASSERT(nRet);

		// 关闭右边 tab
		if (nRet)
		{
			CGraphFile *pFile = reinterpret_cast<CGraphFile*>(nRet);
			Q_ASSERT(pFile);
			if (pFile)
			{
				//重命名
				pFile->SetGraphName(pItem->text().toLocal8Bit().data());
				pFile->SetItemName(pItem->text().toLocal8Bit().data());
				QString strOldFileName = pItem->data(USER_PATH_ROLE).toString();


				QFile tRenameFile(strOldFileName);
				int nLength = strOldFileName.split("/").last().length();

				QString strNewFileName = strOldFileName.remove(strOldFileName.length()-nLength,nLength) + pItem->text() + ".pic";
				tRenameFile.rename(strNewFileName);
				pItem->setData(strNewFileName, USER_PATH_ROLE);

				pFile->SetFilePath(strNewFileName);
			}
		}
	}

	QString strRunPath = qApp->applicationDirPath();

	SaveProject(GetIMainInterface()->GetProjMgr()->GetDocument(), &GetIMainInterface()->GetProjMgr()->GetDocument()->documentElement(), strRunPath);

// 	if (pItem->data(Qt::UserRole) == LOGIC_TAG)
// 	{
// 		if (m_mapIndexNode.contains(pItem->index()))
// 		{
// 			std::shared_ptr<CNodeLogicContainer> pTagContainer = m_mapIndexNode[pItem->index()];
// 
// 			pTagContainer->SetName(pItem->text());
// 		}
// 	}
// 	else if (pItem->data(Qt::UserRole) == LOGIC_EDIT_ITEM)
// 	{
// 		if (m_mapIndexSegOrSr.contains(pItem->index()))
// 		{
// 			std::shared_ptr<CLogicObjectModel> pLogicObject = m_mapIndexSegOrSr[pItem->index()];
// 
// 			pLogicObject->SetName(pItem->text());
// 		}
// 	}
}

//遍历所有子item
void CGraphApi::RenameAllChildItems(QStandardItem *pParent)
{
	Q_ASSERT(pParent);

	if (pParent == nullptr)
	{
		return;
	}

	for (int i = 0; i < pParent->rowCount(); i++)
	{
		CQuoteItem *pChild = dynamic_cast<CQuoteItem*>(pParent->child(i));
		Q_ASSERT(pChild);
		if (pChild == nullptr)
		{
			return;
		}

		if (pChild->hasChildren())
		{
			//文件夹节点
			long long nRet = pChild->data(USER_DATA_ROLE).toLongLong();
			Q_ASSERT(nRet);
			if (!nRet)
			{
				return;
			}

			CGraphFolder *pFolder = reinterpret_cast<CGraphFolder*>(nRet);
			Q_ASSERT(pFolder);
			if (!pFolder)
			{
				return;
			}

			QString strOldFileName = pChild->data(USER_DATA_ROLE).toString();

			QDir tDir(strOldFileName);

			QString szText = GetRelativePath((CQuoteItem *)pParent);

			QString strNewFileName = m_strGraphcsPath + "/" + szText + pChild->text();

			pChild->setData(strNewFileName, USER_PATH_ROLE);


			tDir.rename(tDir.path(), strNewFileName);


			RenameAllChildItems(pChild);
		}
		else
		{
			//文件节点
			long long nRet = pChild->data(USER_DATA_ROLE).toLongLong();
			Q_ASSERT(nRet);
			if (!nRet)
			{
				return;
			}

			CGraphFile *pFile = reinterpret_cast<CGraphFile*>(nRet);
			Q_ASSERT(pFile);
			if (!pFile)
			{
				return;
			}

			QString strOldFileName = pFile->GetFilePath();


			QString szText = GetRelativePath((CQuoteItem *)pParent);

			QString strNewFileName = m_strGraphcsPath + "/" + szText + pChild->text() + ".pic";

			pChild->setData(strNewFileName, USER_PATH_ROLE);
			pFile->SetFilePath(strNewFileName);

		}
	}

}


bool CGraphApi::CheckRenameLegailty(QStandardItem *pParent, QStandardItem *pCurrent, const QString strName)
{
	Q_ASSERT(pParent);
	Q_ASSERT(pCurrent);

	if (pParent == nullptr || pCurrent == nullptr)
	{
		return false;
	}

	for (int i = 0; i < pParent->rowCount(); i++)
	{
		if (pParent->child(i)->text() == strName && pParent->child(i) != pCurrent)
		{
			QMessageBox::warning(0, tr("Warning"), tr("%1 has exists,please rename12").arg(strName));
			return false;
		}
	}

	return true;
}

bool CGraphApi::LoadTemplateInfo()
{
	QString strFilePath = qApp->applicationDirPath();

// 	QDir tDir(strFilePath + "/gtemplate");
// 
// 	QStringList tFilelist = tDir.entryList();
// 
// 	bool tFileFlag = false;
// 
// 	for (auto strFile : tFilelist)
// 	{
// 		if (strFile.endsWith(".xml"))
// 		{
// 			//库文件
// 			GetModuleApi()->GetToolWgt()->AddTemplateItem(strFilePath + "/gtemplate/" + strFile);
// 			tFileFlag = true;
// 		}
// 	}

	//库文件
	GetModuleApi()->GetToolWgt()->AnalyseTemplate(strFilePath + "/gtemplate/tlibrarymanger");
	return  true;
}

void CGraphApi::Simulation()
{

}


void CGraphApi::RenameTreeItem()
{
	if (!m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString().isEmpty())
	{
		m_strOldName = m_pUi->GetLeftTree()->model()->data(m_pUi->GetLeftTree()->currentIndex()).toString();
	}

	m_pUi->GetLeftTree()->edit(m_pUi->GetLeftTree()->currentIndex());
}

/*! \fn bool CGraphApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
********************************************************************************************************* 
** \brief CGraphApi::SaveProject 
** \details 保存图形工程
** \param pXml 
** \param pRoot 
** \param szRoot 
** \return bool 
** \author LiJin 
** \date 2016年8月27日 
** \note 
********************************************************************************************************/
bool CGraphApi::SaveProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_ASSERT(pXml);
	if (pXml == nullptr)
		return false;
	Q_ASSERT(szRoot.length() > 1);
	if (szRoot.length() <= 1)
		return false;

	Q_ASSERT(m_pGraphData);
	if (m_pGraphData == nullptr)
	{
		LogMsg("no graph files.", 0);	
		return false;
	}

	bool bRet = false;

	int nCount = m_pUi->GetTabWidget()->count();
	for (int i = 0; i < nCount; i++)
	{
		QWidget *pWidget = m_pUi->GetTabWidget()->widget(i);
		CGraphTabView *pGraphView = dynamic_cast<CGraphTabView*>(pWidget);

		if (pGraphView)
		{
			pGraphView->GetGraphFile()->SetFlashFlag(true);

		}
	}

	for (QDomNode nNode = pRoot->firstChild(); !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement nElement = nNode.toElement();

		if (nElement.tagName().compare("graph", Qt::CaseInsensitive) == 0)
		{
			//加载前清除之前文件
			while (nElement.hasChildNodes())
			{
				QDomNodeList nList = nElement.childNodes();

				for (int i = 0; i < nList.count(); ++i)
				{
					QDomNode nNode = nList.at(i);
					nElement.removeChild(nNode);
				}
			}
			bRet= m_pGraphData->SaveProject(pXml, &nElement, szRoot);
		}
	}





	//////////////////////

	//////////////////////
	return bRet;
}

bool CGraphApi::LoadProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot)
{
	Q_ASSERT(pXml);
	if (pXml == nullptr)
		return false;
	Q_ASSERT(szRoot.length() > 1);
	if (szRoot.length() <= 1)
		return false;

	InitGraphRootTreeItem();

	if (m_pGraphData)
	{
		m_pGraphData.reset();
	}
	m_pGraphData = std::make_shared<CGraphContainer>();



	for (QDomNode nNode = pRoot->firstChild(); !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement nElement = nNode.toElement();

		if (nElement.tagName().compare("graph", Qt::CaseInsensitive) == 0)
		{
			//生成文件
			if (nElement.hasChildNodes())
			{
				QDomNodeList nList = nElement.childNodes();
				
				for (int i = 0; i < nList.count(); ++i)
				{
					QDomNode nNode = nList.at(i);

					QDomElement tElement = nNode.toElement();
					//读取文件
					if (tElement.tagName() == "file")
					{
						std::string szNewFileName = nNode.toElement().attribute("name").toLocal8Bit().data();;


						//////////////////
						auto pGraph = m_pGraphData->CreateGraph(szNewFileName);
// 
// 						m_pGraphData->OpenFiles(szRoot + "/graph/" + nNode.toElement().attribute("name") + ".pic",pGraph);
// 
// 						if (pGraph == nullptr)
// 							return false;

						//////////////

						//添加图形
						QString strName = QString::fromLocal8Bit(szNewFileName.c_str());
						CQuoteItem *pNewItem = new CQuoteItem(strName);

						pNewItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole + 1);

 						pNewItem->setData((reinterpret_cast<long long>(pGraph)), USER_DATA_ROLE);

						pNewItem->setData(szRoot + "/graph/" + nNode.toElement().attribute("name") + ".pic", USER_PATH_ROLE);

						m_strGraphcsPath = szRoot + "/graph/";

						pNewItem->setIcon(QIcon(GRAPH_PNG));

						QString szText = GetRelativePath((CQuoteItem *)m_pGraphicsRoot);


						pGraph->SetFilePath(m_strGraphcsPath + "/" + szText + strName + ".pic");


						m_pGraphicsRoot->appendRow(pNewItem);						

					}
					else if (tElement.tagName() == "group")
					{
						QString strGroupName = tElement.attribute("name");

						auto pGraph = m_pGraphData->CreateGraphFolder(strGroupName.toStdString());


						CQuoteItem *pNewItem = new CQuoteItem(strGroupName);

						pNewItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole + 1);

						pNewItem->setData((reinterpret_cast<long long>(pGraph)), USER_DATA_ROLE);

						m_strGraphcsPath = szRoot + "/graph/";

						pNewItem->setIcon(QIcon(CLOSE_GROUP_PNG));

						QString szText = GetRelativePath(m_pGraphicsRoot);

						pNewItem->setData(m_strGraphcsPath + "/" + szText + strGroupName, USER_PATH_ROLE);


						m_pGraphicsRoot->appendRow(pNewItem);


						AnalyseGroupFile(pNewItem,&nNode.toElement(), m_strGraphcsPath + strGroupName + "/");
					}
					
				}
			}
		}
	}

	m_pUi->GetLeftTree()->expand(m_pGraphicsRoot->index());

	return true;
}


//遍历group下文件
void CGraphApi::AnalyseGroupFile(CQuoteItem *pParent,QDomElement*pGroupNode, QString strPath)
{
	if (pGroupNode->hasChildNodes())
	{
		QDomNodeList nList = pGroupNode->childNodes();

		//////////////////
		CGraphBaseData* pParentData = reinterpret_cast<CGraphBaseData*>(pParent->data(USER_DATA_ROLE).toLongLong());

		CGraphFolder* pGraphFolder = nullptr;

		if (pParentData != nullptr && pParentData->GetItemType() == CGraphBaseData::BASE_TYPE_FOLDER)
		{
			//上层为目录数据结构
			pGraphFolder = dynamic_cast<CGraphFolder *>(pParentData);
			if (pGraphFolder == nullptr)
			{
				return;
			}
		}

		for (int i=0; i<nList.count(); i++)
		{
			QDomNode nNode = nList.at(i);

			QDomElement tElement = nNode.toElement();

			if (tElement.tagName() == "file")
			{
				std::string szNewFileName = nNode.toElement().attribute("name").toLocal8Bit().data();;



				//////////////////
				auto pGraph = pGraphFolder->CreateGraph(szNewFileName);
				// 

				//添加图形
				QString strName = QString::fromLocal8Bit(szNewFileName.c_str());
				CQuoteItem *pNewItem = new CQuoteItem(strName);

				pNewItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole + 1);

				pNewItem->setData((reinterpret_cast<long long>(pGraph)), USER_DATA_ROLE);

				pNewItem->setData(strPath + "/" + nNode.toElement().attribute("name") + ".pic", USER_PATH_ROLE);

				QString szText = GetRelativePath((CQuoteItem *)pParent);


				pGraph->SetFilePath(m_strGraphcsPath + "/" + szText + strName + ".pic");


				pNewItem->setIcon(QIcon(GRAPH_PNG));

				pParent->appendRow(pNewItem);

				//m_pUi->GetLeftTree()->expand(m_pGraphicsRoot->index());

			}
			else if (tElement.tagName() == "group")
			{
				QString strGroupName = tElement.attribute("name");

				auto pGraph = pGraphFolder->CreateGraphFolder(strGroupName.toStdString());


				CQuoteItem *pNewItem = new CQuoteItem(strGroupName);

				pNewItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole + 1);

				pNewItem->setData((reinterpret_cast<long long>(pGraph)), USER_DATA_ROLE);


				pNewItem->setIcon(QIcon(CLOSE_GROUP_PNG));

				QString szText = GetRelativePath(pParent);

				pNewItem->setData(m_strGraphcsPath + "/" + szText + strGroupName, USER_PATH_ROLE);


				pParent->appendRow(pNewItem);


				AnalyseGroupFile(pNewItem, &nNode.toElement(), strPath + strGroupName + "/");
			}
		}
	}
}


bool CGraphApi::ChangeTagNameNodify(QString &tagName, QString &lastTagName)
{
	Q_UNUSED(tagName);
	Q_UNUSED(lastTagName);
	return true;
}

// 关闭工程文件
void CGraphApi::CloseProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, bool bSaveFlag)
{
	Q_UNUSED(pXml);
	Q_UNUSED(pRoot);
	Q_UNUSED(szRoot);
	Q_UNUSED(bSaveFlag);
	// 先关闭 自己模块所属的所有界面

	// 回收内存

	m_pGraphData.reset();
}
// 新建一个空的工程文件
void CGraphApi::CreateNewProject(QDomDocument *pXml, QDomElement *pRoot, const QString & szRoot, CQuoteItem *pTopItem)
{
	Q_UNUSED(pTopItem);
	Q_ASSERT(pXml && pRoot);
	if (pXml == nullptr || pRoot == nullptr)
		return;
	// 初始化
 	QDomElement root = pXml->createElement("graph");
 	pRoot->appendChild(root);
 
//  CQuoteItem *pRootItem = new CQuoteItem(QStringLiteral("图形"));
// 	pRootItem->setData(REPORT_ROOT, Qt::UserRole);
// 	pRootItem->setIcon(QIcon(":/images/report.ico"));
// 	pRootItem->setEditable(false);
// 
// 	pTopItem->appendRow(pRootItem);
	// 
	InitGraphRootTreeItem();

	if (m_pGraphData)
	{
		m_pGraphData.reset();
	}
	m_pGraphData = std::make_shared<CGraphContainer>();

	// 创建目录 
	QDir dir;
	bool bRet = dir.mkdir(szRoot + "/" + "graph");

	m_strGraphcsPath = szRoot + "/" + "graph/";

	if (!bRet)
	{

		QString strError = QString(tr("%1/graph folder mkdir failed!!!")).arg(szRoot);
		LogMsg(strError.toStdString().c_str(), 0);
	}
	 
}
/*! \fn bool  CGraphApi::LogMsg(const char *szLogTxt, int nLevel)
********************************************************************************************************* 
** \brief CGraphApi::LogMsg 
** \details log日志
** \param szLogTxt log的内容
** \param nLevel   log等级
** \return bool    是否成功标志
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
bool  CGraphApi::LogMsg(const char *szLogTxt, int nLevel)
{
	Q_ASSERT(szLogTxt && strlen(szLogTxt) > 0);
	if ( szLogTxt == nullptr || strlen(szLogTxt) == 0 ) 
		return false;

	Q_ASSERT(m_pMainModuleInterface);
	if (m_pMainModuleInterface == nullptr)
		return false;

	return m_pMainModuleInterface->LogMsg("graph", szLogTxt, nLevel);
}
