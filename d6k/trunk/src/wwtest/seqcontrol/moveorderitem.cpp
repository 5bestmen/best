#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "moveorderitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"

CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = new CLogicPropertyWgt;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

    connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
    connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
}

CMoveOrderItem::~CMoveOrderItem()
{
    delete m_pPropertyWgt;
}

void CMoveOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    if (GetItemTypNum() == BITSHL || GetItemTypNum() == BITSHR || GetItemTypNum() == BITROL || GetItemTypNum() == BITROR)
    {
        lstInput << tr("IN")  << tr("N");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUTMAX2);
        SetOutPutMaxNum(INOUTMAX1);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

QString CMoveOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == BITSHL)
    {
        SetCurrentInputNum(2);
        return QObject::tr("SHL");
    }
    else if (nType == BITSHR)
    {
        SetCurrentInputNum(2);
        return QObject::tr("SHR");
    }
    else if (nType == BITROL)
    {
        SetCurrentInputNum(2);
        return QObject::tr("ROL");
    }
    else if (nType == BITROR)
    {
        SetCurrentInputNum(2);
        return QObject::tr("ROR");
    }
    else if (nType == BITBTST)
    {
        return QObject::tr("BTST");
    }
    else if (nType == BITBSET)
    {
        return QObject::tr("BSET");
    }
    else if (nType == BITCLR)
    {
        return QObject::tr("BCLR");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

void CMoveOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    //更新端子信息
    UpdateCurrentItemInfo();

    m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

void CMoveOrderItem::AnalseBitInput()
{
    //分析EN
    if (m_nOrderType == BITSHL)
    {
        AnalyseShlMethod();
    }
    else if (m_nOrderType == BITSHR)
    {
        AnalyseShrMethod();
    }
    else if (m_nOrderType == BITROL)
    {
        AnalyseRolMethod();
    }
    else if (m_nOrderType == BITROR)
    {
        AnalyseRorMehtod();
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

int CMoveOrderItem::GetLogicValue()
{
    return m_nItemOutValue;

}

void CMoveOrderItem::AnalyseShlMethod()
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

            nValue = nInput << nNumber;
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

void CMoveOrderItem::AnalyseShrMethod()
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

            nValue = nInput >> nNumber;
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

void CMoveOrderItem::AnalyseRolMethod()
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

            nValue = crol(nInput,nNumber);

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

void CMoveOrderItem::AnalyseRorMehtod()
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

            nValue = cror(nInput, nNumber);

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


unsigned char CMoveOrderItem::crol(unsigned char uInput, unsigned uMoveNum)
{
    unsigned char left = uInput << uMoveNum;
    unsigned char right = uInput >> (sizeof(unsigned char) - uMoveNum);
    unsigned char temp = left | right;

    return temp;
}

unsigned char CMoveOrderItem::cror(unsigned char uInput, unsigned uMoveNum)
{
    unsigned char right = uInput >> uMoveNum;
    unsigned char left = uInput << (sizeof(unsigned char) - uMoveNum);
    unsigned char temp = left | right;

    return temp;
}