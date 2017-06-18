/*! @file
<PRE>
********************************************************************************
ģ����       :	���η�֧
�ļ���       :  treewidget.cpp
�ļ�ʵ�ֹ��� :  ���η�֧
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
*  @date    2015.11.16
*/

#include <QMouseEvent>
#include <QStandardItemModel>
#include "treewidget.h"
#include "define.h"
#include "quoteItem.h"

/*! \fn CTreeWidget::CTreeWidget(QWidget *parent)
********************************************************************************************************* 
** \brief CTreeWidget::CTreeWidget 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
CTreeWidget::CTreeWidget(QWidget *parent)
	: QTreeView(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMouseRightButton(const QPoint&)));
	
	//connect(this,SIGNAL(sendFesType(int)))

	setSortingEnabled(true);

	setEditTriggers(QAbstractItemView::SelectedClicked);
}

/*! \fn CTreeWidget::~CTreeWidget()
********************************************************************************************************* 
** \brief CTreeWidget::~CTreeWidget 
** \details 
** \return  
** \author gw
** \date 2015��11��16�� 
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
** \date 2015��11��16�� 
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
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CTreeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
 	QModelIndex index =  this->indexAt(event->pos());
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
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CTreeWidget::mousePressEvent(QMouseEvent *event)
{
	QTreeView::mousePressEvent(event);
}

/*! \fn void CTreeWidget::CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType)
********************************************************************************************************* 
** \brief CTreeWidget::CheckType 
** \details �жϷ�֧
** \param nType 
** \param indexSelect 
** \param nMouseClickType 
** \return void 
** \author gw
** \date 2015��11��16�� 
** \note 
********************************************************************************************************/
void CTreeWidget::CheckType(int nType, QModelIndex &indexSelect, int nMouseClickType)
{	
	//�ж��ĸ�����
	emit SendFesType(nType, indexSelect, nMouseClickType);
}