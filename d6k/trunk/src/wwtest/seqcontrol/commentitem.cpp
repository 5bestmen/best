#include <QGraphicsScene>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsTextItem>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlock>
#include "commentitem.h"
#include "fdborderscene.h"
#include "seqdefine.h"

CCommentItem::CCommentItem()
{
    m_rect = QRectF(0, 0, 200, 30);
    m_nType = GRAPHICSCOMMENT;
    setZValue(1);
    setAcceptDrops(true);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
    m_bScale = false;
    m_cursor = new QCursor;
    m_nDirection = 0;
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setTextWidth(m_rect.width());
    this->setPlainText(tr("starting transportation Belt"));
}

void CCommentItem::ResetRect(QRectF rect)
{
    m_rect = rect;

    if ((int)m_rect.height() % 3 == 0)
    {
        this->setPlainText(this->toPlainText() + "\n");
    }

    setTextWidth(m_rect.width());
    update(boundingRect());
}

QRectF CCommentItem::boundingRect() const
{
    return m_rect;
    //return QRectF(m_rect.x() - 2, m_rect.y() - 2, m_rect.width() + 4, m_rect.height() + 4);
}

QPainterPath CCommentItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

void CCommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & QStyle::State_Selected)
    {
        //设置选中框
        painter->setPen(QPen(QColor(255, 255, 204), 1, Qt::SolidLine));
        painter->setBrush(QBrush(QColor(255, 255, 204)));
        painter->drawRect(m_rect);
    }
    else
    {
        painter->setBrush(QBrush(QColor(255, 255, 204)));
        painter->drawRect(m_rect);
    }

    QGraphicsTextItem::paint(painter, option, widget);
}

void CCommentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos1 = event->scenePos();
    QPointF lt = this->scenePos() + QPointF(m_rect.x(), m_rect.y());
    QPointF lb = this->scenePos() + QPointF(m_rect.x(), m_rect.y() + m_rect.height());
    QPointF rt = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y());
    QPointF rb = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height());

    if ((pos1.x() <= lt.x() + 2 && pos1.y() <= lt.y() + 2) || (pos1.x() >= rb.x() - 2 && pos1.y() >= rb.y() - 2))
    {
        m_cursor->setShape(Qt::SizeFDiagCursor);
        if (pos1.x() <= lt.x() + 2)
        {
            m_nDirection = 8;
        }
        else
        {
            m_nDirection = 4;
        }
    }
    else if ((pos1.x() <= lb.x() + 2 && pos1.y() >= lb.y() - 2) || (pos1.x() >= rt.x() - 2 && pos1.y() <= rt.y() + 2))
    {
        m_cursor->setShape(Qt::SizeBDiagCursor);
        if (pos1.x() <= lb.x() + 2)
        {
            m_nDirection = 6;
        }
        else
        {
            m_nDirection = 2;
        }
    }
    else if ((pos1.x() <= lt.x() + 2 || pos1.x() >= rt.x() - 2) && (pos1.y() <= lb.y() && pos1.y() >= lt.y()))
    {
        m_cursor->setShape(Qt::SizeHorCursor);
        if (pos1.x() <= lt.x() + 2)
        {
            m_nDirection = 7;
        }
        else
        {
            m_nDirection = 3;
        }
    }
    else if ((pos1.y() <= lt.y() + 2 || pos1.y() >= lb.y() - 2) && (pos1.x() >= lt.x() && pos1.x() <= rt.x()))
    {
        m_cursor->setShape(Qt::SizeVerCursor);
        if (pos1.y() <= lt.y() + 2)
        {
            m_nDirection = 1;
        }
        else
        {
            m_nDirection = 5;
        }
    }
    else
    {
        m_nDirection = 0;
        m_bScale = false;
        m_cursor->setShape(Qt::ArrowCursor);
    }

    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void CCommentItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos1 = event->scenePos();
    QPointF lt = this->scenePos() + QPointF(m_rect.x(), m_rect.y());
    QPointF lb = this->scenePos() + QPointF(m_rect.x(), m_rect.y() + m_rect.height());
    QPointF rt = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y());
    QPointF rb = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height());

    m_bScale = true;

    if ((pos1.x() <= lt.x() + 2 && pos1.y() <= lt.y() + 2) || (pos1.x() >= rb.x() - 2 && pos1.y() >= rb.y() - 2))
    {
        m_cursor->setShape(Qt::SizeFDiagCursor);
        if (pos1.x() <= lt.x() + 2)
        {
            m_nDirection = 8;
        }
        else
        {
            m_nDirection = 4;
        }
    }
    else if ((pos1.x() <= lb.x() + 2 && pos1.y() >= lb.y() - 2) || (pos1.x() >= rt.x() - 2 && pos1.y() <= rt.y() + 2))
    {
        m_cursor->setShape(Qt::SizeBDiagCursor);
        if (pos1.x() <= lb.x() + 2)
        {
            m_nDirection = 6;
        }
        else
        {
            m_nDirection = 2;
        }
    }
    else if ((pos1.x() <= lt.x() + 2 || pos1.x() >= rt.x() - 2) && (pos1.y() <= lb.y() && pos1.y() >= lt.y()))
    {
        m_cursor->setShape(Qt::SizeHorCursor);
        if (pos1.x() <= lt.x() + 2)
        {
            m_nDirection = 7;
        }
        else
        {
            m_nDirection = 3;
        }
    }
    else if ((pos1.y() <= lt.y() + 2 || pos1.y() >= lb.y() - 2) && (pos1.x() >= lt.x() && pos1.x() <= rt.x()))
    {
        m_cursor->setShape(Qt::SizeVerCursor);
        if (pos1.y() <= lt.y() + 2)
        {
            m_nDirection = 1;
        }
        else
        {
            m_nDirection = 5;
        }
    }
    else
    {
        m_nDirection = 0;
        m_bScale = false;
        m_cursor->setShape(Qt::ArrowCursor);
    }

    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

void CCommentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    m_bScale = false;
    this->setCursor(*m_cursor);

    setTextInteractionFlags(Qt::NoTextInteraction);

    update();
    QGraphicsItem::hoverLeaveEvent(event);
}

void CCommentItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void CCommentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pfStart = event->scenePos();
    }
    else if (event->button() == Qt::RightButton)
    {
        QMenu *pMenu = new QMenu;
        QAction *pDeleteAct = new QAction(QObject::tr("Delete  "),this);
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_DeleteAct()));

        pMenu->addAction(pDeleteAct);
        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();
    }
    else
    {

    }

    update();
    QGraphicsTextItem::mousePressEvent(event);
}

void CCommentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bScale && m_nDirection!=0)
    {
        QPointF dis;
        m_pfEnd = event->scenePos();
        dis = m_pfEnd - m_pfStart;
        m_pfStart = m_pfEnd;

        switch (m_nDirection)
        {
        case 1:
        {
            //item最大值  最小值判断
            if ((!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200)) || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(0, dis.y() / 2);
            }
            break;

        }
        case 2:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(-dis.x() / 2, -dis.y() / 2);
            }
            break;
        }
        case 3:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 )
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(-dis.x() / 2, 0);
            }
            break;
        }
        case 4:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(-dis.x() / 2, -dis.y() / 2);
            }
            break;
        }
        case 5:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(0, -dis.y() / 2);
            }
            break;
        }
        case 6:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(-dis.x() / 2, -dis.y() / 2);
            }
            break;
        }
        case 7:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 )
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(dis.x() / 2, 0);
            }
            break;
        }
        case 8:
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                //update(boundingRect());
                this->moveBy(dis.x() / 2, dis.y() / 2);
            }
            break;
        }
        default:
            break;
        }
    }
    else
    {
        QGraphicsTextItem::mouseMoveEvent(event);
    }
    

    update();

}

void CCommentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_bScale = false;
    update();

    //更新
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }

    QGraphicsTextItem::mouseReleaseEvent(event);


}

void CCommentItem::Slot_DeleteAct()
{
//     CPlcOrderScene *pScene = qobject_cast<CPlcOrderScene*>(this->scene());
//     if (pScene != nullptr)
//     {
//         pScene->RemoveOneComment(this);
//         pScene->UpdateOperate();
//     }


    this->deleteLater();
}

QRectF CCommentItem::GetItemRect()
{
    return m_rect;
}

int CCommentItem::GetTextType()
{
    return m_nType;
}

CCommentItem::~CCommentItem()
{
    delete m_cursor;
}
