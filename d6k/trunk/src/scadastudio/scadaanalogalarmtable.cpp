#include "scadaanalogalarmtable.h"
#include "scadaanalogalarmmodel.h"

CScadaAnalogAlarmTable::CScadaAnalogAlarmTable(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData)
	: QTableView(parent), m_pModel(nullptr)
{
	m_pModel = new CScadaAnalogAlarmModel(this, pCore, pAlarmData, pItem, pConfig, pData);
	setModel(m_pModel);
}

CScadaAnalogAlarmTable::~CScadaAnalogAlarmTable()
{

}
