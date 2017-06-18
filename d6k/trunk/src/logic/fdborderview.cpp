/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  fdborderview.cpp
文件实现功能 :  视图
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include <QMimeData>
#include <QPainter>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDebug>
#include <QCursor>
#include <QMenu>
#include <QPushButton>
#include "fdborderview.h"
#include "funblockdiagramitem.h"
#include "fdborderscene.h"
#include "axisframe.h"
#include "callsritem.h"
#include "moveorderitem.h"
#include "mathorderitem.h"
#include "compareorderitem.h"
#include "timersorderitem.h"

/*! \fn CFdbOrderView::CFdbOrderView(QGraphicsScene *scene, QWidget *parent)
*********************************************************************************************************
** \brief CFdbOrderView::CFdbOrderView(QGraphicsScene *scene, QWidget *parent)
** \details 构造函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderView::CFdbOrderView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent)
{
    Q_ASSERT(scene);
    if (scene == nullptr)
    {
        return;
    }
    //设置边框 用于放刻度尺
    this->setViewportMargins(20, 20, 0, 0);

    //横轴和纵轴刻度尺
    CAxisFrame *pAxisXFrame = new CAxisFrame(1, this);
    CAxisFrame *pAxisYFrame = new CAxisFrame(2, this);

    pAxisXFrame->setContentsMargins(0, 0, 0, 0);
    pAxisYFrame->setContentsMargins(0, 0, 0, 0);
    m_nNodeIndex = 0;
    m_nItemIndex = 1;
}

CFdbOrderView::~CFdbOrderView()
{
    //     foreach(CLadderBaseGraphicsItem *item,m_lstGraphicsItems)
    //     {
    //         item->deleteLater();
    //     }
}

/*! \fn CFdbOrderView::dragEnterEvent(QDragEnterEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::dragEnterEvent(QDragEnterEvent *event)
** \details 拖动进入
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::dragEnterEvent(QDragEnterEvent *event)
{
    //接收文字类型的数据
    if (event->mimeData()->hasFormat("text/plain"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

/*! \fn CFdbOrderView::dragMoveEvent(QDragEnterEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::dragMoveEvent(QDragEnterEvent *event)
** \details 拖动移动
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
    {
        //接收文字类型的数据
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

/*! \fn CFdbOrderView::dropEvent(QDropEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::dropEvent(QDropEvent *event)
** \details 释放
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::dropEvent(QDropEvent *event)
{
    //取文字类型的数据
    if (event->mimeData()->hasFormat("text/plain"))
    {
        //携带的数据
        QByteArray csvData = event->mimeData()->data("text/plain");
        int ChangeType = csvData.toInt();
        //item 的数量不能超过最大值10000
        if (m_nItemIndex == 10000)
        {
            m_nItemIndex = 1;
        }

        //更新
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());
        //创建item对象
        CFdbOrderBaseItem *pLaderrItem = pScene->CreateItemObj(ChangeType, m_nItemIndex++);

        if (pLaderrItem == nullptr)
        {
            return;
        }

        m_lstGraphicsItems.append(pLaderrItem);
        pLaderrItem->setPos(this->mapToScene(event->pos()));
        this->scene()->addItem(pLaderrItem);

        //更新数据模型
        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
    }
}

/*! \fn CFdbOrderView::mousePressEvent(QMouseEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::mousePressEvent(QMouseEvent *event)
** \details 鼠标移动事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        viewport()->setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

/*! \fn CFdbOrderView::paintEvent(QPaintEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::paintEvent(QPaintEvent *event)
** \details 重绘
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::paintEvent(QPaintEvent *event)
{
    //是视图范围内画图
    QPainter painter(this->viewport());
    painter.setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));

    int nWidth = this->viewport()->width();;
    int nHeight = this->viewport()->height();
    //划田字格
    for (int i = 1; i <= nWidth / 150; i++)
    {
        painter.drawLine(QLine(i * 150, 0, i * 150, nHeight));
    }

    for (int m = 1; m <= nHeight / 150; m++)
    {
        painter.drawLine(QLine(0, m * 150, nWidth, m * 150));
    }
    QGraphicsView::paintEvent(event);
}

/*! \fn CFdbOrderView::enterEvent(QEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::enterEvent(QEvent *event)
** \details 鼠标进入事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::enterEvent(QEvent *event)
{
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());
    if (pScene != NULL)
    {
        if (pScene->GetMode() == CURSOR_LINE)
        {
            // viewport()->setCursor(Qt::CrossCursor);
        }
        else
        {
            viewport()->setCursor(Qt::ArrowCursor);
        }
    }
    QGraphicsView::enterEvent(event);
}

/*! \fn CFdbOrderView::UpdateItemsExecNum()
*********************************************************************************************************
** \brief CFdbOrderView::UpdateItemsExecNum()
** \details 更新执行编号
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::UpdateItemsExecNum()
{
    m_nNodeIndex = 0;

    QList<CFdbOrderBaseItem*> lstItems = m_lstGraphicsItems;
    QList<CFdbOrderBaseItem*> lstHeaderItems;

    foreach(CFdbOrderBaseItem* pItem, m_lstGraphicsItems)
    {
        //更新最新的item  num
        pItem->SetInputNum();

        //取起始节点
        if (pItem->GetInputNums() == 0)
        {
            lstHeaderItems.append(pItem);
        }
    }

    m_nNodeIndex = 1;
    m_lstEnterItems.clear();
    //取入度为0的点作为起始点
    foreach(CFdbOrderBaseItem *pItem, lstHeaderItems)
    {
        TravelItems(pItem);
    }
}

/*! \fn CFdbOrderView::TravelItems(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief CFdbOrderView::TravelItems(CFdbOrderBaseItem* pItem)
** \details 引入入度 出度算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::TravelItems(CFdbOrderBaseItem* pItem)
{
    Q_ASSERT(pItem);
    if (pItem == nullptr)
    {
        return;
    }

    pItem->SetIndexNumber(m_nNodeIndex);

    m_nNodeIndex++;

    for (int i = 0; i < pItem->GetOutPutList().values().count(); i++)
    {
        for (int j = 0; j < pItem->GetOutPutList().values()[i].count(); j++)
        {
            CFdbOrderBaseItem* pChildItem = pItem->GetOutPutList().values()[i].at(j);

            int nInputNums = pChildItem->MinusInputNum();

            if (nInputNums == 0)
            {
                //递归遍历 所有数据
                TravelItems(pChildItem);
            }
        }
    }
}

/*! \fn CFdbOrderView::DeleteRemoveItem(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief CFdbOrderView::DeleteRemoveItem(CFdbOrderBaseItem* pItem)
** \details 删除item
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::DeleteRemoveItem(CFdbOrderBaseItem* pItem)
{
    Q_ASSERT(pItem);
    if (pItem == nullptr)
    {
        return;
    }
    m_lstGraphicsItems.removeOne(pItem);
}

QList<CFdbOrderBaseItem*> CFdbOrderView::GetAllItems()
{
    return m_lstGraphicsItems;
}

/*! \fn CFdbOrderView::InsertOneItem(CFdbOrderBaseItem* pItem)
*********************************************************************************************************
** \brief CFdbOrderView::InsertOneItem(CFdbOrderBaseItem* pItem)
** \details 插入item
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::InsertOneItem(CFdbOrderBaseItem* pItem)
{
    Q_ASSERT(pItem);
    if (pItem == nullptr)
    {
        return;
    }
    m_lstGraphicsItems.append(pItem);
}

void CFdbOrderView::ClearItems()
{
    m_lstGraphicsItems.clear();
}

void CFdbOrderView::AddCreateNum()
{
    m_nItemIndex++;
}

/*! \fn CFdbOrderView::ClearCreateNum()
*********************************************************************************************************
** \brief CFdbOrderView::ClearCreateNum()
** \details 重置编号  当清空等情况时使用
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::ClearCreateNum()
{
    m_nItemIndex = 1;
}

int CFdbOrderView::GetNextCreateNum()
{
    return m_nItemIndex;
}

/*! \fn CFdbOrderView::UpdateItemList()
*********************************************************************************************************
** \brief CFdbOrderView::UpdateItemList()
** \details       根据index 编号改变顺序
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFdbOrderView::UpdateItemList()
{
    for (int i = 0; i < m_lstGraphicsItems.count() - 1; i++)
    {
        for (int j = 1; j < m_lstGraphicsItems.count(); j++)
        {
            if (m_lstGraphicsItems.at(j - 1)->GetIndexNum() > m_lstGraphicsItems.at(j)->GetIndexNum())
            {
                CFdbOrderBaseItem* pTemp = m_lstGraphicsItems.at(j - 1);
                m_lstGraphicsItems[j - 1] = m_lstGraphicsItems.at(j);
                m_lstGraphicsItems[j] = pTemp;
            }
        }
    }
}

