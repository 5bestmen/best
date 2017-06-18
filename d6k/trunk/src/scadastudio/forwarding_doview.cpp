#include "forwarding_doview.h"
#include "forwarding_dotable.h"
#include "config_data.h"

CForwardingDOView::CForwardingDOView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
	: QMainWindow(parent)
{	
	Q_ASSERT(pCore);
	Q_ASSERT(pChannelData);
	Q_ASSERT(pFes);

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

	m_pTable = new CForwardingDOTable(this, pCore, pChannelData, pFes);

	setCentralWidget(m_pTable);
}

CForwardingDOView::~CForwardingDOView()
{

}

void CForwardingDOView::Refresh()
{
	m_pTable->refresh();
}

void CForwardingDOView::Delete()
{
	m_pTable->setModel(nullptr);
}