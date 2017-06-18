/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  logicitemfactory.cpp
�ļ�ʵ�ֹ��� :  �߼�����������
����         :  ww
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��> �߼��༭
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details ������ͬ���͵��߼���
** \param   ChangeType:�߼�������  nIndex:�߼��鴴�����
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CFdbOrderBaseItem* CLogicItemFactory::CreateLogicItemProduct(int ChangeType, int nIndex)
{
    if (ChangeType >= LOGIC_AND && ChangeType <= LOGIC_XOR)
    {
        //�߼�����
        return new CFunBlockDiagramItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_SHL && ChangeType <= LOGIC_CLR)
    {
        //λ������
        return new CMoveOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_MATH_MOVE && ChangeType <= LOGIC_MATH_MIN)
    {
        //��ѧ��������
        return new CMathOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_COMPARE_EQ && ChangeType <= LOGIC_COMPARE_NE)
    {
        //�Ƚ�����
        return new CCompareOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType >= LOGIC_TIMERCTD && ChangeType <= LOGIC_TIMER_TP)
    {
        //����������
        return new CTimersOrderItem(ChangeType, nIndex);
    }
    else if (ChangeType == LOGIC_CALLSR)
    {
        //�����ӳ�������
        return new CCallSRItem(ChangeType, nIndex);
    }
    else
    {
        return nullptr;
    }

}
