/*! @file
<PRE>
********************************************************************************
ģ����       :  ����
�ļ���       :  reportmodel.cpp
�ļ�ʵ�ֹ��� :  ����
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
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CReportModel::CReportModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("����"));

	m_pTopFesItem->setData(REPORT_ITEM, Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);


	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�ձ���"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);
	
	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�±���"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�걨��"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("���ⱨ��"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("ʱ��ͳ�Ʊ���"));
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
** \date 2015��11��26�� 
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
** \date 2015��11��26�� 
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
