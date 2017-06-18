#include "generatereport.h"

QGenerateReport::QGenerateReport(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    InitDialog();
    setFixedSize(this->width(), this->height());  
}

QGenerateReport::~QGenerateReport()
{
  
}

void QGenerateReport::slot_on_produceReport()
{
    m_Time=ui.dateEdit->date();
    QDialog::accept();
}

void QGenerateReport::InitDialog()
{  
   ui.dateEdit->setDate(QDate::currentDate());
}
QDate& QGenerateReport::getResult()
{
    return m_Time;
}
