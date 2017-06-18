/*! @file
<PRE>
********************************************************************************
模块名       :	检查远动装置点表
文件名       :	checkremotewgt.cpp
文件实现功能 :  
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2016.9.27
*/
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QApplication>
#include <QDir>
#include "checkremotewgt.h"
#include "infoconfigwgt.h"
#include "traninfo.h"

/*! \fn CCheckRemoteWgt::CCheckRemoteWgt(QString strPointTable,QWidget *parent)
*********************************************************************************************************
** \brief CCheckRemoteWgt::CCheckRemoteWgt
** \details   构造函数
** \param 点表名称 parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
CCheckRemoteWgt::CCheckRemoteWgt(QString strPointTable,QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_nTypeFlag = 0;

    m_pConfigInfo = new CTranInfo;
    m_strLocalTableName = strPointTable;
    ui.progressBar->setHidden(true);
    m_pManager = new QNetworkAccessManager(this);
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_UpLoadFile()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));
    if (strPointTable.isEmpty())
    {
        ui.pushButton_2->setHidden(true);
    }
}

CCheckRemoteWgt::~CCheckRemoteWgt()
{
    delete m_pConfigInfo;
}

/*! \fn CCheckRemoteWgt::BeginFtpDownLoad()
*********************************************************************************************************
** \brief CCheckRemoteWgt::BeginFtpDownLoad
** \details   开始ftp下载文件
** \param 
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CCheckRemoteWgt::BeginFtpDownLoad()
{
    //检查ftp参数真确性
    if (ui.lineEdit->text().isEmpty() || ui.lineEdit_2->text().isEmpty() || ui.lineEdit_3->text().isEmpty() || ui.lineEdit_5->text().isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("input Parameter error,please check!"));
        return;
    }

    if (ui.lineEdit_5->text().at(0) == '/')
    {
        QMessageBox::warning(this, tr("Warning"), tr("input Parameter error,please check!"));
        return;
    }


    m_strPointTableName = qApp->applicationDirPath() + REMOTETABLE + ui.lineEdit_5->text().split('/').last();

    SetFtpDownLoad(ui.lineEdit_5->text(), ui.lineEdit->text(), ui.lineEdit_2->text().toInt(), ui.lineEdit_3->text(), ui.lineEdit_4->text());
}

/*! \fn CCheckRemoteWgt::SetFtpDownLoad()
*********************************************************************************************************
** \brief CCheckRemoteWgt::SetFtpDownLoad
** \details   开始ftp下载文件
** \param  下载文件名  ip地址  端口号  用户名  密码
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/

void CCheckRemoteWgt::SetFtpDownLoad(const QString &strUpFile, const QString &strIp, int nport, const QString &strUserName, const QString &strPasswd)
{

    ui.progressBar->setHidden(false);

    QString strfpt = "ftp://" + strUserName + ":" + strPasswd + "@" + strIp + "/" + strUpFile;


    QNetworkRequest getRequest;
    QUrl ftpUrl(strfpt);
    ftpUrl.setPort(nport);
    getRequest.setUrl(ftpUrl);

    QNetworkReply *pPutReplay = m_pManager->get(getRequest);
    

    connect(pPutReplay, SIGNAL(finished()), this, SLOT(Slot_FtpFinish()));
    connect(pPutReplay, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(Slot_DownloadProcess(qint64, qint64)));
    connect(pPutReplay, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(Slot_FtpError(QNetworkReply::NetworkError)));
}

/*! \fn CCheckRemoteWgt::Slot_UpLoadFile()
*********************************************************************************************************
** \brief CCheckRemoteWgt::Slot_UpLoadFile
** \details   点击下载按钮槽函数
** \param  
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CCheckRemoteWgt::Slot_UpLoadFile()
{
    if (!m_nTypeFlag)
    {
        //更新ftp配置文件
        FTPCONFIG downloadinfo;
        downloadinfo.strFtpIP = ui.lineEdit->text();
        downloadinfo.nPort = ui.lineEdit_2->text().toInt();
        downloadinfo.strUsername = ui.lineEdit_3->text();
        downloadinfo.strpassWd = ui.lineEdit_4->text();
        downloadinfo.strfile = ui.lineEdit_5->text();

        m_pConfigInfo->SetDownLoadInfo(downloadinfo);
        m_pConfigInfo->BeginWriteXmlFIle();
    }
    //end
    
    if (m_strLocalTableName.isEmpty())
    {
        BeginFtpDownLoad();
    }
    else
    {
        emit Signal_Conform();
    }
    
}

void CCheckRemoteWgt::Slot_Cancel()
{
    this->close();
}

/*! \fn CCheckRemoteWgt::Slot_FtpFinish()
*********************************************************************************************************
** \brief CCheckRemoteWgt::Slot_FtpFinish
** \details   下载完成触发槽函数
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CCheckRemoteWgt::Slot_FtpFinish()
{
    ui.progressBar->setHidden(true);
    QNetworkReply *reply = (QNetworkReply *)sender();
    if (reply != nullptr)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            if (m_nTypeFlag == 1)
            {
                QString strFileName = qApp->applicationDirPath() + CMDHISTORYPATH + ui.lineEdit_5->text().split('/').last();

                QDir directory;

                directory.setPath(qApp->applicationDirPath() + CMDHISTORYPATH);
                if (!directory.exists())
                {
                    directory.mkdir(qApp->applicationDirPath() + CMDHISTORYPATH);
                }

                QByteArray btRead = reply->readAll();

                QFile file(strFileName);

                if (!file.open(QFile::WriteOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }

                file.write(btRead);
                file.close();
            }
            else if (m_nTypeFlag == 0)
            {
                //清空文件
                QDir tDir;
                QString strDir = QFileInfo(m_strPointTableName).dir().absolutePath();
                tDir.setPath(strDir);
                QStringList fileLst = tDir.entryList(QDir::Files);

                for each (QString item in fileLst)
                {
                    tDir.remove(item);
                }

                QByteArray btRead = reply->readAll();

                if (m_strPointTableName.isEmpty())
                {
                    return;
                }

                QFile file(m_strPointTableName);

                if (!file.open(QFile::WriteOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }

                file.write(btRead);
                file.close();


                //下装成功
                emit Signal_DownloadStatus(1);
                emit Signal_FileDownLoadSuccess(m_strPointTableName);
            }
            else if (m_nTypeFlag == 2)
            {
                QString strFileName = qApp->applicationDirPath() + RECORDFILEPATH + ui.lineEdit_5->text().split('/').last();

                QDir directory;

                directory.setPath(qApp->applicationDirPath() + RECORDFILEPATH);
                if (!directory.exists())
                {
                    directory.mkdir(qApp->applicationDirPath() + RECORDFILEPATH);
                }

                QByteArray btRead = reply->readAll();

                QFile file(strFileName);

                if (!file.open(QFile::WriteOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }

                file.write(btRead);
                file.close();
            }
            else if (m_nTypeFlag == 3)
            {
                QString strFileName = qApp->applicationDirPath() + HISTORYTABLE + ui.lineEdit_5->text().split('/').last();

                QDir directory;

                directory.setPath(qApp->applicationDirPath() + HISTORYTABLE);
                if (!directory.exists())
                {
                    directory.mkdir(qApp->applicationDirPath() + HISTORYTABLE);
                }

                QByteArray btRead = reply->readAll();

                QFile file(strFileName);

                if (!file.open(QFile::WriteOnly))
                {
                    QMessageBox::warning(this, tr("Warning"), file.errorString());
                    return;
                }

                file.write(btRead);
                file.close();

            }
                        
        }
    }
}

/*! \fn CCheckRemoteWgt::Slot_DownloadProcess()
*********************************************************************************************************
** \brief CCheckRemoteWgt::Slot_DownloadProcess
** \details   更新下载精度
** \param    当前已经下载量  总的需要下载量
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CCheckRemoteWgt::Slot_DownloadProcess(qint64 bytesSent, qint64 bytesTotal)
{
    ui.progressBar->setMaximum(bytesTotal);
    ui.progressBar->setValue(bytesSent);
}

/*! \fn CCheckRemoteWgt::Slot_FtpError()
*********************************************************************************************************
** \brief CCheckRemoteWgt::Slot_FtpError
** \details   下载错误槽函数
** \param    错误编码
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CCheckRemoteWgt::Slot_FtpError(QNetworkReply::NetworkError)
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QMessageBox::warning(this, tr("Waring"), reply->errorString());
    //下装失败
    emit Signal_DownloadStatus(2);
}

QString CCheckRemoteWgt::GetPointBinFileName()
{
    return m_strPointTableName;
}

void CCheckRemoteWgt::Slot_ReadXmlConfig(const QString &strFileName)
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

    ui.lineEdit->setText(m_pConfigInfo->GetDownLoadInfo().strFtpIP);
    ui.lineEdit_2->setText(QString::number(m_pConfigInfo->GetDownLoadInfo().nPort));
    ui.lineEdit_3->setText( m_pConfigInfo->GetDownLoadInfo().strUsername);
    ui.lineEdit_4->setText(m_pConfigInfo->GetDownLoadInfo().strpassWd);
    ui.lineEdit_5->setText(m_pConfigInfo->GetDownLoadInfo().strfile);
}

void CCheckRemoteWgt::SetDownLoadFtpIP(const QString &strFtpIp)
{
    ui.lineEdit->setText(strFtpIp);
}

void CCheckRemoteWgt::SetDownLoadFtpPort(const QString &strFtpPort)
{
    ui.lineEdit_2->setText(strFtpPort);
}

void CCheckRemoteWgt::SetDownLoadFtpUserName(const QString &strFtpUserName)
{
    ui.lineEdit_3->setText(strFtpUserName);
}

void CCheckRemoteWgt::SetDownLoadFtpPassWd(const QString &strFtpPasswd)
{
    ui.lineEdit_4->setText(strFtpPasswd);
}

void CCheckRemoteWgt::SetDonwLoadFileName(const QString &strFileName)
{
    ui.lineEdit_5->setText(strFileName);
}

QString CCheckRemoteWgt::GetDownLoadFtpIP()
{
    return ui.lineEdit->text();
}

QString CCheckRemoteWgt::GetDownLoadFtpPort()
{
    return ui.lineEdit_2->text();
}

QString CCheckRemoteWgt::GetDownLoadFtpUserName()
{
    return ui.lineEdit_3->text();
}

QString CCheckRemoteWgt::GetDownLoadFtpPassWd()
{
    return ui.lineEdit_4->text();
}

QString CCheckRemoteWgt::GetDonwLoadFileName()
{
    return ui.lineEdit_5->text();
}

void CCheckRemoteWgt::SetCmdFlag(int nTypeFlag)
{
    m_nTypeFlag = nTypeFlag;

    ui.lineEdit->setText("192.168.116.19");
    ui.lineEdit_3->setText("wj");
    ui.lineEdit_4->setText("wj");

    if (nTypeFlag == 1)
    {
        ui.lineEdit_5->setText("tffs0/CmdHistory.txt");
    }
    else if (nTypeFlag == 2)
    {
        ui.lineEdit_5->setText("tffs0/COMTRADE");
    }
    else if (nTypeFlag == 3)
    {
        ui.lineEdit_5->setText("tffs0/soeHstry.bin");
    }
}








