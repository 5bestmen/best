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

//注释类
class CCommentItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    CCommentItem();
    ~CCommentItem();
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QPainterPath shape() const;
    void ResetRect(const QRectF &rect);
    //获取rect
    const QRectF & GetItemRect();
    int GetTextType();
public slots:
    void Slot_DeleteAct();
signals:
    void Signal_DeleteItem();
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
