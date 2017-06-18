/*! @file
<PRE>
********************************************************************************
ģ����       :  �ڵ����ģ��
�ļ���       :  nodeparamodel.cpp
�ļ�ʵ�ֹ��� :  �ڵ����ģ��
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

#include "nodeparamodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"

/*! \fn CNodeParaModel::CNodeParaModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CNodeParaModel::CNodeParaModel 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CNodeParaModel::CNodeParaModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(NODE_PNG), QStringLiteral("�ڵ����"));

	m_pTopFesItem->setData(NODE_ITEM ,Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);
}

/*! \fn CNodeParaModel::~CNodeParaModel()
********************************************************************************************************* 
** \brief CNodeParaModel::~CNodeParaModel 
** \details 
** \return  
** \author gw
** \date 2015��11��23�� 
** \note 
********************************************************************************************************/
CNodeParaModel::~CNodeParaModel()
{

}

/*! \fn void CNodeParaModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
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
void CNodeParaModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == NODE_ITEM)
	{
		//CEquip *pEquip = new CEquip(this, m_pUi, indexSelect, nMouseClickType);

		//pEquip->ShowMenu();

		//pEquip->deleteLater();
	}
}