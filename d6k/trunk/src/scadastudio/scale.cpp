#include "scale.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/imainwindow.h"
#include "fesmodule.h"
#include "scaleview.h"
#include "scadastudio/tabwidget.h"
#include "scalemodel.h"
#include "attriwindow/attriwindow.h"
#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qteditorfactory.h"
#include "scadastudio/icore.h"
#include "scadastudio/define.h"
#include "global.h"

#include <QModelIndex>
#include <QMenu>
#include <QMessageBox>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CSacle::CSacle(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CSacle::CSacle 
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
CScale::CScale(IMainModuleInterface *pCore)
	: QObject(pCore->GetUIMgr())
{
	m_pUi = pCore->GetUIMgr();

	connect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	connect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(Expanded(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(Collapsed(const QModelIndex &)));
	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	m_pCore = pCore;
}
  
/*! \fn CSacle::~CSacle()
********************************************************************************************************* 
** \brief CSacle::~CSacle 
** \details 
** \return  
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
CScale::~CScale()
{

}

/*! \fn void CSacle::AddScaleGroup()
********************************************************************************************************* 
** \brief CSacle::AddScale 
** \details 
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CScale::AddScaleGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(SCALE_GROUP_NAME).arg(0);

	auto nType = index.data(Qt::UserRole).toInt();
	//组深度
	auto nDeepth = index.data(GROUP_NUMBER_ROLE).toInt();
	if (nType == FES_SCALE_ITEM_LINEAR || nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM)
	{
		//深度限制
		if (nDeepth < MAX_GROUP_NUM)
		{
			//线性组
			auto pLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pLinearGroup);
			if (!pLinearGroup)
			{
				return;
			}

			auto pGroupChild = pLinearGroup->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}


			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setData(FES_TYPE_SCALE_LINEAR_GROUP_ITEM, Qt::UserRole);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
	else if (nType == FES_SCALE_ITEM_NONLINEAR || nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		//深度限制
		if (nDeepth < MAX_GROUP_NUM)
		{
			//非线性组
			auto pNonLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pNonLinearGroup);
			if (!pNonLinearGroup)
			{
				return;
			}

			auto pGroupChild = pNonLinearGroup->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}


			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setData(FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM, Qt::UserRole);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
	else if (nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM)
	{

	}
}

/*! \fn void CSacle::ShowMenu()
********************************************************************************************************* 
** \brief CSacle::AddScaleGroup 
** \details 
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CScale::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	if (index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM_LINEAR ||
		index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM_NONLINEAR ||
		index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_LINEAR_GROUP_ITEM ||
		index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		QAction *pActionAlarm = new QAction(SCALE_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(SCALE_GROUP_NAME, pMenu);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_LINEAR_GROUP_ITEM ||
			index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
		{
			//重命名
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			//删除
			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}
		

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//添加转换
			AddScale(index);
		}
		else if(selectedItem == pActionGroup)
		{
			//添加转换组
			AddScaleGroup(index);
		}
		else if (selectedItem == pActionRename)
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
			if (index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_LINEAR_GROUP_ITEM ||
				index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
			{
				//删除组 包括它的子项
				DeleteGroup(index);
			}
			else if (index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM_LINEAR ||
				index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM_NONLINEAR)
			{
				DeleteItem(index);
			}
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_LINEAR_CHILD_ITEM ||
		index.data(Qt::UserRole).toInt() == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
	{
		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);
		

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		//删除
		auto pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionRename)
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
			DeleteItem(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_SCALE_ITEM)
	{
		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionExpand)
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

/*! \fn void CSacle::AddScale()
********************************************************************************************************* 
** \brief CSacle::AddScale 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CScale::AddScale(QModelIndex &index)
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	Q_ASSERT(item);
	if (!item)
	{
		return;
	}

	auto nType = index.data(Qt::UserRole).toInt();
	if (nType == FES_SCALE_ITEM_LINEAR || nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM)
	{
		//线性组
		auto indexTmp = index;
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		auto pLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pLinearGroup);
		if (!pLinearGroup)
		{
			return;
		}

		auto pLinear = pFes->AddNewLinearTransform();
		Q_ASSERT(pLinear);
		if (!pLinear)
		{
			return;
		}

		pLinearGroup->AddTransformItem(pLinear);

		CQuoteItem *newItem = new CQuoteItem(pLinear->m_pLinearScale->m_szTagName);
		newItem->setData(pLinear->m_pLinearScale->m_szTagName, LASTVALUE);
		newItem->setData(FES_TYPE_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
		//newItem->setData(pTmp->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pLinear), POINTERDATA);
		newItem->setData(pLinear->m_pLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);

		newItem->setIcon(QIcon(SCALE_PNG));

		item->appendRow(newItem);

		m_pUi->GetLeftTree()->expand(index);


	}
	else if (nType == FES_SCALE_ITEM_NONLINEAR || nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		auto indexTmp = index;
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pNonLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pNonLinearGroup);
		if (!pNonLinearGroup)
		{
			return;
		}

		auto pNonLinear = pFes->AddNewNonLinearTransform();
		Q_ASSERT(pNonLinear);
		if (!pNonLinear)
		{
			return;
		}

		pNonLinearGroup->AddTransformItem(pNonLinear);
		
		Q_ASSERT(pNonLinear->m_pNonLinearScale);
		if (!pNonLinear->m_pNonLinearScale)
		{

		}
		CQuoteItem *newItem = new CQuoteItem(pNonLinear->m_pNonLinearScale->m_szTagName);
		newItem->setData(pNonLinear->m_pNonLinearScale->m_szTagName, LASTVALUE);
		newItem->setData(FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM, Qt::UserRole);
		//newItem->setData(pTmp->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pNonLinear), POINTERDATA);
		newItem->setData(pNonLinear->m_pNonLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);

		newItem->setIcon(QIcon(SCALE_PNG));

		item->appendRow(newItem);

		m_pUi->GetLeftTree()->expand(index);


	}






	//QString strTransformTag = QString(tr("Transform tag%1")).arg(pFes->m_arrTransforms.size());
	//QString strTransformDesc = QString(tr("Transform desc%2")).arg(pFes->m_arrTransforms.size());
	//auto bFalg = false;

	//auto pTransform = new Config::CTransform();
	//pTransform->m_nType = LINEAR;
	//memset(pTransform->m_szTagName, 0, sizeof(pTransform->m_szTagName));
	//strncpy(pTransform->m_szTagName, strTransformTag.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, strTransformTag.size()));

	//auto pTmp = new Config::CLinearTransform();
	//pTmp->m_strDescription = strTransformDesc;
	//memset(pTmp->m_szTagName, 0, sizeof(pTmp->m_szTagName));
	//int nSize = strTransformTag.size();
	//strncpy(pTmp->m_szTagName, strTransformTag.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
	//pTransform->SetLinear(pTmp);

	//pFes->m_arrTransforms.push_back(pTransform);


	//CQuoteItem *newItem = new CQuoteItem(pTmp->m_szTagName);
	//newItem->setData(pTmp->m_strDescription, LASTVALUE);
	//newItem->setData(FES_TYPE_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
	////newItem->setData(pTmp->m_szTagName, TAGNAME);
	//newItem->setData(reinterpret_cast<long long>(pTransform), POINTERDATA);
	//newItem->setData(pTmp->m_strDescription, TREE_TOOL_TIP_ROLE);

	//newItem->setIcon(QIcon(SCALE_PNG));

	//item->appendRow(newItem);

	//m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CSacle::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CSacle::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CScale::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_SCALE_ITEM ||
		nType == FES_SCALE_ITEM_LINEAR || 
		nType == FES_SCALE_ITEM_NONLINEAR ||
		nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM ||
		nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM ||
		nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM ||
		nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
	{
		ShowMenu(indexSelect);
	}
}

/*! \fn void CScale::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::doubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年4月13日 
** \note 
********************************************************************************************************/
void CScale::doubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
	{
		//int nType = index.data(Qt::UserRole).toInt();
		auto strScaleTagName = index.data(Qt::EditRole).toString();

		auto pTransform = reinterpret_cast<Config::CTransform *>(index.data(POINTERDATA).toLongLong());

		Q_ASSERT(pTransform);

		if (!pTransform)
		{
			return;
		}

		auto indexTmp = index;
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
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


		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strScaleTagName, WIDGET_TYPE::SCALE_WIDGET_TYPE, pFes->m_szTagName);
		if (bFlag)
		{
			return;
		}
		m_pUi->GetTabWidget()->m_fnBeforeCloseTab = std::bind(&CScale::CallBackBeforeCloseTabScale, this, std::placeholders::_1);

		auto pItem = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		CScaleView *pView = new CScaleView(m_pUi, pTransform, (CQuoteItem *)pItem, this, pFes);
		auto icon = pItem->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strScaleTagName;
		keyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;

		//m_pUi->GetTabWidget()->addTab(pView, index.data().toString());
		m_pUi->GetTabWidget()->AddTab(pView, strScaleTagName, keyInfo, icon);
	}
	else if (nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
	{
		//int nType = index.data(Qt::UserRole).toInt();

		auto strScaleTagName = index.data(Qt::EditRole).toString();

		m_pUi->GetTabWidget()->m_fnBeforeCloseTab = std::bind(&CScale::CallBackBeforeCloseTabScale, this, std::placeholders::_1);

		auto pTransform = reinterpret_cast<Config::CTransform *>(index.data(POINTERDATA).toLongLong());

		Q_ASSERT(pTransform);

		if (!pTransform)
		{
			return;
		}

		auto indexTmp = index;
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
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

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strScaleTagName, SCALE_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}


		auto pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		CScaleView *pView = new CScaleView(m_pUi, pTransform, pItem, this, pFes);
		
		auto &icon = pItem->icon();

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strScaleTagName;
		keyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;
		m_pUi->GetTabWidget()->AddTab(pView, strScaleTagName, keyInfo, icon);
	}
}

/*! \fn void CScale::itemChanged(QStandardItem *item)
********************************************************************************************************* 
** \brief CScale::itemChanged 
** \details 修改操作
** \param item 
** \return void 
** \author gw
** \date 2016年8月1日 
** \note 
********************************************************************************************************/
void CScale::itemChanged(QStandardItem *item)
{
	Q_ASSERT(m_pCore);
	if (!m_pCore)
	{
		return;
	}
	int nType = item->data(Qt::UserRole).toInt();
	QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();


	if (nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM || nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		auto strLastTagValue = item->data(LASTVALUE).toString();
		auto strScaleTagName = item->data(Qt::EditRole).toString();
		if (strLastTagValue == strScaleTagName)
		{
			return;
		}

		auto indexTmp = pModel->indexFromItem(item);
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

	
		auto index = pModel->indexFromItem(item);

		//父组
		auto pGroupParent = reinterpret_cast<Config::CTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}


		//子组
		auto pGroupChild = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupChild);
		if (!pGroupChild)
		{
			return;
		}

		//检查父组下的子组是否存在该组名
		if (pGroupParent->CheckModifyGroupNameExist(pGroupChild, strScaleTagName))
		{
			auto strError = QString(tr("transform group name %1 exist!!!")).arg(strScaleTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			item->setData(strLastTagValue, Qt::EditRole);

			return;
		}

		pGroupChild->SetGroupName(strScaleTagName);
		item->setData(strScaleTagName, LASTVALUE);
		item->setData(strScaleTagName, TREE_TOOL_TIP_ROLE);

		////修改分组,view的数据跟着修改
		//QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();

		//QVector<QModelIndex> vec;
		//vec.clear();

		////转换组组修改,遍历查找前置项
		////线性转换
		//CFesModule::GetItemMap(pModel, pModel->indexFromItem(item), FES_TYPE_SCALE_LINEAR_CHILD_ITEM, vec);
		//for (auto &tmp : vec)
		//{
		//	QString strGroupName = CFesModule::GetGroupName(tmp, FES_SCALE_ITEM);
		//
		//	auto pTransform = reinterpret_cast<Config::CTransform *>(tmp.data(POINTERDATA).toLongLong());
		//	if (!pTransform)
		//	{
		//		auto strError = QString("transform pointer is null!!!");

		//		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		//		return;
		//	}
		//	auto pLinear = pTransform->GetLinear();
		//	Q_ASSERT(pLinear);
		//	if (!pLinear)
		//	{
		//		auto strError = QString("can not find linear");
		//		
		//		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		//		return;
		//	}

		//	pLinear->m_strGroupName = strGroupName;
		//	
		//	this->blockSignals(true);
		//	item->setData(strScaleTagName, LASTVALUE);
		//	this->blockSignals(false);		
		//}
		////非线性转换
		//vec.clear();
		//CFesModule::GetItemMap(pModel, pModel->indexFromItem(item), FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM, vec);
		//for (auto tmp : vec)
		//{
		//	//auto strTagName = CFesModule::GetTagname(pModel->indexFromItem(item), FES_ITEM);

		//	//auto pTmp = CFesModule::GetFes(m_arrFes, strTagName);
		//	//Q_ASSERT(pTmp);
		//	//if (pTmp == nullptr)
		//	//{
		//	//	m_pCore->LogMsg(FES_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);

		//	//	return;
		//	//}

		//	auto strGroupName = CFesModule::GetGroupName(tmp, FES_SCALE_ITEM);

		//	auto pTransform = reinterpret_cast<Config::CTransform *>(tmp.data(POINTERDATA).toLongLong());
		//	if (!pTransform)
		//	{
		//		auto strError = QString("transform pointer is null!!!");

		//		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		//		return;
		//	}
		//	auto pNonLinear = pTransform->GetNonLinear();
		//	if (!pNonLinear)
		//	{
		//		auto strError = QString("can not find nonlinear tagname");

		//		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

		//		return;
		//	}

		//	pNonLinear->m_strGroupName = strGroupName;
		//	item->setData(strScaleTagName, LASTVALUE);
		//}


	}
	else if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM || nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
	{
		auto strLastTagValue = item->data(LASTVALUE).toString();
		auto strTagValue = item->data(Qt::EditRole).toString();
		if (strLastTagValue == strTagValue)
		{
			return;
		}

		//校验tagname是否有效
		if (!CheckTagNameIsValid(strTagValue, FES_DESC) || !CheckTagNameIsValid(strLastTagValue, FES_DESC))
		{
			return;
		}

		auto indexTmp = pModel->indexFromItem(item);
		while (indexTmp.isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole).toInt() == FES_ITEM)
			{
				break;
			}

			indexTmp = indexTmp.parent();
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = QString(tr("fes can find!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = CheckTagNameIsValid(strFesTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			auto strError = QString(tr("fes tagname %1 size is invalid!!!")).arg(strFesTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}


		//整个前置检查tagname是否存在
		if (pFes->CheckTagNameIsExist(strTagValue))
		{
			auto strError = QString(tr("transform name %1 exist!!!")).arg(strTagValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			item->setData(strLastTagValue, Qt::EditRole);

			return;
		}

		auto pWidget = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		if (pWidget)
		{
			pWidget->Refresh();
		}

		//QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();
		//auto strTagName = CFesModule::GetTagname(pModel->indexFromItem(item), FES_ITEM);

		/*auto pTmp = CFesModule::GetFes(m_arrFes, strTagName);
		Q_ASSERT(pTmp);
		if (pTmp == nullptr)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);

			return;
		}*/
		
		if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
		{
			auto pTransform = reinterpret_cast<Config::CTransform *>(item->data(POINTERDATA).toLongLong());
			if (!pTransform)
			{
				auto strError = QString("transform pointer is null!!!");

				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}
			auto pLinear = pTransform->GetLinear();
			Q_ASSERT(pLinear);
			if (!pLinear)
			{
				auto strError = QString("can not find linear!!!");

				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagValue, WIDGET_TYPE::SCALE_WIDGET_TYPE, strFesTagName);
			if (bFlag)
			{
				TabKeyStruct newKeyInfo;
				TabKeyStruct lastkeyInfo;
				newKeyInfo.BigTagName = strFesTagName;
				newKeyInfo.TagName = strTagValue;
				newKeyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;
				lastkeyInfo.BigTagName = strFesTagName;
				lastkeyInfo.TagName = strLastTagValue;
				lastkeyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;

				m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastkeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}

			memset(pLinear->m_szTagName, 0, sizeof(pLinear->m_szTagName));
			auto nSize = strTagValue.size();
			strncpy(pLinear->m_szTagName, strTagValue.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			item->setData(pLinear->m_szTagName, LASTVALUE);
			//item->setData(strTagValue, TREE_TOOL_TIP_ROLE);

			if (!pFes->ModifyFesHashMap(HASHID::TRANSFORMHASHID, strTagValue, strLastTagValue))
			{
				auto strError = QString("modify sacle fail!!!");
				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}
			
			///////////////////////////修改转换关联/////////////////////////////////////
			//修改转换的tagname 如果有关联 关联的转换跟着修改
			//转换修改,装置AIs绑定关联的转换也跟着修改
			pFes->ModifyAIsScaleArr(std::string(pLinear->m_szTagName), strLastTagValue.toStdString()
				, pLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,装置AOs关联的转换tagname
			pFes->ModifyAOsScaleArr(std::string(pLinear->m_szTagName), strLastTagValue.toStdString()
				, pLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,前置用户变量表关联转换也修改
			pFes->ModifyUserVariableScaleArr(std::string(pLinear->m_szTagName), strLastTagValue.toStdString()
				, pLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,转发通道AIs绑定关联的转换也修改
			pFes->ModifyForwardingAIsScaleArr(std::string(pLinear->m_szTagName), strLastTagValue.toStdString()
				, pLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,转发通道Aos绑定关联的转换也修改
			pFes->ModifyForwardingAOsScaleArr(std::string(pLinear->m_szTagName), strLastTagValue.toStdString()
				, pLinear->m_strDescription.toStdString(), pTransform->m_nType);
		}
		else
		{
			auto pTransform = reinterpret_cast<Config::CTransform *>(item->data(POINTERDATA).toLongLong());
			if (!pTransform)
			{
				auto strError = QString("transform pointer is null!!!");

				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}
			auto pNonLinear = pTransform->GetNonLinear();
			Q_ASSERT(pNonLinear);
			if (!pNonLinear)
			{
				auto strError = QString("can not find nonlinear!!!");

				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagValue, WIDGET_TYPE::SCALE_WIDGET_TYPE, strFesTagName);
			if (bFlag)
			{
				TabKeyStruct newKeyInfo;
				TabKeyStruct lastkeyInfo;
				newKeyInfo.BigTagName = strFesTagName;
				newKeyInfo.TagName = strTagValue;
				newKeyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;
				lastkeyInfo.BigTagName = strFesTagName;
				lastkeyInfo.TagName = strLastTagValue;
				lastkeyInfo.Type = WIDGET_TYPE::SCALE_WIDGET_TYPE;

				m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastkeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}

			memset(pNonLinear->m_szTagName, 0, sizeof(pNonLinear->m_szTagName));
			auto nSize = strTagValue.size();
			strncpy(pNonLinear->m_szTagName, strTagValue.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			item->setData(strTagValue, LASTVALUE);
			//item->setData(strTagValue, TREE_TOOL_TIP_ROLE);

			if (!pFes->ModifyFesHashMap(HASHID::TRANSFORMHASHID, strTagValue, strLastTagValue))
			{
				auto strError = QString("modify sacle fail!!!");
				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//////////////////////////////////修改转换关联////////////////////////////////////////////
			//修改AIs关联的tagname
			pFes->ModifyAIsScaleArr(std::string(pNonLinear->m_szTagName), strLastTagValue.toStdString()
				, pNonLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//修改AOs关联的tagname
			pFes->ModifyAOsScaleArr(std::string(pNonLinear->m_szTagName), strLastTagValue.toStdString()
				, pNonLinear->m_strDescription.toStdString(), pTransform->m_nType);
			pFes->ModifyUserVariableScaleArr(std::string(pNonLinear->m_szTagName), strLastTagValue.toStdString()
				, pNonLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,转发通道AIs绑定关联的转换也修改
			pFes->ModifyForwardingAIsScaleArr(std::string(pNonLinear->m_szTagName), strLastTagValue.toStdString()
				, pNonLinear->m_strDescription.toStdString(), pTransform->m_nType);
			//转换修改,转发通道Aos绑定关联的转换也修改
			pFes->ModifyForwardingAOsScaleArr(std::string(pNonLinear->m_szTagName), strLastTagValue.toStdString()
				, pNonLinear->m_strDescription.toStdString(), pTransform->m_nType);
		}
	}
}

/*! \fn void CScale::SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel)
********************************************************************************************************* 
** \brief CScale::SendSelectedRows 
** \details 创建属性栏
** \param set 
** \param nType 
** \param pModel 
** \return void 
** \author gw
** \date 2016年2月25日 
** \note 
********************************************************************************************************/
void CScale::SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel)
{
	Q_UNUSED(set);
	Q_UNUSED(nType);
	Q_UNUSED(pModel);
	
	if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
	{
		//线性转换
		//CScaleModel *pTmp = (CScaleModel *)pModel;
	}
}

/*! \fn void CScale::SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, void *pModel)
********************************************************************************************************* 
** \brief CScale::SendChangeChannelModelRows 
** \details 创建属性栏
** \param set 
** \param nType 
** \param pModel 
** \return void 
** \author gw
** \date 2016年2月25日 
** \note 
********************************************************************************************************/
void CScale::SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, void *pModel)
{
	Q_UNUSED(set);
	Q_UNUSED(nType);
	Q_UNUSED(pModel);
}

void CScale::CreateAttriConnect()
{
	if (m_bConnectAttr)
	{
		return;
	}

	m_bConnectAttr = true;

	connect(m_pAttr->GetEnumProperty(), SIGNAL(valueChanged(QtProperty *, int)),
		this, SLOT(valueChanged(QtProperty *, int)));

	connect(m_pAttr->GetDoubleProperty(), SIGNAL(valueChanged(QtProperty *, double)),
		this, SLOT(valueChanged(QtProperty *, double)));

	connect(m_pAttr->GetStringProperty(), SIGNAL(valueChanged(QtProperty *, const QString &)),
		this, SLOT(valueChanged(QtProperty *, const QString &)));

	connect(m_pAttr->GetBoolProperty(), SIGNAL(valueChanged(QtProperty *, bool)),
		this, SLOT(valueChanged(QtProperty *, bool)));

	connect(m_pAttr->GetSizeProperty(), SIGNAL(valueChanged(QtProperty *, const QSize &)),
		this, SLOT(valueChanged(QtProperty *, const QSize &)));

	connect(m_pAttr->GetSizePolicyProperty(), SIGNAL(valueChanged(QtProperty *, const QSizePolicy &)),
		this, SLOT(valueChanged(QtProperty *, const QSizePolicy &)));
}

void CScale::Disconnect()
{
	disconnect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	disconnect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
}

/*! \fn void CScale::Expanded(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::Expanded 
** \details 展开组
** \param index 
** \return void 
** \author gw
** \date 2016年7月29日 
** \note 
********************************************************************************************************/
void CScale::Expanded(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	
	if (nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM || nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		
		item->setIcon(QIcon(OPEN_GROUP_PNG));
	}
}

/*! \fn void CScale::Collapsed(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::Collapsed 
** \details 折叠组
** \param index 
** \return void 
** \author gw
** \date 2016年7月29日 
** \note 
********************************************************************************************************/
void CScale::Collapsed(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(CLOSE_GROUP_PNG));
	}
}

/*! \fn bool CScale::DeletGroup(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::DeletGroup 
** \details 删除组
** \param index 
** \return bool 
** \author gw
** \date 2016年11月8日 
** \note 
********************************************************************************************************/
bool CScale::DeleteGroup(const QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();

	auto indexTmp = index;
	while (indexTmp.parent().data(Qt::UserRole).toInt() != FES_ITEM)
	{
		indexTmp = indexTmp.parent();
	}
	auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}
	QString strFesTagName = pFes->m_szTagName;
	auto bFlag = CheckTagNameIsValid(strFesTagName, FES_DESC);
	Q_ASSERT(bFlag);
	if (!bFlag)
	{
		return false;
	}

	auto nType = index.data(Qt::UserRole).toInt();
	if (nType == FES_TYPE_SCALE_LINEAR_GROUP_ITEM)
	{
		switch (ret)
		{
			case QMessageBox::Yes:
			{
				//被删除组的父组
				auto pLinearGroupParent = reinterpret_cast<Config::CTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
				Q_ASSERT(pLinearGroupParent);
				if (!pLinearGroupParent)
				{
					return false;
				}

				//被删除组
				auto pLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
				Q_ASSERT(pLinearGroup);
				if (!pLinearGroup)
				{
					return false;
				}

				QVector<QModelIndex> vec;
				vec.clear();
				//大组内的所有的线性转换项
				CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_SCALE_LINEAR_CHILD_ITEM, vec);
				//删除前置hash内的线性转换
				for each (auto var in vec)
				{
					auto strTagName = var.data(Qt::EditRole).toString();
					if (!CheckTagNameIsValid(strFesTagName, FES_DESC))
					{
						return false;
					}
					
					//关闭打开的窗口
					DeleteModuleWidgetInTabWidget(var, strFesTagName, pFes);

					pFes->DeleteFesHashItem(HASHID::TRANSFORMHASHID, strTagName);
				}

				//删除内存结构 组和项
				if (!pLinearGroupParent->DeletGroup(pLinearGroup))
				{
					auto strError = tr("delete linear group %1 fail").arg(index.data(Qt::EditRole).toString());
					m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					return false;
				}

				auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
				pTop->parent()->removeRow(pTop->row());
			}
			case QMessageBox::Cancel:
				// Cancel was clicked
				break;
			default:
				// should never be reached
				break;
		}
	}
	else if (nType == FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM)
	{
		switch (ret)
		{
		case QMessageBox::Yes:
		{
			//被删除组的父组
			auto pNonLinearGroupParent = reinterpret_cast<Config::CTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
			Q_ASSERT(pNonLinearGroupParent);
			if (!pNonLinearGroupParent)
			{
				return false;
			}

			//被删除组
			auto pNonLinearGroup = reinterpret_cast<Config::CTransformGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pNonLinearGroup);
			if (!pNonLinearGroup)
			{
				return false;
			}

			QVector<QModelIndex> vec;
			vec.clear();
			//大组内的所有的线性转换项
			CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM, vec);
			//删除前置hash内的线性转换
			for each (auto var in vec)
			{
				auto strTagName = var.data(Qt::EditRole).toString();
				if (!CheckTagNameIsValid(strTagName, FES_DESC))
				{
					return false;
				}
				//关闭打开的窗口
				DeleteModuleWidgetInTabWidget(var, strFesTagName, pFes);

				pFes->DeleteFesHashItem(HASHID::TRANSFORMHASHID, strTagName);
			}

			//删除内存结构 组和项
			if (!pNonLinearGroupParent->DeletGroup(pNonLinearGroup))
			{
				auto strError = tr("delete linear group %1 fail").arg(index.data(Qt::EditRole).toString());
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}

			auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
			pTop->parent()->removeRow(pTop->row());
		}
		case QMessageBox::Cancel:
			// Cancel was clicked
			break;
		default:
			// should never be reached
			break;
		}
	}

	return false;
}


/*! \fn bool CScale::DeleteLinearModuleWidgetInTabWidget(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::DeleteLinearModuleWidgetInTabWidget 
** \details 删除打开的窗口
** \param index 
** \return bool 
** \author gw
** \date 2016年11月8日 
** \note 
********************************************************************************************************/
bool CScale::DeleteModuleWidgetInTabWidget(const QModelIndex &index, QString &strFesTagName, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}
	
	auto pLinear = reinterpret_cast<Config::CTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pLinear);
	if (!pLinear)
	{
		return false;
	}
	auto nType = index.data(Qt::UserRole).toInt();
	QString strTag;
	strTag.clear();
	if (nType == FES_TYPE_SCALE_LINEAR_CHILD_ITEM)
	{
		strTag = pLinear->GetLinear()->m_szTagName;
	}
	else if (nType == FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM)
	{
		strTag = pLinear->GetNonLinear()->m_szTagName;
	}

	Q_ASSERT(!strTag.isEmpty());
	if (strTag.isEmpty())
	{
		return false;
	}

	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::SCALE_WIDGET_TYPE, strFesTagName))
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

	//删除前置转换，关联该转换的关联关系也删除
	if (!pFes->DeleteFesScaleCombineRelaton(strTag.toStdString()))
	{
		auto strError = tr("Delete FesScaleCombineRelaton %1 fail").arg(strTag);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	return true;
}


/*! \fn bool CScale::DeleteItem(const QModelIndex &index)
********************************************************************************************************* 
** \brief CScale::DeleteItem 
** \details 删除项
** \param index 
** \return bool 
** \author gw
** \date 2016年11月8日 
** \note 
********************************************************************************************************/
bool CScale::DeleteItem(const QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the transform, the transform data will be deleted. if scale tagname is related to AIs or AOs, relation scale will bi deleted.Do u comform to delete the item?");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pGroupParent = reinterpret_cast<Config::CTransformGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return false;
		}

		auto pItem = reinterpret_cast<Config::CTransform *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
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
			return false;
		}
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = CheckTagNameIsValid(strFesTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return false;
		}


		DeleteModuleWidgetInTabWidget(index, strFesTagName, pFes);

		auto strTag = index.data(Qt::EditRole).toString();
		//组内的通道项删除 删除通道内存
		if (!pGroupParent->DeleteItem(pItem))
		{
			auto strError = tr("delete transform fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		//hash管理删除通道 不删除通道内存
		if (!pFes->DeleteFesHashItem(HASHID::TRANSFORMHASHID, strTag))
		{
			auto strError = tr("delete transform fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		////////////////////////////////删除转换关联AI AO USERVARAIBLE////////////////////////////////////////////
		////删除AIs关联的转换tagname
		//pFes->DeleteAIsScaleArr(strTag.toStdString());
		//pFes->DeleteAOsScaleArr(strTag.toStdString());
		////前置用户变量关联转换删除
		//pFes->DeleteUserVariableScaleArr(strTag.toStdString());

		////转换删除，转发通道AIs绑定关联的转换也删除
		//pFes->DeleteForwardingAIsScaleArr(strTag.toStdString());
		////转换删除，转发通道AOs绑定关联的转换也删除
		//pFes->DeleteForwardingAOsScaleArr(strTag.toStdString());

		auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
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

	return true;
}