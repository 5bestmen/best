#include <QComboBox>
#include <QSpinBox>
#include "logicpropertywgt.h"
#include "seqdefine.h"
#include "pincallwgt.h"

CLogicPropertyWgt::CLogicPropertyWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_nItemTypeCode = 0;
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));

    ui.tableWidget->setColumnWidth(1, 300);
    ui.tableWidget->verticalHeader()->setHidden(true);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui.tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(Slot_DoubleClickTableItem(QTableWidgetItem *)));
}

CLogicPropertyWgt::~CLogicPropertyWgt()
{

}

void CLogicPropertyWgt::Slot_Conform()
{
    emit Signal_ConformComment(ui.textEdit->toPlainText());
    if (ui.checkBox->checkState() == Qt::Checked)
    {
        emit Signal_EnoDisplay(1);
    }
    else
    {
        emit Signal_EnoDisplay(0);
    }
    this->close();
}

void CLogicPropertyWgt::Slot_Cancel()
{
    this->close();
}

void CLogicPropertyWgt::UpdateItemPro(const QList<LOGICITEMPRO>& lstInput, const QList<LOGICITEMPRO>& lstOutput)
{
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);

    //输入行
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(tr("Input"));
    

    ui.tableWidget->insertRow(0);
    ui.tableWidget->setItem(0, 0, item0);
    ui.tableWidget->setRowHeight(0, 25);

    for (int i = 0; i < lstInput.count(); i++)
    {
        ui.tableWidget->insertRow(i + 1);
        ui.tableWidget->setRowHeight(i+1, 25);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(lstInput.at(i).strPinName);
        ui.tableWidget->setItem(i + 1, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        if (lstInput.at(i).nPinType == 1)
        {
            //output
            item1->setText("F" + QString::number(lstInput.at(i).outPinInfo.nCreateNum)
                + "_(" + QString::number(lstInput.at(i).outPinInfo.nExecNum) + 
                +")." + lstInput.at(i).outPinInfo.strOutPutPinName);

        }
        else if (lstInput.at(i).nPinType == 2)
        {
            //元件
            item1->setText(lstInput.at(i).sCellInfo.strCell);
        }
        else if (lstInput.at(i).nPinType == 3)
        {
            item1->setText(tr("null"));
        }

        ui.tableWidget->setItem(i + 1, 1, item1);

        if (i == 0)
        {
            //EN
            QTableWidgetItem *item2 = new QTableWidgetItem;
            item2->setText(tr("Enable"));
            ui.tableWidget->setItem(i + 1, 2, item2);
        }
        else
        {
            if (m_nItemTypeCode >= BITAND && m_nItemTypeCode <= BITXOR)
            {
                //逻辑运算
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setText(tr("Operator Bit %1").arg(i));
                ui.tableWidget->setItem(i + 1, 2, item2);

            }
            else if (m_nItemTypeCode == CALLSR)
            {
                //子程序
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setText(tr("Child Name"));
                ui.tableWidget->setItem(i + 1, 2, item2);
            }
        }

        if (lstInput.at(i).bEnFlag == false && i == 0)
        {
            ui.tableWidget->setRowHidden(i+1,true);
        }
        else
        {

        }
    }

    //输出行
    int nOutRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nOutRow);
    ui.tableWidget->setRowHeight(nOutRow, 25);

    QTableWidgetItem *itemOut = new QTableWidgetItem;
    itemOut->setText(tr("Output"));
    ui.tableWidget->setItem(nOutRow, 0, itemOut);

    for (int j = 0; j < lstOutput.count(); j++)
    {
        ui.tableWidget->insertRow(nOutRow + j + 1);

        ui.tableWidget->setRowHeight(nOutRow + j + 1, 25);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setText(lstOutput.at(j).strPinName);
        ui.tableWidget->setItem(nOutRow + j + 1, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;

        if (lstOutput.at(j).nPinType == 2)
       {
           //元件
           item1->setText(lstOutput.at(j).sCellInfo.strCell);
       }
        else if (lstOutput.at(j).nPinType == 3)
       {
           item1->setText(tr("null"));
       }

       ui.tableWidget->setItem(nOutRow + j + 1, 1, item1);

       if (j == 0)
       {
           //ENO
           QTableWidgetItem *item2 = new QTableWidgetItem;
           item2->setText(tr("Enable Output"));
           ui.tableWidget->setItem(nOutRow + j + 1, 2, item2);
       }
       else
       {
           if (m_nItemTypeCode >= BITAND && m_nItemTypeCode <= BITXOR)
           {
               //逻辑运算
               QTableWidgetItem *item2 = new QTableWidgetItem;
               item2->setText(tr("Status Output"));
               ui.tableWidget->setItem(nOutRow + j + 1, 2, item2);

           }
       }

       if (lstOutput.at(j).bEnFlag == false && j == 0)
       {
           ui.tableWidget->setRowHidden(nOutRow + j + 1, true);
       }
    }
}

void CLogicPropertyWgt::Slot_DoubleClickTableItem(QTableWidgetItem *pItem)
{
    //框架提供
    if (m_nItemTypeCode == CALLSR)
    {
        CPinCallWgt *pCallWgt = new CPinCallWgt;
        pCallWgt->exec();
    }
}

void CLogicPropertyWgt::SetItemTypeCode(int nCode)
{
    m_nItemTypeCode = nCode;
}

void CLogicPropertyWgt::SetChildLst(const QStringList &lstChildNames)
{
    m_lstChildNames = lstChildNames;
}
