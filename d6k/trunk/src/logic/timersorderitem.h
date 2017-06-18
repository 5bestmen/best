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
    //��ʼ����������
    void InitInOutItems();
    //�жϵ�ǰitem����
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //�߼��ж�
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
    //��ǰָ������
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //���ֵ
    int m_nItemOutValue;
    //��ǰ�����resetValue
    int m_nResetValue;
    //�ϴε�triggerֵ
    int m_nLastTrigger;

    //�ϴ�ctdu  cuֵ
    int m_nCtudUpTrigger;
    //��һ�ε�  ctdu  cdֵ
    int m_nCtudDownTrigger;
    //presetvalue
    int m_nCtudPresetValue;
};

#endif