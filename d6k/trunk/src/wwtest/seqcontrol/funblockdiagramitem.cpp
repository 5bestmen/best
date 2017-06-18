#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "funblockdiagramitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"

CFunBlockDiagramItem::CFunBlockDiagramItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = new CLogicPropertyWgt;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

    connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
    connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
}

CFunBlockDiagramItem::~CFunBlockDiagramItem()
{
    delete m_pPropertyWgt;
}

QString CFunBlockDiagramItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == BITAND)
    {
        SetCurrentInputNum(4);
        return QObject::tr("AND");
    }
    else if (nType == BITOR)
    {
        SetCurrentInputNum(4);
        return QObject::tr("OR");
    }
    else if (nType == BITXOR)
    {
        SetCurrentInputNum(4);
        return QObject::tr("XOR");
    }
    else if (nType == BITNOT)
    {
        SetCurrentInputNum(1);
        return QObject::tr("NOT");
    }
    else
    {
        SetCurrentInputNum(4);
        return QObject::tr("UnKnown");
    }
}

void CFunBlockDiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //更新端子信息
    UpdateCurrentItemInfo();
    m_pPropertyWgt->SetItemTypeCode(m_nOrderType);
    m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

void CFunBlockDiagramItem::AnalseBitInput()
{
    //分析EN
    if (m_nOrderType == BITAND)
    {
        AnalyseAndMethod();
    }
    else if (m_nOrderType == BITOR)
    {
        AnalyseOrMethod();
    }
    else if (m_nOrderType == BITNOT)
    {
        AnalyseNotMethod();
    }
    else if (m_nOrderType == BITXOR)
    {
        AnalyseXorMethod();
    }

    QList<CConnectionRouter*> lstLines = GetAllLines();

    if (m_nItemOutValue == 0)
    {
        SetStatusColor(QColor(0,0,255));

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

int CFunBlockDiagramItem::GetLogicValue()
{
    return m_nItemOutValue;
}

void CFunBlockDiagramItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    if (GetItemTypNum() == BITAND || GetItemTypNum() == BITOR || GetItemTypNum() == BITXOR )
    {
        lstInput << tr("IN1") << tr("IN2") << tr("IN3") << tr("IN4") << tr("IN5") << tr("IN6") << tr("IN7")
            << tr("IN8") << tr("IN9") << tr("IN10") << tr("IN11") << tr("IN12") << tr("IN13") << tr("IN14")
            << tr("IN15") << tr("IN16");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUTMAX16);
        SetOutPutMaxNum(INOUTMAX1);
    }
    else if (GetItemTypNum() == BITNOT)
    {
        lstInput << tr("IN");
        lstOutput << tr("OUT");

        SetInputMaxNum(INOUTMAX1);
        SetOutPutMaxNum(INOUTMAX1);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

void CFunBlockDiagramItem::AnalyseAndMethod()
{
    int nValue = 0;
    int nEno = 0;
    //首次标示
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  在没有输入的情况下为1
        SetOutputData(1,1);
        //在没有input时  输出为0
        SetOutputData(2,0);
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
                    nValue = nValue & it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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

void CFunBlockDiagramItem::AnalyseOrMethod()
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
                    nValue = nValue | it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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
        SetOutputData(1, 1);
        SetOutputData(2, nValue);
    }

    m_nItemOutValue = nValue;
}

void CFunBlockDiagramItem::AnalyseNotMethod()
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
                nValue = !it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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

void CFunBlockDiagramItem::AnalyseXorMethod()
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
                    nValue = nValue ^ it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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
