#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QAbstractSocket>

namespace Ui {
class CTcpClient;
}

class QTcpSocket;
class QTimer;
class CTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit CTcpClient(QWidget *parent = 0);
    ~CTcpClient();
    //记录日志
    void WriteLog(const QString &strLog);
    //十六进制转换
    QByteArray hexStringtoByteArray(QString hex);

    void formatString(QString &org, int n=2, const QChar &ch=QChar(' '));
    //更新连接次数
    void UpdateConnectTimes();
    //更新发送次数
    void UpdateSendTimes();
    //更新接收次数
    void UpdateRecvTimes();

private slots:
    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_textEdit_textChanged();
    void slot_connectSuccess();
    void slot_disconnect();
    void slot_socketError(QAbstractSocket::SocketError);

    void Slot_CheckNetWork();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();
    //循环发送
    void Slot_CycleSendMsg();
    //循环断开
    void Slot_cycleStop();
    //发送的字节数
    void slot_sendbytesData(qint64 bytes);
    //接收超时
    void Slot_RecvTimeOut();
    //
    void Slot_readyReadNetData();

    void on_pushButton_4_clicked();

private:
    Ui::CTcpClient *ui;
    QTcpSocket * m_pTcpSocket;
    //定时检测网络状态
    QTimer *m_pTimerNetWork;
    //循环发送时间间隔
    QTimer *m_pCycleSend;
    //循环断开时间间隔
    QTimer *m_pCycleClose;
    //接收超时定时器
    QTimer *m_pRecvTimeOut;
    //连接次数
    unsigned long m_uLongConnectTimes;
    //发送次数、
    unsigned long m_uLongSendTimes;
    //接收次数
    unsigned long m_uLongRecvTimes;
};

#endif // TCPCLIENT_H
