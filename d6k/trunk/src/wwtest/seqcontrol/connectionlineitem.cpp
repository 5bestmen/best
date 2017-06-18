#include <QPen>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include "connectionlineitem.h"
#include "connectionrouter.h"

CConnectionLineItem::CConnectionLineItem(qreal x1, qreal y1, qreal x2, qreal y2):
    QGraphicsLineItem(x1,y1,x2,y2)
{
    m_pfBgin.setX(x1);
    m_pfBgin.setY(y1);
    m_pfEnd.setX(x2);
    m_pfEnd.setY(y2);
    m_pLinkLineItem1 = NULL;
    m_pLinkLineItem2 = NULL;


    m_cursor = new QCursor;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    m_bMoveFalg = true;
    this->setZValue(2);
}

CConnectionLineItem::~CConnectionLineItem()
{
    delete m_cursor;
}


void CConnectionLineItem::SetLinkLineItem1(CConnectionLineItem *pLineItem)
{
    m_pLinkLineItem1 = pLineItem;
}

void CConnectionLineItem::SetLinkLineItem2(CConnectionLineItem *pLineItem)
{
    m_pLinkLineItem2 = pLineItem;
}

void CConnectionLineItem::UpdateLinkLineItem(CConnectionLineItem *pLineItem, QPointF pfNewPoint)
{
    Q_ASSERT(pLineItem);

    if (pLineItem == NULL)
    {
        return;
    }

    QLineF itemLine = pLineItem->line();

    //判断移动的线是横向还是纵向
    if (m_pfBgin.x() == m_pfEnd.x())
    {
        // 纵向线
        if (itemLine.p1().x() == itemLine.p2().x() && itemLine.p1().y() != itemLine.p2().y())
        {
//             if (itemLine.p1().y() == itemLine.p2().y())
//             {
//                 return;
//             }

            if (fabs(itemLine.p1().y()-this->line().p1().y())==0)
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                item0->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item0);
                UpdateLinkRelationship(pLineItem,item0,this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p2().y()-this->line().p2().y())==0)
            {
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p2().x(),itemLine.p2().y(),pfNewPoint.x(),itemLine.p2().y());
                item1->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item1);
                UpdateLinkRelationship(pLineItem,item1,this);
                this->scene()->addItem(item1);

            }
            else if (fabs(itemLine.p1().y()-this->line().p2().y())==0)
            {
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                item1->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item1);
                UpdateLinkRelationship(pLineItem,item1,this);
                this->scene()->addItem(item1);
            }
            else if (fabs(itemLine.p2().y()-this->line().p1().y())==0)
            {
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p2().x(),itemLine.p2().y(),pfNewPoint.x(),itemLine.p2().y());
                item1->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item1);
                UpdateLinkRelationship(pLineItem,item1,this);
                this->scene()->addItem(item1);
            }
            else
            {
                //donothing
            }
        }
        else if (itemLine.p1().y() == itemLine.p2().y())
        {
           if (fabs(itemLine.p1().x()-m_pfClickPoint.x())<2)
           {
               pLineItem->setLine(itemLine.p2().x(),itemLine.p2().y(),pfNewPoint.x(),itemLine.p2().y());
           }
           else
           {
               pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
           }

            CConnectionLineItem *pLinkItem = NULL;
            if (pLineItem->GetLinkLineItem1() != this && pLineItem->GetLinkLineItem1() != NULL)
            {
                pLinkItem = pLineItem->GetLinkLineItem1();
            }
            else if (pLineItem->GetLinkLineItem2() != this && pLineItem->GetLinkLineItem2() != NULL)
            {
                pLinkItem = pLineItem->GetLinkLineItem2();
            }
            else if (pLineItem->GetLinkLineItem1() != NULL)
            {
                if (fabs(itemLine.p1().x()-pfNewPoint.x()) > fabs(itemLine.p2().x()-pfNewPoint.x()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                }

                return;

            }
            else if (pLineItem->GetLinkLineItem2() != NULL)
            {
                if (fabs(itemLine.p1().x()-pfNewPoint.x()) > fabs(itemLine.p2().x()-pfNewPoint.x()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
                }
                return;
            }
            else
            {

            }


            if (pLinkItem == NULL)
            {
                return;
            }

            QLineF tLine = pLinkItem->line();
            if (tLine.p1() == itemLine.p1() || tLine.p2() == itemLine.p1())
            {
                //p1为不动点
                pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
            }
            else if (tLine.p1() == itemLine.p2() || tLine.p2() == itemLine.p2())
            {
                //p2为不动点
                pLineItem->setLine(itemLine.p2().x(),itemLine.p1().y(),pfNewPoint.x(),itemLine.p1().y());
            }
        }
        else
        {
            //donothing
        }

    }
    else if (m_pfBgin.y() == m_pfEnd.y())
    {
        //横向线
        if (itemLine.p1().y() == itemLine.p2().y() && itemLine.p1().x() != itemLine.p2().x())
        {
            //增加一条线
            if (fabs(itemLine.p1().x()-this->line().p1().x())==0)
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(),itemLine.p1().y(),itemLine.p1().x(),pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item0);
                //item0->setZValue(-1);
                UpdateLinkRelationship(pLineItem,item0,this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p2().x()-this->line().p2().x())==0)
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p2().x(),itemLine.p2().y(),itemLine.p2().x(),pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item0);
                //item0->setZValue(-1);
                UpdateLinkRelationship(pLineItem,item0,this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p1().x()-this->line().p2().x())==0)
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(),itemLine.p1().y(),itemLine.p1().x(),pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item0);
               // item0->setZValue(-1);
                UpdateLinkRelationship(pLineItem,item0,this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p2().x()-this->line().p1().x())==0)
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p2().x(),itemLine.p2().y(),itemLine.p2().x(),pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                m_lstAddNewItems.append(item0);
               // item0->setZValue(-1);
                UpdateLinkRelationship(pLineItem,item0,this);
                this->scene()->addItem(item0);
            }
            else
            {
                //donothing
            }
        }
        else if (itemLine.p1().x() == itemLine.p2().x())
        {
                CConnectionLineItem *pLinkItem = NULL;
                if (pLineItem->GetLinkLineItem1() != this && pLineItem->GetLinkLineItem1() != NULL)
                {
                    pLinkItem = pLineItem->GetLinkLineItem1();
                }
                else if (pLineItem->GetLinkLineItem2() != this && pLineItem->GetLinkLineItem2() != NULL)
                {
                    pLinkItem = pLineItem->GetLinkLineItem2();
                }
                else if (pLineItem->GetLinkLineItem1() != NULL)
                {
                    if (fabs(itemLine.p1().y()-pfNewPoint.y()) > fabs(itemLine.p2().y()-pfNewPoint.y()))
                    {
                        //取p1
                        pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),itemLine.p1().x(),pfNewPoint.y());
                    }
                    else
                    {
                        pLineItem->setLine(itemLine.p2().x(),itemLine.p2().y(),itemLine.p2().x(),pfNewPoint.y());
                    }
                }
                else if (pLineItem->GetLinkLineItem2() != NULL)
                {
                    if (fabs(itemLine.p1().y()-pfNewPoint.y()) > fabs(itemLine.p2().y()-pfNewPoint.y()))
                    {
                        //取p1
                        pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),itemLine.p1().x(),pfNewPoint.y());
                    }
                    else
                    {
                        pLineItem->setLine(itemLine.p2().x(),itemLine.p2().y(),itemLine.p2().x(),pfNewPoint.y());
                    }
                }
                else
                {

                }

                if (pLinkItem == NULL)
                {
                    return;
                }

                QLineF tLine = pLinkItem->line();
                if (tLine.p1() == itemLine.p1() || tLine.p2() == itemLine.p1())
                {
                    //p1为不动点
                    pLineItem->setLine(itemLine.p1().x(),itemLine.p1().y(),itemLine.p1().x(),pfNewPoint.y());
                }
                else if (tLine.p1() == itemLine.p2() || tLine.p2() == itemLine.p2())
                {
                    //p2为不动点
                    pLineItem->setLine(itemLine.p1().x(),itemLine.p2().y(),itemLine.p1().x(),pfNewPoint.y());
                }

        }
        else
        {
            //return;
        }
    }
    else
    {
        //return;
    }
}

CConnectionLineItem *CConnectionLineItem::GetLinkLineItem1() const
{
    return m_pLinkLineItem1;
}

CConnectionLineItem *CConnectionLineItem::GetLinkLineItem2() const
{
    return m_pLinkLineItem2;
}

void CConnectionLineItem::SetFixPoint(const QPointF &pfpoint)
{
    m_pfFixPoint = pfpoint;
}

QPointF CConnectionLineItem::GetFixPoint() const 
{
    return m_pfFixPoint;
}

void CConnectionLineItem::UpdateLinkRelationship(CConnectionLineItem *pItemOne, CConnectionLineItem *pNewItem, CConnectionLineItem *pthisItem)
{
    m_polyLine->InsertLineItemToVector(pItemOne,pNewItem,pthisItem);
}

void CConnectionLineItem::SetUnMoveable()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setAcceptHoverEvents(false);
}

void CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //改变颜色
        //setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_cursor->setShape(Qt::ClosedHandCursor);

        this->setCursor(*m_cursor);
        m_pfClickPoint = event->scenePos();
    }
    else if (event->button() == Qt::RightButton)
    {
        QMenu *pMenu = new QMenu;
        QAction *pDeleteAct = new QAction(QObject::tr("Delete  "),this);
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_deleteAct()));

        pMenu->addAction(pDeleteAct);
        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }

    m_pfMoveBegin = event->scenePos();
    m_pfMoveEnd = event->scenePos();

    QGraphicsLineItem::mousePressEvent(event);
}

void CConnectionLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_pfMoveEnd = event->scenePos();

    QPointF pfDis = m_pfMoveEnd - m_pfMoveBegin;

    //单位位移太大 忽略
    if (fabs(pfDis.x()) > 15)
    {
        return;
    }

    if (fabs(pfDis.y()) > 15)
    {
        return;
    }

    m_pfMoveBegin = m_pfMoveEnd;

    if (!CheckLinePosition(pfDis))
    {
        m_bMoveFalg = false;
        return;
    }

    if (!m_bMoveFalg)
    {
        return;
    }

    m_cursor->setShape(Qt::ClosedHandCursor);
    this->setCursor(*m_cursor);

    //判断当前直线状态
    //根据状态判断那个方向移动
    if (m_pfBgin.x() == m_pfEnd.x())
    {
        if (fabs(pfDis.x())<1)
        {

            return;
        }
        //只能在x轴方向上移动
        QPointF pfEvent = event->scenePos();
        event->setScenePos(QPointF(pfEvent.x(),m_pfClickPoint.y()));
        this->setLine(event->scenePos().x(),this->line().y1(),event->scenePos().x(),this->line().y2());

    }
    else if (m_pfBgin.y() == m_pfEnd.y())
    {
        if (fabs(pfDis.y())<1)
        {
            return;
        }

        //只能在y周方向上移动
        QPointF pfEvent = event->scenePos();
        event->setScenePos(QPointF(m_pfClickPoint.x(),pfEvent.y()));
        this->setLine(this->line().x1(), event->scenePos().y(), this->line().x2(), event->scenePos().y());
    }
    else
    {
        return;
    }
// 
    //当长度太小时不做处理
    UpdateLinkLineItem(this->GetLinkLineItem1(),event->scenePos());
    UpdateLinkLineItem(this->GetLinkLineItem2(),event->scenePos());


    //QGraphicsLineItem::mouseMoveEvent(event);
}

void CConnectionLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_bMoveFalg = true;
    setPen(QPen());
    m_cursor->setShape(Qt::OpenHandCursor);
    this->setCursor(*m_cursor);
    QGraphicsLineItem::mouseReleaseEvent(event);

    m_polyLine->UpdateLinesFromLink();

//     updateLinkLineItem(this->getLinkLineItem1(),event->scenePos());
//     updateLinkLineItem(this->getLinkLineItem2(),event->scenePos());
//    this->update();

}

void CConnectionLineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsLineItem::hoverMoveEvent(event);
}

void CConnectionLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    this->setCursor(*m_cursor);

    QGraphicsLineItem::hoverLeaveEvent(event);
}

void CConnectionLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::OpenHandCursor);
    this->setCursor(*m_cursor);

    QGraphicsLineItem::hoverEnterEvent(event);
}

void CConnectionLineItem::Slot_deleteAct()
{
    //删除链接关系
    m_polyLine->DeleteLinkInfo();
    m_polyLine = nullptr;
}

void CConnectionLineItem::SetOwnRounter(CConnectionRouter *pRounter)
{
    m_polyLine = pRounter;
}

bool CConnectionLineItem::CheckLinePosition(QPointF pfDis)
{
    int nXlength = pfDis.x() / fabs(pfDis.x());
    int nYlength = pfDis.y() / fabs(pfDis.y());

    if (m_pfBgin.x() == m_pfEnd.x())
    {
        //x不变
        QGraphicsItem *pItem1 = this->scene()->itemAt(this->line().p1().x() + nXlength*14, this->line().p1().y(), QTransform());

        if (pItem1 != nullptr && pItem1->type() == OBJECTITEM)
        {
            return false;
        }

        QGraphicsItem *pItem2 = this->scene()->itemAt(this->line().p2().x() + nXlength * 14, this->line().p2().y(), QTransform());
        if (pItem2 != nullptr && pItem2->type() == OBJECTITEM)
        {
            return false;
        }

        QPointF pfMid = QPointF((this->line().p1().x() + this->line().p2().x()) / 2, (this->line().p1().y() + this->line().p2().y()) / 2);
        QGraphicsItem *pItem3 = this->scene()->itemAt(pfMid.x() + nXlength * 14, pfMid.y(), QTransform());

        if (pItem3 != nullptr && pItem3->type() == OBJECTITEM)
        {
            return false;
        }
    }
    else if (m_pfBgin.y() == m_pfEnd.y())
    {
        //x不变
        QGraphicsItem *pItem1 = this->scene()->itemAt(this->line().p1().x(), this->line().p1().y() + nYlength * 14, QTransform());

        if (pItem1 != nullptr && pItem1->type() == OBJECTITEM)
        {
            return false;
        }

        QGraphicsItem *pItem2 = this->scene()->itemAt(this->line().p2().x(), this->line().p2().y() + nYlength * 14, QTransform());
        if (pItem2 != nullptr && pItem2->type() == OBJECTITEM)
        {
            return false;
        }

        QPointF pfMid = QPointF((this->line().p1().x() + this->line().p2().x()) / 2, (this->line().p1().y() + this->line().p2().y()) / 2);
        QGraphicsItem *pItem3 = this->scene()->itemAt(pfMid.x(), pfMid.y() + nYlength * 14, QTransform());

        if (pItem3 != nullptr && pItem3->type() == OBJECTITEM)
        {
            return false;
        }
    }

//     m_polyLine->UpdateLinesFromLink();

    return true;
}

void CConnectionLineItem::SetLineColor(QColor lineColor)
{
    setPen(QPen(lineColor));
    update();
}

