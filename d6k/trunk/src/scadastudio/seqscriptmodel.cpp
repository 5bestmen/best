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
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CSeqscriptModel::CSeqscriptModel(IMainWindow *pUi)
	: QObject(pUi)
{
	CQuoteItem *pTop = pUi->GetTopItem();

	//˳����ű�
	CQuoteItem *pItem = new CQuoteItem(QIcon(SEQ_SCRIPT_PNG), QStringLiteral("˳����ű�"));

	pItem->setData(NODE_ITEM, Qt::UserRole);

	pItem->setEditable(false);

	pTop->appendRow(pItem);

	//˳��
	CQuoteItem *pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("˳��"));
	pTmp->setData(SEQ_SCRIPT_ITEM, Qt::UserRole);
	pTmp->setEditable(false);

	pItem->appendRow(pTmp);

	//�ű�
	pTmp = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�ű�"));
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
** \date 2015��11��23�� 
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
** \date 2015��12��9�� 
** \note 
********************************************************************************************************/
void CSeqscriptModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
}