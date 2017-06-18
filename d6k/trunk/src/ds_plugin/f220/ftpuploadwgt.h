#ifndef FTPUPLOADWGT_H
#define FTPUPLOADWGT_H

#include <QWidget>
#include <QNetworkReply>
#include "ui_ftpuploadwgt.h"


class QNetworkAccessManager;
class CTranInfo;

class CFtpUploadWgt : public QWidget
{
    Q_OBJECT

public:
    CFtpUploadWgt(QWidget *parent = 0);
    ~CFtpUploadWgt();
    //开始上传文件
    void BeginFtpUpLoad();
    //设置点表名称
    void SetPointTableName(const QString &strName);
    //ftp下载文件
    void SetFtpDownLoad(const QString &strUpFile,const QString &strIp,int nport,const QString &strUserName,const QString &strPasswd,const QString &strPath);
    //
    void ReadXmlConfig(const QString &strFileName);
signals:
    void Signal_Conform();
    //发送成功失败标识
    //1下装成功   2下装失败
    void Signal_UploadStatus(int nStatus);
public slots:
    //上传文件
    void Slot_UpLoadFile();
    //取消
    void Slot_Cancel();
    //ftp上传结束
    void Slot_FtpFinish();
    //更新进度
    void Slot_UploadProcess(qint64 bytesSent, qint64 bytesTotal);
    //报错
    void Slot_FtpError(QNetworkReply::NetworkError);

private:
    Ui::CFtpUploadWgt ui;
    //ftp
    QNetworkAccessManager *m_pManager;
    //
    QString m_strPointName;
    CTranInfo *m_pConfigInfo;
};

#endif // FTPUPLOADWGT_H
