#ifndef SIMUDIALOG_H
#define SIMUDIALOG_H

#include <QDialog>
#include "ui_simudialog.h"

class CSimuDialog : public QDialog
{
	Q_OBJECT

public:
	CSimuDialog(QWidget *parent = 0);
	~CSimuDialog();

private:
	Ui::CSimuDialog ui;
};

#endif // SIMUDIALOG_H
