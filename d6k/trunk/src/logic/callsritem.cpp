/*! @file
<PRE>
********************************************************************************
ģ����       :  �����ӳ��� �߼���
�ļ���       :  callsritem.cpp
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

#include "callsritem.h"
#include "logicpropertywgt.h"

/*! \fn CCallSRItem::CCallSRItem(int nItemType, int nCreateNum)
*********************************************************************************************************
** \brief CCallSRItem(int nItemType, int nCreateNum)
** \details ���캯��
** \param nItemType�����ͱ���   nCreateNum:�������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CCallSRItem::CCallSRItem(int nItemType, int nCreateNum)
{
    //����widget
    m_pPropertyWgt = nullptr;
    //������ʾ�������
    SetCreateNum(nCreateNum);
    //��������
    SetItemType(nItemType);

    //���Ϊ16��
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
** \details �������ͱ���  ������ʾ�������ַ���
** \param nType�����ͱ���   
** \return
** \author ww
** \date 2017��1��4��
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
** \details ˫����ʾ��ϸ��Ϣ
** \param nType��
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CCallSRItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_pPropertyWgt == nullptr)
    {
        //������ϸ��Ϣwidget
        m_pPropertyWgt = new CLogicPropertyWgt;
        //������ʾ��ע����Ϣ
        connect(m_pPropertyWgt, SIGNAL(Signal_ConformComment(const QString &)), this, SLOT(Slot_UpdateComment(const QString &)));
        //�����Ƿ���ʾen/eno
        connect(m_pPropertyWgt, SIGNAL(Signal_EnoDisplay(int)), this, SLOT(Slot_UpdateEnoInfo(int)));
    }

    //��ȡ��������������Ϣ
    UpdateCurrentItemInfo();
    //��������,��������ʹ��
    m_pPropertyWgt->SetItemTypeCode(LOGIC_CALLSR);
    //��ȡeno/en�Ƿ���ʾ��־
    m_pPropertyWgt->SetEnoFlag(GetEnoFlag());

    //������롢�����Ϣ
    m_pPropertyWgt->UpdateItemDetail(GetInputPros(), GetOutPutPros());
    //���ע��
    m_pPropertyWgt->SetComment(GetCommentData());

    m_pPropertyWgt->activateWindow();
    m_pPropertyWgt->show();
}
