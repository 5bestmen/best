#include "modfilename.h"

QModFileName::QModFileName(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    setFixedSize(this->width(), this->height());  
}

QModFileName::~QModFileName()
{

}
void QModFileName::slot_on_ok()
{
    m_szFileName=ui.lineEdit->text();
    QDialog::accept();
}
void QModFileName::slot_on_cancel()
{
    QDialog::reject();
}
QString& QModFileName::GetMyFileName()
{
    return m_szFileName;
}
