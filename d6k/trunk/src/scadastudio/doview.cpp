#include "doview.h"
#include "dotable.h"
#include "config_data.h"

CDOView::CDOView(QWidget *parent, IMainModuleInterface *pCore, Config::CDevice *pDeviceData, QString &strTag,
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

	m_pTable = new CDOTable(this, pDeviceData->m_arrDOs, pCore, pModule, pFes);

	setCentralWidget(m_pTable);
}

CDOView::~CDOView()
{

}

void CDOView::Delete()
{
	m_pTable->setModel(nullptr);
}