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
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
//#include "equip.h"
#include "scadastudio/imainwindow.h"

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
CEquipmentModel::CEquipmentModel(IMainWindow *pUi)
	:QObject(pUi)
{

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

/*! \fn void CEquipmentModel::Init(ICoreui *pUi)
********************************************************************************************************* 
** \brief CEquipmentModel::Init 
** \details 
** \param pUi 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CEquipmentModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
	
	//CQuoteItem *pTop = pUi->GetTopItem();

	////对象节点
	//CQuoteItem *pItem = new CQuoteItem(QIcon(EQUIPMENT_PNG), QStringLiteral("对象组态"));

	//pItem->setData(EQUIPMENT_ITEM, Qt::UserRole);

	//pItem->setEditable(false);

	//pTop->appendRow(pItem);
}

/*! \fn void CEquipmentModel::UnInit()
********************************************************************************************************* 
** \brief CEquipmentModel::UnInit 
** \details 
** \return void 
** \author gw
** \date 2015年12月8日 
** \note 
********************************************************************************************************/
void CEquipmentModel::UnInit()
{

}
