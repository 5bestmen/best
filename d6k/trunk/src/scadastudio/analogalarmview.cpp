/*! @file
<PRE>
********************************************************************************
ģ����       :  ģ�����澯view
�ļ���       :  analogalarmview.cpp
�ļ�ʵ�ֹ��� :  
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \date 2016��11��10�� 
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