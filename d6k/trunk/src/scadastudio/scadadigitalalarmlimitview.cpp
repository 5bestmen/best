#include "scadadigitalalarmlimitview.h"
#include "scadadigitalalarmlimittable.h"

CScadaDigitalAlarmLimitView::CScadaDigitalAlarmLimitView(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pScada)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CScadaDigitalAlarmLimitTable(this, pCore, pAlarmData, pItem, pConfig, pScada);

	setCentralWidget(m_pTable);
}

CScadaDigitalAlarmLimitView::~CScadaDigitalAlarmLimitView()
{

}

/*! \fn void CScadaDigitalAlarmLimitView::Refresh()
********************************************************************************************************* 
** \brief CScadaDigitalAlarmLimitView::Refresh 
** \details 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmLimitView::Refresh()
{
	m_pTable->Refresh();
}

/*! \fn void CScadaDigitalAlarmLimitView::Delete()
********************************************************************************************************* 
** \brief CScadaDigitalAlarmLimitView::Delete 
** \details 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmLimitView::Delete()
{
	m_pTable->setModel(nullptr);
}
