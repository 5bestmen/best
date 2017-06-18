/********************************************************************************
模块名       :  通信插件接口类
文件名       :  icommplugin.h
文件实现功能 :  提供通信接口
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef ICOMMPLUGIN_H
#define ICOMMPLUGIN_H
#include <QObject>
#include <QTextEdit>
#include <QDateTime>
#include <QMap>
#include "asdu_info.h"

class ICommPlugin : public QObject
{
    Q_OBJECT
public:
    ICommPlugin()
    {

    }
    virtual ~ICommPlugin()
    {

    }
	virtual void StartRun(const QString &strIP, int iPort) = 0;

	virtual void StopRun() = 0;

    virtual bool WriteApduData(const char *pSendData, int iLength)=0;

signals:
    //socket创建成功
    void Signal_SocketConnectSuccess(QString strLocalInfo);
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
	//遥控反馈
	void Signal_ControlFeedBack(int iControlType, int iPointNum, int iOperateType, QString strStatus);
	//定值数据
	void Signal_DevReadBack(QMap<int, short> mapInfo);
	//设定数据响应   48/136    6/7   1/0  
	void Signal_devWriteBack(int iControlType, int iCot, int iQos);
    //故障
    void Signal_MalFuction(int nBinaryType, int nBinaryAddr, int nBinaryValue, QString strTime, int nAnalogType, QMap<int, int>);
    //死区
    void Signal_ZoneFeedBack(int nAddr, float fValue, int qpm, int nCot);
	//定值
	void Signal_ReadFixData(DEV_BASE &devData);

};



#endif // ICOMMPLUGIN_H
