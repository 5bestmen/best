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

    //��ʼ����������
    void InitInOutItems();
    //�жϵ�ǰitem����
    virtual  QString  AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //�߼��ж�
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
    //��ǰָ������
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //���ֵ
    int m_nItemOutValue;
};

#endif // COMPAREORDERITEM_H
