#include "scadaanalogalarmlimitview.h"
#include "scadaanalogalarmlimittable.h"

CScadaAnalogAlarmLimitView::CScadaAnalogAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QMainWindow(parent)
{
	m_pTable = new CScadaAnalogAlarmLimitTable(this, pCore, pAlarmData, pItem, pConfig, pScada);

	setCentralWidget(m_pTable);
}

CScadaAnalogAlarmLimitView::~CScadaAnalogAlarmLimitView()
{

}

void CScadaAnalogAlarmLimitView::Refresh()
{
	m_pTable->Refresh();
}

void CScadaAnalogAlarmLimitView::Delete()
{
	m_pTable->setModel(nullptr);
}
