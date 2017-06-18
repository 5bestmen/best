#include "modreporttype.h"

QModReportType::QModReportType(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());  
}

QModReportType::~QModReportType()
{

}

void QModReportType::slot_on_ok()
{
   selIndex=ui.comboBox->currentIndex();
   QDialog::accept();
}

int QModReportType::getResult()
{
    return selIndex;
}
