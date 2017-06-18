/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ����
�ļ���       :  sysmodel.cpp
�ļ�ʵ�ֹ��� :  ϵͳ����
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

#include "sysmodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"

/*! \fn CSysModel::CSysModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CSysModel::CSysModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CSysModel::CSysModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(SYSTEM_PNG), QStringLiteral("ϵͳ����"));

	m_pTopFesItem->setData(REPORT_ITEM, Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);
}

/*! \fn CSysModel::~CSysModel()
********************************************************************************************************* 
** \brief CSysModel::~CSysModel 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CSysModel::~CSysModel()
{

}

/*! \fn void CSysModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CSysModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CSysModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{

}