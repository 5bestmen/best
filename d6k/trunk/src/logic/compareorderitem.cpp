/*! @file
<PRE>
********************************************************************************
ģ����       :  �Ƚ��߼���
�ļ���       :  compareorderitem.cpp
�ļ�ʵ�ֹ��� :
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
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"
#include "compareorderitem.h"


/*! \fn CCompareOrderItem::CCompareOrderItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CCompareOrderItem::CCompareOrderItem(int nItemType, int nCreateNum)
** \details ���캯��
** \param nItemType�����ͱ���   nCreateNum:�������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CCompareOrderItem::CCompareOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = nullptr;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;
}

CCompareOrderItem::~CCompareOrderItem()
{
    if (m_pPropertyWgt != nullptr)
    {
        delete m_pPropertyWgt;
        m_pPropertyWgt = nullptr;
    }
    
}

/*! \fn CCompareOrderItem::InitInOutItems()
*********************************************************************************************************
** \brief CCompareOrderItem::InitInOutItems()
** \details ��ʼ���߼���
** \param 
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::InitInOutItems()
{
    //����
    QStringList lstInput;
    //���
    QStringList lstOutput;

    //�߼���  ����  ���
    if (GetItemTypNum() == LOGIC_COMPARE_EQ || GetItemTypNum() == LOGIC_COMPARE_GE || GetItemTypNum() == LOGIC_COMPARE_GT 
        || GetItemTypNum() == LOGIC_COMPARE_LE || GetItemTypNum() == LOGIC_COMPARE_LT)
    {
        lstInput << tr("IN1") << tr("IN2") << tr("IN3") << tr("IN4") << tr("IN5") << tr("IN6") << tr("IN7")
            << tr("IN8") << tr("IN9") << tr("IN10") << tr("IN11") << tr("IN12") << tr("IN13") << tr("IN14")
            << tr("IN15") << tr("IN16");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX16);
        SetOutPutMaxNum(INOUT_MAX1);
    }
    else if (GetItemTypNum() == LOGIC_COMPARE_NE)
    {
        lstInput << tr("IN1") << tr("IN2");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX2);
        SetOutPutMaxNum(INOUT_MAX1);
    }

    //��������ܽ�����
    SetInputPinNames(lstInput);
    //��������ܽ�����
    SetOutputPinNames(lstOutput);
}

/*! \fn CCompareOrderItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseItemType(int nType)
** \details �������ͱ��, �������ͱ���
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
QString CCompareOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //��ʾ����
    if (nType == LOGIC_COMPARE_EQ)
    {
        return QObject::tr("EQ");
    }
    else if (nType == LOGIC_COMPARE_GE)
    {
        return QObject::tr("GE");
    }
    else if (nType == LOGIC_COMPARE_GT)
    {
        return QObject::tr("GT");
    }
    else if (nType == LOGIC_COMPARE_LE)
    {
        return QObject::tr("LE");
    }
    else if (nType == LOGIC_COMPARE_LT)
    {
        return QObject::tr("LT");
    }
    else if (nType == LOGIC_COMPARE_NE)
    {
        return QObject::tr("NE");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

/*! \fn CCompareOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CCompareOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details ˫���߼���  ��ʾ����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (m_pPropertyWgt == nullptr)
    {
        //������ϸ��Ϣwidget
        m_pPropertyWgt = new CLogicPropertyWgt;
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
		connect(m_pPropertyWgt,SIGNAL(Signal_ChangeBindValue(QMap<int, QString> &)),this,SLOT(Slot_updateBindValueInfo(QMap<int, QString> &)));
    }

    //���¶�����Ϣ
    UpdateCurrentItemInfo();
    //��ȡeno  ��ʾ��Ϣ
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());
    //��ȡ �������Ϣ  �� �������Ϣ
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    m_pPropertyWgt->SetComment(GetCommentData());
    
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

void CCompareOrderItem::AnalseBitInput()
{
    if (m_nOrderType == LOGIC_COMPARE_EQ)
    {
        AnalyseEqMethod();
    }
    else if (m_nOrderType == LOGIC_COMPARE_GE)
    {
        AnalyseGeMethod();
    }
    else if (m_nOrderType == LOGIC_COMPARE_GT)
    {
        AnalyseGtMethod();
    }
    else if (m_nOrderType == LOGIC_COMPARE_LE)
    {
        AnalyseLeMethod();
    }
    else if (m_nOrderType == LOGIC_COMPARE_LT)
    {
        AnalyseLtMethod();
    }
    else if (m_nOrderType == LOGIC_COMPARE_NE)
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

/*! \fn CCompareOrderItem::AnalyseEqMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseEqMethod()
** \details ��������  �˲����ݲ���ѽ
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseEqMethod()
{
    int nValue = 0;
    int nEno = 0;
    //�״α�ʾ
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en��������Ϣ
            if (GetBinMapInfo().contains(1))
            {
                //�����ֵ 
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
            //��ͨ����
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //��һ�ν���
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

/*! \fn CCompareOrderItem::AnalyseGeMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseGeMethod()
** \details ge����  �˲����ݲ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseGeMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //�״α�ʾ
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en��������Ϣ
            if (GetBinMapInfo().contains(1))
            {
                //�����ֵ 
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
            //��ͨ����
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //��һ�ν���
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

/*! \fn CCompareOrderItem::AnalyseGtMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseGtMethod()
** \details gt����  �˲����ݲ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseGtMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //�״α�ʾ
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en��������Ϣ
            if (GetBinMapInfo().contains(1))
            {
                //�����ֵ 
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
            //��ͨ����
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //��һ�ν���
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

/*! \fn CCompareOrderItem::AnalyseLeMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseLeMethod()
** \details le����  �˲����ݲ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseLeMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //�״α�ʾ
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en��������Ϣ
            if (GetBinMapInfo().contains(1))
            {
                //�����ֵ 
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
            //��ͨ����
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //��һ�ν���
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

/*! \fn CCompareOrderItem::AnalyseLtMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseLtMethod()
** \details lt����  �˲����ݲ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseLtMethod()
{
    int nValue = 0;
    int nBigValue = 0;
    int nEno = 0;
    //�״α�ʾ
    int nFirstFlag = 0;

    QMap<int, CFdbOrderBaseItem*> mapProInfo = GetInputItems();

    if (mapProInfo.isEmpty())
    {
        //eno  ��û������������Ϊ1
        SetOutputData(1, 1);
        //��û��inputʱ  ���Ϊ0
        SetOutputData(2, 0);
        return;
    }

    QMap<int, CFdbOrderBaseItem*>::const_iterator it = mapProInfo.constBegin();

    while (it != mapProInfo.constEnd())
    {
        if (it.key() == 1)
        {
            //en��������Ϣ
            if (GetBinMapInfo().contains(1))
            {
                //�����ֵ 
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
            //��ͨ����
            if (GetBinMapInfo().contains(it.key()))
            {
                nFirstFlag++;
                if (nFirstFlag == 1)
                {
                    //��һ�ν���
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

/*! \fn CCompareOrderItem::AnalyseNeMethod()
*********************************************************************************************************
** \brief CCompareOrderItem::AnalyseNeMethod()
** \details ne����  �˲����ݲ�����
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCompareOrderItem::AnalyseNeMethod()
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
