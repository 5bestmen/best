#include "nodefesview.h"
#include "nodefestable.h"
#include "nodefesmodel.h"

CNodeFesView::CNodeFesView(QWidget *parent, IMainModuleInterface *pCore, Config::CNode *pData
	, CQuoteItem *pItem, Config::CNodeConfigData *pNodeConfig)
	: QMainWindow(parent)
{
	m_pTable = new CNodeFesTable(this, pCore, pData, pItem, pNodeConfig);

	setCentralWidget(m_pTable);
}

CNodeFesView::~CNodeFesView()
{

}

void CNodeFesView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CNodeFesView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}