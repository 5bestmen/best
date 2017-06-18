/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  timersorderitem.cpp
文件实现功能 :  定时逻辑块
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
#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"
#include "timersorderitem.h"

/*! \fn CTimersOrderItem::CTimersOrderItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CTimersOrderItem::CTimersOrderItem(int nItemType, int nCreateNum)
** \details 构造函数
** \param  父类
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CTimersOrderItem::CTimersOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = nullptr;
    m_nResetValue = 0;
    m_nLastTrigger = 0;
    m_nCtudUpTrigger = 0;
    m_nCtudDownTrigger = 0;
    m_nCtudPresetValue = 0;

    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

    connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
    connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
}


CTimersOrderItem::~CTimersOrderItem()
{
    delete m_pPropertyWgt;
}

/*! \fn CTimersOrderItem::InitInOutItems()
*********************************************************************************************************
** \brief CTimersOrderItem::InitInOutItems()
** \details 初始化逻辑块显示内容
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    //根据不同的类型  显示不同的输入和输出
    if (GetItemTypNum() == LOGIC_TIMERCTD || GetItemTypNum() == LOGIC_TIMER_CTU )
    {
        lstInput << tr("CD") << tr("LD") << tr("PV") ;

        lstOutput << tr("Q")<<tr("CV");

        SetInputMaxNum(INOUT_MAX3);
        SetOutPutMaxNum(INOUT_MAX2);
    }
    else if (GetItemTypNum() == LOGIC_TIMER_CTUD)
    {
        lstInput << tr("CU") << tr("CD") << tr("R") << tr("LD") << tr("PV");

        lstOutput << tr("QU")<< tr("QD") << tr("CV");

        SetInputMaxNum(INOUT_MAX5);
        SetOutPutMaxNum(INOUT_MAX3);
    }
    else if (GetItemTypNum() == LOGIC_TIMER_TOF || GetItemTypNum() == LOGIC_TIMER_TON || GetItemTypNum() == LOGIC_TIMER_TP)
    {
        lstInput << tr("IN") << tr("PT");

        lstOutput << tr("Q")<<tr("ET");

        SetInputMaxNum(INOUT_MAX2);
        SetOutPutMaxNum(INOUT_MAX2);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

/*! \fn CTimersOrderItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief CTimersOrderItem::AnalyseItemType(int nType)
** \details 根据类型反馈 类型标识
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString CTimersOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == LOGIC_TIMERCTD)
    {
        return QObject::tr("CTD");
    }
    else if (nType == LOGIC_TIMER_CTU)
    {
        return QObject::tr("CTU");
    }
    else if (nType == LOGIC_TIMER_CTUD)
    {
        SetCurrentInputNum(6);
        return QObject::tr("CTUD");
    }
    else if (nType == LOGIC_TIMER_TOF)
    {
        return QObject::tr("TOF");
    }
    else if (nType == LOGIC_TIMER_TON)
    {
        return QObject::tr("TON");
    }
    else if (nType == LOGIC_TIMER_TP)
    {
        return QObject::tr("TP");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

/*! \fn CTimersOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CTimersOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details 双击显示逻辑块详细信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (m_pPropertyWgt == nullptr)
    {
        m_pPropertyWgt = new CLogicPropertyWgt;
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
		connect(m_pPropertyWgt, SIGNAL(Signal_ChangeBindValue(QMap<int, QString> &)), this, SLOT(Slot_updateBindValueInfo(QMap<int, QString> &)));

    }

    //更新端子信息
    UpdateCurrentItemInfo();
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    m_pPropertyWgt->SetComment(GetCommentData());
   
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

/*! \fn CTimersOrderItem::AnalseBitInput()
*********************************************************************************************************
** \brief CTimersOrderItem::AnalseBitInput()
** \details 根据不同的逻辑块类型进行不同的运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::AnalseBitInput()
{
    if (m_nOrderType == LOGIC_TIMERCTD)
    {
        AnalyseCtdMethod();
    }
    else if (m_nOrderType == LOGIC_TIMER_CTU)
    {
        AnalyseCtuMethod();
    }
    else if (m_nOrderType == LOGIC_TIMER_CTUD)
    {
        AnalyseCtudMethod();
    }
    else if (m_nOrderType == LOGIC_TIMER_TOF)
    {
        AnalyseTopMethod();
    }
    else if (m_nOrderType == LOGIC_TIMER_TON)
    {
        AnalyseTonMethod();
    }
    else if (m_nOrderType == LOGIC_TIMER_TP)
    {
        AnalyseTpMethod();
    }

    QList<CConnectionRouter*> lstLines = GetAllLines();
    //根据不同的运算结果
    //逻辑线进行不同颜色的显示
    if (m_nItemOutValue == 0)
    {
        SetStatusColor(QColor(0, 0, 255));

        for each (CConnectionRouter* item  in lstLines)
        {
            if (item->StartItem() == this)
            {
                item->SetLineColor(QColor(0, 0, 255));
            }
        }
    }
    else
    {
        SetStatusColor(QColor(255, 0, 0));

        for each (CConnectionRouter* item  in lstLines)
        {
            if (item->StartItem() == this)
            {
                item->SetLineColor(QColor(255, 0, 0));
            }
        }
    }
    update();
}

int CTimersOrderItem::GetLogicValue()
{
    return m_nItemOutValue;
}

/*! \fn CTimersOrderItem::AnalyseCtdMethod()
*********************************************************************************************************
** \brief CTimersOrderItem::AnalyseCtdMethod()
** \details ctd运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::AnalyseCtdMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        //cv
        SetOutputData(3,0);
        return;
    }

    if ((!mapProInfo.contains(2)) || (!mapProInfo.contains(3)) || (!mapProInfo.contains(4)))
    {
        //
        SetOutputData(1, 1);
        SetOutputData(2, 0);
        SetOutputData(3, 0);
        return;
    }

    if (mapProInfo.contains(1))
    {
        //en有输入信息
        if (GetBinMapInfo().contains(1))
        {
            //输出的值 
            nEno = mapProInfo[1]->GetOutputData()[GetBinMapInfo()[1]];

            if (nEno == 0)
            {
                SetOutputData(1, 0);
                SetOutputData(2, 0);
                SetOutputData(3, 0);
                return;
            }
        }
    }

    if (mapProInfo.contains(2) && mapProInfo.contains(3) && mapProInfo.contains(4))
    {
        if (GetBinMapInfo().contains(2) && GetBinMapInfo().contains(3) && GetBinMapInfo().contains(4))
        {
            int nTrigger = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];
            int nLoad = mapProInfo[3]->GetOutputData()[GetBinMapInfo()[3]];
            int nPresetValue = mapProInfo[3]->GetOutputData()[GetBinMapInfo()[4]];

            if (nLoad == 1)
            {
                //更新初始值
                m_nResetValue = nPresetValue;
            }

            if (nTrigger == 0 && m_nLastTrigger == 1)
            {
                m_nResetValue--;
            }

            if (m_nResetValue <= 0)
            {
                SetOutputData(2, 1);
                nValue = 1;
                
            }
            else
            {
                SetOutputData(2, 0);
                nValue = 0;
            }

            SetOutputData(3, m_nResetValue);

            if (m_nResetValue == 0)
            {
                SetOutputData(1, 0);
            }
            else
            {
                SetOutputData(1, 1);
            }

        }
    }

    m_nItemOutValue = nValue;
}

/*! \fn CTimersOrderItem::AnalyseCtuMethod()
*********************************************************************************************************
** \brief CTimersOrderItem::AnalyseCtuMethod()
** \details ctu运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::AnalyseCtuMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        //cv
        SetOutputData(3, 0);
        return;
    }

    if ((!mapProInfo.contains(2)) || (!mapProInfo.contains(3)) || (!mapProInfo.contains(4)))
    {
        //
        SetOutputData(1, 1);
        SetOutputData(2, 0);
        SetOutputData(3, 0);
        return;
    }

    if (mapProInfo.contains(1))
    {
        //en有输入信息
        if (GetBinMapInfo().contains(1))
        {
            //输出的值 
            nEno = mapProInfo[1]->GetOutputData()[GetBinMapInfo()[1]];

            if (nEno == 0)
            {
                SetOutputData(1, 0);
                SetOutputData(2, 0);
                SetOutputData(3, 0);
                return;
            }
        }
    }

    if (mapProInfo.contains(2) && mapProInfo.contains(3) && mapProInfo.contains(4))
    {
        if (GetBinMapInfo().contains(2) && GetBinMapInfo().contains(3) && GetBinMapInfo().contains(4))
        {
            int nTrigger = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];
            int nLoad = mapProInfo[3]->GetOutputData()[GetBinMapInfo()[3]];
            int nPresetValue = mapProInfo[4]->GetOutputData()[GetBinMapInfo()[4]];

            if (nLoad == 1)
            {
                //更新初始值
                m_nResetValue = nPresetValue;
            }

            if (nTrigger == 0 && m_nLastTrigger == 1)
            {
                m_nResetValue++;
            }

            if (m_nResetValue <= 0)
            {
                SetOutputData(2, 1);
                nValue = 1;

            }
            else
            {
                SetOutputData(2, 0);
                nValue = 0;
            }

            SetOutputData(3, m_nResetValue);

            if (m_nResetValue == 0)
            {
                SetOutputData(1, 0);
            }
            else
            {
                SetOutputData(1, 1);
            }

        }
    }

    m_nItemOutValue = nValue;
}

/*! \fn CTimersOrderItem::AnalyseCtudMethod()
*********************************************************************************************************
** \brief CTimersOrderItem::AnalyseCtudMethod()
** \details ctud运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CTimersOrderItem::AnalyseCtudMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //updisplay
        SetOutputData(2, 0);
        //downDisplay
        SetOutputData(3, 0);
        //countValue
        SetOutputData(4, 0);
        return;
    }

    if ((!mapProInfo.contains(2)) || (!mapProInfo.contains(3)) || (!mapProInfo.contains(4))
        || !mapProInfo.contains(5) || !mapProInfo.contains(6))
    {
        //
        SetOutputData(1, 1);
        SetOutputData(2, 0);
        SetOutputData(3, 0);
        return;
    }

    if (mapProInfo.contains(1))
    {
        //en有输入信息
        if (GetBinMapInfo().contains(1))
        {
            //输出的值 
            nEno = mapProInfo[1]->GetOutputData()[GetBinMapInfo()[1]];

            if (nEno == 0)
            {
                SetOutputData(1, 0);
                SetOutputData(2, 0);
                SetOutputData(3, 0);
                SetOutputData(4, 0);
                return;
            }
        }
    }

    if (mapProInfo.contains(2) && mapProInfo.contains(3) && mapProInfo.contains(4) && mapProInfo.contains(5) && mapProInfo.contains(6))
    {
        if (GetBinMapInfo().contains(2) && GetBinMapInfo().contains(3) && GetBinMapInfo().contains(4)
            && GetBinMapInfo().contains(5) && GetBinMapInfo().contains(6))
        {
            int nUpTrigger = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];
            int nDouwnTrigger = mapProInfo[3]->GetOutputData()[GetBinMapInfo()[3]];
            int nReset = mapProInfo[4]->GetOutputData()[GetBinMapInfo()[4]];
            int nLoad = mapProInfo[5]->GetOutputData()[GetBinMapInfo()[5]];
            int nPresetValue = mapProInfo[6]->GetOutputData()[GetBinMapInfo()[6]];

            if (nReset == 1)
            {
                //重置
                SetOutputData(4,0);
            }

            if (nLoad == 1 && nReset != 1)
            {
                SetOutputData(4,nPresetValue);
                m_nCtudPresetValue = nPresetValue;
            }

            //uptrigger由0转为1
            if (m_nCtudDownTrigger == 0 && nUpTrigger == 1)
            {
                m_nCtudDownTrigger = nUpTrigger;
                m_nCtudPresetValue--;
            }

            //dwontrigger由0转为1
            if (m_nCtudDownTrigger == 0 && nUpTrigger == 1)
            {
                m_nCtudDownTrigger = nUpTrigger;
                m_nCtudPresetValue++;
            }

            //
            SetOutputData(4, m_nCtudPresetValue);
            //
            if (m_nCtudPresetValue >= nPresetValue)
            {
                SetOutputData(2, 1);
            }

            if (m_nCtudPresetValue <=0)
            {
                SetOutputData(3, 1);
            }

        }
    }

    m_nItemOutValue = nValue;
}

void CTimersOrderItem::AnalyseTopMethod()
{

}

void CTimersOrderItem::AnalyseTonMethod()
{

}

void CTimersOrderItem::AnalyseTpMethod()
{

}
