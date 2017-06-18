#ifndef CFUNBLOCKIDIAGRAMITEM
#define CFUNBLOCKIDIAGRAMITEM

#include "fdborderbaseitem.h"



// class CInputItem
// {
// public:
//     CInputItem()
//     {
// 
//     }
//     ~CInputItem()
//     {
// 
//     }
// 
// private:
//     unsigned int m_nPinIdx;  // 管脚
//     QString  m_szPinName; 
// 
//     QString  m_szTagName;
//     unsigned int  m_nDataType;  // bool /float/....
// 
// 
// }; 

class CLogicPropertyWgt;
class CFunBlockDiagramItem :public CFdbOrderBaseItem
{
    Q_OBJECT
public:
    CFunBlockDiagramItem(int nItemType, int nCreateNum = 0);
    ~CFunBlockDiagramItem();
    //初始化输入和输出
    void InitInOutItems();
    //判断当前item类型
    virtual QString AnalyseItemType(int nType);

    virtual void  mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    //逻辑判断
    virtual void AnalseBitInput();
    //
    virtual int GetLogicValue();
    //and算法
    void AnalyseAndMethod();
    //or算法
    void AnalyseOrMethod();
    //not算法
    void AnalyseNotMethod();
    //xor算法
    void AnalyseXorMethod();

private:
    CLogicPropertyWgt *m_pPropertyWgt;
    //当前指令类型
    int m_nOrderType;
    //std::vector<CInputItem *> m_arrInputs;
    //输出值
    int m_nItemOutValue;

};

#endif // CFUNBLOCKIDIAGRAMITEM
