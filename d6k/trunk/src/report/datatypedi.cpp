#include "datatypedi.h"
#include "public_define.h"

QDataTypeDI::QDataTypeDI(QWidget *parent)
    : QDialog(parent)
{
        ui.setupUi(this);
        InitDialog();
        setFixedSize(this->width(), this->height());  
}

QDataTypeDI::~QDataTypeDI()
{

}

void QDataTypeDI::slot_on_cancel()
{
        QDialog::reject();
}
void QDataTypeDI::slot_on_ok()
{
        QDialog::accept();
}
void QDataTypeDI::slot_on_setbasedatatype()
{
        if (ui.dayruntime->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_DAYRUNTIME;
        }
        if (ui.daystoptime->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_DAYSTOPCOUNT;
        }
        if (ui.daytotalruntime->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_DAYRUNTIME;
        }
        if(ui.daytotalstopcount->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_DAYTOTALRUNTIME;
        }
}
void QDataTypeDI::slot_on_setstayicsdata()
{
        if (ui.totalruntime->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_TOTALRUNTIME;
        }
        if (ui.totalstopcount->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_TOTALSTOPCOUNT;
        }
        if (ui.maxstopcount->isChecked())
        {
            m_DataType.m_DataFlag=HISDATA_MAXSTOPCOUNT;
        }
}
void QDataTypeDI::slot_on_settime()
{
        ui.startDateEdit->setEnabled(true);
        ui.endDateEdit->setEnabled(true); 

        m_DataType.calTime.m_StartTime=ui.startDateEdit->text();
        m_DataType.calTime.m_EndTime=ui.endDateEdit->text();
}
void QDataTypeDI::InitDialog()
{
        m_DataType.calTime.m_EndTime=QString::null;
        m_DataType.calTime.m_StartTime=QString::null;
        m_DataType.increaseTime.m_nIndex=-1;
        m_DataType.increaseTime.indexStr=QString::null;
        m_DataType.m_DataFlag=-1;
        ui.TimeComboBox->setCurrentIndex(0);
        ui.startDateEdit->setEnabled(false);
        ui.endDateEdit->setEnabled(false);    
        SetStaticsFalse();
}
void QDataTypeDI::SetStaticsFalse()
{
        ui.totalruntime->setEnabled(false);
        ui.totalstopcount->setEnabled(false);
        ui.maxstopcount->setEnabled(false);
}
void QDataTypeDI::SetStaticsTrue()
{
        ui.totalruntime->setEnabled(true);
        ui.totalstopcount->setEnabled(true);
        ui.maxstopcount->setEnabled(true);
}

DIDataType& QDataTypeDI::GetDIdataType()
{
        m_DataType.increaseTime.indexStr=ui.TimeComboBox->currentText();

        m_DataType.increaseTime.m_nIndex=ui.TimeComboBox->currentIndex();

        return m_DataType;
}