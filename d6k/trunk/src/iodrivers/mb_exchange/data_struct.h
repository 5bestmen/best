/*! @file
<PRE>
********************************************************************************
模块名       :  数据结构定义
文件名       :
文件实现功能 :
作者         :  HuangYuqing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容
2016.9.12    v1.1     HuangYuQing        增加数据转发结构
********************************************************************************
</PRE>
*  @brief
*  @author  HuangYuqing
*  @version 1.0
*  @date    2016.8.18
*/
#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <QString>
#include <QVector>

#define MAX_STATIONNAME_LENGTH 256
#define MAX_CHANNELNAME_LENGTH MAX_STATIONNAME_LENGTH

#define CONFIG_NAME "mb_exchange.xml"
#define NEED_EXCHANGE_FLAG 1

#define BEGIN_FLAG_LEFT        '['
#define END_FLAG_RIGHT         ']'
#define MORE_FLAG			   '|'
#define STATE_FLAG			   ':'
#define MORE_MSG			   ','

#define TRANSINFO_LEN			512

#define T_MAX_POINT_INFO_SIZE				4096

#define T_MAX_KEY＿LEN						32

typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位
typedef unsigned char   BIT08;  //08位

#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))



enum DATATYPE
{
	DATATYPE_BINARY = 1, 
	DATATYPE_ANALOG = 2,
	DATATYPE_KWH = 3,
};

enum TRANSTYPE
{
	TRANSTYPE_BINARY = 1,
	TRANSTYPE_ANALOG = 2,
};

//厂站信息
struct SStationInfo
{
	int Station_id;
	QString Station_name;
};
//设备信息
struct SDeviceInfo
{
	int Device_id;
	QString Device_name;
	int Device_address;
	QString Protocol;
	QString strIP;
	int Port;
	int BinaryStartAddr;
	int AnalogStartAddr;
	int KwhStartAddr;
	int BinaryTelectrlStartAddr;
	int AnalogTelectrlStartAddr;
	int FuncodeBinary;
	int FuncodeAnalog;
	int FuncodeKwh;
	int FuncodeTelectrlBinary;
	int FuncodeTelectrlAnalog;
};

struct STargetInfo
{
	STargetInfo()
	{
		Target_station_id = 0;
		Targert_device_id = 0;
		Target_id = 0;
//		Target_YKnumber = 0;

	}
	int Target_station_id;
	int Targert_device_id;
	int Target_id;
//	int Target_YKnumber;
};

struct SConditionInfo
{
	SConditionInfo()
	{
		Condition_station_id = 0;
		Condition_device_id = 0;
		Condition_id = 0;
		Condition_flag = 0;
	}
	int Condition_station_id;
	int Condition_device_id;
	int Condition_id;
	int Condition_flag;
};

struct STransInfo
{
// 	//遥控号
// 	int Trans_YKnumber;
	//目标点
	QVector<STargetInfo> s_vecTarget;
	//条件点
	QVector<SConditionInfo> s_vecCondition;
};

//AI数据
struct SAiInfo
{
	int nAiID;
	int nAddr;
	QString strAiName;
	int nDataType;
	//转发类型区分发送遥控还是遥调 1是遥控，2是遥调
	int nTransType;
	STransInfo transInfo;

	SAiInfo()
	{
		nAiID = 0;
		nAddr = 0;
		nTransType = 0;
		strAiName.clear();
		nDataType = DATATYPE_ANALOG;
	}
};
//DI数据
struct SDiInfo
{
	int nDiID;
	int nAddr;
	QString strDiName;
	int nDataType;
	//转发类型区分发送遥控还是遥调 1是遥控，2是遥调
	int nTransType;
	STransInfo transInfo;

	SDiInfo()
	{
		nDiID = 0;
		nAddr = 0;
		nTransType = 0;
		strDiName.clear();
		nDataType = DATATYPE_BINARY;
	}
};
//Kwh数据
struct SKwhInfo
{
	int nKwhID;
	int nAddr;
	QString strKwhName;
	int nDataType;

	SKwhInfo()
	{
		nKwhID = 0;
		nAddr = 0;
		strKwhName.clear();
		nDataType = DATATYPE_KWH;
	}
};

struct SPoint
{
	int nStationID;
	int nDeviceID;
	int nPointID;
	short nValue;
	int nMsgType;
	SPoint()
	{
		nStationID = 1;
		nDeviceID = 0;
		nPointID = 0;
		nValue = 0;
		nMsgType = -1;
	}
}; 



#endif // DATA_STRUCT_H