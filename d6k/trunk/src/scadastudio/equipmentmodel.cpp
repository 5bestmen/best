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
** \date 2015��11��26�� 
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
** \date 2015��11��26�� 
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
** \date 2015��12��8�� 
** \note 
********************************************************************************************************/
void CEquipmentModel::Init(IMainWindow *pUi)
{
	Q_UNUSED(pUi);
	
	//CQuoteItem *pTop = pUi->GetTopItem();

	////����ڵ�
	//CQuoteItem *pItem = new CQuoteItem(QIcon(EQUIPMENT_PNG), QStringLiteral("������̬"));

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
** \date 2015��12��8�� 
** \note 
********************************************************************************************************/
void CEquipmentModel::UnInit()
{

}
