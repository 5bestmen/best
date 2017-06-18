#ifndef CALLSRITEM_H
#define CALLSRITEM_H

#include "fdborderbaseitem.h"

//�����ӳ�����
class CLogicPropertyWgt;
class CCallSRItem : public CFdbOrderBaseItem
{
    Q_OBJECT

public:
    enum ITEMNUMS
    {
        //������������
        MAX_INPUTN_NUM = 1,
        //������������
        MAX_OUTPUT_NUM = 0    
    };

    CCallSRItem(int nItemType, int nCreateNum = 0);
    ~CCallSRItem();

    //�жϵ�ǰitem����
    virtual QString AnalyseItemType(int nType);
    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


private:
    CLogicPropertyWgt *m_pPropertyWgt;

};

#endif // CALLSRITEM_H
