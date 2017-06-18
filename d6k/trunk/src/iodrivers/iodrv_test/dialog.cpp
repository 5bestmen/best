#include "dialog.h"
#include "ui_dialog.h"
#include "fesapi/fesapi.h"

#include <QMessageBox>
#include <QLibrary>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_btn_start_clicked()
{
	bool bRet = OpenIO(0);

}
