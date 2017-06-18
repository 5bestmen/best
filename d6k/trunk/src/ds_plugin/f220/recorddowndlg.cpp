#include <QMessageBox>
#include <QDir>
#include "recorddowndlg.h"

CRecordDownDlg::CRecordDownDlg(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_pFtp = nullptr;
    ui.progressBar->setHidden(true);

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_UpLoadFile()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));
    //ftp下载文件
    //connect(m_pFtp, SIGNAL(readyRead()), this, SLOT(Slot_DownLoadFiels()));
}

CRecordDownDlg::~CRecordDownDlg()
{
    for each (QFile * pFile in m_pDownLoadFile)
    {
        pFile->deleteLater();
    }
}

void CRecordDownDlg::Slot_UpLoadFile()
{
    QString strFtp = ui.lineEdit->text();
    int nPort = ui.lineEdit_2->text().toInt();

    QString strUsername = ui.lineEdit_3->text();
    QString strPasswd = ui.lineEdit_4->text();

    if (!strFtp.isEmpty() && nPort>0 && !strUsername.isEmpty())
    {
        if (m_pFtp != nullptr)
        {
            m_pFtp->deleteLater();
        }
        
        m_pFtp = new QFtp(this);
        
        m_pFtp->connectToHost(strFtp, nPort);
        m_pFtp->login(strUsername,strPasswd);
        m_pFtp->setTransferMode(QFtp::Active);        

        //ftp命令状态改变
        connect(m_pFtp, SIGNAL(stateChanged(int)), this, SLOT(Slot_FtpCommandStarted(int)));
        //
        connect(m_pFtp, SIGNAL(commandFinished(int, bool)), this, SLOT(Slot_FtpCommandFinished(int, bool)));
        //
        connect(m_pFtp, SIGNAL(listInfo(QUrlInfo)), this, SLOT(Slot_FtpAddToList(QUrlInfo)));
        //
        connect(m_pFtp, SIGNAL(dataTransferProgress(qint64, qint64)), this, SLOT(Slot_updateProcessBar(qint64, qint64)));
        //
        connect(m_pFtp, SIGNAL(done(bool)), this, SLOT(Slot_FtpDone(bool)));
    }
}

void CRecordDownDlg::Slot_Cancel()
{
    this->close();
}

void CRecordDownDlg::Slot_FtpCommandStarted(int nState)
{
    //暂时不使用
    if (nState == QFtp::Login)
    {

    }
    else if (nState == QFtp::Cd)
    {

    }
}

void CRecordDownDlg::Slot_FtpCommandFinished(int nCommand, bool bFlag)
{
    Q_UNUSED(nCommand);
    if (m_pFtp->currentCommand() ==  QFtp::Login)
    {
        m_pFtp->cd(ui.lineEdit_5->text());
    }
    else if (m_pFtp->currentCommand() == QFtp::Cd)
    {
        m_pFtp->list();
    }
    else if (m_pFtp->currentCommand() == QFtp::List)
    {

    }
    else if (m_pFtp->currentCommand() == QFtp::Get)
    {
        if (m_pDownLoadFile.count() == 0)
        {
            return;
        }

        QFile *tDownFile = m_pDownLoadFile.at(0);
        m_pDownLoadFile.removeAt(0);

        if (bFlag)
        {
            tDownFile->close();
            tDownFile->remove();
        }
        else 
        {
            tDownFile->close();
        }
        delete tDownFile;
    }
}

#include <QDebug>
void CRecordDownDlg::Slot_FtpAddToList(QUrlInfo urlInfo)
{
    //
    if (urlInfo.isFile())
    {
        QDir directory;
        QString strRdFilePath = ui.lineEdit_5->text().split("/").last();

        directory.setPath(m_strFilePath);

        if (!directory.exists())
        {
            directory.mkdir(m_strFilePath);
            directory.mkdir(m_strFilePath + "/" + strRdFilePath);
        }
        else
        {
            directory.setPath(m_strFilePath + "/" + strRdFilePath);

            if (!directory.exists())
            {
                directory.mkdir(m_strFilePath + "/" + strRdFilePath);
            }
        }
        QString strFtpFilename = urlInfo.name();


        QFile *pDownFile = new QFile(m_strFilePath + "/" + strRdFilePath + "/" + urlInfo.name(), this);

        if (pDownFile->open(QIODevice::WriteOnly))
        {
            m_pFtp->get(strFtpFilename, pDownFile);
        }

        m_pDownLoadFile.append(pDownFile);

    }
}

void CRecordDownDlg::Slot_updateProcessBar(qint64 currentIndex, qint64 totalIndex)
{
    if (currentIndex != totalIndex)
    {
        ui.progressBar->setVisible(true);
    }
    else
    {
        ui.progressBar->setVisible(false);
    }

    ui.progressBar->setMaximum(totalIndex);
    ui.progressBar->setValue(currentIndex);
}

void CRecordDownDlg::Slot_DownLoadFiels()
{
    if (m_pFtp->error() == QFtp::NoError)
    {
        ui.progressBar->hide();
    }
    else
    {
        m_pFtp->close();
        QMessageBox::warning(this, tr("Warning"), m_pFtp->errorString());
    }
}

void CRecordDownDlg::SetDownLoadPath(const QString &strPath)
{
    m_strFilePath = strPath;
}

void CRecordDownDlg::Slot_FtpDone(bool bFlag)
{
    if (bFlag)
    {
        QMessageBox::warning(this, tr("Warning"),m_pFtp->errorString());
    }
}
