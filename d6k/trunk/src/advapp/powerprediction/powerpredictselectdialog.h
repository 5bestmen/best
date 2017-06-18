#ifndef CSELECTDIALOG_H
#define CSELECTDIALOG_H

#include <QWidget>
#include "ui_cselectdialog.h"

class CSelectDialog : public QWidget
{
	Q_OBJECT

public:
	CSelectDialog(QWidget *parent = 0);
	~CSelectDialog();

private:
	Ui::cselectdialog ui;
};

#endif // CSELECTDIALOG_H
