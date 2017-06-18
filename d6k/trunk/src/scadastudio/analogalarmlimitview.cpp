#include "analogalarmlimitview.h"
#include "analogalarmlimittable.h"
#include "analogalarmlimitmodel.h"

CAnalogAlarmLimitView::CAnalogAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	m_pTable = new CAnalogAlarmLimitTable(this, pCore, pModule, pAlarmData, pItem, pFes);

	setCentralWidget(m_pTable);
}

CAnalogAlarmLimitView::~CAnalogAlarmLimitView()
{

}

void CAnalogAlarmLimitView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CAnalogAlarmLimitView::Delete()
{
	m_pTable->setModel(nullptr);
}
