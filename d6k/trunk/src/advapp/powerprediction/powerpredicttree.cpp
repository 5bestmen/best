#include "powerpredictdefine.h"
#include "powerpredicttree.h"
#include "define.h"


/*! \fn CPredictTree::CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore)
******************************************************************************************************
** \brief CPredictTree::CPredictTree
** \details 构造函数
** \return 
** \author GuoHaijun
** \date 2017年2月3日
** \note
*****************************************************************************************************/
CPredictTree::CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore)
{
	Q_ASSERT(nullptr != parent);
	Q_ASSERT(nullptr != pUi);
	Q_ASSERT(nullptr != pCore);

	m_pTreeWidget = parent;
	m_pUi = pUi;
	m_pCore = pCore;
}

/*! \fn void CPredictTree::InitTreeModel()
******************************************
** \brief CPredictTree::InitTreeModel
** \details 初始化树
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
*****************************************/
void CPredictTree::InitTreeModel()
{
	QObject::connect(GetTree(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ShowMouseRightButton(const QPoint&)));
	QObject::connect(GetTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));
	QObject::connect(GetTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Slot_DoubleClicked(const QModelIndex &)));
	QObject::connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_TreeItemChanged(QStandardItem *)));
}


/*! \fn void CPredictTree::ShowRootMenu(QModelIndex &index)
***********************************************************
** \brief CPredictTree::ShowRootMenu
** \details 显示根菜单,可增加厂站、厂站组
** \return void
** \author GuoHaijun
** \date 2017年2月24日
** \note
**********************************************************/
void CPredictTree::ShowRootMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//添加厂站组
	QAction* pAddStationGroup = new QAction(GROUP_NAME, pMenu);
	pAddStationGroup->setIcon(QIcon(PREDICT_GROUP_PNG));
	pMenu->addAction(pAddStationGroup);

	//添加厂站
	QAction *pAddStation = new QAction(ADD_STATION_NAME, pMenu);
	pAddStation->setIcon(QIcon(POWERPREDICT_PNG));
	pMenu->addAction(pAddStation);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pAddStationGroup)
	{
		AddGroup(index);
	}
	else  if (pSelectAction == pAddStation)
	{
		AddItem(index);
	}

	pMenu->deleteLater();

}

/*! \fn void CPredictTree::ShowGroupMenu(QModelIndex &index)
************************************************************
** \brief CPredictTree::ShowGroupMenu
** \details 显示组菜单
** \return void
** \author GuoHaijun
** \date 2017年2月24日
** \note
***********************************************************/
void CPredictTree::ShowGroupMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	QAction *pAddPredictItem = new QAction(ADD_POWER_FREDICT_ITEM, pMenu);
	pAddPredictItem->setIcon(QIcon(POWERPREDICT_PNG));
	pMenu->addAction(pAddPredictItem);

	QAction *pActionAddGroup = new QAction(GROUP_NAME, pMenu);
	pActionAddGroup->setIcon(QIcon(PREDICT_GROUP_PNG));
	pMenu->addAction(pActionAddGroup);

	QAction *pGroupRename = new QAction(RENAME_ITEM, pMenu);
	pGroupRename->setIcon(QIcon(PREDICT_RENAME_PNG));
	pMenu->addAction(pGroupRename);

	//此处DELETE_ITEM用于删除本Group
	QAction *pActionDeleteGroup = new QAction(DELETE_ITEM, pMenu);
	pActionDeleteGroup->setIcon(QIcon(PREDICT_DELETE_ITEM_PNG));
	pMenu->addAction(pActionDeleteGroup);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pActionAddGroup)
	{
		AddGroup(index);
	}
	else  if (pSelectAction == pActionDeleteGroup)
	{
		DeleteGroup(index);
	}
	else  if (pSelectAction == pAddPredictItem)
	{
		AddItem(index);
	}
	else  if (pSelectAction == pGroupRename)
	{
		m_pTreeWidget->edit(index);
	}

	pMenu->deleteLater();
}

/*! \fn void CPredictTree::ShowItemMenu(QModelIndex &index)
************************************************************
** \brief CPredictTree::ShowItemMenu
** \details 显示项菜单
** \return void
** \author GuoHaijun
** \date 2017年2月24日
** \note
***********************************************************/
void CPredictTree::ShowItemMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	QAction *pActionAddDevice = new QAction(ADD_DEVICE, pMenu);
	pActionAddDevice->setIcon(QIcon(PREDICT_DEVICE_PNG));
	pMenu->addAction(pActionAddDevice);

	QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
	pActionRename->setIcon(QIcon(PREDICT_RENAME_PNG));
	pMenu->addAction(pActionRename);

	QAction *pActionDeleteItem = new QAction(DELETE_ITEM, pMenu);
	pActionDeleteItem->setIcon(QIcon(PREDICT_DELETE_ITEM_PNG));
	pMenu->addAction(pActionDeleteItem);

	QAction *pActionAddItem = new QAction(ADD_POWER_FREDICT_ITEM, pMenu);
	pActionAddItem->setIcon(QIcon(POWERPREDICT_PNG));
	pMenu->addAction(pActionAddItem);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pActionRename)
	{
		m_pTreeWidget->edit(index);
	}
	else if (pSelectAction == pActionDeleteItem)
	{
		DeleteItem(index);
	}
	else if (pSelectAction == pActionAddDevice)
	{
		AddDevice(index);
	}
	else if (pSelectAction == pActionAddItem)
	{
		AddItem(index);
	}

	pMenu->deleteLater();
}

/*! \fn void CPredictTree::ShowDeviceItemMenu(QModelIndex &index)
*****************************************************************
** \brief CPredictTree::ShowDeviceItemMenu
** \details 显示装置项菜单
** \return void
** \author GuoHaijun
** \date 2017年2月24日
** \note
****************************************************************/
void CPredictTree::ShowDeviceItemMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	QAction *pDeleteDevice = new QAction(DELETE_DEVICE, pMenu);
	pDeleteDevice->setIcon(QIcon(PREDICT_DEVICE_PNG));
	pMenu->addAction(pDeleteDevice);

	QAction *pRenameAction = new QAction(RENAME_ITEM, pMenu);
	pRenameAction->setIcon(QIcon(PREDICT_RENAME_PNG));
	pMenu->addAction(pRenameAction);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pRenameAction)
	{
		m_pTreeWidget->edit(index);
	}
	else  if (pSelectAction == pDeleteDevice)
	{
		DeleteDevice(index);
	}

	pMenu->deleteLater();
}


/*! \fn void CPredictTree::Slot_ShowMouseRightButton(const QPoint& pos)
***********************************************************************
** \brief CPredictTree::Slot_ShowMouseRightButton
** \details 右击显示
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
**********************************************************************/
void CPredictTree::Slot_ShowMouseRightButton(const QPoint& pos)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(pos);

	int nType = indexSelect.data(PREDICT_ROOT_ROLE).toInt();

	switch (nType)
	{
	case POWER_PREDICT_ROOT_TYPE:
	{
		ShowRootMenu(indexSelect);
		break;
	}
	case POWER_PREDICT_GROUP_TYPE:
	{
		ShowGroupMenu(indexSelect);
		break;
	}
	case POWER_PREDICT_DEVICE_TYPE:
	{
		ShowDeviceItemMenu(indexSelect);
		break;
	}
	case POWER_PREDICT_ITEM_TYPE:
	{
		ShowItemMenu(indexSelect);
		break;
	}
	default:
		break;
	}
}

/*! \fn void CPredictTree::AddGroup(QModelIndex &index)
*******************************************************
** \brief CPredictTree::AddGroup
** \details 新增组
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
******************************************************/
void CPredictTree::AddGroup(QModelIndex &index)
{
	emit Signl_AddGroup(index);
}

/*! \fn void CPredictTree::DeleteGroup(QModelIndex &index)
**********************************************************
** \brief CPredictTree::DeleteGroup
** \details 删除组
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
*********************************************************/
void CPredictTree::DeleteGroup(QModelIndex &index)
{
	emit Signl_DeleteGroup(index);
}

/*! \fn void CPredictTree::AddItem(QModelIndex &index)
******************************************************
** \brief CPredictTree::AddItem
** \details 新增项
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
*****************************************************/
void CPredictTree::AddItem(QModelIndex &index)
{
	emit Signl_AddItem(index);
}

/*! \fn void CPredictTree::DeleteItem(QModelIndex &index)
*********************************************************
** \brief CPredictTree::DeleteItem
** \details 删除项
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
********************************************************/
void CPredictTree::DeleteItem(QModelIndex &index)
{
	emit Signl_DeleteItem(index);
}

/*! \fn void CPredictTree::AddDevice(QModelIndex &index)
*********************************************************
** \brief CPredictTree::AddDevice
** \details 添加设备
** \return void
** \author GuoHaijun
** \date 2017年2月23日
** \note
********************************************************/
void CPredictTree::AddDevice(QModelIndex &index)
{
	emit Signl_AddDevice(index);
}

/*! \fn void CPredictTree::DeleteDevice(QModelIndex &index)
*********************************************************
** \brief CPredictTree::DeleteDevice
** \details 删除设备
** \return void
** \author GuoHaijun
** \date 2017年2月23日
** \note
********************************************************/
void CPredictTree::DeleteDevice(QModelIndex &index)
{
	emit Signl_DeleteDevice(index);
}


/*! \fn void CPredictTree::Slot_Clicked(const QModelIndex &index)
****************************************************************
** \brief CPredictTree::Slot_Clicked
** \details 单击显示
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
***************************************************************/
void CPredictTree::Slot_Clicked(const QModelIndex &index)
{
	QVariant varCurrent = index.data(PREDICT_ROOT_ROLE);
	if (!varCurrent.isValid())
	{
		return;
	}

	int nType = varCurrent.toInt();
	switch (nType)
	{
	case POWER_PREDICT_GROUP_TYPE:
	{

		break;
	}
	case POWER_PREDICT_ITEM_TYPE:
	{
		

		break;
	}
	default:
		break;
	}
}


/*! \fn void CPredictTree::Slot_DoubleClicked(const QModelIndex &index)
***********************************************************************
** \brief CPredictTree::Slot_DoubleClicked
** \details 双击显示
** \return void
** \author GuoHaijun
** \date 2017年2月3日
** \note
**********************************************************************/
void CPredictTree::Slot_DoubleClicked(const QModelIndex &index)
{
	QModelIndex nIndex = index;

	int nType = nIndex.data(PREDICT_ROOT_ROLE).toInt();
/*

	if (nType != POWER_PREDICT_ITEM)
	{
		GetTree()->expanded(nIndex);
	}*/


	if (nType == POWER_PREDICT_ITEM_TYPE)
	{
		//CPredictSelectDialog* pPrdtDialog = new CPredictSelectDialog();
		//pPrdtDialog->show();
		
		//CSelectDialog* dialog = new CSelectDialog();
		//dialog->show();

		//CConfigWidget* dialog = new CConfigWidget();
		//dialog->show();

		/*TabKeyStruct keyInfo;
		keyInfo.BigTagName = "123";
		keyInfo.TagName = "456";

		m_pUi->GetTabWidget()->AddTab(dialog, "456", keyInfo, QIcon(":/images/powerpredictitem.png"));
		m_pUi->GetTabWidget()->setCurrentWidget(dialog);*/



/*
		CPredictDevice* pDevice = new CPredictDevice(m_pCore);


		auto pView = new CPredictAiView(nullptr, pDevice, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = "123";
		keyInfo.TagName = "456";

		m_pUi->GetTabWidget()->AddTab(pView, "456", keyInfo, QIcon(QString(":/images/powerpredictitem.png")));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);*/


/*

		QString strTagName = index.data(Qt::EditRole).toString();

		CSelectDialog* dialog = new CSelectDialog();
		dialog->show();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strTagName;
		keyInfo.TagName = strTagName;

		m_pUi->GetTabWidget()->AddTab(dialog, strTagName, keyInfo, QIcon(":/images/powerpredictitem.png"));
		m_pUi->GetTabWidget()->setCurrentWidget(dialog);*/

	}else if (nType == POWER_PREDICT_DEVICE_TYPE)
	{
		/*CPredictDevice* pDevice = new CPredictDevice(m_pCore);

		auto pView = new CPredictAiView(nullptr, pDevice, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = "";
		keyInfo.TagName = "";

		m_pUi->GetTabWidget()->AddTab(pView, "a", keyInfo, QIcon(QString(":/images/powerpredictitem.png")));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);*/

	}else if (nType == POWER_PREDICT_DEVICE_AI)
	{
		auto indexTmp = index;
		bool bFlag = false;
		
		auto strPredictTagName = QString("powerprediction");

		auto strTagName = index.data(Qt::EditRole).toString();

		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, PREDICT_WIDGET_TYPE::DEVICE_WIDGET_AI_TYPE, strPredictTagName);
		if (bFlag)
		{
			return;
		}

		auto pDevice = reinterpret_cast<CPredictDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			return;
		}

		auto pView = new CPredictAiView(nullptr, pDevice, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strPredictTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = PREDICT_WIDGET_TYPE::DEVICE_WIDGET_AI_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, QIcon(POWER_PREDICT_PNG));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);


	}else if (nType == POWER_PREDICT_DEVICE_DI)
	{
		auto indexTmp = index;
		bool bFlag = false;

		auto strPredictTagName = QString("powerprediction");

		auto strTagName = index.data(Qt::EditRole).toString();

		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, PREDICT_WIDGET_TYPE::DEVICE_WIDGET_DI_TYPE, strPredictTagName);

		if (bFlag)
		{
			return;
		}

		auto pDevice = reinterpret_cast<CPredictDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			return;
		}

		auto pView = new CPredictAiView(nullptr, pDevice, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strPredictTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = PREDICT_WIDGET_TYPE::DEVICE_WIDGET_DI_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, QIcon(POWER_PREDICT_PNG));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);

	}else if (nType == POWER_PREDICT_DEVICE_PATAM)
	{
		auto indexTmp = index;
		bool bFlag = false;

		auto strPredictTagName = QString("powerprediction");

		auto strTagName = index.data(Qt::EditRole).toString();

		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, PREDICT_WIDGET_TYPE::DEVICE_WIDGET_DI_TYPE, strPredictTagName);

		if (bFlag)
		{
			return;
		}

		auto pDevice = reinterpret_cast<CPredictDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			return;
		}

		auto pView = new CPredictParamView(nullptr, pDevice, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strPredictTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = PREDICT_WIDGET_TYPE::DEVICE_WIDGET_DI_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, QIcon(POWER_PREDICT_PNG));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);
	}
}

/*! \fn void CPredictTree::Slot_TreeItemChanged(QStandardItem *pItem)
*********************************************************************
** \brief CPredictTree::Slot_TreeItemChanged
** \details  树项更改
** \return void
** \author GuoHaijun
** \date 2017年2月24日
** \note
********************************************************************/
void CPredictTree::Slot_TreeItemChanged(QStandardItem *pItem)
{
	emit Signl_ItemChanged(pItem);
}


/*! \fn bool CPredictTree::LoadDataFromFile(const QModelIndex &index)
*********************************************************************
** \brief CPredictTree::LoadDataFromFile
** \details 从文件加载数据
** \return bool
** \author GuoHaijun
** \date 2017年2月14日
** \note
********************************************************************/
bool CPredictTree::LoadDataFromFile(const QModelIndex &index)
{
	CPredictItem* pPdtItem = reinterpret_cast<CPredictItem*>(index.data(PREDICT_ITEM_ROLE).toLongLong());
	if (nullptr != pPdtItem)
	{
		return true;
	}



	return true;
}


/*! \fn bool CPredictTree::ReadFile(QStandardItem *pItem)
*********************************************************
** \brief CPredictTree::ReadFile
** \details 读取文件
** \return bool
** \author GuoHaijun
** \date 2017年2月14日
** \note
********************************************************/
bool CPredictTree::ReadFile(const QString& strPathName, CPredictItem* pPdtItem)
{
	if (strPathName.isEmpty() || pPdtItem == nullptr)
	{
		return false;
	}

	QFile file(strPathName);
	if (!file.exists())
	{
		return false;
	}

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}

	QDomDocument  document;

	if (!document.setContent(&file))
	{
		file.close();
		return false;
	}
	
	QDomElement root = document.documentElement();
	if (root.tagName() != "powerprediction")
	{
		file.close();
		return false;
	}

	QDomNode node = root.firstChild();
	for (; !node.isNull(); node = node.nextSibling())
	{
		if (node.nodeName() == "o")
		{
			QDomElement domElm = node.toElement();
			if (domElm.isNull())
			{
				continue;
			}

			CItemDataInfo itemInfo;

			itemInfo.m_ID = domElm.attribute("id").toULong();
			itemInfo.m_strTagName = domElm.attribute("tagname");
			itemInfo.m_strDesc = domElm.attribute("desc");
			itemInfo.m_OccNo = domElm.attribute("occno").toULong();

			pPdtItem->m_vecTableInfo.push_back(itemInfo);
		}
	}

	file.close();
	return true;
}

