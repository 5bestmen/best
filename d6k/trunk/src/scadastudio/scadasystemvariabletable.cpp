#include "scadasystemvariabletable.h"
#include "sortfiltermodel.h"
#include "scadasystemvariablemodel.h"

CScadaSystemVariableTable::CScadaSystemVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CScadaVariableConfigData *pConfigData
	, Config::CScadaData *pData)
	: QTableView(parent), m_pSortModel(nullptr), m_pModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pModel = new CScadaSystemVariableModel(this, pCore, pConfigData, pData);

	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);

	setSortingEnabled(true);

	sortByColumn(0, Qt::AscendingOrder);
}

CScadaSystemVariableTable::~CScadaSystemVariableTable()
{
	if (m_pSortModel)
	{
		m_pSortModel->deleteLater();
	}

	if (m_pModel)
	{
		m_pModel->deleteLater();
	}
}
