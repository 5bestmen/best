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
** \date 2015��11��26�� 
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
** \date 2015��11��26�� 
** \note 
********************************************************************************************************/
CGraphModel::~CGraphModel()
{

}

void CGraphModel::Init(IMainWindow *pUi, CXmlReader *pXmlReader)
{
	Q_UNUSED(pUi);
	Q_UNUSED(pXmlReader);
	
	////����ڵ�
	//m_pTopFesItem = new CQuoteItem(QIcon(GRAPH_PNG), QStringLiteral("ͼ��"));

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
			//���ͼ��
			AddGraph(index);
		}
		else if (selectedItem == pActionGroup)
		{
			//���ͨ����
			AddGraphGroup(index);
		}
	}

	pMenu->deleteLater();
}

void CGraphModel::AddGraph(QModelIndex &index)
{
	//���ͨ��
	CQuoteItem *item = (CQuoteItem *)m_pUi->GetLeftTreeModel()->itemFromIndex(index);

	QString str = item->text();

	QString strName = QString("%1%2").arg(QStringLiteral("ͼ��")).arg(0);

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

	//Qt::WhatsThisRole ���
	//int nTest = item->data(Qt::WhatsThisRole).toInt();

	QString str = item->text();

	QString strName = QString("%1%2").arg(QStringLiteral("ͼ����")).arg(0);

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
