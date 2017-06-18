/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fdbitemchoosewgt.cpp
�ļ�ʵ�ֹ��� :  �߼����б�
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
#include <QMimeData>
#include <QDrag>
#include "fdbitemchoosewgt.h"
#include "seqdefine.h"

/*! \fn CFdbItemChooseWgt::CFdbItemChooseWgt(QWidget *parent)
*********************************************************************************************************
** \brief CFdbItemChooseWgt(QWidget *parent)
** \details ���캯��
** \param   parent:������
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
CFdbItemChooseWgt::CFdbItemChooseWgt(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(1);
    setHeaderLabel(tr("FDB"));
    //
    //����fbd items
    CreateFdbTreeItems();
}

CFdbItemChooseWgt::~CFdbItemChooseWgt()
{

}

/*! \fn CFdbItemChooseWgt::CreateFdbTreeItems()
*********************************************************************************************************
** \brief CreateFdbTreeItems()
** \details ����item����
** \param   
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateFdbTreeItems()
{
    QTreeWidgetItem *pFdbOrderItem = new QTreeWidgetItem(this, QStringList(tr("FBD Order")));
    pFdbOrderItem->setData(0, Qt::UserRole, LOGIC_FDB_ORDER);
    pFdbOrderItem->setExpanded(true);

    //λָ��
    QTreeWidgetItem *pBitOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Bit Order")));
    CreateBitChildItems(pBitOrderItem);
    //λ��ָ��
    QTreeWidgetItem *pMoveOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Move Order")));
    CreateMoveChildItems(pMoveOrderItem);
    //��ѧָ��
    QTreeWidgetItem *pMathOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Math Order")));
    CreateMathChildItems(pMathOrderItem);
    //�Ƚ�ָ��
    QTreeWidgetItem *pCompareOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Compare Order")));
    CreateCompareChildItems(pCompareOrderItem);
    //����ָ��
    QTreeWidgetItem *pProjectControlItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Project Control")));
    CreateHighLevelChildItems(pProjectControlItem);
    //��ʱ��
    QTreeWidgetItem *pTimersOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Timers")));
    CreateTimerChildItems(pTimersOrderItem);
}

void CFdbItemChooseWgt::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
}

/*! \fn CFdbItemChooseWgt::mouseMoveEvent(QMouseEvent *mouseEvent)
*********************************************************************************************************
** \brief mouseMoveEvent(QMouseEvent *mouseEvent)
** \details ����ƶ�
** \param
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
    //ֻ���ӽڵ�����϶�
    if (currentItem() == nullptr)
    {
        return;
    }

    if (currentItem()->parent() == nullptr)
    {
        return;
    }

    if (currentItem()->parent()->parent() == nullptr)
    {
        return;
    }

    if (currentItem()->parent()->parent()->data(0, Qt::UserRole) != LOGIC_FDB_ORDER)
    {
        return;
    }

    //�϶������� Ϊ ���ͱ�ʶ
    QByteArray itemData;
    itemData.setNum(currentItem()->data(0, Qt::UserRole).toInt());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/plain", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    //�϶�ʱ��ʾ��ͼƬ
    QPixmap pixmap;
    pixmap.load(":/images/logicmove.png");
    drag->setPixmap(pixmap);
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {

    }
}

/*! \fn CFdbItemChooseWgt::CreateBitChildItems(QTreeWidgetItem *pBitOrderItem)
*********************************************************************************************************
** \brief CreateBitChildItems(QTreeWidgetItem *pBitOrderItem)
** \details ����bit�ӽڵ�
** \param  bit���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateBitChildItems(QTreeWidgetItem *pBitOrderItem)
{
    QTreeWidgetItem *pFdbAndItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("AND")));
    pFdbAndItem->setData(0, Qt::UserRole, LOGIC_AND);

    QTreeWidgetItem *pFdbOrItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("OR")));
    pFdbOrItem->setData(0, Qt::UserRole, LOGIC_OR);

    QTreeWidgetItem *pFdbNotItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("NOT")));
    pFdbNotItem->setData(0, Qt::UserRole, LOGIC_NOT);

    QTreeWidgetItem *pFdbxOrItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("XOR")));
    pFdbxOrItem->setData(0, Qt::UserRole, LOGIC_XOR);
}

/*! \fn CFdbItemChooseWgt::CreateMoveChildItems(QTreeWidgetItem *pBitOrderItem)
*********************************************************************************************************
** \brief CreateMoveChildItems(QTreeWidgetItem *pBitOrderItem)
** \details ����move�ӽڵ�
** \param  move���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateMoveChildItems(QTreeWidgetItem *pMoveOrderItem)
{
    QTreeWidgetItem *pFdbShlItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("SHL")));
    pFdbShlItem->setData(0, Qt::UserRole, LOGIC_SHL);

    QTreeWidgetItem *pFdbShrItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("SHR")));
    pFdbShrItem->setData(0, Qt::UserRole, LOGIC_SHR);

    QTreeWidgetItem *pFdbROLItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("ROL")));
    pFdbROLItem->setData(0, Qt::UserRole, LOGIC_ROL);

    QTreeWidgetItem *pFdbRORItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("ROR")));
    pFdbRORItem->setData(0, Qt::UserRole, LOGIC_ROR);

    QTreeWidgetItem *pFdbBtstItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BTST")));
    pFdbBtstItem->setData(0, Qt::UserRole, LOGIC_BTST);

    QTreeWidgetItem *pFdbBsetItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BSET")));
    pFdbBsetItem->setData(0, Qt::UserRole, LOGIC_BSET);

    QTreeWidgetItem *pFdbBclrItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BCLR")));
    pFdbBclrItem->setData(0, Qt::UserRole, LOGIC_CLR);
}

/*! \fn CFdbItemChooseWgt::CreateMathChildItems(QTreeWidgetItem *pBitOrderItem)
*********************************************************************************************************
** \brief CreateMathChildItems(QTreeWidgetItem *pBitOrderItem)
** \details ����math�ӽڵ�
** \param  math���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateMathChildItems(QTreeWidgetItem *pMathOrderItem)
{
    QTreeWidgetItem *pFdbMoveItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MOVE")));
    pFdbMoveItem->setData(0, Qt::UserRole, LOGIC_MATH_MOVE);

    QTreeWidgetItem *pFdbAddItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ADD")));
    pFdbAddItem->setData(0, Qt::UserRole, LOGIC_MATH_ADD);

    QTreeWidgetItem *pFdbSubItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SUB")));
    pFdbSubItem->setData(0, Qt::UserRole, LOGIC_MATH_SUB);

    QTreeWidgetItem *pFdbMulItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MUL")));
    pFdbMulItem->setData(0, Qt::UserRole, LOGIC_MATH_MUL);

    QTreeWidgetItem *pFdbDivItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("DIV")));
    pFdbDivItem->setData(0, Qt::UserRole, LOGIC_MATH_DIV);

    QTreeWidgetItem *pFdbModItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MOD")));
    pFdbModItem->setData(0, Qt::UserRole, LOGIC_MATH_MOD);

    QTreeWidgetItem *pFdbSqrtItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SQRT")));
    pFdbSqrtItem->setData(0, Qt::UserRole, LOGIC_MATH_SQRT);

    QTreeWidgetItem *pFdbAbsItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ABS")));
    pFdbAbsItem->setData(0, Qt::UserRole, LOGIC__MATH_ABS);

    QTreeWidgetItem *pFdbLOgItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("LOG")));
    pFdbLOgItem->setData(0, Qt::UserRole, LOGIC_MATH_LOG);

    QTreeWidgetItem *pFdbLnItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("LN")));
    pFdbLnItem->setData(0, Qt::UserRole, LOGIC__MATH_LN);

    QTreeWidgetItem *pFdbExpItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("EXP")));
    pFdbExpItem->setData(0, Qt::UserRole, LOGIC_MATH_EXP);

    QTreeWidgetItem *pFdbExptItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("EXPT")));
    pFdbExptItem->setData(0, Qt::UserRole, LOGIC_MATH_EXPT);

    QTreeWidgetItem *pFdbSinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SIN")));
    pFdbSinItem->setData(0, Qt::UserRole, LOGIC_MATH_SIN);

    QTreeWidgetItem *pFdbCosItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("COS")));
    pFdbCosItem->setData(0, Qt::UserRole, LOGIC_MATH_COS);

    QTreeWidgetItem *pFdbTanItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("TAN")));
    pFdbTanItem->setData(0, Qt::UserRole, LOGIC_MATH_TAN);

    QTreeWidgetItem *pFdbAsinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ASIN")));
    pFdbAsinItem->setData(0, Qt::UserRole, LOGIC_MATH_ASIN);

    QTreeWidgetItem *pFdbAcosItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ACOS")));
    pFdbAcosItem->setData(0, Qt::UserRole, LOGIC_MATH_ACOS);

    QTreeWidgetItem *pFdbAtantItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ATAN")));
    pFdbAtantItem->setData(0, Qt::UserRole, LOGIC_MATH_ATAN);

    QTreeWidgetItem *pFdbMaxItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MAX")));
    pFdbMaxItem->setData(0, Qt::UserRole, LOGIC_MATH_MAX);

    QTreeWidgetItem *pFdbMinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MIN")));
    pFdbMinItem->setData(0, Qt::UserRole, LOGIC_MATH_MIN);

}

/*! \fn CFdbItemChooseWgt::CreateCompareChildItems(QTreeWidgetItem *pBitOrderItem)
*********************************************************************************************************
** \brief CreateCompareChildItems(QTreeWidgetItem *pBitOrderItem)
** \details ����compare�ӽڵ�
** \param  compare���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateCompareChildItems(QTreeWidgetItem *pCompareOrderItem)
{
    QTreeWidgetItem *pFdbEqItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("EQ")));
    pFdbEqItem->setData(0, Qt::UserRole, LOGIC_COMPARE_EQ);

    QTreeWidgetItem *pFdbGeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("GE")));
    pFdbGeItem->setData(0, Qt::UserRole, LOGIC_COMPARE_GE);

    QTreeWidgetItem *pFdbGtItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("GT")));
    pFdbGtItem->setData(0, Qt::UserRole, LOGIC_COMPARE_GT);

    QTreeWidgetItem *pFdbLeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("LE")));
    pFdbLeItem->setData(0, Qt::UserRole, LOGIC_COMPARE_LE);

    QTreeWidgetItem *pFdbLtItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("LT")));
    pFdbLtItem->setData(0, Qt::UserRole, LOGIC_COMPARE_LT);

    QTreeWidgetItem *pFdbNeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("NE")));
    pFdbNeItem->setData(0, Qt::UserRole, LOGIC_COMPARE_NE);
}

/*! \fn CFdbItemChooseWgt::CreateHighLevelChildItems(QTreeWidgetItem *pBitOrderItem)
*********************************************************************************************************
** \brief CreateHighLevelChildItems(QTreeWidgetItem *pBitOrderItem)
** \details ����highlevel�ӽڵ�
** \param  highlevel���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateHighLevelChildItems(QTreeWidgetItem *pHighOrderItem)
{
    QTreeWidgetItem *pFdbCallChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("CALL SR")));
    pFdbCallChildItem->setData(0, Qt::UserRole, LOGIC_CALLSR);

    QTreeWidgetItem *pFdbGotoChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("GOTO")));
    pFdbGotoChildItem->setData(0, Qt::UserRole, LOGIC_GTOT);

    QTreeWidgetItem *pFdbLabelChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("LABEL")));
    pFdbLabelChildItem->setData(0, Qt::UserRole, LOGIC_LABEL);

    QTreeWidgetItem *pFdbPidhildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("PID")));
    pFdbPidhildItem->setData(0, Qt::UserRole, LOGIC_PID);
}

/*! \fn CFdbItemChooseWgt::CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem)
*********************************************************************************************************
** \brief CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem)
** \details ����timer�ӽڵ�
** \param  highlevel���ڵ�
** \return
** \author ww
** \date 2017��1��4��
** \note
********************************************************************************************************/
void CFdbItemChooseWgt::CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem)
{
    QTreeWidgetItem *pFdbCtdItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTD")));
    pFdbCtdItem->setData(0, Qt::UserRole, LOGIC_TIMERCTD);

    QTreeWidgetItem *pFdbCtuItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTU")));
    pFdbCtuItem->setData(0, Qt::UserRole, LOGIC_TIMER_CTU);

    QTreeWidgetItem *pFdbCtudItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTUD")));
    pFdbCtudItem->setData(0, Qt::UserRole, LOGIC_TIMER_CTUD);

    QTreeWidgetItem *pFdbTofItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TOF")));
    pFdbTofItem->setData(0, Qt::UserRole, LOGIC_TIMER_TOF);

    QTreeWidgetItem *pFdbTonItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TON")));
    pFdbTonItem->setData(0, Qt::UserRole, LOGIC_TIMER_TON);

    QTreeWidgetItem *pFdbTpItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TP")));
    pFdbTpItem->setData(0, Qt::UserRole, LOGIC_TIMER_TP);
}
