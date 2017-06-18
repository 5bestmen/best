/*! @file
<PRE>
********************************************************************************
模块名       :  画图模块
文件名       :  hissave.cpp
文件实现功能 :  画图模块
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

#include <qmenu.h>

#include "graphmodel.h"
#include "scadastudio/imainwindow.h"
#include "scadastudio/quoteItem.h"
#include "scadastudiodefine.h"
#include "graph.h"
#include "scadastudio/treewidget.h"
#include "scadastudio/tabwidget.h"

/*! \fn CGraphModel::CGraphModel(CScadastudio *pUi, CQuoteItem *item)
********************************************************************************************************* 
** \brief CGraphModel::CGraphModel 
** \details 
** \param pUi 
** \param item 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CGraphModel::CGraphModel(IMainWindow *pUi)
	:QObject(pUi)
{
	m_pModel = nullptr;
	m_pTree = nullptr;
	m_pTopFesItem = nullptr;
	m_pTopProjectItem = nullptr;
	m_pTabWidget = nullptr;
	m_pUi = nullptr;

	Q_ASSERT(pUi);
	if (pUi == nullptr)
		return;

	m_pTree = pUi->GetLeftTree();
	m_pModel = pUi->GetLeftTreeModel();
	m_pTabWidget = pUi->GetTabWidget();
	m_pTopProjectItem = pUi->GetTopItem();
	m_pUi = pUi;

//	connect(pUi->GetLeftTree(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
//	connect(pUi->GetLeftTree(), SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(doubleClicked(const QModelIndex &)));
// 	connect(pUi->GetLeftTreeModel(), SIGNAL(itemChanged(QStandardItem *)), this, SLOT(itemChanged(QStandardItem *)));
// 	connect(pUi->GetLeftTree(), SIGNAL(collapsed(const QModelIndex &)), this, SLOT(collapsed(const QModelIndex &)));
// 	connect(pUi->GetLeftTree(), SIGNAL(expanded(const QModelIndex &)), this, SLOT(expanded(const QModelIndex &)));
}

/*! \fn CGraphModel::~CGraphModel()
********************************************************************************************************* 
** \brief CGraphModel::~CGraphModel 
** \details 
** \return  
** \author gw
** \date 2015年11月26日 
** \note 
********************************************************************************************************/
CGraphModel::~CGraphModel()
{

}

void CGraphModel::Init(IMainWindow *pUi, CXmlReader *pXmlReader)
{
	Q_UNUSED(pUi);
	Q_UNUSED(pXmlReader);
	
	////对象节点
	//m_pTopFesItem = new CQuoteItem(QIcon(GRAPH_PNG), QStringLiteral("图形"));

	//m_pTopFesItem->setData(GRAPH_ROOT_ITEM, Qt::UserRole);

	//m_pTopFesItem->setEditable(false);

	//m_pTopProjectItem->appendRow(m_pTopFesItem);
}

void CGraphModel::UnInit()
{

}

#if 0
void CGraphModel::showMouseRightButton(const QPoint &point)
{
	QModelIndex indexSelect = m_pUi->GetLeftTree()->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	if (nType == GRAPH_ITEM || nType == GRAPH_GROUP_CHILD_ITEM)
	{
		ShowMenu(indexSelect);
	}
}

void CGraphModel::itemChanged(QStandardItem *pItem)
{

}

void CGraphModel::ShowMenu(QModelIndex &index)
{
	QMenu *pMenu = new QMenu(NULL);

	if (index.data(Qt::UserRole).toInt() == GRAPH_ITEM ||
		index.data(Qt::UserRole).toInt() == GRAPH_GROUP_CHILD_ITEM)
	{
		QAction *pChannelAction = new QAction(GRAPH_NAME, pMenu);

		pChannelAction->setIcon(QIcon(CHANNEL_PNG));

		pMenu->addAction(pChannelAction);


		QAction *pActionGroup = new QAction(GRAPH_GROUP_NAME, pMenu);

		pMenu->addAction(pActionGroup);

		pActionGroup->setIcon(QIcon(CLOSE_GROUP_PNG));

		pMenu->addAction(pActionGroup);

		QAction* selectedItem = pMenu->exec(QCursor::pos());

		if (selectedItem == pChannelAction)
		{
			//添加图形
			AddGraph(index);
		}
		else if (selectedItem == pActionGroup)
		{
			//添加通道组
			AddGraphGroup(index);
		}
	}

	pMenu->deleteLater();
}

void CGraphModel::AddGraph(QModelIndex &index)
{
	//添加通道
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(QStringLiteral("图形")).arg(0);

	CQuoteItem *newItem = new CQuoteItem(strName);

	newItem->setData(GRAPH_CHILD_ITEM, Qt::UserRole);
	//Tagname
	//newItem->setData(strTagName, TAGNAME);

	newItem->setIcon(QIcon(GRAPH_PNG));

	item->appendRow(newItem);
}

void CGraphModel::AddGraphGroup(QModelIndex &index)
{
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	//Qt::WhatsThisRole 组层
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(QStringLiteral("图形组")).arg(0);

	if (item->data(Qt::WhatsThisRole).toInt() < MAX_GROUP_NUM)
	{
		CQuoteItem *newItem = new CQuoteItem(strName);

		newItem->setData(item->data(Qt::WhatsThisRole).toInt() + 1, Qt::WhatsThisRole);

		newItem->setData(GRAPH_GROUP_CHILD_ITEM, Qt::UserRole);

		newItem->setIcon(QIcon(CLOSE_GROUP_PNG));

		item->appendRow(newItem);
	}

	m_pUi->GetLeftTree()->expand(index);
}


void CGraphModel::doubleClicked(const QModelIndex &index)
{
	int nTest = index.data(Qt::UserRole).toInt();
}

void CGraphModel::OpenView(QWidget *pParent)
{

}
#endif
