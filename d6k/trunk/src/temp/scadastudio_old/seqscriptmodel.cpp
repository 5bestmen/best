/*! @file
<PRE>
********************************************************************************
ģ����       :  ˳����ű�
�ļ���       :  seqscriptmodel.cpp
�ļ�ʵ�ֹ��� :  ˳����ű�
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
*  @date    2015.11.23
*/

#include "seqscriptmodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"

/*! \fn CSeqscriptModel::CSeqscriptModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CSeqscriptModel::CSeqscriptModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CSeqscriptModel::CSeqscriptModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//˳����ű�
	m_pTopFesItem = new CQuoteItem(QIcon(SEQ_SCRIPT_PNG), QStringLiteral("˳����ű�"));

	m_pTopFesItem->setData(NODE_ITEM ,Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);

	//˳��
	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("˳��"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	//�ű�
	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�ű�"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);
}

/*! \fn CSeqscriptModel::~CSeqscriptModel()
********************************************************************************************************* 
** \brief CSeqscriptModel::~CSeqscriptModel 
** \details 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CSeqscriptModel::~CSeqscriptModel()
{

}

/*! \fn void CSeqscriptModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CNodeParaModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
void CSeqscriptModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == SEQ_SCRIPT_ITEM)
	{
		
	}
}
