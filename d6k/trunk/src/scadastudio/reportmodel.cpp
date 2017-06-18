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
** \date 2015��11��26�� 
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
** \date 2015��11��26�� 
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
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void CReportModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
	
	//CQuoteItem *pTop = pUi->GetTopItem();

	////����ڵ�
	//CQuoteItem *pItem = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("����"));

	//pItem->setData(REPORT_ITEM, Qt::UserRole);

	//pItem->setEditable(false);

	//pTop->appendRow(pItem);


	//CQuoteItem *pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("�ձ���"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("�±���"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("�걨��"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("���ⱨ��"));
	//pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	//pTmp->setEditable(false);

	//pItem->appendRow(pTmp);

	//pTmp = new CQuoteItem(QIcon(REPORT_PNG), QStringLiteral("ʱ��ͳ�Ʊ���"));
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
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void CReportModel::showMouseRightButton(const QPoint& point)
{
	Q_UNUSED(point);
}
