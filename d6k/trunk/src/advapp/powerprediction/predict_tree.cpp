#include "predict_tree.h"
#include "predict_define.h"
#include "predict_station_widget.h"
#include "predict_inverter_widget.h"
#include "predict_weather_widget.h"
#include "scadastudio/tabwidget.h"
#include "predict_station_table_widget.h"
#include "predict_weather_table_widget.h"
#include "predict_inverter_table_widget.h"
#include "predict_output_table_widget.h"

#include <QMenu>
#include <QStandardItem>
#include <QModelIndex>

/*! \fn CPredictTree::CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore)
******************************************************************************************************
** \brief   CPredictTree::CPredictTree
** \details 构造函数
** \return
** \author  GuoHaijun
** \date    2017年2月3日
** \note
*****************************************************************************************************/
CPredictTree::CPredictTree(CTreeWidget* parent, IMainWindow* pUi, IMainModuleInterface* pCore)
{
	Q_ASSERT(parent != nullptr);
	Q_ASSERT(pUi != nullptr);
	Q_ASSERT(pCore != nullptr);

	m_pTreeWidget = parent;
	m_pUi = pUi;
	m_pCore = pCore;
}


/*! \fn void CPredictTree::InitTreeModel()
******************************************
** \brief   CPredictTree::InitTreeModel
** \details 初始化树
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
*****************************************/
void CPredictTree::InitTreeModel()
{
	QObject::connect(GetTree(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ShowMouseRightButton(const QPoint&)));
	QObject::connect(GetTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Slot_Clicked(const QModelIndex &)));
	QObject::connect(GetTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(Slot_DoubleClicked(const QModelIndex &)));
	//QObject::connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(Slot_TreeItemChanged(QStandardItem *)));
}

/*! \fn void CPredictTree::ShowRootMenu(QModelIndex &index)
***********************************************************
** \brief   CPredictTree::ShowRootMenu
** \details 显示根菜单,可增加group、item
** \return  void
** \author  GuoHaijun
** \date    2017年2月24日
** \note
**********************************************************/
void CPredictTree::ShowRootMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//添加组
	QAction* pAddGroup = new QAction(GROUP_NAME, pMenu);
	pAddGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pAddGroup);

	//添加项
	QAction *pAddItem = new QAction(ADD_POWER_FREDICT_ITEM, pMenu);
	pAddItem->setIcon(QIcon(PREDICT_ITEM_PNG));
	pMenu->addAction(pAddItem);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pAddGroup)
	{
		AddGroup(index);
	}
	else  if (pSelectAction == pAddItem)
	{
		AddItem(index);
	}

	pMenu->deleteLater();
}


/*! \fn void CPredictTree::ShowGroupMenu(QModelIndex &index)
************************************************************
** \brief   CPredictTree::ShowGroupMenu
** \details 显示组菜单
** \return  void
** \author  GuoHaijun
** \date    2017年2月24日
** \note
***********************************************************/
void CPredictTree::ShowGroupMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//添加item
	QAction *pAddPredictItem = new QAction(ADD_POWER_FREDICT_ITEM, pMenu);
	pAddPredictItem->setIcon(QIcon(PREDICT_ITEM_PNG));
	pMenu->addAction(pAddPredictItem);

	//添加group
	QAction *pActionAddGroup = new QAction(GROUP_NAME, pMenu);
	pActionAddGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
	pMenu->addAction(pActionAddGroup);

	//改名
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
** \brief   CPredictTree::ShowItemMenu
** \details 显示项菜单
** \return  void
** \author  GuoHaijun
** \date    2017年3月30日
** \note
***********************************************************/
void CPredictTree::ShowItemMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//改名
	QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
	pActionRename->setIcon(QIcon(PREDICT_RENAME_PNG));
	pMenu->addAction(pActionRename);

	//删除item
	QAction *pActionDeleteItem = new QAction(DELETE_ITEM, pMenu);
	pActionDeleteItem->setIcon(QIcon(PREDICT_DELETE_ITEM_PNG));
	pMenu->addAction(pActionDeleteItem);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pActionRename)
	{
		m_pTreeWidget->edit(index);
	}
	else if (pSelectAction == pActionDeleteItem)
	{
		DeleteItem(index);
	}

	pMenu->deleteLater();
}



/*! \fn void CPredictTree::ShowInvterGrpMenu(QModelIndex &index)
************************************************************
** \brief   CPredictTree::ShowInvterGrpMenu
** \details 显示项菜单
** \return  void
** \author  GuoHaijun
** \date    2017年4月8日
** \note
***********************************************************/
void CPredictTree::ShowInvterGrpMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//新增逆变器
	QAction *pActionAddInverter = new QAction(ADD_INVERTER, pMenu);
	pActionAddInverter->setIcon(QIcon(PREDICT_ADD_INVERTER_PNG));
	pMenu->addAction(pActionAddInverter);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pActionAddInverter)
	{
		AddInverter(index);
	}

	pMenu->deleteLater();
}



/*! \fn void CPredictTree::ShowInvterMenu(QModelIndex &index)
************************************************************
** \brief   CPredictTree::ShowInvterMenu
** \details 显示项菜单
** \return  void
** \author  GuoHaijun
** \date    2017年4月8日
** \note
***********************************************************/
void CPredictTree::ShowInvterMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(this);

	//删除逆变器
	QAction *pActionDelInverter = new QAction(DELETE_ITEM, pMenu);
	pActionDelInverter->setIcon(QIcon(PREDICT_DELETE_ITEM_PNG));
	pMenu->addAction(pActionDelInverter);

	//改名
	QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
	pActionRename->setIcon(QIcon(PREDICT_RENAME_PNG));
	pMenu->addAction(pActionRename);

	QAction *pSelectAction = pMenu->exec(QCursor::pos());

	if (pSelectAction == pActionDelInverter)
	{
		DeleteInverter(index);
	}
	else if(pSelectAction == pActionRename)
	{
		m_pTreeWidget->edit(index);
	}

	pMenu->deleteLater();
}



/*! \fn void CPredictTree::AddGroup(QModelIndex &index)
*******************************************************
** \brief   CPredictTree::AddGroup
** \details 新增组
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
******************************************************/
void CPredictTree::AddGroup(QModelIndex &index)
{
	emit Signl_AddGroup(index);
}



/*! \fn void CPredictTree::DeleteGroup(QModelIndex &index)
**********************************************************
** \brief   CPredictTree::DeleteGroup
** \details 删除组
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
*********************************************************/
void CPredictTree::DeleteGroup(QModelIndex &index)
{
	emit Signl_DeleteGroup(index);
}



/*! \fn void CPredictTree::AddItem(QModelIndex &index)
******************************************************
** \brief   CPredictTree::AddItem
** \details 新增项
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
*****************************************************/
void CPredictTree::AddItem(QModelIndex &index)
{
	emit Signl_AddItem(index);
}



/*! \fn void CPredictTree::DeleteItem(QModelIndex &index)
*********************************************************
** \brief   CPredictTree::DeleteItem
** \details 删除项
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
********************************************************/
void CPredictTree::DeleteItem(QModelIndex &index)
{
	emit Signl_DeleteItem(index);
}




/*! \fn void CPredictTree::AddInverter(QModelIndex &index)
******************************************************
** \brief   CPredictTree::AddInverter
** \details 新增逆变器
** \return  void
** \author  GuoHaijun
** \date    2017年4月8日
** \note
*****************************************************/
void CPredictTree::AddInverter(QModelIndex &index)
{
	emit Signl_AddInterver(index);
}



/*! \fn void CPredictTree::DeleteInverte(QModelIndex &index)
*********************************************************
** \brief   CPredictTree::DeleteInverte
** \details 删除逆变器
** \return  void
** \author  GuoHaijun
** \date    2017年4月8日
** \note
********************************************************/
void CPredictTree::DeleteInverter(QModelIndex &index)
{
	emit Signl_DeleteInverter(index);
}



/*! \fn void CPredictTree::Slot_ShowMouseRightButton(const QPoint& pos)
***********************************************************************
** \brief   CPredictTree::Slot_ShowMouseRightButton
** \details 右击显示
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
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
	case POWER_PREDICT_ITEM_TYPE:
	{
		ShowItemMenu(indexSelect);
		break;
	}
	case POWER_PREDICT_INVERTER_GROUP_TYPE:
	{
		ShowInvterGrpMenu(indexSelect);
		break;
	}
	case POWER_PREDICT_INVERTER_TYPE:
	{
		ShowInvterMenu(indexSelect);
		break;
	}
	default:
		break;
	}

	return;
}



/*! \fn void CPredictTree::Slot_Clicked(const QModelIndex &index)
****************************************************************
** \brief   CPredictTree::Slot_Clicked
** \details 单击显示
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
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
		case POWER_PREDICT_ROOT_TYPE:
		{
			//单击展开根组
			GetTree()->expanded(index);
			break;
		}
		case POWER_PREDICT_GROUP_TYPE:
		{
			//单击展开组
			GetTree()->expanded(index);
			break;
		}
		case POWER_PREDICT_ITEM_TYPE:
		{

			break;
		}
		case POWER_PREDICT_STATION_TYPE:
		{
			

			break;
		}
		case POWER_PREDICT_INVERTER_GROUP_TYPE:
		{
			

			break;
		}
		case POWER_PREDICT_INVERTER_TYPE:
		{
			

			break;
		}
		case POWER_PREDICT_WEATHER_TYPE:
		{

			break;
		}
		case POWER_PREDICT_PREDICT_TYPE:
		{

			break;
		}
		default:
			break;
	}
}



/*! \fn void CPredictTree::Slot_DoubleClicked(const QModelIndex &index)
***********************************************************************
** \brief   CPredictTree::Slot_DoubleClicked
** \details 双击显示
** \return  void
** \author  GuoHaijun
** \date    2017年2月3日
** \note
**********************************************************************/
void CPredictTree::Slot_DoubleClicked(const QModelIndex &index)
{
	QModelIndex nIndex = index;

	int nType = nIndex.data(PREDICT_ROOT_ROLE).toInt();

	QString staName = nIndex.data(STATION_NAME_ROLE).toString();

	switch(nType)
	{
		case POWER_PREDICT_ITEM_TYPE:
		{
			break;
		}
		case POWER_PREDICT_STATION_TYPE:
		{
			//CPlantInfo* pItem = reinterpret_cast<CPlantInfo*>(index.data(POWER_PREDICT_STATION_TYPE).toLongLong());
			
			CPlantInfo* pItem = reinterpret_cast<CPlantInfo*>(index.data(POINTERDATA).toLongLong());

			CTabWidget* pTabWidget = m_pUi->GetTabWidget();
			Q_ASSERT(pTabWidget);

			CPredictStationWidget* pWidget = new CPredictStationWidget(pTabWidget, m_pCore);

			pWidget->AddToTableCtrl(pItem,staName);

			m_pUi->GetTabWidget()->setCurrentWidget(pWidget);			

			break;
		}
		case POWER_PREDICT_INVERTER_GROUP_TYPE:
		{
			GetTree()->expanded(index);
			
			break;
		}
		case POWER_PREDICT_INVERTER_TYPE:
		{
			//CInverterInfo* pItem = reinterpret_cast<CInverterInfo*>(index.data(POWER_PREDICT_WEATHER_TYPE).toLongLong());

			CInverterInfo* pItem = reinterpret_cast<CInverterInfo*>(index.data(POINTERDATA).toLongLong());

			CTabWidget* pTabWidget = m_pUi->GetTabWidget();
			Q_ASSERT(pTabWidget);

			CPredictInverterWidget* pWidget = new CPredictInverterWidget(pTabWidget, m_pCore);

			pWidget->AddToTableCtrl(pItem,staName);

			m_pUi->GetTabWidget()->setCurrentWidget(pWidget);

			break;
		}
		case POWER_PREDICT_WEATHER_TYPE:
		{
			//CWeatherData* pItem = reinterpret_cast<CWeatherData*>(index.data(POWER_PREDICT_WEATHER_TYPE).toLongLong());

			CWeatherData* pItem = reinterpret_cast<CWeatherData*>(index.data(POINTERDATA).toLongLong());

			CTabWidget* pTabWidget = m_pUi->GetTabWidget();
			Q_ASSERT(pTabWidget);

			CPredictWeatherWidget* pWidget = new CPredictWeatherWidget(pTabWidget, m_pCore);

			pWidget->AddToTableCtrl(pItem,staName);

			m_pUi->GetTabWidget()->setCurrentWidget(pWidget);

			break;
		}
		case POWER_PREDICT_PREDICT_TYPE:
		{
			//CPredictData* pItem = reinterpret_cast<CPredictData*>(index.data(POWER_PREDICT_PREDICT_TYPE).toLongLong());

			CPredictData* pItem = reinterpret_cast<CPredictData*>(index.data(POINTERDATA).toLongLong());

			CTabWidget* pTabWidget = m_pUi->GetTabWidget();
			Q_ASSERT(pTabWidget);

			CPredictOutputWidget* pWidget = new CPredictOutputWidget(pTabWidget, m_pCore);

			pWidget->AddToTableCtrl(pItem,staName);

			m_pUi->GetTabWidget()->setCurrentWidget(pWidget);

			break;
		}
		default:
			break;
	}

	return;
}



/*! \fn void CPredictTree::Slot_TreeItemChanged(QStandardItem *pItem)
*********************************************************************
** \brief   CPredictTree::Slot_TreeItemChanged
** \details 树项更改
** \return  void
** \author  GuoHaijun
** \date    2017年2月24日
** \note
********************************************************************/
/*
void CPredictTree::Slot_TreeItemChanged(QStandardItem *pItem)
{
	emit Signl_ItemChanged(pItem);
}*/



/*! \fn bool CPredictTree::LoadDataFromFile(const QModelIndex &index)
*********************************************************************
** \brief   CPredictTree::LoadDataFromFile
** \details 从文件加载数据
** \return  bool
** \author  GuoHaijun
** \date    2017年2月14日
** \note
********************************************************************/
bool CPredictTree::LoadDataFromFile(const QModelIndex &index)
{
	CStationData* pPdtItem = reinterpret_cast<CStationData*>(index.data(PREDICT_ITEM_ROLE).toLongLong());
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
bool CPredictTree::ReadFile(const QString& strPathName, CStationData* pPdtItem)
{
	Q_UNUSED(strPathName);
	Q_UNUSED(pPdtItem);

	return true;
}