#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"
#include "timersorderitem.h"


CTimersOrderItem::CTimersOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = new CLogicPropertyWgt;
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

void CTimersOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    if (GetItemTypNum() == TIMERCTD || GetItemTypNum() == TIMERCTU )
    {
        lstInput << tr("CD") << tr("LD") << tr("PV") ;

        lstOutput << tr("Q")<<tr("CV");

        SetInputMaxNum(INOUTMAX3);
        SetOutPutMaxNum(INOUTMAX2);
    }
    else if (GetItemTypNum() == TIMERCTUD)
    {
        lstInput << tr("CU") << tr("CD") << tr("R") << tr("LD") << tr("PV");

        lstOutput << tr("QU")<< tr("QD") << tr("CV");

        SetInputMaxNum(INOUTMAX5);
        SetOutPutMaxNum(INOUTMAX3);
    }
    else if (GetItemTypNum() == TIMERTOF || GetItemTypNum() == TIMERTON || GetItemTypNum() == TIMERTP)
    {
        lstInput << tr("IN") << tr("PT");

        lstOutput << tr("Q")<<tr("ET");

        SetInputMaxNum(INOUTMAX2);
        SetOutPutMaxNum(INOUTMAX2);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

QString CTimersOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == TIMERCTD)
    {
        return QObject::tr("CTD");
    }
    else if (nType == TIMERCTU)
    {
        return QObject::tr("CTU");
    }
    else if (nType == TIMERCTUD)
    {
        SetCurrentInputNum(6);
        return QObject::tr("CTUD");
    }
    else if (nType == TIMERTOF)
    {
        return QObject::tr("TOF");
    }
    else if (nType == TIMERTON)
    {
        return QObject::tr("TON");
    }
    else if (nType == TIMERTP)
    {
        return QObject::tr("TP");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

void CTimersOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //更新端子信息
    UpdateCurrentItemInfo();

    m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

void CTimersOrderItem::AnalseBitInput()
{
    if (m_nOrderType == TIMERCTD)
    {
        AnalyseCtdMethod();
    }
    else if (m_nOrderType == TIMERCTU)
    {
        AnalyseCtuMethod();
    }
    else if (m_nOrderType == TIMERCTUD)
    {
        AnalyseCtudMethod();
    }
    else if (m_nOrderType == TIMERTOF)
    {
        AnalyseTopMethod();
    }
    else if (m_nOrderType == TIMERTON)
    {
        AnalyseTonMethod();
    }
    else if (m_nOrderType == TIMERTP)
    {
        AnalyseTpMethod();
    }

    QList<CConnectionRouter*> lstLines = GetAllLines();

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
