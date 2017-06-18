#include "fes_di_select_doubledi_dialog.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "config_data.h"
#include "scadastudio/icore.h"
#include "forwarding_select_dimodel.h"
#include "datatypes.h"
#include "fes_di_select_doubledi_model.h"

#include <QStandardItemModel>

#include <QTableView>

CFes_di_select_doubledi_dialog::CFes_di_select_doubledi_dialog(QWidget *parent, Config::CFesData *pFes)
	: QDialog(parent), m_pModel(nullptr)
{
	ui.setupUi(this);

	auto pModel = new QStandardItemModel(this);
	ui.treeView->setModel(pModel);

	auto pTopFesItem = new CQuoteItem(tr("fes config"));
	pTopFesItem->setIcon(QIcon(FES_PNG));
	pTopFesItem->setEditable(false);

	pModel->appendRow(pTopFesItem);

	InitChannel(pTopFesItem, pFes);

	ui.treeView->resize(58, ui.treeView->size().height());

	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);

	connect(ui.treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
	connect(ui.pushButtonOk, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CFes_di_select_doubledi_dialog::~CFes_di_select_doubledi_dialog()
{

}

void CFes_di_select_doubledi_dialog::InitChannel(CQuoteItem *pItem, Config::CFesData *pFesData)
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

bool CFes_di_select_doubledi_dialog::InitChannelGroup(CQuoteItem *pItem, Config::CChannelGroup *pGroup)
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

void CFes_di_select_doubledi_dialog::InitDevice(CQuoteItem *pItem, std::vector <Config::CDevice *> &pTmp)
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

void CFes_di_select_doubledi_dialog::DoubleClicked(const QModelIndex &index)
{
	auto nType = index.data(Qt::UserRole).toInt();

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

	if (nType == FES_DEVICE_DI)
	{
		auto pTmp = reinterpret_cast<Config::CDevice *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pTmp);
		if (!pTmp)
		{
			return;
		}

		if (m_pModel)
		{
			delete m_pModel;
			m_pModel = nullptr;
		}

		m_pModel = new CFes_DI_Select_DoubleDI_DataModel(nullptr, pTmp);
		ui.tableView->setModel(m_pModel);
	}
}

void CFes_di_select_doubledi_dialog::OnOk()
{
	if (!ui.tableView->currentIndex().isValid())
	{
		return;
	}

	int row = ui.tableView->currentIndex().row();
	QAbstractItemModel *model = ui.tableView->model();
	QModelIndex index = model->index(row, CForwardingDISelectDataModel::COLUMN::TagName);//
	QVariant data = model->data(index);

	m_strTagName = data.toString();

	accept();
	close();
}

void CFes_di_select_doubledi_dialog::OnCancel()
{
	m_strTagName.clear();

	reject();
	close();
}