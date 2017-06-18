/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :  moveorderitem.cpp
文件实现功能 :  位移运算
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
#include "moveorderitem.h"
#include "connectionrouter.h"
#include "seqdefine.h"
#include "fdborderview.h"
#include "logicpropertywgt.h"

/*! \fn CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CMoveOrderItem::CMoveOrderItem(int nItemType, int nCreateNum)
** \details 位移运算 构造函数
** \param  nItemType:逻辑块类型  nCreateNum:创建编号
** \return
** \author ww
** \date 2017年1月4日
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
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMoveOrderItem::InitInOutItems()
{
    //输入
    QStringList lstInput;
    //输出
    QStringList lstOutput;

    //根据不同类型 显示不同的输入和输出
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
** \details  根据类型编码  显示 类型标识
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString CMoveOrderItem::AnalyseItemType(int nType)
{
    m_nOrderType = nType;
    //显示内容
    if (nType == LOGIC_SHL)
    {
        //初始化输入输出个数
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
** \details  双击显示详情
** \param
** \return
** \author ww
** \date 2017年1月4日
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

    //更新端子信息
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
** \details  根据不同的类型 进行不同的算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CMoveOrderItem::AnalseBitInput()
{
    //分析EN
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
** \details  shl算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CMoveOrderItem::AnalyseShrMethod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseShrMethod()
** \details  shr算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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

/*! \fn CMoveOrderItem::AnalyseRolMethod()
*********************************************************************************************************
** \brief CMoveOrderItem::AnalyseRolMethod()
** \details  rol算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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
** \details  ror算法
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
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
** \details  左移
** \param
** \return
** \author ww
** \date 2017年1月4日
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
** \details  右移
** \param
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
unsigned char CMoveOrderItem::MoveRight(unsigned char uInput, unsigned uMoveNum)
{
    unsigned char right = uInput >> uMoveNum;
    unsigned char left = uInput << (sizeof(unsigned char) - uMoveNum);
    unsigned char temp = left | right;

    return temp;
}