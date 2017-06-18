#ifndef TOTALSELECTDIALOG_H
#define TOTALSELECTDIALOG_H

#include <QDialog>
#include "ui_totalselectdialog.h"

class QMainWindow;

class CTotalSelectDialog : public QDialog
{
	Q_OBJECT

public:
	CTotalSelectDialog(QWidget *parent = 0);
	~CTotalSelectDialog();

	void AddWidget(QWidget *pWidget, QString strModuleName);

	void closeEvent(QCloseEvent *event);

private slots:
	void SelectModule(QListWidgetItem *item);

	void reject()
	{
		return;
	}

private:
	Ui::CTotalSelectDialog ui;
	QMap<QString, QWidget *> m_arrWidgets;
	QMainWindow *m_pMain;


};

#endif // TOTALSELECTDIALOG_H
