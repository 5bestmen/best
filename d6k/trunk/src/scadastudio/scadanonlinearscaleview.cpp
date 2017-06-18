#include "scadanonlinearscaleview.h"
#include "scadanonlinearscaletalbe.h"

CScadaNonlinearScaleView::CScadaNonlinearScaleView(QWidget *parent, Config::CScadaTransform *pTransform, CQuoteItem *pItem, CScadaVariableModule *pModule
	, Config::CScadaVariableConfigData *pConfig)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CScadaNonlinearScaleTable(this, pTransform, pItem, pModule, pConfig);
	
	setCentralWidget(m_pTable);
}

CScadaNonlinearScaleView::~CScadaNonlinearScaleView()
{

}

void CScadaNonlinearScaleView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CScadaNonlinearScaleView::Save()
{

}

void CScadaNonlinearScaleView::Refresh()
{
	m_pTable->Refresh();
}
