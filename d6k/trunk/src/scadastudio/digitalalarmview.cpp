/*! @file
<PRE>
********************************************************************************
ģ����       :	�������澯view
�ļ���       :  digitalalarmview.cpp
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

#include "digitalalarmview.h"
#include "digitalalarmtable.h"
#include "digitalalarmmodel.h"

CDigitalAlarmView::CDigitalAlarmView(QWidget *parent, IMainModuleInterface *pCore, CAlarm *pModule,
	Config::CDigitalAlarm *pAlarmData, QStandardItem *pItem, Config::CFesData *pFes)
	: QMainWindow(parent)
{
	Q_ASSERT(pCore);

	if (!pCore)
	{
		return;
	}

	m_pTable = new CDigitalAlarmTable(this, pCore, pModule, pAlarmData, pItem, pFes);

	setCentralWidget(m_pTable);
}

CDigitalAlarmView::~CDigitalAlarmView()
{

}

void CDigitalAlarmView::Refresh()
{
	m_pTable->GetModel()->RefrushModel();
}

void CDigitalAlarmView::Delete()
{
	m_pTable->setModel(nullptr);
}
