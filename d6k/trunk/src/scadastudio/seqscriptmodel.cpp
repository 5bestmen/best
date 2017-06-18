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
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "scadastudio/imainwindow.h"

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
CSeqscriptModel::CSeqscriptModel(IMainWindow *pUi)
	: QObject(pUi)
{
	CQuoteItem *pTop = pUi->GetTopItem();

	//顺控与脚本
	CQuoteItem *pItem = new CQuoteItem(QIcon(SEQ_SCRIPT_PNG), QStringLiteral("顺控与脚本"));

	pItem->setData(NODE_ITEM, Qt::UserRole);

	pItem->setEditable(false);

	pTop->appendRow(pItem);

	//顺控
	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("顺控"));
	pTmp->setData(SEQ_SCRIPT_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	pItem->appendRow(pTmp);

	//脚本
	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("脚本"));
	pTmp->setData(SEQ_SCRIPT_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	pItem->appendRow(pTmp);
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

/*! \fn 
********************************************************************************************************* 
** \brief CSeqscriptModel::Init 
** \details 
** \param pUi 
** \return void 
** \author gw
** \date 2015年12月9日 
** \note 
********************************************************************************************************/
void CSeqscriptModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
}