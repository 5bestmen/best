#ifndef MODBUSTRANS_H
#define MODBUSTRANS_H

#include <QString>
#include <QObject>
#include "socketmgr.h"
#include "msg_queue.h"

#define MB_ERROR_CODE_ADDRESS       -1 //地址错误
#define MB_ERROR_CODE_FUNCODE     -2 //功能码错误
#define MB_ERROR_CODE_LENGTH        -3 //长度有错
#define MB_ERROR_CODE_CRC              -4 //CRC校验
#define MB_ERROR_CODE_EXCEPTION   -5 //异常信息

enum
{
	PER_FRAME_ANALOG = 120, //2bytes
	PER_FRAME_BINARY = 1920,//1bit
	PER_FRAME_YM = 60,//4bytes
};

enum
{
	MAX_ERRORS_TO_HALT = 10,    //ERROR达到该值，则重连
};

enum
{
	BIT_ADDRESS = 6,	//报文装置地址字节
	BIT_FUNCODE = 7,	//报文功能码字节
	BIT_DATA_LEN = 8,	//报文数据长度字节
	BIT_DATA_HEAD = 9,
};

enum TELETRCL_TYPES
{
	TELECTRL_REQUEST_SELECT = 0,//遥控预置请求
	TELECTRL_ACK_SELECT = 1,//遥控预置确认
	TELECTRL_REQUEST_EXECUTE = 2,//遥控执行请求
	TELECTRL_ACK_EXECUTE = 3,//遥控执行确认
	TELECTRL_REQUEST_UNSELECT = 4,//遥控撤销请求
	TELECTRL_ACK_UNSELECT = 5,//遥控撤销确认

	TELECTRL_REQUEST_GENERALCALL,//总召请求
	TELECTRL_ACK_GENERALCALL,//总召确认
	TELECTRL_ACK_GENERALCALL_OVER,//总召结束
	TELECTRL_REQUEST_SYNCTIME,//对时请求
	TELECTRL_ACK_SYNCTIME,//对时确认
	TELECTRL_REQUEST_KWHGENERALCALL,//电度总召请求
	TELECTRL_ACK_KWHGENERALCALL,//电度总召确认
};

class CModbusTrans:public QObject
{
	Q_OBJECT
public:
	CModbusTrans();
	CModbusTrans(CSocketMgr* pSocketMgr, CReadXML* pReadXML,int nDeviceID);
	~CModbusTrans();

public:
	//发送数据
	int SendData(int nDeviceID, int nDataType);
	//接收数据
	int RecvData(QByteArray bydata);

//	int HandleTrans();

	std::string FormatBuffer(char* pBuff, int nMsgLen);

	//处理数据
	void ProcessData(QByteArray RecvData);
	//解析遥信数据
	void ParseBinary(QByteArray RecvData);
	//解析遥测数据
	void ParseAnalog(QByteArray RecvData);
	//解析电度量
	void ParseKwh(QByteArray RecvData);
	//解析遥调返回报文
	void ParseYT(QByteArray RecvData);
	//解析遥控返回报文
	void ParseYK(QByteArray RecvData);
	//获取功能码
	int GetFuncode(int DeviceID, int nDataType);


public slots:
	//接收数据槽函数
    void Slot_RecvNewData(QByteArray bydata);
	//开始发送数据
	void Slot_ConnectScuess();
	//
	void SLot_SocketError(const QString &strError);


	// 转发遥控
	bool Slot_TransBinary(int nStationID, int nDeviceID, int DiID, int nValue);

	// 转发遥调
	bool Slot_TransAnalog(int nStationID, int nDeviceID, int AiID, int ncrtValue);


signals:
	//Signal DI数据更新
	void Signal_ShowDiValue(int StationID, int DeviceID, int DiID, int realData, QString DiName);
	//AI数据更新
 	void Signal_ShowAiValue(int StationID, int DeviceID, int AiID, int realData, QString AiName);
	//Kwh数据更新
	void Signal_ShowKwhValue(int StationID, int DeviceID, int KwhID, int realData, QString KwhName);
	//设备连接状态更新
	void Signal_ShowDeviceState(int DeviceID, QString DeviceName);
	//连接成功
	void Signal_ConnectSuccess(int nDevice,const QString &strDevName);
	//socket错误
	void Signal_SocketError(int nDevice, const QString &strDevName,const QString &strError);


	//功能码
private:
	unsigned char m_funCode;

public:
	//发送遥控遥调命令
	bool SendTeleCmd(int nDeviceID, int nDataID, int nDataType, float fDataValue);

	//处理遥控转发信息
	int HandleYKTransInfo(STransInfo& transInfo, int nValue);

	//处理遥调转发信息
	int HandleYTTransInfo(STransInfo& transInfo, int nValue);
	
	unsigned short CRC16(unsigned char* buf, unsigned long len);

private:
	//时间同步
	bool SyncTime();

private:
	//数据发送和接收缓冲
    #define BUFFER_SIZE 4096
 	unsigned char m_recv_buf[BUFFER_SIZE + 1];
 	unsigned char m_recv_len;
	unsigned char m_send_buf[BUFFER_SIZE + 1];
	unsigned char m_send_len;

	CReadXML* m_pReadXML;
	CSocketMgr* m_pSocketMgr;
	int m_nDeviceID;
	int m_nDataType;

};


#endif // MODBUSTRANS_H