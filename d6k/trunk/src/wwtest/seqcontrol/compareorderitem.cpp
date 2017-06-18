#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"
#include "compareorderitem.h"

CCompareOrderItem::CCompareOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = new CLogicPropertyWgt;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

    connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
    connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));

}

CCompareOrderItem::~CCompareOrderItem()
{
    delete m_pPropertyWgt;
}

void CCompareOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    if (GetItemTypNum() == COMPAREEQ || GetItemTypNum() == COMPAREGE || GetItemTypNum() == COMPAREGT 
        || GetItemTypNum() == COMPARELE || GetItemTypNum() == COMPARELT)
    {
        lstInput << tr("IN1") << tr("IN2") << tr("IN3") << tr("IN4") << tr("IN5") << tr("IN6") << tr("IN7")
            << tr("IN8") << tr("IN9") << tr("IN10") << tr("IN11") << tr("IN12") << tr("IN13") << tr("IN14")
            << tr("IN15") << tr("IN16");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUTMAX16);
        SetOutPutMaxNum(INOUTMAX1);
    }
    else if (GetItemTypNum() == COMPARENE)
    {
        lstInput << tr("IN1") << tr("IN2");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUTMAX2);
        SetOutPutMaxNum(INOUTMAX1);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

QString CCompareOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == COMPAREEQ)
    {
        return QObject::tr("EQ");
    }
    else if (nType == COMPAREGE)
    {
        return QObject::tr("GE");
    }
    else if (nType == COMPAREGT)
    {
        return QObject::tr("GT");
    }
    else if (nType == COMPARELE)
    {
        return QObject::tr("LE");
    }
    else if (nType == COMPARELT)
    {
        return QObject::tr("LT");
    }
    else if (nType == COMPARENE)
    {
        return QObject::tr("NE");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

void CCompareOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //更新端子信息
    UpdateCurrentItemInfo();

    m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

void CCompareOrderItem::AnalseBitInput()
{
    if (m_nOrderType == COMPAREEQ)
    {
        AnalyseEqMethod();
    }
    else if (m_nOrderType == COMPAREGE)
    {
        AnalyseGeMethod();
    }
    else if (m_nOrderType == COMPAREGT)
    {
        AnalyseGtMethod();
    }
    else if (m_nOrderType == COMPARELE)
    {
        AnalyseLeMethod();
    }
    else if (m_nOrderType == COMPARELT)
    {
        AnalyseLtMethod();
    }
    else if (m_nOrderType == COMPARENE)
    {
        AnalyseNeMethod();
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

int CCompareOrderItem::GetLogicValue()
{
    return m_nItemOutValue;
}

void CCompareOrderItem::AnalyseEqMethod()
{
    int nValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en有输入信息
            if (GetBinMapInfo().contains(1))
            {
                //输出的值 
                nEno = it.value()->GetOutputData()[GetBinMapInfo()[1]];

                if (nEno == 0)
                {
                    SetOutputData(1, 0);
                    SetOutputData(2, 0);
                    return;

                }
            }
        }
        else
        {
            //普通输入
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //第一次进来
                    nValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                }
                else
                {
                    if (nValue == it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nValue = 1;
                    }
                    else
                    {
                        nValue = 0;
                        break;
                    }
                }
            }
        }
        it++;
    }


    if (nValue == 0)
    {
        SetOutputData(1, 0);
        SetOutputData(2, 0);
    }
    else
    {
        SetOutputData(1, 0);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CCompareOrderItem::AnalyseGeMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en有输入信息
            if (GetBinMapInfo().contains(1))
            {
                //输出的值 
                nEno = it.value()->GetOutputData()[GetBinMapInfo()[1]];

                if (nEno == 0)
                {
                    SetOutputData(1, 0);
                    SetOutputData(2, 0);
                    return;

                }
            }
        }
        else
        {
            //普通输入
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //第一次进来
                    nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                    //

                }
                else
                {
                    if (nBigValue >= it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                        nValue = 1;
                    }
                    else
                    {
                        nValue = 0;
                        break;
                    }
                }
            }
        }
        it++;
    }


    if (nValue == 0)
    {
        SetOutputData(1, 0);
        SetOutputData(2, 0);
    }
    else
    {
        SetOutputData(1, 0);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CCompareOrderItem::AnalyseGtMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en有输入信息
            if (GetBinMapInfo().contains(1))
            {
                //输出的值 
                nEno = it.value()->GetOutputData()[GetBinMapInfo()[1]];

                if (nEno == 0)
                {
                    SetOutputData(1, 0);
                    SetOutputData(2, 0);
                    return;

                }
            }
        }
        else
        {
            //普通输入
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //第一次进来
                    nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                    //

                }
                else
                {
                    if (nBigValue > it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                        nValue = 1;
                    }
                    else
                    {
                        nValue = 0;
                        break;
                    }
                }
            }
        }
        it++;
    }


    if (nValue == 0)
    {
        SetOutputData(1, 0);
        SetOutputData(2, 0);
    }
    else
    {
        SetOutputData(1, 0);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CCompareOrderItem::AnalyseLeMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en有输入信息
            if (GetBinMapInfo().contains(1))
            {
                //输出的值 
                nEno = it.value()->GetOutputData()[GetBinMapInfo()[1]];

                if (nEno == 0)
                {
                    SetOutputData(1, 0);
                    SetOutputData(2, 0);
                    return;

                }
            }
        }
        else
        {
            //普通输入
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //第一次进来
                    nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                    //

                }
                else
                {
                    if (nBigValue <= it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                        nValue = 1;
                    }
                    else
                    {
                        nValue = 0;
                        break;
                    }
                }
            }
        }
        it++;
    }


    if (nValue == 0)
    {
        SetOutputData(1, 0);
        SetOutputData(2, 0);
    }
    else
    {
        SetOutputData(1, 0);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CCompareOrderItem::AnalyseLtMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1, 1);
        //在没有input时  输出为0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en有输入信息
            if (GetBinMapInfo().contains(1))
            {
                //输出的值 
                nEno = it.value()->GetOutputData()[GetBinMapInfo()[1]];

                if (nEno == 0)
                {
                    SetOutputData(1, 0);
                    SetOutputData(2, 0);
                    return;

                }
            }
        }
        else
        {
            //普通输入
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //第一次进来
                    nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                    //

                }
                else
                {
                    if (nBigValue < it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nBigValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
                        nValue = 1;
                    }
                    else
                    {
                        nValue = 0;
                        break;
                    }
                }
            }
        }
        it++;
    }


    if (nValue == 0)
    {
        SetOutputData(1, 0);
        SetOutputData(2, 0);
    }
    else
    {
        SetOutputData(1, 0);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CCompareOrderItem::AnalyseNeMethod()
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
        return;
    }

    if ((!mapProInfo.contains(2)) || (!mapProInfo.contains(3)))
    {
        //
        SetOutputData(1, 1);
        SetOutputData(2, 0);
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
                return;
            }
        }
    }

    if (mapProInfo.contains(2) && mapProInfo.contains(3))
    {
        if (GetBinMapInfo().contains(2) && GetBinMapInfo().contains(3))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];
            int nNumber = mapProInfo[3]->GetOutputData()[GetBinMapInfo()[3]];

            nValue = nInput != nNumber ? 1:0;

            if (nValue == 0)
            {
                SetOutputData(1, 0);
            }
            else
            {
                SetOutputData(1, 1);
            }

            SetOutputData(2, nValue);
        }
    }

    m_nItemOutValue = nValue;
}
