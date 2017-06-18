#include "nodefesgroupview.h"
#include "nodefesgrouptable.h"
#include "nodefesgroupmodel.h"

CNodeFesGroupView::CNodeFesGroupView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pFesGroupData
	, Config::CNodeConfigData *pNodeConfig)
	: QMainWindow(parent)
{
	m_pTable = new CNodeFesGroupTable(this, pCore, pFesGroupData, pNodeConfig);

	setCentralWidget(m_pTable);
}

CNodeFesGroupView::~CNodeFesGroupView()
{

}

void CNodeFesGroupView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CNodeFesGroupView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}