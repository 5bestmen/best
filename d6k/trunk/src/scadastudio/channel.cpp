/*! @file
<PRE>
********************************************************************************
模块名       :  通道
文件名       :  channel.cpp
文件实现功能 :  通道 装置
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
*  @date    2015.11.23
*/

#include "channel.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/uimgr.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"
#include "standarview.h"
#include "scadastudio/tabwidget.h"
#include "channelview.h"
#include "channelmodel.h"
#include "attriwindow/attriwindow.h"
#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qteditorfactory.h"
#include "scadastudio/quoteItem.h"
#include "deviceview.h"

#include "logdefine.h"
#include "scadastudio/icore.h"
#include "aimodel.h"
#include "dimodel.h"
#include "aomodel.h"
#include "domodel.h"
#include "fesmodule.h"
#include "scadastudio/define.h"
#include "aiview.h"
#include "diview.h"
#include "aoview.h"
#include "doview.h"
#include "exportmodeldialog.h"
#include "scadastudio/iprojmgr.h"
#include "global.h"

#include <QMenu>
#include <QRegExp>
#include <QStandardItemModel>
#include <QFile>
#include <QDomDocument>
#include <QTextStream>
#include <QMessageBox>

//#include "uimodule/outputmessagerclient.h"
//#include "uimodule/outputmessagerclient.h"
#include <QGraphicsView>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

/*! \fn CChannel::CChannel(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CChannel::CChannel 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CChannel::CChannel(IMainModuleInterface *pCore)
	: QObject(pCore->GetUIMgr()), m_pDriverParaInfo(NULL), m_bConnectAttr(false), m_pCurrentTabItem(NULL),
	m_bCreateAIAttr(false), m_pAIModel(nullptr), m_pDOModel(nullptr), m_pModel(nullptr)
{
	m_pUi = pCore->GetUIMgr();
	
	m_pAttr = m_pUi->GetAtrri();

	connect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	connect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	connect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	connect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));

	connect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));

	//m_pArrChannel = pArrChannel;
	//m_pArrChannel = new std::vector<Config::CChannelData *>;
	//for (size_t i = 0; i < m_arrFes.size(); i++)
	//{
	//	m_pArrChannel->insert(m_pArrChannel->end(), m_arrFes[i]->m_arrChannels.begin(), m_arrFes[i]->m_arrChannels.end());
	//}

	//m_pClient = new COutputMessagerClient(this);

	m_pCore = pCore;
}

/*! \fn CChannel::~CChannel()
********************************************************************************************************* 
** \brief CChannel::~CChannel 
** \details 
** \return  
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CChannel::~CChannel()
{

}

/*! \fn void CChannel::AddChannel()
********************************************************************************************************* 
** \brief CChannel::AddChannel 
** \details 
** \return void 
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
void CChannel::AddChannel(QModelIndex &index)
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
	
	auto nType = item->data(Qt::UserRole).toInt();
	if (nType == FES_CHANNEL_ITEM)
	{
		auto pFes = GetFes(index);
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("fes pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}

		auto pGroup = reinterpret_cast<Config::CChannelGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return;
		}

		auto pChannel = pFes->AddNewChannelItem(pGroup);
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			auto strError = QString(tr("add channel fail!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		CQuoteItem *newItem = new CQuoteItem(pChannel->m_szTagName);

		newItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);
		newItem->setData(pChannel->m_szTagName, LASTVALUE);
		//Tagname
		//newItem->setData(pChannel->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		newItem->setData(pChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(CHANNEL_PNG));
		item->appendRow(newItem);

		m_pUi->GetLeftTree()->expand(index);
	}
	else if (nType == FES_CHANNEL_GROUP_ITEM)
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

		
		auto pGroup = reinterpret_cast<Config::CChannelGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = tr("group pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pChannel = pFes->AddNewChannelItem(pGroup);
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			auto strError = QString(tr("add channel fail!!!"));
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		CQuoteItem *newItem = new CQuoteItem(pChannel->m_szTagName);

		newItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);
		newItem->setData(pChannel->m_szTagName, LASTVALUE);
		//Tagname
		//newItem->setData(pChannel->m_szTagName, TAGNAME);
		newItem->setData(reinterpret_cast<long long>(pChannel), POINTERDATA);
		newItem->setData(pChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(CHANNEL_PNG));
		item->appendRow(newItem);

		m_pUi->GetLeftTree()->expand(index);
	}
}

/*! \fn void CChannel::AddChannelGroup()
********************************************************************************************************* 
** \brief CChannel::AddChannelGroup 
** \details 
** \return void 
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
void CChannel::AddChannelGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(CHANNEL_GROUP_NAME).arg(0);

	int nType = item->data(Qt::UserRole).toInt();

	//通道组和前置节点
	if (nType == FES_CHANNEL_GROUP_ITEM)
	{
		//auto nTest = item->data(GROUP_NUMBER_ROLE).toInt();
		if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			auto pChannelGroup = reinterpret_cast<Config::CChannelGroup *>(index.data(POINTERDATA).toLongLong());
			Q_ASSERT(pChannelGroup);
			if (!pChannelGroup)
			{
				return;
			}

			auto pChannelGroupChild = pChannelGroup->CreateGroup();
			Q_ASSERT(pChannelGroupChild);
			if (!pChannelGroupChild)
			{
				return;
			}

			QString strName = pChannelGroupChild->m_strGroup;

			CQuoteItem *newItem = new CQuoteItem(strName);
			newItem->setData(strName, LASTVALUE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(reinterpret_cast<long long>(pChannelGroupChild), POINTERDATA);
			newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}
	else if (nType == FES_CHANNEL_ITEM)
	{
		auto pFes = reinterpret_cast<Config::CFesData *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("add channel group fail!!!");
			
			return;
		}

		Q_ASSERT(pFes->m_pChannelGroup);
		if (!pFes->m_pChannelGroup)
		{
			return;
		}

		auto pChannelGroup = pFes->m_pChannelGroup->CreateGroup();
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			auto strError = tr("add channel group fail!!!");

			return;
		}

		if (item->data(GROUP_NUMBER_ROLE).toInt() < MAX_GROUP_NUM)
		{
			QString strName = pChannelGroup->m_strGroup;
			CQuoteItem *newItem = new CQuoteItem(strName);
			newItem->setData(strName, LASTVALUE);
			newItem->setData(item->data(GROUP_NUMBER_ROLE).toInt() + 1, GROUP_NUMBER_ROLE);
			newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);
			newItem->setData(reinterpret_cast<long long>(pChannelGroup), POINTERDATA);

			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

			item->appendRow(newItem);

			m_pUi->GetLeftTree()->expand(index);
		}
	}

	m_pUi->GetLeftTree()->expand(index);
}


/*! \fn void CChannel::ShowMenu()
********************************************************************************************************* 
** \brief CChannel::ShowMenu 
** \details 右键菜单
** \return void 
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
void CChannel::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	if (index.data(Qt::UserRole).toInt() == FES_CHANNEL_ITEM ||
		index.data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		QAction *pChannelAction = new QAction(CHANNEL_NAME, pMenu);

		pChannelAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pChannelAction);

		
		QAction *pActionGroup = new QAction(CHANNEL_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);


		QAction *pActionMofidyGroup = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pActionMofidyGroup);

		pActionMofidyGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionMofidyGroup);


		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pChannelAction)
		{
			//添加通道
			AddChannel(index);
		}
		else if(selectedItem == pActionGroup)
		{
			//添加通道组
			AddChannelGroup(index);
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
			if (index.data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
			{
				DeleteChannelGroup(index);
			}
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
	{
		//通道配置
		QAction *pDeviceAction = new QAction(DEVICE_NAME, pMenu);
		pMenu->addAction(pDeviceAction);
		pDeviceAction->setIcon(QIcon(DEVICE_PNG));
		
		QAction *pAction = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pAction);

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);

		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pDeviceAction)
		{
			//添加装置
			AddDevice(index);
		}
		else if(selectedItem == pAction)
		{
			//配置通道
			ModifyChannel(index);
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
			DeleteChannelItem(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{
		auto *pExportModelAction = new QAction(tr("export it to be device model"), pMenu);
		pMenu->addAction(pExportModelAction);
		
		QAction *pDeviceAction = new QAction(RENAME_ITEM, pMenu);
		pMenu->addAction(pDeviceAction);
		

		QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
		pMenu->addAction(pActionExpand);


		QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
		pMenu->addAction(pActionCollapse);

		QAction *pActionDelete = new QAction(DELETE_ITEM, pMenu);
		pMenu->addAction(pActionDelete);

		QAction* selectedItem = pMenu->exec(QCursor::pos());
		if (selectedItem == pExportModelAction)
		{
			DeviceExportToBeModelAction(index);
		}
		else if (selectedItem == pDeviceAction)
		{
			MofifyDevice(index);
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
			DeleteDeviceItem(index);
		}
	}
	else if (index.data(Qt::UserRole).toInt() == FES_DEVICE_AI ||
		index.data(Qt::UserRole).toInt() == FES_DEVICE_DI ||
		index.data(Qt::UserRole).toInt() == FES_DEVICE_AO ||
		index.data(Qt::UserRole).toInt() == FES_DEVICE_DO)
	{
		//QAction *pActionMofidy = new QAction(RENAME_ITEM, pMenu);
		//pMenu->addAction(pActionMofidy);
		//pActionMofidy->setIcon(QIcon(CLOSE_GROUP_PNG));
		//pMenu->addAction(pActionMofidy); 

		//QAction* selectedItem = pMenu->exec(QCursor::pos());
		//if (selectedItem == pActionMofidy)
		//{
		//	m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
		//}
	}

	pMenu->deleteLater();
}

/*! \fn void CChannel::AddDevice()
********************************************************************************************************* 
** \brief CChannel::AddDevice 
** \details 
** \return void 
** \author gw
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
void CChannel::AddDevice(QModelIndex &index)
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
	if (!pFes)
	{
		auto strError = QString(tr("fes %1 can find!!!"));
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}
	
	//添加装置
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	auto strChannelTag = index.data(Qt::EditRole).toString();
	auto pChannel = reinterpret_cast<Config::CChannelData *>(item->data(POINTERDATA).toLongLong());
	Q_ASSERT(pChannel);
	if (!pChannel)
	{
		auto strError = QString(tr("channel %1 can find!!!")).arg(strChannelTag);
		m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	auto pDevice = pFes->AddNewDeviceItem();
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		return;
	}

	CQuoteItem *newItem = new CQuoteItem(pDevice->m_szTagName);
	newItem->setData(pDevice->m_szTagName, LASTVALUE);
	newItem->setData(FES__CHANNEL_CHILD_DEVICE_ITEM, Qt::UserRole);
	//newItem->setData(pDevice->m_szTagName, TAGNAME);
	newItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
	newItem->setData(pDevice->m_strDescription, TREE_TOOL_TIP_ROLE);
	newItem->setIcon(QIcon(DEVICE_PNG));

	////////////////////////////////////////////////////////////////////////////////////////////////////
	auto strDesc = QString("%1_%2").arg(pDevice->m_strDescription).arg((tr("ai")));
	QString strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("ai");
	auto *pSubItem = new CQuoteItem(strAITag);
	pSubItem->setData(strAITag, LASTVALUE);
	pSubItem->setData(FES_DEVICE_AI, Qt::UserRole);
	//pSubItem->setData(strAITag, TAGNAME);
	pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
	pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
	pSubItem->setIcon(QIcon(FES_PNG));
	pSubItem->setEditable(true);
	newItem->appendRow(pSubItem);

	strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("di"));
	strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("di");
	pSubItem = new CQuoteItem(strAITag);
	pSubItem->setData(strAITag, LASTVALUE);
	pSubItem->setData(FES_DEVICE_DI, Qt::UserRole);
	//pSubItem->setData(strAITag, TAGNAME);
	pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
	pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
	pSubItem->setIcon(QIcon(FES_PNG));
	pSubItem->setEditable(true);
	newItem->appendRow(pSubItem);

	strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("ao"));
	strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("ao");
	pSubItem = new CQuoteItem(strAITag);
	pSubItem->setData(strAITag, LASTVALUE);
	pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
	//pSubItem->setData(strAITag, TAGNAME);
	pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
	pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
	pSubItem->setIcon(QIcon(FES_PNG));
	pSubItem->setEditable(true);
	newItem->appendRow(pSubItem);

	strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("do"));
	strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("do");
	pSubItem = new CQuoteItem(strAITag);
	pSubItem->setData(strAITag, LASTVALUE);
	pSubItem->setData(FES_DEVICE_DO, Qt::UserRole);
	strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("do");
	//pSubItem->setData(strAITag, TAGNAME);
	pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
	pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
	pSubItem->setIcon(QIcon(FES_PNG));
	pSubItem->setEditable(true);
	newItem->appendRow(pSubItem);
	////////////////////////////////////////////////////////////////////////////////////////////////////

	item->appendRow(newItem);

	pChannel->AddDevice(pDevice);

	m_pUi->GetLeftTree()->expand(index);
}

/*! \fn void CChannel::ConfigDevice(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::ConfigDevice 
** \details 配置装置
** \param index 
** \return void 
** \author gw
** \date 2016年8月22日 
** \note 
********************************************************************************************************/
void CChannel::MofifyDevice(QModelIndex &index)
{
	m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
}

/*! \fn void CChannel::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CChannel::showMouseRightButton 
** \details 右键菜单
** \param point 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CChannel::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_CHILD_ITEM || nType == FES_CHANNEL_GROUP_ITEM
	|| nType == FES__CHANNEL_CHILD_DEVICE_ITEM || nType == FES_DEVICE_AI ||
	nType == FES_DEVICE_DI || nType == FES_DEVICE_AO || nType == FES_DEVICE_DO)
	{
		ShowMenu(indexSelect);
	}
}
/*! \fn void CChannel::ConfigChannel(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::ConfigChannel 
** \details 配置通道
** \param index 
** \return void 
** \author gw
** \date 2015年12月11日 
** \note 
********************************************************************************************************/
void CChannel::ModifyChannel(QModelIndex &index)
{
	m_pCore->GetUIMgr()->GetLeftTree()->edit(index);
}

/*! \fn void CChannel::DeleteChannelItem(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::DeleteChannelItem 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2016年10月24日 
** \note 
********************************************************************************************************/
void CChannel::DeleteChannelItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the channel, the channel data will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the item?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pChannelGroupParent = reinterpret_cast<Config::CChannelGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroupParent);
		if (!pChannelGroupParent)
		{
			return;
		}

		auto pChannelItem = reinterpret_cast<Config::CChannelData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelItem);
		if (!pChannelItem)
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


		DeleteChannelModuleWidgetInTabWidget(index, pFes);

		auto strTag = index.data(Qt::EditRole).toString();
		//组内的通道项删除 删除通道内存
		if (!pChannelGroupParent->DeleteItem(strTag))
		{
			auto strError = tr("delete channel fail!!!");

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//hash管理删除通道 不删除通道内存
		if (!pFes->DeleteFesHashItem(CHANNELHASHID, strTag))
		{
			auto strError = tr("delete channel fail!!!");

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


/*! \fn void CChannel::DeleteChannelGroup(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::DeleteChannelGroup 
** \details 删除通道组
** \param index 
** \return void 
** \author gw
** \date 2016年10月24日 
** \note 
********************************************************************************************************/
void CChannel::DeleteChannelGroup(QModelIndex &index)
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

		auto pChannelGroupParent = reinterpret_cast<Config::CChannelGroup *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroupParent);
		if (!pChannelGroupParent)
		{
			return;
		}

		auto pChannelGroup = reinterpret_cast<Config::CChannelGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
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
		//大组内的所有的通道项
		CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_CHANNEL_CHILD_ITEM, vec);
		for each (auto var in vec)
		{
			//遍历删除通道相关项
			DeleteChannelModuleWidgetInTabWidget(var, pFes);

			//删除hash管理通道项 不删除实际前置内存
			auto bFlag = pFes->DeleteFesHashItem(CHANNELHASHID, var.data().toString());
			if (!bFlag)
			{
				auto strError = tr("delete channel item fail!!!");

				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		auto strTag = index.data(Qt::EditRole).toString();
		if (!pChannelGroupParent->DeleteGroup(strTag))
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


void CChannel::DeleteDeviceItem(QModelIndex &index)
{
	QMessageBox msgBox;
	msgBox.setText("If u delete the device, device data will be deleted!!!");
	msgBox.setInformativeText("Do you want to delete the device?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	switch (ret)
	{
	case QMessageBox::Yes:
	{
		// Save was clicked

		auto pChannel = reinterpret_cast<Config::CChannelData *>(index.parent().data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}

		auto pDevice = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
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

		DeleteDeviceModuleWidgetInTabWidget(index, pFes);

		//删除内存和hash
		if (!pFes->DeleteFesHashItem(HASHID::DEVICEHASHID, pDevice->m_szTagName))
		{
			auto strError = tr("delete device hash fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		if (!pChannel->DeleteDevice(pDevice))
		{
			auto strError = tr("delete device fail!!!");
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


/*! \fn void CChannel::DeleteDeviceModuleWidgetInTabWidget(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::DeleteDeviceModuleWidgetInTabWidget 
** \details 窗口包括装置窗口、装置AI窗口、装置DI窗口、装置AO窗口和装置DO窗口
** \param index 
** \return void 
** \author gw
** \date 2016年10月26日 
** \note 
********************************************************************************************************/
void CChannel::DeleteDeviceModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	if (!CheckTagNameIsValid(pFes->m_szTagName, FES_DESC))
	{
		return;
	}

	QVector<QModelIndex> vec;
	vec.clear();

	auto pDevice = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		return;
	}

	{
		QString strFesTagName = pFes->m_szTagName;
		//装置窗口
		auto strTag = pDevice->m_szTagName;
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_WIDGET_TYPE, strFesTagName))
		{
			//删除打开的前置窗口
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
	}

	vec.clear();
	//装置AI窗口
	CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_DEVICE_AI, vec);
	for each (auto var in vec)
	{
		//
		auto strTag = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
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
	}

	vec.clear();
	//装置DI窗口
	CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_DEVICE_DI, vec);
	for each (auto var in vec)
	{
		//
		auto strTag = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
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
	}

	vec.clear();
	//装置AO窗口
	CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_DEVICE_AO, vec);
	for each (auto var in vec)
	{
		//
		auto strTag = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
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
	}

	vec.clear();
	//装置DO窗口
	CFesModule::GetItemMap(m_pUi->GetLeftTreeModel(), index, FES_DEVICE_DO, vec);
	for each (auto var in vec)
	{
		//
		auto strTag = var.data(Qt::EditRole).toString();
		if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE, pFes->m_szTagName))
		{
			//删除打开的前置窗口
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
	}

	QString strTmp;
	//删除hash管理 ais
	for each (auto var in pDevice->m_arrAIs)
	{
		strTmp = var->m_szTagName;
		if (!pFes->DeleteFesHashItem(AIHASHID, strTmp))
		{
			Q_ASSERT(m_pCore);
			if (!m_pCore)
			{
				auto strError = tr("delete ai fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		//删除前置AI相关的关联和被关联的关系
		if (!pFes->DeleteFesDeviceAICombineRelaton(var->m_szTagName, var))
		{
			auto strError = tr("Delete Fes Device AI tagname %1 CombineRelaton fail!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
	}

	//删除hash管理 dis
	for each (auto var in pDevice->m_arrDIs)
	{
		strTmp = var->m_szTagName;
		if (!pFes->DeleteFesHashItem(DIHASHID, strTmp))
		{
			Q_ASSERT(m_pCore);
			if (!m_pCore)
			{
				auto strError = tr("delete di fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		//删除前置DI相关的关联和被关联的关系
		if (!pFes->DeleteFesDeviceDICombineRelaton(var->m_szTagName, var))
		{
			auto strError = tr("Delete Fes Device DI tagname %1 CombineRelaton fail!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
	}

	//删除hash管理 aos
	for each (auto var in pDevice->m_arrAOs)
	{
		strTmp = var->m_szTagName;
		if (!pFes->DeleteFesHashItem(AOHASHID, strTmp))
		{
			Q_ASSERT(m_pCore);
			if (!m_pCore)
			{
				auto strError = tr("delete ao fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		//删除前置DI相关的关联和被关联的关系
		if (!pFes->DeleteFesDeviceAOCombineRelaton(var->m_szTagName, var))
		{
			auto strError = tr("Delete Fes Device AO tagname %1 CombineRelaton fail!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
	}

	//删除hash管理 dos
	for each (auto var in pDevice->m_arrDOs)
	{
		strTmp = var->m_szTagName;
		if (!pFes->DeleteFesHashItem(DOHASHID, strTmp))
		{
			Q_ASSERT(m_pCore);
			if (!m_pCore)
			{
				auto strError = tr("delete do fail!!!");
				m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

				return;
			}
		}

		if (!pFes->DeleteFesDeviceDOCombineRelaton(var->m_szTagName, var))
		{
			auto strError = tr("Delete Fes Device DO tagname %1 CombineRelaton fail!!!").arg(var->m_szTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
	}
}

/*! \fn void CChannel::DeleteChannelModuleWidgetInTabWidget(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::DeleteChannelModuleWidgetInTabWidget 
** \details 窗口包括通道窗口、装置窗口、装置AI窗口、装置DI窗口、装置AO窗口和装置DO窗口
** \param index 
** \return void 
** \author gw
** \date 2016年10月26日 
** \note 
********************************************************************************************************/
void CChannel::DeleteChannelModuleWidgetInTabWidget(QModelIndex &index, Config::CFesData *pFes)
{
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return;
	}
	
	QVector<QModelIndex> vec;
	vec.clear();

	auto pChannelItem = reinterpret_cast<Config::CChannelData *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pChannelItem);
	if (!pChannelItem)
	{
		return;
	}

	if (!CheckTagNameIsValid(pFes->m_szTagName, FES_DESC))
	{
		return;
	}

	QString strFesTagName = pFes->m_szTagName;
	auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
	Q_ASSERT(!bFlag);
	if (bFlag)
	{
		return;
	}
	//通道窗口
	auto strTag = index.data(Qt::EditRole).toString();
	if (m_pCore->GetUIMgr()->GetTabWidget()->IsWidgetInTab(strTag, WIDGET_TYPE::CHANNEL_WIDGET_TYPE, strFesTagName))
	{
		//删除打开的前置窗口
		int nIndex = m_pCore->GetUIMgr()->GetTabWidget()->currentIndex();
		//model view detach
		auto pIbase = dynamic_cast<IBase *>(m_pCore->GetUIMgr()->GetTabWidget()->currentWidget());
		Q_ASSERT(pIbase);
		pIbase->Delete();

		m_pCore->GetUIMgr()->GetTabWidget()->closeTileWidget(nIndex);
	}

	CFesModule::GetItemMap(m_pCore->GetUIMgr()->GetLeftTreeModel(), index, FES__CHANNEL_CHILD_DEVICE_ITEM, vec);
	for each (auto var in vec)
	{
		DeleteDeviceModuleWidgetInTabWidget(var, pFes);

		if (!pFes->DeleteFesHashItem(DEVICEHASHID, var.data(Qt::EditRole).toString()))
		{
			auto strError = tr("delete device fail!!!");
			Q_ASSERT(m_pCore);
			if (!m_pCore)
			{
				return;
			}

			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			
			return;
		}

	}
}

/*! \fn void CChannel::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::doubleClicked 
** \details 装置
** \param index 
** \return void 
** \author gw
** \date 2015年12月11日 
** \note 
********************************************************************************************************/
void CChannel::doubleClicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_CHANNEL_CHILD_ITEM)
	{	
		//配置通道
		QString strChannelDesc = index.data().toString();

		//int nType = index.data(Qt::UserRole).toInt();
		QString strTagChannelName = index.data(Qt::EditRole).toString();

		auto indexTmp = index;

		//向上遍历查找FES
		auto bFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//前置TagName
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("fes tagname is empty!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagChannelName, WIDGET_TYPE::CHANNEL_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("can not find fes!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		auto pChannel = reinterpret_cast<Config::CChannelData *>(index.data(POINTERDATA).toLongLong());
		if (!pChannel)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		m_pUi->GetTabWidget()->m_fnBeforeCloseTab = std::bind(&CChannel::CallBackBeforeCloseTab, this, std::placeholders::_1);

		//通道挂载对应的前置,通道修改,此前置整个保存
		CChannelView *pDialog = new CChannelView(m_pUi, pChannel, (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index), this, pFes, m_pCore);

		auto &icon = m_pUi->GetLeftTreeModel()->itemFromIndex(index)->icon();
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagChannelName;
		keyInfo.Type = WIDGET_TYPE::CHANNEL_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pDialog, strTagChannelName, keyInfo, icon);

		m_pUi->GetTabWidget()->setCurrentWidget(pDialog);
	}
	else if (nType == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{
		//配置通道
		QString strDeviceDesc = index.data().toString();

		//int nType = index.data(Qt::UserRole).toInt();
		QString strTagDeviceName = index.data(Qt::EditRole).toString();

		//向上遍历查找FES
		auto indexTmp = index;
		auto bFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			auto strError = tr("can not find fes!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}


		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagDeviceName, WIDGET_TYPE::DEVICE_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto pDevice = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			auto strError = tr("Config device	device pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pItem = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return;
		}

		auto item = (CQuoteItem *)m_pCore->GetUIMgr()->GetLeftTreeModel()->itemFromIndex(index);
		CDeviceView *pView = new CDeviceView(m_pCore, pDevice, strTagDeviceName, item, this, pFes);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagDeviceName;
		keyInfo.Type = WIDGET_TYPE::DEVICE_WIDGET_TYPE;
		
		auto &icon = item->icon();
		m_pUi->GetTabWidget()->AddTab(pView, strTagDeviceName, keyInfo, icon);

		m_pUi->GetTabWidget()->setCurrentWidget(pView);

		if (pItem->hasChildren())
		{
			return;
		}

		auto strDesc = QString("%1_%2").arg(pDevice->m_strDescription).arg((tr("ai")));
		QString strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("ai");
		auto *pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(true);
		pItem->appendRow(pSubItem);

		strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("di"));
		strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("di");
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(true);
		pItem->appendRow(pSubItem);

		strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("ao"));
		strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("ao");
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(true);
		pItem->appendRow(pSubItem);

		strDesc = QString("%1_%2").arg(pDevice->m_szTagName).arg(tr("do"));
		strAITag = QString("%1_%2").arg(pDevice->m_szTagName).arg("do");
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
		
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pDevice), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(true);
		pItem->appendRow(pSubItem);

		m_pCore->GetUIMgr()->GetLeftTree()->expand(index);
	}
	else if (nType == FES_DEVICE_AI)
	{
		auto indexTmp = index;

		//向上遍历查找FES
		bool bFesFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFesFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFesFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("Not to find fes!!!"), LEVEL_1);

			return;
		}
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		//遥测
		auto strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}
		
		auto var = index.data(POINTERDATA);
		auto pDevice = reinterpret_cast<Config::CDevice *>(var.toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			auto strError = QString("device tag %1 can not find!!!").arg(strTagName);
			m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		//auto strFesTagname = CFesModule::GetTagname(index, FES_ITEM);
		//auto *pFes = CFesModule::GetFes(m_arrFes, strFesTagname);
		//Q_ASSERT(pFes);
		//if (!pFes)
		//{
		//	auto strError = QString("fes tag %1 can not find!!!").arg(strFesTagname);
		//	m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);
		//	
		//	return;
		//}
		auto pView = new CAIView(nullptr, m_pCore, pDevice, strTagName, 
			(CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index), this, pFes);
		
		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE;

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, QIcon(FES_PNG));
		m_pUi->GetTabWidget()->setCurrentWidget(pView);
	}
	else if (nType == FES_DEVICE_DI)
	{
		auto indexTmp = index;

		//向上遍历查找FES
		bool bFesFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFesFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFesFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("Not to find fes!!!"), LEVEL_1);

			return;
		}
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		//遥信
		auto strDesc = index.data().toString();
		auto strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto var = index.data(POINTERDATA);
		auto pDevice = reinterpret_cast<Config::CDevice *>(var.toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			auto strError = QString("device tag %1 can not find!!!").arg(strTagName);
			m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		//auto strFesTagname = CFesModule::GetTagname(index, FES_ITEM);
		//auto *pFes = CFesModule::GetFes(m_arrFes, strFesTagname);
		//Q_ASSERT(pFes);
		//if (!pFes)
		//{
		//	auto strError = QString("fes tag %1 can not find!!!").arg(strFesTagname);
		//	m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

		//	return;
		//}

		auto pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		auto pView = new CDIView(nullptr, m_pCore, pDevice, strTagName, pItem, this, pFes);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE;
		auto &icon = pItem->icon();
		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
		m_pUi->GetTabWidget()->setCurrentWidget(pView);
	}
	else if (nType == FES_DEVICE_AO)
	{
		auto indexTmp = index;

		//向上遍历查找FES
		bool bFesFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFesFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFesFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("Not to find fes!!!"), LEVEL_1);

			return;
		}
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		auto strDesc = index.data().toString();
		auto strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto var = index.data(POINTERDATA);
		auto pDevice = reinterpret_cast<Config::CDevice *>(var.toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			auto strError = QString("device tag %1 can not find!!!").arg(strTagName);
			m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		//auto strFesTagname = CFesModule::GetTagname(index, FES_ITEM);
		//auto *pFes = CFesModule::GetFes(m_arrFes, strFesTagname);
		//Q_ASSERT(pFes);
		//if (!pFes)
		//{
		//	auto strError = QString("fes tag %1 can not find!!!").arg(strFesTagname);
		//	m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

		//	return;
		//}

		auto pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		auto pView = new CAOView(nullptr, m_pCore, pDevice, strTagName, pItem, this, pFes);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE;
		auto &icon = pItem->icon();

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
		m_pUi->GetTabWidget()->setCurrentWidget(pView);
	}
	else if (nType == FES_DEVICE_DO)
	{
		auto indexTmp = index;

		//向上遍历查找FES
		bool bFesFlag = false;
		while (indexTmp.parent().isValid())
		{
			if (indexTmp.parent().data(Qt::UserRole) == FES_ITEM)
			{
				indexTmp = indexTmp.parent();
				bFesFlag = true;
				break;
			}

			indexTmp = indexTmp.parent();
		}

		if (!bFesFlag)
		{
			auto strError = tr("Config channel channel pointer is nullptr!!!");
			m_pCore->LogMsg(CHANNEL_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		auto pFes = reinterpret_cast<Config::CFesData *>(indexTmp.data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			m_pCore->LogMsg(FES_DESC, LOG_STRING("Not to find fes!!!"), LEVEL_1);

			return;
		}
		auto strFesTagName = indexTmp.data(Qt::EditRole).toString();
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}
		
		auto strDesc = index.data().toString();
		auto strTagName = index.data(Qt::EditRole).toString();

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strTagName, WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			return;
		}

		auto var = index.data(POINTERDATA);
		auto pDevice = reinterpret_cast<Config::CDevice *>(var.toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			auto strError = QString("device tag %1 can not find!!!").arg(strTagName);
			m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);
			return;
		}

		//auto strFesTagname = CFesModule::GetTagname(index, FES_ITEM);
		//auto *pFes = CFesModule::GetFes(m_arrFes, strFesTagname);
		//Q_ASSERT(pFes);
		//if (!pFes)
		//{
		//	auto strError = QString("fes tag %1 can not find!!!").arg(strFesTagname);
		//	m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

		//	return;
		//}

		auto pItem = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);
		auto pView = new CDOView(nullptr, m_pCore, pDevice, strTagName, pItem, this, pFes);

		TabKeyStruct keyInfo;
		keyInfo.BigTagName = strFesTagName;
		keyInfo.TagName = strTagName;
		keyInfo.Type = WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE;
		auto &icon = pItem->icon();

		m_pUi->GetTabWidget()->AddTab(pView, strTagName, keyInfo, icon);
		m_pUi->GetTabWidget()->setCurrentWidget(pView);
	}
}

#include <qmessagebox.h>
/*! \fn void CChannel::itemChanged(QStandardItem *item)
********************************************************************************************************* 
** \brief CChannel::itemChanged 
** \details 左边树数据修改
** \param item 
** \return void 
** \author gw
** \date 2015年12月16日 
** \note 
********************************************************************************************************/
void CChannel::itemChanged(QStandardItem *item)
{
	QString strTmp = item->data(Qt::EditRole).toString();

	//int nTmp = item->data(Qt::UserRole).toInt();
	
	if (item->data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
	{	
		QString strChannelTagName = item->data(Qt::EditRole).toString();
		auto strChannelLastTagValue = item->data(LASTVALUE).toString();
		if (strChannelTagName == strChannelLastTagValue)
		{
			return;
		}

		//检查修改的通道tagname是否存在
		auto pItemParent = item->parent();
		while (pItemParent->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}

		auto strFesTagName = pItemParent->data(Qt::EditRole).toString();
		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return;
		}


		if (pFes->CheckTagNameIsExist(strChannelTagName))
		{
			//修改的通道tagname存在
			auto strError = tr("channel tag name %1 is exist!!!").arg(strChannelTagName);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strChannelLastTagValue, Qt::EditRole);

			return;
		}

		if (!CheckTagNameIsValid(strChannelTagName, FES_DESC))
		{
			return;
		}

		//通道的parent 组 channel node的组是前置的m_pChannelGroup
		auto pGroup = reinterpret_cast<Config::CChannelGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			auto strError = tr("parent group is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}
		

		auto pChannel = reinterpret_cast<Config::CChannelData *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannel);
		if (!pChannel)
		{
			return;
		}

		//前置内的通道hash 修改key
		if (!pFes->ModifyFesHashMap(CHANNELHASHID, strChannelTagName, strChannelLastTagValue))
		{
			auto strError = tr("channel item modify fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strChannelLastTagValue, Qt::EditRole);

			return;
		}

		//修改组内的通道
		if (!pGroup->ModifyItem(strChannelTagName, strChannelLastTagValue))
		{
			auto strError = tr("channel item %1 modify fail!!!").arg(strChannelLastTagValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strChannelLastTagValue, Qt::EditRole);

			return;
		}

		//memset(pChannel->m_szTagName, 0, sizeof(pChannel->m_szTagName));
		//auto nSize = strChannelTagName.size();
		//strncpy(pChannel->m_szTagName, strChannelTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));
		
		item->setData(strChannelTagName, LASTVALUE);


		//判断是否存在该widget
		auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strChannelLastTagValue, WIDGET_TYPE::CHANNEL_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			TabKeyStruct newKeyInfo;
			TabKeyStruct lastKeyInfo;
			newKeyInfo.BigTagName = strFesTagName;
			newKeyInfo.TagName = strChannelTagName;
			newKeyInfo.Type = WIDGET_TYPE::CHANNEL_WIDGET_TYPE;
			lastKeyInfo.BigTagName = strFesTagName;
			lastKeyInfo.TagName = strChannelLastTagValue;
			lastKeyInfo.Type = WIDGET_TYPE::CHANNEL_WIDGET_TYPE;

			m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}
	}
	else if (item->data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{	
		auto strLastValue = item->data(LASTVALUE).toString();
		auto strValue = item->data(Qt::EditRole).toString();
		if (strLastValue == strValue)
		{
			return;
		}

		//parent group
		auto pChannelGroupParent = reinterpret_cast<Config::CChannelGroup *>(item->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroupParent);
		if (!pChannelGroupParent)
		{
			auto strError = tr("group parent pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//当前修改的组
		auto pChannelGroup = reinterpret_cast<Config::CChannelGroup *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pChannelGroup);
		if (!pChannelGroup)
		{
			auto strError = tr("group pointer is nullptr!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//检查组名是否存在
		if (pChannelGroupParent->CheckModifyGroupNameExist(pChannelGroup, strValue))
		{
			auto strError = tr("group name %1 is exist!!!").arg(strValue);
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastValue, Qt::EditRole);

			return;
		}

		if (!pChannelGroupParent->ModifyGroup(strValue, strLastValue))
		{
			auto strError = tr("group name is modify fail!!!");
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastValue, Qt::EditRole);

			return;
		}

		pChannelGroup->m_strGroup = strValue;


		//修改分组,view的数据跟着修改
		QStandardItemModel *pModel = m_pCore->GetUIMgr()->GetLeftTreeModel();

		QVector<QModelIndex> vec;
		vec.clear();

		//前置组组修改,遍历查找前置项
		CFesModule::GetItemMap(pModel, pModel->indexFromItem(item), FES_CHANNEL_CHILD_ITEM, vec);

		//通道遍历自己的通道组
		for (auto &tmp : vec)
		{
			QString strGroupName = CFesModule::GetGroupName(tmp,FES_CHANNEL_ITEM);


			auto strChannelTagName = tmp.data(Qt::EditRole).toString();
			auto pChannel = reinterpret_cast<Config::CChannelData *>(tmp.data(POINTERDATA).toLongLong());
			Q_ASSERT(pChannel);
			if (!pChannel)
			{
				return;
			}

			pChannel->m_strGroupName = strGroupName;
			item->setData(strValue, LASTVALUE);


			//判断是否存在该widget
			auto bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strChannelTagName, WIDGET_TYPE::CHANNEL_WIDGET_TYPE);
			if (bFlag)
			{
				//m_pUi->GetTabWidget()->SetTabName(strTagName, FES_WIDGET_TYPE, strGroupName);

				IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
				Q_ASSERT(pBase);
				if (!pBase)
				{
					return;
				}

				pBase->Refresh();
			}
		}

	}
	else if (item->data(Qt::UserRole).toInt() == FES__CHANNEL_CHILD_DEVICE_ITEM)
	{	
		auto strDeviceTagName = item->data(Qt::EditRole).toString();
		auto strLastDeviceTagName = item->data(LASTVALUE).toString();
		if (strDeviceTagName == strLastDeviceTagName)
		{
			return;
		}
		
		if (!CheckTagNameIsValid(strDeviceTagName, FES_DESC))
		{
			return;
		}


		auto pItemParent = item;
		while (pItemParent->parent()->data(Qt::UserRole).toInt() != FES_ITEM)
		{
			pItemParent = pItemParent->parent();
		}


		auto pFes = reinterpret_cast<Config::CFesData *>(pItemParent->parent()->data(POINTERDATA).toLongLong());
		Q_ASSERT(pFes);
		if (!pFes)
		{
			item->setData(strLastDeviceTagName, Qt::EditRole);
			
			return;
		}
		auto strFesTagName = pItemParent->parent()->data(Qt::EditRole).toString();
		auto bFlag = strFesTagName.isEmpty() || strFesTagName.size() >= MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return;
		}

		if (pFes->CheckTagNameIsExist(strDeviceTagName))
		{
			auto strError = tr("device tagname %1 is exist!!!").arg(strDeviceTagName);
			
			m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			item->setData(strLastDeviceTagName, Qt::EditRole);

			return;
		}

		//auto strFesTagName = CFesModule::GetTagname(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item), FES_ITEM);
		//auto pFes = CFesModule::GetFes(m_arrFes, strFesTagName);
		//Q_ASSERT(pFes);
		//if (!pFes)
		//{
		//	m_pCore->LogMsg(CHANNEL_DESC, LOG_STRING("can not find fes!!!"), LEVEL_1);
		//	
		//	return;
		//}

		//auto &strChannelTagName = CFesModule::GetTagname(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem(item), FES_CHANNEL_CHILD_ITEM);
		//CFesModule::GetChannel(pFes, strChannelTagName);
		//auto pChannel = CFesModule::GetChannel(pFes, strChannelTagName);
		//Q_ASSERT(pChannel);
		//if (!pChannel)
		//{
		//	return;
		//}

		auto pDevice = reinterpret_cast<Config::CDevice *>(item->data(POINTERDATA).toLongLong());
		Q_ASSERT(pDevice);
		if (!pDevice)
		{
			m_pCore->LogMsg(DEVICE_DESC, LOG_STRING("can not find device!!!"), LEVEL_1);

			return;
		}

		memset(pDevice->m_szTagName, 0, sizeof(pDevice->m_szTagName));
		auto nSize = strDeviceTagName.size();
		strncpy(pDevice->m_szTagName, strDeviceTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

		//pFes->SetModifyFlag(true);

		item->setData(strDeviceTagName, LASTVALUE);

		if (!pFes->ModifyFesHashMap(DEVICEHASHID, strDeviceTagName, strLastDeviceTagName))
		{
			return;
		}

		//判断是否存在该widget
		bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strLastDeviceTagName, WIDGET_TYPE::DEVICE_WIDGET_TYPE, strFesTagName);
		if (bFlag)
		{
			TabKeyStruct newKeyInfo;
			TabKeyStruct lastKeyInfo;
			newKeyInfo.BigTagName = strFesTagName;
			newKeyInfo.TagName = strDeviceTagName;
			newKeyInfo.Type = WIDGET_TYPE::DEVICE_WIDGET_TYPE;
			lastKeyInfo.BigTagName = strFesTagName;
			lastKeyInfo.TagName = strLastDeviceTagName;
			lastKeyInfo.Type = WIDGET_TYPE::DEVICE_WIDGET_TYPE;

			m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);
		
			IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
			Q_ASSERT(pBase);
			if (!pBase)
			{
				return;
			}

			pBase->Refresh();
		}

		auto pModel = m_pUi->GetLeftTreeModel();
		QModelIndex index = m_pUi->GetLeftTreeModel()->indexFromItem(item);
		auto nRow = pModel->rowCount(index);
		for (int i = 0; i < nRow; i++)
		{
			auto childIndex = pModel->index(i, 0, index);
			int nType = childIndex.data(Qt::UserRole).toInt();
			if (nType == FES_DEVICE_AI)
			{
				/////////////////////////////ai//////////////////////////////////////////
				auto pAiItem = pModel->itemFromIndex(childIndex);
				QString strAITag = QString("%1_ai").arg(strDeviceTagName);

				auto strAILastTagName = pAiItem->data(LASTVALUE).toString();
				bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strAILastTagName, WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strAITag;
					newKeyInfo.Type = WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strAILastTagName;
					lastKeyInfo.Type = WIDGET_TYPE::DEVICE_AI_WIDGET_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return;
					}

					pBase->Refresh();
				}

				pAiItem->setData(strAITag, Qt::EditRole);
				pAiItem->setData(strAITag, LASTVALUE);
			}
			else if (nType == FES_DEVICE_DI)
			{
				//////////////////////////////di/////////////////////////////////////////////
				auto pDiItem = pModel->itemFromIndex(childIndex);
				QString strDITag = QString("%1_di").arg(strDeviceTagName);

				auto strDILastTagName = pDiItem->data(LASTVALUE).toString();
				bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strDILastTagName, WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strDITag;
					newKeyInfo.Type = WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strDILastTagName;
					lastKeyInfo.Type = WIDGET_TYPE::DEVICE_DI_WIDGET_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return;
					}

					pBase->Refresh();
				}

				pDiItem->setData(strDITag, Qt::EditRole);
				pDiItem->setData(strDITag, LASTVALUE);
			}
			else if (nType == FES_DEVICE_AO)
			{
				//////////////////////////////ao///////////////////////////////////////////////////////
				auto pAOItem = pModel->itemFromIndex(childIndex);
				QString strAOTag = QString("%1_ao").arg(strDeviceTagName);

				auto strAOLastTagName = pAOItem->data(LASTVALUE).toString();
				bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strAOLastTagName, WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newKeyInfo;
					TabKeyStruct lastKeyInfo;
					newKeyInfo.BigTagName = strFesTagName;
					newKeyInfo.TagName = strAOTag;
					newKeyInfo.Type = WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strAOLastTagName;
					lastKeyInfo.Type = WIDGET_TYPE::DEVICE_AO_WIDGET_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newKeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return;
					}

					pBase->Refresh();
				}

				pAOItem->setData(strAOTag, Qt::EditRole);
				pAOItem->setData(strAOTag, LASTVALUE);
			}
			else if (nType == FES_DEVICE_DO)
			{
				//////////////////////////////do///////////////////////////////////////////////////////
				auto pDOItem = pModel->itemFromIndex(childIndex);
				QString strDOTag = QString("%1_do").arg(strDeviceTagName);

				auto strDOLastTagName = pDOItem->data(LASTVALUE).toString();
				bFlag = m_pUi->GetTabWidget()->IsWidgetInTab(strDOLastTagName, WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE, strFesTagName);
				if (bFlag)
				{
					TabKeyStruct newkeyInfo;
					TabKeyStruct lastKeyInfo;
					newkeyInfo.BigTagName = strFesTagName;
					newkeyInfo.TagName = strDOTag;
					newkeyInfo.Type = WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE;
					lastKeyInfo.BigTagName = strFesTagName;
					lastKeyInfo.TagName = strDOLastTagName;
					lastKeyInfo.Type = WIDGET_TYPE::DEVICE_DO_WIDGET_TYPE;

					m_pUi->GetTabWidget()->SetTabName(newkeyInfo, lastKeyInfo);

					IBase *pBase = dynamic_cast<IBase *>(m_pUi->GetTabWidget()->currentWidget());
					Q_ASSERT(pBase);
					if (!pBase)
					{
						return;
					}

					pBase->Refresh();
				}

				pDOItem->setData(strDOTag, Qt::EditRole);
				pDOItem->setData(strDOTag, LASTVALUE);
			}
		}
	}
}

/*! \fn void CChannel::InitPara()
********************************************************************************************************* 
** \brief CChannel::InitPara 
** \details 
** \return void 
** \author gw
** \date 2016年1月15日 
** \note 
********************************************************************************************************/
void CChannel::InitPara()
{
	//规约名
	//m_arrDriveName.push_back("101");
	//m_arrDriveName.push_back("104");
	//m_arrDriveName.push_back("Modubs");
}

QString CChannel::GetGroupName(QModelIndex &index)
{
	QString strGroup = "";

	if (index.data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
	{
		strGroup = index.data().toString();


		QString strTmp = GetGroupName(index.parent());

		if (strTmp.isEmpty())
		{
			return strGroup;
		}
		else
		{
			strGroup = strTmp + "." + strGroup;
		}
	}
	else
	{
		return strGroup = "";
	}

	return strGroup;
}

/*! \fn void CChannel::AddChannelTagName(QString &strTagName, QString &strDesc)
********************************************************************************************************* 
** \brief CChannel::AddChannelTagName 
** \details 添加通道代码
** \param strTagName 
** \param strDesc 
** \return void 
** \author gw
** \date 2016年2月1日 
** \note 
********************************************************************************************************/
void CChannel::AddChannelTagName(QString &strTagName, QString &strDesc)
{
	int32u nCount = 0;

	strDesc += QString("%1").arg(nCount + 1);
	strTagName += QString("%1").arg(nCount + 1);

	//int nTmp;
	QRegExp rx("(\\d+)");  // 匹配数字
	//int pos = 0;

	int nIndex = strDesc.lastIndexOf(rx);

	strDesc = strDesc.mid(0, nIndex);
}                      

//void CChannel::SendSelectedRows(QSet<int32s> &set)
//{
//
//}

/*! \fn void CChannel::CreateChannelAttr()
********************************************************************************************************* 
** \brief CChannel::CreateChannelAttr 
** \details 创建通道属性栏
** \return void 
** \author gw
** \date 2016年1月25日 
** \note 
********************************************************************************************************/
void CChannel::CreateChannelAttr(CChannelModel *pModel, QSet<int32s> &set)
{
	Q_UNUSED(set);
	
	Q_ASSERT(pModel);

	m_pCurrentTabItem = pModel->GetCurrentTreeItem();

	m_pModel = pModel;

	QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToId.constBegin();
	//while (itProp != m_propertyToId.constEnd()) {
	//	delete itProp.key();
	//	itProp++;
	//}
	//m_propertyToId.clear();
	//m_idToProperty.clear();

	CreateAttriConnect();

	//QtProperty *property = m_pUi->GetAtrri()->AddGroupProperty(QStringLiteral("基本"));

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	QString strTmp = QStringLiteral("基本");

	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToId[property] = strTmp;
	m_idToProperty[strTmp] = property;


	QMap<int32s, CONFIG_PARA> tmp = pModel->GetColumnInfo();

	//Tagname
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(tmp[CChannelModel::COLUMN::TagName].desc);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pModel->GetChannelData()->m_szTagName);
	QString pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::TagName].desc;
	m_idToProperty[tmp[CChannelModel::COLUMN::TagName].desc] = pProperty;


	//Desc
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(tmp[CChannelModel::COLUMN::ChannelName].desc);
	m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, pModel->GetChannelData()->m_strDescription);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::ChannelName].desc;
	m_idToProperty[tmp[CChannelModel::COLUMN::ChannelName].desc] = pProperty;

	//ID
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(tmp[CChannelModel::COLUMN::ID].desc);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);

	m_pUi->GetAtrri()->GetIntProperty()->setValue(pProperty, pModel->GetChannelData()->m_nID);
	property->addSubProperty(pProperty);

	//DriverName
	m_pDriverNameInfo = &pModel->GetDriverName();

	pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(tmp[CChannelModel::COLUMN::DriverName].desc);
	//m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, )

	QStringList list;
	int32s value = 0;
	for (int32s i = 0; i < m_pDriverNameInfo->size(); i++) 
	{
		if ((*m_pDriverNameInfo)[i].desc.compare(pModel->GetChannelData()->m_CommPara.DriverName, Qt::CaseInsensitive) == 0)
		{
			value = i;
		}
		
		list.append((*m_pDriverNameInfo)[i].desc);
	}

	m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, list);
	m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, value);

	property->addSubProperty(pProperty);
	m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::DriverName].desc;
	m_idToProperty[tmp[CChannelModel::COLUMN::DriverName].desc] = pProperty;

	////DriverParam
	//strTmp = tmp[CChannelModel::COLUMN::DriverPara].desc;
	//
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(tmp[CChannelModel::COLUMN::DriverPara].desc);
	//
	//auto driver = pModel->GetChannelData()->m_CommPara.DriverName;
	//if (pModel->GetChannelData()->m_CommPara.DriverName.compare(DRIVE_104, Qt::CaseInsensitive) == 0)
	//{
	//	m_pDriverParaInfo = &pModel->GetDriverPara104();
	//}
	//else if (pModel->GetChannelData()->m_CommPara.DriverName.compare(DRIVE_101, Qt::CaseInsensitive) == 0)
	//{
	//	m_pDriverParaInfo = &pModel->GetDriverPara101();
	//}
	//else if (pModel->GetChannelData()->m_CommPara.DriverName.compare(DRIVE_MODBUS, Qt::CaseInsensitive) == 0)
	//{
	//	m_pDriverParaInfo = &pModel->GetDriverParaModbus();
	//}

	//Q_ASSERT(m_pDriverParaInfo);
	//if (!m_pDriverParaInfo)
	//{
	//	m_pCore->LogMsg(FES_DESC, LOG_STRING("驱动名没有找到!!!"), 1);

	//	return;
	//}

	//list.clear();
	//value = 0;

	//for (int32s i = 0; i < m_pDriverParaInfo->size(); i++)
	//{
	//	if ((*m_pDriverParaInfo)[i].ID.toInt() == pModel->GetChannelData()->m_CommPara.DriverParam)
	//	{
	//		value = i;
	//	}

	//	list.append((*m_pDriverParaInfo)[i].desc);
	//}

	//m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, list);
	//m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, value);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::DriverPara].desc;
	//m_idToProperty[tmp[CChannelModel::COLUMN::DriverPara].desc] = pProperty;

	////Medium
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(tmp[CChannelModel::COLUMN::Medium].desc);
	//m_pMediumInfo = &pModel->GetMedium();
	//list.clear();
	//value = 0;
	//for (int32s i = 0; i < m_pMediumInfo->size(); i++)
	//{
	//	if ((*m_pMediumInfo)[i].ID.toInt() == pModel->GetChannelData()->m_CommPara.Medium)
	//	{
	//		value = i;
	//	}

	//	list.append((*m_pMediumInfo)[i].desc);
	//}

	//m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, list);
	//m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, value);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::Medium].desc;
	//m_idToProperty[tmp[CChannelModel::COLUMN::Medium].desc] = pProperty;

	////DeviceAddress
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(tmp[CChannelModel::COLUMN::DeviceAddress].desc);
	//pattern = QString(MYIPPORTREG);
	//regExp.setPattern(pattern);
	//m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	//strTmp = QString("%1").arg(pModel->GetChannelData()->m_CommPara.Port);
	//strTmp = pModel->GetChannelData()->m_CommPara.IP + ":" + strTmp;
	//m_pUi->GetAtrri()->GetStringProperty()->setValue(pProperty, strTmp);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::DeviceAddress].desc;
	//m_idToProperty[tmp[CChannelModel::COLUMN::DeviceAddress].desc] = pProperty;

	////COM
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(tmp[CChannelModel::COLUMN::COM].desc);
	//m_pCOMInfo = &pModel->GetCOM();

	//list.clear();
	//value = 0;
	//for (int32s i = 0; i < m_pCOMInfo->size(); i++)
	//{
	//	if ((*m_pCOMInfo)[i].desc.compare(pModel->GetChannelData()->m_CommPara.Com, Qt::CaseInsensitive) == 0)
	//	{
	//		value = i;
	//	}

	//	list.append((*m_pCOMInfo)[i].desc);
	//}

	//m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, list);
	//m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, value);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::COM].desc;
	//m_idToProperty[tmp[CChannelModel::COLUMN::COM].desc] = pProperty;

	////Baud
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(tmp[CChannelModel::COLUMN::Baud].desc);
	//m_pBaudInfo = &pModel->GetBaud();

	//list.clear();
	//value = 0;
	//for (int32s i = 0; i < m_pBaudInfo->size(); i++)
	//{
	//	if ((*m_pBaudInfo)[i].ID.toInt() == pModel->GetChannelData()->m_CommPara.Baud)
	//	{
	//		value = i;
	//	}

	//	list.append((*m_pBaudInfo)[i].desc);
	//}

	//m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(pProperty, list);
	//m_pUi->GetAtrri()->GetEnumProperty()->setValue(pProperty, value);

	//property->addSubProperty(pProperty);
	//m_propertyToId[pProperty] = tmp[CChannelModel::COLUMN::Baud].desc;
	//m_idToProperty[tmp[CChannelModel::COLUMN::Baud].desc] = pProperty;

	CChannelModel::COLUMN::TagName;

	pTree->addProperty(property);
}

#include <qthread.h>
void CChannel::SendSelectedRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel)
{	
	Q_UNUSED(set);
	Q_UNUSED(nType);
	//Q_UNUSED(pModel);
	
	//QString strTest = "test";

	//m_pCore->LogMsg("fes", "test", 1);

	//m_pCore->LogMsg((char *)QStringLiteral("f前置").toStdString().c_str(), (char *)QStringLiteral("测试").toStdString().c_str(), 1);


	//memcpy(send, &log, sizeof(MSG_LOG));
	//
	//int nCount = sizeof(MSG_LOG);


	//m_pClient->sendData(send, nCount);
	
	

	Q_ASSERT(pModel);

	//QThread::msleep(5000);

	//创建属性栏
	CreateChannelAttr(pModel, set);
}

void CChannel::SendChangeChannelModelRows(QSet<int32s> &set, int32s nType, CChannelModel *pModel)
{
	Q_UNUSED(nType);
	
	Q_ASSERT(pModel);



	//创建属性栏
	CreateChannelAttr(pModel, set);
}


void CChannel::CreateAttriConnect()
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

	connect(m_pAttr->GetIntProperty(), SIGNAL(valueChanged(QtProperty *, int)),
		this, SLOT(valueChanged(QtProperty *, int)));
}

/*! \fn void CChannel::valueChanged(QtProperty *property, const QString &val)
********************************************************************************************************* 
** \brief CChannel::valueChanged 
** \details 值改变
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2016年1月29日 
** \note 
********************************************************************************************************/
void CChannel::valueChanged(QtProperty *property, const QString &val)
{
	if (m_propertyToIdAI.contains(property))
	{
		
		Q_ASSERT(m_pAIModel);

		if (m_pAIModel)
		{
			ValueChangedAI(property, val);
		}
	}
	else if (m_propertyToId.contains(property) && m_pModel)
	{	
		//Tagname
		QString strTmp = m_propertyToId[property];

		QMap<int32s, CONFIG_PARA> tmp = m_pModel->GetColumnInfo();

		if (strTmp.compare(tmp[CChannelModel::COLUMN::TagName].desc, Qt::CaseInsensitive) == 0)
		{
			//Tagname
			//m_pModel->GetChannelData()->m_szTagName = val;
			//sprintf_s(m_pModel->GetChannelData()->m_szTagName, TAGNAME_SIZE + 1, "%s", val.toStdString().c_str());
			memset(m_pModel->GetChannelData()->m_szTagName, 0, sizeof(m_pModel->GetChannelData()->m_szTagName));
			int nSize = val.size();
			strncpy(m_pModel->GetChannelData()->m_szTagName, val.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, nSize));

			m_pCurrentTabItem->setData(val, Qt::EditRole);

			m_pModel->RefrushModel();
		}
		else if (strTmp.compare(tmp[CChannelModel::COLUMN::ChannelName].desc, Qt::CaseInsensitive) == 0)
		{
			//ChannelName
			m_pModel->GetChannelData()->m_strDescription = val;

			//m_pModel->setData(m_pModel->createDataIndex(0, CChannelModel::COLUMN::ChannelName), val);

			m_pCurrentTabItem->setData(val, Qt::EditRole);

			m_pModel->RefrushModel();
		}
		//else if (strTmp.compare(tmp[CChannelModel::COLUMN::DeviceAddress].desc, Qt::CaseInsensitive) == 0)
		//{
		//	//DeviceAddress
		//	QStringList list = val.split(":");

		//	if (list.count() == 2)
		//	{
		//		m_pModel->GetChannelData()->m_CommPara.IP = list[0];
		//		m_pModel->GetChannelData()->m_CommPara.Port = list[1].toInt();
		//	}

		//	m_pModel->RefrushModel();
		//}
	}
	else if (m_propertyToIdAI.contains(property))
	{
		//Tagname
		QString strTmp = m_propertyToIdAI[property];

		QMap<int32s, CONFIG_PARA> tmp = m_pAIModel->GetColumnInfo();

		if (strTmp.compare(tmp[CChannelModel::COLUMN::TagName].desc, Qt::CaseInsensitive) == 0)
		{
			////Tagname
			//m_pModel->GetChannelData()->m_szTagName = val;

			//m_pCurrentTabItem->setData(val, TAGNAME);

			//m_pModel->RefrushModel();
		}
	}
	else if (m_propertyToIdDI.contains(property))
	{
		Q_ASSERT(m_pDIModel);

		if (m_pDIModel)
		{
			ValueChangedDI(property, val);
		}
	}
	else if (m_propertyToIdAO.contains(property))
	{
		Q_ASSERT(m_pAOModel);

		if (m_pAOModel)
		{
			ValueChangedAO(property, val);
		}
	}
	else if (m_propertyToIdDO.contains(property))
	{
		Q_ASSERT(m_pDOModel);

		if (m_pDOModel)
		{
			ValueChangedDO(property, val);
		}
	}
}

/*! \fn void CChannel::valueChanged(QtProperty *property, int val)
********************************************************************************************************* 
** \brief CChannel::valueChanged 
** \details 值改变
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2016年1月29日 
** \note 
********************************************************************************************************/
void CChannel::valueChanged(QtProperty *property, int val)
{
	if (m_propertyToId.contains(property))
	{
		static int i = 0;

		i = i + 1;

		QString strTmp = m_propertyToId[property];

		QMap<int32s, CONFIG_PARA> tmp = m_pModel->GetColumnInfo();

		QString strDesc = property->valueText();

		if (strTmp.compare(tmp[CChannelModel::COLUMN::DriverName].desc, Qt::CaseInsensitive) == 0)
		{
			//
			if (strDesc.compare(DRIVE_104, Qt::CaseInsensitive) == 0)
			{
				strTmp = tmp[CChannelModel::COLUMN::DriverPara].desc;

				m_pDriverParaInfo = &m_pModel->GetDriverPara104();
			}
			else if (strDesc.compare(DRIVE_101, Qt::CaseInsensitive) == 0)
			{
				strTmp = tmp[CChannelModel::COLUMN::DriverPara].desc;

				m_pDriverParaInfo = &m_pModel->GetDriverPara101();
			}
			else if (strDesc.compare(DRIVE_MODBUS, Qt::CaseInsensitive) == 0)
			{
				strTmp = tmp[CChannelModel::COLUMN::DriverPara].desc;

				m_pDriverParaInfo = &m_pModel->GetDriverParaModbus();
			}


			QStringList list;
			int value = 0;

			for (int32s i = 0; i < m_pDriverParaInfo->size(); i++)
			{
				if ((*m_pDriverParaInfo)[i].ID == m_pModel->GetChannelData()->m_CommPara.m_strDriverParam)
				{
					value = i;
				}

				list.append((*m_pDriverParaInfo)[i].desc);
			}

			property = m_idToProperty[tmp[CChannelModel::COLUMN::DriverPara].desc];


			m_pUi->GetAtrri()->GetEnumProperty()->setEnumNames(property, list);
			m_pUi->GetAtrri()->GetEnumProperty()->setValue(property, value);

			m_pModel->GetChannelData()->m_CommPara.DriverName = strDesc;

			m_pModel->RefrushModel();
		}
		else if (strTmp.compare(tmp[CChannelModel::COLUMN::DriverPara].desc, Qt::CaseInsensitive) == 0)
		{
			for (int32s i = 0; i < m_pDriverParaInfo->size(); i++)
			{
				if ((*m_pDriverParaInfo)[i].desc.compare(strDesc) == 0)
				{
					m_pModel->GetChannelData()->m_CommPara.m_strDriverParam = (*m_pDriverParaInfo)[i].ID;

					m_pModel->RefrushModel();

					break;
				}
			}
		}
		//else if (strTmp.compare(tmp[CChannelModel::COLUMN::Medium].desc, Qt::CaseInsensitive) == 0)
		//{
		//	for (int32s i = 0; i < m_pMediumInfo->size(); i++)
		//	{
		//		if ((*m_pMediumInfo)[i].desc.compare(strDesc) == 0)
		//		{
		//			m_pModel->GetChannelData()->m_CommPara.Medium = (*m_pMediumInfo)[i].ID.toInt();

		//			m_pModel->RefrushModel();

		//			break;
		//		}
		//	}
		//}
		/*else if (strTmp.compare(tmp[CChannelModel::COLUMN::COM].desc, Qt::CaseInsensitive) == 0)
		{
			for (int32s i = 0; i < m_pCOMInfo->size(); i++)
			{
				if ((*m_pCOMInfo)[i].desc.compare(strDesc) == 0)
				{
					m_pModel->GetChannelData()->m_CommPara.Com = (*m_pCOMInfo)[i].ID;

					m_pModel->RefrushModel();

					break;
				}
			}
		}*/
		/*else if (strTmp.compare(tmp[CChannelModel::COLUMN::Baud].desc, Qt::CaseInsensitive) == 0)
		{
			for (int32s i = 0; i < m_pBaudInfo->size(); i++)
			{
				if ((*m_pBaudInfo)[i].desc.compare(strDesc) == 0)
				{
					m_pModel->GetChannelData()->m_CommPara.Baud = (*m_pBaudInfo)[i].ID.toInt();

					m_pModel->RefrushModel();

					break;
				}
			}
		}*/
	}
	else if (m_propertyToIdAI.contains(property))
	{
		Q_ASSERT(m_pAIModel);

		if (m_pAIModel)
		{
			ValueChangedAI(property, val);
		}
	}
	else if (m_propertyToIdDI.contains(property))
	{
		Q_ASSERT(m_pDIModel);

		if (m_pDIModel)
		{
			ValueChangedDI(property, val);
		}
	}
	else if (m_propertyToIdAO.contains(property))
	{
		Q_ASSERT(m_pAOModel);

		if (m_pAOModel)
		{
			ValueChangedAO(property, val);
		}
	}
	else if (m_propertyToIdDO.contains(property))
	{
		Q_ASSERT(m_pDOModel);

		if (m_pDOModel)
		{
			ValueChangedDO(property, val);
		}
	}
}

void CChannel::valueChanged(QtProperty *property, double val)
{
	if (m_propertyToIdAI.contains(property))
	{
		Q_ASSERT(m_pAIModel);

		if (m_pAIModel)
		{
			ValueChangedAI(property, val);
		}
	}
	else if(m_propertyToId.contains(property))
	{
		
	}
	else if (m_propertyToIdDI.contains(property))
	{
		Q_ASSERT(m_pDIModel);

		if (m_pDIModel)
		{
			ValueChangedDI(property, val);
		}
	}
	else if (m_propertyToIdAO.contains(property))
	{
		Q_ASSERT(m_pAOModel);

		if (m_pAOModel)
		{
			ValueChangedAO(property, val);
		}
	}
	else if (m_propertyToIdDO.contains(property))
	{
		Q_ASSERT(m_pDOModel);

		if (m_pDOModel)
		{
			ValueChangedDO(property, val);
		}
	}
}

void CChannel::valueChanged(QtProperty *property, bool val)
{
	if (m_propertyToIdAI.contains(property))
	{
		Q_ASSERT(m_pAIModel);

		if (m_pAIModel)
		{
			ValueChangedAI(property, val);
		}
	}
	else if (m_propertyToId.contains(property))
	{

	}
	if (m_propertyToIdDI.contains(property))
	{
		Q_ASSERT(m_pDIModel);

		if (m_pDIModel)
		{
			ValueChangedDI(property, val);
		}
	}
	else if (m_propertyToIdAO.contains(property))
	{
		Q_ASSERT(m_pAOModel);

		if (m_pAOModel)
		{
			ValueChangedAO(property, val);
		}
	}
	else if (m_propertyToIdDO.contains(property))
	{
		Q_ASSERT(m_pDOModel);

		if (m_pDOModel)
		{
			ValueChangedDO(property, val);
		}
	}
}

/*! \fn void CChannel::collapsed(const QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::collapsed 
** \details 展开组
** \param index 
** \return void 
** \author gw
** \date 2016年7月29日 
** \note 
********************************************************************************************************/
void CChannel::collapsed(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_CHANNEL_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(CLOSE_GROUP_PNG));
	}
}

void CChannel::expanded(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();

	if (nType == FES_CHANNEL_GROUP_ITEM)
	{
		CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

		item->setIcon(QIcon(OPEN_GROUP_PNG));
	}
}

#include "channeltable.h"
/*! \fn bool CChannel::CheckTagNameAndChannelName(QString &strTagName, QString &strDesc)
********************************************************************************************************* 
** \brief CChannel::CheckTagNameAndChannelName 
** \details 检查代码和通道名是否重复
** \param strTagName 
** \param strDesc 
** \return bool 
** \author gw
** \date 2016年2月1日 
** \note 
********************************************************************************************************/
bool CChannel::CheckTagNameAndChannelName(QString &strTagName, QString &strDesc)
{
	Q_UNUSED(strTagName);
	Q_UNUSED(strDesc);
	
	bool bFlag = false;
	
	return bFlag;
}

QStandardItem * CChannel::FindChannelItem(QStandardItem *item, QStandardItem *channelItem)
{
	Q_ASSERT(item);

	QStandardItem *pTmp = item->child(0);

	QString strTmp = pTmp->data(Qt::EditRole).toString();

	if (item->hasChildren())
	{
		//QList<QStandardItem *> list = item->takeColumn(0);
		
		if (item->data(Qt::UserRole).toInt() == FES_CHANNEL_GROUP_ITEM)
		{
			return FindChannelItem(item->child(0), channelItem);
		}
		else if (item->data(Qt::UserRole).toInt() == FES_CHANNEL_CHILD_ITEM)
		{
			channelItem = item;

			return channelItem;
		}
	}
	
	return NULL;
}

/*! \fn void CChannel::SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModule)
********************************************************************************************************* 
** \brief CChannel::SendSelectedRows 
** \details 属性栏
** \param set 
** \param nType 类型
** \param pModule 
** \return void 
** \author gw
** \date 2016年5月17日 
** \note 
********************************************************************************************************/
#include "aimodel.h"
#include "dimodel.h"
#include <QTime>
#include "qtpropertybrowser/QtVariantPropertyManager"
void CChannel::SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModule)
{	
	m_setSelect = set;

	if (nType == DEVICE_AI)
	{
		CAIModel *pModuleAI = (CAIModel *)pModule;
		
		//CreateAIAttr((CAIModel *)pModule);

		QtVariantPropertyManager *variantManager = new QtVariantPropertyManager(this);
		QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
		QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();
		pTree->setFactoryForManager(variantManager, variantFactory);

		m_propertyToIdAI.clear();
		m_idToPropertyAI.clear();

		CreateAttriConnect();

		QMap<int32s, CONFIG_PARA> tmp = pModuleAI->GetColumnInfo();

		//QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

		pTree->clear();

		QTime time;
		time.start();

		QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

		QString strTmp = tr("base");

		QtProperty *property = pGroup->addProperty(strTmp);
		m_propertyToIdAI[property] = strTmp;
		m_idToPropertyAI[strTmp] = property;
		pTree->addProperty(property);

		//Tagname
		strTmp = tmp[CAIModel::COLUMN::TagName].desc;
		QtVariantProperty *pProperty = variantManager->addProperty(QVariant::String, strTmp);
		QString pattern = QString(TAGNAMEREG).arg(MAX_NAME_LENGTH_SCADASTUDIO);
		QRegExp regExp;
		regExp.setPattern(pattern);
		variantManager->setAttribute(pProperty, QLatin1String("regExp"), regExp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//Desc
		strTmp = tmp[CAIModel::COLUMN::Desc].desc;
		pProperty = variantManager->addProperty(QVariant::String, strTmp);
		pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
		regExp.setPattern(pattern);
		variantManager->setAttribute(pProperty, QLatin1String("regExp"), regExp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//ID
		strTmp = tmp[CAIModel::COLUMN::ID].desc;
		pProperty = variantManager->addProperty(QVariant::Int, strTmp);
		pProperty->setAttribute(QLatin1String("minimum"), UINT_LOW);
		pProperty->setAttribute(QLatin1String("maximum"), UINT_HIGH);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//OccNo
		strTmp = tmp[CAIModel::COLUMN::OccNo].desc;
		pProperty = variantManager->addProperty(QVariant::Int, strTmp);
		pProperty->setAttribute(QLatin1String("minimum"), UINT_LOW);
		pProperty->setAttribute(QLatin1String("maximum"), UINT_HIGH);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//BlockOccNo
		strTmp = tmp[CAIModel::COLUMN::BlockOccNo].desc;
		pProperty = variantManager->addProperty(QVariant::Int, strTmp);
		pProperty->setAttribute(QLatin1String("minimum"), UINT_LOW);
		pProperty->setAttribute(QLatin1String("maximum"), UINT_HIGH);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		////DataType
		//strTmp = tmp[CAIModel::COLUMN::DataType].desc;
		//pProperty = variantManager->addProperty(variantManager->enumTypeId(), strTmp);
		//QStringList list;
		//list << "1" << "2" << "3";
		//pProperty->setAttribute(QLatin1String("enumNames"), list);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		//InitValue
		strTmp = tmp[CAIModel::COLUMN::InitValue].desc;
		pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//InitialQua
		strTmp = tmp[CAIModel::COLUMN::InitQua].desc;
		pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//Address
		strTmp = tmp[CAIModel::COLUMN::Address].desc;
		pProperty = variantManager->addProperty(QVariant::String, strTmp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		//PinLabel
		strTmp = tmp[CAIModel::COLUMN::PinLabel].desc;
		pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		property->addSubProperty(pProperty);
		m_propertyToIdAI[pProperty] = strTmp;
		m_idToPropertyAI[strTmp] = pProperty;

		////ScaleType
		//strTmp = tmp[CAIModel::COLUMN::ScaleType].desc;
		//pProperty = variantManager->addProperty(QVariant::UInt, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////ScaleOccNo
		//strTmp = tmp[CAIModel::COLUMN::ScaleOccNo].desc;
		//pProperty = variantManager->addProperty(QVariant::Int, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////RangeL
		//strTmp = tmp[CAIModel::COLUMN::RangeL].desc;
		//pProperty = variantManager->addProperty(QVariant::Int, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////RangeH
		//strTmp = tmp[CAIModel::COLUMN::RangeH].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////HighQua
		//strTmp = tmp[CAIModel::COLUMN::HighQua].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////LowQua
		//strTmp = tmp[CAIModel::COLUMN::LowQua].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////SaveDisk
		//strTmp = tmp[CAIModel::COLUMN::SaveDisk].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////SaveDiskPeriod
		//strTmp = tmp[CAIModel::COLUMN::SaveDiskPeriod].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////SaveDB
		//strTmp = tmp[CAIModel::COLUMN::SaveDB].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//property->addSubProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////SaveDBPeriod
		//strTmp = tmp[CAIModel::COLUMN::SaveDBPeriod].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////Sensitivity
		//strTmp = tmp[CAIModel::COLUMN::Sensitivity].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////AlarmType
		//strTmp = tmp[CAIModel::COLUMN::AlarmType].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////AlarmOccNo
		//strTmp = tmp[CAIModel::COLUMN::AlarmOccNo].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////ChannelOccNo
		//strTmp = tmp[CAIModel::COLUMN::ChannelOccNo].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////DeviceOccNo
		//strTmp = tmp[CAIModel::COLUMN::DeviceOccNo].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////ReferenceCount
		//strTmp = tmp[CAIModel::COLUMN::ReferenceCount].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////DataSource
		//strTmp = tmp[CAIModel::COLUMN::DataSource].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////UnitName
		//strTmp = tmp[CAIModel::COLUMN::UnitName].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////Express
		//strTmp = tmp[CAIModel::COLUMN::Express].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		////GroupName
		//strTmp = tmp[CAIModel::COLUMN::GroupName].desc;
		//pProperty = variantManager->addProperty(QVariant::Double, strTmp);
		//pTree->addProperty(pProperty);
		//m_propertyToIdAI[pProperty] = strTmp;
		//m_idToPropertyAI[strTmp] = pProperty;

		
		//SetAIAttr(pModuleAI);

		int nTest = time.elapsed();

		nTest = 5;
	}
	else if (nType == DEVICE_DI)
	{
		CDIModel *pModuleDI = static_cast<CDIModel *>(pModule);

		CreateDIAttr(pModuleDI);
	}
	else if(nType == DEVICE_AO)
	{
		CAOModel *pModuleAO = static_cast<CAOModel *>(pModule);

		CreateAOAttr(pModuleAO);
	}
	else if (nType == DEVICE_DO)
	{
		CDOModel *pModuleDO = static_cast<CDOModel *>(pModule);

		CreateDOAttr(pModuleDO);
	}
}

/*! \fn void CChannel::CreateAIAttr(CAIModel *pModel)
********************************************************************************************************* 
** \brief CChannel::CreateAIAttr 
** \details 创建AI attr
** \param pModel 
** \return void 
** \author gw
** \date 2016年5月26日 
** \note 
********************************************************************************************************/
void CChannel::CreateAIAttr(CAIModel *pModel)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		m_pCore->LogMsg(CHANNEL_DESC, (char *)tr("Create AI attr fail").toStdString().c_str(), 1);
	
		return;
	}
	
	//if (m_bCreateAIAttr)
	//{
	//	return;
	//}

	//m_bCreateAIAttr = true;

	m_pAIModel = pModel;
	m_pModel = nullptr;

	//QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToIdAI.constBegin();
	//while (itProp != m_propertyToIdAI.constEnd())
	//{
	//	delete itProp.key();
	//	itProp++;
	//}
	m_propertyToIdAI.clear();
	m_idToPropertyAI.clear();

	CreateAttriConnect();

	QMap<int32s, CONFIG_PARA> tmp = pModel->GetColumnInfo();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	QString strTmp = tr("base");

	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToIdAI[property] = strTmp;
	m_idToPropertyAI[strTmp] = property;
	pTree->addProperty(property);

	//Tagname
	strTmp = tmp[CAIModel::COLUMN::TagName].desc;
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	QString pattern = QString(TAGNAMEREG).arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;


	//Desc
	strTmp = tmp[CAIModel::COLUMN::Desc].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//ID
	strTmp = tmp[CAIModel::COLUMN::ID].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;


	//OccNo
	strTmp = tmp[CAIModel::COLUMN::OccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//BlockOccNo
	strTmp = tmp[CAIModel::COLUMN::BlockOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	////DataType
	//strTmp = tmp[CAIModel::COLUMN::DataType].desc;
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	//InitValue
	strTmp = tmp[CAIModel::COLUMN::InitValue].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//InitialQua
	strTmp = tmp[CAIModel::COLUMN::InitQua].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//Address
	strTmp = tmp[CAIModel::COLUMN::Address].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//PinLabel
	strTmp = tmp[CAIModel::COLUMN::PinLabel].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	////ScaleType
	//strTmp = tmp[CAIModel::COLUMN::ScaleType].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////ScaleOccNo
	//strTmp = tmp[CAIModel::COLUMN::ScaleDesc].desc;
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	//RangeL
	strTmp = tmp[CAIModel::COLUMN::RangeL].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//RangeH
	strTmp = tmp[CAIModel::COLUMN::RangeH].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//HighQua
	strTmp = tmp[CAIModel::COLUMN::HighQua].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	//LowQua
	strTmp = tmp[CAIModel::COLUMN::LowQua].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAI[pProperty] = strTmp;
	m_idToPropertyAI[strTmp] = pProperty;

	////SaveDisk
	//strTmp = tmp[CAIModel::COLUMN::SaveDisk].desc;
	//pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////SaveDiskPeriod
	//strTmp = tmp[CAIModel::COLUMN::SaveDiskPeriod].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////SaveDB
	//strTmp = tmp[CAIModel::COLUMN::SaveDB].desc;
	//pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////SaveDBPeriod
	//strTmp = tmp[CAIModel::COLUMN::SaveDBPeriod].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////Sensitivity
	//strTmp = tmp[CAIModel::COLUMN::Sensitivity].desc;
	//pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////AlarmType
	//strTmp = tmp[CAIModel::COLUMN::AlarmType].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////AlarmOccNo
	//strTmp = tmp[CAIModel::COLUMN::AlarmOccNo].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////ChannelOccNo
	//strTmp = tmp[CAIModel::COLUMN::ChannelOccNo].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////DeviceOccNo
	//strTmp = tmp[CAIModel::COLUMN::DeviceOccNo].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////ReferenceCount
	//strTmp = tmp[CAIModel::COLUMN::ReferenceCount].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////DataSource
	//strTmp = tmp[CAIModel::COLUMN::DataSource].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////UnitName
	//strTmp = tmp[CAIModel::COLUMN::UnitName].desc;
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////Express
	//strTmp = tmp[CAIModel::COLUMN::Express].desc;
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;

	////GroupName
	//strTmp = tmp[CAIModel::COLUMN::GroupName].desc;
	//pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAI[pProperty] = strTmp;
	//m_idToPropertyAI[strTmp] = pProperty;
}

void CChannel::SetAIAttr(CAIModel *pModel)
{
	Q_UNUSED(pModel);
	
	QMap<QtProperty *, QString>::Iterator itProp = m_propertyToIdAI.begin();
	while (itProp != m_propertyToIdAI.end())
	{	
		m_pUi->GetAtrri()->GetStringProperty()->setValue(itProp.key(), "0");

		itProp++;
	}
}

/*! \fn void CChannel::ValueChangedAI(QtProperty *property, const QString &val)
********************************************************************************************************* 
** \brief CChannel::ValueChangedAI 
** \details 修改AI
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2016年5月23日 
** \note 
********************************************************************************************************/
void CChannel::ValueChangedAI(QtProperty *property, const QString &val)
{
	QString strTmp = m_propertyToIdAI[property];

	auto tmp = m_pAIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAIModel::COLUMN::TagName].desc) == 0)
	{
		//TagName
		auto AIs = m_pAIModel->GetAIs();
		
		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_szTagName = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::TagName), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::Desc].desc) == 0)
	{
		//Desc
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_strDescription = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::Desc), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::Address].desc) == 0)
	{
		//Address
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_strAddress = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::Address), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::PinLabel].desc) == 0)
	{
		//PinLabel
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_strPinLabel = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::PinLabel), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::Unit].desc) == 0)
	{
		//UnitName
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_strUnitName = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::Unit), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::Express].desc) == 0)
	{
		//Express
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_strExpress = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::Express), val, Qt::EditRole);
		}
	}

	m_pAIModel->RefrushModel();
}

void CChannel::ValueChangedAI(QtProperty *property, int val)
{
	QString strTmp = m_propertyToIdAI[property];

	auto tmp = m_pAIModel->GetColumnInfo();
	
	if (strTmp.compare(tmp[CAIModel::COLUMN::OccNo].desc) == 0)
	{
		//OccNo
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nOccNo = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::OccNo), val, Qt::EditRole);
		}
	}
	if (strTmp.compare(tmp[CAIModel::COLUMN::ID].desc) == 0)
	{
		//ID
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nID = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::ID), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::ChannelOccNo].desc) == 0)
	{
		//ChannelOccNo
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nChannelOccNo = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::ChannelOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::DeviceOccNo].desc) == 0)
	{
		//DeviceOccNo
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nDeviceOccNo = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::DeviceOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::BlockOccNo].desc) == 0)
	{
		//BlockOccNo
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nBlockOccNo = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::BlockOccNo), val, Qt::EditRole);
		}
	}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::DataType].desc) == 0)
	//{
	//	//DataType
	//	auto AIs = m_pAIModel->GetAIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//AIs[value]->m_nDataType = val;

	//		m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::DataType), val, Qt::EditRole);
	//	}
	//}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::InitQua].desc) == 0)
	{
		//InitQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nInitialQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::ReferenceCount].desc) == 0)
	{
		//ReferenceCount
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nReferenceCount = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::ReferenceCount), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::DataSource].desc) == 0)
	{
		//DataSource
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nDataSource = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::DataSource), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::SaveDiskPeriod].desc) == 0)
	{
		//SaveDiskPeriod
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nSaveDiskPeriod = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::SaveDiskPeriod), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::SaveDBPeriod].desc) == 0)
	{
		//SaveDBPeriod
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nSaveDBPeriod = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::SaveDBPeriod), val, Qt::EditRole);
		}
	}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::ScaleType].desc) == 0)
	//{
	//	//ScaleType
	//	auto AIs = m_pAIModel->GetAIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//AIs[value]->m_nScaleType = val;

	//		m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::ScaleType), val, Qt::EditRole);
	//	}
	//}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::ScaleDesc].desc) == 0)
	//{
	//	//ScaleOccNo
	//	auto AIs = m_pAIModel->GetAIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//AIs[value]->m_nScaleOccNo = val;

	//		m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::ScaleDesc), val, Qt::EditRole);
	//	}
	//}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::AlarmType].desc) == 0)
	//{
	//	//AlarmType
	//	auto AIs = m_pAIModel->GetAIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//AIs[value]->m_nAlarmType = val;

	//		m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::AlarmType), val, Qt::EditRole);
	//	}
	//}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::AlarmDesc].desc) == 0)
	//{
	//	//AlarmOccNo
	//	auto AIs = m_pAIModel->GetAIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//AIs[value]->m_nAlarmOccNo = val;

	//		m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::AlarmDesc), val, Qt::EditRole);
	//	}
	//}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::HighQua].desc) == 0)
	{
		//HighQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbHighQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::HighQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::LowQua].desc) == 0)
	{
		//LowQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbLowQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::LowQua), val, Qt::EditRole);
		}
	}

	m_pAIModel->RefrushModel();
}

/*! \fn void CChannel::ValueChangedAI(QtProperty *property, double val)
********************************************************************************************************* 
** \brief CChannel::ValueChangedAI 
** \details 
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2016年5月23日 
** \note 
********************************************************************************************************/
void CChannel::ValueChangedAI(QtProperty *property, double val)
{
	QString strTmp = m_propertyToIdAI[property];

	auto tmp = m_pAIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAIModel::COLUMN::InitValue].desc) == 0)
	{
		//InitValue
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbInitValue = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::InitValue), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::InitValue].desc) == 0)
	{
		//InitValue
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbInitValue = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::InitValue), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::Sensitivity].desc) == 0)
	{
		//Sensitivity
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbSensitivity = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::Sensitivity), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::RangeL].desc) == 0)
	{
		//RangeL
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbRangeL = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::RangeL), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::RangeH].desc) == 0)
	{
		//RangeH
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbRangeH = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::RangeH), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::HighQua].desc) == 0)
	{
		//HighQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbHighQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::HighQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::LowQua].desc) == 0)
	{
		//LowQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_dbLowQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::LowQua), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedAI(QtProperty *property, bool val)
{
	QString strTmp = m_propertyToIdAI[property];

	auto tmp = m_pAIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAIModel::COLUMN::SaveDisk].desc) == 0)
	{
		//SaveDisk
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_bSaveDisk = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::SaveDisk), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::SaveDB].desc) == 0)
	{
		//SaveDB
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_bSaveDB = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::SaveDB), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::InitQua].desc) == 0)
	{
		//InitQua
		auto AIs = m_pAIModel->GetAIs();

		foreach(const int &value, m_setSelect)
		{
			//AIs[value]->m_nInitialQua = val;

			m_pAIModel->setData(m_pAIModel->index(value, CAIModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
}

/*! \fn void CChannel::CreateDIAttr(CDIModel *pModel)
********************************************************************************************************* 
** \brief CChannel::CreateDIAttr 
** \details 创建DI attr
** \param pModel 
** \return void 
** \author gw
** \date 2016年5月26日 
** \note 
********************************************************************************************************/
void CChannel::CreateDIAttr(CDIModel *pModel)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		return;
	}

	//if (m_bCreateAIAttr)
	//{
	//	return;
	//}

	//m_bCreateDIAttr = true;

	m_pDIModel = pModel;
	m_pModel = nullptr;

	//QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToIdDI.constBegin();
	//while (itProp != m_propertyToIdDI.constEnd())
	//{
	//	delete itProp.key();
	//	itProp++;
	//}
	m_propertyToIdDI.clear();
	m_idToPropertyDI.clear();

	CreateAttriConnect();

	auto &tmp = pModel->GetColumnInfo();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	QString strTmp = tr("base");

	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToIdDI[property] = strTmp;
	m_idToPropertyDI[strTmp] = property;
	pTree->addProperty(property);

	//Tagname
	strTmp = tmp[CDIModel::COLUMN::TagName].desc;
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	QString pattern = QString(TAGNAMEREG).arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;


	//Desc
	strTmp = tmp[CDIModel::COLUMN::Desc].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//ID
	strTmp = tmp[CDIModel::COLUMN::ID].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;


	//OccNo
	strTmp = tmp[CDIModel::COLUMN::OccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//BlockOccNo
	strTmp = tmp[CDIModel::COLUMN::BlockOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//InitValue
	strTmp = tmp[CDIModel::COLUMN::InitValue].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//InitialQua
	strTmp = tmp[CDIModel::COLUMN::InitQua].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//Address
	strTmp = tmp[CDIModel::COLUMN::Address].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//PinLabel
	strTmp = tmp[CDIModel::COLUMN::PinLabel].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//SaveDisk
	strTmp = tmp[CDIModel::COLUMN::SaveDisk].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//SaveDiskPeriod
	strTmp = tmp[CDIModel::COLUMN::SaveDiskPeriod].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//SaveDB
	strTmp = tmp[CDIModel::COLUMN::SaveDB].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	////SaveDBPeriod
	//strTmp = tmp[CDIModel::COLUMN::SaveDBPeriod].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdDI[pProperty] = strTmp;
	//m_idToPropertyDI[strTmp] = pProperty;

	//AlarmType
	strTmp = tmp[CDIModel::COLUMN::AlarmTagName].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	////AlarmOccNo
	//strTmp = tmp[CDIModel::COLUMN::AlarmDesc].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdDI[pProperty] = strTmp;
	//m_idToPropertyDI[strTmp] = pProperty;

	//ChannelOccNo
	strTmp = tmp[CDIModel::COLUMN::ChannelOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//DeviceOccNo
	strTmp = tmp[CDIModel::COLUMN::DeviceOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//ReferenceCount
	strTmp = tmp[CDIModel::COLUMN::ReferenceCount].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//DataSource
	strTmp = tmp[CDIModel::COLUMN::DataSource].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//Express
	strTmp = tmp[CDIModel::COLUMN::Express].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//BitCloseString
	strTmp = tmp[CDIModel::COLUMN::BitCloseString].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//BitOpenString
	strTmp = tmp[CDIModel::COLUMN::BitOpenString].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;

	//SOE
	strTmp = tmp[CDIModel::COLUMN::SOE].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDI[pProperty] = strTmp;
	m_idToPropertyDI[strTmp] = pProperty;
}

void CChannel::ValueChangedDI(QtProperty *property, const QString &val)
{
	QString strTmp = m_propertyToIdDI[property];

	auto &tmp = m_pDIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDIModel::COLUMN::TagName].desc) == 0)
	{
		//TagName
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_szTagName = val;
			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::TagName), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::Desc].desc) == 0)
	{
		//Desc
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strDescription = val;
			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::Desc), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::Address].desc) == 0)
	{
		//Address
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strAddress = val;
			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::Address), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::PinLabel].desc) == 0)
	{
		//PinLabel
		//auto DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strPinLabel = val;
			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::PinLabel), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::Express].desc) == 0)
	{
		//Express
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strExpress = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::Express), val, Qt::EditRole);
		}
	}

	//m_pDIModel->RefrushModel();
}

void CChannel::ValueChangedDI(QtProperty *property, bool val)
{
	auto &strTmp = m_propertyToIdDI[property];
	auto &tmp = m_pDIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDIModel::COLUMN::SaveDB].desc) == 0)
	{
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strExpress = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDB), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::SaveDisk].desc) == 0)
	{
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strExpress = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDisk), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::InitQua].desc) == 0)
	{
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strExpress = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedDI(QtProperty *property, double val)
{
	QString strTmp = m_propertyToIdDI[property];

	auto &tmp = m_pDIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDIModel::COLUMN::InitValue].desc) == 0)
	{
		//InitValue
		auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			DIs[value]->m_bInitValue = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDisk), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::InitQua].desc) == 0)
	{
		//InitValue
		auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			DIs[value]->m_bInitValue = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDisk), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedDI(QtProperty *property, int val)
{
	QString strTmp = m_propertyToIdDI[property];

	auto &tmp = m_pDIModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDIModel::COLUMN::OccNo].desc) == 0)
	{
		//OccNo
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nOccNo = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::OccNo), val, Qt::EditRole);
		}
	}
	if (strTmp.compare(tmp[CDIModel::COLUMN::ID].desc) == 0)
	{
		//ID
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nID = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::ID), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::ChannelOccNo].desc) == 0)
	{
		//ChannelOccNo
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nChannelOccNo = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::ChannelOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::DeviceOccNo].desc) == 0)
	{
		//DeviceOccNo
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nDeviceOccNo = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::DeviceOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::BlockOccNo].desc) == 0)
	{
		//BlockOccNo
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nBlockOccNo = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::BlockOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::InitQua].desc) == 0)
	{
		//InitQua
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nInitialQua = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::ReferenceCount].desc) == 0)
	{
		//ReferenceCount
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nReferenceCount = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::ReferenceCount), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::DataSource].desc) == 0)
	{
		//DataSource
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nDataSource = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::DataSource), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::SaveDiskPeriod].desc) == 0)
	{
		//SaveDiskPeriod
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nSaveDiskPeriod = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDiskPeriod), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDIModel::COLUMN::SaveDBPeriod].desc) == 0)
	{
		//SaveDBPeriod
		//auto &DIs = m_pDIModel->GetDIs();

		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nSaveDBPeriod = val;

			m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::SaveDBPeriod), val, Qt::EditRole);
		}
	}
	//else if (strTmp.compare(tmp[CAIModel::COLUMN::AlarmType].desc) == 0)
	//{
	//	//AlarmType
	//	auto &DIs = m_pDIModel->GetDIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//DIs[value]->m_nAlarmType = val;

	//		m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::AlarmType), val, Qt::EditRole);
	//	}
	//}
	//else if (strTmp.compare(tmp[CDIModel::COLUMN::AlarmOccNo].desc) == 0)
	//{
	//	//AlarmOccNo
	//	auto &DIs = m_pDIModel->GetDIs();

	//	foreach(const int &value, m_setSelect)
	//	{
	//		//DIs[value]->m_nAlarmOccNo = val;

	//		m_pDIModel->setData(m_pDIModel->index(value, CDIModel::COLUMN::AlarmOccNo), val, Qt::EditRole);
	//	}
	//}

	m_pDIModel->RefrushModel();
}

void CChannel::CreateAOAttr(CAOModel *pModel)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		m_pCore->LogMsg(CHANNEL_DESC, (char *)tr("Create AO attr fail").toStdString().c_str(), 1);

		return;
	}

	//if (m_bCreateAIAttr)
	//{
	//	return;
	//}

	//m_bCreateAIAttr = true;

	m_pAOModel = pModel;
	m_pModel = nullptr;

	//QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToIdAI.constBegin();
	//while (itProp != m_propertyToIdAI.constEnd())
	//{
	//	delete itProp.key();
	//	itProp++;
	//}
	m_propertyToIdAO.clear();
	m_idToPropertyAO.clear();

	CreateAttriConnect();

	auto &tmp = pModel->GetColumnInfo();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	QString strTmp = tr("base");

	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToIdAO[property] = strTmp;
	m_idToPropertyAO[strTmp] = property;
	pTree->addProperty(property);

	//Tagname
	strTmp = tmp[CAOModel::COLUMN::TagName].desc;
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	QString pattern = QString(TAGNAMEREG).arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//Desc
	strTmp = tmp[CAOModel::COLUMN::Desc].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//ID
	strTmp = tmp[CAOModel::COLUMN::ID].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;


	//OccNo
	strTmp = tmp[CAOModel::COLUMN::OccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//BlockOccNo
	strTmp = tmp[CAOModel::COLUMN::BlockOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	////DataType
	//strTmp = tmp[CAOModel::COLUMN::DataType].desc;
	//pProperty = m_pUi->GetAtrri()->GetEnumProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAO[pProperty] = strTmp;
	//m_idToPropertyAO[strTmp] = pProperty;

	//InitValue
	strTmp = tmp[CAOModel::COLUMN::InitValue].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//InitialQua
	strTmp = tmp[CAOModel::COLUMN::InitQua].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//Address
	strTmp = tmp[CAOModel::COLUMN::Address].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//PinLabel
	strTmp = tmp[CAOModel::COLUMN::PinLabel].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	////ScaleType
	//strTmp = tmp[CAOModel::COLUMN::ScaleType].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//property->addSubProperty(pProperty);
	//m_propertyToIdAO[pProperty] = strTmp;
	//m_idToPropertyAO[strTmp] = pProperty;


	//RangeL
	strTmp = tmp[CAOModel::COLUMN::RangeL].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//RangeH
	strTmp = tmp[CAOModel::COLUMN::RangeH].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//HighQua
	strTmp = tmp[CAOModel::COLUMN::HighQua].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//LowQua
	strTmp = tmp[CAOModel::COLUMN::LowQua].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	////Sensitivity
	//strTmp = tmp[CAOModel::COLUMN::Sensitivity].desc;
	//pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAO[pProperty] = strTmp;
	//m_idToPropertyAO[strTmp] = pProperty;

	//ChannelOccNo
	strTmp = tmp[CAOModel::COLUMN::ChannelOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//DeviceOccNo
	strTmp = tmp[CAOModel::COLUMN::DeviceOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	//ReferenceCount
	strTmp = tmp[CAOModel::COLUMN::ReferenceCount].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

	////DataSource
	//strTmp = tmp[CAOModel::COLUMN::DataSource].desc;
	//pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdAO[pProperty] = strTmp;
	//m_idToPropertyAO[strTmp] = pProperty;

	//UnitName
	strTmp = tmp[CAOModel::COLUMN::Unit].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdAO[pProperty] = strTmp;
	m_idToPropertyAO[strTmp] = pProperty;

}

void CChannel::ValueChangedAO(QtProperty *property, const QString &val)
{
	QString strTmp = m_propertyToIdAO[property];

	auto &tmp = m_pAOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAOModel::COLUMN::TagName].desc) == 0)
	{
		//TagName
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::TagName), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::Desc].desc) == 0)
	{
		//Desc
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::Desc), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::Address].desc) == 0)
	{
		//Address
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::Address), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::PinLabel].desc) == 0)
	{
		//PinLabel
		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strPinLabel = val;
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::PinLabel), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedAO(QtProperty *property, bool val)
{
	auto &strTmp = m_propertyToIdAO[property];
	auto &tmp = m_pAOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAOModel::COLUMN::InitQua].desc) == 0)
	{
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedAO(QtProperty *property, double val)
{
	QString strTmp = m_propertyToIdAO[property];

	auto &tmp = m_pAOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAOModel::COLUMN::InitValue].desc) == 0)
	{
		//InitValue
		auto &DIs = m_pAOModel->GetAOs();

		foreach(const int &value, m_setSelect)
		{
			DIs[value]->m_dbInitValue = val;

			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::InitValue), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedAO(QtProperty *property, int val)
{
	QString strTmp = m_propertyToIdAO[property];

	auto &tmp = m_pAOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CAOModel::COLUMN::OccNo].desc) == 0)
	{
		//OccNo
		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nOccNo = val;

			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::OccNo), val, Qt::EditRole);
		}
	}
	if (strTmp.compare(tmp[CAOModel::COLUMN::ID].desc) == 0)
	{
		//ID
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::ID), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::ChannelOccNo].desc) == 0)
	{
		//ChannelOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::ChannelOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAIModel::COLUMN::DeviceOccNo].desc) == 0)
	{
		//DeviceOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::DeviceOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::BlockOccNo].desc) == 0)
	{
		//BlockOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::BlockOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::InitQua].desc) == 0)
	{
		//InitQua
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CAOModel::COLUMN::ReferenceCount].desc) == 0)
	{
		//ReferenceCount
		foreach(const int &value, m_setSelect)
		{
			m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::ReferenceCount), val, Qt::EditRole);
		}
	}
	//else if (strTmp == tmp[CAOModel::COLUMN::ScaleType].desc)
	//{
	//	//ScaleType
	//	foreach(const int &value, m_setSelect)
	//	{
	//		m_pAOModel->setData(m_pAOModel->index(value, CAOModel::COLUMN::ScaleType), val, Qt::EditRole);
	//	}
	//}
}

void CChannel::CreateDOAttr(CDOModel *pModel)
{
	Q_ASSERT(pModel);

	if (!pModel)
	{
		m_pCore->LogMsg(CHANNEL_DESC, (char *)tr("Create DO attr fail").toStdString().c_str(), 1);

		return;
	}

	//if (m_bCreateAIAttr)
	//{
	//	return;
	//}

	//m_bCreateAIAttr = true;

	m_pDOModel = pModel;
	m_pModel = nullptr;

	//QMap<QtProperty *, QString>::ConstIterator itProp = m_propertyToIdAI.constBegin();
	//while (itProp != m_propertyToIdAI.constEnd())
	//{
	//	delete itProp.key();
	//	itProp++;
	//}
	m_propertyToIdDO.clear();
	m_idToPropertyDO.clear();

	CreateAttriConnect();

	auto &tmp = pModel->GetColumnInfo();

	QtTreePropertyBrowser *pTree = m_pUi->GetAtrri()->GetTreePropertyBrowser();

	pTree->clear();

	QtGroupPropertyManager *pGroup = m_pUi->GetAtrri()->GetGroupProperty();

	QString strTmp = tr("base");

	QtProperty *property = pGroup->addProperty(strTmp);
	m_propertyToIdDO[property] = strTmp;
	m_idToPropertyDO[strTmp] = property;
	pTree->addProperty(property);

	//Tagname
	strTmp = tmp[CDOModel::COLUMN::TagName].desc;
	QtProperty *pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	QString pattern = QString(TAGNAMEREG).arg(MAX_NAME_LENGTH_SCADASTUDIO);
	QRegExp regExp;
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//Desc
	strTmp = tmp[CDOModel::COLUMN::Desc].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$").arg(CHANNELNAME_SIZE);
	regExp.setPattern(pattern);
	m_pUi->GetAtrri()->GetStringProperty()->setRegExp(pProperty, regExp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//ID
	strTmp = tmp[CDOModel::COLUMN::ID].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	m_pUi->GetAtrri()->GetIntProperty()->setRange(pProperty, 1, UINT_HIGH);
	m_pUi->GetAtrri()->GetIntProperty()->setMinimum(pProperty, UINT_LOW);
	m_pUi->GetAtrri()->GetIntProperty()->setMaximum(pProperty, UINT_HIGH);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;


	//OccNo
	strTmp = tmp[CDOModel::COLUMN::OccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//BlockOccNo
	strTmp = tmp[CDOModel::COLUMN::BlockOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//InitValue
	strTmp = tmp[CDOModel::COLUMN::InitValue].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//InitialQua
	strTmp = tmp[CDOModel::COLUMN::InitQua].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//Address
	strTmp = tmp[CDOModel::COLUMN::Address].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//PinLabel
	strTmp = tmp[CDOModel::COLUMN::PinLabel].desc;
	pProperty = m_pUi->GetAtrri()->GetStringProperty()->addProperty(strTmp);
	property->addSubProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	////Sensitivity
	//strTmp = tmp[CDOModel::COLUMN::Sensitivity].desc;
	//pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	//pTree->addProperty(pProperty);
	//m_propertyToIdDO[pProperty] = strTmp;
	//m_idToPropertyDO[strTmp] = pProperty;

	//ChannelOccNo
	strTmp = tmp[CDOModel::COLUMN::ChannelOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//DeviceOccNo
	strTmp = tmp[CDOModel::COLUMN::DeviceOccNo].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//ReferenceCount
	strTmp = tmp[CDOModel::COLUMN::ReferenceCount].desc;
	pProperty = m_pUi->GetAtrri()->GetIntProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//IsSBO
	strTmp = tmp[CDOModel::COLUMN::IsSBO].desc;
	pProperty = m_pUi->GetAtrri()->GetBoolProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//TelCtlWaitTime
	strTmp = tmp[CDOModel::COLUMN::TelCtlWaitTime].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//TelCtlSendTime
	strTmp = tmp[CDOModel::COLUMN::TelCtlSendTime].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;

	//TelCtlExecTime
	strTmp = tmp[CDOModel::COLUMN::TelCtlSendTime].desc;
	pProperty = m_pUi->GetAtrri()->GetDoubleProperty()->addProperty(strTmp);
	pTree->addProperty(pProperty);
	m_propertyToIdDO[pProperty] = strTmp;
	m_idToPropertyDO[strTmp] = pProperty;
}

void CChannel::ValueChangedDO(QtProperty *property, const QString &val)
{
	Q_ASSERT(m_pDOModel);
	
	if (!m_pDOModel)
	{
		return;
	}

	QString strTmp = m_propertyToIdDO[property];

	auto &tmp = m_pDOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDOModel::COLUMN::TagName].desc) == 0)
	{
		//TagName
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::TagName), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::Desc].desc) == 0)
	{
		//Desc
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pAOModel->index(value, CDOModel::COLUMN::Desc), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::Address].desc) == 0)
	{
		//Address
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::Address), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::PinLabel].desc) == 0)
	{
		//PinLabel
		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_strPinLabel = val;
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::PinLabel), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedDO(QtProperty *property, bool val)
{
	auto &strTmp = m_propertyToIdDO[property];
	auto &tmp = m_pDOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDOModel::COLUMN::InitQua].desc) == 0)
	{
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
	if (strTmp.compare(tmp[CDOModel::COLUMN::IsSBO].desc) == 0)
	{
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::IsSBO), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedDO(QtProperty *property, double val)
{
	QString strTmp = m_propertyToIdDO[property];

	auto &tmp = m_pDOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDOModel::COLUMN::InitValue].desc) == 0)
	{
		//InitValue
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::InitValue), val, Qt::EditRole);
		}
	}
	else if (strTmp == tmp[CDOModel::COLUMN::TelCtlWaitTime].desc)
	{
		//TelCtlWaitTime
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::TelCtlWaitTime), val, Qt::EditRole);
		}
	}
	else if (strTmp == tmp[CDOModel::COLUMN::TelCtlSendTime].desc)
	{
		//TelCtlWaitTime
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::TelCtlSendTime), val, Qt::EditRole);
		}
	}
	else if (strTmp == tmp[CDOModel::COLUMN::TelCtlExecTime].desc)
	{
		//TelCtlWaitTime
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::TelCtlExecTime), val, Qt::EditRole);
		}
	}
}

void CChannel::ValueChangedDO(QtProperty *property, int val)
{
	QString strTmp = m_propertyToIdDO[property];

	auto &tmp = m_pDOModel->GetColumnInfo();

	if (strTmp.compare(tmp[CDOModel::COLUMN::OccNo].desc) == 0)
	{
		//OccNo
		foreach(const int &value, m_setSelect)
		{
			//DIs[value]->m_nOccNo = val;

			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::OccNo), val, Qt::EditRole);
		}
	}
	if (strTmp.compare(tmp[CDOModel::COLUMN::ID].desc) == 0)
	{
		//ID
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::ID), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::ChannelOccNo].desc) == 0)
	{
		//ChannelOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::ChannelOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::DeviceOccNo].desc) == 0)
	{
		//DeviceOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::DeviceOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::BlockOccNo].desc) == 0)
	{
		//BlockOccNo
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::BlockOccNo), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::InitQua].desc) == 0)
	{
		//InitQua
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::InitQua), val, Qt::EditRole);
		}
	}
	else if (strTmp.compare(tmp[CDOModel::COLUMN::ReferenceCount].desc) == 0)
	{
		//ReferenceCount
		foreach(const int &value, m_setSelect)
		{
			m_pDOModel->setData(m_pDOModel->index(value, CDOModel::COLUMN::ReferenceCount), val, Qt::EditRole);
		}
	}
}

void CChannel::DisConnect()
{
	disconnect(m_pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	disconnect(m_pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	disconnect(m_pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	disconnect(m_pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));

	disconnect(m_pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
}

/*! \fn void CChannel::DeviceExportToBeModel(QModelIndex &index)
********************************************************************************************************* 
** \brief CChannel::DeviceExportToBeModel 
** \details 导出为装置模板
** \param index 
** \return void 
** \author gw
** \date 2016年8月27日 
** \note 
********************************************************************************************************/
void CChannel::DeviceExportToBeModelAction(QModelIndex &index)
{
	CExportModelDialog dialog;
	dialog.setWindowModality(Qt::ApplicationModal);
	bool bFlag = false;
	if (dialog.exec() == QDialog::Accepted)
	{
		bFlag = true;
	}
	else
	{
		bFlag = false;

		return;
	}

	auto strDeviceTagName = index.data(Qt::EditRole).toString();
	auto pDevice = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pDevice);
	if (!pDevice)
	{
		auto strError = QString(tr("device tag name %1 can find!!!")).arg(strDeviceTagName);
		m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

		return;
	}

	QString strProjectPath = m_pCore->GetProjMgr()->GetProjectPath();

	ExportToBeModel(pDevice, dialog.m_strModelPath, dialog.m_strModelDesc, dialog.m_strFactory);
}

/*! \fn void CChannel::SaveToBeModel(Config::CDevice *pDevice)
********************************************************************************************************* 
** \brief CChannel::SaveToBeModel 
** \details 保存为装置模板
** \param pDevice 
** \return void 
** \author gw
** \date 2016年8月27日 
** \note 
********************************************************************************************************/
#include <QDebug>
void CChannel::ExportToBeModel(Config::CDevice *pDevice, QString &strModelPath, QString &strModelDesc, QString &strFactory)
{
	Q_UNUSED(strModelDesc);
	Q_UNUSED(strFactory);

	QString strTmp = m_pCore->GetProjMgr()->GetProjectPath();
	strTmp = strTmp + "/templates/" + strModelPath + ".xml";
	
	QFile file(strTmp);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug("Failed to open file for writing.");
		return ;
	}

	QDomDocument document("MyML");

	QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
	document.appendChild(instruction);

	QDomElement root = document.createElement("device");//创建根节点  
	root.setAttribute("name", pDevice->m_strDescription);
	root.setAttribute("factory", "");

	//遥测接口

	//遥信接口

	//遥控接口

	//遥调接口
	document.appendChild(root);

	file.resize(0);

	QTextStream stream(&file);
	stream.setDevice(&file);
	document.save(stream, 4);
}

Config::CFesData * CChannel::GetFes(QModelIndex &index)
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