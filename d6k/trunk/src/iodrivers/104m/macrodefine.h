

#ifndef _MACRO_DEFINE_H
#define _MACRO_DEFINE_H

enum IO_DEFINE
{
	UNDEFINED = 0,
	DEFINED = 1
};

//驱动类型
enum
{
	/*	DRIVER_TYPE_EXE,
		DRIVER_TYPE_DLL,
		DRIVER_TYPE_UNKNOWN,*/
};


enum
{
	MAX_GROUPNAME_LENGTH = 50,        //!<Group名称长度
	MAX_CHANNELNAME_LENGTH = 50,      //!<Channel名称长度
	MAX_DEVICENAME_LENGTH = 20,       //!<Device名称长度
	MAX_DRIVERNAME_LENGTH = 20,     //!<驱动名称长度
	MAX_IPADDRESS_LENGTH = 20,      //!<IP地址长度
	MAX_POINTNAME_LENGTH = 120,     //!<点名长度
};


//////////////////////////////////////////////////////////////////////////
//通道状态
enum
{
	CHANNEL_STATE_UNKNOWN,	         //!<初始状态
	CHANNEL_STATE_RUN,		         //!<运行态
	CHANNEL_STATE_STOP,              //!<停运态
};

enum
{
	MAX_CHANNEL_TELECMD = 10,          //!<通道遥控命令队列长度
};

//驱动状态
enum
{
	/*   DRIVER_STATE_UNKNOWN,	         //!<初始状态
	   DRIVER_STATE_RUN,		         //!<运行态
	   DRIVER_STATE_UNLOAD,	         //!<卸载驱动(内核设置通道之内核状态为UNLOAD后，驱动检测到该设置，则自身退出)*/
};

//驱动线程状态
enum
{
	DRIVER_THREAD_STATE_UNKNOWN,	  //!<初始状态
	DRIVER_THREAD_STATE_RUN,		  //!<运行态
	DRIVER_THREAD_STATE_UNLOAD,	      //!<卸载驱动(内核设置通道之内核状态为UNLOAD后，驱动检测到该设置，则自身退出)
};


//Trace消息子类型
enum TRACE_SUB_TYPES
{
	TRACE_INFO = 0,
	TRACE_ERROR,
	TRACE_WARNING,
	TRACE_DATA,
	TRACE_LOG,                        //!<写log时，写一条trace
	TRACE_DRIVER,                     //!<驱动启动状态信息
	TRACE_TELECTRL,                   //!<遥控trace信息
};

//SOE消息子类型
enum SOE_SUB_TYPES
{
	SOE_COS = 0,
	SOE_SOE,
};

//Log消息子类型
enum LOG_SUB_TYPES
{
	LOG_INFO = 1000,
};


//Alarm消息子类型
enum ALARM_SUB_TYPES
{
	ALARM_ANALOG = 3000,
	ALARM_BINARY,
};


enum
{
	MAX_DRIVER_PER_FES = 100,          //!<最多支持的运行驱动的数目  
	MAX_THREAD_PER_DRIVER = 10,        //!<每个驱动最多10个线程
};


enum
{
	VALUE_SET_OK,
	VALUE_SET_FAILED,
	VALUE_CHANGED,
	VALUE_INVALID,
};

//通道通讯类型
enum
{
	MEDIUM_ETHERNET,                 //以太网
	MEDIUM_SERIALPORT,               //串口
};

enum
{
	//数据库文档定义
	QUALITY_NORMAL = 0,           //!<正常
	QUALITY_MANUAL = 1,           //!<置入
	QUALITY_ILLEGAL = 1 << 1,     //!<非法
	QUALITY_DEAD = 1 << 2,        //!<死数
	QUALITY_UNCERTAIN = 1 << 3,   //!<不可靠
	QUALITY_BLOCK = 1 << 4,       //!<封锁
	QUALITY_DOUBT = 1 << 5,       //!<可疑
	QUALITY_UNINIT = 1 << 7,      //!<未初始化
	QUALITY_CALCDATA = 1 << 8,    //!<计算量
	QUALITY_UNREASONABLE = 1 << 9,//!<不合理
	QUALITY_INTERRUPT = 1 << 10,  //!<采集中断
	QUALITY_REPLACE = 1 << 11,    //!<替代
	QUALITY_OVERFLOW = 1 << 12,   //!<溢出
	QUALITY_BLOCKED = 1 << 13,    //!<被闭锁
	QUALITY_REPLACED = 1 << 14,   //!<被取代
	QUALITY_UNCURRENT = 1 << 15,  //!<非当前值
	QUALITY_INVALID = 1 << 16,    //!<无效
};

#endif //_MACRO_DEFINE_H
