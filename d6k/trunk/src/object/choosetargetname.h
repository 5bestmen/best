#ifndef CHOOSETARGETNAME_H
#define CHOOSETARGETNAME_H

#include <QDialog>
#include "ui_choosetargetname.h"

class CChooseTargetName : public QDialog
{
    Q_OBJECT

public:
    CChooseTargetName(QWidget *parent = 0);
    ~CChooseTargetName();

private:
    Ui::choosetargetname ui;
};

#endif // CHOOSETARGETNAME_H
