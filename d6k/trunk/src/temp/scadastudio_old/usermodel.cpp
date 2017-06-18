/*! @file
<PRE>
********************************************************************************
模块名       :  用户模块
文件名       :  usermodel.cpp
文件实现功能 :  用户模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 通道 装置
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CUserModel::CUserModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//对象节点
	m_pTopFesItem = new CQuoteItem(QIcon(":images/fes.png"), QStringLiteral("用户"));

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
** \date 2015年11月26日 
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
** \date 2015年11月26日 
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
