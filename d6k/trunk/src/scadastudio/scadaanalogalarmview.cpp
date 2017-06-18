#include "scadaanalogalarmview.h"
#include "scadaanalogalarmtable.h"
#include "scadaanalogalarmtable.h"
#include "scadaanalogalarmmodel.h"

CScadaAnalogAlarmView::CScadaAnalogAlarmView(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData)
	: QMainWindow(parent)
{
	m_pTable = new CScadaAnalogAlarmTable(this, pCore, pAlarmData, pItem, pConfig, pData);

	setCentralWidget(m_pTable);
}

CScadaAnalogAlarmView::~CScadaAnalogAlarmView()
{

}

void CScadaAnalogAlarmView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CScadaAnalogAlarmView::Delete()
{
	m_pTable->setModel(nullptr);
}
