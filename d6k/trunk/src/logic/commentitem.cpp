/*! @file
<PRE>
********************************************************************************
模块名       :  注释逻辑块
文件名       :  commentitem.cpp
文件实现功能 :
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

/*! \fn CCommentItem::CCommentItem()
*********************************************************************************************************
** \brief CCommentItem::CCommentItem()
** \details 构造函数
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CCommentItem::CCommentItem()
{
    m_rect = QRectF(0, 0, 200, 30);
    m_nType = GRAPHICS_COMMENT;

    //注释放在最底层,被连接线 和 逻辑块覆盖
    setZValue(1);

    setAcceptDrops(true);
    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);

    m_bScale = false;
    //用于控制鼠标的样式
    m_cursor = new QCursor;

    //描述鼠标拖动放大缩小的方向
    //上 下 左 右  左上 右上 左下  右下
    m_nDirection = 0;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setTextWidth(m_rect.width());
    this->setPlainText(tr("starting transportation Belt"));
}

/*! \fn CCommentItem::ResetRect(const QRectF &rect)
*********************************************************************************************************
** \brief CCommentItem::ResetRect(const QRectF &rect)
** \details 改变注释框大小
** \param rect:新的大小
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::ResetRect(const QRectF &rect)
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
}

QPainterPath CCommentItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);
    return path;
}

/*! \fn CCommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
*********************************************************************************************************
** \brief CCommentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
** \details 重绘
** \param 
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CCommentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
** \details 鼠标进入
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    //鼠标所在位置 
    QPointF pos1 = event->scenePos();
    //记录 上下左右 四个点 相对于scene的坐标
    //左上角点
    QPointF lt = this->scenePos() + QPointF(m_rect.x(), m_rect.y());
    //左下角点
    QPointF lb = this->scenePos() + QPointF(m_rect.x(), m_rect.y() + m_rect.height());
    //右上角点
    QPointF rt = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y());
    //右下角点
    QPointF rb = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height());

    if ((pos1.x() <= lt.x() + 10 && pos1.y() <= lt.y() + 10) || (pos1.x() >= rb.x() - 10 && pos1.y() >= rb.y() - 10))
    {
        //左上角  右下角
        m_cursor->setShape(Qt::SizeFDiagCursor);
        if (pos1.x() <= lt.x() + 10)
        {
            //左上角
            m_nDirection = 8;
        }
        else
        {
            //右下角
            m_nDirection = 4;
        }
    }
    else if ((pos1.x() <= lb.x() + 10 && pos1.y() >= lb.y() - 10) || (pos1.x() >= rt.x() - 10 && pos1.y() <= rt.y() + 10))
    {
        //右上角  左下角
        m_cursor->setShape(Qt::SizeBDiagCursor);
        if (pos1.x() <= lb.x() + 10)
        {
            //左下角
            m_nDirection = 6;
        }
        else
        {
            //右上角
            m_nDirection = 2;
        }
    }
    else if ((pos1.x() <= lt.x() + 10 || pos1.x() >= rt.x() - 10) && (pos1.y() <= lb.y() && pos1.y() >= lt.y()))
    {
        //左  右
        m_cursor->setShape(Qt::SizeHorCursor);
        if (pos1.x() <= lt.x() + 10)
        {
            //左
            m_nDirection = 7;
        }
        else
        {
            //右
            m_nDirection = 3;
        }
    }
    else if ((pos1.y() <= lt.y() + 10 || pos1.y() >= lb.y() - 10) && (pos1.x() >= lt.x() && pos1.x() <= rt.x()))
    {
        //上下
        m_cursor->setShape(Qt::SizeVerCursor);
        if (pos1.y() <= lt.y() + 10)
        {
            //上
            m_nDirection = 1;
        }
        else
        {
            //下
            m_nDirection = 5;
        }
    }
    else
    {
        m_nDirection = 0;
        m_bScale = false;
        m_cursor->setShape(Qt::ArrowCursor);
    }

    //改变鼠标样式
    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

/*! \fn CCommentItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
** \details 鼠标移动事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //鼠标所在位置 
    QPointF pos1 = event->scenePos();
    //记录 上下左右 四个点 相对于scene的坐标
    //左上角点
    QPointF lt = this->scenePos() + QPointF(m_rect.x(), m_rect.y());
    //左下角点
    QPointF lb = this->scenePos() + QPointF(m_rect.x(), m_rect.y() + m_rect.height());
    //右上角点
    QPointF rt = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y());
    //右下角点
    QPointF rb = this->scenePos() + QPointF(m_rect.x() + m_rect.width(), m_rect.y() + m_rect.height());

    //状态标识  是移动位置 还是改变大小
    m_bScale = true;

    if ((pos1.x() <= lt.x() + 10 && pos1.y() <= lt.y() + 10) || (pos1.x() >= rb.x() - 10 && pos1.y() >= rb.y() - 10))
    {
        //左上角  右下角
        m_cursor->setShape(Qt::SizeFDiagCursor);
        if (pos1.x() <= lt.x() + 10)
        {
            //左上角
            m_nDirection = 8;
        }
        else
        {
            //右下角
            m_nDirection = 4;
        }
    }
    else if ((pos1.x() <= lb.x() + 10 && pos1.y() >= lb.y() - 10) || (pos1.x() >= rt.x() - 10 && pos1.y() <= rt.y() + 10))
    {
        //右上角  左下角
        m_cursor->setShape(Qt::SizeBDiagCursor);
        if (pos1.x() <= lb.x() + 10)
        {
            //左下角
            m_nDirection = 6;
        }
        else
        {
            //右上角
            m_nDirection = 2;
        }
    }
    else if ((pos1.x() <= lt.x() + 10 || pos1.x() >= rt.x() - 10) && (pos1.y() <= lb.y() && pos1.y() >= lt.y()))
    {
        //左  右
        m_cursor->setShape(Qt::SizeHorCursor);
        if (pos1.x() <= lt.x() + 10)
        {
            //左
            m_nDirection = 7;
        }
        else
        {
            //右
            m_nDirection = 3;
        }
    }
    else if ((pos1.y() <= lt.y() + 10 || pos1.y() >= lb.y() - 10) && (pos1.x() >= lt.x() && pos1.x() <= rt.x()))
    {
        //上下
        m_cursor->setShape(Qt::SizeVerCursor);
        if (pos1.y() <= lt.y() + 10)
        {
            //上
            m_nDirection = 1;
        }
        else
        {
            //下
            m_nDirection = 5;
        }
    }
    else
    {
        m_nDirection = 0;
        //移动位置状态
        m_bScale = false;
        m_cursor->setShape(Qt::ArrowCursor);
    }

    this->setCursor(*m_cursor);
    update();
    QGraphicsItem::hoverMoveEvent(event);
}

/*! \fn CCommentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
** \details 鼠标离开事件
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_cursor->setShape(Qt::ArrowCursor);
    //状态标识  是移动位置 还是改变大小
    m_bScale = false;
    this->setCursor(*m_cursor);

    //非编辑状态
    setTextInteractionFlags(Qt::NoTextInteraction);

    update();
    QGraphicsItem::hoverLeaveEvent(event);
    //更新
    //更新
    CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

    //保存数据
    if (pScene != nullptr)
    {
        pScene->UpdateOperate();
    }
}

/*! \fn CCommentItem::mouseDoubleClickEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::mouseDoubleClickEvent(QGraphicsSceneHoverEvent *event)
** \details 双击进入编辑状态
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //编辑状态
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsItem::mouseDoubleClickEvent(event);
}

/*! \fn CCommentItem::mouseDoubleClickEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::mousePressEvent(QGraphicsSceneHoverEvent *event)
** \details 双击进入编辑状态
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //记录鼠标点击起始位置   拖动改变大小时用
        m_pfStart = event->scenePos();
    }
    else if (event->button() == Qt::RightButton)
    {
        //删除
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

/*! \fn CCommentItem::mouseMoveEvent(QGraphicsSceneHoverEvent *event)
*********************************************************************************************************
** \brief CCommentItem::mouseMoveEvent(QGraphicsSceneHoverEvent *event)
** \details 鼠标拖动
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //判断是否处于拖动状态
    if (m_bScale && m_nDirection!=0)
    {
        QPointF dis;
        //终点
        m_pfEnd = event->scenePos();
        dis = m_pfEnd - m_pfStart;
        m_pfStart = m_pfEnd;

        //根据方位改变大小
        switch (m_nDirection)
        {
        case 1:   //拖动上面一条线
        {
            //改变后的大小不能小于 高30  长200
            if ((!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200)) || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                this->setPos(this->pos().x(),m_pfEnd.y());
            }
            break;

        }
        case 2:   //拖动右上角的线
        {
            //改变后的大小不能小于 高30  长200
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                this->setPos(this->pos().x(), m_pfEnd.y());
            }
            break;
        }
        case 3:   //拖动右 边线
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 )
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height());
                this->ResetRect(tem);
                this->setPos(this->pos().x(), this->pos().y());
            }
            break;
        }
        case 4:   //拖动右下角线
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.x()>0 || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() + dis.x(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                this->setPos(this->pos().x(), this->pos().y()+0.1);
            }
            break;
        }
        case 5:   //拖动  下线
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                this->setPos(this->pos().x(), this->pos().y()+0.1);
            }
            break;
        }
        case 6:   //拖动左下线
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 || dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height() + dis.y());
                this->ResetRect(tem);
                this->setPos(m_pfEnd.x(), this->pos().y());
            }
            break;
        }
        case 7:    //拖动左线
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 )
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height());
                this->ResetRect(tem);
                this->setPos(m_pfEnd.x(), this->pos().y());
            }
            break;
        }
        case 8:   //拖动左上角
        {
            if (!(m_rect.height() + dis.y() < 30 || m_rect.width() + dis.x() < 200) || -dis.x()>0 || -dis.y()>0)
            {
                QRectF tem = QRectF(0, 0, m_rect.width() - dis.x(), m_rect.height() - dis.y());
                this->ResetRect(tem);
                this->setPos(m_pfEnd.x(), m_pfEnd.y());
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

/*! \fn CCommentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CCommentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标释放
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CCommentItem::Slot_DeleteAct()
*********************************************************************************************************
** \brief CCommentItem::Slot_DeleteAct()
** \details 删除对象
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCommentItem::Slot_DeleteAct()
{
     this->deleteLater();

      //更新  数据模型
     CFdbOrderScene *pScene = qobject_cast<CFdbOrderScene*>(this->scene());

     if (pScene != nullptr)
     {
         pScene->removeItem(this);
         pScene->UpdateOperate();
     }
}

const QRectF & CCommentItem::GetItemRect()
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
