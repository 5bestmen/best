#ifndef FDBITEMCHOOSEWGT_H
#define FDBITEMCHOOSEWGT_H

#include <QTreeWidget>

class CFdbItemChooseWgt : public QTreeWidget
{
    Q_OBJECT

public:
    CFdbItemChooseWgt(QWidget *parent=0);
    ~CFdbItemChooseWgt();

    //����PLCitem
    void CreateFdbTreeItems();
    //����λָ��
    void CreateBitChildItems(QTreeWidgetItem *pBitOrderItem);
    //�ƶ�ָ��
    void CreateMoveChildItems(QTreeWidgetItem *pMoveOrderItem);
    //��ѧָ��
    void CreateMathChildItems(QTreeWidgetItem *pMathOrderItem);
    //�Ƚ�ָ��
    void CreateCompareChildItems(QTreeWidgetItem *pCompareOrderItem);
    //�߼�����
    void CreateHighLevelChildItems(QTreeWidgetItem *pHighOrderItem);
    //��ʱ������
    void CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *mouseEvent);


private:
    
};

#endif // FDBITEMCHOOSEWGT_H
