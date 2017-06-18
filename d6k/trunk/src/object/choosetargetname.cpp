#include "choosetargetname.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>

CChooseTargetName::CChooseTargetName(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setWindowIcon(QIcon(":/images/choose.ico"));
    setWindowTitle(QStringLiteral("TargetNameѡ��"));
    setWindowModality(Qt::ApplicationModal);

    move(QApplication::desktop()->screenGeometry().center() - rect().center());

    ui.TNComboBox->addItem(QStringLiteral("֧��ģ����ѯ"));
    ui.TNComboBox->addItem(QStringLiteral("֧��ģ����ѯ"));
    //ui.TNComboBox->insertItem(2, tr("Insert item"));
    //ui.TNComboBox->insertSeparator(2);

    this->setFixedSize(this->width(), this->height());
    /*QComboBox* pTargetNameCB = new QComboBox();
    pTargetNameCB->addItem(QWidget::tr("male"));
    pTargetNameCB->addItem(QWidget::tr("female"));
    pTargetNameCB->insertItem(2, tr("Insert item"));
    pTargetNameCB->insertSeparator(2);*/
}

CChooseTargetName::~CChooseTargetName()
{

}
