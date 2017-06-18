/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  logicitemfactory.cpp
文件实现功能 :  逻辑块制作工厂
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明> 逻辑编辑
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2017.1.4
*/
#include "logicitemfactory.h"
#include "fdborderbaseitem.h"
#include "funblockdiagramitem.h"
#include "mathorderitem.h"
#include "moveorderitem.h"
#include "compareorderitem.h"
#include "timersorderitem.h"
#include "callsritem.h"

CLogicItemFactory::CLogicItemFactory()
{

}

CLogicItemFactory::~CLogicItemFactory()
{

}

/*! \fn CLogicContainter::CLogicItemFactory::CreateLogicItemProduct(int ChangeType, int nIndex)
*********************************************************************************************************
** \brief CLogicContainter::CLogicItemFactory::CreateLogicItemProduct(int ChangeType, int nIndex)
** \details 创建不同类型的逻辑块
** \param   ChangeType:逻辑块类型  nIndex:逻辑块创建编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFdbOrderBaseItem* CLogicItemFactory::CreateLogicItemProduct(int ChangeType, int nIndex)
{
    if (ChangeType >= LOGIC_AND && ChangeType <= LOGIC_XOR)
    {
        //逻辑类型
        return new CFunBlockDiagramItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_SHL && ChangeType <= LOGIC_CLR)
    {
        //位移类型
        return new CMoveOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_MATH_MOVE && ChangeType <= LOGIC_MATH_MIN)
    {
        //数学计算类型
        return new CMathOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_COMPARE_EQ && ChangeType <= LOGIC_COMPARE_NE)
    {
        //比较类型
        return new CCompareOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_TIMERCTD && ChangeType <= LOGIC_TIMER_TP)
    {
        //计数器类型
        return new CTimersOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType == LOGIC_CALLSR)
    {
        //调用子程序类型
        return new CCallSRItem(ChangeType, nIndex);
    }
    else
    {
        return nullptr;
    }

}
