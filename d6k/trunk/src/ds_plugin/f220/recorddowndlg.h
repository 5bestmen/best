#ifndef RECORDDOWNDLG_H
#define RECORDDOWNDLG_H

#include <QDialog>
#include "ui_recorddowndlg.h"
#include "qftp.h"

class QFile;
class CRecordDownDlg : public QDialog
{
    Q_OBJECT

public:
    CRecordDownDlg(QWidget *parent = 0);
    ~CRecordDownDlg();
    //设置生成目录
    void SetDownLoadPath(const QString &strPath);
public slots:
    void Slot_UpLoadFile();
    void Slot_Cancel();
    //ftp命令
    void Slot_FtpCommandStarted(int nState);
    //ftp命令结束
    void Slot_FtpCommandFinished(int nCommand,bool bFlag);
    //ftp list命令
    void Slot_FtpAddToList(QUrlInfo urlInfo);
    //更新processbar
    void Slot_updateProcessBar(qint64 currentIndex,qint64 totalIndex);
    //写文件
    void Slot_DownLoadFiels();
    //ftp出错
    void Slot_FtpDone(bool bFlag);

private:
    Ui::CRecordDownDlg ui;
    //文件生成目录
    QString m_strFilePath;
    //
    QFtp *m_pFtp;

    //
    QList<QFile *> m_pDownLoadFile;
};

#endif // RECORDDOWNDLG_H
