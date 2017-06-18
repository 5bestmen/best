

#ifndef _ASDU_BASE_H
#define _ASDU_BASE_H

#include "ace/Basic_Types.h"
#include "scada_common.h"


class ACE_InputCDR;
class ACE_OutputCDR;


enum
{
	STARTCHAR = 0x68,
};

enum
{
	TYPE_I = 1,
	TYPE_S = 2,
	TYPE_U = 4,
	TYPE_N = 8,          //序列号类型(辅助)
	TYPE_T = 16,        //定时器类型(辅助)
	TYPE_ERROR = 32,    //错误信息
	TYPE_K = 64,          //K值信息
	TYPE_W = 128,         //W值信息
};

enum
{
	TESTFR_CON = 0x83,
	TESTFR_ACT = 0x43,
	STOPDT_CON = 0x23,
	STOPDT_ACT = 0x13,
	STARTDT_CON = 0x0b,
	STARTDT_ACT = 0x07,
};


enum
{
	TIMER_ID_T1 = 1001,   //TIMER_INTERVAL_T1=15, //发送激活类U帧，等待确认的超时
	TIMER_ID_T2 = 1002,   //TIMER_INTERVAL_T2=10,//接收到I帧后，w值小于DEFAULT_W，不满足回确认W条件，但T2到时，则回确认
	TIMER_ID_T3 = 1003,   //TIMER_INTERVAL_T3=20,//长期空闲t3>t1状态下发送测试帧的超时
};


//端类型
enum
{
	PEER_MONITOR,         //监视方向,装置 M
	PEER_CONTROL,         //控制方向,后台 C
};

//回调显示文本输出时，消息的分类(Message Callback)
enum
{
	MCB_TIMER,
	MCB_SEND,
	MCB_RECV,
	MCB_ERROR,
};


enum
{
	SEND_ERROR_K,          //发送失败，k > DEFAULT_K
	SEND_ERROR_SOCKET,     //发送失败，socket错误
	SEND_OK,               //发送成功
};

enum
{
	ERROR_NONE,            //无错误
	ERROR_SERIALNUMBER,    //序列号错误
};


typedef unsigned short  BIT16;  //16位
typedef unsigned long   BIT32;  //32位
typedef unsigned char   BIT08;  //08位

#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))



#pragma pack(push, 1)


//////////////////////////////////////////////////////////////////////////
//1字节表示的传输原因
class COT1
{
public:
	unsigned char m_cot;
public:
	void SetCot(unsigned char cot);
	unsigned char   GetCot();
};
int operator >> (ACE_InputCDR &cdr, COT1 &cot1);
int operator<< (ACE_OutputCDR &cdr, const COT1 &cot1);
//2字节表示的传输原因
class COT2
{
public:
	unsigned char m_cot0;
	unsigned char m_cot1;
public:
	void SetCot(unsigned short cot);
	unsigned short   GetCot();
};
int operator >> (ACE_InputCDR &cdr, COT2 &cot2);
int operator<< (ACE_OutputCDR &cdr, const COT2 &cot2);

//////////////////////////////////////////////////////////////////////////
//1字节表示的应用服务数据单元公共地址
class ASDUADDR1
{
public:
	unsigned char m_asduaddr;
public:
	void SetAddr(unsigned char addr);
	unsigned char   GetAddr();
};
int operator >> (ACE_InputCDR &cdr, ASDUADDR1 &asduaddr1);
int operator<< (ACE_OutputCDR &cdr, const ASDUADDR1 &asduaddr1);
//2字节表示的应用服务数据单元公共地址
class ASDUADDR2
{
public:
	unsigned char m_asduaddr0;
	unsigned char m_asduaddr1;
public:
	void SetAddr(unsigned short addr);
	unsigned short   GetAddr();
};
int operator >> (ACE_InputCDR &cdr, ASDUADDR2 &asduaddr2);
int operator<< (ACE_OutputCDR &cdr, const ASDUADDR2 &asduaddr2);
//////////////////////////////////////////////////////////////////////////
//2字节表示的信息对象地址
class INFOADDR2
{
public:
	unsigned char m_infoaddr0;
	unsigned char m_infoaddr1;
public:
	void SetAddr(unsigned short addr);
	unsigned short   GetAddr();
};
int operator >> (ACE_InputCDR &cdr, INFOADDR2 &infoaddr2);
int operator<< (ACE_OutputCDR &cdr, const INFOADDR2 &infoaddr2);
//3字节表示的信息对象地址
class INFOADDR3
{
public:
	unsigned char m_infoaddr0;
	unsigned char m_infoaddr1;
	unsigned char m_infoaddr2;
public:
	void SetAddr(int addr);
	int   GetAddr();
};
int operator >> (ACE_InputCDR &cdr, INFOADDR3 &infoaddr3);
int operator<< (ACE_OutputCDR &cdr, const INFOADDR3 &infoaddr3);

//////////////////////////////////////////////////////////////////////////
//#define CFG_223 
#ifdef CFG_223
typedef COT2            COT;
typedef ASDUADDR2 ASDUADDR;
typedef INFOADDR3  INFOADDR;
#elif defined(CFG_112)
typedef COT1            COT;
typedef ASDUADDR1 ASDUADDR;
typedef INFOADDR2  INFOADDR;
#else
typedef COT2            COT;
typedef ASDUADDR2 ASDUADDR;
typedef INFOADDR3  INFOADDR;
#endif




//////////////////////////////////////////////////////////////////////////

class ASDU_BASE
{
public:
	ACE_UINT8 type;         //类型标识
	ACE_UINT8 vsq;          //可变结构限定词
	COT cot;                     //传送原因
	ASDUADDR asduAddr;  //应用服务数据单元公共地址

public:
	bool IsSQ1()
	{
		if ((vsq & 0x80) == 0x80)
			return true;
		return false;
	}
};
int operator >> (ACE_InputCDR &cdr, ASDU_BASE &asduBase);
int operator<< (ACE_OutputCDR &cdr, const ASDU_BASE &asduBase);
//////////////////////////////////////////////////////////////////////////



#pragma pack(pop)

#endif//_ASDU_BASE_H
