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
    //初始化输入和输出
    void InitInOutItems();
    //判断当前item类型
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //逻辑判断
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //shl  分析
    void AnalyseShlMethod();
    //shr  分析
    void AnalyseShrMethod();
    //rol  分析
    void AnalyseRolMethod();
    //ror  分析
    void AnalyseRorMehtod();
    //向左循环位移
    unsigned char MoveLeft(unsigned char uInput, unsigned uMoveNum);
    //向右循环位移
    unsigned char MoveRight(unsigned char uInput, unsigned uMoveNum);

private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //当前指令类型
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //输出值
    int m_nItemOutValue;
    
};

#endif // MOVEORDERITEM_H
