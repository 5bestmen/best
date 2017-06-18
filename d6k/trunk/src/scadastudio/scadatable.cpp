
#include "scadatable.h"
#include "sortfiltermodel.h"
#include "scadamodel.h"

CScadaTable::CScadaTable(QWidget *parent, Config::CScadaData *pData, CQuoteItem *pItem, int nType, IMainModuleInterface *pCore
	, Config::CScadaVariableConfigData *pConfig)
	: QTableView(parent)
{
	Q_UNUSED(nType);
	
	//setObjectName(strObjectType);

	this->setAlternatingRowColors(true);

	m_pSortModel = new CSortFilterModel(this);

	m_pModel = new CScadaModel(this, pData, pItem, pCore, pConfig);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);
}

CScadaTable::~CScadaTable()
{

}

void CScadaTable::Refresh()
{
	m_pModel->RefrushModel();
}
