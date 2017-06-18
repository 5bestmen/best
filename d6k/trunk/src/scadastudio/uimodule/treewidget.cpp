/*! @file
<PRE>
********************************************************************************
模块名       :	树形分支
文件名       :  treewidget.cpp
文件实现功能 :  树形分支
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
*  @date    2015.11.16
*/

#include <QMouseEvent>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include "scadastudio/treewidget.h"
#include "scadastudiodefine.h"
#include "scadastudio/quoteItem.h"
#include "delegate.h"
#include "scadastudio/define.h"

/*! \fn CTreeWidget::CTreeWidget(QWidget *parent)
********************************************************************************************************* 
** \brief CTreeWidget::CTreeWidget 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTreeWidget::CTreeWidget(QWidget *parent)
	: QTreeView(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	
	//connect(this,SIGNAL(sendFesType(int)))

	setSortingEnabled(true);

	setDragEnabled(true);

	//单击鼠标编辑
	//setEditTriggers(QAbstractItemView::SelectedClicked);
	setEditTriggers(QAbstractItemView::EditKeyPressed);
	auto *pDelete = new CLineEditDelegate(this, LINESTRING, MAX_TAGNAME_LEN_SCADASTUDIO);
	this->setItemDelegateForColumn(0, pDelete);
}

/*! \fn CTreeWidget::~CTreeWidget()
********************************************************************************************************* 
** \brief CTreeWidget::~CTreeWidget 
** \details 
** \return  
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
CTreeWidget::~CTreeWidget()
{

}


/*! \fn void CTreeWidget::showMouseRightButton(const QPoint& point)
********************************************************************************************************* 
** \brief CTreeWidget::showMouseRightButton 
** \details 
** \param point 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CTreeWidget::showMouseRightButton(const QPoint& point)
{
	QModelIndex indexSelect = this->indexAt(point);

	int nType = indexSelect.data(Qt::UserRole).toInt();

	CheckType(nType, indexSelect, RIGHTCLICK);
}

/*! \fn void CTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
********************************************************************************************************* 
** \brief CTreeWidget::mouseDoubleClickEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	QModelIndex index = this->indexAt(event->pos());
	
	if(!index.isValid())
	{
		return;
	}

	QStandardItemModel *model = (QStandardItemModel *)this->model();

	CQuoteItem *item = (CQuoteItem *)model->itemFromIndex(index);

	int nTest = item->data(Qt::UserRole).toInt();

	QString strKey = item->data(Qt::EditRole).toString();

	CheckType(nTest, index, LEFTDOUBLECLICK);

	QTreeView::mouseDoubleClickEvent(event);
}

/*! \fn void CTreeWidget::mousePressEvent(QMouseEvent *event)
********************************************************************************************************* 
** \brief CTreeWidget::mousePressEvent 
** \details 
** \param event 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CTreeWidget::mousePressEvent(QMouseEvent *event)
{
	QTreeView::mousePressEvent(event);
}
/*! \fn void CTreeWidget::CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CTreeWidget::CheckType 
** \details 判断分支
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015年11月16日 
** \note 
********************************************************************************************************/
void CTreeWidget::CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{	
	//判断哪个分项
	emit SendFesType(nType, indexSelect, nMouseClickType);
}

void ExpandChildren(const QModelIndex &index, QTreeView *pView)
{
	if (!index.isValid()) 
	{
		return;
	}
	
	int childCount = index.model()->rowCount(index);
	for (int i = 0; i < childCount; i++) 
	{
		const QModelIndex &child = index.child(i, 0);
	//  Recursively call the function for each child node.
		ExpandChildren(child, pView);
	}

	Q_ASSERT(pView);
	if (pView == nullptr)
		return;

	if (!pView->isExpanded(index))
	{
		pView->expand(index);
	}
}

void CollapseChildren(const QModelIndex &index, QTreeView *pView)
{
	if (!index.isValid())
	{
		return;
	}

	int childCount = index.model()->rowCount(index);
	for (int i = 0; i < childCount; i++)
	{
		const QModelIndex &child = index.child(i, 0);
		// Recursively call the function for each child node.
		CollapseChildren(child, pView);
	}

	Q_ASSERT(pView);
	if (pView == nullptr)
		return;

	pView->collapse(index);
}