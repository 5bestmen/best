#include "forwarding_ai_view.h"
#include "forwading_ai_table.h"
#include "config_data.h"
#include "aitable.h"

CForwardingAIView::CForwardingAIView(QWidget *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes)
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

	m_pTable = new CForwadingAITable(this, pCore, pChannelData, pFes);

	setCentralWidget(m_pTable);
}

CForwardingAIView::~CForwardingAIView()
{

}

void CForwardingAIView::Refresh()
{
	m_pTable->Refresh();
}

void CForwardingAIView::Delete()
{
	m_pTable->setModel(nullptr);
}
