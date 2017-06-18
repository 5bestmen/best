#ifndef MATHORDERITEM_H
#define MATHORDERITEM_H

#include "fdborderbaseitem.h"

class CLogicPropertyWgt;
class CMathOrderItem : public CFdbOrderBaseItem
{
    Q_OBJECT

public:
    CMathOrderItem(int nItemType, int nCreateNum = 0);
    ~CMathOrderItem();
    //初始化输入和输出
    void InitInOutItems();
    //判断当前item类型
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //逻辑判断
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //赋值
    void AnalyseMoveMethod();
    //加法 
    void AnalyseAddMethod();
    //减法
    void AnalyseSubMethod();
    //乘法'
    void AnalyseMulMethod();
    //除法
    void AnalyseDivMethod();
    //求余
    void AnalyseModMethod();
    //平方根
    void AnalyseSqrtMethod();
    //abs
    void AnalyseAbsMethod();
    //十为底的对数
    void AnalyseLogMethod();
    //自然对数
    void AnalyseLnMethod();
    //指数
    void AnalyseExpMethod();
    //正弦
    void AnalyseSinMethod();
    //余弦
    void AnalyseCosMethod();
    //正切
    void AnalyseTanMethod();
    //反正弦
    void AnalyseASinMethod();
    //反余弦
    void AnalyseACosMethod();
    //反正切
    void AnalyseATanMethod();
    //最大值
    void AnalyseAmaxMethod();
    //最小值
    void AnalyseAMinMethod();
private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //当前指令类型
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //输出值
    int m_nItemOutValue;    
};

#endif // MATHORDERITEM_H
