#ifndef COMMTHREAD_H
#define COMMTHREAD_H

#include <QThread>
#include <QDateTime>
#include <QMap>
#include "asdu_info.h"

class CFtuModule;

class CCommThread : public QThread
{
	Q_OBJECT

public:
	CCommThread(QObject *parent);
	~CCommThread();
	//开始运行
	void StartRun(const char* strIP, int iPort);
	//获取发送方信息
	QString GetOwnNetInfo();
	//获取对方信息
	QString GetOtherInfo();
	//发送遥控命令接口
	//0:single 1:double    0:on 1:off
    void SendRemoteControl(int iControlType, int iPointNum, int iOpType, int iMessType);
	//发送定值指令
	void SendDevOrder(DEV_BASE &dveBaseInfo);
    //发送文件获取指令
    void SendRecordOrder(QList<LB_DATA> &lbInfo);
    //设置总召时间
    void SetGeneralTimes(int nGeneralTime,int nSyncTime,int nKwhTime);
    //手动总召
    void SetGeneralCall();
    //手动电度召唤
    void SetKwhCall();
    //死区设置
    void SetZoomArea(ZOOM_BASE &zoomBaseInfo);
	//设置定值区域
	void SetFixAreaCall(unsigned short uFixArea);
	//读取定值区域
	void ReadFixArea();
	//发送召唤目录请求
	void SendCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest);
	//发送写入激活请求
	void SendWriteActionRequest(FILE_ATTR_INFO &writeAction);
	//发送文件激活请求
	void SendReadActionRequest(FILE_ATTR_INFO &ReadAction);
signals:
	//socket创建成功
	void Signal_SocketConnectSuccess();
	//socket错误
	void Signal_SocketError(QString strError);
	//总召唤确认
	void Signal_AllCallRespond();
	//单点连续遥信数据
	void Signal_OnePointRemote(int iDeviceID, int nPointID, int nValue);
	//单点非连续遥信数据
	void Signal_onePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//双点连续遥信数据
	void Signal_DoublePointRemote(int iDeviceID, int nPointID, int nValue);
	//双点非连续遥信数据
	void Signal_DoublePointDisRemote(int iDeviceID, int nPointID, int nValue);
	//带3个字节短时标的单点遥信   SOE
	void Signal_BinarySignalPointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//短时标双点遥信
    void Signal_BinaryDoublePointShortTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);

	//SOE
    void Signal_SoeBinarySinglePointTime(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);
	//带长时标的双点信息
    void Signal_BinaryDoublePoint(int iDeviceID, int nPointID, int nValue, int nQuality, QString pDateTime);

	//累积量  电度  连续
	void Signal_KwhNormal(int iDeviceID, int nPointID, int nValue);
	//累计量  不连续
	void Signal_KwhDisNormal(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  连续
	void Signal_BinaryGroupPoint(int iDeviceID, int nPointID, int nValue);
	//成组单点遥信  非连续
	void Signal_BinaryGroupDisPoint(int iDeviceID, int nPointID, int nValue);

	//带短时标的累积量   电度
	void Signal_KwhShortTimeNormal(int iDeviceID, int nPointID, float nValue);
	//带长时标的累积量  电度
	void Signal_KwhLongTimeNormal(int iDeviceID, int nPointID, float nValue);

	//归一化遥测连续数据
	void Signal_AnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//归一化遥测不连续数据
	void Signal_ScaledAnalogNormal(int iDeviceID, int nPointID, float nValue, int nQuality);
	//浮点型遥测连续数据
	void Signal_AnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//浮点型遥测不连续数据
	void Signal_DisAnalogShortFloat(int iDeviceID, int nPointID, float nValue, int nQuality);
	//带短时标的归一化测量值
	void Signal_AnalogNormalShortTime(int iDeviceID, int nPointID, float nValue, int nQuality);
	//发送出去的数据
	void Signal_Write16Data(QByteArray buff);
	//接收到的数据报文
    void Signal_recv16Data(QByteArray btData, int nLenght);
	//遥控命令信号
    void Signal_Control(int iControlType, int iPointNum, int iOpType, int iMessType);
	//遥控反馈
	void Signal_ControlFeedBack(int iControlType, int iPointNum, int iOperateType, QString strStatus);
	//定值数据
	void Signal_DevReadBack(QMap<int, short> mapInfo);
	//发送定值数据接口
	void Signal_DevSendInfo(DEV_BASE &dveBaseInfo);
    //发送录波获取信号
    void Signal_RecordSendInfo(QList<LB_DATA> &lbInfo);
	//设定数据响应   48/136    6/7   1/0  
	void Signal_devWriteBack(int iControlType, int iCot, int iQos);
    //操作总召
    void Signal_GeneralCall();
    //召唤电度
    void Signal_KwhCall();
    //时间同步
    void Signal_SycsTime(QDateTime sycsTime);
    //复位进程
    void Signal_ResetProcess();
    //故障
    void Signal_MalFuction(int nBinaryType, int nBinaryAddr, int nBinaryValue, QString strTime, int nAnalogType, QMap<int, int>);
    //死区
    void Signal_ZoomArea(ZOOM_BASE &zoomBaseInfo);
    //死区反馈
    void Signal_ZoneFeedBack(int nAddr, float fValue, int qpm, int nCot);
	//切换定值区域
	void Signal_SwitchFixArea(unsigned short uFixArea);
	//读取定值区号
	void Signal_ReadFixArea();

	//发送读取文件目录服务
	void Signal_ReadCatalogRequest(FILE_CATALOG_REQUEST_1 &catalogRequest);

	//发送写入 激活 
	void SIGNAL_WriteActionRequest(FILE_ATTR_INFO &writeAction);
	//读取文件  激活
	void Signal_ReadActionRequest(FILE_ATTR_INFO &readAction);
	//定值读取
	void Signal_ReadFixData(DEV_BASE &);
	//文件目录
	void Signal_FIleCatalogINfo(QList<Catalog_Info>& lstFileInfo);

public slots:
    void Slot_ConnectSuccess(QString strLocalInfo);

private:
	//开启线程
	void run();
	
private:
	//ip
	QString m_strIP;
	//port
	int m_iPort;

	//本地信息
	QString m_LocalInfo;
    CFtuModule *m_pModule;
    //总召时间  分钟
    int m_nGeneranTime;
    //时间同步世家
    int m_nSyncGeneralTime;
    //电度召唤时间
    int m_nKwhCallTime;
};

#endif // COMMTHREAD_H
