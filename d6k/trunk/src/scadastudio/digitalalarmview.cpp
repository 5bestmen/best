/*! @file
<PRE>
********************************************************************************
模块名       :	开关量告警view
文件名       :  digitalalarmview.cpp
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
