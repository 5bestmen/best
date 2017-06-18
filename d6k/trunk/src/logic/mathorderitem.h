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
    //��ʼ����������
    void InitInOutItems();
    //�жϵ�ǰitem����
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //�߼��ж�
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //��ֵ
    void AnalyseMoveMethod();
    //�ӷ� 
    void AnalyseAddMethod();
    //����
    void AnalyseSubMethod();
    //�˷�'
    void AnalyseMulMethod();
    //����
    void AnalyseDivMethod();
    //����
    void AnalyseModMethod();
    //ƽ����
    void AnalyseSqrtMethod();
    //abs
    void AnalyseAbsMethod();
    //ʮΪ�׵Ķ���
    void AnalyseLogMethod();
    //��Ȼ����
    void AnalyseLnMethod();
    //ָ��
    void AnalyseExpMethod();
    //����
    void AnalyseSinMethod();
    //����
    void AnalyseCosMethod();
    //����
    void AnalyseTanMethod();
    //������
    void AnalyseASinMethod();
    //������
    void AnalyseACosMethod();
    //������
    void AnalyseATanMethod();
    //���ֵ
    void AnalyseAmaxMethod();
    //��Сֵ
    void AnalyseAMinMethod();
private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //��ǰָ������
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //���ֵ
    int m_nItemOutValue;    
};

#endif // MATHORDERITEM_H
