#include "createtimecolumn.h"

QCreateTimeColumn::QCreateTimeColumn(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    InitDay();
    ui.comboBox->setCurrentIndex(0);
    setFixedSize(this->width(), this->height());  
}

QCreateTimeColumn::~QCreateTimeColumn()
{

}

void QCreateTimeColumn::slot_on_reportTypeChanged(int index)
{
    switch(index)
    {
        case 0://DAY
            {
                ui.comboBox_2->clear();
                InitDay();
            }
            break;
        case 1://MONTH
            {
                ui.comboBox_2->clear();
                InitMonth();
            }
            break;
        case 2://YEAR
            {
                ui.comboBox_2->clear();
                InitYear();
            }
            break;
        case 3://SPECIAL
            {
                ui.comboBox_2->clear();
                InitSpecial();
            }
            break;
        default:
            break;
    }
}

void QCreateTimeColumn::InitDay()
{
	ui.comboBox_2->addItem(tr("5分钟"));
	ui.comboBox_2->addItem(tr("15分钟"));
	ui.comboBox_2->addItem(tr("30分钟"));
	ui.comboBox_2->addItem(tr("1小时"));
	ui.comboBox_2->addItem(tr("2小时"));
	ui.comboBox_2->addItem(tr("3小时"));
   /*  ui.comboBox_2->addItem(tr("5 mins"));
    ui.comboBox_2->addItem(tr("15 mins"));
    ui.comboBox_2->addItem(tr("30 mins"));
    ui.comboBox_2->addItem(tr("1 hour"));
    ui.comboBox_2->addItem(tr("2 hours"));
    ui.comboBox_2->addItem(tr("3 hours"));*/
}

void QCreateTimeColumn::InitMonth()
{
	ui.comboBox_2->addItem(tr("1天"));
	ui.comboBox_2->addItem(tr("2天"));
	ui.comboBox_2->addItem(tr("3天"));
   /* ui.comboBox_2->addItem(tr("1 day"));
    ui.comboBox_2->addItem(tr("2 days"));
    ui.comboBox_2->addItem(tr("3 days"));*/
}
void QCreateTimeColumn::InitYear()
{
	ui.comboBox_2->addItem(tr("1月"));
	ui.comboBox_2->addItem(tr("2月"));

 /*   ui.comboBox_2->addItem(tr("1 month"));
    ui.comboBox_2->addItem(tr("2 months"));*/
}
void QCreateTimeColumn::InitSpecial()
{

}

void QCreateTimeColumn::slot_on_produce()
{
    NTIME.dataType=ui.comboBox->currentIndex();
    NTIME.stepIncrease=ui.comboBox_2->currentIndex();

    QDialog::accept();
}

TimeIncrease& QCreateTimeColumn::getResult()
{
    return NTIME;
}