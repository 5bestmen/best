#ifndef PINCALLWGT_H
#define PINCALLWGT_H

#include <QDialog>
#include "ui_pincallwgt.h"

class CPinCallWgt : public QDialog
{
    Q_OBJECT

public:
    CPinCallWgt(QWidget *parent = 0);
    ~CPinCallWgt();

private:
    Ui::CPinCallWgt ui;
};

#endif // PINCALLWGT_H
