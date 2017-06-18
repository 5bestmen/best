#include "sycsconfigdlg.h"

CSycsConfigDlg::CSycsConfigDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.timeEdit_2->setTime(QDateTime::currentDateTime().time());
    //ȷ��
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Comform()));
}

CSycsConfigDlg::~CSycsConfigDlg()
{

}

void CSycsConfigDlg::Slot_Comform()
{
    QDateTime tSycsTime;
    if (ui.checkBox->checkState() == Qt::Checked)
    {
        //ϵͳʱ��
        tSycsTime = QDateTime::currentDateTime();
    }
    else
    {
        //��ǰ����ʱ��
        tSycsTime.setDate(ui.calendarWidget->selectedDate());
        tSycsTime.setTime(ui.timeEdit_2->time());
    }

    emit Signal_SycsTime(tSycsTime);
}
