/*! @file
<PRE>
********************************************************************************
ģ����       :  ��ͼ
�ļ���       :  graph.cpp
�ļ�ʵ�ֹ��� :  ��ͼ
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

#include <QModelIndex>
#include <QMenu>
#include "graph.h"
#include "quoteItem.h"
#include "scadastudio.h"
#include "define.h"
#include "treewidget.h"

/*! \fn CGraph::CGraph(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
********************************************************************************************************* 
** \brief CGraph::CGraph 
** \details 
** \param parent 
** \param pUi 
** \param index 
** \param mouseClickType 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CGraph::CGraph(QObject *parent, CScadastudio *pUi, QModelIndex &index, int mouseClickType)
	:QObject(parent), m_index(index)
{
	m_pUi = pUi;

	m_index = index;

	m_nMouseClickType = mouseClickType;
}

/*! \fn CGraph::~CGraph()
********************************************************************************************************* 
** \brief CGraph::~CGraph 
** \details 
** \return  
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CGraph::~CGraph()
{

}

/*! \fn void CGraph::AddGraph()
********************************************************************************************************* 
** \brief CGraph::AddGraph 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CGraph::AddGraph()
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	QString str = item->text();

	QString strName = QString("%1%2").arg("ת��").arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole);

	newItem->setIcon(QIcon(CHANNEL_PNG));

	item->appendRow(newItem);

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CGraph::AddGraphGroup()
********************************************************************************************************* 
** \brief CGraph::AddGraphGroup 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CGraph::AddGraphGroup()
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//Qt::WhatsThisRole ���
	int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg("ͼ����").arg(0);

	if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CGraph::ShowMenu()
********************************************************************************************************* 
** \brief CGraph::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
void CGraph::ShowMenu()
{
	if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	{
		return;
	}

	QMenu *pMenu = new QMenu(NULL);

	if(m_index.data(Qt::UserRole).toInt() == GRAPH_ITEM ||
		m_index.data(Qt::UserRole).toInt() == GRAPH_GROUP_CHILD_ITEM)
	{
		QAction *pActionAlarm = new QAction(GRAPH_NAME, pMenu);

		pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pActionAlarm);


		QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if(selectedItem == pActionAlarm)
		{
			//���ת��
			AddGraph();
		}
		else if(selectedItem == pActionGroup)
		{
			//���ת����
			AddGraphGroup();
		}
	}
	else if(m_index.data(Qt::UserRole).toInt() == GRAPH_GROUP_CHILD_ITEM)
	{

	}

	pMenu->deleteLater();
}