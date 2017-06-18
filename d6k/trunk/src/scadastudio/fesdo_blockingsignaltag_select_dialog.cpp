#include "fesDo_BlockingSignalTag_Select_Dialog.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "fes_do_blockingsignaltag_select_devicedi_model.h"
#include "fes_do_blockingsignaletan_select_fesuservaraible_model.h"

#include <QMessageBox>

CFesDo_BlockingSignalTag_Select_Dialog::CFesDo_BlockingSignalTag_Select_Dialog(QWidget *parent, Config::CFesData *pFes)
	: QDialog(parent), m_pDeviceDIModel(nullptr), m_pFesUserVaraibleModel(nullptr), m_strTagName("")
{
	ui.setupUi(this);

	auto pModel = new QStandardItemModel(this);
	ui.treeView->setModel(pModel);

	auto pTopFesItem = new CQuoteItem(tr("fes config"));
	pTopFesItem->setIcon(QIcon(FES_PNG));
	pTopFesItem->setEditable(false);

	pModel->appendRow(pTopFesItem);

	//初始化通道
	InitChannel(pTopFesItem, pFes);
	//初始化用户变量
	InitUserVariable(pTopFesItem, pFes);

	ui.treeView->resize(58, ui.treeView->size().height());

	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);

	connect(ui.treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CFesDo_BlockingSignalTag_Select_Dialog::~CFesDo_BlockingSignalTag_Select_Dialog()
{

}

void CFesDo_BlockingSignalTag_Select_Dialog::InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
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

bool CFesDo_BlockingSignalTag_Select_Dialog::InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup)
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

void CFesDo_BlockingSignalTag_Select_Dialog::InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
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

		//////////////////////////////////////////////////////////////////////////////
		//auto strDesc = QObject::tr("ai");
		//QString strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		//auto *pSubItem = new CQuoteItem(strAITag);
		//pSubItem->setData(strAITag, LASTVALUE);
		//pSubItem->setData(FES_DEVICE_AI, Qt::UserRole);
		////pSubItem->setData(strAITag, TAGNAME);
		//pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		//pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		//pSubItem->setIcon(QIcon(FES_PNG));
		//pSubItem->setEditable(false);
		//newItem->appendRow(pSubItem);

		auto strDesc = QObject::tr("di");
		auto strDITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		auto pSubItem = new CQuoteItem(strDITag);
		pSubItem->setData(strDITag, LASTVALUE);
		pSubItem->setData(FES_DEVICE_DI, Qt::UserRole);
		//pSubItem->setData(strAITag, TAGNAME);
		pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		pSubItem->setIcon(QIcon(FES_PNG));
		pSubItem->setEditable(false);
		newItem->appendRow(pSubItem);

		//strDesc = QObject::tr("ao");
		//strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		//pSubItem = new CQuoteItem(strAITag);
		//pSubItem->setData(strAITag, LASTVALUE);
		//pSubItem->setData(FES_DEVICE_AO, Qt::UserRole);
		////pSubItem->setData(strAITag, TAGNAME);
		//pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		//pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		//pSubItem->setIcon(QIcon(FES_PNG));
		//pSubItem->setEditable(false);
		//newItem->appendRow(pSubItem);

		//strDesc = QObject::tr("do");
		//strAITag = QString("%1_%2").arg(pTmp[i]->m_szTagName).arg(strDesc);
		//pSubItem = new CQuoteItem(strAITag);
		//pSubItem->setData(strAITag, LASTVALUE);
		//pSubItem->setData(FES_DEVICE_DO, Qt::UserRole);
		////pSubItem->setData(strAITag, TAGNAME);
		//pSubItem->setData(reinterpret_cast<long long>(pTmp[i]), POINTERDATA);
		//pSubItem->setData(strDesc, TREE_TOOL_TIP_ROLE);
		//pSubItem->setIcon(QIcon(FES_PNG));
		//pSubItem->setEditable(false);
		//newItem->appendRow(pSubItem);
		//////////////////////////////////////////////////////////////////////////////

		pItem->appendRow(newItem);
	}
}

void CFesDo_BlockingSignalTag_Select_Dialog::InitUserVariable(CQuoteItem *pItem, Config::CFesData *pFesData)
{
	Q_ASSERT(pFesData);
	if (!pFesData)
	{
		return;
	}

	Q_ASSERT(pFesData->m_pUserVariableGroup);
	if (!pFesData->m_pUserVariableGroup)
	{
		return;
	}

	//用户变量
	CQuoteItem *itemUserVariableList = new CQuoteItem(tr("user variable"));
	itemUserVariableList->setData(FES_USER_VARIABLE_ITEM, Qt::UserRole);
	itemUserVariableList->setData(reinterpret_cast<long long>(pFesData->m_pUserVariableGroup), POINTERDATA);
	itemUserVariableList->setIcon(QIcon(VARIABLE_USER_PNG));
	itemUserVariableList->setEditable(false);

	pItem->appendRow(itemUserVariableList);

	InitUserVariableGroup(itemUserVariableList, pFesData->m_pUserVariableGroup);
}

bool CFesDo_BlockingSignalTag_Select_Dialog::InitUserVariableGroup(CQuoteItem *pItem, Config::CUserVariableGroup *pGroup)
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
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitUserVariableGroup(newItem, var);
	}

	return true;
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesDo_BlockingSignalTag_Select_Dialog::DoubleClicked 
** \details 
** \param index 
** \return void 
** \author gw
** \date 2017年5月26日 
** \note 
********************************************************************************************************/
void CFesDo_BlockingSignalTag_Select_Dialog::DoubleClicked(const QModelIndex &index)
{
	auto nType = index.data(Qt::UserRole).toInt();
	auto indexTmp = index;

	if (nType == FES_DEVICE_DI)
	{
		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		ui.tableView->setModel(nullptr);

		if (m_pDeviceDIModel)
		{
			delete m_pDeviceDIModel;
			m_pDeviceDIModel = nullptr;
		}

		if (m_pFesUserVaraibleModel)
		{
			delete m_pFesUserVaraibleModel;
			m_pFesUserVaraibleModel = nullptr;
		}

		m_pDeviceDIModel = new CFes_Do_BlockingSignalTag_Select_DeviceDI_Model(nullptr, pTmp);
		ui.tableView->setModel(m_pDeviceDIModel);
	}
	else if (nType == FES_USER_VARIBALE_GROUP_ITEM)
	{
		//用户变量 root
		auto pVariable = reinterpret_cast<Config::CUserVariableGroup *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pVariable);
		if (!pVariable)
		{
			return;
		}

		ui.tableView->setModel(nullptr);

		if (m_pDeviceDIModel)
		{
			delete m_pDeviceDIModel;
			m_pDeviceDIModel = nullptr;
		}

		if (m_pFesUserVaraibleModel)
		{
			delete m_pFesUserVaraibleModel;
			m_pFesUserVaraibleModel = nullptr;
		}

		m_pFesUserVaraibleModel = new CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model(ui.tableView, pVariable);
		ui.tableView->setModel(m_pFesUserVaraibleModel);
	}
}

/*! \fn 
********************************************************************************************************* 
** \brief CFesDo_BlockingSignalTag_Select_Dialog::OnOk 
** \details 
** \return void 
** \author gw
** \date 2017年5月26日 
** \note 
********************************************************************************************************/
void CFesDo_BlockingSignalTag_Select_Dialog::OnOk()
{
	if (!ui.tableView->currentIndex().isValid())
	{
		return;
	}

	int row = ui.tableView->currentIndex().row();
	if (m_pDeviceDIModel)
	{
		m_strTagName = m_pDeviceDIModel->index(row, CFes_Do_BlockingSignalTag_Select_DeviceDI_Model::COLUMN::TagName).data(Qt::EditRole).toString();
	}
	else if (m_pFesUserVaraibleModel)
	{
		m_strTagName = m_pFesUserVaraibleModel->index(row, CFes_Do_BlockingSignaleTag_Select_FesUserVaraible_Model::COLUMN::TagName).data(Qt::EditRole).toString();
	
		
	}

	if (m_strTagName.isEmpty())
	{
		QMessageBox box;
		box.setText(QObject::tr("No data is selected!!!"));
		box.exec();

		return;
	}

	accept();
	close();
}

void CFesDo_BlockingSignalTag_Select_Dialog::OnCancel()
{
	reject();
	close();
}
