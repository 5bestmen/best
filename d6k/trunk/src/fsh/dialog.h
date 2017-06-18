#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLibrary>
#include <QLineEdit>
#include <functional>
#include <memory>
#include <deque>
namespace Ui {
class Dialog;
}

class QLibrary;

 
struct  FileInfo
{
	QString m_szName;
	QString m_szPath;
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
protected:
	void InitComView();
	bool IsFileHasAdded(const QString& szName);
private slots:
    void on_btn_start_clicked();
	void on_BtnGetFilePath();
	void on_BtnDataShow();
	void on_BtnAlarmView();
	void on_BtnStop();
	void on_ComChannged(int nIndex);
private:
	QString m_szFilePath;
	QLibrary m_LibFes;
    Ui::Dialog *ui;
	QLineEdit* m_pLineEdit;
	int m_nMaxRecentFileNum;
	std::deque< std::shared_ptr<FileInfo> > m_FileInfoQue;
	std::function < int() > m_fnFesStop;
};

#endif // DIALOG_H
