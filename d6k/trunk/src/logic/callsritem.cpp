/*! @file
<PRE>
********************************************************************************
模块名       :  调用子程序 逻辑块
文件名       :  callsritem.cpp
文件实现功能 :  
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

#include "callsritem.h"
#include "logicpropertywgt.h"

/*! \fn CCallSRItem::CCallSRItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CCallSRItem(int nItemType, int nCreateNum)
** \details 构造函数
** \param nItemType：类型编码   nCreateNum:创建编号
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
CCallSRItem::CCallSRItem(int nItemType, int nCreateNum)
{
    //属性widget
    m_pPropertyWgt = nullptr;
    //设置显示创建编号
    SetCreateNum(nCreateNum);
    //设置类型
    SetItemType(nItemType);

    //最多为16个
    SetInputMaxNum(MAX_INPUTN_NUM);

    QStringList lstInput;
    lstInput << tr("CALL");

    SetInputPinNames(lstInput);
    //
    SetOutPutMaxNum(MAX_OUTPUT_NUM);
}

CCallSRItem::~CCallSRItem()
{
    if (m_pPropertyWgt != nullptr)
    {
        delete m_pPropertyWgt;
        m_pPropertyWgt = nullptr;
    }
    
}

/*! \fn CCallSRItem::AnalyseItemType(int nType)
*********************************************************************************************************
** \brief AnalyseItemType(int nType)
** \details 根据类型编码  返回显示的类型字符串
** \param nType：类型编码   
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
QString CCallSRItem::AnalyseItemType(int nType)
{
    if (nType == LOGIC_CALLSR)
    {
        return tr("SR");
    }

    return "";
}

/*! \fn CCallSRItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
*********************************************************************************************************
** \brief mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
** \details 双击显示详细信息
** \param nType：
** \return
** \author ww
** \date 2017年1月4日
** \note
********************************************************************************************************/
void CCallSRItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_pPropertyWgt == nullptr)
    {
        //创建详细信息widget
        m_pPropertyWgt = new CLogicPropertyWgt;
        //更新显示的注释信息
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        //更新是否显示en/eno
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
    }

    //读取输入和输出端子信息
    UpdateCurrentItemInfo();
    //设置类型,用于描述使用
    m_pPropertyWgt->SetItemTypeCode(LOGIC_CALLSR);
    //读取eno/en是否显示标志
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());

    //填充输入、输出信息
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    //填充注释
    m_pPropertyWgt->SetComment(GetCommentData());

    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}
