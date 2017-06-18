/*! @file
<PRE>
********************************************************************************
模块名       :  设备模块
文件名       :  equipmentmodel.cpp
文件实现功能 :  设备模块
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CEquipmentModel::CEquipmentModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//对象节点
	m_pTopFesItem = new CQuoteItem(QIcon(EQUIPMENT_PNG), QStringLiteral("对象组态"));

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
** \date 2015年11月26日 
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
** \date 2015年11月26日 
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
