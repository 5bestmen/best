#include <QTcpSocket>
#include <QMessageBox>
#include <QDateTime>
#include <QMenu>
#include <QAction>
#include <QTimer>
#include <QCalendarWidget>
#include <QTimeEdit>
#include <QFile>
#include "maintecancewgt.h"
#include "ftpuploadwgt.h"
#include "checkremotewgt.h"
#include "infoconfigwgt.h"
#include "traninfo.h"
#include "infoconfigwgt.h"
#include "recorddowndlg.h"
#include "lcdoperatorwgt.h"
#include "lcdcontrolwgt.h"
#include "asdu_info.h"
#include "commthread.h"

CMaintecanceWgt::CMaintecanceWgt(CCommThread *pCommunicate, CInfoConfigWgt *pConfigWgt, CLcdoperatorWgt *plcdOperatorWgt, QString strPontName, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    Q_ASSERT(pConfigWgt);
    if (pConfigWgt == nullptr)
    {
        return;
    }

    Q_ASSERT(plcdOperatorWgt);
    if (plcdOperatorWgt == nullptr)
    {
        return;
    }

	Q_ASSERT(pCommunicate);
	if (pCommunicate == nullptr)
	{
		return;
	}

	m_strPointTableName = strPontName;

	m_pCommuncate = pCommunicate;


    m_pTimeLcd = new QTimer(this);
    m_pTimeLcd->setInterval(1000);
    connect(m_pTimeLcd, SIGNAL(timeout()), this, SLOT(Slot_LcdRequesetTimer()));

    m_pLedTimer = new QTimer(this);
    //m_pLedTimer->setInterval(2000);
    connect(m_pLedTimer, SIGNAL(timeout()), this, SLOT(Slot_GetLedTimerOut()));

    m_pInfoConfigWgt = pConfigWgt;
    m_pLcdOperatorWgt = plcdOperatorWgt;

    connect(m_pLcdOperatorWgt->GetControl(), SIGNAL(Signal_ClickOperator(int)),this,SLOT(Slot_SendLcdClickOperator(int)));

    //
    
    m_nTimerout = 0;
    m_pTimeWgt = new CTimeChooseWgt;

    m_pFtpDownLoad = new CCheckRemoteWgt(strPontName);

    m_pCmdFtpDownLoad = new CCheckRemoteWgt("");
    m_pCmdFtpDownLoad->SetCmdFlag(1);
    m_pCmdFtpDownLoad->setWindowTitle(tr("Down Cmd File"));

    m_pFtpHistoryDownLoad = new CCheckRemoteWgt("");
    m_pFtpHistoryDownLoad->setWindowTitle(tr("History Download"));
    m_pFtpHistoryDownLoad->SetCmdFlag(3);

    m_pRecordDownLoad = new CRecordDownDlg;
    m_pRecordDownLoad->setWindowTitle(tr("Down Record File"));

    QString strRunPath = qApp->applicationDirPath();

    m_pRecordDownLoad->SetDownLoadPath(strRunPath + PROJECTPATH + pConfigWgt->GetProjectName());

    m_pFtpUpload = new CFtpUploadWgt;
    m_pFtpUpload->SetPointTableName(strPontName);
    //虚遥信
    m_pVirtualBinaryUpload = new CFtpUploadWgt;
    m_pVirtualBinaryUpload->setWindowTitle(tr("Upload Virutal Binary"));
    m_pVirtualBinaryUpload->SetPointTableName(strRunPath + PROJECTPATH + pConfigWgt->GetProjectName() + "/merge.bin");

    //ftp下载

    ui.tableWidget->setColumnWidth(0, 200);

    ui.tableWidget->setSortingEnabled(true);
    ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(Slot_ContextMenuRequest(const QPoint &)));

    m_pTcpSocket = new QTcpSocket(this);
    m_pTextTimer = new QTimer(this);
    connect(m_pTextTimer, SIGNAL(timeout()), this, SLOT(Slot_TextTimeOut()));

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(Slot_ConnectSocket()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(Slot_DisConnectSocket()));
    connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(Slot_SetDcZero()));
    connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(Slot_DcTrans()));
    connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(Slot_AcTrans()));
    connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(Slot_DeviceInit ()));
    connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(Slot_Reset()));
    connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(Slot_ReportClear()));


    connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(Slot_TextNetwork()));
    connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(Slot_TimeSet()));
    connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(Slot_GetVersion()));
    connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(Slot_UpLoadPontFile()));
    connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(Slot_CheckDeviceTable()));
    connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(Slot_SignalReset()));
    connect(ui.pushButton_15, SIGNAL(clicked()), this, SLOT(Slot_GetHistoryData()));
    connect(ui.pushButton_16, SIGNAL(clicked()), this, SLOT(Slot_GetRecordConfig()));
    connect(ui.pushButton_17, SIGNAL(clicked()), this, SLOT(Slot_GetCmdHistory()));
    connect(ui.pushButton_18, SIGNAL(clicked()), this, SLOT(Slot_UploadVirutalBinary()));

    connect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(Slot_ConnectSuccess()));
    connect(m_pTcpSocket, SIGNAL(disconnected()), this, SLOT(Slot_DisConnect()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(Slot_SocketError(QAbstractSocket::SocketError)));
    //接收数据
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(Slot_RecvNewData()));
    //设置时间
    connect(m_pTimeWgt, SIGNAL(Signal_SetTime(QDateTime &)), this, SLOT(Slot_SendTime(QDateTime &)));
    //下装点表确定
    connect(m_pFtpUpload, SIGNAL(Signal_Conform()), this, SLOT(SLot_UpLoadRequest()));
    //下装成功失败状态
    connect(m_pFtpUpload, SIGNAL(Signal_UploadStatus(int)), this, SLOT(Slot_UpLoadStatus(int)));
    //检查当前远动点表请求 
    connect(m_pFtpDownLoad, SIGNAL(Signal_Conform()), this, SLOT(Slot_CheckRemotePoint()));
    //检查成功失败
    connect(m_pFtpDownLoad, SIGNAL(Signal_DownloadStatus(int)), this, SLOT(Slot_CheckStatus(int)));

}

CMaintecanceWgt::~CMaintecanceWgt()
{
    m_pTcpSocket->deleteLater();
    m_pTimeWgt->deleteLater();
    m_pFtpUpload->deleteLater();
    m_pFtpDownLoad->deleteLater();
    m_pCmdFtpDownLoad->deleteLater();
    m_pRecordDownLoad->deleteLater();
    m_pVirtualBinaryUpload->deleteLater();
    m_pFtpHistoryDownLoad->deleteLater();
}

void CMaintecanceWgt::Slot_SetDcZero()
{
    //直流调零
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x5;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero),sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send data Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send data Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Dc zero Request Success!"));
    }
}

void CMaintecanceWgt::Slot_ConnectSocket()
{
    QString strIp = ui.lineEdit->text();
    int nPort = ui.lineEdit_2->text().toInt();

    if (strIp.isEmpty())
    {
        QMessageBox::warning(this, tr("warning"), tr("Ip address is error"));
    }

    if (nPort<=0)
    {
        QMessageBox::warning(this, tr("warning"), tr("Port is error"));
    }

    m_pTcpSocket->abort();
    m_pTcpSocket->connectToHost(strIp, nPort);
    WriteTableLog(tr("Connecting to server....."));
}

void CMaintecanceWgt::Slot_DisConnectSocket()
{
    m_pTcpSocket->abort();
    WriteTableLog(tr("Disconnect Socket....."));
    m_pLcdOperatorWgt->WriteTableLog(tr("Disconnect Socket....."));
}

void CMaintecanceWgt::WriteTableLog(const QString &strContext)
{
    int nRow = ui.tableWidget->rowCount();
    ui.tableWidget->insertRow(nRow);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

    ui.tableWidget->setItem(nRow, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(strContext);
    ui.tableWidget->setItem(nRow, 1, item1);

    if (nRow % 2 != 0)
    {
        item0->setBackgroundColor(QColor(230,231,232));
        item1->setBackgroundColor(QColor(230, 231, 232));
    }
    ui.tableWidget->scrollToBottom();
}

void CMaintecanceWgt::Slot_DisConnect()
{
    m_nTimerout = 0;
    m_pTimeLcd->stop();
    m_pLedTimer->stop();

    WriteTableLog(tr("Close Socket"));
}

void CMaintecanceWgt::Slot_ConnectSuccess()
{
    WriteTableLog(tr("Connect Success"));
    m_pLcdOperatorWgt->WriteTableLog(tr("Connect Success"));
    m_pTimeLcd->start();
    //m_pLedTimer->start();

}

void CMaintecanceWgt::Slot_SocketError(QAbstractSocket::SocketError error)
{
    qDebug() << error;
    m_nTimerout = 0;
    WriteTableLog(m_pTcpSocket->errorString());
    m_pLcdOperatorWgt->WriteTableLog(m_pTcpSocket->errorString());
    m_pTcpSocket->close();
    m_pTimeLcd->stop();
    m_pLedTimer->stop();
}

void CMaintecanceWgt::Slot_RecvNewData()
{

    QByteArray btArray = m_pTcpSocket->readAll();

    //qDebug() << "RecvBVddd:" << btArray.toHex();

    if ((unsigned)btArray.length() < sizeof(MAINTANCEINFO))
    {
        return;
    }

    MAINTANCEINFO RecInfo;
    memcpy((char *)&RecInfo, btArray.data(), sizeof(MAINTANCEINFO));

    //起始报文
    if (RecInfo.head == 0x68)
    {
        if (RecInfo.type == 0x5)
        {
            //直流调零
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Dc Zero Set Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Dc Zero Set Refuse!"));
            }
        }
        else if (RecInfo.type == 0x6)
        {
            //直流调幅
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Dc Trans Set Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Dc Trans Set Refuse!"));
            }
        }
        else if (RecInfo.type == 0x07)
        {
            //交流调整
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("AC Trans Set Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("AC Trans Set Refuse!"));
            }
        }
        else if (RecInfo.type == 0x09)
        {
            //交流调整
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Device Init Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Device Init Refuse!"));
            }
        }
        else if (RecInfo.type == 0xa)
        {
            //复位
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Reset Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Reset Refuse!"));
            }
        }
        else if (RecInfo.type == 0xb)
        {
            //报告清除
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Report Clear Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Report Clear Refuse!"));
            }
        }
        else if (RecInfo.type == 0xff)
        {
            //测试帧
            if (RecInfo.flag == 0x1)
            {
                //重新开始计时
                m_nTimerout = 0;
                
            }

        }
        else if (RecInfo.type == 0x22)
        {
            //时间设置
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Time Set Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Time Set Refuse!"));
            }
        }
        else if (RecInfo.type == 0x23)
        {
            //版本信息
            if (RecInfo.flag == 0x1)
            {
                MAINTANCEVERSION RecInfo;
                memcpy((char *)&RecInfo, btArray.data(), sizeof(MAINTANCEVERSION));

                QString strDspMain = RecInfo.dspmain;
                QString strDspTime = RecInfo.dsTime;
                QString strdspVersion = RecInfo.dsVersion;
                QString strdspCode = RecInfo.dsCode;

                QString strArmMain = RecInfo.armMain;
                QString strArmTime = RecInfo.armTime;
                QString strArmVersion = RecInfo.armVersion;
                QString strArmCode = RecInfo.armCode;


                QString strVersin = tr("DSP Version info( Device Name:") + strDspMain + tr("  CreateTime:") + strDspTime
                    + tr("   Version:") + strdspVersion + tr("   Code:") + strdspCode + tr(")       DSP Version info( Device Name:")
                    + strArmMain + tr("   CreateTime:") + strArmTime + tr("   Version:") + strArmVersion + tr("   Code:")
                    + strArmCode;

                WriteTableLog(strVersin);
            }
            else
            {
                //失败
                WriteTableLog(tr("Get Versin infomation Refuse!"));
            }
        }
        else if (RecInfo.type == 0x28)
        {
            //发送数据
            //m_pLedTimer->singleShot(0);
            Slot_GetLedTimerOut();
            //液晶显示数据
            m_pLcdOperatorWgt->AnalyseLcdRecvInfo(btArray);
        }
        else if (RecInfo.type == 0x30)
        {
            //给灯上色
            m_pLcdOperatorWgt->AnalyseLEDRecvInfo(btArray);

        }
        else if (RecInfo.type == 0x50)
        {
            //信号复归
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Signal Set Success!"));
            }
            else
            {
                //失败
                WriteTableLog(tr("Signal Set Refuse!"));
            }
        }
        else if (RecInfo.type == 0xc)
        {
            //下装点表文件请求
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("Upload Request Success!"));
                m_pFtpUpload->BeginFtpUpLoad();
            }
            else if (RecInfo.flag == 0x2)
            {
                //失败
                WriteTableLog(tr("Upload Request Refuse!"));
                QMessageBox::warning(this, tr("Warning"), tr("Refuse Upload point table"));

            }
            else if (RecInfo.flag == 0x3)
            {
                //返回确认
                WriteTableLog(tr("Recv Conform Info!"));

            }

        }
        else if (RecInfo.type == 0xd)
        {
            //下装点表文件请求
            if (RecInfo.flag == 0x1)
            {
                //成功
                WriteTableLog(tr("DownLoad Request Success!"));
                m_pFtpDownLoad->BeginFtpDownLoad();
            }
            else if (RecInfo.flag == 0x2)
            {
                //失败
                WriteTableLog(tr("Upload Request Refuse!"));
                QMessageBox::warning(this, tr("Warning"), tr("Refuse DownLoad point table"));

            }
            else if (RecInfo.flag == 0xff)
            {
                //不存在
                WriteTableLog(tr("Remote Point Table Cannot Find!"));
                QMessageBox::warning(this, tr("Warning"), tr("Remote Point Table Cannot Find"));
            }
            else if (RecInfo.flag == 0x3)
            {
                //返回确认
                WriteTableLog(tr("Recv Conform Info!"));

            }

        }



    }
    else if (RecInfo.head == 0x69)
    {
        //请求历史数据
        FILETRANSDATA historydata;
        memcpy((char *)&historydata, btArray.data(), sizeof(FILETRANSDATA));
        
        //同意发送历史数据
        if (historydata.type == HISTORY_TYPE_CONFORM)
        {
            m_byHistory.clear();
            m_byNodeHistory.clear();
            //发送请求你数据
//             SendRequestHistory();
//             WriteTableLog(tr("Recv Conform can downLoad file!"));
        }
        else if (historydata.type == HISTORY_TYPE_REFUSE)
        {
            //拒绝发送历史数据文件
            QMessageBox::warning(this,tr("Warning"),tr("request refused!"));
            WriteTableLog(tr("request refused!"));
        }
//         else if (historydata.type == HISTORY_TYPE_UPDATE)
//         {
//             //接收数据
//             m_byNodeHistory.append(btArray);
// 
//             if (m_byNodeHistory.length() == sizeof(FILETRANSDATA))
//             {
//                 FILETRANSDATA historyNodedata;
//                 memcpy((char *)&historyNodedata, m_byNodeHistory.data(), sizeof(FILETRANSDATA));
//                 //和校验
//                 if (historyNodedata.crc == checkAllData(historyNodedata.con, g_ConstMaxLength))
//                 {
//                     m_byHistory.append((char *)historyNodedata.con, historyNodedata.length);
//                     //发送请求你数据
//                     SendRequestHistory();
//                 }
//                 else
//                 {
//                     m_byHistory.clear();
//                     WriteTableLog(tr("add Check Failed!"));
//                     QMessageBox::warning(this, tr("Warning"), tr("add Check Failed!"));
//                 }
// 
//                 m_byNodeHistory.clear();
// 
//             }
//         }
//         else if (historydata.type == HISTORY_TYPE_NODATE)
//         {
//             WriteTableLog(tr("History download success!"));
//             m_byNodeHistory.clear();
//             //开始写文件
//             SaveHistory();
//         }
    }
    else if (RecInfo.head == RECORD_CONFIG_HEAD)
    {
        //请求录波配置数据
        FILETRANSDATA fileTransData;
        memcpy((char *)&fileTransData, btArray.data(), sizeof(FILETRANSDATA));

        //同意发送录波配置数据
        if (fileTransData.type == HISTORY_TYPE_CONFORM)
        {
            m_byRecordConfig.clear();
            m_byRecodNodeConfig.clear();
            //发送请求你数据
            SendRequestRecordConfig();
            WriteTableLog(tr("Recv Conform can downLoad file!"));
        }
        else if (fileTransData.type == HISTORY_TYPE_REFUSE)
        {
            //拒绝发送历史数据文件
            QMessageBox::warning(this, tr("Warning"), tr("request refused!"));
            WriteTableLog(tr("request refused!"));
        }
        else if (fileTransData.type == HISTORY_TYPE_UPDATE)
        {
            //接收数据
            m_byRecodNodeConfig.append(btArray);

            if (m_byRecodNodeConfig.length() == sizeof(FILETRANSDATA))
            {
                FILETRANSDATA fileTransdata;
                memcpy((char *)&fileTransdata, m_byRecodNodeConfig.data(), sizeof(FILETRANSDATA));
                //和校验
                if (fileTransdata.crc == checkAllData(fileTransdata.con, g_ConstMaxLength))
                {
                    m_byRecordConfig.append((char *)fileTransdata.con, fileTransdata.length);
                    //发送请求你数据
                    SendRequestRecordConfig();
                }
                else
                {
                    m_byRecordConfig.clear();
                    WriteTableLog(tr("add Check Failed!"));
                    QMessageBox::warning(this, tr("Warning"), tr("add Check Failed!"));
                }

                m_byRecodNodeConfig.clear();

            }
        }
        else if (fileTransData.type == HISTORY_TYPE_NODATE)
        {
            WriteTableLog(tr("Record Config download success!"));
            m_byRecodNodeConfig.clear();
            //开始写文件
            SaveRecordConifg();
            //下载
            emit Signal_UpdateRecordFile();
        }
    }
    
}

unsigned char CMaintecanceWgt::checkAllData(const unsigned char *data, int length)
{
    unsigned char lValue = 0;
    for (int i = 0; i < length; i++)
    {
        lValue += data[i];
    }
    return lValue;
}

void CMaintecanceWgt::Slot_DcTrans()
{
    //直流调幅
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x6;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Dc Trans Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Dc Trans Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Dc Trans Request Success!"));
    }
}

void CMaintecanceWgt::Slot_AcTrans()
{
    //交流调整
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x7;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Ac Trans Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Ac Trans Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Ac Trans Request Success!"));
    }
}

void CMaintecanceWgt::Slot_DeviceInit()
{
    //装置初始化
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x9;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Device Init Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Device Init Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Device Init Request Success!"));
    }
}

void CMaintecanceWgt::Slot_Reset()
{
    //复位
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xa;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Reset Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Reset Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Reset Request Success!"));
    }
}

void CMaintecanceWgt::Slot_ReportClear()
{
    //报告清除
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xb;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Report Clean Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Report Clean Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Report Clean Request Success!"));
    }
}

void CMaintecanceWgt::Slot_ContextMenuRequest(const QPoint &)
{
    QMenu *pMenu = new QMenu(ui.tableWidget);

    QAction *pDownAct = new QAction(tr("Clear"), ui.tableWidget);

    pMenu->addAction(pDownAct);

    connect(pDownAct, SIGNAL(triggered()), this, SLOT(Slot_clearTableInfo()));

    pMenu->exec(QCursor::pos());
    pMenu->deleteLater();
}

void CMaintecanceWgt::Slot_clearTableInfo()
{
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
}

void CMaintecanceWgt::Slot_TextNetwork()
{
    //测试帧
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xff;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Text Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Text Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Text Request Success!"));
    }
    //每一秒钟发一次测试帧
    m_pTextTimer->start(1000);
}

void CMaintecanceWgt::Slot_TextTimeOut()
{
    m_nTimerout++;
    if (m_nTimerout > 20)
    {
        WriteTableLog(tr("Text Error,Plsase Check NetWork!"));
        m_pTextTimer->stop();
    }

    //每秒都发送测试针
    //测试帧
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xff;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        m_nTimerout = 0;
        m_pTextTimer->stop();
        WriteTableLog(tr("Send Text Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Text Request Failed!"));
    }
    else
    {
        //WriteTableLog(tr("Send Text Request Success!"));
    }
}

void CMaintecanceWgt::Slot_TimeSet()
{
    //
    m_pTimeWgt->exec();
}

void CMaintecanceWgt::Slot_SendTime(QDateTime &dataTime)
{
    //发送时间设置
    MAINTANCETIMEINFO dcTime;
    dcTime.head = 0x68;
    dcTime.length = 0x8;
    dcTime.type = 0x22;
    dcTime.flag = 0x1;
    dcTime.year = dataTime.date().year() - 2000;
    dcTime.month = dataTime.date().month();
    dcTime.day = dataTime.date().day();
    dcTime.hour = dataTime.time().hour();
    dcTime.minute = dataTime.time().minute();
    dcTime.second = dataTime.time().second();

    int nRet = m_pTcpSocket->write((char *)(&dcTime), sizeof(MAINTANCETIMEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Time Set Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Time Set Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Time Set Request Success!"));
    }
}

void CMaintecanceWgt::Slot_GetVersion()
{
    //版本信息
    //报告清除
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x23;
    dcZero.flag = 0x1;

    qDebug() <<"Sender:::"<< QByteArray((char *)(&dcZero), sizeof(MAINTANCEINFO)).toHex();

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Get Version infomation Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Get Version infomation Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Get Version infomation Request Success!"));
    }
}

void CMaintecanceWgt::Slot_SignalReset()
{
    //报告清除
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0x50;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Reset Signal Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Reset Signal Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Reset Signal Request Success!"));
    }
}

void CMaintecanceWgt::Slot_UpLoadPontFile()
{
//     QString strName = m_pInfoConfigWgt->GetFtpConfigName();
//     //检查当前装置远动点表
//     m_pFtpUpload->ReadXmlConfig(strName);
// 
//     //下装远动点表
//     m_pFtpUpload->show();
	//104  文件上传  点表文件
	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

										   //tWriteAction.m_nOperatorType = 7;    //写目录

										   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tReadAction.m_strFileName = "/tffs0//iec104sList.ini";    //文件名称

	tReadAction.m_nOperatorType = 9;    //只召唤目录

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
	//Slot_CheckDeviceTable();

	FILE_ATTR_INFO tWriteAction;

	tWriteAction.asduAddr.SetAddr(1);    //装置地址
	tWriteAction.cot.SetCot(COT_ACT);   //激活

	tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

	tWriteAction.m_nOperatorType = 7;    //写目录

	tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tWriteAction.m_strFileName = "/tffs0//" + m_strPointTableName.split("/").last();    //文件名称

	tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

	QFile tFile(m_strPointTableName);

	if (!tFile.open(QFile::ReadOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Point table file error!"));
		return;
	}

	tWriteAction.m_FileSize = tFile.size();   //文件大小

	tWriteAction.m_btArrayData =  tFile.readAll();   //发送数据


	m_pCommuncate->SendWriteActionRequest(tWriteAction);
}

void CMaintecanceWgt::SLot_UpLoadRequest()
{
    //下装点表请求报文
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xc;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Upload point table Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Upload point table Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Upload point table Request Success!"));
    }
}

void CMaintecanceWgt::Slot_CheckDeviceTable()
{

	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

	//tWriteAction.m_nOperatorType = 7;    //写目录

	//tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tReadAction.m_strFileName = "/tffs0//iec104sList.ini";    //文件名称

	tReadAction.m_nOperatorType = 1;

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);

}

void CMaintecanceWgt::Slot_UpLoadStatus(int nFlag)
{
    if (nFlag == 1)
    {
        //下装成功
        MAINTANCEINFO dcZero;
        dcZero.head = 0x68;
        dcZero.length = 0x2;
        dcZero.type = 0xc;
        dcZero.flag = 0x3;

        int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
        if (nRet == -1)
        {
            WriteTableLog(tr("Send Upload Confirm Failed!"));
        }
        else
        {
            WriteTableLog(tr("Send Upload Confirm Success!"));
        }
    }
    else if (nFlag == 2)
    {
        //下装失败
        MAINTANCEINFO dcZero;
        dcZero.head = 0x68;
        dcZero.length = 0x2;
        dcZero.type = 0xc;
        dcZero.flag = 0x4;

        int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
        if (nRet == -1)
        {
            WriteTableLog(tr("Send Upload Error Failed!"));
            QMessageBox::warning(this, tr("Warning"), tr("Send Upload Error Failed!"));
        }
        else
        {
            WriteTableLog(tr("Send Upload Error Success!"));
        }
    }
    else
    {
        //donoting
    }
}

void CMaintecanceWgt::Slot_CheckRemotePoint()
{
    //检查远动点表请求
    MAINTANCEINFO dcZero;
    dcZero.head = 0x68;
    dcZero.length = 0x2;
    dcZero.type = 0xd;
    dcZero.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Check Remote Point Table Request Failed!"));
        QMessageBox::warning(this, tr("Warning"), tr("Send Check Remote Point Table Request Failed!"));
    }
    else
    {
        WriteTableLog(tr("Send Check Remote Point Table Request Success!"));
    }
}

void CMaintecanceWgt::Slot_CheckStatus(int nFlag)
{
    if (nFlag == 1)
    {
        //下装成功
        MAINTANCEINFO dcZero;
        dcZero.head = 0x68;
        dcZero.length = 0x2;
        dcZero.type = 0xd;
        dcZero.flag = 0x3;

        int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
        if (nRet == -1)
        {
            WriteTableLog(tr("Send Check Remote Point Table Confirm Failed!"));
        }
        else
        {
            WriteTableLog(tr("Send Check Remote Point Table Confirm Success!"));
        }
    }
    else if (nFlag == 2)
    {
        //下装失败
        MAINTANCEINFO dcZero;
        dcZero.head = 0x68;
        dcZero.length = 0x2;
        dcZero.type = 0xd;
        dcZero.flag = 0x4;

        int nRet = m_pTcpSocket->write((char *)(&dcZero), sizeof(MAINTANCEINFO));
        if (nRet == -1)
        {
            WriteTableLog(tr("Send Check Remote Point Table Error Failed!"));
            QMessageBox::warning(this, tr("Warning"), tr("Send Check Remote Point Table Error Failed!"));
        }
        else
        {
            WriteTableLog(tr("Send Check Remote Point Table Error Success!"));
        }
    }
    else
    {
        //donoting
    }
}

void CMaintecanceWgt::Slot_GetHistoryData()
{
	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

										   //tWriteAction.m_nOperatorType = 7;    //写目录

										   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tReadAction.m_strFileName = "/tffs0//soeHstry.bin";    //文件名称

	tReadAction.m_nOperatorType = 1;

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
}


void CMaintecanceWgt::SendRequestHistory()
{
    //发送请求  历史数据
    FILETRANSDATA sendStruct;
    sendStruct.head = 0x69;
    sendStruct.itemIndex = 0;
    sendStruct.crc = 0;
    //请求数据
    sendStruct.type = HISTORY_TYPE_REQUEST;

    memset(sendStruct.con, 0, g_ConstMaxLength);

    int ret = m_pTcpSocket->write((char*)(&sendStruct), sizeof(sendStruct));

    if (ret == -1)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Send Update History error,Error:%1").arg(m_pTcpSocket->errorString()));
        return;
    }
}

void CMaintecanceWgt::SaveHistory()
{
    QString strRunPath = qApp->applicationDirPath(); //POINTTABLEPATH


    QString strFileName = strRunPath + HISTORYTABLE + "history_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".bin";

    QFile file(strFileName);

    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(this, tr("Warning"), tr("History file write error!"));
        return;
    }

    file.write(m_byHistory);
    file.close();
    m_byHistory.clear();
}

CCheckRemoteWgt * CMaintecanceWgt::GetftpPointWgt()
{
    return m_pFtpDownLoad;
}

void CMaintecanceWgt::Slot_GetRecordConfig()
{

//     m_pRecordDownLoad->show();
    //组织报文 召唤文件目录
	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
	//默认时间为所有时间

	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输


	tReadAction.m_strFileName = "/tffs0//COMTRADE";    //文件名称

	tReadAction.m_nOperatorType = 1;

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
}

void CMaintecanceWgt::SendRequestRecordConfig()
{
    //发送请求  录波配置文件
    FILETRANSDATA sendStruct;
    sendStruct.head = RECORD_CONFIG_HEAD;
    sendStruct.itemIndex = 0;
    sendStruct.crc = 0;
    //请求数据
    sendStruct.type = HISTORY_TYPE_REQUEST;

	memset(sendStruct.con, 0, g_ConstMaxLength);;

    int ret = m_pTcpSocket->write((char*)(&sendStruct), sizeof(sendStruct));

    if (ret == -1)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Send Update Record Conifg error,Error:%1").arg(m_pTcpSocket->errorString()));
        return;
    }
}

void CMaintecanceWgt::SaveRecordConifg()
{
    QFile file(m_strRecordConfig);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Warning"), tr("History file write error!"));
        return;
    }

    file.write(m_byRecordConfig);
    file.close();
    m_byRecordConfig.clear();
}

void CMaintecanceWgt::SetRecordConifgName(const QString &strRecordName)
{
    m_strRecordConfig = strRecordName;
}

void CMaintecanceWgt::Slot_GetCmdHistory()
{
	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

										   //tWriteAction.m_nOperatorType = 7;    //写目录

										   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tReadAction.m_strFileName = "/tffs0//CmdHistory.txt";    //文件名称

	tReadAction.m_nOperatorType = 1;

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
}

void CMaintecanceWgt::Slot_LcdRequesetTimer()
{
    //发送请求数据

    MAINTANCEINFO lcdRequest;
    lcdRequest.head = 0x68;
    lcdRequest.length = 0x2;
    lcdRequest.type = 0x28;
    lcdRequest.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&lcdRequest), sizeof(MAINTANCEINFO));
    if (nRet == -1)
    {
        WriteTableLog(tr("Send Lcd Request Failed!"));
        m_pTimeLcd->stop();
        m_pLedTimer->stop();
        m_pLcdOperatorWgt->WriteTableLog(tr("Send Lcd Request Failed"));
    }
    else
    {
       // WriteTableLog(tr("Send Lcd Request Success!"));
    }
}

void CMaintecanceWgt::StartLcdTimer()
{
    m_pTimeLcd->start();
    //m_pLedTimer->start();
}

void CMaintecanceWgt::StopLcdTimer()
{
//     m_pTimeLcd->stop();
//     m_pLedTimer->stop();
}

void CMaintecanceWgt::Slot_SendLcdClickOperator(int nFlag)
{
    MAINTANCEINFO lcdRequest;
    lcdRequest.head = 0x68;
    lcdRequest.length = 0x2;
    lcdRequest.type = 0x29;
    lcdRequest.flag = nFlag;

    //lcdRequest.keyValue = nFlag;

    int nRet = m_pTcpSocket->write((char *)(&lcdRequest), sizeof(MAINTANCEINFO));

    if (nRet == -1)
    {
        WriteTableLog(tr("Send Lcd Request Failed!"));
        m_pLcdOperatorWgt->WriteTableLog(tr("Send Lcd Request Failed"));
        m_pLedTimer->stop();
        m_pTimeLcd->stop();
    }
    else
    {
        // WriteTableLog(tr("Send Lcd Request Success!"));
    }
}

void CMaintecanceWgt::Slot_GetLedTimerOut()
{
    MAINTANCEINFO lcdRequest;
    lcdRequest.head = 0x68;
    lcdRequest.length = 0x2;
    lcdRequest.type = 0x30;
    lcdRequest.flag = 0x1;

    int nRet = m_pTcpSocket->write((char *)(&lcdRequest), sizeof(MAINTANCEINFO));

    if (nRet == -1)
    {
        WriteTableLog(tr("Send LED Request Failed!"));
        m_pLedTimer->stop();
        m_pTimeLcd->stop();

    }
    else
    {
        // WriteTableLog(tr("Send Lcd Request Success!"));
    }

}

void CMaintecanceWgt::Slot_UploadVirutalBinary()
{
    //QString strName = m_pInfoConfigWgt->GetFtpConfigName();
    //检查当前装置远动点表
   // m_pVirtualBinaryUpload->ReadXmlConfig(strName);

    //下装远动点表
    //m_pVirtualBinaryUpload->show();

	FILE_CATALOG_REQUEST_1 tCatalogRequest;

	tCatalogRequest.asduAddr.SetAddr(1);    //装置地址
	tCatalogRequest.cot.SetCot(COT_REQ);   //激活

	tCatalogRequest.type = D_FILE_TRANSPORT;   //文件传输

	tCatalogRequest.m_nOperatorType = 1;    //读目录
	tCatalogRequest.m_nCatalogID.SetAddr(0);       //目录ID  暂时置为0

	strcpy(tCatalogRequest.m_cCatalogName, "/tffs0/\0");    //目录
	tCatalogRequest.m_uCatalogLength = strlen(tCatalogRequest.m_cCatalogName);   //目录长度

	tCatalogRequest.m_nCallFlag = 0;   //目录下所有文件
									   //默认时间为所有时间

									   //发送数据


	FILE_ATTR_INFO tReadAction;

	tReadAction.asduAddr.SetAddr(1);    //装置地址
	tReadAction.cot.SetCot(COT_ACT);   //激活

	tReadAction.type = D_FILE_TRANSPORT;   //文件传输

										   //tWriteAction.m_nOperatorType = 7;    //写目录

										   //tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tReadAction.m_strFileName = "/tffs0//merge.bin";    //文件名称

	tReadAction.m_nOperatorType = 9;    //只召唤目录

	tReadAction.m_cFileNameLength = tReadAction.m_strFileName.length();   //文件长度

	tReadAction.m_cFileAttr = 0; //文件

	tCatalogRequest.m_fileInfo = tReadAction;

	m_pCommuncate->SendCatalogRequest(tCatalogRequest);
	//Slot_CheckDeviceTable();

	FILE_ATTR_INFO tWriteAction;

	tWriteAction.asduAddr.SetAddr(1);    //装置地址
	tWriteAction.cot.SetCot(COT_ACT);   //激活

	tWriteAction.type = D_FILE_TRANSPORT;   //文件传输

	tWriteAction.m_nOperatorType = 7;    //写目录

	tWriteAction.m_nFileID = 0;       //文件ID  暂时置为0

	tWriteAction.m_strFileName = "/tffs0//merge.bin";    //文件名称

	tWriteAction.m_cFileNameLength = tWriteAction.m_strFileName.length();   //目录长度

	QString strRunPath = qApp->applicationDirPath();

	QFile tFile(strRunPath + "/ini/f220/merge/merge.bin");

	if (!tFile.open(QFile::ReadOnly))
	{
		QMessageBox::warning(this, tr("Warning"), tr("Point table file error!"));
		return;
	}

	tWriteAction.m_FileSize = tFile.size();   //文件大小

	tWriteAction.m_btArrayData = tFile.readAll();   //发送数据


	m_pCommuncate->SendWriteActionRequest(tWriteAction);
}










CTimeChooseWgt::CTimeChooseWgt(QWidget *parent /*= 0*/)
{
    Q_UNUSED(parent);
    m_pCalenderItem = new QCalendarWidget(this);
    m_pCTimeEdit = new QTimeEdit(this);


    QPushButton *pConform = new QPushButton(this);
    pConform->setText(tr("Conform"));
    QPushButton *pCancel = new QPushButton(this);
    pCancel->setText(tr("Cancel"));

    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addStretch();
    pHLayout->addWidget(pConform);
    pHLayout->addWidget(pCancel);

    QVBoxLayout *pVlayout = new QVBoxLayout(this);
    pVlayout->addWidget(m_pCalenderItem);
    pVlayout->addWidget(m_pCTimeEdit);
    pVlayout->addLayout(pHLayout);


    connect(pConform, SIGNAL(clicked()), this, SLOT(Slot_Conform()));
    connect(pCancel, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));
}

CTimeChooseWgt::~CTimeChooseWgt()
{

}

void CTimeChooseWgt::Slot_Conform()
{
    QDateTime dataTime;
    dataTime.setDate(m_pCalenderItem->selectedDate());
    dataTime.setTime(m_pCTimeEdit->time());
    emit Signal_SetTime(dataTime);
    this->close();
}

void CTimeChooseWgt::Slot_Cancel()
{
    this->close();
}
