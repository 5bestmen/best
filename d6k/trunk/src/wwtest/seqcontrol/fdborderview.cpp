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

CFdbOrderView::CFdbOrderView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene,parent)
{
    //需要增加刻度尺  延后实现
    this->setViewportMargins(20, 20, 0, 0);
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

void CFdbOrderView::dragEnterEvent(QDragEnterEvent *event)
{
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

void CFdbOrderView::dragMoveEvent(QDragMoveEvent *event)
{
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

void CFdbOrderView::dropEvent(QDropEvent *event)
{
    
    if (event->mimeData()->hasFormat("text/plain"))
    {
        QByteArray csvData = event->mimeData()->data("text/plain");
        int ChangeType = csvData.toInt();
        //int nCreateNum = m_lstGraphicsItems.count() + 1;
        if (m_nItemIndex == 10000)
        {
            m_nItemIndex = 1;
        }

        CFdbOrderBaseItem *pLaderrItem = nullptr;

        if (ChangeType >= BITAND && ChangeType <= BITXOR)
        {
            pLaderrItem = new CFunBlockDiagramItem(ChangeType, m_nItemIndex++);
            
        }
        else if (ChangeType >= BITSHL && ChangeType <= BITCLR)
        {
            pLaderrItem = new CMoveOrderItem(ChangeType, m_nItemIndex++);
        }
        else if (ChangeType >= MATHMOVE && ChangeType <= MATHMIN)
        {
            pLaderrItem = new CMathOrderItem(ChangeType, m_nItemIndex++);
        }
        else if (ChangeType >= COMPAREEQ && ChangeType <= COMPARENE)
        {
            pLaderrItem = new CCompareOrderItem(ChangeType, m_nItemIndex++);
        }
        else if (ChangeType >= TIMERCTD && ChangeType <= TIMERTP)
        {
            pLaderrItem = new CTimersOrderItem(ChangeType, m_nItemIndex++);
        }
        else if (ChangeType == CALLSR)
        {
            pLaderrItem = new CCallSRItem(ChangeType, m_nItemIndex++);
        }

        if (pLaderrItem == nullptr)
        {
            return;
        }

        m_lstGraphicsItems.append(pLaderrItem);
        pLaderrItem->setPos(this->mapToScene(event->pos()));
        this->scene()->addItem(pLaderrItem);

        //更新
        CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

        if (pScene != nullptr)
        {
            pScene->UpdateOperate();
        }
    }
}

void CFdbOrderView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        viewport()->setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mousePressEvent(event);
}


void CFdbOrderView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this->viewport());
    painter.setPen(QPen(QColor(0, 0, 0), 1, Qt::DotLine));

    int nWidth = this->viewport()->width();;
    int nHeight = this->viewport()->height();

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

void CFdbOrderView::enterEvent(QEvent *event)
{
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());
    if (pScene != NULL)
    {
        if (pScene->GetMode() == CURSORLINE)
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

    foreach(CFdbOrderBaseItem *pItem, lstHeaderItems)
    {
        TravelItems(pItem);
    }
}

void CFdbOrderView::TravelItems(CFdbOrderBaseItem* pItem)
{
    if (pItem == nullptr)
    {
        return;
    }

//     if (m_lstEnterItems.contains(pItem))
//     {
//         return;
//     }
//     else
//     {
//         m_lstEnterItems.append(pItem);
//     }

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
                TravelItems(pChildItem);
            }
        }
    }
}

void CFdbOrderView::DeleteRemoveItem(CFdbOrderBaseItem* pItem)
{
    m_lstGraphicsItems.removeOne(pItem);
}

QList<CFdbOrderBaseItem*> CFdbOrderView::GetAllItems()
{
    return m_lstGraphicsItems;
}

void CFdbOrderView::InsertOneItem(CFdbOrderBaseItem* pItem)
{
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

void CFdbOrderView::ClearCreateNum()
{
    m_nItemIndex = 1;
}

int CFdbOrderView::GetNextCreateNum()
{
    return m_nItemIndex;
}

void CFdbOrderView::UpdateItemList()
{
    for (int i = 0; i < m_lstGraphicsItems.count() - 1; i++)
    {
        for (int j = 1; j < m_lstGraphicsItems.count(); j++)
        {
            if (m_lstGraphicsItems.at(j-1)->GetIndexNum() > m_lstGraphicsItems.at(j)->GetIndexNum())
            {
                CFdbOrderBaseItem* pTemp = m_lstGraphicsItems.at(j-1);
                m_lstGraphicsItems[j-1] = m_lstGraphicsItems.at(j);
                m_lstGraphicsItems[j] = pTemp;
            }
        }
    }
}




// void CGraphicsView::mouseMoveEvent(QMouseEvent *event)
// {
//     QGraphicsView;; mouseMoveEvent(event);
// 
// }
