#ifndef EXPORTMODELDIALOG_H
#define EXPORTMODELDIALOG_H

#include <QDialog>
#include "ui_exportmodeldialog.h"

class CExportModelDialog : public QDialog
{
	Q_OBJECT

public:
	CExportModelDialog(QWidget *parent = 0);
	~CExportModelDialog();

	QString m_strFactory;
	QString m_strModelPath;
	QString m_strModelDesc;
public slots:
	void OnOk();
	void OnCancel();

private:
	Ui::CExportModelDialog ui;
};

#endif // EXPORTMODELDIALOG_H
