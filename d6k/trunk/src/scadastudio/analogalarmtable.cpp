#include "analogalarmtable.h"
#include "sortfiltermodel.h"
#include "analogalarmmodel.h"

CAnalogAlarmTable::CAnalogAlarmTable(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QTableView(parent)
{
	auto pSortModel = new CSortFilterModel(this);
	pSortModel->clear();

	m_pModel = new CAnalogAlarmModel(this, pCore, pModule, pAlarmData, pItem, pFes);

	pSortModel->setSourceModel(m_pModel);

	setModel(pSortModel);
}

CAnalogAlarmTable::~CAnalogAlarmTable()
{

}

/*! \fn void CAnalogAlarmTable::Refresh()
********************************************************************************************************* 
** \brief CAnalogAlarmTable::Refresh 
** \details 
** \return void 
** \author gw
** \date 2017Äê2ÔÂ7ÈÕ 
** \note 
********************************************************************************************************/
void CAnalogAlarmTable::Refresh()
{
	m_pModel->RefrushModel();
}
