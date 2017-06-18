/*! @file
<PRE>
********************************************************************************
模块名       :  模拟量告警view
文件名       :  analogalarmview.cpp
文件实现功能 :  
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2016.9.20
*/

#include "analogalarmview.h"
#include "analogalarmtable.h"

CAnalogAlarmView::CAnalogAlarmView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CAnalogAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	Q_ASSERT(pCore);

	if (!pCore)
	{
		return;
	}

	m_pTable = new CAnalogAlarmTable(this, pCore, pModule, pAlarmData, pItem, pFes);

	setCentralWidget(m_pTable);
}

CAnalogAlarmView::~CAnalogAlarmView()
{

}

/*! \fn void CAnalogAlarmView::Delete()
********************************************************************************************************* 
** \brief CAnalogAlarmView::Delete 
** \details 
** \return void 
** \author gw
** \date 2016年11月10日 
** \note 
********************************************************************************************************/
void CAnalogAlarmView::Delete()
{
	m_pTable->setModel(nullptr);
}

void CAnalogAlarmView::Refresh()
{
	m_pTable->Refresh();
}