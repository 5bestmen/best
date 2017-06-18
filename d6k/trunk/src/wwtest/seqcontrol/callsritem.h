#ifndef CALLSRITEM_H
#define CALLSRITEM_H

#include "fdborderbaseitem.h"

class CLogicPropertyWgt;
class CCallSRItem : public CFdbOrderBaseItem
{
    Q_OBJECT

public:
    enum ITEMNUMS
    {
        MAXINPUTNUMX = 1,
        MAXOUTPUTNUM = 0    
    };
    //��ȡ�ӳ����б�
    QStringList GetChildNames();
    CCallSRItem(int nItemType, int nCreateNum = 0);
    ~CCallSRItem();

    //�жϵ�ǰitem����
    virtual QString AnalyseItemType(int nType);
    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


private:
    CLogicPropertyWgt *m_pPropertyWgt;

};

#endif // CALLSRITEM_H
