/*! @file
<PRE>
********************************************************************************
模块名       :  前置选点对话框
文件名       :  selectpointdialog.cpp
文件实现功能 :  前置选点
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
*  @date	2016.8.19
*/

#include "selectpointdialog.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "fesselectpointmodel.h"
#include "fesapi/fescnst.h"
#include "fesapi/fesdatadef.h"
#include "../fespub/tag_att.h"



#include <QStandardItem>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QComboBox>
#include <QMessageBox>

extern IMainModuleInterface *s_pGlobleCore;

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

CSelectPointDialog::CSelectPointDialog(QWidget *parent, Config::CFesConfigData *pConfig
	, const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName)
	: QDialog(parent), m_pModel(nullptr), m_pFesSelectPointModel(nullptr), m_pSort(nullptr), m_szTagName(szTagName)
	, m_arrAllowDataTypes(arrAllowDataTypes), m_bFilterEnWrite(bFilterEnWrite)
	, m_szNodeTagName(szNodeTagName), m_szFiledName(szFiledName), m_nType(NOSELECT), m_pTagAtt(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}

	m_pConfig = pConfig;

	ui.setupUi(this);
	ui.lineEdit->setReadOnly(true);

	m_pSort = new QSortFilterProxyModel(this);
	ui.tableView->setModel(m_pSort);
	m_pSort->sort(0);
	//m_pSort->setFilterKeyColumn()

	if (!m_pTagAtt)
	{
		m_pTagAtt = new CTagAttMgr;
	}

	connect(ui.lineEdit_3, SIGNAL(textChanged(QString)), this, SLOT(FilterChanged(QString)));
	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(filterColumnChanged(int)));

	connect(ui.treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(ui.ok, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.cancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(SelectPoint(const QModelIndex &)));

	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	CreateFesUi(m_pConfig);


}

CSelectPointDialog::~CSelectPointDialog()
{
	if (!m_pTagAtt)
	{
		delete m_pTagAtt;
		m_pTagAtt = nullptr;
	}
}

/*! \fn void CSelectPointDialog::InitUi()
********************************************************************************************************* 
** \brief CSelectPointDialog::InitUi 
** \details 
** \return void 
** \author gw
** \date 2016年9月28日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitUi()
{
	ui.treeView;
}

/*! \fn void CSelectPointDialog::CreateFesUi(Config::CFesConfigData *pConfig)
********************************************************************************************************* 
** \brief CSelectPointDialog::CreateFesUi 
** \details 前置选点对话框创建界面
** \param pConfig 
** \return void 
** \author gw
** \date 2016年9月29日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::CreateFesUi(Config::CFesConfigData *pConfig)
{
	Q_UNUSED(pConfig);
	
	m_pModel = new QStandardItemModel(this);

	ui.treeView->setModel(m_pModel);

	auto strTitleName = QObject::tr("fes select point");
	m_pModel->setHorizontalHeaderLabels(QStringList() << strTitleName);

	//auto &arrVec = pConfig->GetFecArr();

	auto pTopFesItem = new CQuoteItem(tr("fes config"));
	pTopFesItem->setData(FES_CONFIG, Qt::UserRole);
	pTopFesItem->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pTopFesItem->setData(reinterpret_cast<long long>(m_pConfig->m_pFesGroup), POINTERDATA);
	pTopFesItem->setIcon(QIcon(FES_PNG));
	pTopFesItem->setEditable(false);

	m_pModel->appendRow(pTopFesItem);

	CreateFesGroupUi(m_pConfig->m_pFesGroup, pTopFesItem);

	QMap<QString, CQuoteItem *> groupItem;

	//for (auto const &tmp : arrVec)
	//{
	//	CQuoteItem *pTmp = pTopFesItem;

	//	auto &strGroup = tmp->m_strGroupName;

	//	//分组;
	//	QStringList list = strGroup.split(".");

	//	if (strGroup.isEmpty())
	//	{
	//		//组空
	//	}
	//	else if (groupItem.contains(strGroup))
	//	{
	//		//已建立组
	//		pTmp = groupItem[strGroup];
	//	}
	//	else
	//	{
	//		QString strTmp;
	//		CQuoteItem *newItem;

	//		for (int32s i = 0; i < list.count(); i++)
	//		{
	//			strTmp += list[i];

	//			QString strTagName = list[i];

	//			newItem = new CQuoteItem(list[i]);
	//			newItem->setData(list[i], LASTVALUE);
	//			newItem->setData(FES_ITEM_GROUP, Qt::UserRole);
	//			//newItem->setData(strTagName, TAGNAME);
	//			newItem->setData(reinterpret_cast<long long>(tmp), POINTERDATA);
	//			newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
	//			newItem->setEditable(false);

	//			pTmp->appendRow(newItem);

	//			if (!groupItem.contains(strTmp))
	//			{
	//				groupItem.insert(strTmp, newItem);
	//			}

	//			pTmp = newItem;

	//			strTmp += ".";
	//		}
	//	}


	//	//添加前置节点
	//	CQuoteItem *pFesItem = new CQuoteItem(tmp->m_szTagName);
	//	pFesItem->setData(tmp->m_szTagName, LASTVALUE);
	//	pFesItem->setData(FES_ITEM, Qt::UserRole);
	//	//pFesItem->setData(tmp->m_szTagName, TAGNAME);
	//	pFesItem->setData((reinterpret_cast<std::size_t>(tmp)), POINTERDATA);
	//	pFesItem->setData(tmp->m_strDescription, TREE_TOOL_TIP_ROLE);
	//	pFesItem->setIcon(QIcon(FES_PNG));
	//	pFesItem->setEditable(false);

	//	pTmp->appendRow(pFesItem);

	//	//初始化树分支
	//	InitChannel(pFesItem, tmp);

	//	//InitTransform(pFesItem, tmp);

	//	//InitAlarm(pFesItem, tmp);
	//	
	//	InitVariable(pFesItem, tmp);
	//}
}


void CSelectPointDialog::CreateFesGroupUi(Config::CFesGroup *pFesGroup, CQuoteItem *pItem)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pFesGroup->m_arrFesGroup.begin();
	for (; it != pFesGroup->m_arrFesGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroup;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(FES_ITEM_GROUP, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		CreateFesGroupUi(it->second, newItem);
	}

	auto ite = pFesGroup->m_arrFesItem.begin();
	for (; ite != pFesGroup->m_arrFesItem.end(); ++ite)
	{
		//添加前置节点
		auto pFes = ite->second;
		CQuoteItem *pFesItem = new CQuoteItem(pFes->m_szTagName);
		pFesItem->setData(pFes->m_szTagName, LASTVALUE);
		pFesItem->setData(FES_ITEM, Qt::UserRole);
		//pFesItem->setData(tmp->m_szTagName, TAGNAME);
		pFesItem->setData((reinterpret_cast<std::size_t>(pFes)), POINTERDATA);
		pFesItem->setData(pFes->m_strDescription, TREE_TOOL_TIP_ROLE);
		pFesItem->setIcon(QIcon(FES_PNG));
		pFesItem->setEditable(false);

		pItem->appendRow(pFesItem);

		//初始化树分支
		InitChannel(pFesItem, pFes);

		//InitTransform(pFesItem, pFes);

		//InitAlarm(pFesItem, pFes);

		InitVariable(pFesItem, pFes);
	}
}

/*! \fn void CSelectPointDialog::InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CSelectPointDialog::InitChannel 
** \details 初始化界面通道模块
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年9月29日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}

	auto strChannel = tr("channel list");
	//通道
	CQuoteItem *itemChannel = new CQuoteItem(strChannel);
	itemChannel->setData(strChannel, LASTVALUE);
	//channel node root group
	itemChannel->setData(reinterpret_cast<long long>(pFesData->m_pChannelGroup), POINTERDATA);
	itemChannel->setData(FES_CHANNEL_ITEM, Qt::UserRole);
	itemChannel->setIcon(QIcon(CHANNEL_PNG));
	itemChannel->setEditable(false);

	pItem->appendRow(itemChannel);

	InitChannelGroup(itemChannel, pFesData->m_pChannelGroup);
}


bool CSelectPointDialog::InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup)
{
	QString strTmp;
	CQuoteItem *newItem;

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;
	auto it = pGroup->m_arrChannelGroup.begin();
	for (; it != pGroup->m_arrChannelGroup.end(); it++)
	{
		auto strGroupName = it->second->m_strGroup;
		newItem = new CQuoteItem(strGroupName);
		newItem->setData(strGroupName, LASTVALUE);
		newItem->setData(FES_CHANNEL_GROUP_ITEM, Qt::UserRole);
		newItem->setData(reinterpret_cast<long long>(it->second), POINTERDATA);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitChannelGroup(newItem, it->second);
	}

	auto ite = pGroup->m_arrChannelItem.begin();
	for (; ite != pGroup->m_arrChannelItem.end(); ++ite)
	{
		//添加前置节点
		auto pChannel = ite->second;
		CQuoteItem *pChannelItem = new CQuoteItem(pChannel->m_szTagName);
		pChannelItem->setData(pChannel->m_szTagName, LASTVALUE);
		pChannelItem->setData(FES_CHANNEL_CHILD_ITEM, Qt::UserRole);
		//pFesItem->setData(tmp->m_szTagName, TAGNAME);
		pChannelItem->setData((reinterpret_cast<std::size_t>(pChannel)), POINTERDATA);
		pChannelItem->setData(pChannel->m_strDescription, TREE_TOOL_TIP_ROLE);
		pChannelItem->setIcon(QIcon(FES_PNG));
		pChannelItem->setEditable(false);

		pItem->appendRow(pChannelItem);

		InitDevice(pChannelItem, pChannel->m_arrDevices);
	}

	return true;
}

/*! \fn void InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
********************************************************************************************************* 
** \brief InitDevice 
** \details 初始化界面装置模块
** \param pItem 
** \param pTmp 
** \return void 
** \author gw
** \date 2016年9月29日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return;
	}

	for (int32u i = 0; i < pTmp.size(); i++)
	{
		Q_ASSERT(pTmp[i]);
		if (!pTmp[i])
		{
			auto strError = QObject::tr("init device  device is nullptr!!!");
			s_pGlobleCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

			return;
		}

		//装置
		//装置名字
		CQuoteItem *newItem = new CQuoteItem(pTmp[i]->m_szTagName);

		newItem->setData(pTmp[i]->m_szTagName, LASTVALUE);
		newItem->setData(FES__CHANNEL_CHILD_DEVICE_ITEM, Qt::UserRole);
		//Tagname
		//newItem->setData(pTmp[i]->m_szTagName, TAGNAME);
		newItem->setData((reinterpret_cast<long long>(pTmp[i])), POINTERDATA);
		newItem->setData(pTmp[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		newItem->setIcon(QIcon(DEVICE_PNG));
		newItem->setEditable(false);

		////////////////////////////////////////////////////////////////////////////
		auto strDesc = QObject::tr("ai");
		QString strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		auto *pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = QObject::tr("di");
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = QObject::tr("ao");
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		strDesc = QObject::tr("do");
		strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		pSubItem = new CQuoteItem(strAITag);
		pSubItem->setData(strAITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DO, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);
		////////////////////////////////////////////////////////////////////////////

		pItem->appendRow(newItem);
	}
}

/*! \fn void CSelectPointDialog::InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CSelectPointDialog::InitVariable 
** \details 初始化变量
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	//内存变量
	CQuoteItem *itemVariableList = new CQuoteItem(tr("memory variable"));
	itemVariableList->setData(FES_VARIABLE_ITEM, Qt::UserRole);
	itemVariableList->setIcon(QIcon(VARIABLE_PNG));
	itemVariableList->setEditable(false);

	pItem->appendRow(itemVariableList);


	InitSystemVariable(itemVariableList, pFesData);

	InitUserVariable(itemVariableList, pFesData);
}

/*! \fn void CSelectPointDialog::InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CSelectPointDialog::InitSystemVariable 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitSystemVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	//系统变量
	auto strTmp = tr("system variable");
	CQuoteItem *itemSysVariableList = new CQuoteItem(strTmp);
	itemSysVariableList->setData(FES_SYSTEM_VARIABLE_ITEM, Qt::UserRole);
	itemSysVariableList->setData(reinterpret_cast<long long>(pFesData), POINTERDATA);
	itemSysVariableList->setData(strTmp, TREE_TOOL_TIP_ROLE);
	itemSysVariableList->setIcon(QIcon(VARIABLE_SYSTEM_PNG));
	itemSysVariableList->setEditable(false);


	pItem->appendRow(itemSysVariableList);
}

/*! \fn void CSelectPointDialog::InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
********************************************************************************************************* 
** \brief CSelectPointDialog::InitUserVariable 
** \details 
** \param pItem 
** \param pFesData 
** \return void 
** \author gw
** \date 2016年10月12日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}
	
	//用户变量
	CQuoteItem *itemUserVariableList = new CQuoteItem(tr("user variable"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	pItem->appendRow(itemUserVariableList);

	Q_ASSERT(pFesData->m_pUserVariableGroup);
	if (!pFesData->m_pUserVariableGroup)
	{
		return;
	}
	InitUserVariableGroup(itemUserVariableList, pFesData->m_pUserVariableGroup);
}

bool CSelectPointDialog::InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup)
{
	Q_ASSERT(pItem);
	if (!pItem)
	{
		return false;
	}

	Q_ASSERT(pGroup);
	if (!pGroup)
	{
		return false;
	}

	int nDeepths = pItem->data(GROUP_NUMBER_ROLE).toInt();

	nDeepths++;

	auto &arrGroup = pGroup->GetGroup();
	for each (auto var in arrGroup)
	{

		auto newItem = new CQuoteItem(var->GetGroupName());

		newItem->setData(var->GetGroupName(), LASTVALUE);
		newItem->setData(var->GetGroupName(), TREE_TOOL_TIP_ROLE);
		newItem->setData(reinterpret_cast<long long>(var), POINTERDATA);
		newItem->setData(FES_USER_VARIBALE_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitUserVariableGroup(newItem, var);
	}

	return true;
}

/*! \fn void CSelectPointDialog::doubleClicked(const QModelIndex &index)
********************************************************************************************************* 
** \brief CSelectPointDialog::doubleClicked 
** \details 槽函数
** \param index 
** \return void 
** \author gw
** \date 2016年9月29日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::DoubleClicked(const QModelIndex &index)
{
	auto nType = index.data(Qt::UserRole).toInt();
	//if (m_pFesSelectPointModel)
	//{
	//	delete m_pFesSelectPointModel;
	//	m_pFesSelectPointModel = nullptr;
	//}

	auto indexTmp = index;

	QString strFesTagName = "";
	while (indexTmp.parent().isValid())
	{
		auto nType = indexTmp.data(Qt::UserRole).toInt();
		if (nType == FES_ITEM)
		{
			strFesTagName = indexTmp.data(Qt::EditRole).toString();
			break;
		}

		indexTmp = indexTmp.parent();
	}

	m_szNodeTagName = strFesTagName.toStdString();

	m_pSort->sourceModel()->deleteLater();

	if (nType == FES_DEVICE_AI)
	{
		m_nType = FES_DEVICE_AI;

		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_DEVICE_AI);

		m_pSort->setSourceModel(m_pFesSelectPointModel);

		ui.comboBox_2->clear();
		CTagAttMgr mgr; 
		if (m_bFilterEnWrite)
		{
			//可写 CTagAttMgr::ATT_RW
			std::vector<std::string> vec;
			vec.clear();
			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalWriteableAttrssWithDataType(IDD_AIN, var, vec);
				
				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
		else
		{
			//不可写 CTagAttMgr::ATT_RO
			std::vector<std::string> vec;
			vec.clear();
			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_AIN, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
	}
	else if (nType == FES_DEVICE_DI)
	{
		m_nType = FES_DEVICE_DI;
		
		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_DEVICE_DI);

		m_pSort->setSourceModel(m_pFesSelectPointModel);

		ui.comboBox_2->clear();
		CTagAttMgr mgr;
		std::vector<std::string> vec;
		if (m_bFilterEnWrite)
		{
			std::vector<std::string> vec;
			vec.clear();

			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_DIN, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
		else
		{
			std::vector<std::string> vec;
			vec.clear();
			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_DIN, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
	}
	else if (nType == FES_DEVICE_AO)
	{
		m_nType = FES_DEVICE_AO;
		
		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_DEVICE_AO);

		m_pSort->setSourceModel(m_pFesSelectPointModel);

		ui.comboBox_2->clear();
		CTagAttMgr mgr;
		if (m_bFilterEnWrite)
		{
			std::vector<std::string> vec;
			vec.clear();

			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_AOUT, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
		else
		{
			std::vector<std::string> vec;
			vec.clear();
			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_AOUT, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
	}
	else if (nType == FES_DEVICE_DO)
	{
		m_nType = FES_DEVICE_DO;
		
		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_DEVICE_DO);

		m_pSort->setSourceModel(m_pFesSelectPointModel);

		ui.comboBox_2->clear();
		CTagAttMgr mgr;
		if (m_bFilterEnWrite)
		{
			std::vector<std::string> vec;
			vec.clear();

			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_DOUT, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
		else
		{
			std::vector<std::string> vec;
			vec.clear();
			for each (auto var in m_arrAllowDataTypes)
			{
				mgr.GetTotalReadOnlyAttrsWithDataType(IDD_DOUT, var, vec);

				for each (auto var in vec)
				{
					ui.comboBox_2->addItem(var.c_str());
				}
			}
		}
	}
	else if (nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		m_nType = FES_USER_VARIBALE_GROUP_ITEM;
		
		//前置用户变量组
		auto pTmp = reinterpret_cast<Config::CUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}
	
		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_USER_VARIBALE_GROUP_ITEM);

		m_pSort->setSourceModel(m_pFesSelectPointModel);

		ui.comboBox_2->clear();
		InitAttrCombo(IDD_USERVAR);
	}
	else if (nType == FES_SYSTEM_VARIABLE_ITEM)
	{
		m_nType = FES_SYSTEM_VARIABLE_ITEM;
		
		//前置用户变量组
		auto pTmp = reinterpret_cast<Config::CFesData *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		m_pFesSelectPointModel = new CFesSelectPointModel(this, pTmp, FES_SYSTEM_VARIABLE_ITEM);
		
		m_pSort->setSourceModel(m_pFesSelectPointModel);
		ui.comboBox_2->clear();
		InitAttrCombo(IDD_SYSVAR);
	}
	else
	{
		m_nType = NOSELECT;
		
		m_pSort->setSourceModel(nullptr);
		ui.comboBox_2->clear();
	}
}

void CSelectPointDialog::InitAttrCombo(int nType)
{
	ui.comboBox_2->clear();

	if (m_bFilterEnWrite)
	{
		std::vector<std::string> vec;
		vec.clear();

		for each (auto var in m_arrAllowDataTypes)
		{
			m_pTagAtt->GetTotalReadOnlyAttrsWithDataType(nType, var, vec);

			for each (auto var in vec)
			{
				ui.comboBox_2->addItem(var.c_str());
			}
		}
	}
	else
	{
		std::vector<std::string> vec;
		vec.clear();
		for each (auto var in m_arrAllowDataTypes)
		{
			m_pTagAtt->GetTotalReadOnlyAttrsWithDataType(nType, var, vec);

			for each (auto var in vec)
			{
				ui.comboBox_2->addItem(var.c_str());
			}
		}
	}
}

/*! \fn void CSelectPointDialog::OnOk()
********************************************************************************************************* 
** \brief CSelectPointDialog::OnOk 
** \details 字符串为空返回
** \return void 
** \author gw
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::OnOk()
{	
	//没有选择点
	if (m_nType == NOSELECT)
	{
		QMessageBox box;
		box.setText(QObject::tr("u donot select any data!!!"));
		box.exec();

		return;
	}

	//系统变量没有属性
	if (m_nType == FES_SYSTEM_VARIABLE_ITEM || m_nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		m_szFiledName = "Value";
	}
	else if (m_nType == FES_DEVICE_AI || m_nType == FES_DEVICE_DI ||
		m_nType == FES_DEVICE_AO || m_nType == FES_DEVICE_DO)
	{
		if (ui.comboBox_2->count() == 0)
		{
			QMessageBox box;
			box.setText(QObject::tr("Attribute combox is empty, please check the conditon!!!"));
			box.exec();

			return;
		}

		auto strText = ui.comboBox_2->currentText();

		if (m_szTagName.empty())
		{
			return;
		}

		if (strText.isEmpty())
		{
			//默认属性
			m_szTagName = "Value";
		}
		else
		{
			m_szFiledName = strText.toStdString();
		}
	}

	auto pDialog = static_cast<QDialog *>(this->parent()->parent()->parent());
	Q_ASSERT(pDialog);
	if (!pDialog)
	{
		return;
	}

	pDialog->accept();
	pDialog->close();
	//this->parent()->parent()->parent()->deleteLater();
}

void CSelectPointDialog::OnCancel()
{
	m_szNodeTagName.clear();
	m_szTagName.clear();
	m_szFiledName.clear();

	auto pDialog = static_cast<QDialog *>(this->parent()->parent()->parent());
	Q_ASSERT(pDialog);
	if (!pDialog)
	{
		return;
	}

	pDialog->reject();
	pDialog->close();

	//this->parent()->parent()->parent()->deleteLater();
} 

/*! \fn void CSelectPointDialog::SelectPoint(const QModelIndex &index)
********************************************************************************************************* 
** \brief CSelectPointDialog::SelectPoint 
** \details 前置选点
** \param index 
** \return void 
** \author gw
** \date 2016年9月30日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::SelectPoint(const QModelIndex &index)
{
	m_szNodeTagName;
	QModelIndex indexTmp = m_pSort->mapToSource(index);

	indexTmp = m_pFesSelectPointModel->index(indexTmp.row(), CFesSelectPointModel::TagName);

	m_szTagName = m_pFesSelectPointModel->data(indexTmp, Qt::DisplayRole).toString().toStdString();

	m_szTagName = m_szTagName;

	auto strTmp = QString("%1.%2").arg(m_szNodeTagName.c_str()).arg(m_szTagName.c_str());

	ui.lineEdit->setText(strTmp);
}

/*! \fn void CSelectPointDialog::FilterChanged(QString text)
********************************************************************************************************* 
** \brief CSelectPointDialog::FilterChanged 
** \details 根据字符串过滤
** \param text 
** \return void 
** \author gw
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::FilterChanged(QString text)
{
	QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax::FixedString;
	QRegExp regExp(text, Qt::CaseInsensitive, syntax);
	m_pSort->setFilterRegExp(regExp);
}

/*! \fn void CSelectPointDialog::filterColumnChanged()
********************************************************************************************************* 
** \brief CSelectPointDialog::filterColumnChanged 
** \details 根据列过滤
** \return void 
** \author gw
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::filterColumnChanged(int nIndex)
{
	Q_UNUSED(nIndex);
	m_pSort->setFilterKeyColumn(ui.comboBox->currentIndex());
}

/*! \fn void CSelectPointDialog::showMouseRightButton(const QPoint&)
********************************************************************************************************* 
** \brief CSelectPointDialog::showMouseRightButton 
** \details 
** \param & 
** \return void 
** \author gw
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
void CSelectPointDialog::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = ui.treeView->indexAt(point);

	QMenu *pMenu = new QMenu(NULL);
	
	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
	QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
	pMenu->addAction(pActionExpand);
	pMenu->addAction(pActionCollapse);

	QAction* selectedItem = pMenu->exec(QCursor::pos());

	if (selectedItem == pActionExpand)
	{
		ExpandChildren(indexSelect, ui.treeView);
	}
	else if (selectedItem == pActionCollapse)
	{
		CollapseChildren(indexSelect, ui.treeView);
	}

	pMenu->deleteLater();
}

void CSelectPointDialog::reject()
{	
	return;
}