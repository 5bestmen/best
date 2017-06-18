#include "systemvariabletable.h"
#include "sortfiltermodel.h"
#include "systemvariablemodel.h"

CSystemVariableTable::CSystemVariableTable(QWidget *parent, IMainModuleInterface *pCore, Config::CFesData *pFesData)
	: QTableView(parent), m_pSortModel(nullptr), m_pModel(nullptr)
{
	m_pSortModel = new CSortFilterModel(this);
	m_pModel = new CSystemVariableModel(this, pCore, pFesData);

	m_pSortModel->setSourceModel(m_pModel);
	setModel(m_pSortModel);
}

CSystemVariableTable::~CSystemVariableTable()
{

}
