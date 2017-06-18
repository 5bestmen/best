#ifndef CCONNECTROUTER_H
#define CCONNECTROUTER_H

#include <QVector>
#include <QMap>
#include <QObject>
#include <QColor>
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
class CFdbOrderBaseItem;
class QGraphicsItem;
class QPointF;
class CConnectionLineItem;

struct CPolyPoints
{
    double top_x;
    double top_y;
    double buttom_x;
    double buttom_y;
};

class CConnectionRouter : public QObject
{
    Q_OBJECT
public:
    CConnectionRouter(CFdbOrderBaseItem *startItem, CFdbOrderBaseItem *endItem, QGraphicsScene *scene = 0);
    ~CConnectionRouter();
    //更新位置
    void UpdatePosition();
    //
    void GetPoint();
    //
    void GetNewPoint();
    //更新直线
    void UpdateAllLines();
    //计算点
    void AnalyseAllPoints();
    //开始和结束item
    CFdbOrderBaseItem *StartItem() const { return m_pStartItem; }
    CFdbOrderBaseItem *EndItem() const { return m_pEndItem; }
    //根据点集划线
    void DrawLineFromPoints();
    //
    void StepToEnd();
    void DeleteLinesPoints();
    //增加点集合
    void InsertLineItemToVector(CConnectionLineItem *pBeginItem, CConnectionLineItem *pMidItem, CConnectionLineItem *pEndItem);
    //设置结束点
    void SetEndInput(int nIndex);
    //设置开始点
    void SetBegiOutput(int nIndex);
    //获取结束点
    int GetEndInput();
    //
    void DeleteLinkInfo();
    //获取所有的点
    const QList<QPointF>& GetAllPoints();
    //重新设置所有的点
    void SetAllPoints(const QList<QPointF> &lstPoints);
    //根据linelink重新生成所有的点
    void UpdateLinesFromLink();
    //设置颜色
    void SetLineColor(const QColor &colorLine);

public slots:

private:
    //直线集合
    QVector<CConnectionLineItem*> m_pVectorLines;
    //关键点信息
    CPolyPoints m_SPoints;
    //起始item
    CFdbOrderBaseItem *m_pStartItem;
    //终止item
    CFdbOrderBaseItem *m_pEndItem;
    //起始width
    int m_nBeginWidth;
    //起始height
    int m_nBeginHeight;
    //终止width
    int m_nEndWidth;
    //终止height
    int m_nEndHeight;
    //场景
    QGraphicsScene *m_pScene;
    //找到所有的拐点
    QList<QPointF> m_pLstAllPoints;
    //改变后的起始点
    QPointF m_pfBeginPoint;
    //改变后的结束点
    QPointF m_pfEndPoint;
    //结束点编号
    int m_nEndEnterIndex;
    //开始点编号
    int m_nBeginEnterIndex;
    //颜色
    QColor m_LineColor;
};

#endif // CCONNECTROUTER_H
