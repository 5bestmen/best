#include <QMenu>
#include <QMessageBox>

#include "alarm.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/icore.h"
#include "fesmodule.h"
#include "attriwindow/attriwindow.h"
#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qteditorfactory.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "analogalarmview.h"
#include "scadastudio/tabwidget.h"
#include "analogalarmlimitview.h"
#include "digitalalarmview.h"
#include "digitalalarmlimitview.h"
#include "global.h"


extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CAlarm::CAlarm(ICoreui *pUi)
********************************************************************************************************* 
** \brief CAlarm::CAlarm 
** \details 告警
** \param pUi 框架Ui
** \return  
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
CAlarm::CAlarm(IMainModuleInterface *pCore)
	: QObject(pCore->GetUIMgr()), m_pUi(nullptr), m_pCore(nullptr), m_bConnectAttr(false),
	m_pAttr(nullptr)
{
	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	
	m_pCore = pCore;
	
	m_pUi = pCore->GetUIMgr();

	m_pAttr = pCore->GetUIMgr()->GetAtrri();

	Q_ASSERT(m_pUi);
	if (!m_pUi)
	{
		return;
	}

	connect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	//connect(m_pUi->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Clicked(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(ItemChanged(QStandardItem *)));
	connect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(Expanded(const QModelIndex &)));
	connect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(Collapsed(const QModelIndex &)));
}

/*! \fn CAlarm::~CAlarm()
********************************************************************************************************* 
** \brief CAlarm::~CAlarm 
** \details 析构
** \return  
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
CAlarm::~CAlarm()
{

}

/*! \fn void CAlarm::AddAlarmGroup()
********************************************************************************************************* 
** \brief CAlarm::AddAlarmGroup 
** \details 添加告警组
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CAlarm::AddAnalogAlarmGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(ADD_ALARM_GROUP_NAME).arg(0);

	auto nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_ANALOG_ALARM_ITEM || nType == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
	{	
		auto nDeepth = item->data(GROUP_NUMBER_ROLE).toInt();
		if (nDeepth < MAX_GROUP_NUM)
		{
			auto pGroupParent = reinterpret_cast<Config::CAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}

			auto pGroupChild = pGroupParent->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}

			nDeepth++;

			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
			newItem->setData(FES_TYPE_ANALOG_ALARM_GROUP_ITEM, Qt::UserRole);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);
		}
	}



	m_pUi->GetLeftTree()->expand(index);
}

void CAlarm::AddDigtalAlarmGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(ADD_ALARM_GROUP_NAME).arg(0);

	auto nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_DIGTAL_ALARM_ITEM || nType == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
	{
		auto nDeepth = item->data(GROUP_NUMBER_ROLE).toInt();
		if (nDeepth < MAX_GROUP_NUM)
		{
			auto pGroupParent = reinterpret_cast<Config::CDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pGroupParent);
			if (!pGroupParent)
			{
				return;
			}

			auto pGroupChild = pGroupParent->CreateGroup();
			Q_ASSERT(pGroupChild);
			if (!pGroupChild)
			{
				return;
			}

			nDeepth++;

			CQuoteItem *newItem = new CQuoteItem(pGroupChild->GetGroupName());
			newItem->setData(pGroupChild->GetGroupName(), LASTVALUE);
			newItem->setData(pGroupChild->GetGroupName(), TREE_TOOL_TIP_ROLE);
			newItem->setData(nDeepth, GROUP_NUMBER_ROLE);
			newItem->setData(FES_TYPE_DIGTAL_ALARM_GROUP_ITEM, Qt::UserRole);
			newItem->setData(reinterpret_cast<long long>(pGroupChild), POINTERDATA);
			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);
		}
	}

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CAlarm::ShowMenu()
********************************************************************************************************* 
** \brief CAlarm::ShowMenu 
** \details 右键菜单
** \return void 
** \author gw
** \date 2015年11月22日 
** \note 
********************************************************************************************************/
void CAlarm::ShowMenu(QModelIndex &index)
{	
	QMenu *pMenu = new QMenu(NULL);

	//auto nType = index.data(Qt::UserRole).toInt();

	if (index.data(Qt::UserRole).toInt() == FES_ANALOG_ALARM_ITEM 
		|| index.data(Qt::UserRole).toInt() == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
	{
		QAction *pActionAlarm = new QAction(ADD_ALARM_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(ALARM_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(ADD_ALARM_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (index.data(Qt::UserRole).toInt() == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

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
			//添加报警
			AddAnalogAlarm(index);
		}
		else if(selectedItem == pActionGroup)
		{
			//添加报警组
			AddAnalogAlarmGroup(index);
		}
		else if (selectedItem == pActionRename && pActionRename != nullptr)
		{
			if (index.data(Qt::UserRole).toInt() == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
			{
				m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
			}
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionDelete && pActionDelete != nullptr)
		{
			DeleteAnalogAlarmGroup(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_TYPE_ALARM_ANALOG_ITEM)
	{
		//QAction *pActionAlarm = new QAction(ALARM_OFFLIMIT_NAME, pMenu);

		//pActionAlarm->setIcon(QIcon(ALARM_PNG));

		//pMenu->addAction(pActionAlarm);


		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		//if (selectedItem == pActionAlarm)
		//{
		//	//添加限值
		//	AddAnalogOfflimit(index);
		//}
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
			DeleteAnalogAlarmItem(index);
		}
		else
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM 
		|| index.data(Qt::UserRole).toInt() == FES_DIGTAL_ALARM_ITEM)
	{
		QAction *pActionAlarm = new QAction(ADD_ALARM_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(ALARM_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(ADD_ALARM_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);


		QAction *pActionRename = nullptr;
		QAction *pActionDelete = nullptr;
		if (index.data(Qt::UserRole).toInt() == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
		{
			pActionRename = new QAction(RENAME_ITEM, pMenu);
			pMenu->addAction(pActionRename);

			pActionDelete = new QAction(DELETE_ITEM, pMenu);
			pMenu->addAction(pActionDelete);
		}


		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pActionAlarm)
		{
			//添加报警
			AddDigtalAlarm(index);
		}
		else if (selectedItem == pActionGroup)
		{
			//添加报警组
			AddDigtalAlarmGroup(index);
		}
		else if (pActionRename == pActionGroup)
		{
			if (index.data(Qt::UserRole).toInt() == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
			{
				m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
			}
		}
		else if (selectedItem == pActionExpand)
		{
			ExpandChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(index, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (pActionRename && selectedItem == pActionRename)
		{
			m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		}
		else if (pActionRename != nullptr && selectedItem == pActionDelete)
		{
			DeleteDigitalAlarmGroup(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_TYPE_ALARM_DIGITAL_ITEM)
	{
		//QAction *pActionAlarm = new QAction(tr("add digtal limit"), pMenu);

		//pActionAlarm->setIcon(QIcon(ALARM_PNG));

		//pMenu->addAction(pActionAlarm);


		QAction *pActionRename = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionRename);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		//if (selectedItem == pActionAlarm)
		//{
		//	//添加限值
		//	AddDigtalOfflimit(index);
		//}
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
			DeleteDigitalAlarmItem(index);
		}
	}

	pMenu->deleteLater();
}


/*! \fn void CAlarm::AddAlarm(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::AddAlarm 
** \details 添加告警
** \param index 
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CAlarm::AddAnalogAlarm(QModelIndex &index)
{	
	QString strFesTagname;

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
		QString strError = QString(tr("can not find fes tagname:%1!!!")).arg(strFesTagname);
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);
		return;
	}

	Config::CAnalogAlarm *pTmp = pFes->AddNewAnalogAlarm();
	Q_ASSERT(pTmp);
	//添加模拟量告警
	if (pTmp)
	{
		//添加报警
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		QString str = item->text();
		pTmp->m_arrAlarmOfflimit;
		QString strName = pTmp->m_szTagName;

		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(FES_TYPE_ALARM_ANALOG_ITEM, Qt::UserRole);
		newItem->setData(strName, LASTVALUE);
		//newItem->setData(pTmp->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);
		newItem->setData(pTmp->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(ALARM_PNG));
		//limit
		auto strLimitTagName = QString("%1_limit").arg(strName);
		auto strLimitDesc = QString(tr("%1_limit")).arg(strName);
		CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
		pLimitItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);
		pLimitItem->setData(FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, Qt::UserRole);
		pLimitItem->setData(strLimitTagName, LASTVALUE);
		pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
		pLimitItem->setIcon(QIcon(ALARM_PNG));
		newItem->appendRow(pLimitItem);

		item->appendRow(newItem);

		auto pGroupParent = reinterpret_cast<Config::CAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		//模拟量告警项添加到父组
		pGroupParent->AddAnalogItem(pTmp);

		m_pUi->GetLeftTree()->expand(index);
	}
	else
	{
		m_pCore->LogMsg(AI_ALARM_DESC, LOG_STRING("add analog error!!!"), LEVEL_1);

		return;
	}
}

/*! \fn void CAlarm::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CAlarm::showMouseRightButton 
** \details 右键菜单
** \param point 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CAlarm::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_ALARM_ANALOG_ITEM || nType == FES_TYPE_ALARM_DIGITAL_ITEM ||
		nType == FES_TYPE_ANALOG_ALARM_GROUP_ITEM || nType == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM ||
		nType == FES_ANALOG_ALARM_ITEM || nType == FES_DIGTAL_ALARM_ITEM ||
		nType == FES_TYPE_ALARM_ANALOG_LIMIT_ITEM || nType == FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM)
	{
		ShowMenu(indexSelect);
	}
	else if (nType == FES_ALARM_ITEM)
	{
		auto pMenu = new QMenu;

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pActionExpand)
		{
			ExpandChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}
		else if (selectedItem == pActionCollapse)
		{
			CollapseChildren(indexSelect, m_pCore->GetUIMgr()->GetLeftTree());
		}

		pMenu->deleteLater();
	}
}

/*! \fn void CAlarm::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::doubleClicked 
** \details 双击
** \param index 
** \return void 
** \author gw
** \date 2015年12月11日 
** \note 
********************************************************************************************************/
void CAlarm::doubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_ALARM_ANALOG_ITEM)
	{
		//检查修改的通道tagname是否存在
		auto pItemParent = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		auto strFesTagName = pItemParent->data(Qt::EditRole).toString();
		auto bFlag = CheckTagNameIsValid(strFesTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return;
		}
		
		auto pAlarm = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			auto strError = tr("analog alarm pointer is nullptr!!!");
			m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto strTagName = pAlarm->m_szTagName;
		bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return;
		}


		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_ANALOG_ALARM_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}
		CAnalogAlarmView *pView = new CAnalogAlarmView(nullptr, m_pCore, this, pAlarm, pItem, pFes);
		
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_TYPE;

		auto &icon = pItem->icon();
		m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
	}
	else if (nType == FES_TYPE_ALARM_ANALOG_LIMIT_ITEM)
	{
		//检查修改的通道tagname是否存在
		auto pItemParent = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		
		auto pAlarm = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			auto strError = tr("analog alarm limit pointer is nullptr!!!");
			m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();

		QString strFesTagName = pFes->m_szTagName;
		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto *pView = new CAnalogAlarmLimitView(nullptr, m_pCore, this, pAlarm, m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index), pFes);
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE;

		m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo);
	}
	else if (nType == FES_TYPE_ALARM_DIGITAL_ITEM)
	{
		//检查修改的通道tagname是否存在
		auto pItemParent = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		
		auto pAlarm = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAlarm);
		if (!pAlarm)
		{
			auto strError = tr("digital alarm pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("tagname is invalid!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE;

		auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		auto &icon = pItem->icon();
		CDigitalAlarmView *pView = new CDigitalAlarmView(nullptr, m_pCore, this, pAlarm, pItem, pFes);
		m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
	}
	else if (nType == FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM)
	{
		auto pItemParent = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index)->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		QString strFesTagName = pFes->m_szTagName;
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto pDigital = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigital);
		if (!pDigital)
		{
			auto strError = tr("digital alarm pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto pItem = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}
		auto &icon = pItem->icon();
		auto *pView = new CDigitalAlarmLimitView(nullptr, m_pCore, this, pDigital, pItem, pFes);
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE;
		m_pCore->GetUIMgr()->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
	}
}

/*! \fn void CAlarm::itemChanged(QStandardItem *item)
********************************************************************************************************* 
** \brief CAlarm::itemChanged 
** \details 左边树子项修改处理
** \param item 
** \return void 
** \author gw
** \date 2015年12月16日 
** \note 
********************************************************************************************************/
void CAlarm::ItemChanged(QStandardItem *item)
{
	auto strLastValue = item->data(LASTVALUE).toString();
	auto strValue = item->data(Qt::EditRole).toString();
	if (strLastValue == strValue)
	{
		return;
	}


	
	if (item->data(Qt::UserRole).toInt() == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
	{	
		QString strAlarmTagName = item->data(Qt::EditRole).toString();
		auto strAlarmLastTagValue = item->data(LASTVALUE).toString();
		if (strAlarmTagName == strAlarmLastTagValue)
		{
			return;
		}

		//检查修改的通道tagname是否存在
		auto pItemParent = item->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		
		auto pGroupParent = reinterpret_cast<Config::CAnalogAlarmGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CAnalogAlarmGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		if (pGroupParent->CheckModifyGroupNameExist(pGroup, strAlarmTagName))
		{
			auto strError = tr("group name %1 is modify fail!!!").arg(strAlarmTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strAlarmLastTagValue, Qt::EditRole);

			return;
		}

		pGroup->SetGroupName(strAlarmTagName);
		item->setData(strAlarmTagName, LASTVALUE);
		//item->setData(strAlarmTagName, TREE_TOOL_TIP_ROLE);
		


		//QModelIndex &index = m_pUi->GetLeftTreeModel()->indexFromItem(item);
		//
		//QVector<QModelIndex> vec;
		//vec.clear();
		//CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_ALARM_ANALOG_ITEM, vec);

		//for (auto &tmp : vec)
		//{
		//	//auto tmp = CFesModel::GetFesFromAlarmTagname(m_arrFes, strFesTagname, AIALARM);

		//	auto *pAnalog = reinterpret_cast<Config::CAnalogAlarm *>(tmp.data(POINTERDATA).toLongLong());
		//	
		//	Q_ASSERT(pAnalog);
		//	if (pAnalog)     
		//	{
		//		auto strTmp = CFesModule::GetGroupName(tmp, FES_ANALOG_ALARM_ITEM);

		//		pAnalog->m_strGroupName = strTmp;
		//	}
		//	else
		//	{
		//		QString strDesc = tmp.data(Qt::EditRole).toString() + " ananlog alarm can not find";
		//		
		//		m_pCore->LogMsg(AI_ALARM_DESC, (char *)strDesc.toStdString().c_str(), LEVEL_1);
		//	}
		//}

		//item->setData(strValue, LASTVALUE);
	}
	else if (item->data(Qt::UserRole).toInt() == FES_TYPE_ALARM_ANALOG_ITEM)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		//校验tagname是否有效
		if (!CheckTagNameIsValid(strTagName, FES_DESC) || !CheckTagNameIsValid(strLastTagName, FES_DESC))
		{
			return;
		}

		//检查修改的通道tagname是否存在
		auto pItemParent = item->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		auto strFesTagName = pItemParent->data(Qt::EditRole).toString();
		auto bFlag = CheckTagNameIsValid(strFesTagName, FES_DESC);
		Q_ASSERT(bFlag);
		if (!bFlag)
		{
			return;
		}


		if (pFes->CheckTagNameIsExist(strTagName))
		{
			//修改的通道tagname存在
			auto strError = tr("analog alarm tag name %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		//auto pGroupParent = reinterpret_cast<Config::CAnalogGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		//Q_ASSERT(pGroupParent);
		//if (!pGroupParent)
		//{
		//	return;
		//}

		auto *pAnalog = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalog);
		if (!pAnalog)
		{
			QString strDesc = index.data(Qt::EditRole).toString() + tr(" ananlog alarm can not find");

			m_pCore->LogMsg(AI_ALARM_DESC, (char *)strDesc.toStdString().c_str(), LEVEL_1);
			item->setData(strLastTagName, Qt::EditRole);

			return;
		}
		else
		{
			memset(pAnalog->m_szTagName, 0, sizeof(pAnalog->m_szTagName));
			int nSize = strTagName.size();
			strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			item->setData(pAnalog->m_szTagName, LASTVALUE);
			item->setData(pAnalog->m_szTagName, TREE_TOOL_TIP_ROLE);
		}

		//修改hash管理数据
		if (!pFes->ModifyFesHashMap(HASHID::ANALOGALARMHASHID, strTagName, strLastTagName))
		{
			auto strError = tr("modify alarm hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//模拟量告警修改,通知和模拟量相关的模块修改
		//装置 AIs 修改模拟量输入点关联的ALARM tagname	模拟量关联模拟量告警 开关量关联开关量告警
		pFes->ModifyAIsAlarmArr(std::string(pAnalog->m_szTagName), strLastTagName.toStdString(), pAnalog->m_strDescription.toStdString(), AIALARM);
		
		//用户变量关联的模拟量告警
		pFes->ModifyUserVariableAlarmArr(std::string(pAnalog->m_szTagName), strLastTagName.toStdString(), pAnalog->m_strDescription.toStdString(), AIALARM);

		//转发通道没有告警

		//判断是否存在该widget窗口
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::FES_ANALOG_ALARM_TYPE, strFesTagName);
		if (bFlag)
		{
			TabKeyStruct keyInfo;
			TabKeyStruct lastKeyInfo;
			keyInfo.BigTagName = strFesTagName;
			keyInfo.TagName = strTagName;
			keyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_TYPE;
			lastKeyInfo.BigTagName = strFesTagName;
			lastKeyInfo.TagName = strLastTagName;
			lastKeyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_TYPE;
			
			m_pUi->GetTabWidget()->SetTabName(keyInfo, lastKeyInfo);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}

		//告警限值
		if (item->hasChildren())
		{
			auto pLimit = item->child(ALARM_CHILD::LIMIT);

			QString strAlarm = pAnalog->m_szTagName;
			QString strAlarmLimit = QString("%1_limit").arg(strAlarm);
			QString strLastAlarmLimit = pLimit->data(Qt::EditRole).toString();
			bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastAlarmLimit, WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE, strFesTagName);
			if (bFlag)
			{
				TabKeyStruct newKeyInfo;
				TabKeyStruct lastKeyInfo;
				newKeyInfo.BigTagName = strFesTagName;
				newKeyInfo.TagName = strAlarmLimit;
				newKeyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE;
				lastKeyInfo.BigTagName = strFesTagName;
				lastKeyInfo.TagName = strLastAlarmLimit;
				lastKeyInfo.Type = WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE;

				m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}

			pLimit->setData(strAlarmLimit, Qt::EditRole);
			pLimit->setData(strAlarmLimit, LASTVALUE);
			pLimit->setData(strAlarmLimit, TREE_TOOL_TIP_ROLE);
		}
		else 
		{
			return;
		}

	}
	//else if (item->data(Qt::UserRole).toInt() == FES_TYPE_ALARM_ANALOG_LIMIT_ITEM)
	//{
	//	auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

	//	auto strTagName = index.data(Qt::EditRole).toString();
	//	auto strLastTagName = index.data(LASTVALUE).toString();
	//	if (strTagName == strLastTagName)
	//	{
	//		return;
	//	}

	//	//检查修改的通道tagname是否存在
	//	auto pItemParent = item->parent();
	//	while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
	//	{
	//		pItemParent = pItemParent->parent();
	//	}
	//	auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pFes);
	//	if (!pFes)
	//	{
	//		return;
	//	}

	//	if (pFes->CheckTagNameIsExist(strTagName))
	//	{
	//		//修改的通道tagname存在
	//		auto strError = tr("analog alarm limit tag name %1 is exist!!!").arg(strTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		item->setData(strLastTagName, Qt::EditRole);

	//		return;
	//	}

	//	auto pAnalogAlarmLimit = reinterpret_cast<Config::CAinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pAnalogAlarmLimit);
	//	if (!pAnalogAlarmLimit)
	//	{
	//		QString strDesc = QString(tr(" analog alarm limit can not find"));

	//		m_pCore->LogMsg(FES_DESC, (char *)strDesc.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}

	//	memset(pAnalogAlarmLimit->m_szTagName, 0, sizeof(pAnalogAlarmLimit->m_szTagName));
	//	auto nSize = strTagName.size();
	//	strncpy(pAnalogAlarmLimit->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
	//	item->setData(strTagName, LASTVALUE);

	//	if (!pFes->ModifyFesHashMap(HASHID::ANALOGALARMLIMITHASHID, pAnalogAlarmLimit->m_szTagName, strLastTagName))
	//	{
	//		auto strError = tr("analog alarm limit modify fail!!!");
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}

	//	//判断是否存在该widget
	//	auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE);
	//	if (bFlag)
	//	{
	//		m_pUi->GetTabWidget()->SetTabName(strTagName, FES_ANALOG_ALARM_LIMIT_TYPE, strLastTagName);

	//		IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
	//		Q_ASSERT(pBase);
	//		if (!pBase)
	//		{
	//			return;
	//		}

	//		pBase->Refresh();
	//	}
	//}
	else if (item->data(Qt::UserRole).toInt() == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
	{
		QString strAlarmTagName = item->data(Qt::EditRole).toString();
		auto strAlarmLastTagValue = item->data(LASTVALUE).toString();
		if (strAlarmTagName == strAlarmLastTagValue)
		{
			return;
		}

		auto pGroupParent = reinterpret_cast<Config::CDigitalAlarmGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pGroup = reinterpret_cast<Config::CDigitalAlarmGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		if (pGroupParent->CheckModifyGroupNameExist(pGroup, strAlarmTagName))
		{
			auto strError = tr("group name %1 is modify fail!!!").arg(strAlarmTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastValue, Qt::EditRole);

			return;
		}

		pGroup->SetGroupName(strAlarmTagName);
		item->setData(strAlarmTagName, LASTVALUE);
		item->setData(strAlarmTagName, TREE_TOOL_TIP_ROLE);
		
		
		//QModelIndex &index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		//QVector<QModelIndex> vec;
		//vec.clear();
		//CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_ALARM_DIGITAL_ITEM, vec);

		//for (auto &tmp : vec)
		//{
		//	//auto tmp = CFesModel::GetFesFromAlarmTagname(m_arrFes, strFesTagname, AIALARM);

		//	auto *pAnalog = reinterpret_cast<Config::CAnalogAlarm *>(tmp.data(POINTERDATA).toLongLong());

		//	Q_ASSERT(pAnalog);
		//	if (pAnalog)
		//	{
		//		auto strTmp = CFesModule::GetGroupName(tmp, FES_DIGTAL_ALARM_ITEM);

		//		pAnalog->m_strGroupName = strTmp;
		//		item->setData(strValue, LASTVALUE);
		//	}
		//	else
		//	{
		//		QString strDesc = tmp.data(Qt::EditRole).toString() + " digtal alarm can not find";

		//		m_pCore->LogMsg(DI_ALARM_DESC, (char *)strDesc.toStdString().c_str(), LEVEL_1);
		//	}
		//}
	}
	else if (item->data(Qt::UserRole).toInt() == FES_TYPE_ALARM_DIGITAL_ITEM)
	{
		auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);

		auto strTagName = index.data(Qt::EditRole).toString();
		auto strLastTagName = index.data(LASTVALUE).toString();
		if (strTagName == strLastTagName)
		{
			return;
		}

		//校验tagname是否有效
		if (!CheckTagNameIsValid(strTagName, FES_DESC) || !CheckTagNameIsValid(strLastTagName, FES_DESC))
		{
			return;
		}

		//检查修改的通道tagname是否存在
		auto pItemParent = item->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}
		if (!CheckTagNameIsValid(pFes->m_szTagName, FES_DESC))
		{
			return;
		}
		QString strFesTagName = pFes->m_szTagName;

		if (pFes->CheckTagNameIsExist(strTagName))
		{
			//修改的告警tagname存在
			auto strError = tr("digital alarm tag name %1 is exist!!!").arg(strTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastTagName, Qt::EditRole);

			return;
		}

		auto *pDigtal = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigtal);
		if (!pDigtal)
		{
			auto strError = tr("digtal alarm tagname %1 can not find!!!").arg(strTagName);
			m_pCore->LogMsg(DI_ALARM_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		else
		{
			memset(pDigtal->m_szTagName, 0, sizeof(pDigtal->m_szTagName));
			int nSize = strTagName.size();
			strncpy(pDigtal->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			item->setData(strValue, LASTVALUE);

			if (!pFes->ModifyFesHashMap(HASHID::DIGITALALARMHASHID, strTagName, strLastTagName))
			{
				auto strError = tr("digtal alarm tagname %1 hash can not find!!!").arg(strTagName);
				m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

				return;
			}

			//////////////////////////关联修改///////////////////////////////
			//开关量告警关联修改,DIs关联的开关量告警也修改
			pFes->ModifyDIsAlarmArr(std::string(pDigtal->m_szTagName), strLastTagName.toStdString(), pDigtal->m_strDescription.toStdString(), DIALARM);
			
			//开关量告警修改,用户变量关联的开关量告警也修改
			pFes->ModifyUserVariableAlarmArr(std::string(pDigtal->m_szTagName), strLastTagName.toStdString(), pDigtal->m_strDescription.toStdString(), DIALARM);

			//转发通道没有告警

			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE, strFesTagName);
			if (bFlag)
			{
				TabKeyStruct newKeyInfo;
				TabKeyStruct lastKeyInfo;
				newKeyInfo.BigTagName = strFesTagName;
				newKeyInfo.TagName = strTagName;
				newKeyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE;
				lastKeyInfo.BigTagName = strFesTagName;
				lastKeyInfo.TagName = strLastTagName;
				lastKeyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE;

				m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}

			//告警限值tabwidget窗口
			if (item->hasChildren())
			{
				auto pLimit = item->child(ALARM_CHILD::LIMIT);

				QString strAlarm = pDigtal->m_szTagName;
				QString strAlarmLimit = QString("%1_limit").arg(strAlarm);
				QString strLastAlarmLimit = pLimit->data(Qt::EditRole).toString();
				bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastAlarmLimit, WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strAlarmLimit;
					newKeyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strLastAlarmLimit;
					lastKeyInfo.Type = WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return;
					}

					pBase->Refresh();
				}

				pLimit->setData(strAlarmLimit, Qt::EditRole);
				pLimit->setData(strAlarmLimit, LASTVALUE);
				pLimit->setData(strAlarmLimit, TREE_TOOL_TIP_ROLE);
			}
		}
	}
	//else if (item->data(Qt::UserRole).toInt() == FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM)
	//{
	//	auto index = m_pUi->GetLeftTreeModel()->indexFromItem(item);
	//	auto strTagName = index.data(Qt::EditRole).toString();
	//	auto strLastTagName = index.data(LASTVALUE).toString();
	//	if (strTagName == strLastTagName)
	//	{
	//		return;
	//	}

	//	//检查修改的通道tagname是否存在
	//	auto pItemParent = item->parent();
	//	while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
	//	{
	//		pItemParent = pItemParent->parent();
	//	}
	//	auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pFes);
	//	if (!pFes)
	//	{
	//		return;
	//	}

	//	if (pFes->CheckTagNameIsExist(strTagName))
	//	{
	//		//修改的告警tagname存在
	//		auto strError = tr("digital alarm limit tag name %1 is exist!!!").arg(strTagName);
	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		item->setData(strLastTagName, Qt::EditRole);

	//		return;
	//	}

	//	auto pDigtalAlarmLimit = reinterpret_cast<Config::CDinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pDigtalAlarmLimit);
	//	if (!pDigtalAlarmLimit)
	//	{
	//		QString strDesc = QString(tr(" analog alarm limit %1 can not find")).arg(strTagName);

	//		m_pCore->LogMsg(FES_DESC, (char *)strDesc.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}

	//	memset(pDigtalAlarmLimit->TagName, 0, sizeof(pDigtalAlarmLimit->TagName));
	//	auto nSize = strTagName.size();
	//	strncpy(pDigtalAlarmLimit->TagName, strValue.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
	//	item->setData(strValue, LASTVALUE);

	//	if (!pFes->ModifyFesHashMap(HASHID::DIGITALALARMLIMITHASHID, strTagName, strLastTagName))
	//	{
	//		auto strError = tr("digtal alarm limit tagname %1 hash can not find!!!").arg(strTagName);
	//		m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}

	//	//判断是否存在该widget
	//	auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastTagName, WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE);
	//	if (bFlag)
	//	{
	//		m_pUi->GetTabWidget()->SetTabName(strTagName, FES_DIGITAL_ALARM_LIMIT_TYPE, strLastTagName);

	//		IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
	//		Q_ASSERT(pBase);
	//		if (!pBase)
	//		{
	//			return;
	//		}

	//		pBase->Refresh();
	//	}
	//}
}

/*! \fn void CAlarm::Clicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::Clicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年5月6日 
** \note 
********************************************************************************************************/
#include <QTime>
void CAlarm::Clicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	QString strTag = index.data(Qt::EditRole).toString();

	if (nType == FES_TYPE_ALARM_ANALOG_ITEM)
	{
		
		/*QString strFesTagName = CFesModule::GetTagname(index, FES_ITEM);
		Config::CFesData *pFes = CFesModule::GetFes(m_arrFes, strFesTagName);*/
		
		auto const pAnalogAlarm = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarm);
		if (!pAnalogAlarm)
		{
			return;
		}

		CreateAttri(pAnalogAlarm);
	}
	else if (nType == FES_TYPE_ALARM_ANALOG_LIMIT_ITEM)
	{
		////查找所属前置TagName
		//QString strFesTagName = CFesModule::GetTagname(index, FES_ITEM);
		//Config::CFesData *pFes = CFesModule::GetFes(m_arrFes, strFesTagName);
		//
		//Q_ASSERT(pFes);
		//if (pFes == nullptr)
		//{
		//	m_pCore->LogMsg(AI_ALARM_DESC, LOG_STRING("Can not find fes!!!"), LEVEL_1);
		//	return;
		//}

		//auto strAlarmTag = CFesModule::GetTagname(index, FES_TYPE_ALARM_ANALOG_ITEM);
		//auto const pAnalogAlarm = CFesModule::GetAnalogAlarm(pFes, strAlarmTag);
		//Q_ASSERT(pAnalogAlarm);
		//if (!pAnalogAlarm)
		//{
		//	auto strError = QString("ananlog alarm %1 can not find!!!").arg(strAlarmTag);
		//	m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);
		//	return;
		//}

		auto strAnalogLimit = index.data(Qt::EditRole).toString();
		auto pAnalogAlarmLimit = reinterpret_cast<Config::CAinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarmLimit);
		if(!pAnalogAlarmLimit)
		{
			auto strError = QString("ananlog alarm limit %1 can not find!!!").arg(strAnalogLimit);
			m_pCore->LogMsg(AI_ALARM_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		//QTime myTimer;
		//myTimer.start();
		//// do something..

		CreateAttri(pAnalogAlarmLimit);

		//int nMilliseconds = myTimer.elapsed();

		//int nTest;
		//nTest = 5;

	}
	else if (nType == FES_TYPE_ALARM_DIGITAL_ITEM)
	{
		////查找所属前置TagName
		//QString strFesTagName = CFesModule::GetTagname(index, FES_ITEM);
		//Config::CFesData *pFes = CFesModule::GetFes(m_arrFes, strFesTagName);
		//
		//Q_ASSERT(pFes);
		//if (pFes == nullptr)
		//{
		//	m_pCore->LogMsg(DI_ALARM_DESC, LOG_STRING("Can not find fes!!!"), LEVEL_1);
		//	return;
		//}

		auto const pDigitalAlarm = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarm);
		if (!pDigitalAlarm)
		{
			return;
		}

		CreateAttri(pDigitalAlarm);
	}
	else if (nType == FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM)
	{
		////查找所属前置TagName
		//QString strFesTagName = CFesModule::GetTagname(index, FES_ITEM);
		//Config::CFesData *pFes = CFesModule::GetFes(m_arrFes, strFesTagName);

		//Q_ASSERT(pFes);
		//if (pFes == nullptr)
		//{
		//	m_pCore->LogMsg(DI_ALARM_DESC, LOG_STRING("Can not find fes!!!"), LEVEL_1);
		//	return;
		//}
		//
		//auto strDigitalTagName = CFesModule::GetTagname(index, FES_TYPE_ALARM_DIGITAL_ITEM);
		//auto pDigital = CFesModule::GetDigtitalAlarm(pFes, strDigitalTagName);
		//Q_ASSERT(pDigital);
		//if (!pDigital)
		//{
		//	auto strError = QString("digital alarm tag %1 can not find!!!").arg(strDigitalTagName);
		//	m_pCore->LogMsg(DI_ALARM_DESC, strDigitalTagName.toStdString().c_str(), LEVEL_1);
		//	return;
		//}

		auto const pDigitalAlarmLimit = reinterpret_cast<Config::CDinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmLimit);
		if (!pDigitalAlarmLimit)
		{
			return;
		}

		CreateAttri(pDigitalAlarmLimit);
	}
}

/*! \fn void CAlarm::CreateAttriConnect()
********************************************************************************************************* 
** \brief CAlarm::CreateAttriConnect 
** \details 信号槽关联
** \return void 
** \author gw
** \date 2016年5月6日 
** \note 
********************************************************************************************************/
void CAlarm::CreateAttriConnect()
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

	connect(m_pAttr->GetColorProperty(), SIGNAL(valueChanged(QtProperty *, const QColor &)),
		this, SLOT(valueChanged(QtProperty *, const QColor &)));
}

/*! \fn void CAlarm::CreateAttri(Config::CAnalogAlarm *pAnalogAlarm)
********************************************************************************************************* 
** \brief CAlarm::CreateAttri 
** \details 创建属性栏
** \param pAnalogAlarm 
** \return void 
** \author gw
** \date 2016年5月6日 
** \note 
********************************************************************************************************/
void CAlarm::CreateAttri(Config::CAnalogAlarm *pAnalogAlarm)
{
 	CreateAttriConnect();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	m_idToProperty.clear();
	m_propertyToId.clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	auto strTmp = QStringLiteral("基本");
	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToId[property] = strTmp;
	m_idToProperty[strTmp] = property;

	auto strAlarmEnalbe = QStringLiteral("告警");
	QtProperty *pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strAlarmEnalbe);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pAnalogAlarm->m_bEnable);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strAlarmEnalbe;
	m_idToProperty[strAlarmEnalbe] = pProperty;

	//Tagname
	auto strTagname = QStringLiteral("代码");
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTagname);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarm->m_szTagName);
	QString pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	m_idToProperty[strTagname] = pProperty;
	m_propertyToId[pProperty] = strTagname;

	property->addSubProperty(pProperty);


	//Desc
	auto strAlarm = QStringLiteral("告警名");
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strAlarm);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarm->m_strDescription);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strAlarm;
	m_idToProperty[strAlarm] = pProperty;


	//ID
	auto strID = "ID";
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strID);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);

	m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pAnalogAlarm->m_nID);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strID;
	m_idToProperty[strID] = pProperty;


	auto strQualityGoodEnalbe = QStringLiteral("品质");
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strQualityGoodEnalbe);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pAnalogAlarm->m_bOnQualityGood);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strQualityGoodEnalbe;
	m_idToProperty[strQualityGoodEnalbe] = pProperty;


	auto strDeadArea = QStringLiteral("死区");
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strDeadArea);
	m_pUi->GetAtrri()->GetDoubleProperty()->setValue(pProperty, pAnalogAlarm->m_dbDeadArea);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strDeadArea;
	m_idToProperty[strDeadArea] = pProperty;


	auto strDeadAreaType = QStringLiteral("死区类型");
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strDeadAreaType);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pAnalogAlarm->m_nDeadType);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strDeadAreaType;
	m_idToProperty[strDeadAreaType] = pProperty;

	auto strDelay = QStringLiteral("延时告警");
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strDelay);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pAnalogAlarm->m_nDelayAlarm);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strDelay;
	m_idToProperty[strDelay] = pProperty;


	pTree->addProperty(property);
}

void CAlarm::CreateAttri(Config::CAinAlarmLimit *pAnalogAlarmLimit)
{
	CreateAttriConnect();

	QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
	while (itProp != m_propertyToId.constEnd()) 
	{
		delete itProp.key();
		itProp++;
	}

	m_propertyToId.clear();
	m_idToProperty.clear();


	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	//pTree->clear();

	//m_idToProperty.clear();
	//m_propertyToId.clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	auto strTmp = QStringLiteral("基本");
	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToId[property] = strTmp;
	m_idToProperty[strTmp] = property;


	pTree->addProperty(property);


	auto strName = QStringLiteral("限值名称");
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->m_strDescription);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("报警时间格式");
	pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->m_strDescription);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("条件");
	pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, pAnalogAlarmLimit->m_nCondition);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("限值");
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetDoubleProperty()->setValue(pProperty, pAnalogAlarmLimit->m_dbValue);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("限值低值");
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetDoubleProperty()->setValue(pProperty, pAnalogAlarmLimit->m_dbValue);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("命令");
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->m_strCommands);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	strName = QStringLiteral("告警方式");
	pProperty = pGroup->addProperty(strName);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("推画面");
	QtProperty *pTmp = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strName);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;


	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("文本色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("语音重复");
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pAnalogAlarmLimit->m_nPlaySoundTimes);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("背景色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("闪烁背景色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("闪烁文本色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	strTmp = QStringLiteral("报警通知");
	pProperty = pGroup->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToId[property] = strTmp;
	m_idToProperty[strTmp] = property;

	//
	strName = QStringLiteral("接受者");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("发送文本");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("电子邮件");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("语音电话");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("短信");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//
	strName = QStringLiteral("传真");
	pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pAnalogAlarmLimit->Acceptor);

	pProperty->addSubProperty(pTmp);
	m_propertyToId[pProperty] = strName;
	m_idToProperty[strName] = pProperty;

	//pTree->addProperty(property);
}

void CAlarm::CreateAttri(Config::CDigitalAlarm *pDigitalAlarm)
{
	CreateAttriConnect();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	m_idToProperty.clear();
	m_propertyToId.clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	auto strTmp = QStringLiteral("基本");
	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToId[property] = strTmp;
	m_idToProperty[strTmp] = property;


	auto strAlarmEnalbe = QStringLiteral("告警");
	QtProperty *pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strAlarmEnalbe);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pDigitalAlarm->m_bEnable);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strAlarmEnalbe;
	m_idToProperty[strAlarmEnalbe] = pProperty;

	//Tagname
	auto strTagname = QStringLiteral("代码");
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTagname);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarm->m_szTagName);
	QString pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	m_idToProperty[strTagname] = pProperty;
	m_propertyToId[pProperty] = strTagname;

	property->addSubProperty(pProperty);


	//Desc
	auto strAlarm = QStringLiteral("告警名");
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strAlarm);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarm->m_strDescription);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strAlarm;
	m_idToProperty[strAlarm] = pProperty;


	//ID
	auto strID = "ID";
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strID);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);

	m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pDigitalAlarm->m_nID);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strID;
	m_idToProperty[strID] = pProperty;


	auto strQualityGoodEnalbe = QStringLiteral("品质");
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strQualityGoodEnalbe);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pDigitalAlarm->m_bOnQualityGood);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strQualityGoodEnalbe;
	m_idToProperty[strQualityGoodEnalbe] = pProperty;



	auto strDelay = QStringLiteral("延时告警");
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strDelay);
	m_pUi->GetAtrri()->GetBoolProperty()->setValue(pProperty, pDigitalAlarm->m_nDelayAlarm);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = strDelay;
	m_idToProperty[strDelay] = pProperty;


	pTree->addProperty(property);
}

void CAlarm::CreateAttri(Config::CDinAlarmLimit *pDigitalAlarmLimit)
{
	Q_UNUSED(pDigitalAlarmLimit);
	
	CreateAttriConnect();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	Q_UNUSED(pTree);

	//pTree->clear();

	//m_idToProperty.clear();
	//m_propertyToId.clear();

	//QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	//auto strTmp = QStringLiteral("基本");
	//QtProperty *property = pGroup->addProperty(strTmp);
	//m_propertyToId[property] = strTmp;
	//m_idToProperty[strTmp] = property;


	//auto strName = QStringLiteral("限值名称");
	//QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Description);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	///*strName = QStringLiteral("报警时间格式");
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->TimeFormat);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	//strName = QStringLiteral("读访问级别");
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pDigitalAlarmLimit->ReadAccessLevel);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	//strName = QStringLiteral("写访问级别");
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pDigitalAlarmLimit->WriteAccessLevel);*/

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	//strName = QStringLiteral("条件");
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, pDigitalAlarmLimit->Condition);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	///*strName = QStringLiteral("限值");
	//pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetDoubleProperty()->setValue(pProperty, pDigitalAlarmLimit->Value);*/

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	////strName = QStringLiteral("限值低值");
	////pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strName);
	////m_pUi->GetAtrri()->GetDoubleProperty()->setValue(pProperty, pDigitalAlarmLimit->Value);

	////property->addSubProperty(pProperty);
	////m_propertyToId[pProperty] = strName;
	////m_idToProperty[strName] = pProperty;


	//strName = QStringLiteral("命令");
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);
	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Commands);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	//strName = QStringLiteral("1->0告警方式");
	//pProperty = pGroup->addProperty(strName);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("推画面");

	////
	//strName = QStringLiteral("文本色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("语音重复");
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pDigitalAlarmLimit->PlaySoundTimes);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("背景色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("闪烁背景色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("闪烁文本色");
	//pProperty = m_pUi->GetAtrri()->GetColorProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetColorProperty()->setValue(pProperty, QColor(Qt::black));

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;


	//strTmp = QStringLiteral("报警通知");
	//pProperty = pGroup->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToId[property] = strTmp;
	//m_idToProperty[strTmp] = property;

	////
	//strName = QStringLiteral("接受者");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("发送文本");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("电子邮件");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("语音电话");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("短信");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	////
	//strName = QStringLiteral("传真");
	//pTmp = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strName);

	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pDigitalAlarmLimit->Acceptor);

	//pProperty->addSubProperty(pTmp);
	//m_propertyToId[pProperty] = strName;
	//m_idToProperty[strName] = pProperty;

	//pTree->addProperty(property);
}

/*! \fn 
********************************************************************************************************* 
** \brief CAlarm::AddDigtalAlarm 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年6月17日 
** \note 
********************************************************************************************************/
void CAlarm::AddDigtalAlarm(QModelIndex &index)
{
	QString strFesTagname;

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
		QString strError = QString(tr("can not find fes tagname:%1!!!")).arg(strFesTagname);
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);
		return;
	}

	auto pGroupParent = reinterpret_cast<Config::CDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pGroupParent);
	if (!pGroupParent)
	{
		return;
	}

	Config::CDigitalAlarm *pTmp = pFes->AddNewDigitalAlarm();
	Q_ASSERT(pTmp);
	if (!pTmp)
	{
		QString strError = QString(tr("create digtal fail!!!"));
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);
		return;
	}

	if (!pGroupParent->AddDigitalItem(pTmp))
	{
		return;
	}

	//添加报警
	CQuoteItem *pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString strName = QString("%1%2").arg(ADD_ALARM_NAME).arg(0);

	CQuoteItem *pNewItem = new CQuoteItem(pTmp->m_szTagName);
	pNewItem->setData(pTmp->m_szTagName, LASTVALUE);
	pNewItem->setData(FES_TYPE_ALARM_DIGITAL_ITEM, Qt::UserRole);
	//pNewItem->setData(pTmp->m_szTagName, TAGNAME);
	pNewItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);
	pNewItem->setData(pTmp->m_strDescription, TREE_TOOL_TIP_ROLE);
	pNewItem->setIcon(QIcon(ALARM_PNG));
	//limit
	auto strLimitTagName = QString("%1_limit").arg(pTmp->m_szTagName);
	auto strLimitDesc = QString(tr("%1_limit")).arg(pTmp->m_szTagName);
	CQuoteItem *pLimitItem = new CQuoteItem(strLimitTagName);
	pLimitItem->setData(reinterpret_cast<long long>(pTmp), POINTERDATA);
	pLimitItem->setData(FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, Qt::UserRole);
	pLimitItem->setData(strLimitTagName, LASTVALUE);
	pLimitItem->setData(strLimitDesc, TREE_TOOL_TIP_ROLE);
	pLimitItem->setIcon(QIcon(ALARM_PNG));
	pNewItem->appendRow(pLimitItem);


	pItem->appendRow(pNewItem);

	m_pUi->GetLeftTree()->expand(index);
}

void CAlarm::Disconnect()
{
	disconnect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	disconnect(m_pUi->GetLeftTree(), SIGNAL(clicked(const QModelIndex &)), this, SLOT(Clicked(const QModelIndex &)));
}

bool CAlarm::CreateDigtalAlarm(Config::CFesData *pFes, Config::CDigitalAlarm *&pDigtal)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	Config::CDigitalAlarm *pDigtalTmp = new Config::CDigitalAlarm;

	int32u nAlarmCount = (int32u)pFes->m_arrDIAlarms.size();

	auto strTagname = QString("%1%2").arg(tr("digtalalarm")).arg(nAlarmCount + 1);

	auto strDesc = QString("%1%2").arg(tr("digtalalarmdesc")).arg(nAlarmCount + 1);

	memset(pDigtalTmp->m_szTagName, 0, sizeof(pDigtalTmp->m_szTagName));
	int nSize = strTagname.size();
	strncpy(pDigtalTmp->m_szTagName, strTagname.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
	pDigtalTmp->m_strDescription = strDesc;
	pDigtalTmp->m_bEnable = true;
	pDigtalTmp->m_bOnQualityGood = true;
	pDigtalTmp->m_nAlarmType = DIALARM;
	pDigtalTmp->m_nID = nAlarmCount + 1;
	pDigtalTmp->m_nDelayAlarm = 0;

	pFes->m_arrDIAlarms.push_back(pDigtalTmp);

	pDigtal = pDigtalTmp;

	return true;
}

/*! \fn bool CAlarm::FindAnalogGroup(QModelIndex &index, QString &strGroup)
********************************************************************************************************* 
** \brief CAlarm::FindAnalogGroup 
** \details 查找item的分组
** \param index 
** \param strGroup 
** \return bool 
** \author gw
** \date 2016年6月23日 
** \note 
********************************************************************************************************/
bool CAlarm::FindAnalogGroup(QModelIndex &index, QString &strGroup)
{
	QList<QString> list;
	list.clear();


	while (index.parent().isValid())
	{
		//int nTmp = index.parent().data(Qt::UserRole).toInt();

		if (index.parent().data(Qt::UserRole).toInt() == FES_ANALOG_ALARM_ITEM)
		{
			int nCount = list.count();

			for (int i = nCount; i > 0;  i--)
			{
				strGroup += list.at(i - 1) + ".";
			}

			break;
		}
		else if (index.parent().data(Qt::UserRole).toInt() == FES_TYPE_ANALOG_ALARM_GROUP_ITEM)
		{
			list.append(index.parent().data(Qt::EditRole).toString());

			index = index.parent();
		}
		else
		{
			index = index.parent();
		}
	}

	if (list.count() == 0)
	{
		return false;
	}

	int nIndex = strGroup.lastIndexOf(".");

	strGroup = strGroup.mid(0, nIndex);

	return true;
}

/*! \fn void CAlarm::FindAnalogAlarm(const QAbstractItemModel *pModel, const QModelIndex & top, const QString &s)
********************************************************************************************************* 
** \brief CAlarm::FindAnalogAlarm 
** \details 查找告警项
** \param pModel 
** \param top 
** \param s 
** \return void 
** \author gw
** \date 2016年6月23日 
** \note 
********************************************************************************************************/
void CAlarm::FindAnalogAlarm(const QAbstractItemModel *pModel, const QModelIndex & top, QVector<QModelIndex> &vec)
{
	QModelIndex Item;

	if (!top.isValid()) 
	{

	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const auto strTmp = Item.data(Qt::UserRole).toInt();

			if(strTmp == FES_TYPE_ALARM_ANALOG_ITEM)
			{
				vec.append(Item);
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			FindAnalogAlarm(pModel, Item, vec);
		}
	}
}

/*! \fn void CAlarm::AddAnalogOfflimit(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::AddAnalogOfflimit 
** \details 添加限值
** \param index 
** \return void 
** \author gw
** \date 2016年6月24日 
** \note 
********************************************************************************************************/
void CAlarm::AddAnalogOfflimit(QModelIndex &index)
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
	auto strFesTagname = indexTmp.parent().data(Qt::EditRole).toString();
	auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pFes);
	if (!pFes)
	{
		QString strError = QString(tr("can not find fes tagname:%1!!!")).arg(strFesTagname);
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);
		return;
	}

	auto *pAnalog = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAnalog);
	if (!pAnalog)
	{
		return;
	}

	Config::CAinAlarmLimit *pAnalogLimit = pFes->AddNewAinAlarmLimit();
	Q_ASSERT(pAnalogLimit);
	if (!pAnalogLimit)
	{
		return;
	}
	if (!pAnalog->AddAinAlarmLimit(pAnalogLimit))
	{
		return;
	}

	auto item = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	CQuoteItem *newItem = new CQuoteItem(pAnalogLimit->m_szTagName);
	newItem->setData(pAnalogLimit->m_szTagName, LASTVALUE);
	newItem->setData(FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, Qt::UserRole);
	//newItem->setData(pAnalogLimit->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pAnalogLimit), POINTERDATA);
	newItem->setData(pAnalogLimit->m_strDescription, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(ALARM_PNG));

	item->appendRow(newItem);
	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

void CAlarm::AddDigtalOfflimit(QModelIndex &index)
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
	auto strFesTagname = indexTmp.parent().data(Qt::EditRole).toString();
	auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
	Q_ASSERT(pFes);
	if (!pFes)
	{
		QString strError = QString(tr("can not find fes tagname:%1!!!")).arg(strFesTagname);
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);
		return;
	}

	auto *pDigital = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigital);
	if (!pDigital)
	{
		return;
	}

	Config::CDinAlarmLimit *pDigitalLimit = pFes->AddNewDigitalAlarmLimit();
	Q_ASSERT(pDigitalLimit);
	if (!pDigitalLimit)
	{
		return;
	}

	//添加到组内存结构
	if (!pDigital->AddDinAlarmLimit(pDigitalLimit))
	{
		return;
	}

	auto item = m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);

	CQuoteItem *newItem = new CQuoteItem(pDigitalLimit->TagName);
	newItem->setData(pDigitalLimit->TagName, LASTVALUE);
	newItem->setData(FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, Qt::UserRole);
	//newItem->setData(pDigitalLimit->TagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pDigitalLimit), POINTERDATA);
	newItem->setData(pDigitalLimit->Description, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(ALARM_PNG));

	item->appendRow(newItem);
	m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
}

bool CAlarm::CreateDigtalAlarmOfflimit(Config::CDigitalAlarm *pDigtalAlarm, Config::CDinAlarmLimit *&pDigtalAlarmOfflimit)
{
	Q_ASSERT(pDigtalAlarm);
	if (!pDigtalAlarm)
	{
		return false;
	}

	pDigtalAlarmOfflimit = new Config::CDinAlarmLimit;

	int nAlarmCount = (int)pDigtalAlarm->m_arrDigtalOfflimit.size();

	auto strTagname = QString("%1%2%3").arg(pDigtalAlarm->m_szTagName).arg("digtal alarm offlimit tag").arg(nAlarmCount + 1);
	auto strDesc = QString("%1%2%3").arg(pDigtalAlarm->m_strDescription).arg("digtal alarm offlimit desc").arg(nAlarmCount + 1);


	pDigtalAlarmOfflimit->ID = nAlarmCount + 1;
	memset(pDigtalAlarmOfflimit->TagName, 0, sizeof(pDigtalAlarmOfflimit->TagName));
	int nSize = strTagname.size();
	strncpy(pDigtalAlarmOfflimit->TagName, strTagname.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));
	pDigtalAlarmOfflimit->Description = strDesc;

	pDigtalAlarmOfflimit->Condition = GREATEREQUAL;

	pDigtalAlarmOfflimit->Priority = 0;
	pDigtalAlarmOfflimit->Commands = "";
	pDigtalAlarmOfflimit->SoundFile = "";
	pDigtalAlarmOfflimit->SupportAck = false;

	pDigtalAlarm->m_arrDigtalOfflimit.push_back(pDigtalAlarmOfflimit);

	return true;
}

/*! \fn void CFesModel::expanded(const QModelIndex &index)
*********************************************************************************************************
** \brief CFesModel::expanded
** \details 展开组
** \param index
** \return void
** \author gw
** \date 2016年7月29日
** \note
********************************************************************************************************/
void CAlarm::Expanded(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_ANALOG_ALARM_GROUP_ITEM || nType == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		m_pUi->GetLeftTreeModel();
		item->setIcon(QIcon(OPEN_GROUP_PNG));
	}
}

/*! \fn void CFesModel::collapsed(const QModelIndex &index)
*********************************************************************************************************
** \brief CFesModel::collapsed
** \details 折叠组
** \param index
** \return void
** \author gw
** \date 2016年7月29日
** \note
********************************************************************************************************/
void CAlarm::Collapsed(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_TYPE_ANALOG_ALARM_GROUP_ITEM || nType == FES_TYPE_DIGTAL_ALARM_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(CLOSE_GROUP_PNG));
	}
}


/*! \fn void CAlarm::DeleteAnalogAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::DeleteAnalogAlarmGroup 
** \details 删除模拟量告警组
** \param index 
** \return void 
** \author gw
** \date 2016年11月14日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteAnalogAlarmGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		//删除组的父组
		auto pGroupParent = reinterpret_cast<Config::CAnalogAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		//删除选中组
		auto pAnalogAlarmGroup = reinterpret_cast<Config::CAnalogAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarmGroup);
		if (!pAnalogAlarmGroup)
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

		QVector<QModelIndex> vec;
		vec.clear();
		//大组内的所有的告警项
		CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_ALARM_ANALOG_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTag = var.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strTag, FES_DESC))
			{
				return;
			}
			
			////////////////////////////////AI和USERVARAIBLE关联模拟量告警删除/////////////////////////////////////
			////删除关联告警 模拟量告警
			//pFes->DeleteAIsAlarmArr(strTag.toStdString());
			////模拟量告警删除,用户变量关联的告警hash删除
			//pFes->DeleteUserVariableAlarmArr(strTag.toStdString());
			
			//遍历删除通道相关项
			DeleteAnalogAlarmModuleWidgetInTabWidget(var, pFes);

			//删除hash管理告警项 不删除实际告警内存
			auto bFlag = pFes->DeleteFesHashItem(HASHID::ANALOGALARMHASHID, var.data().toString());
			if (!bFlag)
			{
				auto strError = tr("delete channel item fail!!!");

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pGroupParent->DeleteGroup(pAnalogAlarmGroup))
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


/*! \fn void CAlarm::DeleteDigitalAlarmGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::DeleteDigitalAlarmGroup 
** \details 删除开关量告警组
** \param index 
** \return void 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteDigitalAlarmGroup(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the group, its childs will be deleted!!!"));
	msgBox.setInformativeText(tr("Do you want to delete the group?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked
		//删除组的父组
		auto pGroupParent = reinterpret_cast<Config::CDigitalAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		//删除选中组
		auto pDigitalAlarmGroup = reinterpret_cast<Config::CDigitalAlarmGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmGroup);
		if (!pDigitalAlarmGroup)
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

		QVector<QModelIndex> vec;
		vec.clear();
		//大组内的所有的告警项
		CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_TYPE_ALARM_DIGITAL_ITEM, vec);
		for each (auto var in vec)
		{
			auto strTag = var.data(Qt::EditRole).toString();
			if (!CheckTagNameIsValid(strTag, FES_DESC))
			{
				return;
			}
			
			//////////////////////////DI和USERVARAIBLE关联开关量告警删除/////////////////////////////////
			////删除关联的告警
			//pFes->DeleteDIsAlarmArr(strTag.toStdString());
			////开关量告警删除,用户变量关联的告警hash删除
			//pFes->DeleteUserVariableAlarmArr(strTag.toStdString());
			
			//遍历删除通道窗口相关项
			DeleteDigitalAlarmModuleWidgetInTabWidget(var, pFes);

			//删除hash管理告警项 不删除实际告警内存
			auto bFlag = pFes->DeleteFesHashItem(HASHID::DIGITALALARMHASHID, var.data().toString());
			if (!bFlag)
			{
				auto strError = tr("delete channel item fail!!!");

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pGroupParent->DeleteGroup(pDigitalAlarmGroup))
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

/*! \fn bool CAlarm::DeleteAnalogAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
********************************************************************************************************* 
** \brief CAlarm::DeleteAnalogAlarmModuleWidgetInTabWidget 
** \details 关闭打开的模拟量告警view
** \param index 
** \param pFes 
** \return bool 
** \author gw
** \date 2016年11月14日 
** \note 
********************************************************************************************************/
bool CAlarm::DeleteAnalogAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	QVector<QModelIndex> vec;
	vec.clear();

	auto pAnalogAlarmItem = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAnalogAlarmItem);
	if (!pAnalogAlarmItem)
	{
		return false;
	}

	//模拟量告警窗口
	auto strTag = index.data(Qt::EditRole).toString();
	QString strFesTagName = pFes->m_szTagName;
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::FES_ANALOG_ALARM_TYPE, strFesTagName))
	{
		//删除模拟量告警窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIbase);
		pIbase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_ANALOG_LIMIT_ITEM, vec);
	for each (auto var in vec)
	{
		DeleteAnalogAlarmLimitModuleWidgetInTabWidget(var, pFes);

		for each (auto limit in pAnalogAlarmItem->m_arrAlarmOfflimit)
		{
			if (!pFes->DeleteFesHashItem(HASHID::ANALOGALARMLIMITHASHID, QString("%1").arg(limit->m_szTagName)))
			{
				auto strError = tr("delete AnalogAlarmLimit fail!!!");
				Q_ASSERT(m_pCore);
				if (!m_pCore)
				{
					return false;
				}

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}
	}

	//前置模拟量告警删除，如果有关联他的，关联关系删除
	if (!pFes->DeleteFesAnalogAlarmCombineRelaton(pAnalogAlarmItem->m_szTagName, pAnalogAlarmItem))
	{
		auto strError = tr("Fes %1  Delete FesAnalogAlarm %2 CombineRelaton fail!!!").arg(pFes->m_szTagName).arg(pAnalogAlarmItem->m_szTagName);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}

	return true;
}


/*! \fn bool CAlarm::DeleteDigitalAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
********************************************************************************************************* 
** \brief CAlarm::DeleteDigitalAlarmModuleWidgetInTabWidget 
** \details 关闭打开的开关量告警view
** \param index 
** \param pFes 
** \return bool 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
bool CAlarm::DeleteDigitalAlarmModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	QVector<QModelIndex> vec;
	vec.clear();

	auto pDigitalAlarmItem = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigitalAlarmItem);
	if (!pDigitalAlarmItem)
	{
		return false;
	}

	QString strFegTagName = pFes->m_szTagName;
	auto bFlag = strFegTagName.isEmpty() || strFegTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}

	//模拟量告警窗口
	auto strTag = index.data(Qt::EditRole).toString();
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::FES_DIGITAL_ALARM_TYPE, strFegTagName))
	{
		//删除开关量告警窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIbase);
		pIbase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM, vec);
	for each (auto var in vec)
	{
		DeleteDigitalAlarmLimitModuleWidgetInTabWidget(var, pFes);

		//delete hash limit
		for each (auto limit in pDigitalAlarmItem->m_arrDigtalOfflimit)
		{
			if (!pFes->DeleteFesHashItem(HASHID::DIGITALALARMLIMITHASHID, QString("%1").arg(limit->TagName)))
			{
				auto strError = tr("delete DigitalAlarmLimit fail!!!");
				Q_ASSERT(m_pCore);
				if (!m_pCore)
				{
					return false;
				}

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return false;
			}
		}
	}

	//前置开关量告警删除，如果有关联他的，关联关系删除
	if (!pFes->DeleteFesDigitalAlarmCombineRelaton(pDigitalAlarmItem->m_szTagName, pDigitalAlarmItem))
	{
		auto strError = tr("Fes %1  Delete FesDigitalAlarm %2 CombineRelaton fail!!!").arg(pFes->m_szTagName).arg(pDigitalAlarmItem->m_szTagName);
		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

		return false;
	}
	return true;
}


/*! \fn bool CAlarm::DeleteAnalogAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
********************************************************************************************************* 
** \brief CAlarm::DeleteAnalogAlarmLimitModuleWidgetInTabWidget 
** \details 关闭打开的模拟量告警限值
** \param index 
** \param pFes 
** \return bool 
** \author gw
** \date 2016年11月14日 
** \note 
********************************************************************************************************/
bool CAlarm::DeleteAnalogAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	QVector<QModelIndex> vec;
	vec.clear();

	auto pAlarm = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pAlarm);
	if (!pAlarm)
	{
		return false;
	}

	//装置窗口
	auto strTag = index.data(Qt::EditRole).toString();
	auto strFesTagName = pFes->m_szTagName;
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::FES_ANALOG_ALARM_LIMIT_TYPE, strFesTagName))
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

	return true;
}


/*! \fn bool CAlarm::DeleteDigitalAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
********************************************************************************************************* 
** \brief CAlarm::DeleteDigitalAlarmLimitModuleWidgetInTabWidget 
** \details 关闭打开的开关量告警限值
** \param index 
** \param pFes 
** \return bool 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
bool CAlarm::DeleteDigitalAlarmLimitModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}

	QVector<QModelIndex> vec;
	vec.clear();

	auto pDigital = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDigital);
	if (!pDigital)
	{
		return false;
	}
	QString strFegTagName = pFes->m_szTagName;
	auto bFlag = strFegTagName.isEmpty() || strFegTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return false;
	}


	//装置窗口
	auto strTag = QString("%1_limit").arg(pDigital->m_szTagName);
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::FES_DIGITAL_ALARM_LIMIT_TYPE, strFegTagName))
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

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CAlarm::DeleteAnalogAlarmItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月14日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteAnalogAlarmItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the analog alarm, the data will be deleted!!!"));
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		//删除项的父组
		auto pGroupParent = reinterpret_cast<Config::CAnalogAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pAnalogAlarmItem = reinterpret_cast<Config::CAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarmItem);
		if (!pAnalogAlarmItem)
		{
			return;
		}
		if (!CheckTagNameIsValid(pAnalogAlarmItem->m_szTagName, FES_DESC))
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


		DeleteAnalogAlarmModuleWidgetInTabWidget(index, pFes);

		QString strTag = QString("%1").arg(pAnalogAlarmItem->m_szTagName);
		//组内的告警项删除 删除通道内存
		if (!pGroupParent->DeleteItem(pAnalogAlarmItem))
		{
			auto strError = tr("delete analog alarm fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除告警项 不删除告警项内存
		if (!pFes->DeleteFesHashItem(HASHID::ANALOGALARMHASHID, strTag))
		{
			auto strError = tr("delete analog alarm fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		////////////////////////////////AI和USERVARAIBLE关联模拟量告警删除/////////////////////////////////////
		////删除关联告警 模拟量告警
		//pFes->DeleteAIsAlarmArr(strTag.toStdString());
		////模拟量告警删除,用户变量关联的告警hash删除
		//pFes->DeleteUserVariableAlarmArr(strTag.toStdString());

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

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
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


/*! \fn void CAlarm::DeleteDigitalAlarmItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::DeleteDigitalAlarmItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteDigitalAlarmItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the analog alarm, the channel data will be deleted!!!"));
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		//删除项的父组
		auto pGroupParent = reinterpret_cast<Config::CDigitalAlarmGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroupParent);
		if (!pGroupParent)
		{
			return;
		}

		auto pDigitalAlarmItem = reinterpret_cast<Config::CDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmItem);
		if (!pDigitalAlarmItem)
		{
			return;
		}
		if (!CheckTagNameIsValid(pDigitalAlarmItem->m_szTagName, FES_DESC))
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
		if (!CheckTagNameIsValid(pFes->m_szTagName, FES_DESC))
		{
			return;
		}

		DeleteDigitalAlarmModuleWidgetInTabWidget(index, pFes);

		auto strTag = index.data(Qt::EditRole).toString();
		if (CheckTagNameIsValid(strTag, FES_DESC))
		{
			return;
		}
		//组内的告警项删除 删除通道内存
		if (!pGroupParent->DeleteItem(pDigitalAlarmItem))
		{
			auto strError = tr("delete digital alarm fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除告警项 不删除告警项内存
		if (!pFes->DeleteFesHashItem(HASHID::DIGITALALARMHASHID, strTag))
		{
			auto strError = tr("delete digital alarm fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//////////////////////////DI和USERVARAIBLE关联开关量告警删除/////////////////////////////////
		////删除关联的告警
		//pFes->DeleteDIsAlarmArr(strTag.toStdString());
		////开关量告警删除,用户变量关联的告警hash删除
		//pFes->DeleteUserVariableAlarmArr(strTag.toStdString());

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

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
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


/*! \fn void CAlarm::DeleteAnalogAlarmLimitItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::DeleteAnalogAlarmLimitItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteAnalogAlarmLimitItem(QModelIndex &index)
{
	//QMessageBox msgBox;
	//msgBox.setText(tr("If u delete the analog alarm limit, the data will be deleted!!!"));
	//msgBox.setInformativeText("Do you want to delete the item?");
	//msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Yes);
	//int ret = msgBox.exec();
	//switch (ret)
	//{
	//case QMessageBox::Yes:
	//{
	//	// Save was clicked

	//	//删除项的父项
	//	auto pAnalogAlarmParent = reinterpret_cast<Config::CAnalogAlarm *>(index.parent().data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pAnalogAlarmParent);
	//	if (!pAnalogAlarmParent)
	//	{
	//		return;
	//	}

	//	auto pAnalogAlarmLimitItem = reinterpret_cast<Config::CAinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pAnalogAlarmLimitItem);
	//	if (!pAnalogAlarmLimitItem)
	//	{
	//		return;
	//	}

	//	auto indexTmp = index;
	//	while (indexTmp.parent().data(Qt::UserRole).toInt() != FES_ITEM)
	//	{
	//		indexTmp = indexTmp.parent();
	//	}
	//	auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.parent().data(POINTERDATA).toLongLong());
	//	Q_ASSERT(pFes);
	//	if (!pFes)
	//	{
	//		return;
	//	}


	//	DeleteAnalogAlarmLimitModuleWidgetInTabWidget(index, pFes);

	//	auto strTag = index.data(Qt::EditRole).toString();
	//	//组内的告警项删除 删除通道内存
	//	if (!pAnalogAlarmParent->DeleteItem(pAnalogAlarmLimitItem))
	//	{
	//		auto strError = tr("delete analog alarm limit fail!!!");

	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}

	//	//hash管理删除告警项 不删除告警项内存
	//	if (!pFes->DeleteFesHashItem(HASHID::ANALOGALARMLIMITHASHID, strTag))
	//	{
	//		auto strError = tr("delete analog alarm limit fail!!!");

	//		m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//		return;
	//	}


	//	//QList<QStandardItem *> carryItems; //Last In First Out stack of items
	//	//QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
	//	auto pTop = m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	//	//while (pTop->rowCount())
	//	//{
	//	//	itemsToBeDeleted << pTop->takeRow(0);
	//	//	//if the row removed has children:
	//	//	if (itemsToBeDeleted.at(0)->hasChildren())
	//	//	{
	//	//		carryItems << pTop;                 //put on the stack the current loopItem
	//	//		pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
	//	//	}
	//	//	//if current loopItem has no more rows but carryItems list is not empty:
	//	//	if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

	//	//}
	//	//qDeleteAll(itemsToBeDeleted);

	//	pTop->parent()->removeRow(pTop->row());

	//	//QVector<QModelIndex> vec;
	//	//vec.clear();
	//	//GetItemMap(m_pModel, index, FES_ITEM, vec);
	//	break;
	//}
	//case QMessageBox::Cancel:
	//	// Cancel was clicked
	//	break;
	//default:
	//	// should never be reached
	//	break;
	//}
}


/*! \fn void CAlarm::DeleteDigitalAlarmLimitItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CAlarm::DeleteDigitalAlarmLimitItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年11月15日 
** \note 
********************************************************************************************************/
void CAlarm::DeleteDigitalAlarmLimitItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText(tr("If u delete the digital limit, the data will be deleted!!!"));
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		//删除项的父项
		auto pDigitalAlarmParent = reinterpret_cast<Config::CDigitalAlarm *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmParent);
		if (!pDigitalAlarmParent)
		{
			return;
		}

		auto pDigitalAlarmLimitItem = reinterpret_cast<Config::CDinAlarmLimit *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarmLimitItem);
		if (!pDigitalAlarmLimitItem)
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


		DeleteAnalogAlarmLimitModuleWidgetInTabWidget(index, pFes);

		auto strTag = index.data(Qt::EditRole).toString();
		//组内的告警项删除 删除通道内存
		if (!pDigitalAlarmParent->DeleteItem(pDigitalAlarmLimitItem))
		{
			auto strError = tr("delete digital alarm limit fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除告警项 不删除告警项内存
		if (!pFes->DeleteFesHashItem(HASHID::DIGITALALARMLIMITHASHID, strTag))
		{
			auto strError = tr("delete digital alarm limit fail!!!");

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

		//QVector<QModelIndex> vec;
		//vec.clear();
		//GetItemMap(m_pModel, index, FES_ITEM, vec);
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