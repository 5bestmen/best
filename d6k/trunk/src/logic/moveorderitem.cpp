/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  moveorderitem.cpp
�ļ�ʵ�ֹ��� :  λ������
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
#include <QtWidgets>
#include <QCursor>
#include <QTimer>
#include "moveorderitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"

/*! \fn CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
** \details λ������ ���캯��
** \param  nItemType:�߼�������  nCreateNum:�������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = nullptr;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

}

CMoveOrderItem::~CMoveOrderItem()
{
    if (m_pPropertyWgt != nullptr)
    {
        delete m_pPropertyWgt;
        m_pPropertyWgt = nullptr;
    }
    
}

/*! \fn CMoveOrderItem::InitInOutItems()
*********************************************************************************************************
** \brief CMoveOrderItem::InitInOutItems()
** \details  
** \param  
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::InitInOutItems()
{
    //����
    QStringList lstInput;
    //���
    QStringList lstOutput;

    //���ݲ�ͬ���� ��ʾ��ͬ����������
    if (GetItemTypNum() == LOGIC_SHL || GetItemTypNum() == LOGIC_SHR || GetItemTypNum() == LOGIC_ROL || GetItemTypNum() == LOGIC_ROR)
    {
        lstInput << tr("IN")  << tr("N");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX2);
        SetOutPutMaxNum(INOUT_MAX1);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

/*! \fn CMoveOrderItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseItemType(int nType)
** \details  �������ͱ���  ��ʾ ���ͱ�ʶ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QString CMoveOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //��ʾ����
    if (nType == LOGIC_SHL)
    {
        //��ʼ�������������
        SetCurrentInputNum(2);
        return QObject::tr("SHL");
    }
    else if (nType == LOGIC_SHR)
    {
        SetCurrentInputNum(2);
        return QObject::tr("SHR");
    }
    else if (nType == LOGIC_ROL)
    {
        SetCurrentInputNum(2);
        return QObject::tr("ROL");
    }
    else if (nType == LOGIC_ROR)
    {
        SetCurrentInputNum(2);
        return QObject::tr("ROR");
    }
    else if (nType == LOGIC_BTST)
    {
        return QObject::tr("BTST");
    }
    else if (nType == LOGIC_BSET)
    {
        return QObject::tr("BSET");
    }
    else if (nType == LOGIC_CLR)
    {
        return QObject::tr("BCLR");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

/*! \fn CMoveOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CMoveOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details  ˫����ʾ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_pPropertyWgt == nullptr)
    {
        m_pPropertyWgt = new CLogicPropertyWgt;
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
		connect(m_pPropertyWgt, SIGNAL(Signal_ChangeBindValue(QMap<int, QString> &)), this, SLOT(Slot_updateBindValueInfo(QMap<int, QString> &)));

    }

    //���¶�����Ϣ
    UpdateCurrentItemInfo();
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    m_pPropertyWgt->SetComment(GetCommentData());

    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

/*! \fn CMoveOrderItem::AnalseBitInput()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalseBitInput()
** \details  ���ݲ�ͬ������ ���в�ͬ���㷨
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalseBitInput()
{
    //����EN
    if (m_nOrderType == LOGIC_SHL)
    {
        //shl
        AnalyseShlMethod();
    }
    else if (m_nOrderType == LOGIC_SHR)
    {
        //shr
        AnalyseShrMethod();
    }
    else if (m_nOrderType == LOGIC_ROL)
    {
        //rol
        AnalyseRolMethod();
    }
    else if (m_nOrderType == LOGIC_ROR)
    {
        //ror
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

/*! \fn CMoveOrderItem::AnalyseShlMethod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseShlMethod()
** \details  shl�㷨
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalyseShlMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
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
        //en��������Ϣ
        if (GetBinMapInfo().contains(1))
        {
            //�����ֵ 
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

/*! \fn CMoveOrderItem::AnalyseShrMethod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseShrMethod()
** \details  shr�㷨
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalyseShrMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
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
        //en��������Ϣ
        if (GetBinMapInfo().contains(1))
        {
            //�����ֵ 
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

/*! \fn CMoveOrderItem::AnalyseRolMethod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseRolMethod()
** \details  rol�㷨
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalyseRolMethod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
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
        //en��������Ϣ
        if (GetBinMapInfo().contains(1))
        {
            //�����ֵ 
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

            nValue = MoveLeft(nInput,nNumber);

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

/*! \fn CMoveOrderItem::AnalyseRorMehtod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseRorMehtod()
** \details  ror�㷨
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalyseRorMehtod()
{
    int nValue = 0;
    int nEno = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
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
        //en��������Ϣ
        if (GetBinMapInfo().contains(1))
        {
            //�����ֵ 
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

            nValue = MoveRight(nInput, nNumber);

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

/*! \fn CMoveOrderItem::MoveLeft(unsigned char uInput, unsigned uMoveNum)
*********************************************************************************************************
** \brief CMoveOrderItem::MoveLeft(unsigned char uInput, unsigned uMoveNum)
** \details  ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
unsigned char CMoveOrderItem::MoveLeft(unsigned char uInput, unsigned uMoveNum)
{
    unsigned char left = uInput << uMoveNum;
    unsigned char right = uInput >> (sizeof(unsigned char) - uMoveNum);
    unsigned char temp = left | right;

    return temp;
}

/*! \fn CMoveOrderItem::MoveRight(unsigned char uInput, unsigned uMoveNum)
*********************************************************************************************************
** \brief CMoveOrderItem::MoveRight(unsigned char uInput, unsigned uMoveNum)
** \details  ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
unsigned char CMoveOrderItem::MoveRight(unsigned char uInput, unsigned uMoveNum)
{
    unsigned char right = uInput >> uMoveNum;
    unsigned char left = uInput << (sizeof(unsigned char) - uMoveNum);
    unsigned char temp = left | right;

    return temp;
}