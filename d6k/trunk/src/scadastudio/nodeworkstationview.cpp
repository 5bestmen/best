#include "nodeworkstationview.h"
#include "nodeworkstationtable.h"

CNodeWorkstationView::CNodeWorkstationView(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pWorkstationData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CNodeWorkstationTable(this, pCore, pWorkstationData, pItem, pNodeConfig);

	setCentralWidget(m_pTable);
}

CNodeWorkstationView::~CNodeWorkstationView()
{

}

void CNodeWorkstationView::Refresh()
{
	m_pTable->Refresh();
}

void CNodeWorkstationView::Delete()
{
	m_pTable->setModel(nullptr);
}