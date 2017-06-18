#ifndef CHECKREMOTEWGT_H
#define CHECKREMOTEWGT_H

#include <QWidget>
#include <QNetworkReply>
#include "ui_checkremotewgt.h"


class QNetworkAccessManager;
class QNetworkReply;
class CTranInfo;

class CCheckRemoteWgt : public QWidget
{
    Q_OBJECT

public:
    CCheckRemoteWgt(QString strPointTable,QWidget *parent = 0);
    ~CCheckRemoteWgt();
    //开始上传文件
    void BeginFtpDownLoad();
    //ftp下载文件
    void SetFtpDownLoad(const QString &strUpFile, const QString &strIp, int nport, const QString &strUserName, const QString &strPasswd);
    //获取下载到的点表的名称
    QString GetPointBinFileName();
    //设置ip
    void SetDownLoadFtpIP(const QString &strFtpIp);
    //设置port
    void SetDownLoadFtpPort(const QString &strFtpPort);
    //设置username
    void SetDownLoadFtpUserName(const QString &strFtpUserName);
    //设置password
    void SetDownLoadFtpPassWd(const QString &strFtpPasswd);
    //设置下载文件
    void SetDonwLoadFileName(const QString &strFileName);

    //获取ip
    QString GetDownLoadFtpIP();
    //获取port
    QString GetDownLoadFtpPort();
    //获取username
    QString GetDownLoadFtpUserName();
    //获取password
    QString GetDownLoadFtpPassWd();
    //获取
    QString GetDonwLoadFileName();
    //设置模式
    void SetCmdFlag(int nTypeFlag);
signals:
    void Signal_Conform();
    //发送成功失败标识
    void Signal_DownloadStatus(int nFlag);
    //文件下载成功后发出信号 供更新
    void Signal_FileDownLoadSuccess(const QString &strFileName);

public slots:
    //上传文件
    void Slot_UpLoadFile();
    //取消
    void Slot_Cancel();
    //ftp上传结束
    void Slot_FtpFinish();
    //更新进度
    void Slot_DownloadProcess(qint64 bytesSent, qint64 bytesTotal);
    //报错
    void Slot_FtpError(QNetworkReply::NetworkError);
    //读取配置文件中数据 
    void Slot_ReadXmlConfig(const QString &strFileName);

private:
    Ui::CCheckRemoteWgt ui;
    //ftp
    QNetworkAccessManager *m_pManager;
    //远动文件名称
    QString m_strPointTableName;
    //本地文件名称
    QString m_strLocalTableName;
    //查询ftp数据
    CTranInfo *m_pConfigInfo;
    //0：bin  1：cmdhistory   2：录波数据
    int m_nTypeFlag;

};

#endif // CHECKREMOTEWGT_H
