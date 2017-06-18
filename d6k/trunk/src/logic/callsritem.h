#ifndef CALLSRITEM_H
#define CALLSRITEM_H

#include "fdborderbaseitem.h"

//调用子程序类
class CLogicPropertyWgt;
class CCallSRItem : public CFdbOrderBaseItem
{
    Q_OBJECT

public:
    enum ITEMNUMS
    {
        //最多输入端子数
        MAX_INPUTN_NUM = 1,
        //最多输出端子数
        MAX_OUTPUT_NUM = 0    
    };

    CCallSRItem(int nItemType, int nCreateNum = 0);
    ~CCallSRItem();

    //判断当前item类型
    virtual QString AnalyseItemType(int nType);
    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


private:
    CLogicPropertyWgt *m_pPropertyWgt;

};

#endif // CALLSRITEM_H
