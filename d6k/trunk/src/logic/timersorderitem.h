#ifndef TIMERSORDERITEM_H
#define TIMERSORDERITEM_H

#include "fdborderbaseitem.h"

class CLogicPropertyWgt;
class CTimersOrderItem :public CFdbOrderBaseItem
{
    Q_OBJECT
public:
    CTimersOrderItem(int nItemType, int nCreateNum = 0);
    ~CTimersOrderItem();
    //初始化输入和输出
    void InitInOutItems();
    //判断当前item类型
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //逻辑判断
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //ctd
    void AnalyseCtdMethod();
    //ctu
    void AnalyseCtuMethod();
    //ctud
    void AnalyseCtudMethod();
    //top
    void AnalyseTopMethod();
    //ton
    void AnalyseTonMethod();
    //tp
    void AnalyseTpMethod();


private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //当前指令类型
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //输出值
    int m_nItemOutValue;
    //当前输入的resetValue
    int m_nResetValue;
    //上次的trigger值
    int m_nLastTrigger;

    //上次ctdu  cu值
    int m_nCtudUpTrigger;
    //上一次的  ctdu  cd值
    int m_nCtudDownTrigger;
    //presetvalue
    int m_nCtudPresetValue;
};

#endif