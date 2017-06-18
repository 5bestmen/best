#include "forwarding_kwhview.h"
#include "forwarding_kwhtable.h"
#include "config_data.h"


CForwardingKWHView::CForwardingKWHView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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

	m_pTable = new CForwardingKWHTable(this, pCore, pChannelData, pFes);

	setCentralWidget(m_pTable);
}

CForwardingKWHView::~CForwardingKWHView()
{

}

void CForwardingKWHView::Refresh()
{
	m_pTable->refresh();
}

void CForwardingKWHView::Delete()
{
	m_pTable->setModel(nullptr);
}
