#ifndef TESTLOG_H
#define TESTLOG_H

#include <QMainWindow>
#include "ui_testlog.h"

class testlog : public QMainWindow
{
	Q_OBJECT

public:
	testlog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~testlog();

public:
	bool Initialize();
	void UnInitialize();

private slots:
	void OnLog();
	void OnExe();
private:
	Ui::testlogClass ui;
};

#endif // TESTLOG_H
