/*! @file
<PRE>
********************************************************************************
模块名       :  画图
文件名       :  graph.cpp
文件实现功能 :  画图
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

#include <QModelIndex>
#include <QMenu>
#include "graph.h"
#include "scadastudio/quoteItem.h"
#include "scadastudio/uimgr.h"
#include "scadastudiodefine.h"
#include "scadastudio/treewidget.h"

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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CGraph::CGraph(IMainWindow *pUi)
	:QObject(pUi)
{
	m_pUi = pUi;

	connect(pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));

	connect(pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));

	connect(pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));

	connect(pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));

	connect(pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
}

/*! \fn CGraph::~CGraph()
********************************************************************************************************* 
** \brief CGraph::~CGraph 
** \details 
** \return  
** \author gw
** \date 2015年11月26日 
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
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CGraph::AddGraph()
{
	////添加通道
	//CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	//QString str = item->text();

	//QString strName = QString("%1%2").arg("转换").arg(0);

	//CQuoteItem *newItem = new CQuoteItem(strName);

	//newItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole);

	//newItem->setIcon(QIcon(CHANNEL_PNG));

	//item->appendRow(newItem);

	//m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CGraph::AddGraphGroup()
********************************************************************************************************* 
** \brief CGraph::AddGraphGroup 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CGraph::AddGraphGroup()
{
	//CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(m_index);

	////Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	//QString str = item->text();

	//QString strName = QString("%1%2").arg("图形组").arg(0);

	//if(item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	//{
	//	CQuoteItem *newItem = new CQuoteItem(strName);

	//	newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

	//	newItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole);

	//	newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

	//	item->appendRow(newItem);
	//}

	//m_pUi->GetLeftTree()->expand(m_index);
}

/*! \fn void CGraph::ShowMenu()
********************************************************************************************************* 
** \brief CGraph::ShowMenu 
** \details 
** \return void 
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
void CGraph::ShowMenu(QModelIndex &index)
{
	Q_UNUSED(index);
	
	//if(m_nMouseClickType != CTreeWidget::RIGHTCLICK)
	//{
	//	return;
	//}

	//QMenu *pMenu = new QMenu(NULL);

	//if(m_index.data(Qt::UserRole).toInt() == GRAPH_ITEM ||
	//	m_index.data(Qt::UserRole).toInt() == GRAPH_GROUP_CHILD_ITEM)
	//{
	//	QAction *pActionAlarm = new QAction(GRAPH_NAME, pMenu);

	//	pActionAlarm->setIcon(QIcon(CHANNEL_PNG));

	//	pMenu->addAction(pActionAlarm);


	//	QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);

	//	pMenu->addAction(pActionGroup);

	//	pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

	//	pMenu->addAction(pActionGroup);

	//	QAction* selectedItem = pMenu->exec(QCursor::pos());

	//	if(selectedItem == pActionAlarm)
	//	{
	//		//添加转换
	//		AddGraph();
	//	}
	//	else if(selectedItem == pActionGroup)
	//	{
	//		//添加转换组
	//		AddGraphGroup();
	//	}
	//}
	//else if(m_index.data(Qt::UserRole).toInt() == GRAPH_GROUP_CHILD_ITEM)
	//{

	//}

	//pMenu->deleteLater();
}

void CGraph::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == FES_CHANNEL_ITEM || nType == FES_CHANNEL_CHILD_ITEM || FES_CHANNEL_GROUP_ITEM)
	{
		ShowMenu(indexSelect);
	}
}


void CGraph::doubleClicked(const QModelIndex &index)
{
	Q_UNUSED(index);
	
	//int nTest = index.data(Qt::UserRole).toInt();

//	m_pUI->GetTabWidget()->addTab(new QWidget(NULL), "test");
}