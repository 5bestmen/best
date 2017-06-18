#ifndef MYITEM_H
#define MYITEM_H
#include <QTimer>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QRectF>
#include <QPainterPath>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QObject>
#include "seqdefine.h"

class CCommentItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    CCommentItem();
    ~CCommentItem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void ResetRect(QRectF rect);
    //获取rect
    QRectF GetItemRect();
    int GetTextType();
public slots:
    void Slot_DeleteAct();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
private:
    QRectF m_rect;
    bool m_bScale;
    QCursor* m_cursor;
    QPointF m_pfStart;
    QPointF m_pfEnd;
    int m_nDirection;
    int m_nType;
    //输入框
//     QGraphicsTextItem *m_pTextItem;
};

#endif // MYITEM_H
