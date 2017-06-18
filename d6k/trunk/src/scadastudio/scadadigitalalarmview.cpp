#include "scadadigitalalarmview.h"
#include "scadadigitalalarmtable.h"

CScadaDigitalAlarmView::CScadaDigitalAlarmView(QWidget *parent, IMainModuleInterface *pCore,
	Config::CScadaDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CScadaVariableConfigData *pConfig, Config::CScadaData *pData)
	: QMainWindow(parent), m_pTable(nullptr)
{
	m_pTable = new CScadaDigitalAlarmTable(this, pCore, pAlarmData, pItem, pConfig, pData);

	setCentralWidget(m_pTable);
}

CScadaDigitalAlarmView::~CScadaDigitalAlarmView()
{

}

/*! \fn void CScadaDigitalAlarmView::Refresh()
********************************************************************************************************* 
** \brief CScadaDigitalAlarmView::Refresh 
** \details 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmView::Refresh()
{
	m_pTable->Refresh();
}

/*! \fn void CScadaDigitalAlarmView::Delete()
********************************************************************************************************* 
** \brief CScadaDigitalAlarmView::Delete 
** \details 
** \return void 
** \author gw
** \date 2017年1月12日 
** \note 
********************************************************************************************************/
void CScadaDigitalAlarmView::Delete()
{
	m_pTable->setModel(nullptr);
}
