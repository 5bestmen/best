#ifndef TESTLOG_H
#define TESTLOG_H

#include <QMainWindow>
#include "ui_testchildlog.h"

class testchildlog : public QMainWindow
{
	Q_OBJECT

public:
	testchildlog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~testchildlog();

public:
	bool Initialize();
	void UnInitialize();

private slots:
	void OnLog();
private:
	Ui::testlogClass ui;
};

#endif // TESTLOG_H
