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
    //��������Ŀ¼
    void SetDownLoadPath(const QString &strPath);
public slots:
    void Slot_UpLoadFile();
    void Slot_Cancel();
    //ftp����
    void Slot_FtpCommandStarted(int nState);
    //ftp�������
    void Slot_FtpCommandFinished(int nCommand,bool bFlag);
    //ftp list����
    void Slot_FtpAddToList(QUrlInfo urlInfo);
    //����processbar
    void Slot_updateProcessBar(qint64 currentIndex,qint64 totalIndex);
    //д�ļ�
    void Slot_DownLoadFiels();
    //ftp����
    void Slot_FtpDone(bool bFlag);

private:
    Ui::CRecordDownDlg ui;
    //�ļ�����Ŀ¼
    QString m_strFilePath;
    //
    QFtp *m_pFtp;

    //
    QList<QFile *> m_pDownLoadFile;
};

#endif // RECORDDOWNDLG_H
