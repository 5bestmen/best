#ifndef LCDCONTROLWGT_H
#define LCDCONTROLWGT_H

#include <QFrame>
#include "ui_lcdcontrolwgt.h"

class CLcdControlWgt : public QWidget
{
    Q_OBJECT

public:
    CLcdControlWgt(QWidget *parent = 0);
    ~CLcdControlWgt();
signals:
    void Signal_ClickOperator(int nType);
protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void Slot_RetClick();
    void Slot_AddClick();

    void Slot_UpClick();
    void Slot_LeftClick();

    void Slot_EtrClick();
    void Slot_RightClick();

    void Slot_DownClick();
    void Slot_CalClick();

    void Slot_DelClick();

private:
    Ui::CLcdControlWgt ui;
};

#endif // LCDCONTROLWGT_H
