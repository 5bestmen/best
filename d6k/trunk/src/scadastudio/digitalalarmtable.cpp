#include "digitalalarmtable.h"
#include "digitalalarmmodel.h"

CDigitalAlarmTable::CDigitalAlarmTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QTableView(parent)
{
	m_pModel = new CDigitalAlarmModel(this, pCore, pModule, pAlarmData, pItem, pFes);

	setModel(m_pModel);
}

CDigitalAlarmTable::~CDigitalAlarmTable()
{

}
