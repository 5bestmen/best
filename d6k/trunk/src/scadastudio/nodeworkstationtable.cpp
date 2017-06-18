#include "nodeworkstationtable.h"
#include "nodeworkstationmodel.h"
#include "confignodeservicedialog.h"

CNodeWorkstationTable::CNodeWorkstationTable(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pWorkstationData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QTableView(parent), m_pWorkstationData(nullptr), m_pNodeConfig(nullptr)
{
	Q_ASSERT(pWorkstationData);
	if (!pWorkstationData)
	{
		return;
	}
	m_pWorkstationData = pWorkstationData;
	
	Q_ASSERT(pNodeConfig);
	if (!pNodeConfig)
	{
		return;
	}
	m_pNodeConfig = pNodeConfig;

	m_pModel = new CNodeWorkstationModel(this, pCore, pWorkstationData, pItem, pNodeConfig);

	setModel(m_pModel);

	connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(DoubleClicked(const QModelIndex &)));
}

CNodeWorkstationTable::~CNodeWorkstationTable()
{

}

void CNodeWorkstationTable::Refresh()
{
	m_pModel->RefrushModel();
}

void CNodeWorkstationTable::DoubleClicked(const QModelIndex & index)
{
	if (!index.isValid())
	{
		return;
	}

	int nCol = index.column();
	if (nCol == CNodeWorkstationModel::COLUMN::Service)
	{
		auto pNode = m_pWorkstationData;

		CConfigNodeServiceDialog dialog(nullptr, m_pNodeConfig, pNode);
		dialog.exec();
	}
}
