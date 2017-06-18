/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  funblockdiagramitem.cpp
文件实现功能 :  逻辑块
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
#include "funblockdiagramitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"

/*! \fn CFdbOrderView::CFunBlockDiagramItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CFdbOrderView::CFunBlockDiagramItem(int nItemType, int nCreateNum)
** \details 构造函数
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CFunBlockDiagramItem::CFunBlockDiagramItem(int nItemType, int nCreateNum)
{
    m_pPropertyWgt = nullptr;
    SetCreateNum(nCreateNum);
    SetItemType(nItemType);

    InitInOutItems();

    m_nItemOutValue = 0;
}

CFunBlockDiagramItem::~CFunBlockDiagramItem()
{
    delete m_pPropertyWgt;
}

/*! \fn CFdbOrderView::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief CFdbOrderView::AnalyseItemType(int nType)
** \details 根据类型编号  返回类型编码
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString CFunBlockDiagramItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == LOGIC_AND)
    {
        //设置默认高度
        SetCurrentInputNum(4);
        return QObject::tr("AND");
    }
    else if (nType == LOGIC_OR)
    {
        //设置默认高度
        SetCurrentInputNum(4);
        return QObject::tr("OR");
    }
    else if (nType == LOGIC_XOR)
    {
        //设置默认高度
        SetCurrentInputNum(4);
        return QObject::tr("XOR");
    }
    else if (nType == LOGIC_NOT)
    {
        //设置默认高度
        SetCurrentInputNum(1);
        return QObject::tr("NOT");
    }
    else
    {
        //设置默认高度
        SetCurrentInputNum(4);
        return QObject::tr("UnKnown");
    }
}

/*! \fn CFdbOrderView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief CFdbOrderView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details 双击
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFunBlockDiagramItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    //双击显示属性详细信息
    if (m_pPropertyWgt == nullptr)
    {
        m_pPropertyWgt = new CLogicPropertyWgt;
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
		connect(m_pPropertyWgt, SIGNAL(Signal_ChangeBindValue(QMap<int, QString> &)), this, SLOT(Slot_updateBindValueInfo(QMap<int, QString> &)));

    }

    //更新端子信息
    UpdateCurrentItemInfo();
    m_pPropertyWgt->SetItemTypeCode(m_nOrderType);
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());
    //输入详细信息
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    m_pPropertyWgt->SetComment(GetCommentData());
    //m_pPropertyWgt->UpdateItemPro(GetAllInputPro(), GetOutputPro());
    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}

/*! \fn CFdbOrderView::AnalseBitInput()
*********************************************************************************************************
** \brief CFdbOrderView::AnalseBitInput()
** \details 根据类型  分析方法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFunBlockDiagramItem::AnalseBitInput()
{
    //分析EN
    if (m_nOrderType == LOGIC_AND)
    {
        //and方法
        AnalyseAndMethod();
    }
    else if (m_nOrderType == LOGIC_OR)
    {
        //or方法
        AnalyseOrMethod();
    }
    else if (m_nOrderType == LOGIC_NOT)
    {
        //not算法
        AnalyseNotMethod();
    }
    else if (m_nOrderType == LOGIC_XOR)
    {
        //xor算法
        AnalyseXorMethod();
    }

    //根据结果 改变连接线状态
    QList<CConnectionRouter*> lstLines = GetAllLines();

    if (m_nItemOutValue == 0)
    {
        //真为蓝线
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
        //假为红线
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

/*! \fn CFdbOrderView::InitInOutItems()
*********************************************************************************************************
** \brief CFdbOrderView::InitInOutItems()
** \details 初始化 信息
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CFunBlockDiagramItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    if (GetItemTypNum() == LOGIC_AND || GetItemTypNum() == LOGIC_OR || GetItemTypNum() == LOGIC_XOR )
    {
        //输入端子显示内容
        lstInput << tr("IN1") << tr("IN2") << tr("IN3") << tr("IN4") << tr("IN5") << tr("IN6") << tr("IN7")
            << tr("IN8") << tr("IN9") << tr("IN10") << tr("IN11") << tr("IN12") << tr("IN13") << tr("IN14")
            << tr("IN15") << tr("IN16");

        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX16);
        SetOutPutMaxNum(INOUT_MAX1);
    }
    else if (GetItemTypNum() == LOGIC_NOT)
    {
        //输入端子显示内容
        lstInput << tr("IN");
        lstOutput << tr("OUT");

        SetInputMaxNum(INOUT_MAX1);
        SetOutPutMaxNum(INOUT_MAX1);
    }

    //设置管脚名称
    SetInputPinNames(lstInput);
    //
    SetOutputPinNames(lstOutput);
}

/*! \fn CFdbOrderView::AnalyseAndMethod()
*********************************************************************************************************
** \brief CFdbOrderView::AnalyseAndMethod()
** \details and算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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


/*! \fn CFdbOrderView::AnalyseOrMethod()
*********************************************************************************************************
** \brief CFdbOrderView::AnalyseOrMethod()
** \details or算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CFdbOrderView::AnalyseNotMethod()
*********************************************************************************************************
** \brief CFdbOrderView::AnalyseNotMethod()
** \details not算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CFdbOrderView::AnalyseXorMethod()
*********************************************************************************************************
** \brief CFdbOrderView::AnalyseXorMethod()
** \details xor算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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
