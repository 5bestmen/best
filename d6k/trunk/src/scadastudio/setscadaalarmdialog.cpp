#include "setscadaalarmdialog.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/define.h"
#include "scadavirable_data.h"
#include "delegate.h"

#include <QStandardItemModel>
#include <QMenu>

extern void ExpandChildren(const QModelIndex &index, QTreeView *view);
extern void CollapseChildren(const QModelIndex &index, QTreeView *view);

CSetScadaAlarmDialog::CSetScadaAlarmDialog(QWidget *parent, Config::CScadaVariableConfigData *pConfig, QString strTagName, Config::CScadaData *pScada, int nType)
	: QDialog(parent), m_pConfig(nullptr), m_pModel(nullptr)
{
	Q_ASSERT(pConfig);
	if (!pConfig)
	{
		return;
	}
	m_pConfig = pConfig;
	
	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}

	ui.setupUi(this);

	m_pModel = new QStandardItemModel(this);
	ui.treeView->setModel(m_pModel);

	InitPara();

	InitAlarm(pScada, nType);

	DumpModel(m_pModel, m_pModel->indexFromItem(m_pModel->invisibleRootItem()), strTagName);

	ui.treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Clicked(const QModelIndex &)));
	connect(ui.pOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(ui.pCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
	connect(ui.treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
}

CSetScadaAlarmDialog::~CSetScadaAlarmDialog()
{

}

void CSetScadaAlarmDialog::InitAlarm(Config::CScadaData *pScada, int nType)
{
	//报警
	auto strAlarm = tr("alarm list");
	CQuoteItem *itemAlarm = new CQuoteItem(strAlarm);
	itemAlarm->setData(SCADAVARIABLE_ITEM_ALARM, Qt::UserRole);
	itemAlarm->setIcon(QIcon(ALARM_PNG));
	itemAlarm->setEditable(false);
	m_pModel->appendRow(itemAlarm);

	if (nType == AlarmType::AIALARM)
	{
		//模拟量报警
		CQuoteItem *itemAnalogAlarm = new CQuoteItem(tr("analogalarm"));
		itemAnalogAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM, Qt::UserRole);
		itemAnalogAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
		itemAnalogAlarm->setData(reinterpret_cast<long long>(pScada->m_pScadaAnalogAlarmGroup), POINTERDATA);
		itemAnalogAlarm->setIcon(QIcon(ALARM_PNG));
		itemAnalogAlarm->setEditable(false);
		itemAlarm->appendRow(itemAnalogAlarm);

		InitAIAlarmGroup(itemAnalogAlarm, pScada->m_pScadaAnalogAlarmGroup);
	}
	else
	{
		//开关量报警
		CQuoteItem *itemDigtalAlarm = new CQuoteItem(tr("digtalalarm"));
		itemDigtalAlarm->setData(SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM, Qt::UserRole);
		itemDigtalAlarm->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
		itemDigtalAlarm->setData(reinterpret_cast<long long>(pScada->m_pScadaDigitalAlarmGroup), POINTERDATA);
		itemDigtalAlarm->setIcon(QIcon(ALARM_PNG));
		itemDigtalAlarm->setEditable(false);
		itemAlarm->appendRow(itemDigtalAlarm);

		InitDIAlarmGroup(itemDigtalAlarm, pScada->m_pScadaDigitalAlarmGroup);
	}

	ui.treeView->expandAll();
}

bool CSetScadaAlarmDialog::InitAIAlarmGroup(CQuoteItem *pItem, Config::CScadaAnalogAlarmGroup *pGroup)
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
		newItem->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitAIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(false);

			//InitAIOfflimit(pItemAlarm, var->m_arrAlarmOfflimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

void CSetScadaAlarmDialog::InitAIOfflimit(CQuoteItem *pItem, std::vector<Config::CScadaAinAlarmLimit *> &m_arrAlarmOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrAlarmOfflimit.size(); i++)
	{
		CQuoteItem *pAIAlarm = new CQuoteItem(m_arrAlarmOfflimit[i]->m_szTagName);

		pAIAlarm->setData(SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM, Qt::UserRole);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, LASTVALUE);

		//Tagname
		//pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_szTagName, TAGNAME);
		pAIAlarm->setData(m_arrAlarmOfflimit[i]->m_strDescription, TREE_TOOL_TIP_ROLE);
		pAIAlarm->setData(reinterpret_cast<long long>(m_arrAlarmOfflimit[i]), POINTERDATA);
		pAIAlarm->setIcon(QIcon(ALARM_PNG));
		pAIAlarm->setEditable(false);

		pItem->appendRow(pAIAlarm);
	}
}

bool CSetScadaAlarmDialog::InitDIAlarmGroup(CQuoteItem *pItem, Config::CScadaDigitalAlarmGroup *pGroup)
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
		newItem->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));
		newItem->setEditable(false);

		pItem->appendRow(newItem);

		InitDIAlarmGroup(newItem, var);
	}

	auto &arrItem = pGroup->GetItem();
	for each (auto var in arrItem)
	{
		{
			//名字
			CQuoteItem *pItemAlarm = new CQuoteItem(var->m_szTagName);
			pItemAlarm->setData(var->m_szTagName, LASTVALUE);
			pItemAlarm->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			pItemAlarm->setData(reinterpret_cast<long long>(var), POINTERDATA);
			pItemAlarm->setData(var->m_strDescription, TREE_TOOL_TIP_ROLE);
			pItemAlarm->setIcon(QIcon(SCALE_PNG));
			pItemAlarm->setEditable(false);

			//InitDIOfflimit(pItemAlarm, var->m_arrDigtallimit);

			pItem->appendRow(pItemAlarm);
		}
	}

	return true;
}

void CSetScadaAlarmDialog::InitDIOfflimit(CQuoteItem *pItem, std::vector<Config::CScadaDinAlarmLimit *> &m_arrOfflimit)
{
	Q_ASSERT(pItem);

	for (int32u i = 0; i < m_arrOfflimit.size(); i++)
	{
		CQuoteItem *pDIAlarm = new CQuoteItem(m_arrOfflimit[i]->Description);
		pDIAlarm->setData(SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM, Qt::UserRole);
		//Tagname
		pDIAlarm->setData(m_arrOfflimit[i]->TagName, Qt::EditRole);
		pDIAlarm->setData(reinterpret_cast<long long>(m_arrOfflimit[i]), POINTERDATA);
		pDIAlarm->setIcon(QIcon(ALARM_PNG));
		pDIAlarm->setEditable(false);

		pItem->appendRow(pDIAlarm);
	}
}

void CSetScadaAlarmDialog::Clicked(const QModelIndex &index)
{
	int nType = index.data(Qt::UserRole).toInt();
	QString strTagname = index.data(Qt::EditRole).toString();

	if (nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD)
	{
		while (ui.m_pTableAI->rowCount() > 0)
		{
			ui.m_pTableAI->removeRow(0);
		}

		ui.m_pTableAI->clear();
		ui.m_pTableAI->clearContents();

		while (ui.m_pTableAIOfflimit->rowCount() > 0)
		{
			ui.m_pTableAIOfflimit->removeRow(0);
		}

		ui.m_pTableAIOfflimit->clear();


		auto pAnalogAlarm = reinterpret_cast<Config::CScadaAnalogAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pAnalogAlarm);
		if (!pAnalogAlarm)
		{
			return;
		}

		QStringList list;
		list.clear();
		list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood") << tr("DeadType") << tr("DeadArea")
			<< tr("AlarmType");
		ui.m_pTableAI->setColumnCount(7);
		ui.m_pTableAI->setHorizontalHeaderLabels(list);

		ui.m_pTableAI->insertRow(ui.m_pTableAI->rowCount());
		QTableWidgetItem *pItem = nullptr;
		pItem = new QTableWidgetItem(pAnalogAlarm->m_szTagName);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, TagName, pItem);
		pItem = new QTableWidgetItem(pAnalogAlarm->m_strDescription);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Description, pItem);
		//enable
		pItem = new QTableWidgetItem();

		if (pAnalogAlarm->m_bEnable)
		{
			pItem->setCheckState(Qt::Checked);
		} 
		else
		{
			pItem->setCheckState(Qt::Unchecked);
		}
		pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Enable, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_bOnQualityGood));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, OnQualityGood, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_dbDeadArea));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, DeadType, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_nDeadType));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, DeadArea, pItem);
		//pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarm->m_nAlarmType));
		//pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		//ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, AlarmType, pItem);

		for each (auto var in pAnalogAlarm->m_arrAlarmOfflimit)
		{
			Q_ASSERT(var);
			if (!var)
			{
				return;
			}

			auto pAnalogAlarmLimit = var;
			if (!pAnalogAlarmLimit)
			{
				ui.m_pTableAIOfflimit;
			}

			QStringList list;
			list.clear();
			list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
				<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");

			ui.m_pTableAIOfflimit->setColumnCount(12);
			ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
			
			ui.m_pTableAIOfflimit->insertRow(ui.m_pTableAIOfflimit->rowCount());

			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_szTagName);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitTagName, pItem);
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_strDescription);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDescription, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_nID));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitID, pItem);

			//condition
			auto bFlag = m_arrAnalogAlarmConditon.contains(pAnalogAlarmLimit->m_nCondition);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAnalogAlarmConditon[pAnalogAlarmLimit->m_nCondition]));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCondition, pItem);

			//category
			bFlag = m_arrAlarmCategory.contains(pAnalogAlarmLimit->m_nCategory);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAlarmCategory[pAnalogAlarmLimit->m_nCategory]));

			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCategory, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_dbValue));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_dbLowValue));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitLowValue, pItem);
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_nDelayAlarm);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDelayAlarm, pItem);
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_nPriority);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitPriority, pItem);
			pItem = new QTableWidgetItem(pAnalogAlarmLimit->m_bSupportAck);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportAck, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_bSupportDelete));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportDelete, pItem);

			//AckType
			bFlag = m_arrAlarmCategory.contains(pAnalogAlarmLimit->m_nAckType);
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAlarmCategory[pAnalogAlarmLimit->m_nAckType]));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitAckType, pItem);
			//pItem = new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_bLog));
			//pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, 12, pItem);
		}

		m_strTagName = pAnalogAlarm->m_szTagName;
		m_strDesc = pAnalogAlarm->m_strDescription;
		m_nAlarmType = /*pAnalogAlarm->m_nAlarmType*/AIALARM;
	}
	else if (nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD)
	{
		while (ui.m_pTableAI->rowCount() > 0)
		{
			ui.m_pTableAI->removeRow(0);
		}

		ui.m_pTableAI->clear();

		while (ui.m_pTableAIOfflimit->rowCount() > 0)
		{
			ui.m_pTableAIOfflimit->removeRow(0);
		}

		ui.m_pTableAIOfflimit->clear();

		auto pDigitalAlarm = reinterpret_cast<Config::CScadaDigitalAlarm *>(index.data(POINTERDATA).toLongLong());
		Q_ASSERT(pDigitalAlarm);
		if (!pDigitalAlarm)
		{
			return;
		}

		QStringList list;
		list.clear();
		list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood");
		ui.m_pTableAI->setColumnCount(4);
		ui.m_pTableAI->setHorizontalHeaderLabels(list);

		ui.m_pTableAI->insertRow(ui.m_pTableAI->rowCount());
		QTableWidgetItem *pItem = nullptr;
		pItem = new QTableWidgetItem(pDigitalAlarm->m_szTagName);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, TagName, pItem);
		pItem = new QTableWidgetItem(pDigitalAlarm->m_strDescription);
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Description, pItem);
		//enable
		pItem = new QTableWidgetItem();
		if (pDigitalAlarm->m_bEnable)
		{
			pItem->setCheckState(Qt::Checked);
		}
		else
		{
			pItem->setCheckState(Qt::Unchecked);
		}
		pItem->setFlags(pItem->flags() & (~Qt::ItemIsEditable));
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, Enable, pItem);
		pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarm->m_bOnQualityGood));
		pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
		ui.m_pTableAI->setItem(ui.m_pTableAI->rowCount() - 1, OnQualityGood, pItem);


		list.clear();
		list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
			<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");
		ui.m_pTableAIOfflimit->setColumnCount(12);
		ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
		for each (auto var in pDigitalAlarm->m_arrDigtallimit)
		{
			Q_ASSERT(var);
			if (!var)
			{
				return;
			}

			auto pDigitalAlarmLimit = var;
			if (!pDigitalAlarmLimit)
			{
				return;
			}

			ui.m_pTableAIOfflimit->insertRow(ui.m_pTableAIOfflimit->rowCount());

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->TagName);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitTagName, pItem);

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->Description);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDescription, pItem);

			pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarmLimit->ID));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitID, pItem);

			//condition
			auto bFlag = m_arrAnalogAlarmConditon.contains(pDigitalAlarmLimit->Condition);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pDigitalAlarmLimit->Condition]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCondition, pItem);
			//ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, 3, new QTableWidgetItem(QString("%1").arg(pAnalogAlarmLimit->m_nCondition)));

			//category
			bFlag = m_arrAlarmCategory.contains(pDigitalAlarmLimit->Category);
			Q_ASSERT(bFlag);
			if (!bFlag)
			{
				return;
			}
			pItem = new QTableWidgetItem(m_arrAnalogAlarmConditon[pDigitalAlarmLimit->Category]);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitCategory, pItem);

			//value
			if (pDigitalAlarmLimit->Condition == DIAlarmContion::ZEROTOONE)
			{
				pItem = new QTableWidgetItem(QString("%1").arg(tr("ZEROTOONE")));
				pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
				ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			}
			else
			{
				pItem = new QTableWidgetItem(QString("%1").arg(tr("ONETOZERO")));
				pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
				ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitValue, pItem);
			}

			pItem = new QTableWidgetItem(QString("%1").arg(""));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitLowValue, pItem);

			//
			pItem = new QTableWidgetItem(pDigitalAlarmLimit->DelayAlarm);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitDelayAlarm, pItem);

			pItem = new QTableWidgetItem(pDigitalAlarmLimit->Priority);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitPriority, pItem);
			pItem = new QTableWidgetItem(pDigitalAlarmLimit->SupportAck);
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportAck, pItem);
			pItem = new QTableWidgetItem(QString("%1").arg(pDigitalAlarmLimit->SupportDelete));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitSupportDelete, pItem);

			//AckType
			bFlag = m_arrAlarmCategory.contains(pDigitalAlarmLimit->AckType);
			pItem = new QTableWidgetItem(QString("%1").arg(m_arrAlarmCategory[pDigitalAlarmLimit->AckType]));
			pItem->setFlags(pItem->flags() ^ Qt::ItemIsEditable);
			ui.m_pTableAIOfflimit->setItem(ui.m_pTableAIOfflimit->rowCount() - 1, LimitAckType, pItem);
		}

		m_strTagName = pDigitalAlarm->m_szTagName;
		m_strDesc = pDigitalAlarm->m_strDescription;
		m_nAlarmType = /*pDigitalAlarm->m_nAlarmType*/DIALARM;
	}
}

void CSetScadaAlarmDialog::OnOK()
{
	if (ui.m_pTableAI->rowCount() > 0)
	{
		accept();
	}
	else
	{
		rejected();
		this->close();
	}
}

void CSetScadaAlarmDialog::OnCancel()
{
	rejected();

	this->close();
}

void CSetScadaAlarmDialog::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = ui.treeView->indexAt(point);

	QMenu *pMenu = new QMenu(NULL);

	QAction *pActionExpand = new QAction(EXPAND_ITEM, pMenu);
	pMenu->addAction(pActionExpand);

	QAction *pActionCollapse = new QAction(COLLAPSE_ITEM, pMenu);
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

void CSetScadaAlarmDialog::DumpModel(const QAbstractItemModel *pModel, const QModelIndex &top, const QString &s)
{
	QModelIndex Item;

	if (!top.isValid()) {

	}

	for (int r = 0; r < pModel->rowCount(top); r++)
	{
		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);

			const QString strTmp = Item.data(Qt::EditRole).toString();

			auto nType = Item.data(Qt::UserRole).toInt();
			if (s.compare(strTmp) == 0 &&
				(nType == SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD || nType == SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD))
			{
				ui.treeView->setCurrentIndex(Item);

				//qDebug() << s;
			}

			//qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			DumpModel(pModel, Item, s);
		}
	}
}

/*! \fn void CSetScadaAlarmDialog::InitPara()
********************************************************************************************************* 
** \brief CSetScadaAlarmDialog::InitPara 
** \details 
** \return void 
** \author gw
** \date 2017年2月6日 
** \note 
********************************************************************************************************/
void CSetScadaAlarmDialog::InitPara()
{
	QStringList list;
	list.clear();
	list << tr("TagName") << tr("Description") << tr("Enable") << tr("OnQualityGood") << tr("DeadType") << tr("DeadArea")
		<< tr("AlarmType");
	ui.m_pTableAI->setColumnCount(7);
	ui.m_pTableAI->setHorizontalHeaderLabels(list);

	//AATagName, AADescription, AAEnable, AAOnQualityGood, AADeadType, AADeadArea, AAAlarmType

	CONFIG_PARA para;

	para.desc = tr("Enable");
	para.tagname = "Enable";
	m_horizontal_header_list.append(para.desc);
	//false 不可以修改checkbox
	CCheckBoxDelegate *pCheck = new CCheckBoxDelegate(this, false);
	ui.m_pTableAI->setItemDelegateForColumn(Enable, pCheck);
	
	para.desc = tr("OnQualityGood");
	para.tagname = "OnQualityGood";
	m_horizontal_header_list.append(para.desc);
	ui.m_pTableAI->setItemDelegateForColumn(OnQualityGood, pCheck);

	para.desc = tr("DeadArea");
	para.tagname = "DeadArea";
	m_horizontal_header_list.append(para.desc);
	auto delegate = new CLineEditDelegate(this, DOUBLETYPE, SIX_COUNT);
	ui.m_pTableAI->setItemDelegateForColumn(DeadArea, delegate);

	QVector<IDDESC> arrDeadType;
	arrDeadType.clear();

	IDDESC tmp;
	tmp.desc = tr("value");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::VALUE);
	arrDeadType.append(tmp);
	arrDeadType.append(tmp);

	tmp.desc = tr("percent");
	tmp.ID = QString("%1").arg(AnalogAlarmDeadArea::PERCENT);
	arrDeadType.append(tmp);
	CComboBoxDelegate *pCombo = new CComboBoxDelegate(this, arrDeadType);
	ui.m_pTableAI->setItemDelegateForColumn(DeadType, pCombo);

	//limit contidon
	m_arrAnalogAlarmConditon.clear();
	m_arrAnalogAlarmConditon.insert(AlarmContion::GREATEREQUAL, QObject::tr("GREATEREQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::LESSEQUAL, QObject::tr("LESSEQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::EQUAL, QObject::tr("EQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::NOTEQUAL, QObject::tr("NOTEQUAL"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::BWTEEN, QObject::tr("BWTEEN"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::RATEREDUCE, QObject::tr("RATEREDUCE"));
	m_arrAnalogAlarmConditon.insert(AlarmContion::RATEREDUCE, QObject::tr("RATEINCREASE"));

	m_arrDigitalAlarmConditon.clear();
	m_arrDigitalAlarmConditon.insert(DIAlarmContion::ZEROTOONE, QObject::tr("ZEROTOONE"));
	m_arrDigitalAlarmConditon.insert(DIAlarmContion::ONETOZERO, QObject::tr("ONETOZERO"));

	m_arrAlarmCategory.clear();
	m_arrAlarmCategory.insert(AlarmCategory::ACCIDENT, QObject::tr("ACCIDENT"));
	m_arrAlarmCategory.insert(AlarmCategory::FAULT, QObject::tr("FAULT"));
	m_arrAlarmCategory.insert(AlarmCategory::COMMON, QObject::tr("COMMON"));


	list.clear();
	list << tr("TagName") << tr("Description") << tr("ID") << tr("Condition") << tr("Category") << tr("Value")
		<< tr("LowValue") << tr("DelayAlarm") << tr("Priority") << tr("SupportAck") << tr("SupportDelete") << tr("AckType") << tr("Log");

	ui.m_pTableAIOfflimit->setColumnCount(12);
	ui.m_pTableAIOfflimit->setHorizontalHeaderLabels(list);
	//限值
	para.desc = tr("SupportAck");
	para.tagname = "SupportAck";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	ui.m_pTableAIOfflimit->setItemDelegateForColumn(LimitSupportAck, pCheck);

	para.desc = tr("SupportDelete");
	para.tagname = "SupportDelete";
	//m_arrInfo.insert(SupportAck, para);
	m_horizontal_header_list.append(para.desc);
	ui.m_pTableAIOfflimit->setItemDelegateForColumn(LimitSupportDelete, pCheck);

	//AckType
	m_arrAlarmAckType.clear();
	m_arrAlarmAckType.insert(AlarmAckType::DELETE, QObject::tr("DELETE"));
	m_arrAlarmAckType.insert(AlarmAckType::KEEP, QObject::tr("KEEP"));
	m_arrAlarmAckType.insert(AlarmAckType::CONFORM, QObject::tr("CONFORM"));
}

