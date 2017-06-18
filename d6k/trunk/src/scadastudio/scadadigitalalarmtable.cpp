#include "scadadigitalalarmtable.h"
#include "scadadigitalalarmmodel.h"

CScadaDigitalAlarmTable::CScadaDigitalAlarmTable(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData)
	: QTableView(parent)
{
	m_pModel = new CScadaDigitalAlarmModel(this, pCore, pAlarmData, pItem, pConfig, pData);
	setModel(m_pModel);
}

CScadaDigitalAlarmTable::~CScadaDigitalAlarmTable()
{

}

void CScadaDigitalAlarmTable::Refresh()
{
	m_pModel->RefrushModel();
}
