#include "forwarding_cgview.h"
#include "forwarding_cgtable.h"
#include "config_data.h"


CForwardingCGView::CForwardingCGView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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

	m_pTable = new CForwardingCGTable(this, pCore, pChannelData, pFes);

	setCentralWidget(m_pTable);
}

CForwardingCGView::~CForwardingCGView()
{

}

void CForwardingCGView::Refresh()
{
	m_pTable->refresh();
}

void CForwardingCGView::Delete()
{
	m_pTable->setModel(nullptr);
}
