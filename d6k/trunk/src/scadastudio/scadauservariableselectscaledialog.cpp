#include "scadauservariableselectscaledialog.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "scadavirable_data.h"


#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>

CScadaUserVariableSelectScaleDialog::CScadaUserVariableSelectScaleDialog(QWidget *parent, Config::CScadaVariableConfigData *pConfig,
	QString strScaleName, Config::CScadaData *pScada)
	: QDialog(parent), m_pModel(nullptr), m_pConfig(nullptr), m_strTagName("")
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

	InitTransform(m_pModel, pScada);

	m_strTagName = strScaleName;
	DumpModel(m_pModel, m_pModel->indexFromItem(m_pModel->invisibleRootItem()), m_strTagName);
	connect(ui.treeView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(Clicked(const QModelIndex &)));
	connect(ui.pOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(ui.pCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));
}

CScadaUserVariableSelectScaleDialog::~CScadaUserVariableSelectScaleDialog()
{

}

/*! \fn 
********************************************************************************************************* 
** \brief CScadaUserVariableSelectScaleDialog::InitTransform 
** \details 
** \param pModel 
** \param pConfig 
** \return void 
** \author gw
** \date 2017年1月29日 
** \note 
********************************************************************************************************/
void CScadaUserVariableSelectScaleDialog::InitTransform(QStandardItemModel *pModel, Config::CScadaData *pScada)
{
	Q_ASSERT(pModel);
	if (!pModel)
	{
		return;
	}

	Q_ASSERT(pScada);
	if (!pScada)
	{
		return;
	}

	//转换关系
	auto strScale = tr("scale list");
	CQuoteItem *itemScale = new CQuoteItem(strScale);
	itemScale->setData(strScale, LASTVALUE);
	itemScale->setData(SCADAVARIABLE_ITEM_SCALE, Qt::UserRole);
	itemScale->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	itemScale->setData(reinterpret_cast<long long>(pScada), POINTERDATA);
	itemScale->setIcon(QIcon(SCALE_PNG));
	itemScale->setEditable(false);

	//线性转换 非线性转换
	auto pLinear = new CQuoteItem(tr("linear node"));
	pLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT, Qt::UserRole);
	pLinear->setIcon(QIcon(SCALE_PNG));
	pLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pLinear->setData(reinterpret_cast<long long>(pScada->m_pScadaLinearGroup), POINTERDATA);
	pLinear->setEditable(false);
	itemScale->appendRow(pLinear);
	auto pNonLinear = new CQuoteItem(tr("nonlinear node"));
	pNonLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT, Qt::UserRole);
	pNonLinear->setIcon(QIcon(SCALE_PNG));
	pNonLinear->setData(ROOT_GROUP, GROUP_NUMBER_ROLE);
	pNonLinear->setData(reinterpret_cast<long long>(pScada->m_pScadaNonLinearGroup), POINTERDATA);
	pNonLinear->setEditable(false);
	itemScale->appendRow(pNonLinear);

	pModel->appendRow(itemScale);

	InitTransformGroup(pLinear, pScada->m_pScadaLinearGroup);

	InitNonTransformGroup(pNonLinear, pScada->m_pScadaNonLinearGroup);

	ui.treeView->expandAll();
}

bool CScadaUserVariableSelectScaleDialog::InitTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup)
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
		newItem->setData(FES_TYPE_SCALE_LINEAR_GROUP_ITEM, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitTransformGroup(newItem, var);
	}

	auto &arrLinearItem = pGroup->GetItem();
	for each (auto var in arrLinearItem)
	{
		{
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pLinearScale->m_szTagName);
			itemLinear->setData(var->m_pLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(false);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}

bool CScadaUserVariableSelectScaleDialog::InitNonTransformGroup(CQuoteItem *pItem, Config::CScadaTransformGroup *pGroup)
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
		newItem->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP, Qt::UserRole);
		newItem->setData(nDeepths, GROUP_NUMBER_ROLE);
		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		pItem->appendRow(newItem);

		InitNonTransformGroup(newItem, var);
	}

	auto &arrNonLinearItem = pGroup->GetItem();
	for each (auto var in arrNonLinearItem)
	{
		{
			Q_ASSERT(var);
			Q_ASSERT(var->m_pNonLinearScale);
			//线性转换
			//名字
			CQuoteItem *itemLinear = new CQuoteItem(var->m_pNonLinearScale->m_szTagName);
			itemLinear->setData(var->m_pNonLinearScale->m_szTagName, LASTVALUE);
			itemLinear->setData(SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM, Qt::UserRole);
			//Tagname
			//itemLinear->setData(pFesData->m_arrTransforms[i]->m_pLinearScale->m_szTagName, TAGNAME);
			itemLinear->setData(reinterpret_cast<long long>(var), POINTERDATA);
			itemLinear->setData(var->m_pNonLinearScale->m_strDescription, TREE_TOOL_TIP_ROLE);
			itemLinear->setIcon(QIcon(SCALE_PNG));
			itemLinear->setEditable(false);

			pItem->appendRow(itemLinear);
		}
	}

	return true;
}

void CScadaUserVariableSelectScaleDialog::DumpModel(const QAbstractItemModel *pModel, const QModelIndex & top, const QString &s)
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
				(nType == SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM || nType == SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM))
			{
				ui.treeView->setCurrentIndex(Item);

				qDebug() << s;
			}

			qDebug() << Item.data(Qt::EditRole).toString();
		}

		for (int c = 0; c < pModel->columnCount(top); c++)
		{
			Item = pModel->index(r, c, top);
			DumpModel(pModel, Item, s);
		}
	}
}

void CScadaUserVariableSelectScaleDialog::Clicked(const QModelIndex &index)
{
	//int nTest = index.data(Qt::UserRole).toInt();

	if (index.data(Qt::UserRole).toInt() != SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM &&
		index.data(Qt::UserRole).toInt() != SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM)
	{
		return;
	}

	auto *pTmp = reinterpret_cast<Config::CScadaTransform *>(index.data(POINTERDATA).toLongLong());
	Q_ASSERT(pTmp);
	if (!pTmp)
	{
		return;
	}

	while (ui.tableWidget->rowCount() > 0)
	{
		ui.tableWidget->removeRow(0);
	}

	ui.tableWidget->clear();

	int nType;
	pTmp->GetType(nType);
	if (nType == LINEAR)
	{
		auto *pLinear = pTmp->GetLinear();
		Q_ASSERT(pLinear);

		QStringList list;
		list.clear();
		list << tr("ScaleType") << tr("MinRaw") << tr("MaxRaw") << tr("Min") << tr("Max") << tr("Inverse")
			<< tr("Gain") << tr("Offset");
		ui.tableWidget->setColumnCount(8);
		ui.tableWidget->setHorizontalHeaderLabels(list);

		ui.tableWidget->insertRow(ui.tableWidget->rowCount());

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, ScaleType, new QTableWidgetItem(tr("linear")));

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, MinRaw, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbMinRaw)));

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, MaxRaw, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbMaxRaw)));

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, Min, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbMinScale)));

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, Max, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbMaxScale)));

		QTableWidgetItem *pItem = new QTableWidgetItem();
		if (pLinear->m_bEnableInverse == true)
		{
			pItem->setCheckState(Qt::Checked);
		}
		else
		{
			pItem->setCheckState(Qt::Unchecked);
		}

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, Inverse, pItem);

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, Gain, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbGain)));

		ui.tableWidget->setItem(ui.tableWidget->rowCount() - 1, Offset, new QTableWidgetItem(QString("%1").arg(pLinear->m_dbOffset)));

		m_nScaleType = LINEAR;
		m_strTagName = pLinear->m_szTagName;
		m_strDesc = pLinear->m_strDescription;
	}
	else if (nType == NONLINEAR)
	{
		auto *pNonLinear = pTmp->GetNonLinear();
		Q_ASSERT(pNonLinear);

		QStringList list;
		list.clear();
		list << tr("Raw") << tr("Scale");
		ui.tableWidget->setColumnCount(2);
		ui.tableWidget->setHorizontalHeaderLabels(list);

		for (size_t i = 0; i < pNonLinear->m_arrPNonliear.size(); i++)
		{
			Config::SCADA_NONLINEAR_SCALE *pScale = pNonLinear->m_arrPNonliear[i];

			int nRows = ui.tableWidget->rowCount();

			ui.tableWidget->insertRow(nRows);

			QString strRaw = QString("%1").arg(pScale->RawValue);
			QString strScale = QString("%1").arg(pScale->ScaleValue);

			ui.tableWidget->setItem(nRows, 0, new QTableWidgetItem(strRaw));
			ui.tableWidget->setItem(nRows, 1, new QTableWidgetItem(strScale));
		}

		m_nScaleType = NONLINEAR;
		m_strTagName = pNonLinear->m_szTagName;
		m_strDesc = pNonLinear->m_strDescription;
	}
	else
	{
		QMessageBox::warning(this, tr("Scale type is invalid!!!"), tr("Scale type is invalid!!!"));
	}
}

void CScadaUserVariableSelectScaleDialog::OnOK()
{
	if (ui.tableWidget->rowCount() > 0)
	{
		accept();
	}
	else
	{
		rejected();

		this->close();
	}
}

void CScadaUserVariableSelectScaleDialog::OnCancel()
{
	rejected();

	this->close();
}
