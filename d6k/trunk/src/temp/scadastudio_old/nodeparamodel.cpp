/*! @file
<PRE>
********************************************************************************
模块名       :  节点参数模块
文件名       :  nodeparamodel.cpp
文件实现功能 :  节点参数模块
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
** \date 2015年11月23日 
** \note 
********************************************************************************************************/
CNodeParaModel::CNodeParaModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//对象节点
	m_pTopFesItem = new CQuoteItem(QIcon(NODE_PNG), QStringLiteral("节点参数"));

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
** \date 2015年11月23日 
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
** \date 2015年11月23日 
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