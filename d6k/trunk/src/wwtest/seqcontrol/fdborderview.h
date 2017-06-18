#ifndef CFDBORDERVIEW_H
#define CFDBORDERVIEW_H

#include <QGraphicsView>

class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;
class QMouseEvent;
class QPaintEvent;
class QCursor;
class CFdbOrderBaseItem;

class CFdbOrderView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CFdbOrderView(QGraphicsScene *scene,QWidget *parent = 0);
    ~CFdbOrderView();
    void UpdateItemsExecNum();
    //递归遍历
    void TravelItems(CFdbOrderBaseItem* pItem);
    //去掉删掉的item
    void DeleteRemoveItem(CFdbOrderBaseItem* pItem);
    //获取所有item
    QList<CFdbOrderBaseItem*> GetAllItems();
    //插入item
    void InsertOneItem(CFdbOrderBaseItem* pItem);
    //
    void ClearItems();
    //编号增加
    void AddCreateNum();
    //清空编号
    void ClearCreateNum();
    //获取编号
    int GetNextCreateNum();
    //根据执行顺序
    void UpdateItemList();

signals:
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);
    virtual void enterEvent(QEvent *event);
public slots:
    
    //virtual void mouseMoveEvent(QMouseEvent *event);
private:
    //鼠标
    QCursor *m_pCursor;
    QList<CFdbOrderBaseItem*> m_lstGraphicsItems;
    //记录一条链路的编号
    int m_nNodeIndex;
    //已经走过的item
    QList<CFdbOrderBaseItem*> m_lstEnterItems;
    //item编号
    int m_nItemIndex;
};

#endif // CPLCORDERVIEW_H
