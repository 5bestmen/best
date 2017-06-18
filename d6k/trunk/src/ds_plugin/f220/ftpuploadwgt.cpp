#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include "ftpuploadwgt.h"
#include "traninfo.h"

CFtpUploadWgt::CFtpUploadWgt(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_pConfigInfo = new CTranInfo;
    ui.progressBar->setHidden(true);
    m_pManager = new QNetworkAccessManager(this);
    connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_UpLoadFile()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));
}

CFtpUploadWgt::~CFtpUploadWgt()
{
    delete m_pConfigInfo;
}

void CFtpUploadWgt::Slot_UpLoadFile()
{
    emit Signal_Conform();

    //更新ftp配置文件
    FTPCONFIG uploadinfo;
    uploadinfo.strFtpIP = ui.lineEdit->text();
    uploadinfo.nPort = ui.lineEdit_2->text().toInt();
    uploadinfo.strUsername = ui.lineEdit_3->text();
    uploadinfo.strpassWd = ui.lineEdit_4->text();
    uploadinfo.strfile = ui.lineEdit_5->text();

    m_pConfigInfo->SetUploadInfo(uploadinfo);
    m_pConfigInfo->BeginWriteXmlFIle();
    //end

}

void CFtpUploadWgt::Slot_Cancel()
{
    this->close();
}

void CFtpUploadWgt::BeginFtpUpLoad()
{
    //检查ftp参数真确性
    if (ui.lineEdit->text().isEmpty() || ui.lineEdit_2->text().isEmpty() || ui.lineEdit_3->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("input Parameter error,please check!"));
        return;
    }
    if (!ui.lineEdit_5->text().isEmpty())
    {
        if (ui.lineEdit_5->text().at(0) == '/')
        {
            QMessageBox::warning(this, tr("Warning"), tr("input Path Error,can not begin with /"));
            return;
        }

        if (!ui.lineEdit_5->text().endsWith('/'))
        {
            QMessageBox::warning(this, tr("Warning"), tr("input Path Error,must end with /"));
            return;

        }
    }

    SetFtpDownLoad(m_strPointName, ui.lineEdit->text(), ui.lineEdit_2->text().toInt(), ui.lineEdit_3->text(), ui.lineEdit_4->text(), ui.lineEdit_5->text());
}

void CFtpUploadWgt::SetPointTableName(const QString &strName)
{
    m_strPointName = strName;
}

void CFtpUploadWgt::SetFtpDownLoad(const QString &strUpFile, const QString &strIp, int nport, const QString &strUserName, const QString &strPasswd, const QString &strPath)
{
    ui.progressBar->setHidden(false);
    //读取文件内容
    QFile tfile(strUpFile);

    QByteArray tFtpData;
    if (tfile.open(QIODevice::ReadOnly))
    {
        while (!tfile.atEnd())
        {
            tFtpData.append(tfile.readLine());
        }
        tfile.close();
    }
    else
    {
        //
        QMessageBox::warning(this, tr("Warning"), tr("File dose not exist!"));
        return;
    }
    //end
    QString strfpt = "ftp://" + strUserName + ":" + strPasswd + "@" + strIp + "/" + strPath + strUpFile.split('/').last();

    QNetworkRequest putRequest;
    QUrl ftpUrl(strfpt);
    ftpUrl.setPort(nport);
    putRequest.setUrl(ftpUrl);

    QNetworkReply *pPutReplay = m_pManager->put(putRequest, tFtpData);

    connect(pPutReplay,SIGNAL(finished()),this,SLOT(Slot_FtpFinish()));
    connect(pPutReplay, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(Slot_UploadProcess(qint64, qint64)));
    connect(pPutReplay, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(Slot_FtpError(QNetworkReply::NetworkError)));

}

void CFtpUploadWgt::Slot_UploadProcess(qint64 bytesSent, qint64 bytesTotal)
{
    ui.progressBar->setMaximum(bytesTotal);
    ui.progressBar->setValue(bytesSent);
}

void CFtpUploadWgt::Slot_FtpError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QMessageBox::warning(this, tr("Waring"), reply->errorString());
    //下装失败
    emit Signal_UploadStatus(2);
}

void CFtpUploadWgt::Slot_FtpFinish()
{
    ui.progressBar->setHidden(true);
    QNetworkReply *reply = (QNetworkReply *)sender();
    if (reply != nullptr)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            //下装成功
            emit Signal_UploadStatus(1);
        }
    }

}

void CFtpUploadWgt::ReadXmlConfig(const QString &strFileName)
{
    //清空
    ui.lineEdit->clear();
    ui.lineEdit_3->clear();
    ui.lineEdit_4->clear();
    ui.lineEdit_5->clear();

    m_pConfigInfo->SetXmlFilename(strFileName);

    if (!QFile(strFileName).exists())
    {
        return;
    }

    m_pConfigInfo->AnalyseTransInfoFile(strFileName);

    ui.lineEdit->setText(m_pConfigInfo->GetUploadInfo().strFtpIP);
    ui.lineEdit_2->setText(QString::number(m_pConfigInfo->GetUploadInfo().nPort));
    ui.lineEdit_3->setText(m_pConfigInfo->GetUploadInfo().strUsername);
    ui.lineEdit_4->setText(m_pConfigInfo->GetUploadInfo().strpassWd);
    ui.lineEdit_5->setText(m_pConfigInfo->GetUploadInfo().strfile);
}
