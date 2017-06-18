#ifndef FDBITEMCHOOSEWGT_H
#define FDBITEMCHOOSEWGT_H

#include <QTreeWidget>

class CFdbItemChooseWgt : public QTreeWidget
{
    Q_OBJECT

public:
    CFdbItemChooseWgt(QWidget *parent=0);
    ~CFdbItemChooseWgt();

    //设置PLCitem
    void CreateFdbTreeItems();
    //设置位指令
    void CreateBitChildItems(QTreeWidgetItem *pBitOrderItem);
    //移动指令
    void CreateMoveChildItems(QTreeWidgetItem *pMoveOrderItem);
    //数学指令
    void CreateMathChildItems(QTreeWidgetItem *pMathOrderItem);
    //比较指令
    void CreateCompareChildItems(QTreeWidgetItem *pCompareOrderItem);
    //高级功能
    void CreateHighLevelChildItems(QTreeWidgetItem *pHighOrderItem);
    //定时器功能
    void CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);


private:
    
};

#endif // FDBITEMCHOOSEWGT_H
