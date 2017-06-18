#ifndef MAINTECANCEWGT_H
#define MAINTECANCEWGT_H

#include <QWidget>
#include <QAbstractSocket>
#include <QDialog>
#include "ui_maintecancewgt.h"

typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位
typedef unsigned char   BIT08;  //08位

#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))

#define HISTORY_TYPE_LINK        0x01            //请求发送历史数据
#define HISTORY_TYPE_CONFORM     0x02            //请求确认
#define HISTORY_TYPE_REFUSE      0x03            //请求拒绝

#define HISTORY_TYPE_REQUEST     0x04            //请求数据
#define HISTORY_TYPE_UPDATE      0x05            //发送数据

#define HISTORY_TYPE_NODATE      0x06            //工具下发程序，已全部发完，无数据

#define RECORD_CONFIG_HEAD       0x70                //录波配置文件头

//数据发送单个包的长度
const  int g_ConstMaxLength = 256;


#pragma pack(1) 

typedef struct FILETRANSDATA
{
    unsigned char head;  				// 头0x69 历史数据文件  头0x70 录波配置文件
    unsigned short itemIndex;   	    // 传输编号  从1开始  累计加一
    unsigned short length;              //文件长度  con有效长度
    unsigned char type;					// 类型   请求  确认等等
    unsigned char con[256];				// 实际数据
    unsigned char crc;		// con里数据的“和校验”，校验不包括head、len、type
}FILETRANSDATA;

class VERSIONADDR 
{
public:
    unsigned char m_asduaddr0;
    unsigned char m_asduaddr1;
public:
    void SetAddr(unsigned short addr)
    {
        m_asduaddr0 = MYLO8(addr);
        m_asduaddr1 = MYHI8(addr);
    }
    unsigned short GetAddr()
    {
        return MYMAKE16(m_asduaddr0, m_asduaddr1);
    }
};

class MAINTANCEINFO
{
public:
    unsigned char head;        //报文头
    unsigned char length;      //报文长度
    unsigned char type;        //标示类型
    unsigned char flag;        //功能表示
};

class MAINTANCETIMEINFO :public MAINTANCEINFO
{
public:
    unsigned char year;          //年
    unsigned char month;         //月
    unsigned char day;         //日
    unsigned char hour;         //时
    unsigned char minute;         //分
    unsigned char second;         //秒
};



//液晶显示数据
class LCDCONTROLINFO :public MAINTANCEINFO
{
public:
    unsigned char cursorRow;        //光标所在行
    unsigned char cursorColumn;     //光标所在列

    unsigned char ledData[240];
};

class LCDKEYINFO :public MAINTANCEINFO
{
public:
    unsigned char keyValue;    //按键编号
};

struct LIGHTSTATUS
{
    unsigned char EIGHT : 1;     //
    unsigned char SEVEN : 1;     //
    unsigned char SIX : 1;     //
    unsigned char FIVE : 1;     //
    unsigned char FOUR : 1;     //
    unsigned char THREE : 1;     //
    unsigned char TWO : 1;    //
    unsigned char ONE : 1;    //
};

class LEDLIGHTINFO : public MAINTANCEINFO
{
public: 
//     LIGHTSTATUS FirstLights;
//     LIGHTSTATUS SecondLights;
    unsigned char ledBuff[256];
};

class MAINTANCEDAY
{
public:
    unsigned char year;          //年
    unsigned char month;         //月
    unsigned char day;         //日
    unsigned char hour;         //时
    unsigned char minute;         //分
    unsigned char second;         //秒

};
class MAINTANCEVERSION :public MAINTANCEINFO
{
public:
    char dspmain[8];       //DSP设备名称
    char dspReserve[12];
    char dsTime[20];      //DSP子形成时间
    char dsVersion[6];     //dsp版本
    char dsCode[6];     //dsp code

    char armMain[8];       //DSP主版本号
    char armReserve[12];
    char armTime[20];      //arm 设备名称
    char armVersion[6];     //arm版本
    char armCode[6];     //arm code

};

#pragma pack()

class QTcpSocket;
class QTimer;
class CTimeChooseWgt;
class CFtpUploadWgt;
class CCheckRemoteWgt;
class CTranInfo;
class CInfoConfigWgt;
class CRecordDownDlg;
class CLcdoperatorWgt;
class CCommThread;

class CMaintecanceWgt : public QWidget
{
    Q_OBJECT

public:
    CMaintecanceWgt(CCommThread *pCommunicate,CInfoConfigWgt *pConfigWgt, CLcdoperatorWgt *plcdOperatorWgt, QString strPontName, QWidget *parent = 0);
    ~CMaintecanceWgt();

    void WriteTableLog(const QString &strContext);
    //不断发送请求历史数据
    void SendRequestHistory();
    //保存历史数据
    void SaveHistory();
    //和校验
    unsigned char checkAllData(const unsigned char *data, int length);
    //获取ftp
    CCheckRemoteWgt *GetftpPointWgt();
    //请求发送录波配置文件数据
    void SendRequestRecordConfig();
    //保存录波配置文件信息
    void SaveRecordConifg();
    //设置录波配置文件名称
    void SetRecordConifgName(const QString &strRecordName);
    //启动定时器
    void StartLcdTimer();
    //关闭定时器
    void StopLcdTimer();
signals:
    //录波配置文件下载完成后发出信号
    void Signal_UpdateRecordFile();

public slots:
     //table右击菜单
    void Slot_ContextMenuRequest(const QPoint &);
    //清空表格
    void Slot_clearTableInfo();
     //连接
    void Slot_ConnectSocket();
    //断开连接
    void Slot_DisConnectSocket();
    //连接成功
    void Slot_ConnectSuccess();
    //断开连接
    void Slot_DisConnect();
    //socket错误
    void Slot_SocketError(QAbstractSocket::SocketError error);
    //接收数据
    void Slot_RecvNewData();
    //直流调零
    void Slot_SetDcZero();
    //直流调幅
    void Slot_DcTrans();
    //交流调整
    void Slot_AcTrans();
    //装置初始化
    void Slot_DeviceInit();
    //复位
    void Slot_Reset();
    //报告清除
    void Slot_ReportClear();
    //测试帧
    void Slot_TextNetwork();
    //定时发送测试帧
    void Slot_TextTimeOut();
    //时间设置
    void Slot_TimeSet();
    //发送时间
    void Slot_SendTime(QDateTime &dataTime);
    //版本信息
    void Slot_GetVersion();
    //下装远动点表界面
    void Slot_UpLoadPontFile();
    //下装请求
    void SLot_UpLoadRequest();
    //下装成功失败标识 1 成功  2 失败
    void Slot_UpLoadStatus(int nFlag);
    //检查请求
    void Slot_CheckRemotePoint();
    //检查成功失败标识 1成功  2失败
    void Slot_CheckStatus(int nFlag);
    //信号复归'
    void Slot_SignalReset();
    //检查当前装置的远动点表
    void Slot_CheckDeviceTable();
    //获取历史数据
    void Slot_GetHistoryData();
    //获取录波配置文件
    void Slot_GetRecordConfig();
    //获取操作历史文件
    void Slot_GetCmdHistory();
    //上传虚遥信数据
    void Slot_UploadVirutalBinary();
    //定时发送液晶显示请求
    void Slot_LcdRequesetTimer();
    //
    void Slot_SendLcdClickOperator(int nFlag);
    //
    void Slot_GetLedTimerOut();

private:
    Ui::CMaintecanceWgt ui;
    QTcpSocket *m_pTcpSocket;
    QTimer *m_pTextTimer;
    //时间
    int m_nTimerout;
    //选择时间界面
    CTimeChooseWgt *m_pTimeWgt;
    //FTp上传文件对象
    CFtpUploadWgt *m_pFtpUpload;
    //ftp下载文件对象
    CCheckRemoteWgt *m_pFtpDownLoad;
    //ftp cmd 历史文件下载
    CCheckRemoteWgt *m_pCmdFtpDownLoad;
    //ftp 录波文件
    CRecordDownDlg *m_pRecordDownLoad;
    //接收到历史文件数据
    QByteArray m_byNodeHistory;
    //历史数据
    QByteArray m_byHistory;
    //接收到录波配置文件数据
    QByteArray m_byRecodNodeConfig;
    //录波配置数据
    QByteArray m_byRecordConfig;
    //录波配置文件名称
    QString m_strRecordConfig;
    //
    CInfoConfigWgt *m_pInfoConfigWgt;
    //
    CLcdoperatorWgt *m_pLcdOperatorWgt;
    //定时发送液晶请求数据
    QTimer *m_pTimeLcd;
    //定时获取led灯信息
    QTimer *m_pLedTimer;
    //上传虚遥信
    CFtpUploadWgt *m_pVirtualBinaryUpload;
    //ftp下载历史数据
    CCheckRemoteWgt *m_pFtpHistoryDownLoad;
	//
	CCommThread *m_pCommuncate;
	//点表名称
	QString m_strPointTableName;
};


class QCalendarWidget;
class QTimeEdit;
class CTimeChooseWgt :public QDialog
{
    Q_OBJECT
public:
    CTimeChooseWgt(QWidget *parent = 0);
    ~CTimeChooseWgt();
signals:
    void Signal_SetTime(QDateTime &dataTime);
public slots:
    void Slot_Conform();
    void Slot_Cancel();
private:
    //日历
    QCalendarWidget *m_pCalenderItem;
    //时间
    QTimeEdit *m_pCTimeEdit;

};


#endif // MAINTECANCEWGT_H
