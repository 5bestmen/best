/*! @file
<PRE>
********************************************************************************
模块名       :  变量
文件名       :  variable.cpp
文件实现功能 :  变量
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
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

#include "variable.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/define.h"
#include "systemvariableview.h"
#include "scadastudio/icore.h"
#include "scadastudio/tabwidget.h"
#include "config_data.h"
#include "uservariablview.h"


#include <QModelIndex>
#include <QMenu>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CVariable::CVariable(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CVariable::CVariable 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
CVariable::CVariable(IMainModuleInterface *pCore)
	: QObject(pCore->GetUIMgr())
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}

	m_pCore = pCore;
	
	m_pUi = pCore->GetUIMgr();

	connect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
}

/*! \fn CVariable::~CVariable()
********************************************************************************************************* 
** \brief CVariable::~CVariable 
** \details 
** \return  
** \author gw
** \date 2015年11月22日 
** \note 

********************************************************************************************************/
CVariable::~CVariable()
{

}

/*! \fn void CVariable::ShowMenu()
********************************************************************************************************* 
** \brief CVariable::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CVariable::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	if (index.data(Qt::UserRole).toInt() == FES_USER_VARIABLE_ITEM ||
		index.data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
	{


		if (index.data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
		{
			QAction *pActionGroup = new QAction(VARIABLE_GROUP_NAME, pMenu);
			pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionGroup);
			
			QAction *pActionMofidyGroup = new QAction(RENAME_ITEM, pMenu);
			pActionMofidyGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionMofidyGroup);

			QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
			pMenu->addAction(pActionExpand);

			QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
			pMenu->addAction(pActionCollapse);

			QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);

			QAction* selectedItem = pMenu->exec(QCursor::pos());
			if (selectedItem == pActionGroup)
			{
				//添加用户组
				AddUserGroup(index);
			}
			else if (selectedItem == pActionMofidyGroup)
			{
				m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
			}
			else if (selectedItem == pActionExpand)
			{
				ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
			}
			else if (selectedItem == pActionCollapse)
			{
				CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
			}
			else if (selectedItem == pActionDelete)
			{
				DeleteUserGroup(index);
			}

			pMenu->deleteLater();
		}
		else
		{
			QAction *pActionGroup = new QAction(VARIABLE_GROUP_NAME, pMenu);
			pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionGroup);
			
			QAction* selectedItem = pMenu->exec(QCursor::pos());
			if (selectedItem == pActionGroup)
			{
				//添加用户组
				AddUserGroup(index);
			}

			pMenu->deleteLater();
		}
	}
	//else if(index.data(Qt::UserRole).toInt() == FES_USER_VARIBALE_CHILD_ITEM)
	//{
	//	QAction *pModify = new QAction(RENAME_ITEM, pMenu);
	//	pMenu->addAction(pModify);

	//	QAction* selectedItem = pMenu->exec(QCursor::pos());
	//	if (selectedItem == pModify)
	//	{
	//		m_pUi->GetLeftTree()->edit(index);
	//	}
	//}

	pMenu->deleteLater();
}

/*! \fn void CVariable::AddUserGroup()
********************************************************************************************************* 
** \brief CVariable::AddUserGroup 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CVariable::AddUserGroup(QModelIndex &index)
{
	QModelIndex indexTmp = index;
	while (indexTmp.isValid())
	{
		if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
		{
			break;;
		}

		indexTmp = indexTmp.parent();
	}

	auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());

	Q_ASSERT(pFes);
	if (!pFes)
	{
		QString strError = QString(tr("can not find fes!!!"));
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return;
	}
	
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	auto pGroupParent = reinterpret_cast<Config::CUserVariableGroup *>(item->data(POINTERDATA).toLongLong());

	auto nDeepth = item->data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		nDeepth++;
		QString str = item->text();

		auto pTmp = pGroupParent->CreateGroup();
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		CQuoteItem *newItem = new CQuoteItem(pTmp->GetGroupName());
		newItem->setData(pTmp->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		newItem->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
		newItem->setData(pTmp->GetGroupName(), LASTVALUE);

		//Config::CUserVariableGroup *pUserContainer = new Config::CUserVariableGroup();
		newItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(index);
}


/*! \fn 
********************************************************************************************************* 
** \brief CVariable::DeleteUserGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年12月1日 
** \note 
********************************************************************************************************/
void CVariable::DeleteUserGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its children will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pGroupParent = reinterpret_cast<Config::CUserVariableGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		auto indexTmp = index;
		while (indexTmp.parent().data(Qt::UserRole).toInt() != FES_ITEM)
		{
			indexTmp = indexTmp.parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}

		////////////////////////删除前置用户变量组窗口//////////////////////////////
		QVector<QModelIndex> vec;
		vec.clear();
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_USER_VARIBALE_GROUP_ITEM, vec);
		vec.append(index);
		for each (auto var in vec)
		{
			//组名是拼接的 group0.group1
			auto strGroupName = GetGroupName(var);
			if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strGroupName, WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE, pFes->m_szTagName))
			{
				//删除打开的窗口
				int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
				//model view detach
				auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pIBase);
				if (!pIBase)
				{
					return;
				}

				pIBase->Delete();

				m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
			}

			//删除组下的item hash 不删除内存
			auto pGroup = reinterpret_cast<Config::CUserVariableGroup *>(var.data(POINTERDATA).toLongLong());
			for each (auto pItem in pGroup->m_arrItem)
			{
				//删除hash
				if (!pFes->DeleteFesHashItem(HASHID::USERVIRIABLEID, pItem->m_szTagName))
				{
					auto strError = QObject::tr("Fes %1 user variable item tagname %2 hash delete fail!!!").arg(pFes->m_szTagName).arg(pItem->m_szTagName);
					m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}

				//删除用户变量关联的和被关联的
				if (!pFes->DeleteFesUserVaraibleCombineRelaton(pItem->m_szTagName, pItem))
				{
					auto strError = tr("Delete FesUserVaraible %1 Combine Relaton failed!!!").arg(pItem->m_szTagName);
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return;
				}
			}
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pGroupParent->DeleteGroup(pGroup))
		{
			auto strError = tr("delete group fail!!!").arg(strTag);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//QList<QStandardItem *> carryItems; //Last In First Out stack of items
		//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		//while (pTop->rowCount())
		//{
		//	itemsToBeDeleted << pTop->takeRow(0);
		//	//if the row removed has children:
		//	if (itemsToBeDeleted.at(0)->hasChildren())
		//	{
		//		carryItems << pTop;                 //put on the stack the current loopItem
		//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
		//	}
		//	//if current loopItem has no more rows but carryItems list is not empty:
		//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

		//}
		//qDeleteAll(itemsToBeDeleted);

		pTop->parent()->removeRow(pTop->row());


		break;
	}
	case QMessageBox::Cancel:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}
}

/*! \fn void  CVariable::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CVariable::showMouseRightButton 
** \details 右键菜单
** \param point 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void  CVariable::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_USER_VARIABLE_ITEM || nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		ShowMenu(indexSelect);
	}
}

void CVariable::Disconnect()
{
	disconnect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
}

/*! \fn void CVariable::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CVariable::doubleClicked 
** \details 槽函数
** \param index 
** \return void 
** \author gw
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CVariable::DoubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	
	if (nType == FES_SYSTEM_VARIABLE_ITEM)
	{
		auto pFes = reinterpret_cast<Config::CFesData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("system variable pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}


		auto strTagName = index.data(Qt::EditRole).toString();
		bFlag = strTagName.isEmpty() || strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}


		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::VARIABLE_SYSTEM_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		CSystemVariableView *pView = new CSystemVariableView(nullptr, m_pCore, pFes);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::VARIABLE_SYSTEM_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab((QWidget *)pView, strTagName, keyInfo, icon);
	}
	else if (nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		//向上遍历查找前置项
		QModelIndex indexTmp = index;
		//auto nType = indexTmp.parent().data(Qt::UserRole).toInt();
		while (indexTmp.parent().data(Qt::UserRole).toInt() != FES_ITEM)
		{

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		
		//用户变量 root
		auto pVariable = reinterpret_cast<Config::CUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pVariable);
		if (!pVariable)
		{
			auto strError = tr("user variable pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		indexTmp = index;
		auto strTagName = /*index.data(Qt::EditRole).toString()*/GetGroupName(indexTmp);

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();
		CUserVariablView *pView = new CUserVariablView(nullptr, m_pCore, pFes, pVariable);
		
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE;
		
		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
	}
	//else if (nType == FES_TYPE_VARIABLE_GROUP_ITEM)
	//{

	//}
}

void CVariable::itemChanged(QStandardItem *item)
{
	int nType = item->data(Qt::UserRole).toInt();
	if (nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		auto strTagName = item->data(Qt::EditRole).toString();
		auto strLastTagName = item->data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		//向上遍历查找前置项
		QModelIndex index = m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item);
		QModelIndex indexTmp = index;
		//auto nType = indexTmp.parent().data(Qt::UserRole).toInt();
		while (indexTmp.parent().data(Qt::UserRole).toInt() != FES_ITEM)
		{
			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		//检查tagname是否存在
		if (pFes->CheckTagNameIsExist(strTagName))
		{
			auto strError = tr("tagname %1 is exist@!!!").arg(strTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		//用户组的父组
		auto pGroupParent = reinterpret_cast<Config::CUserVariableGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}


		auto pUserContainer = reinterpret_cast<Config::CUserVariableGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pUserContainer);
		if (!pUserContainer)
		{
			auto strError = tr("user variable pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}

		//检查修改的组名是否存在
		if (pGroupParent->CheckGroupNameExist(strTagName))
		{
			auto strError = tr("tagname %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(SCADAVARIABLE_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}


		QVector<QModelIndex> vec;
		GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_USER_VARIBALE_GROUP_ITEM, vec);
		vec.append(index);
		for each (auto var in vec)
		{
			//
			auto varTmp = var, varLastTmp = var;
			auto strGroupName = GetGroupName(var);
			auto strLastGroupTagName = GetLastGroupName(varLastTmp);

			//判断是否存在该widget 若存在,刷新
			auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strLastGroupTagName, WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE, strFesTagName);
			if (bFlag)
			{
				TabKeyStruct keyInfo;
				TabKeyStruct lastKeyInfo;
				keyInfo.BigTagName = strFesTagName;
				keyInfo.TagName = strGroupName;
				keyInfo.Type = WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE;
				lastKeyInfo.BigTagName = strFesTagName;
				lastKeyInfo.TagName = strLastGroupTagName;
				lastKeyInfo.Type = WIDGET_TYPE::VARIABLE_USER_GROUP_WIDGET_TYPE;

				m_pCore->GetUIMgr()->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}
		}

		pUserContainer->SetGroupName(strTagName);

		item->setData(strTagName, LASTVALUE);
		item->setData(strTagName, TREE_TOOL_TIP_ROLE);
	}
}

/*! \fn QString CVariable::GetGroupName()
********************************************************************************************************* 
** \brief CVariable::GetGroupName 
** \details 获得index组名 例如group0.group1
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2016年12月30日 
** \note 
********************************************************************************************************/
QString CVariable::GetGroupName(QModelIndex index)
{
	QStringList list;
	list.clear();

	auto strGroupTmp = index.data(Qt::EditRole).toString();
	list.append(strGroupTmp);
	while (index.parent().data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
	{
		index = index.parent();

		list.append(index.data(Qt::EditRole).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

/*! \fn QString CVariable::GetLastGroupName(QModelIndex &index)
********************************************************************************************************* 
** \brief CVariable::GetLastGroupName 
** \details 获得index的lastvalue组名
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2016年12月30日 
** \note 
********************************************************************************************************/
QString CVariable::GetLastGroupName(QModelIndex index)
{
	QStringList list;
	list.clear();
	auto strGroupTmp = index.data(LASTVALUE).toString();
	list.append(strGroupTmp);
	while (index.parent().data(Qt::UserRole).toInt() == FES_USER_VARIBALE_GROUP_ITEM)
	{
		index = index.parent();

		list.append(index.data(LASTVALUE).toString());
	}

	auto it = list.rbegin();
	strGroupTmp.clear();
	for (; it != list.rend(); ++it)
	{
		strGroupTmp += *it + ".";
	}

	if (strGroupTmp.contains("."))
	{
		//删除最后一个.
		strGroupTmp.chop(1);
	}

	return strGroupTmp;
}

/*! \fn void CVariable::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
********************************************************************************************************* 
** \brief CVariable::GetItemMap 
** \details 遍历指定类型项
** \param pModel 
** \param top 
** \param nType 
** \param indexVec 
** \return void 
** \author gw
** \date 2016年12月30日 
** \note 
********************************************************************************************************/
void CVariable::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
{
	QModelIndex Item;

	if (!top.isValid())
	{
		return;
	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const QString strTmp = Item.data(Qt::EditRole).toString();

			if (nType == Item.data(Qt::UserRole).toInt())
			{
				indexVec.append(Item);

				//qDebug() << s;
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			GetItemMap(pModel, Item, nType, indexVec);
		}
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CVariable::DeleteUserVaraibleCombineRelation 
** \details 
** \param pGroup 
** \return bool 
** \author gw
** \date 2017年6月8日 
** \note 
********************************************************************************************************/
bool CVariable::DeleteUserVaraibleCombineRelation(Config::CUserVariableGroup *pGroup,  Config::CFesData *pFesData)
{
	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return false;
	}

	for each (auto var in pGroup->m_arrItem)
	{
		if (!pFesData->DeleteFesUserVaraibleCombineRelaton(var->m_szTagName, var))
		{
			auto strError = tr("Delete FesUserVaraible %1 Combine Relaton failed!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pGroup->m_arrGroup)
	{
		if (!DeleteUserVaraibleCombineRelation(var, pFesData))
		{
			auto strError = tr("Delete FesUserVaraible Group %1 Combine Relaton failed!!!").arg(pGroup->m_strGroupName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}
