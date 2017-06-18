#include <QMEssageBox>
#include <QTcpSocket>
#include <QDateTime>
#include <QTimer>
#include <QStringList>
#include <QNetworkConfigurationManager>
#include "tcpclient.h"
#include "ui_tcpclient.h"

CTcpClient::CTcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTcpClient),
    m_pTcpSocket(new QTcpSocket(this)),
    m_pTimerNetWork(new QTimer(this)),
    m_pCycleSend(new QTimer(this)),
    m_pCycleClose(new QTimer(this)),
    m_pRecvTimeOut(new QTimer(this))
{
    ui->setupUi(this);
    m_uLongConnectTimes = 0;
    m_uLongSendTimes = 0;
    m_uLongRecvTimes = 0;

    setWindowTitle(tr("TCP客户端自动化测试工具"));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0,300);
    connect (m_pTcpSocket,SIGNAL(connected()),this,SLOT(slot_connectSuccess()));
    connect (m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(slot_disconnect()));

    connect (m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slot_socketError(QAbstractSocket::SocketError)));
    connect (m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(Slot_readyReadNetData()));
    connect (m_pTimerNetWork,SIGNAL(timeout()),this,SLOT(Slot_CheckNetWork()));
    connect (m_pCycleClose,SIGNAL(timeout()),this,SLOT(Slot_cycleStop()));
    connect (m_pCycleSend,SIGNAL(timeout()),this,SLOT(Slot_CycleSendMsg()));
    connect (m_pTcpSocket,SIGNAL(bytesWritten(qint64)),this,SLOT(slot_sendbytesData(qint64)));

    m_pTimerNetWork->setInterval(2000);
    //接收超时为1s
    m_pRecvTimeOut->setInterval(1000);
    connect (m_pRecvTimeOut,SIGNAL(timeout()),this,SLOT(Slot_RecvTimeOut()));
}

CTcpClient::~CTcpClient()
{
    delete ui;
}

void CTcpClient::WriteLog(const QString &strLog)
{
    if (ui->tableWidget->rowCount() > 10000)
    {
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
    }
    //添加一行记录日志
    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setText(strLog);
    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->tableWidget->insertRow(0);

    ui->tableWidget->setItem(0,1,item0);
    ui->tableWidget->setItem(0,0,item1);
}

void CTcpClient::on_lineEdit_3_textEdited(const QString &arg1)
{
    int iTotalNums = arg1.toInt();

    if (iTotalNums <= 0)
    {
        QMessageBox::warning(this,tr("告警"),tr("请输入一个正整数!"));
        ui->lineEdit_3->clear();
        return;
    }

    QString strNewData = "";

    int iCurrentNum = 0;

    for (int i=0; i<iTotalNums; i++)
    {
        iCurrentNum = i%256;

        QString strNums = QString::number(iCurrentNum,16).toUpper();

        if (strNums.count() == 1)
        {
            strNums = "0" + strNums;
        }

        strNewData.append(strNums);

        strNewData.append(" ");
    }

    ui->textEdit->setText(strNewData);
}

void CTcpClient::on_textEdit_textChanged()
{
    QByteArray bAInputData = hexStringtoByteArray(ui->textEdit->toPlainText());

    ui->lineEdit_3->setText(QString::number(bAInputData.length()));
}

void CTcpClient::slot_connectSuccess()
{
    UpdateConnectTimes();
    //记录日志
    WriteLog("连接成功....");
}

void CTcpClient::slot_disconnect()
{
    WriteLog("连接断开....");
    m_pTcpSocket->close();
}

void CTcpClient::slot_socketError(QAbstractSocket::SocketError)
{
    WriteLog(tr("socker错误,错误原因:%1").arg(m_pTcpSocket->errorString()));
    m_pTcpSocket->close();
}

void CTcpClient::Slot_CheckNetWork()
{
    if (m_pTcpSocket->state() < 3 )
    {
        //重新连接
        m_pTcpSocket->abort();
        m_pTcpSocket->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());
        WriteLog("连接处于断开状态,正在连接服务器....");
    }
}

void CTcpClient::on_pushButton_clicked()
{
    //连接
    m_pTcpSocket->abort();
    m_pTcpSocket->connectToHost(ui->lineEdit->text(),ui->lineEdit_2->text().toInt());
    //记录日志
    WriteLog("开始连接服务器....");

    m_pTimerNetWork->start();
}

void CTcpClient::on_pushButton_3_clicked()
{
    ui->pushButton->setDisabled(true);
    if (ui->textEdit->toPlainText().isEmpty() || ui->lineEdit_3->text().toInt()<=0)
    {
        QMessageBox::warning(this,tr("告警"),tr("发送内容不能为空!"));
        return;
    }

    if (ui->lineEdit_4->text().toInt() <= 0)
    {
        QMessageBox::warning(this,tr("告警"),tr("循环发送时间间隔大于0!"));
        return;
    }

    if (ui->lineEdit_5->text().toInt() == 0)
    {
        QMessageBox::warning(this,tr("告警"),tr("循环断开时间间隔不等于0,当小于0时代表不断开!"));
        return;
    }

    //发送tcp信息
    QByteArray bAInputData = hexStringtoByteArray(ui->textEdit->toPlainText());

    qint64 sendLength =  m_pTcpSocket->write(bAInputData.data(),bAInputData.length());
    if (sendLength == -1)
    {
        WriteLog(tr("数据发送失败"));
        m_pTcpSocket->abort();
        m_pTimerNetWork->start();
    }
    else
    {
        //开始定时器
        m_pRecvTimeOut->start();
        //WriteLog(tr("数据发送成功,长度为%1").arg(sendLength));
    }
    //开启定时器
    if (ui->comboBox->currentIndex() == 0)
    {
        //毫秒
        m_pCycleSend->start(ui->lineEdit_4->text().toInt());
    }
    else
    {
        //秒
        int iSenconds = ui->lineEdit_4->text().toInt() * 1000;
        m_pCycleSend->start(iSenconds);
    }

    if (ui->lineEdit_5->text().toInt() < 0)
    {
        m_pCycleClose->stop();
    }
    else
    {
        if (ui->comboBox_2->currentIndex() == 0)
        {
            //毫秒
            m_pCycleClose->start(ui->lineEdit_5->text().toInt());
        }
        else
        {
            //秒
            int iSenconds = ui->lineEdit_5->text().toInt() * 1000;
            m_pCycleClose->start(iSenconds);
        }

    }
}

void CTcpClient::on_pushButton_2_clicked()
{
    ui->pushButton->setDisabled(false);
    //断开连接
    m_pTcpSocket->abort();
    WriteLog("连接手动断开....");

    m_pTimerNetWork->stop();
    m_pCycleClose->stop();
    m_pCycleSend->stop();
}

void CTcpClient::Slot_CycleSendMsg()
{
    //发送信息
    //qDebug()<<m_pTcpSocket->state();
    if (m_pTcpSocket->state() < 3 )
    {
        m_pTcpSocket->abort();
        if (!m_pTimerNetWork->isActive())
        {
            m_pTimerNetWork->start();
        }
        return;
    }

    //发送tcp信息
    //发送tcp信息
    QByteArray bAInputData = hexStringtoByteArray(ui->textEdit->toPlainText());

    qint64 sendLength =  m_pTcpSocket->write(bAInputData.data(),bAInputData.length());

    m_pTcpSocket->waitForBytesWritten();

    if (sendLength == -1)
    {
        WriteLog(tr("数据发送失败"));
        m_pTcpSocket->abort();
        if (!m_pTimerNetWork->isActive())
        {
            m_pTimerNetWork->start();
        }

    }
    else
    {
        if (!m_pRecvTimeOut->isActive())
        {
            m_pRecvTimeOut->start();
        }
        //WriteLog(tr("数据发送成功,长度为%1").arg(sendLength));
    }
}

void CTcpClient::Slot_cycleStop()
{
    m_pTcpSocket->abort();
    WriteLog("循环关闭socket连接....");
}

void CTcpClient::slot_sendbytesData(qint64 bytes)
{
    //更新发送个数
    UpdateSendTimes();
    WriteLog(tr("数据发送成功,长度为%1").arg(bytes));
}

void CTcpClient::Slot_RecvTimeOut()
{
    //接收数据超时
    WriteLog(tr("接收超时,没有收到信息..."));
    //m_pTcpSocket->close();
    m_pRecvTimeOut->stop();
}

void CTcpClient::Slot_readyReadNetData()
{
    //开始读取数据
    while (m_pTcpSocket->bytesAvailable())
    {
        //更新次数
        UpdateRecvTimes();
        //数据解析
        QByteArray btArray = m_pTcpSocket->readAll();
        QString strHex = btArray.toHex();
        QString strNewHex = "";
        for (int i=0; i<strHex.count(); i++)
        {
            if (i%2 == 0)
            {
                strNewHex.append(" ");
                strNewHex.append(strHex.at(i));
            }
            else
            {
                strNewHex.append(strHex.at(i));
            }
        }
        WriteLog(tr("接收到数据,长度为%1,内容为%2").arg(btArray.length()).arg(strNewHex));
        m_pRecvTimeOut->stop();
    }
}

QByteArray CTcpClient::hexStringtoByteArray(QString hex)
{
    QByteArray ret;
    hex=hex.trimmed();
    //补0
    QStringList lstData = hex.split(" ");
    QString strNewHex = "";

    if(lstData.count() > 1)
    {
        for (int i=0; i<lstData.count(); i++)
        {
            if (lstData.at(i).count() == 1)
            {
                QString strNode = "0" + lstData.at(i);
                strNewHex.append(strNode);
            }
            else if(lstData.at(i).count() == 2)
            {
                strNewHex.append(lstData.at(i));
            }
            else if(lstData.at(i).count() > 2)
            {
                QMessageBox::warning(this,tr("告警"),tr("输入的数据格式不正确,请检查:%1").arg(lstData.at(i)));
                return "";
            }
        }
        hex = strNewHex;
    }


    formatString(hex,2,' ');
    QStringList sl=hex.split(" ");
    foreach(QString s,sl)
    {
        if(!s.isEmpty())
            ret.append((char)s.toInt(0,16)&0xFF);
    }
    return ret;
}

void CTcpClient::formatString(QString &org, int n, const QChar &ch)
{
    int size= org.size();
    int space= qRound(size*1.0/n+0.5)-1;
    if(space <= 0)
    {
        return;
    }
    for(int i=0,pos=n;i<space;++i,pos+=(n+1))
    {
        org.insert(pos,ch);
    }
}

void CTcpClient::UpdateConnectTimes()
{
    m_uLongConnectTimes++;
    ui->label_10->setText(QString::number(m_uLongConnectTimes));
}

void CTcpClient::UpdateSendTimes()
{
    m_uLongSendTimes++;
    ui->label_11->setText(QString::number(m_uLongSendTimes));
}

void CTcpClient::UpdateRecvTimes()
{
    m_uLongRecvTimes++;
    ui->label_12->setText(QString::number(m_uLongRecvTimes));
}

void CTcpClient::on_pushButton_4_clicked()
{
    //重新统计
    m_uLongConnectTimes = 0;
    m_uLongSendTimes = 0;
    m_uLongRecvTimes = 0;
    //更新数据
    ui->label_10->setText("0");
    ui->label_11->setText("0");
    ui->label_12->setText("0");
}
