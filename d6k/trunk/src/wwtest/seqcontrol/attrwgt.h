#ifndef ATTRWGT_H
#define ATTRWGT_H

#include <QWidget>
#include "ui_attrwgt.h"

class CAttrWgt : public QWidget
{
    Q_OBJECT

public:
    CAttrWgt(QWidget *parent = 0);
    ~CAttrWgt();
    void SetMastName();
    void SetFastName();
    void SetAuxName();
public slots:
    void Slot_Conform();

private:
    Ui::CAttrWgt ui;
};

#endif // ATTRWGT_H
