#ifndef FDBORDERBASEITEM_H
#define FDBORDERBASEITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>
#include <memory>
#include "seqdefine.h"

class CConnectionRouter;
class QCursor;
class QTimer;
class CBinProperty;

class CFdbOrderBaseItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:

    CFdbOrderBaseItem();
    ~CFdbOrderBaseItem();
public:
    //设置item类型
    void SetItemType(int nItemType);
    //设置createitem
    void SetCreateNum(int nCreateNum);
    //获取item  rect
    const QRectF& GetItemRect();
    //获取创建编号
    int GetCreateNum();
    //获取执行编号
    int GetIndexNum();
    //获取item颜色
    const QColor & GetItemColor();
    //获取item类型
    const QString& GetItemType();
    //获取注释对象
    QGraphicsTextItem *GetCommentItem();

    void SetItemStatus(int nStatus);
    //
    const std::vector<std::shared_ptr<CBinProperty>> GetInputPros();

    const std::vector<std::shared_ptr<CBinProperty>> GetOutPutPros();

    //设置输入端子
    void SetInPutIndex(int nInputIndex);
    //设置输出端子
    void SetOutPutIndex(int nOutputIndex);
    //划选中框
    void AddHoverRect(QPainter *painter);
    //分析位置
    virtual void AnalysePointPosition(const QPointF &pointCurrent);
    //通过scene的坐标来定位item的坐标
    void AnalyseSencePoint(const QPointF &pScene);
    //通过item坐标找出一个正确的点
    virtual void GetLinePoint(const QPointF &pItem);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);
    //item返回值
    virtual int GetLogicValue();
    //
    virtual void AnalseBitInput();
    //更新当前数据
    void UpdateCurrentItemInfo();
    //获取output pins信息
    const QStringList & GetOutputPins();
    //
    void SetComment(const QString &strComment);
public:
    //判断当前item类型
    virtual  QString  AnalyseItemType(int nType);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;

    void RemovePloyLine(CConnectionRouter *ployline);
    void RemovePloyLines();
    void AddPloyLine(CConnectionRouter *ployline);
    //获取长度
    int GetPointWidth() const;
    //获取高度
    int getPointHeight() const;
    void ResetRect(QRectF rect);

    virtual int type() const{ return OBJEC_TITEM; }
    //获取连线的状态
    int GetItemIOStatus();
    //设置执行顺序编号
    void SetIndexNumber(int nIndex);
    //获取输入编号
    int GetInputIndex();
    //获取输出编号
    int GetOutputIndex();
    //获取输出list
    QMap<int, QList<CFdbOrderBaseItem*> > GetOutPutList();
    //获取输入的个数
    int GetInputNums();
    //获取输出个数
    int GetOutputNums();
    //增加输入item
    void InsertInputItem(int nIndex, CFdbOrderBaseItem* mapItem);
    //增加输出item
    void InsertOuputItem(int nIndex,CFdbOrderBaseItem* pItem);
    //删除输入端item
    void DeleteInputItem(CFdbOrderBaseItem* pItem);
    //删除单个输入端item
    void DeleteOneInputItem(CFdbOrderBaseItem* pItem, int nIndex);
    //删除output
    void DeleteOutputItem(CFdbOrderBaseItem* pItem);
    //删除一个output
    void DeleteOneOutputItem(CFdbOrderBaseItem* pItem,int nIndex);
    //pin对应关系插入
    void InsertPins(int nEndIndex,int nBeginIndex);
    //获取前向数据
    QMap<int, CFdbOrderBaseItem*> GetInputItems();
    //设置最大输入高度
    void SetMaxInputHeight(int nMaxHeight);
    //获取输出连线
    QList<QPointF> GetInputLines(int nInputIndex);
    //
    //获取相对于scene中
    QRectF GetSceneRectf();
    //设置入度个数
    void SetInputNum();
    //减
    int MinusInputNum();
    //
    void MoveItemFromKey(double disHeight,double disWidth);
    //
    bool GetEnoFlag();
    //显示eno信息
    void ShowEnoItems();
    //关闭eno信息
    void CloseEnoItems();
    //获取注释信息
    const QString& GetCommentData();
    //获取itemtype编号 
    int GetItemTypNum();
    //设置输入数量最大值
    void SetInputMaxNum(int nIndex);
    //获取输入数量最大值
    int GetInputMaxNum();
    //设置输出数量最大值
    void SetOutPutMaxNum(int nIndex);
    //获取输出数量最大值
    int GetOutPutMaxNum();
    //设置输入管脚名称
    void SetInputPinNames(const QStringList &lstNames);
    //设置输出管脚名称
    void SetOutputPinNames(const QStringList &lstNames);
    //获取map
    QMap<int, int> GetBinMapInfo();
    //remove pin in
    void RemoveOneInputPin(int nPinIndex);
    //add bin text
    void AddTextBinInfo();
    //设置状态颜色
    void SetStatusColor(const QColor &statusColor);
    //
    QList<CConnectionRouter*> GetAllLines();
    //
    void SetCurrentInputNum(int nInputNum);
    //
    int GetCurrentInpusNum();
    //设置EN信息
    void SetEnData(int nEnData);
    int GetEnData();
    //设置ENO
    void SetEnoData(int nEnoData);
    int GetEnoData();

public slots:
    void Slot_DeleteAct();
    //
    void Slot_UpdateComment(const QString &strComment);
    //更新eno信息
    void Slot_UpdateEnoInfo(int nFlag);
	//更新绑定信息值
	void Slot_updateBindValueInfo(QMap<int,QString>& mapBindValue);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
public:
    //鼠标所在输入点处
    int m_nInputIndex;
    //鼠标所在输出处
    int m_nOutputIndex;
    //输入最大高度
    int m_nInsertMaxHeight;
    //输出最大高度
    int m_nOutMaxHeight;
    //当前选中点的长度
    int  m_nCurrentWidth;
    //当前选中点的高度
    int m_nCurrentHeight;
    //连线的状态
    int m_nItemStatus;
    //设置输出信息
    void SetOutputData(int nOutPinNum,int nValue);
    //获取输出信息
    QMap<int,int> GetOutputData();

private:
    QColor m_ItemColor;
    //item类型 AND OR ....
    QString m_strItemType;
    //线集合
    QList<CConnectionRouter*>  m_polyLines;
    //大小
    QRectF m_rect;
    QCursor* m_cursor;
    //缩放  上拉1  下啦2
    int m_nChangeSize;
    //拉动起始点
    QPointF m_nMoveBge;
    //拉动结束点
    QPointF m_nMoveEnd;
    bool m_bScanl;
    //是否更改
    bool m_bChangeFlag;
    //鼠标点击坐标
    QPointF m_pfClickPoint;
    //keyress
    QPointF m_pfKeyPoint;
    //执行编号
    int m_nIndexNumber;
    //创建编号
    int m_nCreateNum;
    //前   输入编号和item
    QMap<int, CFdbOrderBaseItem*>  m_mapPreInfos;
    //后  输出编号和item
    QMap<int, QList<CFdbOrderBaseItem*>> m_mapNextInfos;
    //入度
    int m_nInputNums;
    //eno属性
    bool m_bEnoFlag;
    //注释对象
    QGraphicsTextItem *m_pCommentTextItem;
    //是否显示eno items
    //注释内容
    QString m_strComment;
    //itemtype  num
    int m_nItemTypeNum;
    //最大值
    int m_nInputMaxNum;
    //output 最大值
    int m_nOutMaxNum;
    //输入端端子信息
    std::vector<std::shared_ptr<CBinProperty>> m_vInputPros;

    //输出端端子信息
    std::vector<std::shared_ptr<CBinProperty>> m_vOutputPros;

    //输入列表的名称
    QStringList m_lstInputPin;
    //输出列表名称
    QStringList m_lstOutputPin;
    //item最大高度
    int m_nItemMaxHeight;
    //输入端子->其他输出端子
    QMap<int, int> m_mapPins;
    //当前需要划的点
    QPointF m_pfPinText;
    //输入
    QMap<int, QGraphicsTextItem*> m_mapPinIndexText;
    //输出
    QMap<int, QGraphicsTextItem*> m_mapOutPinIndexText;
    //根据状态变换颜色
    QColor m_colorStatus;
    //输入端因子个数
    int m_nCurrentInputNums;
    //输出端因子个数
    int m_nCurrentOutputNums;
    //EN 信息 
    int m_nEnFlag;
    //ENO 信息
    int m_nEnoFlag;
    //输出端子  和  输出值
    QMap<int, int> m_mapOutput;
};

#endif // PLCORDERBASEITEM_H
