/*! @file
<PRE>
********************************************************************************
模块名       :  转发
文件名       :  forwarding.cpp
文件实现功能 :  转发
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
*  @date    2017.03.20
*/

#include "forwarding.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/define.h"
#include "systemvariableview.h"
#include "scadastudio/icore.h"
#include "scadastudio/tabwidget.h"
#include "config_data.h"
#include "forwaridng_channel_view.h"
#include "forwarding_ai_view.h"
#include "forwarding_diview.h"
#include "forwarding_doview.h"
#include "forwarding_aoview.h"
#include "forwarding_cgview.h"
#include "forwarding_kwhview.h"
#include "global.h"

#include <QModelIndex>
#include <QMenu>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CForwarding::CForwarding(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CForwarding::CForwarding 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
CForwarding::CForwarding(IMainModuleInterface *pCore)
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

/*! \fn CForwarding::~CForwarding()
********************************************************************************************************* 
** \brief CForwarding::~CForwarding 
** \details 
** \return  
** \author gw
** \date 2017.03.20 
** \note 

********************************************************************************************************/
CForwarding::~CForwarding()
{

}

/*! \fn void CForwarding::ShowMenu()
********************************************************************************************************* 
** \brief CForwarding::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void CForwarding::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);
	auto nType = index.data(Qt::UserRole).toInt();
	if (nType == FES_FORWARDING_CHANNEL_LIST || nType == FES_FORWARDING_CHANNEL_GROUP_ITEM)
	{
		if (nType == FES_FORWARDING_CHANNEL_GROUP_ITEM)
		{
			QAction *pActionGroup = new QAction(FES_FORWARDING_ADDCHANNELGROUP_NAME, pMenu);
			pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionGroup);

			QAction *pActionItem = new QAction(FES_FORWARDING_ADDCHANNELITEM_NAME, pMenu);
			pActionItem->setIcon(QIcon(CHANNEL_PNG));
			pMenu->addAction(pActionItem);

			QAction *pActionMofidyGroup = new QAction(RENAME_ITEM, pMenu);
			pActionMofidyGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionMofidyGroup);

			QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);

			QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
			pMenu->addAction(pActionExpand);

			QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
			pMenu->addAction(pActionCollapse);

			QAction* selectedItem = pMenu->exec(QCursor::pos());
			if (selectedItem == pActionGroup)
			{
				//添加用户组
				AddForwardingChannelGroup(index);
			}
			else if (selectedItem == pActionItem)
			{
				AddForwardingChannelItem(index);
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
				DeleteForwardingChannelGroup(index);
			}

			pMenu->deleteLater();
		}
		else
		{
			QAction *pActionGroup = new QAction(FES_FORWARDING_ADDCHANNELGROUP_NAME, pMenu);
			pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));
			pMenu->addAction(pActionGroup);
			
			QAction *pActionItem = new QAction(FES_FORWARDING_ADDCHANNELITEM_NAME, pMenu);
			pActionItem->setIcon(QIcon(CHANNEL_PNG));
			pMenu->addAction(pActionItem);

			QAction* selectedItem = pMenu->exec(QCursor::pos());
			if (selectedItem == pActionGroup)
			{
				//添加用户组
				AddForwardingChannelGroup(index);
			}
			else if (selectedItem == pActionItem)
			{
				AddForwardingChannelItem(index);
			}

			pMenu->deleteLater();
		}
	}
	else if (nType == FES_FORWARDING_CHANNEL_ITEM)
	{
		QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
		pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
		pMenu->addAction(pActionMofidy);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionMofidy)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		}
		else if (selectedItem == pActionDelete)
		{
			DeleteForwardingChannelItem(index);
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
	}

	pMenu->deleteLater();
}

/*! \fn void CForwarding::AddUserGroup()
********************************************************************************************************* 
** \brief CForwarding::AddUserGroup 
** \details 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void CForwarding::AddForwardingChannelGroup(QModelIndex &index)
{
	auto pFes = GetFes(index);
	Q_ASSERT(pFes);
	if (!pFes)
	{
		QString strError = QObject::tr("can not find fes!!!");
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
		
		return;
	}
	
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	auto pGroupParent = reinterpret_cast<Config::CForwardingChannelGroup *>(pItem->data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	auto nDeepth = pItem->data(GROUP_NUMBER_ROLE).toInt();
	if (nDeepth < MAX_GROUP_NUM)
	{
		nDeepth++;
		QString str = pItem->text();

		auto pTmp = pGroupParent->CreateGroup();
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		CQuoteItem *newItem = new CQuoteItem(pTmp->GetGroupName());
		newItem->setData(pTmp->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
		newItem->setData(FES_FORWARDING_CHANNEL_GROUP_ITEM, Qt::UserRole);
		newItem->setData(pTmp->GetGroupName(), LASTVALUE);

		//Config::CUserVariableGroup *pUserContainer = new Config::CUserVariableGroup();
		newItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(index);
}


void CForwarding::AddForwardingChannelItem(QModelIndex &index)
{
	//添加通道
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	//if (nType == FES_FORWARDING_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QObject::tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pGroup = reinterpret_cast<Config::CForwardingChannelGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		//添加新项到组
		auto pForwardingChannel = pFes->AddNewForwardingChannelItem(pGroup);
		Q_ASSERT(pForwardingChannel);
		if (!pForwardingChannel)
		{
			return;
		}

		//校验tagname是否有效
		if (!CheckTagNameIsValid(pForwardingChannel->m_szTagName, FES_DESC))
		{
			return;
		}

		CQuoteItem *newItem = new CQuoteItem(pForwardingChannel->m_szTagName);

		newItem->setData(FES_FORWARDING_CHANNEL_ITEM, Qt::UserRole);
		newItem->setData(pForwardingChannel->m_szTagName, LASTVALUE);
		//Tagname
		//newItem->setData(pChannel->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		newItem->setData(pForwardingChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(CHANNEL_PNG));
		pItem->appendRow(newItem);

		//遥测
		auto strTagNameAI = QString("%1_AI").arg(pForwardingChannel->m_szTagName);
		auto pAIItem = new CQuoteItem(strTagNameAI);
		pAIItem->setData(FES_FORWARDING_CHANNEL_AI_ITEM, Qt::UserRole);
		pAIItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pAIItem->setData(strTagNameAI, LASTVALUE);
		pAIItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pAIItem);

		//遥信
		auto strTagNameDI = QString("%1_DI").arg(pForwardingChannel->m_szTagName);
		auto pDIItem = new CQuoteItem(strTagNameDI);
		pDIItem->setData(FES_FORWARDING_CHANNEL_DI_ITEM, Qt::UserRole);
		pDIItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pDIItem->setData(strTagNameDI, LASTVALUE);
		pDIItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pDIItem);

		//遥控
		auto strTagNameDO = QString("%1_DO").arg(pForwardingChannel->m_szTagName);
		auto pDOItem = new CQuoteItem(strTagNameDO);
		pDOItem->setData(FES_FORWARDING_CHANNEL_DO_ITEM, Qt::UserRole);
		pDOItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pDOItem->setData(strTagNameDO, LASTVALUE);
		pDOItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pDOItem);
		
		//遥调
		auto strTagNameAO = QString("%1_AO").arg(pForwardingChannel->m_szTagName);
		auto pAOItem = new CQuoteItem(strTagNameAO);
		pAOItem->setData(FES_FORWARDING_CHANNEL_AO_ITEM, Qt::UserRole);
		pAOItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pAOItem->setData(strTagNameAO, LASTVALUE);
		pAOItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pAOItem);

		//调档
		auto strTagNameControlGear = QString("%1_CG").arg(pForwardingChannel->m_szTagName);
		auto pCGItem = new CQuoteItem(strTagNameControlGear);
		pCGItem->setData(FES_FORWARDING_CHANNEL_CG_ITEM, Qt::UserRole);
		pCGItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pCGItem->setData(strTagNameControlGear, LASTVALUE);
		pCGItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pCGItem);

		//KWH
		auto strTagNameKWH = QString("%1_KWH").arg(pForwardingChannel->m_szTagName);
		auto pKWHItem = new CQuoteItem(strTagNameKWH);
		pKWHItem->setData(FES_FORWARDING_CHANNEL_KWH_ITEM, Qt::UserRole);
		pKWHItem->setData(reinterpret_cast<long long>(pForwardingChannel), POINTERDATA);
		pKWHItem->setData(strTagNameKWH, LASTVALUE);
		pKWHItem->setIcon(QIcon(CHANNEL_PNG));
		newItem->appendRow(pKWHItem);

		m_pUi->GetLeftTree()->expand(index);
	}

}

void CForwarding::AddScale(QModelIndex &index)
{
	//添加通道
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

}

/*! \fn
********************************************************************************************************* 
** \brief CForwarding::DeleteUserGroup 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void CForwarding::DeleteForwardingChannelGroup(QModelIndex &index)
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

		auto pGroupParent = reinterpret_cast<Config::CForwardingChannelGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CForwardingChannelGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}

		QVector<QModelIndex> vec;
		vec.clear();
		//大组内的所有的通道项
		GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_ITEM, vec);
		for each (auto var in vec)
		{
			//遍历删除通道相关项
			if (!Delete_ForwardingChannelModule_WidgetInTabWidget(var, pFes))
			{
				auto strError = tr("delete forwarding channel item module widget fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
				
				return;
			}

			//删除hash管理通道项 不删除实际前置内存
			auto bFlag = pFes->DeleteFesHashItem(HASHID::FORWARDINGCHANNEL, var.data().toString());
			if (!bFlag)
			{
				auto strError = tr("delete forwarding channel item fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pGroupParent->DeletGroup(strTag))
		{
			auto strError = tr("delete group fail!!!");

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

/*! \fn void CForwarding::DeleteForwardingChannelItem(QModelIndex & index)
********************************************************************************************************* 
** \brief CForwarding::DeleteForwardingChannelItem 
** \details 删除转发通道 通道子项也删除
** \param index 
** \return void 
** \author gw
** \date 2017年4月9日 
** \note 
********************************************************************************************************/
void CForwarding::DeleteForwardingChannelItem(QModelIndex & index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the forwarding channel, the data will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the forwarding channel?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		auto pChannelGroup = reinterpret_cast<Config::CForwardingChannelGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			return;
		}

		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}

		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}

		if (!Delete_ForwardingChannelModule_WidgetInTabWidget(index, pFes))
		{
			auto strError = QObject::tr("delete Forwarding Channel Module widget fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		} 

		//Delete hash
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGCHANNEL, pChannel->m_szTagName))
		{
			auto strError = tr("delete forwarding channel hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		if (!pChannelGroup->DeleteItem(pChannel->m_szTagName))
		{
			auto strError = tr("delete forwarding channel fail!!!");
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

/*! \fn bool CForwarding::Delete_ForwardingChannelModule_WidgetInTabWidget(const QModelIndex & index, Config::CFesData * pFes)
********************************************************************************************************* 
** \brief CForwarding::Delete_ForwardingChannelModule_WidgetInTabWidget 
** \details 删除通道窗口和窗口的子项窗口，view和model必须先detach
			子项hash删除
** \param index 
** \param pFes 
** \return bool 
** \author gw
** \date 2017年4月9日 
** \note 
********************************************************************************************************/
bool CForwarding::Delete_ForwardingChannelModule_WidgetInTabWidget(const QModelIndex & index, Config::CFesData * pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	int nSize = strlen(pFes->m_szTagName);
	bool bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pChannel);
	if (!pChannel)
	{
		return false;
	}
	nSize = strlen(pChannel->m_szTagName);
	bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(pChannel->m_szTagName, WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE, pFes->m_szTagName))
	{
		//删除打开的前置窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIBase);
		if (!pIBase)
		{
			return false;
		}

		pIBase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	QVector<QModelIndex> vec;
	vec.clear();
	//AI
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_AI_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//DI
	vec.clear();
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_DI_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//AO
	vec.clear();
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_AO_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//CG
	vec.clear();
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_CG_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//DO
	vec.clear();
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_DO_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}

	//KWH
	vec.clear();
	GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_FORWARDING_CHANNEL_KWH_ITEM, vec);
	for each (auto var in vec)
	{
		auto strTagName = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
			int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
			//model view detach
			auto pIBase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
			Q_ASSERT(pIBase);
			if (!pIBase)
			{
				return false;
			}

			pIBase->Delete();

			m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
		}
	}


	//删除hash管理 ais
	for each (auto var in pChannel->m_arrAIs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGAIHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding aie hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//删除转发AI关联的关系
		if (!pFes->DeleteFesForwardingAICombineRelaton(var->m_szTagName, var))
		{
			auto strError = QObject::tr("Delete Fes Forwarding AI %1 Combine Relaton failed!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pChannel->m_arrDIs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGDIHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding di hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		if (!pFes->DeleteFesForwardingDICombineRelaton(var->m_szTagName, var))
		{
			auto strError = QObject::tr("Delete Fes Forwarding DI %1 Combine Relaton failed!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pChannel->m_arrAOs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGAOHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding ao hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		if (!pFes->DeleteFesForwardingAOCombineRelaton(var->m_szTagName, var))
		{
			auto strError = QObject::tr("Delete Fes Forwarding DI %1 Combine Relaton failed!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pChannel->m_arrDOs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGDOHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding do hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		if (!pFes->DeleteFesForwardingDOCombineRelaton(var->m_szTagName, var))
		{
			auto strError = QObject::tr("Delete Fes Forwarding DO %1 Combine Relaton failed!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pChannel->m_arrKWHs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGKWHHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding kwh hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	for each (auto var in pChannel->m_arrCGs)
	{
		if (!pFes->DeleteFesHashItem(HASHID::FORWARDINGCGHASHID, var->m_szTagName))
		{
			auto strError = tr("delete forwarding CG hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
	}

	return true;
}

/*! \fn void  CForwarding::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CForwarding::showMouseRightButton 
** \details 右键菜单
** \param point 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void  CForwarding::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_FORWARDING_CHANNEL_LIST || nType == FES_FORWARDING_CHANNEL_GROUP_ITEM || nType == FES_FORWARDING_CHANNEL_ITEM)
	{
		ShowMenu(indexSelect);
	}
}

void CForwarding::Disconnect()
{
	disconnect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
}

/*! \fn void CForwarding::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CForwarding::doubleClicked 
** \details 槽函数
** \param index 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void CForwarding::DoubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	
	if (nType == FES_FORWARDING_CHANNEL_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}


		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(pChannel->m_szTagName, WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();
		auto pItem = static_cast<CQuoteItem *>(m_pUi->GetLeftTreeModel()->itemFromIndex(index));
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		CForwardingChannelView *pView = new CForwardingChannelView(nullptr, pChannel, pItem, pFes, m_pCore);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = pChannel->m_szTagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab((QWidget *)pView, pChannel->m_szTagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_AI_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		//判断是否存在该widget
		auto strForwardingAITagName = QString("%1_AI").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingAITagName, WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		CForwardingAIView *pView = new CForwardingAIView(nullptr, m_pCore, pChannel, pFes);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingAITagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingAITagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_DI_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strForwardingDITagName = QString("%1_DI").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingDITagName, WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}
		CForwardingDIView *pView = new CForwardingDIView(nullptr, m_pCore, pChannel, pFes);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingDITagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingDITagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_DO_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strForwardingDOTagName = QString("%1_DO").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingDOTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		CForwardingDOView *pView = new CForwardingDOView(NULL, m_pCore, pChannel, pFes);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingDOTagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingDOTagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_AO_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strForwardingAOTagName = QString("%1_AO").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingAOTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		CForwardingAOView *pView = new CForwardingAOView(NULL, m_pCore, pChannel, pFes);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingAOTagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingAOTagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_KWH_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strForwardingKWHTagName = QString("%1_KWH").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingKWHTagName, WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		CForwardingKWHView *pView = new CForwardingKWHView(NULL, m_pCore, pChannel, pFes);
		
		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingKWHTagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingKWHTagName, keyInfo, icon);
	}
	else if (nType == FES_FORWARDING_CHANNEL_CG_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断前置tagname长度
		int nSize = (int)strlen(pFes->m_szTagName);
		bool bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		////判断转发通道tagname长度
		auto pChannel = reinterpret_cast<Config::CForwardingChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}
		nSize = (int)strlen(pChannel->m_szTagName);
		bFlag = nSize == 0 || nSize > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strForwardingCGTagName = QString("%1_CG").arg(pChannel->m_szTagName);
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strForwardingCGTagName, WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}

		CForwardingCGView *pView = new CForwardingCGView(NULL, m_pCore, pChannel, pFes);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = pFes->m_szTagName;
		keyInfo.TagName = strForwardingCGTagName;
		keyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strForwardingCGTagName, keyInfo, icon);
	}
}

void CForwarding::itemChanged(QStandardItem *item)
{
	int nType = item->data(Qt::UserRole).toInt();
	if (nType == FES_FORWARDING_CHANNEL_ITEM)
	{
		auto strTagName = item->data(Qt::EditRole).toString();
		auto strLastTagName = item->data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		//判断tagname和last tagname字符串长度是否有效
		bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC) && CheckTagNameIsValid(strTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("Forwarding channel tagname is invalid!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		//向上遍历查找前置项
		auto pFes = GetFes(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item));
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		int nSize = (int)strlen(pFes->m_szTagName);
		bFlag = CheckTagNameIsValid(pFes->m_szTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QObject::tr("Fes tagname is invalid!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

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

		//转发通道的父组
		auto pParent = reinterpret_cast<Config::CForwardingChannelGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pParent);
		if (!pParent)
		{
			return;
		}


		auto pChannelItem = reinterpret_cast<Config::CForwardingChannelData *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelItem);
		if (!pChannelItem)
		{
			auto strError = tr("Forwarding channel pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}

		//前置内的hash 转发通道 修改key
		if (!pFes->ModifyFesHashMap(FORWARDINGCHANNEL, strTagName, strLastTagName))
		{
			auto strError = tr("Forwarding channel item modify hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		//修改组内的item
		if (!pParent->ModifyItem(strTagName, strLastTagName))
		{
			auto strError = tr("Forwarding channel item %1 modify fail!!!").arg(strLastTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			TabKeyStruct newKeyInfo;
			TabKeyStruct lastKeyInfo;
			newKeyInfo.BigTagName = pFes->m_szTagName;
			newKeyInfo.TagName = strTagName;
			newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE;
			lastKeyInfo.BigTagName = pFes->m_szTagName;
			lastKeyInfo.TagName = strLastTagName;
			lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_WIDGET_TYPE;

			m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}
		item->setData(strTagName, LASTVALUE);
		item->setData(strTagName, TREE_TOOL_TIP_ROLE);

		//通道修改，通道子项也修改
		ModifyForwardingChannelChildWidget(item, strTagName, strLastTagName, pFes->m_szTagName);
	}
}

/*! \fn QString CForwarding::GetGroupName()
********************************************************************************************************* 
** \brief CForwarding::GetGroupName 
** \details 获得index组名 例如group0.group1
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
QString CForwarding::GetGroupName(QModelIndex &index)
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

/*! \fn QString CForwarding::GetLastGroupName(QModelIndex &index)
********************************************************************************************************* 
** \brief CForwarding::GetLastGroupName 
** \details 获得index的lastvalue组名
** \param index 
** \return QT_NAMESPACE::QString 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
QString CForwarding::GetLastGroupName(QModelIndex &index)
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

/*! \fn void CForwarding::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
********************************************************************************************************* 
** \brief CForwarding::GetItemMap 
** \details 遍历指定类型项
** \param pModel 
** \param top 
** \param nType 
** \param indexVec 
** \return void 
** \author gw
** \date 2017.03.20 
** \note 
********************************************************************************************************/
void CForwarding::GetItemMap(const QAbstractItemModel *pModel, const QModelIndex &top, int nType, QVector<QModelIndex> &indexVec)
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

/*! \fn Config::CFesData * CForwarding::GetFes()
********************************************************************************************************* 
** \brief CForwarding::GetFes 
** \details 
** \return Config::CFesData * 
** \author gw
** \date 2017年3月20日 
** \note 
********************************************************************************************************/
Config::CFesData * CForwarding::GetFes(const QModelIndex &index)
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

		return nullptr;
	}
	QString strFesTagName = pFes->m_szTagName;
	auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return nullptr;
	}

	return pFes;
}

bool CForwarding::ModifyForwardingChannelChildWidget(QStandardItem *item, const QString & strChannelTagName, const QString & strChannelLastTagName, const QString & strFesTagName)
{	
	//转发通道index
	auto pModel = m_pUi->GetLeftTreeModel();
	QModelIndex index = m_pUi->GetLeftTreeModel()->indexFromItem(item);
	auto nRow = pModel->rowCount(index);

	for (int i = 0; i < nRow; i++)
	{
		auto childIndex = pModel->index(i, 0, index);
		int nType = childIndex.data(Qt::UserRole).toInt();
		if (nType == FES_FORWARDING_CHANNEL_AI_ITEM)
		{
			{
				/////////////////////////////ai//////////////////////////////////////////
				auto pAiItem = pModel->itemFromIndex(childIndex);
				QString strChannel_AITagName = QString("%1_AI").arg(strChannelTagName);
				auto strChannel_LastAITagName = childIndex.data(LASTVALUE).toString();
				auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strChannel_LastAITagName, WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_AITagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastAITagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AI_WIDGET_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pAiItem->setData(strChannel_AITagName, Qt::EditRole);
				pAiItem->setData(strChannel_AITagName, LASTVALUE);
			}
		}
		else if (nType == FES_FORWARDING_CHANNEL_DI_ITEM)
		{
			{
				auto pDiItem = pModel->itemFromIndex(childIndex);
				QString strChannel_DITagName = QString("%1_DI").arg(strChannelTagName);
				QString test = pDiItem->text();
				auto strChannel_LastDITagName = pDiItem->data(LASTVALUE).toString();
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannel_LastDITagName, WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_DITagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastDITagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DI_WIDGET_TYPE;
					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pDiItem->setData(strChannel_DITagName, Qt::EditRole);
				pDiItem->setData(strChannel_DITagName, LASTVALUE);
			}
		}
		else if (nType == FES_FORWARDING_CHANNEL_AO_ITEM)
		{
			//AO
			{
				auto pAoItem = pModel->itemFromIndex(childIndex);
				QString strChannel_AOTagName = QString("%1_AO").arg(strChannelTagName);

				QString test = pAoItem->text();

				auto strChannel_LastAOTagName = pAoItem->data(LASTVALUE).toString();
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannel_LastAOTagName, WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_AOTagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastAOTagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_AO_WIDGET_TYPE;
					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pAoItem->setData(strChannel_AOTagName, Qt::EditRole);
				pAoItem->setData(strChannel_AOTagName, LASTVALUE);
			}
		}
		else if (nType == FES_FORWARDING_CHANNEL_CG_ITEM)
		{
			//CG
			{
				auto pCgItem = pModel->itemFromIndex(childIndex);
				QString test = pCgItem->text();

				QString strChannel_CGTagName = QString("%1_CG").arg(strChannelTagName);
				auto strChannel_LastCGTagName = pCgItem->data(LASTVALUE).toString();
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannel_LastCGTagName, WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_CGTagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastCGTagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_CG_WIDGET_TYPE;
					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pCgItem->setData(strChannel_CGTagName, Qt::EditRole);
				pCgItem->setData(strChannel_CGTagName, LASTVALUE);
			}
		}
		else if (nType == FES_FORWARDING_CHANNEL_DO_ITEM)
		{
			//DO
			{
				auto pDoItem = pModel->itemFromIndex(childIndex);
				QString strChannel_DOTagName = QString("%1_DO").arg(strChannelTagName);

				QString test = pDoItem->text();

				auto strChannel_LastDOTagName = pDoItem->data(LASTVALUE).toString();
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannel_LastDOTagName, WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_DOTagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastDOTagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_DO_WIDGET_TYPE;
					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pDoItem->setData(strChannel_DOTagName, Qt::EditRole);
				pDoItem->setData(strChannel_DOTagName, LASTVALUE);
			}
		}
		else if (nType == FES_FORWARDING_CHANNEL_KWH_ITEM)
		{
			//KWH
			{
				auto pKwhItem = pModel->itemFromIndex(childIndex);
				QString strChannel_KWHTagName = QString("%1_KWH").arg(strChannelTagName);

				QString test = pKwhItem->text();

				auto strChannel_LastKWHTagName = pKwhItem->data(LASTVALUE).toString();
				auto bFlag = m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strChannel_LastKWHTagName, WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strChannel_KWHTagName;
					newKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strChannel_LastKWHTagName;
					lastKeyInfo.Type = WIDGET_TYPE::FORWARDING_CHANNEL_KWH_WIDGET_TYPE;
					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return false;
					}

					pBase->Refresh();
				}
				pKwhItem->setData(strChannel_KWHTagName, Qt::EditRole);
				pKwhItem->setData(strChannel_KWHTagName, LASTVALUE);
			}
		}
	}

	return false;
}
