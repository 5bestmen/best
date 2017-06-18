#ifndef SYCSCONFIGDLG_H
#define SYCSCONFIGDLG_H

#include <QDialog>
#include <QDateTime>
#include "ui_sycsconfigdlg.h"

class CSycsConfigDlg : public QDialog
{
    Q_OBJECT

public:
    CSycsConfigDlg(QWidget *parent = 0);
    ~CSycsConfigDlg();
public slots:
    void Slot_Comform();

signals:
    void Signal_SycsTime(QDateTime sycsTime);

private:
    Ui::CSycsConfigDlg ui;
};

#endif // SYCSCONFIGDLG_H
