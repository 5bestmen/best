#include "aiview.h"
#include "config_data.h"
#include "aitable.h"

CAIView::CAIView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
	CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	Q_UNUSED(strTag);
	
	Q_ASSERT(pCore);
	Q_ASSERT(pDeviceData);
	Q_ASSERT(pItem);
	Q_ASSERT(pModule);

	if (!pCore)
	{
		return;
	}

	if (!pDeviceData)
	{
		return;
	}

	if (!pItem)
	{
		return;
	}

	if (!pModule)
	{
		return;
	}

	m_pTable = new CAITable(this, pDeviceData->m_arrAIs, pCore, pModule, pFes);

	setCentralWidget(m_pTable);
}

CAIView::~CAIView()
{

}

void CAIView::Refresh()
{
	m_pTable->Refresh();
}

void CAIView::Delete()
{
	m_pTable->setModel(nullptr);
}
