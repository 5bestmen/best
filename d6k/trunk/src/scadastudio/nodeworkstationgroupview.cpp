#include "nodeworkstationgroupview.h"
#include "nodeworkstationgrouptable.h"

CNodeWorkstationGroupView::CNodeWorkstationGroupView(QWidget *parent, IMainModuleInterface *pCore, Config::CNodeGroup *pWorkstationData
	, Config::CNodeConfigData *pNodeConfig)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CNodeWorkstationGroupTable(this, pCore, pWorkstationData, pNodeConfig);

	setCentralWidget(m_pTable);
}

CNodeWorkstationGroupView::~CNodeWorkstationGroupView()
{

}

void CNodeWorkstationGroupView::Delete()
{
	m_pTable->setModel(nullptr);
}