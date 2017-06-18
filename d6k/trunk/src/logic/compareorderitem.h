#ifndef COMPAREORDERITEM_H
#define COMPAREORDERITEM_H

#include "fdborderbaseitem.h"

class CLogicPropertyWgt;
class CCompareOrderItem : public CFdbOrderBaseItem
{
    Q_OBJECT
public:
    CCompareOrderItem(int nItemType, int nCreateNum = 0);
    ~CCompareOrderItem();

    //初始化输入和输出
    void InitInOutItems();
    //判断当前item类型
    virtual  QString  AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //逻辑判断
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //eq
    void AnalyseEqMethod();
    //ge
    void AnalyseGeMethod();
    //gt
    void AnalyseGtMethod();
    //le
    void AnalyseLeMethod();
    //lt
    void AnalyseLtMethod();
    //ne
    void AnalyseNeMethod();


private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //当前指令类型
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //输出值
    int m_nItemOutValue;
};

#endif // COMPAREORDERITEM_H
