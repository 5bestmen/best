/*! @file
<PRE>
********************************************************************************
ģ����       :  �û�ģ��
�ļ���       :  usermodel.cpp
�ļ�ʵ�ֹ��� :  �û�ģ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> ͨ�� װ��
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

#include "usermodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"
#include "user.h"

/*! \fn CUserModel::CUserModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CUserModel::CUserModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUserModel::CUserModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("�û�"));

	m_pTopFesItem->setData(USER_ITEM, Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);
}

/*! \fn CUserModel::~CUserModel()
********************************************************************************************************* 
** \brief CUserModel::~CUserModel 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CUserModel::~CUserModel()
{

}

/*! \fn void CUserModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CUserModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CUserModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == USER_ITEM || nType == USER_GROUP_CHILD_ITEM)
	{
		CUser *pUser = new CUser(this, m_pUi, indexSelect, nMouseClickType);

		pUser->ShowMenu();

		pUser->deleteLater();
	}
}
