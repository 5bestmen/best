#ifndef CCONNECTIONLINEITEM_H
#define CCONNECTIONLINEITEM_H
#include <QGraphicsLineItem>
#include <QPointF>
#include <QObject>
#include "seqdefine.h"

class QCursor;
class CConnectionRouter;

class CConnectionLineItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    CConnectionLineItem(qreal x1, qreal y1, qreal x2, qreal y2);
    //设置所属rounter
    void SetOwnRounter(CConnectionRouter *pRounter);
    ~CConnectionLineItem();
    //设置相连的线
    void SetLinkLineItem1(CConnectionLineItem *pLineItem);
    void SetLinkLineItem2(CConnectionLineItem *pLineItem);
    void UpdateLinkLineItem(CConnectionLineItem *pLineItem, QPointF pfNewPoint);
    CConnectionLineItem *GetLinkLineItem1() const;
    CConnectionLineItem *GetLinkLineItem2() const;
    void SetFixPoint(const QPointF &pfpoint);
    QPointF GetFixPoint() const;
    //更新链接关系
    void UpdateLinkRelationship(CConnectionLineItem *pItemOne,CConnectionLineItem *pNewItem,CConnectionLineItem *pthisItem);
    //设置不可编辑状态
    void SetUnMoveable();
    virtual int type() const{ return LINEITEM; }
    //检测直线的位置是否合法
    bool CheckLinePosition(QPointF pfDis);
    //设置颜色
    void SetLineColor(QColor lineColor);
public slots:
    void Slot_deleteAct();
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    //重新定义类型
private:
    QCursor* m_cursor;
    QPointF m_pfBgin;
    QPointF m_pfEnd;
    //鼠标点击的点
    QPointF m_pfClickPoint;
    //相连的两条线
    CConnectionLineItem *m_pLinkLineItem1;
    CConnectionLineItem *m_pLinkLineItem2;
    //保存增加的新直线
    QList<CConnectionLineItem*> m_lstAddNewItems;

    //直线管理
    CConnectionRouter *m_polyLine;
    QPointF m_pfFixPoint;
    QPointF m_pfMoveBegin;
    QPointF m_pfMoveEnd;
    bool m_bMoveFalg;

};

#endif // CCONNECTIONLINEITEM_H
