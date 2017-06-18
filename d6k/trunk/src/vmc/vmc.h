#pragma once

#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include <QLineEdit>
#include <functional>
#include <iostream>
#include <memory>
#include <deque>
#include "ui_vmc.h"

struct  FileInfo
{
	QString m_szName;
	QString m_szPath;
};

class vmc : public QMainWindow
{
	Q_OBJECT

public:
	vmc(QWidget *parent = Q_NULLPTR);
	~vmc(){}
public:
	void InitComView();
	bool IsFileHasAdded(const QString& szName);
protected Q_SLOTS:
	void on_btn_start_clicked();
	void on_BtnGetFilePath();
	void on_BtnDataShow();
	void on_BtnStop();
	void on_BtnAlarmView();
	void on_ComChannged(int nIndex);
private:
	QString m_szFilePath;
	QLibrary m_LibFes;
	QLineEdit* m_pLineEdit;
	int m_nMaxRecentFileNum;
	std::deque< std::shared_ptr<FileInfo> > m_FileInfoQue;

private:
	std::function < int(const char*, unsigned int) > m_fnScdsvcMain;
	std::function < int() > m_fnScdsvcStop;
	std::function < int() > Functional;
private:
	Ui::vmcClass ui;
};

