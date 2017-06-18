#include "callsritem.h"
#include "logicpropertywgt.h"

CCallSRItem::CCallSRItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = new CLogicPropertyWgt;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);
    //最多为16个
    SetInputMaxNum(MAXINPUTNUMX);

    QStringList lstInput;
    lstInput << tr("CALL");

    SetInputPinNames(lstInput);
    //
    SetOutPutMaxNum(MAXOUTPUTNUM);
    //输出
//     QStringList lstOutput;
//     SetOutputPinNames(lstOutput);

    connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
    connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));

}

CCallSRItem::~CCallSRItem()
{
    delete m_pPropertyWgt;
}

QString CCallSRItem::AnalyseItemType(int nType)
{
    if (nType == CALLSR)
    {
        return tr("SR");
    }
    return "";
}

void CCallSRItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //更新端子信息
    UpdateCurrentItemInfo();
    m_pPropertyWgt->SetItemTypeCode(CALLSR);
    m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

QStringList CCallSRItem::GetChildNames()
{
    return QStringList("");
}
