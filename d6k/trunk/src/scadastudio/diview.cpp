#include "diview.h"
#include "ditable.h"
#include "config_data.h"

CDIView::CDIView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
	CQuoteItem *pItem, CChannel *pModule, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	Q_UNUSED(strTag);
	
	Q_ASSERT(pCore);
	Q_ASSERT(pDeviceData);
	Q_ASSERT(pItem);
	Q_ASSERT(pModule);

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

	m_pTable = new CDITable(this, pDeviceData->m_arrDIs, pCore, pModule, pFes);
	setCentralWidget(m_pTable);
}

CDIView::~CDIView()
{

}

void CDIView::Delete()
{
	m_pTable->setModel(nullptr);
}
