#ifndef _SCADA_COMMON_H
#define _SCADA_COMMON_H
#include "stdio.h"
#include "datatypes.h"
#include "variant.h"
#include <string>
#include <memory.h>
using namespace std;

#include <time.h>  

typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位
typedef unsigned char   BIT08;  //08位

#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))

////////////////////////// AI类型参数表SCD_AI_TYPE 的AI类型ID定义////////////////////////////////

//遥测类型编码定义
enum
{
	AI_I = 1,//电流
	AI_Ia = 2,//A相电流
	AI_Ib = 3,//B相电流
	AI_Ic = 4,//C相电流
	AI_Iab = 5, //AB相电流
	AI_Ibc = 6, //BC相电流
	AI_Ica = 7, //CA相电流
	AI_I0 = 8,//零序电流
	AI_I1 = 9, //正序电流
	AI_I2 = 10, //负序电流
	AI_P = 20,	//有功功率
	AI_Pa = 21,  //A相有功
	AI_Pb = 22,  //B相有功
	AI_Pc = 23,  //C相有功
	AI_Pab = 24,  //AB相有功
	AI_Pbc = 25,  //BC相有功
	AI_Pca = 26,  //CA相有功
	AI_Q = 40,	//无功功率
	AI_Qa = 41,  //A相无功
	AI_Qb = 42,  //B相无功
	AI_Qc = 43,  //C相无功
	AI_Qab = 44,  //AB相无功
	AI_Qbc = 45,  //BC相无功
	AI_Qca = 46,  //CA相无功
	AI_S = 60,	//视在功率
	AI_COSYC = 61,	//力率（功率因数）
	AI_ANGLE = 100,	//角度
	AI_HZ = 101,	//频率
	AI_TEMPERATURE = 102,	//温度
	AI_PRESS = 103,	//压力
	AI_WATER = 104,	//水位
	AI_FLOW = 105,	//流量	 
	AI_HUMIDITY = 106,	//湿度
	AI_DW = 107,	//变压器档位
	AI_YC = 127,	//其他遥测
	AI_V = 150,	//电压
	AI_Va = 151,	//A相电压
	AI_Vb = 152,	//B相电压
	AI_Vc = 153,	//C相电压
	AI_Vab = 154,	//AB线电压
	AI_Vbc = 155,	//BC线电压
	AI_Vca = 156,	//CA线电压
	AI_V0 = 157,	//零序电压
	AI_V1 = 158, //正序电压
	AI_V2 = 159, //负序电压
};

//遥信类型编码定义
enum
{
	DI_BREAK = 0,	//开关
	DI_BRK_CAR = 301,//小车开关
	DI_BRK_BUSLKCAR = 302, //母联小车开关
	DI_BRK_BUSLINK = 303, //母联开关
	DI_BRK_BYBUS = 304, //旁母开关
	DI_BRK_BYBUSLK = 305, //旁母兼母联开关
	DI_STATUS = 400,	//状态
	DI_SWITCH = 500,	//刀闸:不区分使用类型
	DI_SW_LINE = 501,	//线路隔离刀
	DI_SW_B1 = 502,	//I母隔离刀
	DI_SW_B2 = 503,	//II母隔离刀
	DI_SW_B3 = 504,	//III母隔离刀
	DI_SW_B4 = 505,	//IV 母隔离刀
	DI_SW_BYB = 506, //旁母隔离刀
	DI_SW_TRAN = 507, //主变刀闸  
	DI_SW_STRAN = 508, //站变刀闸
	DI_SW_PT = 509,	//PT刀闸
	DI_SW_BLINK = 510,	//母联刀闸
	DI_SW_L_GRD = 541,	//线路接地刀
	DI_SW_TRAN_GRD = 542,	//主变接地刀
	DI_SW_STRAN_GRD = 543,	//站变接地刀
	DI_SW_PT_GRD = 544,	//PT接地刀
	DI_SW_B1_GRD = 571,	//I母接地刀
	DI_SW_B2_GRD = 572,	//II母接地刀
	DI_SW_B3_GRD = 573,	//III母接地刀
	DI_SW_B4_GRD = 574,	//IV 母接地刀
	DI_SW_BYB_GRD = 575, // 旁母接地刀
	DI_PROTSIG = 600,	//保护
	DI_SGZ = 601,	//事故总
	DI_YG = 602,	//预告信号

	//配网故障类型编码定义
	DI_DIAG_SGZ = 4001, //配网事故总信号/有故障
	DI_DIAG_NOFAULT = 4002, //无故障
	DI_DIAG_INST = 4003, //瞬间故障
	DI_DIAG_UNKNOWN_FAULT = 4004, //故障类型不明确
	DI_DIAG_FAULT = 4005, //有故障
	DI_DIAG_S_GND = 4011, //单相接地
	DI_DIAG_S_GND_INST = 4012, //单相瞬时接地
	DI_DIAG_A_GND = 4013, //A相接地
	DI_DIAG_B_GND = 4014, //B相接地
	DI_DIAG_C_GND = 4015, //C相接地
	DI_DIAG_D_GND = 4016, //相间接地
	DI_DIAG_D_GND_INST = 4017, //相间瞬时接地
	DI_DIAG_AB_GND = 4018, //AB相间接地
	DI_DIAG_BC_GND = 4019, //BC相间接地
	DI_DIAG_CA_GND = 4020, //CA相间接地
	DI_DIAG_T_GND = 4021, //ABC相间接地
	DI_DIAG_D_SHORT = 4031, //相间短路
	DI_DIAG_D_SHORT_INST = 4032, //相间瞬时短路
	DI_DIAG_AB_SHORT = 4033, //AB相间短路
	DI_DIAG_BC_SHORT = 4034, //BC相间短路
	DI_DIAG_CA_SHORT = 4035, //CA相间短路
	DI_DIAG_T_SHORT = 4036, //ABC三相短路
	DI_DIAG_T_SHORT_INST = 4037, //相间瞬时短路

	//FTU线路故障类型编码定义
	DI_REP_SGZ = 4101, //配网事故总信号/有故障
	DI_REP_NOFAULT = 4102, //无故障
	DI_REP_INST = 4103, //瞬间故障
	DI_REP_UNKNOWN_FAULT = 4104, //故障类型不明确
	DI_REP_FAULT = 4105, //有故障
	DI_REP_S_GND = 4111, //单相接地
	DI_REP_S_GND_INST = 4112, //单相瞬时接地
	DI_REP_A_GND = 4113, //A相接地
	DI_REP_B_GND = 4114, //B相接地
	DI_REP_C_GND = 4115, //C相接地
	DI_REP_S_SHORT = 4121, //单相短路
	DI_REP_A_SHORT = 4122, //A相短路
	DI_REP_B_SHORT = 4123, //B相短路
	DI_REP_C_SHORT = 4124, //C相短路
	DI_REP_N_SHORT = 4125, //零相短路（限值）
	DI_REP_D_SHORT = 4126, //相间短路
	DI_REP_D_SHORT_INST = 4127, //相间瞬时短路
	DI_REP_AB_SHORT = 4128, //AB相间短路
	DI_REP_BC_SHORT = 4129, //BC相间短路
	DI_REP_CA_SHORT = 4130, //CA相间短路
	DI_REP_T_SHORT = 4131, //三相间短路
	DI_REP_T_SHORT_INST = 4132, //三相间瞬时短路
	DI_REP_S_OVER = 4141, //单相过流
	DI_REP_S_OVER_INST = 4142, //单相瞬时过流
	DI_REP_A_OVER = 4143, //A相过流
	DI_REP_B_OVER = 4144, //B相过流
	DI_REP_C_OVER = 4145, //C相过流
	DI_REP_N_OVER = 4146, //零相过流
	DI_REP_D_OVER = 4147, //相间过流
	DI_REP_D_OVER_INST = 4148, //相间瞬时过流
	DI_REP_AB_OVER = 4149, //AB相间过流
	DI_REP_BC_OVER = 4150, //BC相间过流
	DI_REP_CA_OVER = 4151, //CA相间过流
};

//电度类型编码定义
enum
{
	ED_PKWH = 200,	//有功电度
	ED_QKWH = 201,	//无功电度
	ED_PosPKWH = 202,	//正有功电度
	ED_NegPKWH = 203,	//负有功电度
	ED_PosQKWH = 204,	//正无功电度
	ED_NegQKWH = 205,	//负无功电度
	ED_RATE = 206, //其它累积量
};

//越限处理级别
enum
{
	HANDLE_LEVEL_NONE,
	HANDLE_LEVEL_1, //1,2,3级均看作1级越限
	HANDLE_LEVEL_2,//1级为1级，2，3级看作2级
	HANDLE_LEVEL_3,//1为1级，2为2级，3为3级
};

/*
//越限级别
enum
{
		OFF_LIMIT_RESTORE,//复限
		OFF_LIMIT_L1,
		OFF_LIMIT_L2,
		OFF_LIMIT_L3,
		OFF_LIMIT_H1,
		OFF_LIMIT_H2,
		OFF_LIMIT_H3,
};
*/
enum
{
	DATATYPE_ANALOG,
	DATATYPE_BINARY,
	DATATYPE_KWH,
	DATATYPE_DEVICE,
	DATATYPE_CHANNEL,
	DATATYPE_GROUP,
	DATATYPE_LAST,
};

//AI最值统计模式
enum
{
	MOST_VALUE_NATURAL = 0, //自然数
	MOST_VALUE_ABSOLUTE = 1, //绝对值
};


//判事故方式
enum
{
	FAULTBY_FAULTGENERAL,                                       //事故总
	FAULTBY_FAULTGENERAL_AND_PROTECTSIGNAL,//事故总和保护信号
	FAULTBY_FAULTGENERAL_OR_PROTECTSIGNAL,   //事故总或保护信号
	FAULTBY_FAULTGENERAL_AND_ZEROANALOG,    //事故总和遥测归零
	FAULTBY_LOGICPROC,                  //逻辑过程判断
	FAULTBY_NONE = 127,                                              //不判事故
};


//事项分类
enum
{
	EVENT_CLASS_NONE = 0,
	EVENT_CLASS_SCADA = 1,     //SCADA事项
	EVENT_CLASS_OPERATION = 2,//操作事项
	EVENT_CLASS_SYSTEM = 3,
	EVENT_CLASS_PROJECT = 4,//工程事项
};

//事项类型
enum
{
	//SCADA事项        
	//越限
	EVENT_TYPE_OFFLIMIT_RESTORE = 0,//复限
	EVENT_TYPE_OFFLIMIT_L1 = 1,
	EVENT_TYPE_OFFLIMIT_L2 = 2,
	EVENT_TYPE_OFFLIMIT_L3 = 3,
	EVENT_TYPE_OFFLIMIT_H1 = 4,
	EVENT_TYPE_OFFLIMIT_H2 = 5,
	EVENT_TYPE_OFFLIMIT_H3 = 6,

	EVENT_TYPE_ANALOG_DEADVALUE = 99,//遥测死数

	//变位
	EVENT_TYPE_NORMAL_BREAK = 100,//开关正常变位
	EVENT_TYPE_FAULT_BREAK = 101,//开关事故变位
	EVENT_TYPE_SWITCH_BREAK = 102,//刀闸正常变位
	EVENT_TYPE_PROTECTSIGNAL_BREAK = 103,//保护信号变位
	EVENT_TYPE_DEFAULT_BREAK = 104,//未定义的变位

	//SOE
	EVENT_TYPE_STATUS_SOE = 110,//状态量SOE
	EVENT_TYPE_BREAK_SOE = 111,//开关SOE
	EVENT_TYPE_SWITCH_SOE = 112,//刀闸SOE
	EVENT_TYPE_PROTECTSIGNAL_SOE = 113,//保护信号SOE

	//操作事项
	//遥控
	EVENT_TYPE_TELECTRL = 200,
	//顺控
	EVENT_TYPE_SEQCTRL = 201,
	EVENT_TYPE_MANSET = 210,
	//遥调
	EVENT_TYPE_TELE_ANALOG = 226,

	//工程事项
	EVENT_TYPE_DI_FAULTBY_ERROR = 400,//数据配置错误
};


//开关，事故总，保护信号分合定义
enum
{
	BREAK_OFF = 0, //开关正常状态分
	BREAK_ON = 1, //开关正常状态合
	BREAK_TRIP = 2, //开关跳闸
};

enum {
	DEVICE_CONNECTED,      //设备登录
	DEVICE_DISCONNECTED,   //设备掉线
	DEVICE_ABNORMAL,       //设备异常
};

enum //NBM_MSG_TYPES
{
	NBMT_OTHER,

	//前置发往SCADA
	NBMT_FES_EVENT,            //前置往SCADA发事项
	NBMT_FES_ANALOG = 6,   //前置往SCADA发模拟量
	NBMT_FES_BINARY = 7,     //前置往SCADA发开关量
	NBMT_FES_KWH = 8,         //前置往SCADA发电镀量

	//SCADA发往后台
	NBMT_SCADA_EVENT,      //SCADA往后台发事项

	//操作事项
	NBMT_OPERATION_EVENT,//后台操作事项

	//图形发往SCADA
	NBMT_TELECTRL_GRAPH2SCADA, //图形往SCADA发遥控
	NBMT_TELECTRL_SCADA2GRAPH, //SCADA往图形发遥控返校

	NBMT_SEQCTRL,//顺控命令

	//SCADA发往前置
	NBMT_TELECTRL_SCADA2FES, //SCADA往前置发遥控
	NBMT_TELECTRL_FES2SCADA, //SCADA往前置发遥控

	//SCADA发往远动服务
	NBMT_TELECTRL_SCADA2RMT, //SCADA发往远动服务
	NBMT_TELECTRL_RMT2SCADA, //远动服务发往SCADA
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
	TELECTRL_REQUEST_KWHGENERALCALL,//电镀总召请求
	TELECTRL_ACK_KWHGENERALCALL,//电镀总召确认
};

//CP56Time2a时间结构
struct CP56Time2a
{
	unsigned char msL;     //毫秒低
	unsigned char msH;     //毫秒高
	unsigned char minute;  //分钟(0-59)
	unsigned char hour;    //小时(0-23)
	unsigned char day;     //日期 0-4位: 日  5-7位:星期
	unsigned char month;   //月份(1-12)
	unsigned char year;    //年份(0-99)
public:
	string Dump()
	{
		int nMilliSec = MYMAKE16(msL, msH);
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d %d", year + 2000, month, day, hour, minute, nMilliSec / 1000, nMilliSec % 1000);
		return buf;
	}
	void Initialize(int cyear, int cmonth, int cday, int chour, int cminute, int csecond, int cmillisec)
	{
		unsigned short ms = csecond * 1000 + cmillisec / 1000;
		msL = MYLO8(ms);
		msH = MYHI8(ms);
		minute = cminute & 0x3f;
		hour = chour & 0x1f;
		day = cday & 0x1f;
		month = cmonth & 0x0f;
		year = (cyear - 2000) & 0x7f;
	}
};

struct CP32Time2a
{
	unsigned char msL;    //毫秒L
	unsigned char msH;    //毫秒H
	unsigned char minute;    //分
	unsigned char hour;   //小时
public:
	string Dump()
	{
		int nMilliSec = MYMAKE16(msL, msH);
		char buf[128];
		sprintf(buf, "%.2d:%.2d:%.2d %d", hour, minute, nMilliSec / 1000, nMilliSec % 1000);
		return buf;
	}
	void Initialize(int chour, int cminute, int csecond, int cmillisec)
	{
		unsigned short ms = csecond * 1000 + cmillisec / 1000;
		msL = MYLO8(ms);
		msH = MYHI8(ms);
		minute = cminute & 0x3f;
		hour = chour & 0x1f;
	}
};

//三个八位位组二进制时间
struct CP24Time2a
{
	unsigned char msL;     //毫秒低
	unsigned char msH;     //毫秒高
	unsigned char minute;  //分钟(0-59)
public:
	string Dump()
	{
		int nMilliSec = MYMAKE16(msL, msH);
		char buf[128];
		sprintf(buf, "%.2d %d", minute, nMilliSec);
		return buf;
	}
	void Initialize(int cminute, int csecond, int cmillisec)
	{
		unsigned short ms = csecond * 1000 + cmillisec / 1000;
		msL = MYLO8(ms);
		msH = MYHI8(ms);
		minute = cminute & 0x3f;
	}
};

//netbus消息头
struct NBM_BASE
{
public:
	NBM_BASE()
	{
		m_nMsgType = 0;
		m_nMsgLen = 0;
	}
public:
	string Dump() { return ""; }
public:
	int m_nMsgType;
	int m_nMsgLen;
};

//SCADA事项
struct NBM_SCADA_EVENT : public NBM_BASE
{
public:
	NBM_SCADA_EVENT()
	{
		memset(m_strGraphName, 0, MAX_PARAM_NAME_LEN);
		memset(m_strVoiceDesc, 0, MAX_DESCRIPTION_LEN);
		memset(m_strTextDesc, 0, MAX_DESCRIPTION_LEN);
		memset(m_strUserName, 0, MAX_PARAM_NAME_LEN);
		memset(m_strNodeName, 0, MAX_PARAM_NAME_LEN);

		m_nStationID = -1;
		m_nEquipmentID = -1;
		m_nDataID = -1;
		m_nSecurityLevel = -1;
		m_nVoltageGrade = -1;
		m_nDataType = -1;
	}

	enum
	{
		MAX_PARAM_NAME_LEN = 64,
		MAX_DESCRIPTION_LEN = 128,
	};
public:
	int m_nGroupID;
public:
	int m_nClassify;//分类
	int m_nType;    //类型
public:
	int m_nStationID;
	int m_nEquipmentID;//一次设备ID
	int m_nDataID;
	int m_nSecurityLevel;					//安全级
	int m_nVoltageGrade;					//电压等级
public:
	int m_nDataType;//模拟量,开关量,电度量
	float m_fValue;    //点值

public:
	int m_nCtrlType;//遥控类型

public:
	int m_nAlarmMethod;//告警方式

public:
	char m_strGraphName[MAX_PARAM_NAME_LEN];//推图名
	char m_strVoiceDesc[MAX_DESCRIPTION_LEN];	 //语音描述信息
	char m_strTextDesc[MAX_DESCRIPTION_LEN];	 //文字描述信息

public:
	char m_strUserName[MAX_PARAM_NAME_LEN];  //用户名
	char m_strNodeName[MAX_PARAM_NAME_LEN];//节点名

public:
	CP56Time2a m_cpTime;
	string Dump()
	{
		char buf[1024];

		sprintf(buf, "MsgType:%d,Classify:%d Type:%d StationID:%d EquipmentID:%d DataID:%d DataType:%d fValue:%f CtrlType:%d GraphName:%s VoiceDesc:%s TextDesc:%s UserName:%s NodeName:%s time:%s",
			m_nMsgType, m_nClassify, m_nType, m_nStationID, m_nEquipmentID, m_nDataID, m_nDataType, m_fValue, m_nCtrlType,
			m_strGraphName, m_strVoiceDesc, m_strTextDesc, m_strUserName, m_strNodeName, (char*)m_cpTime.Dump().c_str());

		return buf;
	}
};

//遥控命令
struct NBM_TELECTRL : public NBM_BASE
{
public:

	int32u DoutOccNo;
	int32u NodeOccNo;
	int8u  IddType;
	int8u  Att;
	int8u  Datatype;
	int8u  Source1;
	IO_VARIANT	Value[2];


	bool m_bCommandFinished;       //遥控命令全部自动完成，完成之后命令置空，可以进行下一步指令

	int m_nStationID;
	int m_nDeviceID;//二次设备ID
	int m_nEquipmenID;//一次设备ID
	int m_nDataID;
	int m_nRelateDataID;

public:
	int m_nUserID;
	int m_nNodeID;

public:
	int m_nCommandType;//遥控命令类型,45,46,48,49,50?

public:
	int m_nDataType;//模拟量?开关量?
	float m_fValue;    //点值

public:
	int m_nCtrlType; // 遥控类型(总召，总召确认，对时,对时确认，预置，预置确认，执行，执行确认，撤销，撤销确认....)

public:
	string Dump()
	{
		time_t timer;
		struct tm *tblock;
		timer = time(NULL);
		tblock = localtime(&timer);
		printf("Local time is: %s", asctime(tblock));

		char buf[256];

		sprintf(buf, "[%s] Type:%d Length:%d StationID:%d DeviceID:%d DataID:%d UserID:%d NodeID:%d DataType:%d fValue:%f CtrlType:%d m_nEquipmenID:%d m_nDeviceID:%d",
			asctime(tblock), m_nMsgType, m_nMsgLen, m_nStationID, m_nDeviceID, m_nDataID, m_nUserID, m_nNodeID, m_nDataType, m_fValue, m_nCtrlType, m_nEquipmenID, m_nDeviceID);

		return buf;
	}
};


enum
{
	SEQCTRL_TYPE_GRAPH,
	SEQCTRL_TYPE_TIMER,
	SEQCTRL_TYPE_BREAK,
	SEQCTRL_TYPE_LOG,//lua脚本中输出文本事项
};


//顺控命令
struct NBM_SEQCTRL : public NBM_BASE
{
public:
	int m_nSeqctrlType;//图形，变位，定时器
public:
	int m_nUserID;//用户ID
public:
	int m_nStationID;
	int m_nDeviceID;//二次设备ID
	int m_nEquipmenID;//一次设备ID
	int m_nDataID;

public:
	enum { MAX_LUA_NAME_LEN = 128, };
	char m_strLuaName[MAX_LUA_NAME_LEN];  //脚本名

public:
	CP56Time2a m_cpTime;
public:
	string Dump()
	{
		char buf[1024];

		sprintf(buf, "Type:%d Length:%d StationID:%d DeviceID:%d DataID:%d UserID:%d LuaName:%s Time:%s",
			m_nMsgType, m_nMsgLen, m_nStationID, m_nDeviceID, m_nDataID, m_nUserID, m_strLuaName, (char*)m_cpTime.Dump().c_str());

		return buf;
	}
};

#ifdef DDIALOGS_LIBRARY
#include <QMetaType>
Q_DECLARE_METATYPE(NBM_TELECTRL)
#endif //DDIALOGS_LIBRARY

#endif //_SCADA_COMMON_H
