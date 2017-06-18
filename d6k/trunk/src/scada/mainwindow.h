#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLibrary>
#include <QLineEdit>
#include <functional>
#include <iostream>
#include <memory>
#include <deque>

namespace Ui {
class MainWindow;
}

struct  FileInfo
{
	QString m_szName;
	QString m_szPath;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void InitComView();
	bool IsFileHasAdded(const QString& szName);
private slots:
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
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
