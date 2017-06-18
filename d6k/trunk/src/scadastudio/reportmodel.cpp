/*! @file
<PRE>
********************************************************************************
模块名       :  报表
文件名       :  reportmodel.cpp
文件实现功能 :  报表
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
*  @date    2015.11.17
*/

#include "reportmodel.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/imainwindow.h"

/*! \fn CReportModel::CReportModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CReportModel::CReportModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CReportModel::CReportModel(IMainWindow *pUi)
	: QObject(pUi)
{

}

/*! \fn CReportModel::~CReportModel()
********************************************************************************************************* 
** \brief CReportModel::~CReportModel 
** \details 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CReportModel::~CReportModel()
{

}

/*! \fn 
********************************************************************************************************* 
** \brief CReportModel::Init 
** \details 
** \param pUi 
** \return void 
** \author gw
** \date 2015年12月9日 
** \note 
********************************************************************************************************/
void CReportModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
	
	//CQuoteItem *pTop = pUi->GetTopItem();

	////对象节点
	//CQuoteItem *pItem = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("报表"));

	//pItem->setData(REPORT_ITEM, Qt::UserRole);

	//pItem->setEditable(false);

	//pTop->appendRow(pItem);


	//CQuoteItem *pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("日报表"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("月报表"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("年报表"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("特殊报表"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("时段统计报表"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);
}

/*! \fn 
********************************************************************************************************* 
** \brief CReportModel::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015年12月9日 
** \note 
********************************************************************************************************/
void CReportModel::showMouseRightButton(const QPoint& point)
{
	Q_UNUSED(point);
}
