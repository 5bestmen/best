/*! @file
<PRE>
********************************************************************************
ģ����       :  �豸ģ��
�ļ���       :  equipmentmodel.cpp
�ļ�ʵ�ֹ��� :  �豸ģ��
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
*  @date    2015.11.26
*/

#include "equipmentmodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"
#include "equip.h"

/*! \fn CEquipmentModel::CEquipmentModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CEquipmentModel::CEquipmentModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CEquipmentModel::CEquipmentModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(EQUIPMENT_PNG), QStringLiteral("������̬"));

	m_pTopFesItem->setData(EQUIPMENT_ITEM ,Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);
}

/*! \fn CEquipmentModel::~CEquipmentModel()
********************************************************************************************************* 
** \brief CEquipmentModel::~CEquipmentModel 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CEquipmentModel::~CEquipmentModel()
{

}

/*! \fn void CEquipmentModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CEquipmentModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CEquipmentModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == EQUIPMENT_ITEM || nType == EQUIPMENT_GROUP_CHILD_ITEM)
	{
		CEquip *pEquip = new CEquip(this, m_pUi, indexSelect, nMouseClickType);

		pEquip->ShowMenu();

		pEquip->deleteLater();
	}
}
