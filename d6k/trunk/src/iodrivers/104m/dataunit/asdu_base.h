

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
	TYPE_N = 8,          //���к�����(����)
	TYPE_T = 16,        //��ʱ������(����)
	TYPE_ERROR = 32,    //������Ϣ
	TYPE_K = 64,          //Kֵ��Ϣ
	TYPE_W = 128,         //Wֵ��Ϣ
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
	TIMER_ID_T1 = 1001,   //TIMER_INTERVAL_T1=15, //���ͼ�����U֡���ȴ�ȷ�ϵĳ�ʱ
	TIMER_ID_T2 = 1002,   //TIMER_INTERVAL_T2=10,//���յ�I֡��wֵС��DEFAULT_W���������ȷ��W��������T2��ʱ�����ȷ��
	TIMER_ID_T3 = 1003,   //TIMER_INTERVAL_T3=20,//���ڿ���t3>t1״̬�·��Ͳ���֡�ĳ�ʱ
};


//������
enum
{
	PEER_MONITOR,         //���ӷ���,װ�� M
	PEER_CONTROL,         //���Ʒ���,��̨ C
};

//�ص���ʾ�ı����ʱ����Ϣ�ķ���(Message Callback)
enum
{
	MCB_TIMER,
	MCB_SEND,
	MCB_RECV,
	MCB_ERROR,
};


enum
{
	SEND_ERROR_K,          //����ʧ�ܣ�k > DEFAULT_K
	SEND_ERROR_SOCKET,     //����ʧ�ܣ�socket����
	SEND_OK,               //���ͳɹ�
};

enum
{
	ERROR_NONE,            //�޴���
	ERROR_SERIALNUMBER,    //���кŴ���
};


typedef unsigned short  BIT16;  //16λ
typedef unsigned long   BIT32;  //32λ
typedef unsigned char   BIT08;  //08λ

#define MYLO8(w)           ((BIT08)(((BIT32)(w)) & 0xff))
#define MYHI8(w)           ((BIT08)((((BIT32)(w)) >> 8) & 0xff))
#define MYLO16(l)           ((BIT16)(((BIT32)(l)) & 0xffff))
#define MYHI16(l)           ((BIT16)((((BIT32)(l)) >> 16) & 0xffff))

#define MYMAKE16(l, h)      ((BIT16)(((BIT08)(((BIT32)(l)) & 0xff)) | ((BIT16)((BIT08)(((BIT32)(h)) & 0xff))) << 8))
#define MYMAKE32(l, h)      ((BIT32)(((BIT16)(((BIT32)(l)) & 0xffff)) | ((BIT32)((BIT16)(((BIT32)(h)) & 0xffff))) << 16))



#pragma pack(push, 1)


//////////////////////////////////////////////////////////////////////////
//1�ֽڱ�ʾ�Ĵ���ԭ��
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
//2�ֽڱ�ʾ�Ĵ���ԭ��
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
//1�ֽڱ�ʾ��Ӧ�÷������ݵ�Ԫ������ַ
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
//2�ֽڱ�ʾ��Ӧ�÷������ݵ�Ԫ������ַ
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
//2�ֽڱ�ʾ����Ϣ�����ַ
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
//3�ֽڱ�ʾ����Ϣ�����ַ
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
	ACE_UINT8 type;         //���ͱ�ʶ
	ACE_UINT8 vsq;          //�ɱ�ṹ�޶���
	COT cot;                     //����ԭ��
	ASDUADDR asduAddr;  //Ӧ�÷������ݵ�Ԫ������ַ

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
