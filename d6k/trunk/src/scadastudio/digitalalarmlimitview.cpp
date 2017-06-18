#include "digitalalarmlimitview.h"
#include "digitalalarmlimittable.h"
#include "digitalalarmlimitmodel.h"

CDigitalAlarmLimitView::CDigitalAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	m_pTable = new CDigitalAlarmLimitTable(this, pCore, pModule, pAlarmData, pItem, pFes);

	setCentralWidget(m_pTable);
}

CDigitalAlarmLimitView::~CDigitalAlarmLimitView()
{

}

void CDigitalAlarmLimitView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CDigitalAlarmLimitView::Delete()
{
	m_pTable->setModel(nullptr);
}
