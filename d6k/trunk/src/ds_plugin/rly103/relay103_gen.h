#ifndef RELAY_103_GEN_H
#define RELAY_103_GEN_H

#include <stdint.h>
#include <functional>

/************************************************************************/
//		GDD Data Type����
/************************************************************************/
#define IEC103_GDD_TYPE_NONE			0    //!< ������
#define IEC103_GDD_TYPE_ASCII			1    //!< ASCII8λ��
#define IEC103_GDD_TYPE_BS1			    2    //!< ����8λ��
#define IEC103_GDD_TYPE_UINT			3    //!< �޷�������
#define IEC103_GDD_TYPE_INT			    4    //!< ����
#define IEC103_GDD_TYPE_UFLOAT			5    //!< �޷��Ÿ�����
#define IEC103_GDD_TYPE_FLOAT			6    //!< ������
#define IEC103_GDD_TYPE_R3223			7    //!< IEEE��׼754��ʵ��
#define IEC103_GDD_TYPE_R6453			8    //!< IEEE��׼754ʵ��
#define IEC103_GDD_TYPE_DPI			    9    //!< ˫����Ϣ
#define IEC103_GDD_TYPE_SPI			    10   //!< ������Ϣ
#define IEC103_GDD_TYPE_DPI_T_R		    11   //!< ��˲��Ͳ���˫����Ϣ
#define IEC103_GDD_TYPE_MEA_Q			12   //!< ��Ʒ�������ʵı���ֵ

#define IEC103_GDD_TYPE_BIN_TIME		14   //!< ������ʱ��
#define IEC103_GDD_TYPE_GIN			    15   //!< ͨ�÷����ʶ���
#define IEC103_GDD_TYPE_REL_TIME		16   //!< ���ʱ��
#define IEC103_GDD_TYPE_FUN_INF		    17   //!< �������ͺ���Ϣ���
#define IEC103_GDD_TYPE_M_TM_TA		    18   //!< ��ʱ��ı���
#define IEC103_GDD_TYPE_M_TMR_TA		19   //!< �����ʱ���ʱ�걨��
#define IEC103_GDD_TYPE_M_TME_TA		20   //!< �����ʱ���ʱ��ı���ֵ
#define IEC103_GDD_TYPE_EXT_TEXT_NUM	21   //!< �ⲿ�ı����
#define IEC103_GDD_TYPE_GRC		        22   //!< ͨ�÷���ش���
#define IEC103_GDD_TYPE_DATA_STRUCT  	23   //!< ���ݽṹ
#define IEC103_GDD_TYPE_INDEX			24   //!< ����

#define IEC103_GDD_TYPE_PULSE           201  //!< ������
#define IEC103_GDD_TYPE_TAP_SHORT       202  //!< ��λ(��ʱ��)
#define IEC103_GDD_TYPE_CTLCMD	        206	 //!< ���� - ��������
#define IEC103_GDD_TYPE_TAP             207  //!< ��λ

#define IEC103_GDD_MAX_LEN              256  //!< ͨ�÷���������󳤶�

//�ĸ�8λλ��ʱ��
typedef struct _CP32TIME2a
{
	uint8_t msL;   //!< ����L
	uint8_t msH;   //!< ����H
	uint8_t min;   //!< ��
	uint8_t hour;  //!< Сʱ
}CP32TIME2a;

//�߸�8λλ��ʱ��
typedef struct _CP56TIME2a
{
	uint8_t msL;   //!< ����L
	uint8_t msH;   //!< ����H
	uint8_t min;   //!< ��
	uint8_t hour;  //!< Сʱ
	uint8_t day;   //!< ��
	uint8_t month; //!< ��
	uint8_t year;  //!< ��
}CP56TIME2a;

enum KOD_TYPE
{
	IEC103_KOD_NONE             = 0,    //!< �����ƶ����������
	IEC103_KOD_ACTUAL_V         = 1,    //!< ʵ��ֵ
	IEC103_KOD_DEFAULT_V        = 2,    //!< ȱʡֵ
	IEC103_KOD_RANGE            = 3,    //!< ����

	IEC103_KOD_PRECISION        = 5,    //!< ����
	IEC103_KOD_FACTOR           = 6,    //!< ����
	IEC103_KOD_REF_PERCENT      = 7,    //!< %�α�
	IEC103_KOD_ENUM             = 8,    //!< �б�
	IEC103_KOD_UNIT             = 9,    //!< ����
	IEC103_KOD_DESCRIPTION      = 10,   //!< ����

	IEC103_KOD_PASSWORD         = 12,   //!< ������Ŀ
	IEC103_KOD_READ_ONLY        = 13,   //!< ֻ��
	IEC103_KOD_WRITE_ONLY       = 14,   //!< ֻд

	IEC103_KOD_FUN_INF          = 19,   //!< ��Ӧ�Ĺ������ͺ���Ϣ���
	IEC103_KOD_EVENT            = 20,   //!< ��Ӧ���¼�
	IEC103_KOD_ENUM_TEXT_ARRAY  = 21,   //!< �б���ı�����
	IEC103_KOD_ENUM_VALUE_ARRAY = 22,   //!< �б��ֵ����
	IEC103_KOD_RELATED          = 23,   //!< ���������Ŀ
	MAX_IEC103_KOD_TYPE
};

/************************************************************************/
//		ͨ�÷���ش���
/************************************************************************/
enum GEN_REPLY_CODE
{
	IEC103_GRC_OK,             //!< �Ͽ�
	IEC103_GRC_GIN_UNAVAI,     //!< ��Ч��ͨ�÷����ʶ���(GIN)
	IEC103_GRC_DATA_UNEXIST,   //!< �����������������
	IEC103_GRC_DATA_UNAVAI,    //!< ���ݲ����ù�������һ��
	IEC103_GRC_CONFIRM_ERR,    //!< �ı��趨ʱȷ�ϳ���
	IEC103_GRC_OVERRANGE,      //!< �ı��趨ʱ��������
	IEC103_GRC_ENERY,          //!< ��Ŀ�ķ�Χ̫��
	IEC103_GRC_COMMAND,        //!< ̫�������
	IEC103_GRC_READONLY,       //!< ��Ŀ��ֻ��
	IEC103_GRC_PASSWORD,       //!< �趨�ܿ����
	IEC103_GRC_MODIFYING,      //!< �����趨�ڽ�����
	IEC103_GRC_ERR_DESC        //!< ���������������Ĳ��
};

//ͨ�÷����ʶ���
typedef struct tagGin
{
	uint8_t group;    //!< ���
	uint8_t entry;    //!< ��Ŀ��
}GIN;

//ͨ�÷�����������
typedef struct tagGdd
{
	uint8_t dataType; //!< ��������
	uint8_t dataSize; //!< ���ݿ��
	uint8_t contNum;  //!< ������Ŀ(��7λΪ��Ŀ�����λΪ����״̬)
}GDD;

//ͨ�÷������ݼ�
typedef struct tagNgd
{
	GIN     gin;        //!< ͨ�÷����ʶ���
	uint8_t kod;        //!< �������
	GDD     gdd;        //!< ͨ�÷�����������
	uint8_t gid[IEC103_GDD_MAX_LEN];    //!< ͨ�÷����ʶ����
}NGD;

//ͨ�÷������ݼ�
typedef struct tagNgdData
{
	uint8_t ngd_num;              //!< ͨ�÷����������ݼ���Ŀ
	NGD     ngd[256];             //!< ͨ�÷�������
}NGD_DATA;

//��ʱ��ı���
typedef struct tagGddType18
{
	uint8_t    dpi;               //!< ˫����Ϣ
	CP32TIME2a cp32time2a;        //!< 4����λλ��ʱ��
	uint8_t    sin;               //!< ������Ϣ
}GDD_TYPE_18;

//�����ʱ���ʱ�걨��
typedef struct tagGddType19
{
	uint8_t    dpi;               //!< ˫����Ϣ
	uint8_t    relative_time[2];  //!< ���ʱ��
	uint8_t    fan[2];            //!< �������
	CP32TIME2a cp32time2a;        //!< 4����λλ��ʱ��
	uint8_t    sin;               //!< ������Ϣ
}GDD_TYPE_19;

//�����ʱ��ı���ֵ
typedef struct tagGddType20
{
	uint8_t    val[4];            //!< ����ֵ 
	uint8_t    relative_time[2];  //!< ���ʱ��
	uint8_t    fan[2];            //!< �������
	CP32TIME2a cp32time2a;        //!< 4����λλ��ʱ��
}GDD_TYPE_20;

//�����Ƽ������������ڵ�������
typedef struct tagGddType201
{
	int32_t  iValue;              //!< ����������
	int8_t   id;                  //!< ˳���
}GDD_TYPE_201;

//�����ֽ�ʱ���BCD��
typedef struct tagGddType202
{
	uint8_t    bcd[4];            //!< BCD��
	CP32TIME2a cp32time2a;        //!< 4����λλ��ʱ��
}GDD_TYPE_202;

//����ʱ��ı���
typedef struct tagGddType203
{
	uint8_t    dpi;               //!< ˫����Ϣ
	CP56TIME2a cp56time2a;        //!< 7����λλ��ʱ��
	uint8_t    sin;               //!< ������Ϣ
}GDD_TYPE_203;

//�����ʱ��ĳ�ʱ�걨��
typedef struct tagGddType204
{
	uint8_t    dpi;               //!< ˫����Ϣ
	uint8_t    relative_time[2];  //!< ���ʱ��
	uint8_t    fan[2];            //!< �������
	CP56TIME2a cp56time2a;        //!< 7����λλ��ʱ��
	uint8_t    sin;               //!< ������Ϣ
}GDD_TYPE_204;

//�����ʱ�����ֽ�ʱ��ı���ֵ
typedef struct tagGddType205
{
	uint8_t    val[4];            //!< ����ֵ 
	uint8_t    relative_time[2];  //!< ���ʱ��
	uint8_t    fan[2];            //!< �������
	CP56TIME2a cp56time2a;        //!< 7����λλ��ʱ��
}GDD_TYPE_205;

//�����ֽ�ʱ���BCD��
typedef struct tagGddType207
{
	uint8_t    bcd[4];            //!< BCD��
	CP56TIME2a cp56time2a;        //!< 7����λλ��ʱ��
}GDD_TYPE_207;

/*����������ṹ,����IEC103����*/
typedef struct GDD_DATA GDD_DATA;
struct GDD_DATA
{
	GDD_DATA *next;
	uint8_t  flag;      //!< ��Ч��־
	uint8_t  kod;       //!< �������
	GDD      gdd;       //!< ͨ�÷�����������
	uint8_t  *gid;      //!< ͨ�÷����ʶ����
};

#endif // RELAY_103_GEN_H
