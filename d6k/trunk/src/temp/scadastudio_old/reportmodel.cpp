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
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"

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
CReportModel::CReportModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//对象节点
	m_pTopFesItem = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("报表"));

	m_pTopFesItem->setData(REPORT_ITEM, Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);


	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("日报表"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);
	
	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("月报表"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("年报表"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("特殊报表"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("时段统计报表"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);
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

/*! \fn void CReportModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CReportModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CReportModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == GRAPH_ITEM || nType == GRAPH_GROUP_CHILD_ITEM)
	{
		/*CGraph *pGraph = new CGraph(this, m_pUi, indexSelect, nMouseClickType);

		pGraph->ShowMenu();

		pGraph->deleteLater();*/
	}
}
