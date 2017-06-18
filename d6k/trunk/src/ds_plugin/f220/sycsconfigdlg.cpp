#include "sycsconfigdlg.h"

CSycsConfigDlg::CSycsConfigDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    ui.timeEdit_2->setTime(QDateTime::currentDateTime().time());
    //确定
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
        //系统时间
        tSycsTime = QDateTime::currentDateTime();
    }
    else
    {
        //当前设置时间
        tSycsTime.setDate(ui.calendarWidget->selectedDate());
        tSycsTime.setTime(ui.timeEdit_2->time());
    }

    emit Signal_SycsTime(tSycsTime);
}
