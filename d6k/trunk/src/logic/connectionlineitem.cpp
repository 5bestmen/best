/*! @file
<PRE>
********************************************************************************
模块名       :  直线对象
文件名       :  connectionlineitem.cpp
文件实现功能 :  所有的连线都是由 多个直线对象构成
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
#include <QPen>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include "connectionlineitem.h"
#include "connectionrouter.h"

/*! \fn CConnectionLineItem::CConnectionLineItem(qreal x1, qreal y1, qreal x2, qreal y2)
*********************************************************************************************************
** \brief CConnectionLineItem::CConnectionLineItem(qreal x1, qreal y1, qreal x2, qreal y2)
** \details 构造函数
** \param nItemType：起始点x1 y1   结束点 x2 y2
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CConnectionLineItem::CConnectionLineItem(qreal x1, qreal y1, qreal x2, qreal y2) :
QGraphicsLineItem(x1, y1, x2, y2)
{
    //设置起始点
    m_pfBgin.setX(x1);
    m_pfBgin.setY(y1);

    //设置直线 结束点
    m_pfEnd.setX(x2);
    m_pfEnd.setY(y2);

    //与改线相连的两条线
    m_pLinkLineItem1 = NULL;
    m_pLinkLineItem2 = NULL;

    //鼠标状态
    m_cursor = new QCursor;
    //基本属性
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setAcceptHoverEvents(true);
    //该条直线是否可以移动(两端的线不能移动)
    m_bMoveFalg = true;
    //直线  在 注释 上 ,在逻辑块下
    this->setZValue(2);
}

CConnectionLineItem::~CConnectionLineItem()
{
    delete m_cursor;
}

/*! \fn CConnectionLineItem::SetLinkLineItem1(CConnectionLineItem *pLineItem)
*********************************************************************************************************
** \brief CConnectionLineItem::SetLinkLineItem1(CConnectionLineItem *pLineItem)
** \details 设置相连的item
** \param nItemType：相连的item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::SetLinkLineItem1(CConnectionLineItem *pLineItem)
{
    Q_ASSERT(pLineItem);
    if (pLineItem == nullptr)
    {
        return;
    }

    m_pLinkLineItem1 = pLineItem;
}

/*! \fn CConnectionLineItem::SetLinkLineItem2(CConnectionLineItem *pLineItem)
*********************************************************************************************************
** \brief CConnectionLineItem::SetLinkLineItem2(CConnectionLineItem *pLineItem)
** \details 设置相连的item
** \param nItemType：相连的item
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::SetLinkLineItem2(CConnectionLineItem *pLineItem)
{
    Q_ASSERT(pLineItem);
    if (pLineItem == nullptr)
    {
        return;
    }
    m_pLinkLineItem2 = pLineItem;
}

/*! \fn CConnectionLineItem::UpdateLinkLineItem(CConnectionLineItem *pLineItem, QPointF pfNewPoint)
*********************************************************************************************************
** \brief CConnectionLineItem::UpdateLinkLineItem(CConnectionLineItem *pLineItem, QPointF pfNewPoint)
** \details    在改直线移动时 需要更新与之相连的直线的位置信息
** \param nItemType：  pLineItem:相连一端的直线对象   pfNewPoint：移动前的点
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::UpdateLinkLineItem(CConnectionLineItem *pLineItem, QPointF pfNewPoint)
{
    Q_ASSERT(pLineItem);

    if (pLineItem == NULL)
    {
        return;
    }
    //相连线的连接信息
    QLineF itemLine = pLineItem->line();

    //判断移动的线是横向还是纵向
    if (m_pfBgin.x() == m_pfEnd.x())   //纵向线
    {
        // 判断与之相连的线  是纵向线 还是横向线
        if (itemLine.p1().x() == itemLine.p2().x() && itemLine.p1().y() != itemLine.p2().y())     //纵向线
        {

            if (fabs(itemLine.p1().y() - this->line().p1().y()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                //新增一条线
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
                //每条直线都归属于一条链路 链路对象对直线进行管理
                item0->SetOwnRounter(m_polyLine);
                //更新 直线对象之间的关系 
                UpdateLinkRelationship(pLineItem, item0, this);

                this->scene()->addItem(item0);
            }
            else if (fabs(itemLine.p2().y() - this->line().p2().y()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                //新增一条线
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p2().x(), itemLine.p2().y(), pfNewPoint.x(), itemLine.p2().y());
                //每条直线都归属于一条链路 链路对象对直线对象进行管理
                item1->SetOwnRounter(m_polyLine);
                //更新 直线对象之间的关系
                UpdateLinkRelationship(pLineItem, item1, this);

                this->scene()->addItem(item1);
            }
            else if (fabs(itemLine.p1().y() - this->line().p2().y()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                //新增一条线
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
                //每条直线都归属于一条链路 链路对象对直线对象进行管理
                item1->SetOwnRounter(m_polyLine);
                //更新 直线对象之间的关系
                UpdateLinkRelationship(pLineItem, item1, this);
                this->scene()->addItem(item1);
            }
            else if (fabs(itemLine.p2().y() - this->line().p1().y()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                //新增一条线
                CConnectionLineItem *item1 = new CConnectionLineItem(itemLine.p2().x(), itemLine.p2().y(), pfNewPoint.x(), itemLine.p2().y());
                //每条直线都归属于一条链路 链路对象对直线对象进行管理
                item1->SetOwnRounter(m_polyLine);
                //更新 直线对象之间的关系
                UpdateLinkRelationship(pLineItem, item1, this);

                this->scene()->addItem(item1);
            }
            else
            {
                //donothing
            }
        }
        else if (itemLine.p1().y() == itemLine.p2().y())        //当拖动的线和相连的线不在同一直线上时 不需要增加新的直线 只需改变长度
        {
            if (fabs(itemLine.p1().x() - m_pfClickPoint.x()) < 2)
            {
                pLineItem->setLine(itemLine.p2().x(), itemLine.p2().y(), pfNewPoint.x(), itemLine.p2().y());
            }
            else
            {
                pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
            }

            //处理起始或结束 的线
            CConnectionLineItem *pLinkItem = NULL;

            //判断是否是起始或结束的线
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
                if (fabs(itemLine.p1().x() - pfNewPoint.x()) > fabs(itemLine.p2().x() - pfNewPoint.x()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
                }

                return;

            }
            else if (pLineItem->GetLinkLineItem2() != NULL)
            {
                if (fabs(itemLine.p1().x() - pfNewPoint.x()) > fabs(itemLine.p2().x() - pfNewPoint.x()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
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
                pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
            }
            else if (tLine.p1() == itemLine.p2() || tLine.p2() == itemLine.p2())
            {
                //p2为不动点
                pLineItem->setLine(itemLine.p2().x(), itemLine.p1().y(), pfNewPoint.x(), itemLine.p1().y());
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
            if (fabs(itemLine.p1().x() - this->line().p1().x()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(), itemLine.p1().y(), itemLine.p1().x(), pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                //
                UpdateLinkRelationship(pLineItem, item0, this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p2().x() - this->line().p2().x()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p2().x(), itemLine.p2().y(), itemLine.p2().x(), pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                //
                UpdateLinkRelationship(pLineItem, item0, this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p1().x() - this->line().p2().x()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p1().x(), itemLine.p1().y(), itemLine.p1().x(), pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                //
                UpdateLinkRelationship(pLineItem, item0, this);
                this->scene()->addItem(item0);

            }
            else if (fabs(itemLine.p2().x() - this->line().p1().x()) == 0)   //当拖动的线和相连的线是同一方向时 需要增加一条线
            {
                CConnectionLineItem *item0 = new CConnectionLineItem(itemLine.p2().x(), itemLine.p2().y(), itemLine.p2().x(), pfNewPoint.y());
                item0->SetOwnRounter(m_polyLine);
                //
                UpdateLinkRelationship(pLineItem, item0, this);
                this->scene()->addItem(item0);
            }
            else
            {
                //donothing
            }
        }
        else if (itemLine.p1().x() == itemLine.p2().x())  //当拖动的线和相连的线不在同一直线上时 不需要增加新的直线 只需改变长度
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
                if (fabs(itemLine.p1().y() - pfNewPoint.y()) > fabs(itemLine.p2().y() - pfNewPoint.y()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), itemLine.p1().x(), pfNewPoint.y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(), itemLine.p2().y(), itemLine.p2().x(), pfNewPoint.y());
                }
            }
            else if (pLineItem->GetLinkLineItem2() != NULL)
            {
                if (fabs(itemLine.p1().y() - pfNewPoint.y()) > fabs(itemLine.p2().y() - pfNewPoint.y()))
                {
                    //取p1
                    pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), itemLine.p1().x(), pfNewPoint.y());
                }
                else
                {
                    pLineItem->setLine(itemLine.p2().x(), itemLine.p2().y(), itemLine.p2().x(), pfNewPoint.y());
                }
            }
            else
            {

            }

            if (pLinkItem == NULL)
            {
                return;
            }

            //起始或结束线的处理
            QLineF tLine = pLinkItem->line();

            if (tLine.p1() == itemLine.p1() || tLine.p2() == itemLine.p1())
            {
                //p1为不动点
                pLineItem->setLine(itemLine.p1().x(), itemLine.p1().y(), itemLine.p1().x(), pfNewPoint.y());
            }
            else if (tLine.p1() == itemLine.p2() || tLine.p2() == itemLine.p2())
            {
                //p2为不动点
                pLineItem->setLine(itemLine.p1().x(), itemLine.p2().y(), itemLine.p1().x(), pfNewPoint.y());
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

const QPointF& CConnectionLineItem::GetFixPoint() const
{
    return m_pfFixPoint;
}

/*! \fn CConnectionLineItem::UpdateLinkRelationship(CConnectionLineItem *pItemOne, CConnectionLineItem *pNewItem, CConnectionLineItem *pthisItem)
*********************************************************************************************************
** \brief CConnectionLineItem::UpdateLinkRelationship(CConnectionLineItem *pItemOne, CConnectionLineItem *pNewItem, CConnectionLineItem *pthisItem)
** \details    更新直线之间的连接关系
** \param      pItemOne
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::UpdateLinkRelationship(CConnectionLineItem *pItemOne, CConnectionLineItem *pNewItem, CConnectionLineItem *pthisItem)
{
    Q_ASSERT(pItemOne);
    if (pItemOne == nullptr)
    {
        return;
    }

    Q_ASSERT(pNewItem);
    if (pNewItem == nullptr)
    {
        return;
    }

    Q_ASSERT(pthisItem);
    if (pthisItem == nullptr)
    {
        return;
    }
    //更新直线的关系
    m_polyLine->InsertLineItemToVector(pItemOne, pNewItem, pthisItem);
}

void CConnectionLineItem::SetUnMoveable()
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    setAcceptHoverEvents(false);
}

/*! \fn CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
** \details    点击操作
** \param      
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //改变颜色
        //setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        m_cursor->setShape(Qt::ClosedHandCursor);

        this->setCursor(*m_cursor);
        //记录起始地址
        m_pfClickPoint = event->scenePos();
    }
    else if (event->button() == Qt::RightButton)
    {
        //删除
        QMenu *pMenu = new QMenu;
        QAction *pDeleteAct = new QAction(QObject::tr("Delete  "), this);
        connect(pDeleteAct, SIGNAL(triggered()), this, SLOT(Slot_deleteAct()));

        pMenu->addAction(pDeleteAct);
        pMenu->exec(QCursor::pos());
        pMenu->deleteLater();

    }

    m_pfMoveBegin = event->scenePos();
    m_pfMoveEnd = event->scenePos();

    QGraphicsLineItem::mousePressEvent(event);
}

/*! \fn CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CConnectionLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
** \details    点击操作
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //拖动
    m_pfMoveEnd = event->scenePos();
    //拖动距离
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
        if (fabs(pfDis.x()) < 1)
        {

            return;
        }
        //只能在x轴方向上移动
        QPointF pfEvent = event->scenePos();
        event->setScenePos(QPointF(pfEvent.x(), m_pfClickPoint.y()));
        this->setLine(event->scenePos().x(), this->line().y1(), event->scenePos().x(), this->line().y2());

    }
    else if (m_pfBgin.y() == m_pfEnd.y())
    {
        if (fabs(pfDis.y()) < 1)
        {
            return;
        }

        //只能在y周方向上移动
        QPointF pfEvent = event->scenePos();
        event->setScenePos(QPointF(m_pfClickPoint.x(), pfEvent.y()));
        this->setLine(this->line().x1(), event->scenePos().y(), this->line().x2(), event->scenePos().y());
    }
    else
    {
        return;
    }
    // 
    //当长度太小时不做处理
    UpdateLinkLineItem(this->GetLinkLineItem1(), event->scenePos());
    UpdateLinkLineItem(this->GetLinkLineItem2(), event->scenePos());


    //QGraphicsLineItem::mouseMoveEvent(event);
}

/*! \fn CConnectionLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CConnectionLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
** \details    鼠标释放事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CConnectionLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_bMoveFalg = true;
    setPen(QPen());
    m_cursor->setShape(Qt::OpenHandCursor);
    this->setCursor(*m_cursor);
    QGraphicsLineItem::mouseReleaseEvent(event);

    //更新所有的点
    m_polyLine->UpdateLinesFromLink();
}

void CConnectionLineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsLineItem::hoverMoveEvent(event);
}

void CConnectionLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    this->setCursor(*m_cursor);

    setPen(QPen());
    QGraphicsLineItem::hoverLeaveEvent(event);
}

void CConnectionLineItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::OpenHandCursor);
    this->setCursor(*m_cursor);

    SetLineColor(QColor(255, 0, 0));

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
    Q_ASSERT(pRounter);
    if (pRounter == nullptr)
    {
        return;
    }
    m_polyLine = pRounter;
}

/*! \fn CConnectionLineItem::CheckLinePosition(const QPointF& pfDis)
*********************************************************************************************************
** \brief CConnectionLineItem::CheckLinePosition(const QPointF& pfDis)
** \details    检查移动直线所在位置是否合法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
bool CConnectionLineItem::CheckLinePosition(const QPointF& pfDis)
{
    int nXlength = pfDis.x() / fabs(pfDis.x());
    int nYlength = pfDis.y() / fabs(pfDis.y());

    if (m_pfBgin.x() == m_pfEnd.x())
    {
        //x不变
        QGraphicsItem *pItem1 = this->scene()->itemAt(this->line().p1().x() + nXlength * 14, this->line().p1().y(), QTransform());

        if (pItem1 != nullptr && pItem1->type() == OBJEC_TITEM)
        {
            return false;
        }

        QGraphicsItem *pItem2 = this->scene()->itemAt(this->line().p2().x() + nXlength * 14, this->line().p2().y(), QTransform());
        if (pItem2 != nullptr && pItem2->type() == OBJEC_TITEM)
        {
            return false;
        }

        QPointF pfMid = QPointF((this->line().p1().x() + this->line().p2().x()) / 2, (this->line().p1().y() + this->line().p2().y()) / 2);
        QGraphicsItem *pItem3 = this->scene()->itemAt(pfMid.x() + nXlength * 14, pfMid.y(), QTransform());

        if (pItem3 != nullptr && pItem3->type() == OBJEC_TITEM)
        {
            return false;
        }
    }
    else if (m_pfBgin.y() == m_pfEnd.y())
    {
        //x不变
        QGraphicsItem *pItem1 = this->scene()->itemAt(this->line().p1().x(), this->line().p1().y() + nYlength * 14, QTransform());

        if (pItem1 != nullptr && pItem1->type() == OBJEC_TITEM)
        {
            return false;
        }

        QGraphicsItem *pItem2 = this->scene()->itemAt(this->line().p2().x(), this->line().p2().y() + nYlength * 14, QTransform());
        if (pItem2 != nullptr && pItem2->type() == OBJEC_TITEM)
        {
            return false;
        }

        QPointF pfMid = QPointF((this->line().p1().x() + this->line().p2().x()) / 2, (this->line().p1().y() + this->line().p2().y()) / 2);
        QGraphicsItem *pItem3 = this->scene()->itemAt(pfMid.x(), pfMid.y() + nYlength * 14, QTransform());

        if (pItem3 != nullptr && pItem3->type() == OBJEC_TITEM)
        {
            return false;
        }
    }

    //     m_polyLine->UpdateLinesFromLink();

    return true;
}

void CConnectionLineItem::SetLineColor(const QColor& lineColor)
{
    QPen tPen(lineColor);
    tPen.setWidth(10);
    setPen(tPen);

    update();
}

