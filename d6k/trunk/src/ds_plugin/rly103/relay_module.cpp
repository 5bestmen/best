
#if 0
#if _MSC_VER >= 1600
#include <vld.h>
#endif
#endif

#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>

#include "relay_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h"

#include "devicestudio/treeitem_def.h"
#include "devicestudio/tabwidget.h"

#include "relaynet.h"


static CRelayModule *s_ptrModule = nullptr;

extern "C" DS_EXPORT IPluginModule *CreateModule()
{
	s_ptrModule = new CRelayModule;
	return s_ptrModule;
//	return  s_ptrModule.get();
}

bool LogString(const char *szLogTxt, int nLevel)
{
	if (s_ptrModule == nullptr)
		return false;
	 
	return s_ptrModule->LogString("RLY103",  szLogTxt, nLevel);
}

bool CommMsgLog( const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	if (s_ptrModule == nullptr)
		return false;

	return s_ptrModule->CommMsgLog("RLY103", pMsg, nMsgLen,nMsgType,pMsgSubType,pSource,pDesc);
}

CRelayModule::CRelayModule()
{
	m_pNet103 = std::make_shared < CNet103 >();
}

void CRelayModule::Init(IMainModule *pMainModule)
{
	Q_ASSERT(pMainModule);
	if (pMainModule == nullptr)
		return;
	
	m_pMainModule = pMainModule;
	//获取主界面对象
	m_pMainWindow = pMainModule->GetMainWindow();

	Q_ASSERT(m_pMainWindow);
	if (m_pMainWindow == nullptr)	
		return;	

	CreateTreeItem();

	connect(m_pMainWindow->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pMainWindow->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_ClickLeftTreeItem(const QModelIndex &)));
	connect(m_pMainWindow->GetTabWidget(), SIGNAL(tabCloseRequested(int)), this, SLOT(Slot_CloseOneTabWidget(int)));

	QString szLog(tr("rly103 plugin load success!"));
	pMainModule->LogString("rly103", szLog.toStdString().c_str(), 1);
}

void CRelayModule::UnInit()
{

}

void CRelayModule::SetLoadDeviceName(const QString &strDeviceName)
{

}

void CRelayModule::CreateTreeItem()
{
	//FTU
	QStandardItemModel *pTreeModel = (QStandardItemModel*)m_pMainWindow->GetLeftTree()->model();
	QStandardItem *pTopItem = (QStandardItem*)pTreeModel->item(0, 0);

	int iDevice_num = 1;
	for (int i = 0; i < pTopItem->rowCount(); i++)
	{
		if (pTopItem->child(i, 0)->text().contains("RLY", Qt::CaseInsensitive))
		{
			iDevice_num++;
		}
	}
	QString strDeviceName = "RLY_" + QString::number(iDevice_num);

	QStandardItem *pRelayItem = new QStandardItem(strDeviceName);
	pRelayItem->setData(TREE_PLUGIN_ROOT, TREE_LEVEL_DEF);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pTopItem->appendRow(pRelayItem);
	//遥信
	QStandardItem *pRemoteSignal = new QStandardItem(tr("遥信"));
	pRemoteSignal->setData(TREE_REMOTE_SIGNALITEM, Qt::UserRole);
	pRemoteSignal->setData(TREE_PLUGIN_CHILD_1, TREE_LEVEL_DEF);

	//newItem->setIcon(QIcon(ALARM_PNG));
	pRelayItem->appendRow(pRemoteSignal);

	//遥信定值设计
	//QStandardItem *pRemoteSignalFix = new QStandardItem(tr("定值设计"));
	//pRemoteSignalFix->setData(TREE_REMOTE_SIGNALFIX_ITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	//pRemoteSignal->appendRow(pRemoteSignalFix);
	//遥信测点
	//QStandardItem *pRemoteSignalPoint = new QStandardItem(tr("测点选择"));
	//pRemoteSignalPoint->setData(TREE_REMOTE_SINGALPOIN_TITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	//pRemoteSignal->appendRow(pRemoteSignalPoint);
	//遥信实时数据
	//QStandardItem *pRemoteSignalCurrentData = new QStandardItem(tr("实时数据"));
	//pRemoteSignalCurrentData->setData(TREE_REMOTE_CURRENTDATA_ITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	//pRemoteSignal->appendRow(pRemoteSignalCurrentData);
	//遥信实时普通数据
	//QStandardItem *pRemoteSignalCurrentGeneralData = new QStandardItem(tr("普通数据"));
	//pRemoteSignalCurrentGeneralData->setData(TREE_REOMTE_CURRETNDATA_GENERALITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	//pRemoteSignalCurrentData->appendRow(pRemoteSignalCurrentGeneralData);
	//遥信实时双位置数据
	//QStandardItem *pRemoteSignalCurrentDoubleData = new QStandardItem(tr("双位置数据"));
	//pRemoteSignalCurrentDoubleData->setData(TREE_REMOTE_CURRENTDATA_DOUBLEPOSITION_ITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	//pRemoteSignalCurrentData->appendRow(pRemoteSignalCurrentDoubleData);

	//遥测
	QStandardItem *pRemoteMeasure = new QStandardItem(tr("遥测"));
	pRemoteMeasure->setData(TREE_REMOTE_MEASUREITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pRelayItem->appendRow(pRemoteMeasure);

	//遥控
	QStandardItem *pRemotControl = new QStandardItem(tr("遥控"));
	pRemotControl->setData(TREE_REMOTE_CONTROLITEM, Qt::UserRole);
	//newItem->setIcon(QIcon(ALARM_PNG));
	pRelayItem->appendRow(pRemotControl);
}

void CRelayModule::Slot_ClickLeftTreeItem(const QModelIndex &qIndex)
{
	int nType = qIndex.data(Qt::UserRole).toInt();

	if (nType == TREE_REMOTE_SIGNALITEM)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_SIGNALITEM))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_SIGNALITEM]);
		}
		else
		{
			//遥信实时数据
			QWidget *testWgt = new QWidget;
			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥信实时数据"), tr("yxsssj"));
			m_MIndexTab.insert(TREE_REMOTE_SIGNALITEM, testWgt);
			m_MTabIndex.insert(testWgt, TREE_REMOTE_SIGNALITEM);
		}

	}
	else if (nType == TREE_REMOTE_MEASUREITEM)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_MEASUREITEM))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_MEASUREITEM]);
		}
		else
		{
			//遥测实时数据
			QWidget *testWgt = new QWidget;
			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥测实时数据"), tr("ycsssj"));
			m_MIndexTab.insert(TREE_REMOTE_MEASUREITEM, testWgt);
			m_MTabIndex.insert(testWgt, TREE_REMOTE_MEASUREITEM);
		}

	}
	else if (nType == TREE_REMOTE_CONTROLITEM)
	{
		if (m_MIndexTab.contains(TREE_REMOTE_CONTROLITEM))
		{
			m_pMainWindow->GetTabWidget()->setCurrentWidget(m_MIndexTab[TREE_REMOTE_CONTROLITEM]);
		}
		else
		{
			//遥控实时数据
			QWidget *testWgt = new QWidget;
			m_pMainWindow->GetTabWidget()->AddTab(testWgt, tr("遥控实时数据"), tr("yksssj"));
			m_MIndexTab.insert(TREE_REMOTE_CONTROLITEM, testWgt);
			m_MTabIndex.insert(testWgt, TREE_REMOTE_CONTROLITEM);

		}
	}
}

void CRelayModule::Slot_CloseOneTabWidget(int iIndex)
{
	//
	QWidget *pCloseWgt = m_pMainWindow->GetTabWidget()->widget(iIndex);
	
	if (pCloseWgt != nullptr)
	{
		int iIndex = m_MTabIndex[pCloseWgt];
		m_MIndexTab.remove(iIndex);
		m_MTabIndex.remove(pCloseWgt);
		m_pMainWindow->GetTabWidget()->removeTab(iIndex);
	}
}

bool CRelayModule::CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->CommMsgLog(pszModuleName, pMsg, nMsgLen,nMsgType,pMsgSubType,pSource,pDesc);
}

bool CRelayModule::LogString(const char *pszMoudleName,const char *szLogTxt, int nLevel)
{
	Q_ASSERT(m_pMainModule);
	if (m_pMainModule == nullptr)
		return false;
	return m_pMainModule->LogString(pszMoudleName, szLogTxt, nLevel);
}

void CRelayModule::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pMainWindow->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(TREE_LEVEL_DEF).toInt();

	if (nType == TREE_PLUGIN_ROOT)
	{
		ShowMenu(indexSelect);
	}
}

void CRelayModule::ShowMenu(QModelIndex &index)
{
	int nRet = index.data(TREE_LEVEL_DEF).toInt();

	if (nRet == TREE_PLUGIN_ROOT)
	{
		ShowRootMenu(index);
	}
// 	else if (nRet == GRAPH_GROUP_CHILD_ITEM)
// 	{
// 		ShowGraphGrpChildMenu(index);
// 	}
// 	else if (nRet == GRAPH_CHILD_ITEM)
// 	{
// 		ShowGraphChildMenu(index);
// 	}
// 	else
// 	{
// 		Q_ASSERT(false);
// 	}
}
void CRelayModule::ShowRootMenu(QModelIndex &index)
{
	Q_UNUSED(index);
	QMenu *pMenu = new QMenu(nullptr);

  	QAction *pActionConnect = new QAction(tr("Connect"), pMenu);
// 	pActionGraph->setIcon(QIcon(CHANNEL_PNG));
 	pMenu->addAction(pActionConnect);

	QAction *pActionClose = new QAction(tr("Close"), pMenu);
	pMenu->addAction(pActionClose);

// 	QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);
// 	pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
// 	pMenu->addAction(pActionGroup);
  
 	QAction* pSelItem = pMenu->exec(QCursor::pos());

	if (pSelItem == pActionConnect)
	{
		if (m_pNet103 != nullptr)
		{
			m_pNet103->Start();
		}	 
	}
	else if (pSelItem == pActionClose)
	{
		if (m_pNet103 != nullptr)
		{
			m_pNet103->Stop();
		}
	}
  
// 	if (selectedItem == pActionGraph)
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