/*! @file
<PRE>
********************************************************************************
模块名       :  顺控与脚本
文件名       :  seqscriptmodel.cpp
文件实现功能 :  顺控与脚本
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
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CSeqscriptModel::CSeqscriptModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//顺控与脚本
	m_pTopFesItem = new CQuoteItem(QIcon(SEQ_SCRIPT_PNG), QStringLiteral("顺控与脚本"));

	m_pTopFesItem->setData(NODE_ITEM ,Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);

	//顺控
	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("顺控"));
	pTmp->setData(REPORT_CHILD_DAY_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	m_pTopFesItem->appendRow(pTmp);

	//脚本
	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("脚本"));
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
** \date 2015年11月23日 
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
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
void CSeqscriptModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == SEQ_SCRIPT_ITEM)
	{
		
	}
}
