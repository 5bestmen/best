/*! @file
<PRE>
********************************************************************************
模块名       :  系统参数
文件名       :  sysmodel.cpp
文件实现功能 :  系统参数
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CSysModel::CSysModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//对象节点
	m_pTopFesItem = new CQuoteItem(QIcon(SYSTEM_PNG), QStringLiteral("系统参数"));

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
** \date 2015年11月26日 
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CSysModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{

}