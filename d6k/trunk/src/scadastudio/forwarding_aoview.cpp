#include "forwarding_aoview.h"
#include "forwarding_aotable.h"
#include "config_data.h"


CForwardingAOView::CForwardingAOView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
	: QMainWindow(parent)
{	
	Q_ASSERT(pCore);

	if (!pCore)
	{
		return;
	}

	if (!pChannelData)
	{
		return;
	}

	if (!pFes)
	{
		return;
	}

	m_pTable = new CForwardingAOTable(this, pCore, pChannelData, pFes);

	setCentralWidget(m_pTable);
}

CForwardingAOView::~CForwardingAOView()
{

}

void CForwardingAOView::Refresh()
{
	m_pTable->Refresh();
}

void CForwardingAOView::Delete()
{
	m_pTable->setModel(nullptr);
}
