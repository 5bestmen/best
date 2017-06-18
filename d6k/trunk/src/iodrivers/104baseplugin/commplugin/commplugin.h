/********************************************************************************
模块名       :  APDU数据管理类
文件名       :  commplugin.h
文件实现功能 :  APDU结构
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef COMMPLUGIN_H
#define COMMPLUGIN_H

#include <QObject>
#include <QThread>
#include <QAbstractSocket>
#include "commplugin_global.h"
#include "104cache.h"
#include "apdurecver.h"
#include "icommplugin.h"
#include "asdu_info.h"
#include "ftu_module.h"


class CSocketThread;
class QTimer;
class CApduSender;
class CFtuModule;
struct NBM_TELECTRL;

class  CCommPlugin :public ICommPlugin
{
    Q_OBJECT
public:
    CCommPlugin(CFtuModule *pModule);
    ~CCommPlugin();
    //开始运行
	void StartRun(const QString &strIP, int iPort);
	//结束运行
	void StopRun();

    bool OnReceive(char *pBuff, int nLen);
    //写入数据
    bool WriteApduData(const char *pSendData, int iLength);
    //发送U格式信息
    bool SendUData(int nType);

    CApduSender* getSender();

	CApduRecver *GetRecver()
	{
		return m_pApduRecver;
	}

    bool OnCommand(NBM_TELECTRL* pTelectrl);
	// 遥控命令
	void SetControlBckCommand(int iControlType, int ipointNum, int OperateType);
    //
    CFtuModule *GetFtpModule();
    //
    void SetGeneralTime(int nGeneralTime, int nSynTime, int nKwhTime);
    //


public:
        bool OnTimerSend(int nFrameType,int nUtype);
        bool OnTimerRecv(int nFrameType);

        bool OnKwSend(int nFrameType);
        bool OnKwRecv(int nFrameType,int nRecvNum);

        bool OnNumRecv(int nFrameType,int nSendNum);
        bool OnNumSend(int nFrameType);

public:
        bool OnSendFrame(int nFrameType,int nUtype=-1);
        bool OnRecvFrame(int nFrameType,int nSendNum,int nRecvNum);
public:
        bool IsRunning() { return m_bIsRunning; }
        bool m_bIsRunning;
public:
        enum { MAX_SERIAL_NUMBER = 32767,};//序列号15位，最大值
        unsigned short  m_nRecvNum;//本端接收到多少个I帧，初始为0，接收到1个加1
        unsigned short  m_nSendNum;//本端发送了多少个I帧，初始为0，发送了1个加1
//k,w值逻辑
public:
        int m_k; //发送了的I帧中，还有m_k个没有收到对端的确认
        int m_w;//接收到的I帧中，还有m_w个没有给对端发送确认
private slots:
    void Slot_OnRecvData();
    void Slot_SocketError(QString error);
    //连接成功
	void Slot_sockeConnectSuccess(QString strLocalInfo);
	//t0超时
	void Slot_TimeOutT0();
	//t1超时
	void Slot_TimeOutT1();
	//t2超时
	void Slot_TimeOutT2();
	//t3超时
	void Slot_timeOutT3();
	//总召超时
	void Slot_SendGeneralResquestMsg();
    //时间同步
    void Slot_SendSyncRequestMsg();
    //电度召唤
    void Slot_SendKwhTimeRequestMsg();
    //时间手动同步
    void Slot_SetSycsTime(QDateTime tSycsTime);
    //复位进程
    void Slot_ResetProcess();

public slots:
	// 遥控命令
    void Slot_SetControlCommand(int iControlType, int ipointNum, int OperateType, int nMessType);
	//定值指令
	void Slot_setDevOrder(DEV_BASE &devData);
    //录波文件获取指令
    void Slot_SetRecordOrder(QList<LB_DATA> &lbData);
    //死区
    void Slot_SetZoomArea(ZOOM_BASE &zoomBaseInfo);
	//切换定值区
	void Slot_SwitchFixArea(unsigned short uFixArea);
	//读取当前定值区号
	void Slot_ReadCurrentFixArea();

	//目录召唤
	void Slot_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest);
	//文件写入激活
	void Slot_WriteAction(FILE_ATTR_INFO &tFileInfo);
	//文件读取激活
	void Slot_ReadAction(FILE_ATTR_INFO &tFileInfo);
signals:
	//发送出去的数据
	void Signal_Write16Data(QByteArray btbuff);
	//接收到的数据报文
	void Signal_recv16Data(QByteArray btData, int nLenght);

private:
    CSocketThread *m_pSocketThread;
    //定时提取数据
   // QTimer *m_pTimer;
    //线程
    QThread m_thrdSocket;
	//线程2
	QThread m_thrdAnalyse;
    //发送类
    CApduSender *m_pApduSender;
    //接收类
    CApduRecver *m_pApduRecver;
	//IP地址
	QString m_strIP;
	//端口号
	int m_iPort;
	//四个超时设置
	QTimer *m_pTimerOut0;
	QTimer *m_pTimerOut1;
	QTimer *m_pTimerOut2;
	QTimer *m_pTimerOut3;
	//定时发送总召
	QTimer *m_pTimeGeneralSendMsg;
    //定时发送时间同步
    QTimer *m_pTimerSyncTimeMsg;
    //定时发送电度召唤
    QTimer *m_pTimerKwhMsg;

	QThread m_tdRunThread;
    CFtuModule *m_pModule;
};

#endif // COMMPLUGIN_H
