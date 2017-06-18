#include <QMimeData>
#include <QDrag>
#include "fdbitemchoosewgt.h"
#include "seqdefine.h"


CFdbItemChooseWgt::CFdbItemChooseWgt(QWidget *parent)
    : QTreeWidget(parent)
{
    setColumnCount(1);
    setHeaderLabel(tr("FDB"));
    //
    //设置fbd items
    CreateFdbTreeItems();
}

CFdbItemChooseWgt::~CFdbItemChooseWgt()
{

}

void CFdbItemChooseWgt::CreateFdbTreeItems()
{
    QTreeWidgetItem *pFdbOrderItem = new QTreeWidgetItem(this, QStringList(tr("FBD Order")));
    pFdbOrderItem->setData(0, Qt::UserRole, FDBORDER);

    //位指令
    QTreeWidgetItem *pBitOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Bit Order")));
    CreateBitChildItems(pBitOrderItem);
    //位移指令
    QTreeWidgetItem *pMoveOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Move Order")));
    CreateMoveChildItems(pMoveOrderItem);
    //数学指令
    QTreeWidgetItem *pMathOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Math Order")));
    CreateMathChildItems(pMathOrderItem);
    //比较指令
    QTreeWidgetItem *pCompareOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Compare Order")));
    CreateCompareChildItems(pCompareOrderItem);
    //控制指令
    QTreeWidgetItem *pProjectControlItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Project Control")));
    CreateHighLevelChildItems(pProjectControlItem);
    //定时器
    QTreeWidgetItem *pTimersOrderItem = new QTreeWidgetItem(pFdbOrderItem, QStringList(tr("Timers")));
    CreateTimerChildItems(pTimersOrderItem);
}

void CFdbItemChooseWgt::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
}

void CFdbItemChooseWgt::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);
    //只有子节点可以拖动
    if (currentItem()->parent() == nullptr)
    {
        return;
    }

    if (currentItem()->parent()->parent() == nullptr)
    {
        return;
    }

    if (currentItem()->parent()->parent()->data(0, Qt::UserRole) != FDBORDER)
    {
        return;
    }


    QByteArray itemData;
    itemData.setNum(currentItem()->data(0, Qt::UserRole).toInt());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("text/plain", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);

    QPixmap pixmap;
    pixmap.load(":/images/logicmove.png");
    drag->setPixmap(pixmap);
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {

    }
}

void CFdbItemChooseWgt::CreateBitChildItems(QTreeWidgetItem *pBitOrderItem)
{
    QTreeWidgetItem *pFdbAndItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("AND")));
    pFdbAndItem->setData(0, Qt::UserRole, BITAND);

    QTreeWidgetItem *pFdbOrItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("OR")));
    pFdbOrItem->setData(0, Qt::UserRole, BITOR);

    QTreeWidgetItem *pFdbNotItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("NOT")));
    pFdbNotItem->setData(0, Qt::UserRole, BITNOT);

    QTreeWidgetItem *pFdbxOrItem = new QTreeWidgetItem(pBitOrderItem, QStringList(tr("XOR")));
    pFdbxOrItem->setData(0, Qt::UserRole, BITXOR);
}

void CFdbItemChooseWgt::CreateMoveChildItems(QTreeWidgetItem *pMoveOrderItem)
{
    QTreeWidgetItem *pFdbShlItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("SHL")));
    pFdbShlItem->setData(0, Qt::UserRole, BITSHL);

    QTreeWidgetItem *pFdbShrItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("SHR")));
    pFdbShrItem->setData(0, Qt::UserRole, BITSHR);

    QTreeWidgetItem *pFdbROLItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("ROL")));
    pFdbROLItem->setData(0, Qt::UserRole, BITROL);

    QTreeWidgetItem *pFdbRORItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("ROR")));
    pFdbRORItem->setData(0, Qt::UserRole, BITROR);

    QTreeWidgetItem *pFdbBtstItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BTST")));
    pFdbBtstItem->setData(0, Qt::UserRole, BITBTST);

    QTreeWidgetItem *pFdbBsetItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BSET")));
    pFdbBsetItem->setData(0, Qt::UserRole, BITBSET);

    QTreeWidgetItem *pFdbBclrItem = new QTreeWidgetItem(pMoveOrderItem, QStringList(tr("BCLR")));
    pFdbBclrItem->setData(0, Qt::UserRole, BITCLR);
}

void CFdbItemChooseWgt::CreateMathChildItems(QTreeWidgetItem *pMathOrderItem)
{
    QTreeWidgetItem *pFdbMoveItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MOVE")));
    pFdbMoveItem->setData(0, Qt::UserRole, MATHMOVE);

    QTreeWidgetItem *pFdbAddItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ADD")));
    pFdbAddItem->setData(0, Qt::UserRole, MATHADD);

    QTreeWidgetItem *pFdbSubItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SUB")));
    pFdbSubItem->setData(0, Qt::UserRole, MATHSUB);

    QTreeWidgetItem *pFdbMulItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MUL")));
    pFdbMulItem->setData(0, Qt::UserRole, MATHMUL);

    QTreeWidgetItem *pFdbDivItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("DIV")));
    pFdbDivItem->setData(0, Qt::UserRole, MATHDIV);

    QTreeWidgetItem *pFdbModItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MOD")));
    pFdbModItem->setData(0, Qt::UserRole, MATHMOD);

    QTreeWidgetItem *pFdbSqrtItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SQRT")));
    pFdbSqrtItem->setData(0, Qt::UserRole, MATHSQRT);

    QTreeWidgetItem *pFdbAbsItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ABS")));
    pFdbAbsItem->setData(0, Qt::UserRole, MATHABS);

    QTreeWidgetItem *pFdbLOgItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("LOG")));
    pFdbLOgItem->setData(0, Qt::UserRole, MATHLOG);

    QTreeWidgetItem *pFdbLnItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("LN")));
    pFdbLnItem->setData(0, Qt::UserRole, MATHLN);

    QTreeWidgetItem *pFdbExpItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("EXP")));
    pFdbExpItem->setData(0, Qt::UserRole, MATHEXP);

    QTreeWidgetItem *pFdbExptItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("EXPT")));
    pFdbExptItem->setData(0, Qt::UserRole, MATHEXPT);

    QTreeWidgetItem *pFdbSinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("SIN")));
    pFdbSinItem->setData(0, Qt::UserRole, MATHSIN);

    QTreeWidgetItem *pFdbCosItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("COS")));
    pFdbCosItem->setData(0, Qt::UserRole, MATHCOS);

    QTreeWidgetItem *pFdbTanItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("TAN")));
    pFdbTanItem->setData(0, Qt::UserRole, MATHTAN);

    QTreeWidgetItem *pFdbAsinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ASIN")));
    pFdbAsinItem->setData(0, Qt::UserRole, MATHASIN);

    QTreeWidgetItem *pFdbAcosItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ACOS")));
    pFdbAcosItem->setData(0, Qt::UserRole, MATHACOS);

    QTreeWidgetItem *pFdbAtantItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("ATAN")));
    pFdbAtantItem->setData(0, Qt::UserRole, MATHATAN);

    QTreeWidgetItem *pFdbMaxItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MAX")));
    pFdbMaxItem->setData(0, Qt::UserRole, MATHMAX);

    QTreeWidgetItem *pFdbMinItem = new QTreeWidgetItem(pMathOrderItem, QStringList(tr("MIN")));
    pFdbMinItem->setData(0, Qt::UserRole, MATHMIN);

}

void CFdbItemChooseWgt::CreateCompareChildItems(QTreeWidgetItem *pCompareOrderItem)
{
    QTreeWidgetItem *pFdbEqItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("EQ")));
    pFdbEqItem->setData(0, Qt::UserRole, COMPAREEQ);

    QTreeWidgetItem *pFdbGeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("GE")));
    pFdbGeItem->setData(0, Qt::UserRole, COMPAREGE);

    QTreeWidgetItem *pFdbGtItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("GT")));
    pFdbGtItem->setData(0, Qt::UserRole, COMPAREGT);

    QTreeWidgetItem *pFdbLeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("LE")));
    pFdbLeItem->setData(0, Qt::UserRole, COMPARELE);

    QTreeWidgetItem *pFdbLtItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("LT")));
    pFdbLtItem->setData(0, Qt::UserRole, COMPARELT);

    QTreeWidgetItem *pFdbNeItem = new QTreeWidgetItem(pCompareOrderItem, QStringList(tr("NE")));
    pFdbNeItem->setData(0, Qt::UserRole, COMPARENE);
}

void CFdbItemChooseWgt::CreateHighLevelChildItems(QTreeWidgetItem *pHighOrderItem)
{
    QTreeWidgetItem *pFdbCallChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("CALL SR")));
    pFdbCallChildItem->setData(0, Qt::UserRole, CALLSR);

    QTreeWidgetItem *pFdbGotoChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("GOTO")));
    pFdbGotoChildItem->setData(0, Qt::UserRole, GTOT);

    QTreeWidgetItem *pFdbLabelChildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("LABEL")));
    pFdbLabelChildItem->setData(0, Qt::UserRole, LABEL);

    QTreeWidgetItem *pFdbPidhildItem = new QTreeWidgetItem(pHighOrderItem, QStringList(tr("PID")));
    pFdbPidhildItem->setData(0, Qt::UserRole, PID);
}

void CFdbItemChooseWgt::CreateTimerChildItems(QTreeWidgetItem *pTimerOrderItem)
{
    QTreeWidgetItem *pFdbCtdItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTD")));
    pFdbCtdItem->setData(0, Qt::UserRole, TIMERCTD);

    QTreeWidgetItem *pFdbCtuItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTU")));
    pFdbCtuItem->setData(0, Qt::UserRole, TIMERCTU);

    QTreeWidgetItem *pFdbCtudItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("CTUD")));
    pFdbCtudItem->setData(0, Qt::UserRole, TIMERCTUD);

    QTreeWidgetItem *pFdbTofItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TOF")));
    pFdbTofItem->setData(0, Qt::UserRole, TIMERTOF);

    QTreeWidgetItem *pFdbTonItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TON")));
    pFdbTonItem->setData(0, Qt::UserRole, TIMERTON);

    QTreeWidgetItem *pFdbTpItem = new QTreeWidgetItem(pTimerOrderItem, QStringList(tr("TP")));
    pFdbTpItem->setData(0, Qt::UserRole, TIMERTP);
}
