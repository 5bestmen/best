/*! @file
<PRE>
********************************************************************************
ģ����       :  ��ͼģ��
�ļ���       :  hissave.cpp
�ļ�ʵ�ֹ��� :  ��ͼģ��
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

#include "graphmodel.h"
#include "scadastudio.h"
#include "quoteItem.h"
#include "define.h"
#include "graph.h"

/*! \fn CGraphModel::CGraphModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CGraphModel::CGraphModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CGraphModel::CGraphModel(CScadastudio *pUi, CQuoteItem *item)
{
	m_pTree = pUi->GetLeftTree();

	m_pModel = pUi->GetLeftTreeModel();

	m_pTabWidget = pUi->GetTabWidget();

	m_pTopProjectItem = item;

	m_pUi = pUi;

	//����ڵ�
	m_pTopFesItem = new CQuoteItem(QIcon(GRAPH_PNG), QStringLiteral("ͼ��"));

	m_pTopFesItem->setData(GRAPH_ITEM, Qt::UserRole);

	m_pTopFesItem->setEditable(false);

	item->appendRow(m_pTopFesItem);
}

/*! \fn CGraphModel::~CGraphModel()
********************************************************************************************************* 
** \brief CGraphModel::~CGraphModel 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CGraphModel::~CGraphModel()
{

}

/*! \fn void CGraphModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CGraphModel::GetFesType 
** \details 
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CGraphModel::GetFesType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{
	if(nType == GRAPH_ITEM || nType == GRAPH_GROUP_CHILD_ITEM)
	{
		CGraph *pGraph = new CGraph(this, m_pUi, indexSelect, nMouseClickType);

		pGraph->ShowMenu();

		pGraph->deleteLater();
	}
}