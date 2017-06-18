#include "faulteventwgt.h"
#include "commthread.h"
#include "pointinfo.h"
#include "infoconfigwgt.h"

CFaultEventWgt::CFaultEventWgt(CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Q_UNUSED(pPointInfo);
    Q_UNUSED(pConfgWgt);

    Q_ASSERT(pCommunicate);
    if (pCommunicate == nullptr)
    {
        return;
    }

    m_pCommunicate = pCommunicate;

    ui.tableWidget->setColumnWidth(0, 100);
    ui.tableWidget->setColumnWidth(1, 100);
    ui.tableWidget->setColumnWidth(2, 100);
    ui.tableWidget->setColumnWidth(3, 100);
    ui.tableWidget->setColumnWidth(4, 100);
    ui.tableWidget->setColumnWidth(5, 200);
    ui.tableWidget->verticalHeader()->setHidden(true);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->horizontalHeader()->setSelectionBehavior(QAbstractItemView::SelectRows);
//     ui.tableWidget->setColumnHidden(6, true);
    ui.treeWidget->setHidden(true);
    

    connect(m_pCommunicate, SIGNAL(Signal_MalFuction(int, int, int, QString, int, QMap<int, int>)), this, SLOT(Slot_MalFuction(int, int, int, QString, int, QMap<int, int>)));

//     QMap<int, int> ttt;
//     ttt.insert(3344,311);
//     ttt.insert(6677, 44511);
//     ttt.insert(6543, 3445);
//     ttt.insert(3344, 311);
// 
//     Slot_MalFuction(1,2,3,"2016-10-26 326",1,ttt);
}

CFaultEventWgt::~CFaultEventWgt()
{

}

void CFaultEventWgt::Slot_MalFuction(int nBinaryType, int nBinaryAddr, int nBinaryValue, QString strTime, int nAnalogType, QMap<int, int> mapData)
{
    //往表格里插入数据
    int nRow = ui.tableWidget->rowCount();

    ui.tableWidget->insertRow(nRow);
    
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QString::number(nRow+1));
    ui.tableWidget->setItem(nRow, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QString::number(nBinaryType));
    ui.tableWidget->setItem(nRow, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setText(QString::number(nBinaryAddr));
    ui.tableWidget->setItem(nRow, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem;
    item3->setText(QString::number(nBinaryValue));
    ui.tableWidget->setItem(nRow, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem;
    item4->setText(QString::number(nAnalogType));
    ui.tableWidget->setItem(nRow, 4, item4);

    QTableWidgetItem *item5 = new QTableWidgetItem;
    item5->setText(strTime);
    ui.tableWidget->setItem(nRow, 5, item5);



    QMap<int, int>::const_iterator it = mapData.constBegin();

    QString strData;
    while (it != mapData.constEnd())
    {
        strData.append(QString::number(it.key())); 
        strData.append(" : ");
        strData.append(QString::number(it.value()));

        strData.append("     ");
        it++;
    }

    QTableWidgetItem *item6 = new QTableWidgetItem;
    item6->setText(strData);
    ui.tableWidget->setItem(nRow, 6, item6);

    ui.tableWidget->scrollToBottom();
}
