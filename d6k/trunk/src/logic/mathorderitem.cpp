/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  mathorderitem.cpp
文件实现功能 :  数学计算逻辑块
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
#include "mathorderitem.h"

/*! \fn CMathOrderItem::CMathOrderItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CMathOrderItem::CMathOrderItem(int nItemType, int nCreateNum)
** \details 数据计算逻辑块 构造函数
** \param  nItemType:逻辑块类型  nCreateNum:创建编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CMathOrderItem::CMathOrderItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = nullptr;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;

}

CMathOrderItem::~CMathOrderItem()
{
    delete m_pPropertyWgt;
}

/*! \fn CMathOrderItem::InitInOutItems()
*********************************************************************************************************
** \brief CMathOrderItem::InitInOutItems()
** \details 创建逻辑块显示内容
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    //根据不同功能逻辑块  显示不同的输入和输出
    if (GetItemTypNum() == LOGIC_MATH_MOVE || GetItemTypNum() == LOGIC_MATH_SQRT || GetItemTypNum() == LOGIC__MATH_ABS || GetItemTypNum() == LOGIC_MATH_LOG
        || GetItemTypNum() == LOGIC__MATH_LN || GetItemTypNum() == LOGIC_MATH_EXP || GetItemTypNum() == LOGIC_MATH_SIN || GetItemTypNum() == LOGIC_MATH_COS
        || GetItemTypNum() == LOGIC_MATH_TAN || GetItemTypNum() == LOGIC_MATH_ASIN || GetItemTypNum() == LOGIC_MATH_ACOS || GetItemTypNum() == LOGIC_MATH_ATAN)
    {
        lstInput << tr("IN");
        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX1);
        SetOutPutMaxNum(INOUT_MAX1);
    }
    else if (GetItemTypNum() == LOGIC_MATH_ADD || GetItemTypNum() == LOGIC_MATH_MUL || GetItemTypNum() == LOGIC_MATH_MAX || GetItemTypNum() == LOGIC_MATH_MIN)
    {
        lstInput << tr("IN1") << tr("IN2") << tr("IN3") << tr("IN4") << tr("IN5") << tr("IN6") << tr("IN7")
            << tr("IN8") << tr("IN9") << tr("IN10") << tr("IN11") << tr("IN12") << tr("IN13") << tr("IN14")
            << tr("IN15") << tr("IN16");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX16);
        SetOutPutMaxNum(INOUT_MAX1);
    }
    else if (GetItemTypNum() == LOGIC_MATH_SUB || GetItemTypNum() == LOGIC_MATH_DIV || GetItemTypNum() == LOGIC_MATH_MOD)
    {
        lstInput << tr("IN1")<<tr("IN2");
        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX2);
        SetOutPutMaxNum(INOUT_MAX1);
    }

    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

/*! \fn CMathOrderItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseItemType(int nType)
** \details 根据类型编号  找到对应的字符
** \param  nType:类型编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString CMathOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == LOGIC_MATH_MOVE)
    {
        return QObject::tr("MOVE");
    }
    else if (nType == LOGIC_MATH_ADD)
    {
        return QObject::tr("ADD");
    }
    else if (nType == LOGIC_MATH_SUB)
    {
        return QObject::tr("SBU");
    }
    else if (nType == LOGIC_MATH_MUL)
    {
        return QObject::tr("MUL");
    }
    else if (nType == LOGIC_MATH_DIV)
    {
        return QObject::tr("DIV");
    }
    else if (nType == LOGIC_MATH_MOD)
    {
        return QObject::tr("MOD");
    }
    else if (nType == LOGIC_MATH_SQRT)
    {
        return QObject::tr("SQRT");
    }
    else if (nType == LOGIC__MATH_ABS)
    {
        return QObject::tr("ABS");
    }
    else if (nType == LOGIC_MATH_LOG)
    {
        return QObject::tr("LOG");
    }
    else if (nType == LOGIC__MATH_LN)
    {
        return QObject::tr("LN");
    }
    else if (nType == LOGIC_MATH_EXP)
    {
        return QObject::tr("EXP");
    }
    else if (nType == LOGIC_MATH_EXPT)
    {
        return QObject::tr("EXPT");
    }
    else if (nType == LOGIC_MATH_SIN)
    {
        return QObject::tr("SIN");
    }
    else if (nType == LOGIC_MATH_COS)
    {
        return QObject::tr("COS");
    }
    else if (nType == LOGIC_MATH_TAN)
    {
        return QObject::tr("TAN");
    }
    else if (nType == LOGIC_MATH_ASIN)
    {
        return QObject::tr("ASIN");
    }
    else if (nType == LOGIC_MATH_ACOS)
    {
        return QObject::tr("ACOS");
    }
    else if (nType == LOGIC_MATH_ATAN)
    {
        return QObject::tr("ATAN");
    }
    else if (nType == LOGIC_MATH_MAX)
    {
        return QObject::tr("MAX");
    }
    else if (nType == LOGIC_MATH_MIN)
    {
        return QObject::tr("MIN");
    }
    else
    {
        return QObject::tr("UnKnown");
    }
}

/*! \fn CMathOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CMathOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details 鼠标双击事件
** \param  
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //双击显示详细信息
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

/*! \fn CMathOrderItem::AnalseBitInput()
*********************************************************************************************************
** \brief CMathOrderItem::AnalseBitInput()
** \details 根据不同类型 进行不同的计算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalseBitInput()
{
    //分析EN
    if (m_nOrderType == LOGIC_MATH_MOVE)
    {
        //赋值
        AnalyseMoveMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_ADD)
    {
        //加法
        AnalyseAddMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_SUB)
    {
        //减法
        AnalyseSubMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_MUL)
    {
        //乘法
        AnalyseMulMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_DIV)
    {
        //除法
        AnalyseDivMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_MOD)
    {
        AnalyseModMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_SQRT)
    {
        AnalyseSqrtMethod();
    }
    else if (m_nOrderType == LOGIC__MATH_ABS)
    {
        AnalyseAbsMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_LOG)
    {
        AnalyseLogMethod();
    }
    else if (m_nOrderType == LOGIC__MATH_LN)
    {
        AnalyseLnMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_EXP)
    {
        AnalyseExpMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_EXPT)
    {
        //
    }
    else if (m_nOrderType == LOGIC_MATH_SIN)
    {
        AnalyseSinMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_COS)
    {
        AnalyseCosMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_TAN)
    {
        AnalyseTanMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_ASIN)
    {
        AnalyseASinMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_ACOS)
    {
        AnalyseACosMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_ATAN)
    {
        AnalyseATanMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_MAX)
    {
        AnalyseAmaxMethod();
    }
    else if (m_nOrderType == LOGIC_MATH_MIN)
    {
        AnalyseAMinMethod();
    }

    //根据计算结果 对逻辑线进行更新
    QList<CConnectionRouter*> lstLines = GetAllLines();

    if (m_nItemOutValue == 0)
    {
        //为真是 为蓝线
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
        //为假是为红线
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

int CMathOrderItem::GetLogicValue()
{
    return m_nItemOutValue;
}

/*! \fn CMathOrderItem::AnalyseMoveMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseMoveMethod()
** \details 赋值运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseMoveMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            nValue = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

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

/*! \fn CMathOrderItem::AnalyseAddMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseAddMethod()
** \details 加法运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseAddMethod()
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
                    nValue = nValue + it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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

/*! \fn CMathOrderItem::AnalyseSubMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseSubMethod()
** \details 减法运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseSubMethod()
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

            nValue = nInput - nNumber;

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

/*! \fn CMathOrderItem::AnalyseMulMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseMulMethod()
** \details 乘法运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseMulMethod()
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
                    nValue = nValue * it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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

/*! \fn CMathOrderItem::AnalyseDivMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseDivMethod()
** \details 除法运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseDivMethod()
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

            if (nNumber != 0)
            {
                nValue = nInput / nNumber;
            }

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

/*! \fn CMathOrderItem::AnalyseModMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseModMethod()
** \details 求余运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseModMethod()
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

            if (nNumber != 0)
            {
                nValue = nInput % nNumber;
            }

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

/*! \fn CMathOrderItem::AnalyseSqrtMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseSqrtMethod()
** \details 平方根运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseSqrtMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = sqrt(nInput);

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

/*! \fn CMathOrderItem::AnalyseAbsMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseAbsMethod()
** \details abs运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseAbsMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = abs(nInput);

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

/*! \fn CMathOrderItem::AnalyseLogMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseLogMethod()
** \details log运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseLogMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = log(nInput);

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

/*! \fn CMathOrderItem::AnalyseLnMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseLnMethod()
** \details 自然对数运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseLnMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = log10(nInput);

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

/*! \fn CMathOrderItem::AnalyseExpMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseExpMethod()
** \details 指数运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseExpMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = exp(nInput);

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

/*! \fn CMathOrderItem::AnalyseSinMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseSinMethod()
** \details 正弦运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseSinMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = sin(nInput);

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

/*! \fn CMathOrderItem::AnalyseCosMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseCosMethod()
** \details 余弦运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseCosMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = cos(nInput);

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

/*! \fn CMathOrderItem::AnalyseTanMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseTanMethod()
** \details tan运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseTanMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = tan(nInput);

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

/*! \fn CMathOrderItem::AnalyseASinMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseASinMethod()
** \details 反正弦运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseASinMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = asin(nInput);

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

/*! \fn CMathOrderItem::AnalyseACosMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseACosMethod()
** \details 反余弦运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseACosMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = acos(nInput);

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

/*! \fn CMathOrderItem::AnalyseATanMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseATanMethod()
** \details 反正切运算
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseATanMethod()
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

    if (mapProInfo.contains(2))
    {
        if (GetBinMapInfo().contains(2))
        {
            int nInput = mapProInfo[2]->GetOutputData()[GetBinMapInfo()[2]];

            nValue = atan(nInput);

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

/*! \fn CMathOrderItem::AnalyseAmaxMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseAmaxMethod()
** \details 最大值
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseAmaxMethod()
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
                    if (nValue < it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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

/*! \fn CMathOrderItem::AnalyseAMinMethod()
*********************************************************************************************************
** \brief CMathOrderItem::AnalyseAMinMethod()
** \details 最小值
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMathOrderItem::AnalyseAMinMethod()
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
                    if (nValue > it.value()->GetOutputData()[GetBinMapInfo()[it.key()]])
                    {
                        nValue = it.value()->GetOutputData()[GetBinMapInfo()[it.key()]];
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
