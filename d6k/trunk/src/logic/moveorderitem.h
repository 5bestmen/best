#ifndef MOVEORDERITEM_H
#define MOVEORDERITEM_H

#include "fdborderbaseitem.h"

class CLogicPropertyWgt;
class CMoveOrderItem : public CFdbOrderBaseItem
{
    Q_OBJECT

public:
    CMoveOrderItem(int nItemType, int nCreateNum = 0);
    ~CMoveOrderItem();
    //��ʼ����������
    void InitInOutItems();
    //�жϵ�ǰitem����
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //�߼��ж�
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //shl  ����
    void AnalyseShlMethod();
    //shr  ����
    void AnalyseShrMethod();
    //rol  ����
    void AnalyseRolMethod();
    //ror  ����
    void AnalyseRorMehtod();
    //����ѭ��λ��
    unsigned char MoveLeft(unsigned char uInput, unsigned uMoveNum);
    //����ѭ��λ��
    unsigned char MoveRight(unsigned char uInput, unsigned uMoveNum);

private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //��ǰָ������
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //���ֵ
    int m_nItemOutValue;
    
};

#endif // MOVEORDERITEM_H
