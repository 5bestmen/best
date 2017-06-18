#include "prjectatrrwgt.h"

CPrjectAtrrWgt::CPrjectAtrrWgt(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Comform()));
    connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(close()));
}

CPrjectAtrrWgt::~CPrjectAtrrWgt()
{

}

void CPrjectAtrrWgt::SetprojectName(const QString &projectName)
{
    ui.lineEdit->setText(projectName);
}

QString CPrjectAtrrWgt::GetProjectName()
{
    return m_strProjectName;
}

QString CPrjectAtrrWgt::GetPassWord()
{
    return m_strPassWd;
}

QString CPrjectAtrrWgt::GetComment()
{
    return m_strComment;
}

void CPrjectAtrrWgt::Slot_Comform()
{
    m_strProjectName = ui.lineEdit->text();
    //TODO
    m_strPassWd = "xxx";

    m_strComment = ui.textEdit->toPlainText();
    emit Signal_Conform();

    this->close();
}

