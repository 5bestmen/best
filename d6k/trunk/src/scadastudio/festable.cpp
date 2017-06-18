
#include "festable.h"
#include "sortfiltermodel.h"
#include "fesdatamodel.h"

CFesTable::CFesTable(QWidget *parent, Config::CFesData *pFesData, CQuoteItem *pItem,int nType, IMainModuleInterface *pCore
	, Config::CFesConfigData *pFesConfig)
	: QTableView(parent)
{
	Q_UNUSED(nType);
	
	//setObjectName(strObjectType);

	this->setAlternatingRowColors(true);

	m_pSortModel = new CSortFilterModel(this);

	m_pModel = new CFesDataModel(this, pFesData, pItem, pCore, pFesConfig);

	m_pSortModel->setSourceModel(m_pModel);

	setModel(m_pSortModel);
}

CFesTable::~CFesTable()
{

}
