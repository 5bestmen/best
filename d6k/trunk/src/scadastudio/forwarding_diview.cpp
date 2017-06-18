#include "forwarding_diview.h"
#include "forwarding_ditable.h"
#include "config_data.h"

CForwardingDIView::CForwardingDIView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData,
	Config::CFesData *pFes)
	: QMainWindow(parent)
{	
	Q_ASSERT(pCore);
	Q_ASSERT(pChannelData);

	if (!pCore)
	{
		return;
	}

	if (!pChannelData)
	{
		return;
	}

	m_pTable = new CForwardingDITable(this, pCore, pChannelData, pFes);
	setCentralWidget(m_pTable);
}

CForwardingDIView::~CForwardingDIView()
{

}

void CForwardingDIView::Refresh()
{
	m_pTable->Refresh();
}

void CForwardingDIView::Delete()
{
	m_pTable->setModel(nullptr);
}
