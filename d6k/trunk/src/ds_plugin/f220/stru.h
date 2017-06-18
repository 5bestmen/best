/************************************************************************/
/** \file
* \n  �� �� �� : stru.h     
* \n  �ļ����� : ���ݽṹ����    
* \n  ��    �� : DAQO
* \n  ����ʱ�� : 2012.9.26
* \n  ��    ע : 
* \n  ��ʷ��¼ :
*/
/************************************************************************/
#ifndef _STRU_H_
#define _STRU_H_

// #include "msgQLib.h"
// #include "semLib.h"
// #include "prjParams.h"
// #include "stdio.h"
// #include "gydefine.h"
// #include "define.h"
// #include "DevFormulaDefine.h"
// #include "myfile.h"
// #include "basfifo.h"
// #include "interlock.h"

#define MAX_CALLOC_DATA	0x4000

#define  DZ_LEN      10
#define  MAX_BH_NUM  10
#define  NET_BYTES   10
#define  MAX_YC_NUM   10

typedef struct
{
	unsigned short	maxNum;
	unsigned short	ptr;
	char		buf[MAX_CALLOC_DATA];
}tCallocPool;

typedef struct
{/*��ms��������ʮ���Ʊ�ʾ*/
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
}tTimeForm;

typedef struct
{
	unsigned char	data[1024];
	unsigned short	bptr;
	unsigned short	eptr;
}tRecvFrameCtrl;



#define	BIN_PIECE_LEN	24
typedef struct
{
	unsigned long   wrDspBin;
	unsigned long	wrTimer;		/*дӦ��ʱ*/
	unsigned long	len;
	unsigned long	sendPtr;
	unsigned long	dspAck;
	unsigned char	wrPc;
	unsigned char	useFlag;
	unsigned char	wrRslt;			/*DSP�صĳ�����Ϣ*/
	
	unsigned char	*data;
}tDownloadDsp;					/*����ARM��DSPд�ļ�*/





typedef	struct
{
	unsigned char	headEb;
	unsigned char	head90;
	unsigned char	len;		/*����(19*N+4)*/
	unsigned char	r22;		/*code*/
	unsigned char	frameNo;
	unsigned char	bptr;		/*��ʼ��ַ*/
	unsigned char	total;
}tTripHead;

typedef struct
{
	unsigned char	no;		/*��ŵ�ַ*/
	unsigned char 	main_no;
	unsigned char 	sub_no;	
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
	unsigned char	msecond_l;
	unsigned char	msecond_h;
	unsigned char 	dzyj0;
	unsigned char 	dzyj1;
	unsigned char 	dzyj2;
	unsigned char 	dzyj3;
	unsigned char  	qtime_l;
	unsigned char  	qtime_h;
	unsigned char  	dzValue_l;
	unsigned char  	dzValue_h;
	unsigned char 	dz_phase;
	unsigned char 	com_flag;
	unsigned char  	nof_l;
	unsigned char  	nof_h;
	unsigned char 	fan_l;
	unsigned char 	fan_h;
	unsigned char 	qd_end;
}tTripRecFr;

typedef	struct
{
	unsigned char 	main_no;
	unsigned char 	sub_no;		
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char 	dzyj0;
	unsigned char 	dzyj1;
	unsigned char 	dzyj2;
	unsigned char 	dzyj3;
	unsigned char  	qtime_l;
	unsigned char  	qtime_h;
	unsigned char  	dzValue_l;
	unsigned char  	dzValue_h;
	unsigned char 	dz_phase;
	unsigned char 	com_flag;
	unsigned char  	nof_l;
	unsigned char  	nof_h;
	unsigned char 	fan_l;
	unsigned char 	fan_h;
	unsigned char 	qd_end;
}tOneTrip;



typedef struct
{
	unsigned char 	main_no;		
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char	bjyj0;
	unsigned char	bjyj1;
	unsigned char	bjyj2;
	unsigned char	bjyj3;
}tOneFail;





typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char	ob;
	unsigned char 	opVal;		/*yt,yh,xx,ck*/
	unsigned char 	opType;		/*ѡ��,ִ��,ȡ��*/
	unsigned char	com_flag;
	unsigned char 	serno;
	unsigned char	reason0;
	unsigned char	reason1;
	unsigned char	reason2;
	unsigned char	reason3;
	unsigned char	clientId0;
	unsigned char	clientId1;
	unsigned char	clientId2;
	unsigned char	clientId3;
	unsigned char	remote_flag;
}tOneCmd;


typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char 	com_flag;		/*2->1*/
	unsigned char	interval_l;
	unsigned char	interval_h;
	unsigned char	nof_l;
	unsigned char	nof_h;
	unsigned char	fan_l;
	unsigned char	fan_h;
	unsigned char 	sof;		/*8ֵ9��,no use?*/
	unsigned char 	bAddr_0;
	unsigned char 	bAddr_1;
	unsigned char 	bAddr_2;
	unsigned char 	eAddr_0;
	unsigned char 	eAddr_1;
	unsigned char 	eAddr_2;
}tOneGroup;



typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char  	serno;
	unsigned char	ob;	
	unsigned char 	newPos;
	unsigned char	oldPos;		
	unsigned char 	com_flag;	/*ֻ���ǵ���*/
}tOneDwRec;


typedef struct
{
	unsigned char	no;		/*��ŵ�ַ*/
	unsigned char	serno;		/*����˳���*/
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
	unsigned char	msecond_l;
	unsigned char	msecond_h;
	unsigned char	dzyj;
	unsigned char	state;
	unsigned char	fan_l;
	unsigned char	fan_h;
	unsigned char	qtime_l;
	unsigned char	qtime_h;
	unsigned char	nof_l;
	unsigned char	nof_h;
	unsigned char	com_flag;
}tSoeFr;

typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	
	unsigned char	dz_index;	/*����ң�ź�*/
	unsigned char 	state;		/*��ǰ״̬0/1+����*/
	unsigned char	fan_l;
	unsigned char	fan_h;
	unsigned char	qtime_l;	/*	�������ʱ��*/
	unsigned char	qtime_h;		/*�������ʱ��*/
	unsigned char	nof_l;
	unsigned char	nof_h;
	unsigned char 	com_flag;	/*ֻ���ǵ���*/
	unsigned char  	serno;	
}tOneSoe;


typedef struct
{
	unsigned char 	powera_pos_0;
	unsigned char 	powera_pos_1;
	unsigned char 	powera_pos_2;
	unsigned char 	powera_pos_3;
	unsigned char 	powera_neg_0;
	unsigned char 	powera_neg_1;
	unsigned char 	powera_neg_2;
	unsigned char 	powera_neg_3;
	unsigned char 	qowera_pos_0;
	unsigned char 	qowera_pos_1;
	unsigned char 	qowera_pos_2;
	unsigned char 	qowera_pos_3;
	unsigned char 	qowera_neg_0;
	unsigned char 	qowera_neg_1;
	unsigned char 	qowera_neg_2;
	unsigned char 	qowera_neg_3;
	
	unsigned char 	powerb_pos_0;
	unsigned char 	powerb_pos_1;
	unsigned char 	powerb_pos_2;
	unsigned char 	powerb_pos_3;
	unsigned char 	powerb_neg_0;
	unsigned char 	powerb_neg_1;
	unsigned char 	powerb_neg_2;
	unsigned char 	powerb_neg_3;
	unsigned char 	qowerb_pos_0;
	unsigned char 	qowerb_pos_1;
	unsigned char 	qowerb_pos_2;
	unsigned char 	qowerb_pos_3;
	unsigned char 	qowerb_neg_0;
	unsigned char 	qowerb_neg_1;
	unsigned char 	qowerb_neg_2;
	unsigned char 	qowerb_neg_3;
	
	unsigned char 	powerc_pos_0;
	unsigned char 	powerc_pos_1;
	unsigned char 	powerc_pos_2;
	unsigned char 	powerc_pos_3;
	unsigned char 	powerc_neg_0;
	unsigned char 	powerc_neg_1;
	unsigned char 	powerc_neg_2;
	unsigned char 	powerc_neg_3;
	unsigned char 	qowerc_pos_0;
	unsigned char 	qowerc_pos_1;
	unsigned char 	qowerc_pos_2;
	unsigned char 	qowerc_pos_3;
	unsigned char 	qowerc_neg_0;
	unsigned char 	qowerc_neg_1;
	unsigned char 	qowerc_neg_2;
	unsigned char 	qowerc_neg_3;
	unsigned char	sum;
}tPqh5700;

typedef struct
{
	unsigned char 	powera_pos_0;
	unsigned char 	powera_pos_1;
	unsigned char 	powera_pos_2;
	unsigned char 	powera_pos_3;
	unsigned char 	powera_neg_0;
	unsigned char 	powera_neg_1;
	unsigned char 	powera_neg_2;
	unsigned char 	powera_neg_3;
	unsigned char 	qowera_pos_0;
	unsigned char 	qowera_pos_1;
	unsigned char 	qowera_pos_2;
	unsigned char 	qowera_pos_3;
	unsigned char 	qowera_neg_0;
	unsigned char 	qowera_neg_1;
	unsigned char 	qowera_neg_2;
	unsigned char 	qowera_neg_3;
	
	unsigned char 	powerb_pos_0;
	unsigned char 	powerb_pos_1;
	unsigned char 	powerb_pos_2;
	unsigned char 	powerb_pos_3;
	unsigned char 	powerb_neg_0;
	unsigned char 	powerb_neg_1;
	unsigned char 	powerb_neg_2;
	unsigned char 	powerb_neg_3;
	unsigned char 	qowerb_pos_0;
	unsigned char 	qowerb_pos_1;
	unsigned char 	qowerb_pos_2;
	unsigned char 	qowerb_pos_3;
	unsigned char 	qowerb_neg_0;
	unsigned char 	qowerb_neg_1;
	unsigned char 	qowerb_neg_2;
	unsigned char 	qowerb_neg_3;
	
	unsigned char 	powerc_pos_0;
	unsigned char 	powerc_pos_1;
	unsigned char 	powerc_pos_2;
	unsigned char 	powerc_pos_3;
	unsigned char 	powerc_neg_0;
	unsigned char 	powerc_neg_1;
	unsigned char 	powerc_neg_2;
	unsigned char 	powerc_neg_3;
	unsigned char 	qowerc_pos_0;
	unsigned char 	qowerc_pos_1;
	unsigned char 	qowerc_pos_2;
	unsigned char 	qowerc_pos_3;
	unsigned char 	qowerc_neg_0;
	unsigned char 	qowerc_neg_1;
	unsigned char 	qowerc_neg_2;
	unsigned char 	qowerc_neg_3;
}tOneEnergy;

typedef struct
{
	tOneEnergy         l1;
	tOneEnergy         l2;
	tOneEnergy         l3;
	tOneEnergy         l4;
	tOneEnergy         l5;
	tOneEnergy         l6;
	 tOneEnergy         l7;
	tOneEnergy         l8;
	tOneEnergy         l9;
	tOneEnergy         l10;
	tOneEnergy         l11;
	tOneEnergy         l12;
	tOneEnergy         l13;
	tOneEnergy         l14;
	tOneEnergy         l15;
	tOneEnergy         l16;
	/* tOneEnergy         l17;
	tOneEnergy         l18;
	tOneEnergy         l19;
	tOneEnergy         l20;
	tOneEnergy         l21;
	tOneEnergy         l22;*/
	
	unsigned char	sum;
}tPqh5771;

typedef struct
{
	unsigned char ia_l;
	unsigned char ia_h;
	unsigned char ib_l;
	unsigned char ib_h;
	unsigned char ic_l;
	unsigned char ic_h;
	unsigned char i0_l;
	unsigned char i0_h;
	unsigned char imax_l;
	unsigned char imax_h;	
	unsigned char ua_l;
	unsigned char ua_h;
	unsigned char ub_l;
	unsigned char ub_h;
	unsigned char uc_l;
	unsigned char uc_h;
	unsigned char uab_l;
	unsigned char uab_h;
	unsigned char ubc_l;
	unsigned char ubc_h;
	unsigned char uca_l;
	unsigned char uca_h;
	unsigned char u0_l;
	unsigned char u0_h;
}tOneBh;

typedef struct
{
	tOneBh   l1;
	tOneBh   l2;
	tOneBh   l3;
	tOneBh   l4;
	tOneBh   l5;
	tOneBh   l6;
	tOneBh   l7;
	tOneBh   l8;
	tOneBh   l9;
	tOneBh   l10;
	tOneBh   l11;
	tOneBh   l12;
	tOneBh   l13;
	tOneBh   l14;
	tOneBh   l15;
	tOneBh   l16;

	unsigned char	sum;	   
}tProtect5771;

typedef struct
{
	unsigned long ia;
	unsigned long ib;
	unsigned long ic;
	unsigned long i0;
	unsigned long imax;
	unsigned long ua;
	unsigned long ub;
	unsigned long uc;
	unsigned long uab;
	unsigned long ubc;
	unsigned long uca;
	unsigned long u0;
}tBhOneLineData;

typedef struct
{
	tBhOneLineData   l1;
	tBhOneLineData   l2;
	tBhOneLineData   l3;
	tBhOneLineData   l4;
	tBhOneLineData   l5;
	tBhOneLineData   l6;
	tBhOneLineData   l7;
	tBhOneLineData   l8;
	tBhOneLineData   l9;
	tBhOneLineData   l10;
	tBhOneLineData   l11;
	tBhOneLineData   l12;
	tBhOneLineData   l13;
	tBhOneLineData   l14;
	tBhOneLineData   l15;
	tBhOneLineData   l16;
	
}tBh5771Val;

typedef struct
{
	unsigned char	deltFlag;	/*ң��仯��־*/
	union
	{
		unsigned long	data[MAX_BH_NUM];	/*���ݵĵ�1λ��ʾ�Ƿ�deltˢ�£�1��ˢ�£�0��δˢ��*/
		tBh5771Val	instVal;
	}dd;
	unsigned short  BhQuality[MAX_BH_NUM];
	tBh5771Val	val;
	tBh5771Val	valm;	/*δ�������ȵ���ֵ*/
}tProtect5771Data;

typedef struct
{
	tPqh5771	data5771;
	tPqh5700	data;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
	
}tEnergy;

typedef struct
{
	tProtect5771	protectval;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
}tProtect;

typedef union
{
	unsigned long	data[16];
	struct
	{
		unsigned long 	power_pos;
		unsigned long 	power_neg;
		unsigned long 	qower_pos;
		unsigned long 	qower_neg;
		
		unsigned long 	powera_pos;
		unsigned long 	powera_neg;
		unsigned long 	qowera_pos;
		unsigned long 	qowera_neg;
		
		unsigned long 	powerb_pos;
		unsigned long 	powerb_neg;
		unsigned long 	qowerb_pos;
		unsigned long 	qowerb_neg;
		
		unsigned long 	powerc_pos;
		unsigned long 	powerc_neg;
		unsigned long 	qowerc_pos;
		unsigned long 	qowerc_neg;
		
	}param;
}tEnergyData;

typedef struct
{
	unsigned long 	power_pos;
	unsigned long 	power_neg;
	unsigned long 	qower_pos;
	unsigned long 	qower_neg;
	
	unsigned long 	powera_pos;
	unsigned long 	powera_neg;
	unsigned long 	qowera_pos;
	unsigned long 	qowera_neg;
	
	unsigned long 	powerb_pos;
	unsigned long 	powerb_neg;
	unsigned long 	qowerb_pos;
	unsigned long 	qowerb_neg;
	
	unsigned long 	powerc_pos;
	unsigned long 	powerc_neg;
	unsigned long 	qowerc_pos;
	unsigned long 	qowerc_neg;
	
}tOneEnergyData;

typedef union
{
	unsigned long	data[352];
	struct{
		tOneEnergyData l1;
		tOneEnergyData l2;
		tOneEnergyData l3;
		tOneEnergyData l4;
		tOneEnergyData l5;
		tOneEnergyData l6;
		 tOneEnergyData l7;
		tOneEnergyData l8;
		tOneEnergyData l9;
		tOneEnergyData l10;
		tOneEnergyData l11;
		tOneEnergyData l12;
		tOneEnergyData l13;
		tOneEnergyData l14;
		tOneEnergyData l15;
		tOneEnergyData l16;
		/* tOneEnergyData l17;
		tOneEnergyData l18;
		tOneEnergyData l19;
		tOneEnergyData l20;
		tOneEnergyData l21;
		tOneEnergyData l22;	*/
	}param;
}tEnergy5771Data;

typedef struct	
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char	kr[22];			/*���ڴ��ң��*/
	unsigned short board_state;       /*���ڴ�Ű��״̬*/
	/*	unsigned char	ykfx[4];		*//*���ڴ��ң�ط�У*/
	unsigned char	dw[10];			/*���ڴ�ŵ�λ*/
	unsigned char	local;			/*���ڱ�ʶ�͵�*/
	unsigned char	releaLock;		/*���ڱ�ʶ�������*/
	unsigned char	skkr;			/*���ڱ�ʶ�ֿؿ���*/
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
	unsigned long	rcvCnt;			/*û���ܵ�һ��ȫң�ţ��ü�����++*/
}tQyx;

typedef struct	
{
	short    value;	
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tTemp;

typedef struct
{
	/*	unsigned long 	bj_flag;*/
	unsigned char	status[16];	/*���128·ң�ſ���*/
	/*	unsigned char	yxkr_bak[16];*/
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
	unsigned long	rcvCnt;			/*û���ܵ�һ��ȫң�ţ��ü�����++*/
}tProtectStatus;

typedef struct
{
	unsigned char	t[19];
	unsigned char	code[4];
}tCrcFrame;

typedef struct
{
	tCrcFrame	data;
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tCrc;

typedef struct
{
	unsigned char	progName[20];
	unsigned char	version[15];
}tVerFrame;

typedef struct
{
	tVerFrame	data;
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tVersion;

typedef struct
{
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tBhyc;

typedef struct
{	
	unsigned char 	data[128];
	unsigned char 	dataBak[128];
	unsigned char 	no;
	unsigned char	complete;
}tParamSet;

typedef struct
{
	unsigned char 	data[256];
	unsigned char 	dataBak[256];
	unsigned char 	no;
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tRelaySet;

typedef struct
{
	unsigned char	data[4];
	unsigned char	dataBak[4];
	unsigned long	rdTimer;
	unsigned char	rdPc;
	unsigned char	complete;
}tSoftYb;

typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	bus1U;
		unsigned short	bus2U;
		unsigned short	lx1U;
		unsigned short	lx2U;
		unsigned short	line1I;
		unsigned short	line2I;
		unsigned short	lx1I;
		unsigned short	lx2I;
		unsigned short	line1U;
		unsigned short	line2U;
		unsigned short  ma420_range;
		unsigned short	xbType1;
		unsigned short  volt_delt;
		unsigned short  current_delt;
		unsigned short  wattpower_delt;
		unsigned short  wattlesspower_delt;
		unsigned short	cycTimer;
		unsigned short	u0BjTr;
		unsigned short	gooseAddr;
		unsigned short	addr;
		unsigned short	meter;			
		unsigned short	ipA1;
		unsigned short	ipA2;
		unsigned short	ipB1;
		unsigned short	ipB2;
		unsigned short  bs_out;
		
		unsigned short	devMode;	/*��λ����*/
		unsigned short	bsMode;		/*��λ����*/
	}param;
}tZzcs5778Data;

typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	line2I;
		unsigned short	cycTimer;
		unsigned short        ykTime;
		unsigned short	u0BjTr;
		unsigned short       protocolType;
		unsigned short	addr;
		unsigned short	meter;			
	       unsigned short	ipA1;
		   unsigned short	ipA2;
		   unsigned short	ipB1;
		   unsigned short	ipB2;
		   unsigned short       lx_current_delt;
		   unsigned short   bus1A_volt_delt;
		  
		   unsigned short   line1_volt_delt;
		  
		   unsigned short   line1A_current_delt;
		   
		   unsigned short   wattpower_delt;
		   unsigned short   wattlesspower_delt;
		   unsigned short   fifthxb_delt;
		   unsigned short       tlxuyx;
		   unsigned short       fifthxbdz;
		   unsigned short       tfifthxb;
		   unsigned short       i1_ycyxdz;
		   unsigned short       i1_ycyxdz1;
		   unsigned short       tycyx1;
		   unsigned short       i2_ycyxdz;
		   unsigned short       i2_ycyxdz1;
		   unsigned short       tycyx2;
		   unsigned short       i3_ycyxdz;
		   unsigned short       i3_ycyxdz1;
		   unsigned short       tycyx3;
		   unsigned short       i4_ycyxdz;
		   unsigned short       i4_ycyxdz1;
		   unsigned short       tycyx4;
		   unsigned short       i5_ycyxdz;
		   unsigned short       i5_ycyxdz1;
		   unsigned short       tycyx5;
		   unsigned short       i6_ycyxdz;
		   unsigned short       i6_ycyxdz1;
		   unsigned short       tycyx6;

		  
		   unsigned short       i7_ycyxdz;
		   unsigned short       i7_ycyxdz1;
		   unsigned short       tycyx7;
		   unsigned short       i8_ycyxdz;
		   unsigned short       i8_ycyxdz1;
		   unsigned short       tycyx8;
		   unsigned short       i9_ycyxdz;
		   unsigned short       i9_ycyxdz1;
		   unsigned short       tycyx9;
		   unsigned short       i10_ycyxdz;
		   unsigned short       i10_ycyxdz1;
		   unsigned short       tycyx10;
		   unsigned short       i11_ycyxdz;
		   unsigned short       i11_ycyxdz1;
		   unsigned short       tycyx11;

		   unsigned short       i12_ycyxdz;
		   unsigned short       i12_ycyxdz1;
		   unsigned short       tycyx12;
		   unsigned short       i13_ycyxdz;
		   unsigned short       i13_ycyxdz1;
		   unsigned short       tycyx13;
		   unsigned short       i14_ycyxdz;
		   unsigned short       i14_ycyxdz1;
		   unsigned short       tycyx14;
		   unsigned short       i15_ycyxdz;
		   unsigned short       i15_ycyxdz1;
		   unsigned short       tycyx15;
		   unsigned short       i16_ycyxdz;
		   unsigned short       i16_ycyxdz1;
		   unsigned short       tycyx16;
		   unsigned short       getfile_enable;
		   
		   /*		unsigned short	devMode;	*//*��λ����*/
		   /*		unsigned short	bsMode;		*//*��λ����*/
	}param;
}tZzcs5771Data;

typedef struct
{
	
	unsigned char	line2I_l;
	unsigned char	line2I_h;
	unsigned char cycTimer_l;
	unsigned char cycTimer_h;
	unsigned char  ykTime_l;
	unsigned char  ykTime_h;
	unsigned char	u0BjTr_l;
	unsigned char	u0BjTr_h;
	unsigned char protocolType_l;
	unsigned char protocolType_h;
	unsigned char	addr_l;
	unsigned char	addr_h;
	unsigned char	meter_l;			
	unsigned char	meter_h;			
	unsigned char	ipA1_l;
	unsigned char	ipA1_h;
	unsigned char	ipA2_l;
	unsigned char	ipA2_h;
	unsigned char	ipB1_l;
	unsigned char	ipB1_h;
	unsigned char	ipB2_l;
	unsigned char	ipB2_h;
	unsigned char   lx_current_delt_l;
	unsigned char   lx_current_delt_h;
	unsigned char   bus1A_volt_delt_l;
	unsigned char   bus1A_volt_delt_h;
	unsigned char   line1_volt_delt_l;
	unsigned char   line1_volt_delt_h;
	
	unsigned char   line1A_current_delt_l;
	unsigned char   line1A_current_delt_h;
	
	unsigned char   wattpower_delt_l;
	unsigned char   wattpower_delt_h;
	unsigned char   wattlesspower_delt_l;
	unsigned char   wattlesspower_delt_h;
	unsigned char  fifthxb_delt_l;
	unsigned char  fifthxb_delt_h;
	unsigned char  tlxuyx_l;
	unsigned char  tlxuyx_h;
	unsigned char  fifthxbdz_l;
	unsigned char  fifthxbdz_h;
	unsigned char  tfifthxb_l;
	unsigned char  tfifthxb_h;
	unsigned char  i1_ycyxdz_l;
	unsigned char  i1_ycyxdz_h;
	unsigned char  i1_ycyxdz1_l;
	unsigned char  i1_ycyxdz1_h;
	unsigned char  tycyx1_l;
	unsigned char  tycyx1_h;
	unsigned char  i2_ycyxdz_l;
	unsigned char  i2_ycyxdz_h;
	unsigned char  i2_ycyxdz1_l;
	unsigned char  i2_ycyxdz1_h;
	unsigned char  tycyx2_l;
	unsigned char  tycyx2_h;
	unsigned char  i3_ycyxdz_l;
	unsigned char  i3_ycyxdz_h;
	unsigned char  i3_ycyxdz1_l;
	unsigned char  i3_ycyxdz1_h;
	unsigned char  tycyx3_l;
	unsigned char  tycyx3_h;
	unsigned char  i4_ycyxdz_l;
	unsigned char  i4_ycyxdz_h;
	unsigned char  i4_ycyxdz1_l;
	unsigned char  i4_ycyxdz1_h;
	unsigned char  tycyx4_l;
	unsigned char  tycyx4_h;
	unsigned char  i5_ycyxdz_l;
	unsigned char  i5_ycyxdz_h;
	unsigned char  i5_ycyxdz1_l;
	unsigned char  i5_ycyxdz1_h;
	unsigned char  tycyx5_l;
	unsigned char  tycyx5_h;
	unsigned char  i6_ycyxdz_l;
	unsigned char  i6_ycyxdz_h;
	unsigned char  i6_ycyxdz1_l;
	unsigned char  i6_ycyxdz1_h;
	unsigned char  tycyx6_l;
	unsigned char  tycyx6_h;
	
	
		unsigned char  i7_ycyxdz_l;
		unsigned char  i7_ycyxdz_h;
		unsigned char  i7_ycyxdz1_l;
		unsigned char  i7_ycyxdz1_h;
		unsigned char  tycyx7_l;
		unsigned char  tycyx7_h;
		unsigned char  i8_ycyxdz_l;
		unsigned char  i8_ycyxdz_h;
		unsigned char  i8_ycyxdz1_l;
		unsigned char  i8_ycyxdz1_h;
		unsigned char  tycyx8_l;
		unsigned char  tycyx8_h;
		unsigned char  i9_ycyxdz_l;
		unsigned char  i9_ycyxdz_h;
		unsigned char  i9_ycyxdz1_l;
		unsigned char  i9_ycyxdz1_h;
		unsigned char  tycyx9_l;
		unsigned char  tycyx9_h;
		unsigned char  i10_ycyxdz_l;
		unsigned char  i10_ycyxdz_h;
		unsigned char  i10_ycyxdz1_l;
		unsigned char  i10_ycyxdz1_h;
		unsigned char  tycyx10_l;
		unsigned char  tycyx10_h;
		unsigned char  i11_ycyxdz_l;
		unsigned char  i11_ycyxdz_h;
		unsigned char  i11_ycyxdz1_l;
		unsigned char  i11_ycyxdz1_h;
		unsigned char  tycyx11_l;
		unsigned char  tycyx11_h;
		unsigned char  i12_ycyxdz_l;
		unsigned char  i12_ycyxdz_h;
		unsigned char  i12_ycyxdz1_l;
		unsigned char  i12_ycyxdz1_h;
		unsigned char  tycyx12_l;
		unsigned char  tycyx12_h;
	
			unsigned char  i13_ycyxdz_l;
			unsigned char  i13_ycyxdz_h;
			unsigned char  i13_ycyxdz1_l;
			unsigned char  i13_ycyxdz1_h;
			unsigned char  tycyx13_l;
			unsigned char  tycyx13_h;
			
			unsigned char  i14_ycyxdz_l;
			unsigned char  i14_ycyxdz_h;
			unsigned char  i14_ycyxdz1_l;
			unsigned char  i14_ycyxdz1_h;
			unsigned char  tycyx14_l;
			unsigned char  tycyx14_h;
			unsigned char  i15_ycyxdz_l;
			unsigned char  i15_ycyxdz_h;
			unsigned char  i15_ycyxdz1_l;
			unsigned char  i15_ycyxdz1_h;
			unsigned char  tycyx15_l;
			unsigned char  tycyx15_h;
			unsigned char  i16_ycyxdz_l;
			unsigned char  i16_ycyxdz_h;
			unsigned char  i16_ycyxdz1_l;
			unsigned char  i16_ycyxdz1_h;
			unsigned char  tycyx16_l;
			unsigned char  tycyx16_h;
	unsigned char  getfile_enable_l;
	unsigned char  getfile_enable_h;
	
	/*	unsigned char	devMode_l;	*//*��λ����*/
	/*	unsigned char	devMode_h;	*//*��λ����*/
	/*	unsigned char	bsMode_l;	*//*��λ����*/
	/*	unsigned char	bsMode_h;	*//*��λ����*/
	unsigned char	sum;
	unsigned char	commSum;
}tZzcs5771;

typedef union
{
	tZzcs5771Data	d5771;
	tZzcs5778Data	d5778;
}tZzcs5700Data;

typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	type1;  	/*������ ����*/      
		unsigned short	type2;  	/*������ ����*/      
		unsigned short	type3;  	/*������ ����*/      
		unsigned short	type4;  	/*������ ����*/      
	}param;
}tDccs5700Data;
typedef union
{
	unsigned short	zlm[8];
}tDctf5700Data;
typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	zl10;		/*�������*/
		unsigned short	zl20;		/*�������*/
		unsigned short	zl30;		/*�������*/
		unsigned short	zl40;		/*�������*/
		unsigned short	zl1m;		/*��������*/
		unsigned short	zl2m;		/*��������*/
		unsigned short	zl3m;		/*��������*/
		unsigned short	zl4m;		/*��������*/
	}param;
}tDcjd5700Data;

typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	baudRate;
		unsigned short	dataBit;
		unsigned short	parityCheck;
		unsigned short	stopBit;
	}param;
}tCktxcs5700Data;

typedef struct
{
	unsigned char	baudRate_l;
	unsigned char	baudRate_h;
	unsigned char	dataBit_l;
	unsigned char	dataBit_h;
	unsigned char	parityCheck_l;
	unsigned char	parityCheck_h;
	unsigned char	stopBit_l;
	unsigned char	stopBit_h;
	unsigned char	sum;
	unsigned char	commSum;
}tCktxcs5700;

typedef struct
{
	tCktxcs5700       cs;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tCktxcsSet;

typedef struct
{
	union
	{
		tZzcs5771	d5771;
	}cs;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tZzcsSet;

typedef struct
{
	unsigned short	ua;
	unsigned short	ub;
	unsigned short	uc;
	unsigned short	u0;
	unsigned short	uab;
	unsigned short	ubc;
	unsigned short	uca;
	unsigned short	ia;
	unsigned short	ib;
	unsigned short	ic;
	unsigned short	i0;
	unsigned short	pa;
	unsigned short	qa;
	unsigned short	pb;
	unsigned short	qb;
	unsigned short	pc;
	unsigned short	qc;
}tOneJdtzData;

typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	ua;
		unsigned short	ub;
		unsigned short	uc;
		unsigned short	u0;
		unsigned short	uab;
		unsigned short	ubc;
		unsigned short	uca;
		unsigned short	ia;
		unsigned short	ib;
		unsigned short	ic;
		unsigned short	i0;
		unsigned short	pa;
		unsigned short	qa;
		unsigned short	pb;
		unsigned short	qb;
		unsigned short	pc;
		unsigned short	qc;			
	}param;
}tJdtz5771Data;

typedef union
{
	tJdtz5771Data d5771;
}tJdtz5700Data;

typedef struct
{
	short	ua;
	short	ub;
	short	uc;
	short	u0;
	short	uab;
	short	ubc;
	short	uca;
	short	ia;
	short	ib;
	short	ic;
	short	i0;
	short	cosa;
	short	sina;
	short	cosb;
	short	sinb;
	short	cosc;
	short	sinc;
}tOneJdcsData;

typedef struct
{
	unsigned short	ua;
	unsigned short	ub;
	unsigned short	uc;
	unsigned short	u0;
	unsigned short	uab;
	unsigned short	ubc;
	unsigned short	uca;
	unsigned short	ia;
	unsigned short	ib;
	unsigned short	ic;
	unsigned short	i0;
	unsigned short	cosa;
	unsigned short	sina;
	unsigned short	cosb;
	unsigned short	sinb;
	unsigned short	cosc;
	unsigned short	sinc;
}tOneJdcs5771Data;
typedef struct
{
	unsigned short	ua;
	unsigned short	ub;
	unsigned short	uc;
	unsigned short	u0;
	unsigned short	uab;
	unsigned short	ubc;
	unsigned short	uca;
	unsigned short	ia;
	unsigned short	ib;
	unsigned short	ic;
	unsigned short	i0;
}tOneBhJdcs5771Data;
typedef union
{
	unsigned short	data[2*DZ_LEN];
	struct
	{
		tOneJdcs5771Data l1;
		tOneJdcs5771Data l2;
		tOneJdcs5771Data l3;
		tOneJdcs5771Data l4;
		tOneJdcs5771Data l5;
		tOneJdcs5771Data l6;
		tOneJdcs5771Data l7;
		tOneJdcs5771Data l8;
		tOneJdcs5771Data l9;
		tOneJdcs5771Data l10;
		tOneJdcs5771Data l11;
		
		tOneJdcs5771Data l12;
		tOneJdcs5771Data l13;
		tOneJdcs5771Data l14;
		tOneJdcs5771Data l15;
		tOneJdcs5771Data l16;
		tOneBhJdcs5771Data bh_l1;
		tOneBhJdcs5771Data bh_l2;
		tOneBhJdcs5771Data bh_l3;
		tOneBhJdcs5771Data bh_l4;
		tOneBhJdcs5771Data bh_l5;
		tOneBhJdcs5771Data bh_l6;
		tOneBhJdcs5771Data bh_l7;
		tOneBhJdcs5771Data bh_l8;
		tOneBhJdcs5771Data bh_l9;
		tOneBhJdcs5771Data bh_l10;
	
		tOneBhJdcs5771Data bh_l11;
		tOneBhJdcs5771Data bh_l12;
		tOneBhJdcs5771Data bh_l13;
		tOneBhJdcs5771Data bh_l14;
		tOneBhJdcs5771Data bh_l15;
		tOneBhJdcs5771Data bh_l16;

	}param;
}tJdcs5771Data;
typedef struct
{
	unsigned char	ua_l;
	unsigned char	ua_h;
	unsigned char	ub_l;
	unsigned char	ub_h;
	unsigned char	uc_l;
	unsigned char	uc_h;
	unsigned char	u0_l;
	unsigned char	u0_h;
	unsigned char	uab_l;
	unsigned char	uab_h;
	unsigned char	ubc_l;
	unsigned char	ubc_h;
	unsigned char	uca_l;
	unsigned char	uca_h;
	unsigned char	ia_l;
	unsigned char	ia_h;
	unsigned char	ib_l;
	unsigned char	ib_h;
	unsigned char	ic_l;
	unsigned char	ic_h;
	unsigned char	i0_l;
	unsigned char	i0_h;
	unsigned char	cosa_l;
	unsigned char	cosa_h;
	unsigned char	sina_l;
	unsigned char	sina_h;
	unsigned char	cosb_l;
	unsigned char	cosb_h;
	unsigned char	sinb_l;
	unsigned char	sinb_h;
	unsigned char	cosc_l;
	unsigned char	cosc_h;
	unsigned char	sinc_l;
	unsigned char	sinc_h;
}tOneJdcs;
typedef struct
{
	unsigned char	ua_l;
	unsigned char	ua_h;
	unsigned char	ub_l;
	unsigned char	ub_h;
	unsigned char	uc_l;
	unsigned char	uc_h;
	unsigned char	u0_l;
	unsigned char	u0_h;
	unsigned char	uab_l;
	unsigned char	uab_h;
	unsigned char	ubc_l;
	unsigned char	ubc_h;
	unsigned char	uca_l;
	unsigned char	uca_h;
	unsigned char	ia_l;
	unsigned char	ia_h;
	unsigned char	ib_l;
	unsigned char	ib_h;
	unsigned char	ic_l;
	unsigned char	ic_h;
	unsigned char	i0_l;
	unsigned char	i0_h;
	unsigned char	cosa_l;
	unsigned char	cosa_h;
	unsigned char	sina_l;
	unsigned char	sina_h;
	unsigned char	cosb_l;
	unsigned char	cosb_h;
	unsigned char	sinb_l;
	unsigned char	sinb_h;
	unsigned char	cosc_l;
	unsigned char	cosc_h;
	unsigned char	sinc_l;
	unsigned char	sinc_h;
}tOneJdcs5771;

typedef struct
{
    unsigned char	ua_l;
	unsigned char	ua_h;
	unsigned char	ub_l;
	unsigned char	ub_h;
	unsigned char	uc_l;
	unsigned char	uc_h;
	unsigned char	u0_l;
	unsigned char	u0_h;
	unsigned char	uab_l;
	unsigned char	uab_h;
	unsigned char	ubc_l;
	unsigned char	ubc_h;
	unsigned char	uca_l;
	unsigned char	uca_h;
	unsigned char	ia_l;
	unsigned char	ia_h;
	unsigned char	ib_l;
	unsigned char	ib_h;
	unsigned char	ic_l;
	unsigned char	ic_h;
	unsigned char	i0_l;
	unsigned char	i0_h;
}tOneBhJdcs5771;

typedef struct
{
	tOneJdcs5771 l1;
	tOneJdcs5771 l2;
	tOneJdcs5771 l3;
	tOneJdcs5771 l4;
	tOneJdcs5771 l5;
	tOneJdcs5771 l6;

	tOneJdcs5771 l7;
	tOneJdcs5771 l8;
	tOneJdcs5771 l9;
	tOneJdcs5771 l10;
	tOneJdcs5771 l11;
	tOneJdcs5771 l12;

	tOneJdcs5771 l13;

	tOneJdcs5771 l14;
	tOneJdcs5771 l15;
	tOneJdcs5771 l16;

	tOneBhJdcs5771 bh_l1;
	tOneBhJdcs5771 bh_l2;
	tOneBhJdcs5771 bh_l3;
	tOneBhJdcs5771 bh_l4;
	tOneBhJdcs5771 bh_l5;
	tOneBhJdcs5771 bh_l6;
	tOneBhJdcs5771 bh_l7;
	tOneBhJdcs5771 bh_l8;
	tOneBhJdcs5771 bh_l9;
	tOneBhJdcs5771 bh_l10;
	tOneBhJdcs5771 bh_l11;

	tOneBhJdcs5771 bh_l12;

	tOneBhJdcs5771 bh_l13;
	tOneBhJdcs5771 bh_l14;
	tOneBhJdcs5771 bh_l15;
	tOneBhJdcs5771 bh_l16;

	unsigned char	sum;
	unsigned char   commSum;
}tJdcs5771;

typedef union
{
	tJdcs5771Data	d5771;	
}tJdcs5700Data;
typedef struct
{
	union
	{
		tJdcs5771	d5771;
	}cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
	unsigned char	wrDsp;
}tJdcsSet;
typedef struct
{
	unsigned short 	yx_time[200];
	unsigned char	sum;
}tYxcs5700Data;
typedef struct
{
	unsigned char 	yx_time[200*2];
	unsigned char	sum;
	unsigned char	commSum;
}tYxcs5700;
typedef struct
{	
	tYxcs5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
	unsigned short 	validNum;	/*��Чң����*/
}tYxcsSet;
typedef struct
{
	unsigned short	code;		/*0��NULL 1��BCD��2��DJD��3��JW*/
	unsigned short	dwgs;
	unsigned short	dwfdTimer;	/*��λ����ʱ��*/
	unsigned short	hdjt;
	unsigned short	phsdw;
}tOneDwcs;
typedef struct
{
	union
	{
		unsigned short	data[DZ_LEN];
		struct
		{
			unsigned short	Cycle;		/*0��NULL 1��BCD��2��DJD��3��JW*/
			unsigned short	year;
			unsigned short	month;	/*��λ����ʱ��*/
			unsigned short	day;
			unsigned short	hour;
			unsigned short	minute;
			unsigned short	second;
			unsigned short	duration;
		}param;
	}dd;
	tOneDwcs	cs[2];
}tDwcs5700Data;
typedef struct
{
	unsigned char	code_l;		/*0��NULL 1��BCD��2��DJD��3��JW*/
	unsigned char	code_h;		/*0��NULL 1��BCD��2��DJD��3��JW*/
	unsigned char	dwgs_l;
	unsigned char	dwgs_h;
	unsigned char	dwfdTimer_l;	/*��λ����ʱ��*/
	unsigned char	dwfdTimer_h;	/*��λ����ʱ��*/
	unsigned char	hdjt_l;		
	unsigned char	hdjt_h;		
	unsigned char	phsdw_l;		/*���൵λ*/
	unsigned char	phsdw_h;		/*���൵λ*/
	unsigned char	minute_l;		/*���൵λ*/
	unsigned char	minute_h;		/*���൵λ*/
	unsigned char	second_l;		/*���൵λ*/
	unsigned char	second_h;		/*���൵λ*/
	unsigned char	duration_l;		/*���൵λ*/
	unsigned char	duration_h;		/*���൵λ*/
	unsigned char 	sum;
	unsigned char	commSum;
}tDwcs5700;
typedef struct
{
	tDwcs5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
}tDwcsSet;
typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	wyMod;			/*��ѹģʽ*/
		unsigned short	difV;
		unsigned short	difHz;
		unsigned short	difAng;
		unsigned short	bkrTmms;
		unsigned short	rstTmms;		/*ͬ�ڸ���ʱ��*/       
		unsigned short	lineType;   		/*��·��ѹ����*/
		unsigned short	angCor;			/*ͬ�ڲ�����*/
		unsigned short	synMod;			/*ͬ��ͬ��ģʽ,bj,jwy,jtq*/
		unsigned short	dfdt;
	}param;
}tTqcs5700Data;
typedef struct
{
	unsigned char	wyMod_l;			/*��ѹģʽ*/
	unsigned char	wyMod_h;			/*��ѹģʽ*/
	unsigned char	difV_l;
	unsigned char	difV_h;
	unsigned char	difHz_l;
	unsigned char	difHz_h;
	unsigned char	difAng_l;
	unsigned char	difAng_h;
	unsigned char	bkrTmms_l;
	unsigned char	bkrTmms_h;
	unsigned char	rstTmms_l;		/*ͬ�ڸ���ʱ��*/       
	unsigned char	rstTmms_h;		/*ͬ�ڸ���ʱ��*/       
	unsigned char	lineType_l;   		/*��·��ѹ����*/
	unsigned char	lineType_h;   		/*��·��ѹ����*/
	unsigned char	angCor_l;			/*ͬ�ڲ�����*/
	unsigned char	angCor_h;			/*ͬ�ڲ�����*/
	unsigned char	synMod_l;			/*ͬ��ͬ��ģʽ,bj,jwy,jtq*/
	unsigned char	synMod_h;			/*ͬ��ͬ��ģʽ,bj,jwy,jtq*/
	unsigned char	dfdt_l;
	unsigned char	dfdt_h;
	unsigned char 	sum;
	unsigned char	commSum;
}tTqcs5700;
typedef struct
{
	tTqcs5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
}tTqcsSet;
typedef struct
{
	unsigned char	zl10_l;
	unsigned char	zl10_h;
	unsigned char	zl20_l;
	unsigned char	zl20_h;
	unsigned char	zl30_l;
	unsigned char	zl30_h;
	unsigned char	zl40_l;
	unsigned char	zl40_h;
	unsigned char	zl1m_l;
	unsigned char	zl1m_h;
	unsigned char	zl2m_l;
	unsigned char	zl2m_h;
	unsigned char	zl3m_l;
	unsigned char	zl3m_h;
	unsigned char	zl4m_l;
	unsigned char	zl4m_h;
	unsigned char 	sum;
	unsigned char	commSum;
}tDcjd5700;
typedef struct
{
	tDcjd5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
}tDcjdSet;
typedef struct
{
	unsigned char	type1_l;  	/*������1 ����*/      
	unsigned char	type1_h;  	/*������1 ����*/      
	
	unsigned char	type2_l;  	/*������ ����*/      
	unsigned char	type2_h;  	/*������ ����*/             
	
	unsigned char	type3_l;  	/*������ ����*/      
	unsigned char	type3_h;  	/*������ ����*/        
	
	unsigned char	type4_l;  	/*������ ����*/      
	unsigned char	type4_h;  	/*������ ����*/      
	
	unsigned char	sum;
	unsigned char	commSum;
}tDccs5700;

typedef struct
{
	unsigned char	ddcs_l;        
	unsigned char	ddcs_h;  	       
	
	unsigned char	sum;
	unsigned char	commSum;
}tDdcs5700;

typedef struct
{
	tDccs5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
}tDccsSet;

typedef struct
{
	tDdcs5700	cs;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;
}tDdcsSet;

typedef struct
{
	unsigned short 	delt;
	unsigned short	range;
}tOutCommYccs;		/*��վ��ң�����Ͳ����������仯��ֵ�Ͷֵ*/


typedef struct
{
	tOutCommYccs	ua;
	tOutCommYccs	ub;
	tOutCommYccs	uc;
	tOutCommYccs	uab;
	tOutCommYccs	ubc;
	tOutCommYccs	uca;
	tOutCommYccs	u0;
	tOutCommYccs	ia;
	tOutCommYccs	ib;
	tOutCommYccs	ic;
	tOutCommYccs	i0;
	tOutCommYccs	pam;
	tOutCommYccs	sam;
	tOutCommYccs	qam;
	tOutCommYccs	pbm;
	tOutCommYccs	sbm;
	tOutCommYccs	qbm;
	tOutCommYccs	pcm;
	tOutCommYccs	scm;
	tOutCommYccs	qcm;	/*�Ƕ�У��ǰ�ķ��๦��*/
	tOutCommYccs	pa;
	tOutCommYccs	sa;
	tOutCommYccs	cosa;
	tOutCommYccs	qa;
	tOutCommYccs	pb;
	tOutCommYccs	sb;
	tOutCommYccs	cosb;
	tOutCommYccs	qb;
	tOutCommYccs	pc;
	tOutCommYccs	sc;
	tOutCommYccs	cosc;
	tOutCommYccs	qc;	/*�Ƕ�У����ķ��๦��*/
	tOutCommYccs	p;
	tOutCommYccs	s;
	tOutCommYccs	cosq;
	tOutCommYccs	q;
	tOutCommYccs	f;
}tOneDeltYccs;

typedef struct
{
	tOutCommYccs	ua;
	tOutCommYccs	ub;
	tOutCommYccs	uc;
	tOutCommYccs	uab;
	tOutCommYccs	ubc;
	tOutCommYccs	uca;
	tOutCommYccs	u0;
	/*	tOutCommYccs	f;*/
	tOutCommYccs	ia;
	tOutCommYccs	ib;
	tOutCommYccs	ic;
	tOutCommYccs	i0;
	tOutCommYccs	pam;
	tOutCommYccs	sam;
	tOutCommYccs	qam;
	tOutCommYccs	pbm;
	tOutCommYccs	sbm;
	tOutCommYccs	qbm;
	tOutCommYccs	pcm;
	tOutCommYccs	scm;
	tOutCommYccs	qcm;	/*�Ƕ�У��ǰ�ķ��๦��*/
	tOutCommYccs	pa;
	tOutCommYccs	sa;
	tOutCommYccs	cosa;
	tOutCommYccs	qa;
	tOutCommYccs	pb;
	tOutCommYccs	sb;
	tOutCommYccs	cosb;
	tOutCommYccs	qb;
	tOutCommYccs	pc;
	tOutCommYccs	sc;
	tOutCommYccs	cosc;
	tOutCommYccs	qc;	/*�Ƕ�У����ķ��๦��*/
	tOutCommYccs	p;
	tOutCommYccs	s;
	tOutCommYccs	cosq;
	tOutCommYccs	q;
}tOneDeltYccs5771;

typedef struct
{
	tOneDeltYccs5771 l1;
	tOneDeltYccs5771 l2;
	tOneDeltYccs5771 l3;
	tOneDeltYccs5771 l4;
	tOneDeltYccs5771 l5;
	tOneDeltYccs5771 l6;
	
	tOneDeltYccs5771 l7;
		tOneDeltYccs5771 l8;
		tOneDeltYccs5771 l9;
		tOneDeltYccs5771 l10;
		tOneDeltYccs5771 l11;
		tOneDeltYccs5771 l12;
		
		tOneDeltYccs5771 l13;
			tOneDeltYccs5771 l14;
			tOneDeltYccs5771 l15;
			tOneDeltYccs5771 l16;
		
	tOutCommYccs	f1;
	tOutCommYccs	f2;
	tOutCommYccs	f3;
		tOutCommYccs	f4;
	tOutCommYccs	phaseU1aU2a;
	tOutCommYccs	zlm[4];	
	tOutCommYccs      fifthxb[6];
}tDeltYccs5771;

typedef struct
{
	tOutCommYccs	ia;
	tOutCommYccs	ib;
	tOutCommYccs	ic;
	tOutCommYccs	i0;
	tOutCommYccs	imax;
	tOutCommYccs	ua;
	tOutCommYccs	ub;
	tOutCommYccs	uc;
	tOutCommYccs	uab;
	tOutCommYccs	ubc;
	tOutCommYccs	uca;
	tOutCommYccs	u0;	
}tOneDeltBhcs5771;

typedef struct
{
	tOneDeltBhcs5771 l1;
	tOneDeltBhcs5771 l2;
	tOneDeltBhcs5771 l3;
	tOneDeltBhcs5771 l4;
	tOneDeltBhcs5771 l5;
	tOneDeltBhcs5771 l6;
	
	tOneDeltBhcs5771 l7;
		tOneDeltBhcs5771 l8;
		tOneDeltBhcs5771 l9;
		tOneDeltBhcs5771 l10;
		tOneDeltBhcs5771 l11;
		tOneDeltBhcs5771 l12;
		
		tOneDeltBhcs5771 l13;
			tOneDeltBhcs5771 l14;
			tOneDeltBhcs5771 l15;
			tOneDeltBhcs5771 l16;
			
}tDeltBhcs5771;
typedef union
{
	tDeltYccs5771	d5771;
}tDeltYccs;

typedef struct
{
	short		zlm[4];		/*ֱ�����ȵ���*/
}tDctf;
typedef struct
{
	unsigned char	izds1_l;  
	unsigned char	izds1_h; 
	unsigned char	tglzd1_l;  
	unsigned char	tglzd1_h; 
	unsigned char	igfhzds1_l;  
	unsigned char	igfhzds1_h;  
	unsigned char	tgfhzd1_l;  
	unsigned char	tgfhzd1_h; 
	unsigned char	i0zds1_l;  
	unsigned char	i0zds1_h;  
	unsigned char	tgl0zd1_l;  
	unsigned char	tgl0zd1_h; 
	
	unsigned char	izds2_l;  
	unsigned char	izds2_h; 
	unsigned char	tglzd2_l;  
	unsigned char	tglzd2_h; 
	unsigned char	igfhzds2_l;  
	unsigned char	igfhzds2_h;  
	unsigned char	tgfhzd2_l;  
	unsigned char	tgfhzd2_h; 
	unsigned char	i0zds2_l;  
	unsigned char	i0zds2_h;  
	unsigned char	tgl0zd2_l;  
	unsigned char	tgl0zd2_h; 
	
	unsigned char	izds3_l;  
	unsigned char	izds3_h; 
	unsigned char	tglzd3_l;  
	unsigned char	tglzd3_h; 
	unsigned char	igfhzds3_l;  
	unsigned char	igfhzds3_h;  
	unsigned char	tgfhzd3_l;  
	unsigned char	tgfhzd3_h; 
	unsigned char	i0zds3_l;  
	unsigned char	i0zds3_h;  
	unsigned char	tgl0zd3_l;  
	unsigned char	tgl0zd3_h; 
	
	unsigned char	izds4_l;  
	unsigned char	izds4_h; 
	unsigned char	tglzd4_l;  
	unsigned char	tglzd4_h; 
	unsigned char	igfhzds4_l;  
	unsigned char	igfhzds4_h;  
	unsigned char	tgfhzd4_l;  
	unsigned char	tgfhzd4_h; 
	unsigned char	i0zds4_l;  
	unsigned char	i0zds4_h;  
	unsigned char	tgl0zd4_l;  
	unsigned char	tgl0zd4_h; 
	
	unsigned char	izds5_l;  
	unsigned char	izds5_h; 
	unsigned char	tglzd5_l;  
	unsigned char	tglzd5_h; 
	unsigned char	igfhzds5_l;  
	unsigned char	igfhzds5_h;  
	unsigned char	tgfhzd5_l;  
	unsigned char	tgfhzd5_h; 
	unsigned char	i0zds5_l;  
	unsigned char	i0zds5_h;  
	unsigned char	tgl0zd5_l;  
	unsigned char	tgl0zd5_h; 
	
	unsigned char	izds6_l;  
	unsigned char	izds6_h; 
	unsigned char	tglzd6_l;  
	unsigned char	tglzd6_h; 
	unsigned char	igfhzds6_l;  
	unsigned char	igfhzds6_h;  
	unsigned char	tgfhzd6_l;  
	unsigned char	tgfhzd6_h; 
	unsigned char	i0zds6_l;  
	unsigned char	i0zds6_h;  
	unsigned char	tgl0zd6_l;  
	unsigned char	tgl0zd6_h; 
////////////////////////////////////
	unsigned char	izds7_l;  
	unsigned char	izds7_h; 
	unsigned char	tglzd7_l;  
	unsigned char	tglzd7_h; 
	unsigned char	igfhzds7_l;  
	unsigned char	igfhzds7_h;  
	unsigned char	tgfhzd7_l;  
	unsigned char	tgfhzd7_h; 
	unsigned char	i0zds7_l;  
	unsigned char	i0zds7_h;  
	unsigned char	tgl0zd7_l;  
	unsigned char	tgl0zd7_h; 
	
	unsigned char	izds8_l;  
	unsigned char	izds8_h; 
	unsigned char	tglzd8_l;  
	unsigned char	tglzd8_h; 
	unsigned char	igfhzds8_l;  
	unsigned char	igfhzds8_h;  
	unsigned char	tgfhzd8_l;  
	unsigned char	tgfhzd8_h; 
	unsigned char	i0zds8_l;  
	unsigned char	i0zds8_h;  
	unsigned char	tgl0zd8_l;  
	unsigned char	tgl0zd8_h; 
	
	unsigned char	izds9_l;  
	unsigned char	izds9_h; 
	unsigned char	tglzd9_l;  
	unsigned char	tglzd9_h; 
	unsigned char	igfhzds9_l;  
	unsigned char	igfhzds9_h;  
	unsigned char	tgfhzd9_l;  
	unsigned char	tgfhzd9_h; 
	unsigned char	i0zds9_l;  
	unsigned char	i0zds9_h;  
	unsigned char	tgl0zd9_l;  
	unsigned char	tgl0zd9_h; 
	
	unsigned char	izds10_l;  
	unsigned char	izds10_h; 
	unsigned char	tglzd10_l;  
	unsigned char	tglzd10_h; 
	unsigned char	igfhzds10_l;  
	unsigned char	igfhzds10_h;  
	unsigned char	tgfhzd10_l;  
	unsigned char	tgfhzd10_h; 
	unsigned char	i0zds10_l;  
	unsigned char	i0zds10_h;  
	unsigned char	tgl0zd10_l;  
	unsigned char	tgl0zd10_h; 
	
	
	//////////////////////////////
	unsigned char	izds11_l;  
	unsigned char	izds11_h; 
	unsigned char	tglzd11_l;  
	unsigned char	tglzd11_h; 
	unsigned char	igfhzds11_l;  
	unsigned char	igfhzds11_h;  
	unsigned char	tgfhzd11_l;  
	unsigned char	tgfhzd11_h; 
	unsigned char	i0zds11_l;  
	unsigned char	i0zds11_h;  
	unsigned char	tgl0zd11_l;  
	unsigned char	tgl0zd11_h; 
	
	unsigned char	izds12_l;  
	unsigned char	izds12_h; 
	unsigned char	tglzd12_l;  
	unsigned char	tglzd12_h; 
	unsigned char	igfhzds12_l;  
	unsigned char	igfhzds12_h;  
	unsigned char	tgfhzd12_l;  
	unsigned char	tgfhzd12_h; 
	unsigned char	i0zds12_l;  
	unsigned char	i0zds12_h;  
	unsigned char	tgl0zd12_l;  
	unsigned char	tgl0zd12_h; 
	
	unsigned char	izds13_l;  
	unsigned char	izds13_h; 
	unsigned char	tglzd13_l;  
	unsigned char	tglzd13_h; 
	unsigned char	igfhzds13_l;  
	unsigned char	igfhzds13_h;  
	unsigned char	tgfhzd13_l;  
	unsigned char	tgfhzd13_h; 
	unsigned char	i0zds13_l;  
	unsigned char	i0zds13_h;  
	unsigned char	tgl0zd13_l;  
	unsigned char	tgl0zd13_h; 
	
	unsigned char	izds14_l;  
	unsigned char	izds14_h; 
	unsigned char	tglzd14_l;  
	unsigned char	tglzd14_h; 
	unsigned char	igfhzds14_l;  
	unsigned char	igfhzds14_h;  
	unsigned char	tgfhzd14_l;  
	unsigned char	tgfhzd14_h; 
	unsigned char	i0zds14_l;  
	unsigned char	i0zds14_h;  
	unsigned char	tgl0zd14_l;  
	unsigned char	tgl0zd14_h; 
	
	unsigned char	izds15_l;  
	unsigned char	izds15_h; 
	unsigned char	tglzd15_l;  
	unsigned char	tglzd15_h; 
	unsigned char	igfhzds15_l;  
	unsigned char	igfhzds15_h;  
	unsigned char	tgfhzd15_l;  
	unsigned char	tgfhzd15_h; 
	unsigned char	i0zds15_l;  
	unsigned char	i0zds15_h;  
	unsigned char	tgl0zd15_l;  
	unsigned char	tgl0zd15_h; 
	
	unsigned char	izds16_l;  
	unsigned char	izds16_h; 
	unsigned char	tglzd16_l;  
	unsigned char	tglzd16_h; 
	unsigned char	igfhzds16_l;  
	unsigned char	igfhzds16_h;  
	unsigned char	tgfhzd16_l;  
	unsigned char	tgfhzd16_h; 
	unsigned char	i0zds16_l;  
	unsigned char	i0zds16_h;  
	unsigned char	tgl0zd16_l;  
	unsigned char	tgl0zd16_h; 
	//////////////////
	unsigned char tbhdzfg_l;
	unsigned char tbhdzfg_h;

	unsigned char tchz_l;
	unsigned char tchz_h;
	
	unsigned char  bhMode1_l;
	unsigned char  bhMode1_h;
	unsigned char  bhMode2_l;
	unsigned char  bhMode2_h;
	unsigned char  bhMode3_l;
	unsigned char  bhMode3_h;
	unsigned char  bhMode4_l;
	unsigned char  bhMode4_h;
	unsigned char  bhMode5_l;
	unsigned char  bhMode5_h;
	unsigned char	sum;	
	unsigned char	commSum;
}tYkpz5700;
typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	izds1;  
		unsigned short	tglzd1;  
		unsigned short	igfhzds1;  
		unsigned short	tgfhzd1;  
		unsigned short	i0zds1;  
		unsigned short	tgl0zd1;  
		
		unsigned short	izds2;  
		unsigned short	tglzd2;  
		unsigned short	igfhzds2;  
		unsigned short	tgfhzd2;  
		unsigned short	i0zds2;  
		unsigned short	tgl0zd2;  
		
		unsigned short	izds3;  
		unsigned short	tglzd3;  
		unsigned short	igfhzds3;  
		unsigned short	tgfhzd3;  
		unsigned short	i0zds3;  
		unsigned short	tgl0zd3;  
		
		unsigned short	izds4;  
		unsigned short	tglzd4;  
		unsigned short	igfhzds4;  
		unsigned short	tgfhzd4;  
		unsigned short	i0zds4;  
		unsigned short	tgl0zd4;  
		
		unsigned short	izds5;  
		unsigned short	tglzd5;  
		unsigned short	igfhzds5;  
		unsigned short	tgfhzd5;  
		unsigned short	i0zds5;  
		unsigned short	tgl0zd5;  
		
		unsigned short	izds6;  
		unsigned short	tglzd6;  
		unsigned short	igfhzds6;  
		unsigned short	tgfhzd6;  
		unsigned short	i0zds6;  
		unsigned short	tgl0zd6;  
		/////////////////////////////////
		unsigned short	izds7;  
		unsigned short	tglzd7;  
		unsigned short	igfhzds7;  
		unsigned short	tgfhzd7;  
		unsigned short	i0zds7;  
		unsigned short	tgl0zd7;  
		
		unsigned short	izds8;  
		unsigned short	tglzd8;  
		unsigned short	igfhzds8;  
		unsigned short	tgfhzd8;  
		unsigned short	i0zds8;  
		unsigned short	tgl0zd8;  
		
		unsigned short	izds9;  
		unsigned short	tglzd9;  
		unsigned short	igfhzds9;  
		unsigned short	tgfhzd9;  
		unsigned short	i0zds9;  
		unsigned short	tgl0zd9;  
		
		unsigned short	izds10;  
		unsigned short	tglzd10;  
		unsigned short	igfhzds10;  
		unsigned short	tgfhzd10;  
		unsigned short	i0zds10;  
		unsigned short	tgl0zd10;  
		
	
		//////////////////////////
		unsigned short	izds11;  
		unsigned short	tglzd11;  
		unsigned short	igfhzds11;  
		unsigned short	tgfhzd11;  
		unsigned short	i0zds11;  
		unsigned short	tgl0zd11;  
		
		unsigned short	izds12;  
		unsigned short	tglzd12;  
		unsigned short	igfhzds12;  
		unsigned short	tgfhzd12;  
		unsigned short	i0zds12;  
		unsigned short	tgl0zd12;  
		
		unsigned short	izds13;  
		unsigned short	tglzd13;  
		unsigned short	igfhzds13;  
		unsigned short	tgfhzd13;  
		unsigned short	i0zds13;  
		unsigned short	tgl0zd13;  
		
		unsigned short	izds14;  
		unsigned short	tglzd14;  
		unsigned short	igfhzds14;  
		unsigned short	tgfhzd14;  
		unsigned short	i0zds14;  
		unsigned short	tgl0zd14;  
		unsigned short	izds15;  
		unsigned short	tglzd15;  
		unsigned short	igfhzds15;  
		unsigned short	tgfhzd15;  
		unsigned short	i0zds15;  
		unsigned short	tgl0zd15;  
		
	
		unsigned short	izds16;  
		unsigned short	tglzd16;  
		unsigned short	igfhzds16;  
		unsigned short	tgfhzd16;  
		unsigned short	i0zds16;  
		unsigned short	tgl0zd16;  
		
		
		unsigned short        tbhdzfg;
			unsigned short        tchz;
		unsigned short       bhMode1;
		unsigned short       bhMode2;
		unsigned short       bhMode3;
		unsigned short       bhMode4;
		unsigned short       bhMode5;
		unsigned short   bhMode[2];
		
	}param;
}tYkpz5700Data;
typedef struct
{
	unsigned char	i1Chan_l;
	unsigned char	i1Chan_h;
	
	unsigned char	i2Chan_l;
	unsigned char	i2Chan_h;
	
	unsigned char	i3Chan_l;
	unsigned char	i3Chan_h;
	
	unsigned char	i4Chan_l;
	unsigned char	i4Chan_h;
	
	unsigned char	i5Chan_l;
	unsigned char	i5Chan_h;
	
	unsigned char	i6Chan_l;
	unsigned char	i6Chan_h;
	
		unsigned char	i7Chan_l;
	unsigned char	i7Chan_h;
	
	  unsigned char	i8Chan_l;
	  unsigned char	i8Chan_h;
	  
		unsigned char	i9Chan_l;
		unsigned char	i9Chan_h;
		
		  unsigned char	i10Chan_l;
		  unsigned char	i10Chan_h;
		  
			unsigned char	i11Chan_l;
			unsigned char	i11Chan_h;
			
			  
				unsigned char	i12Chan_l;
				unsigned char	i12Chan_h;
				
				  unsigned char	i13Chan_l;
				  unsigned char	i13Chan_h;
				  
					unsigned char	i14Chan_l;
					unsigned char	i14Chan_h;
					
					  unsigned char	i15Chan_l;
					  unsigned char	i15Chan_h;
					  
						unsigned char	i16Chan_l;
						unsigned char	i16Chan_h;
						
						  /*unsigned char	i17Chan_l;
						  unsigned char	i17Chan_h;
						  
							unsigned char	i18Chan_l;
							unsigned char	i18Chan_h;
							
							  unsigned char	i19Chan_l;
							  unsigned char	i19Chan_h;
							  
								unsigned char	i20Chan_l;
								unsigned char	i20Chan_h;
								
								  unsigned char	i21Chan_l;
								  unsigned char	i21Chan_h;
								  
									unsigned char	i22Chan_l;
	unsigned char	i22Chan_h;*/
	
	
	unsigned char	sum;
	unsigned char	commSum;
}tYcpz5771;

typedef struct
{
	unsigned char  data_l;
	unsigned char  data_h;
	unsigned char  sum;
}tMa420;

typedef struct
{
	unsigned short	uChan;
	unsigned short	iChan;
}tOneChanCfg;
typedef union
{
	unsigned short	data[DZ_LEN];
	struct
	{
		unsigned short	i1Chan;   
		unsigned short	i2Chan;   
		unsigned short	i3Chan;   
		unsigned short	i4Chan;   
		unsigned short	i5Chan;   
		unsigned short	i6Chan;  
				unsigned short	i7Chan;  
		unsigned short	i8Chan;  
		unsigned short	i9Chan;  
		unsigned short	i10Chan;  
		unsigned short	i11Chan;  
		unsigned short	i12Chan;  
		unsigned short	i13Chan;   
		unsigned short	i14Chan;   
		unsigned short	i15Chan;   
		unsigned short	i16Chan;  
		/*unsigned short	i17Chan;  
		unsigned short	i18Chan;  
		unsigned short	i19Chan;  
		unsigned short	i20Chan;  
		unsigned short	i21Chan;  
		unsigned short	i22Chan; */
	}param;
}tYcpz5700Data;

typedef struct
{
	tYkpz5700	cs;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tYkpzSet;
typedef struct
{
	tYcpz5771	cs;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tYcpzSet;

typedef struct
{
	tMa420	cs;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tMa420Set;

typedef struct
{
	short   DdcsData;
	unsigned char    complete; 
}tDdcs5700Data;

#define MAX_EDIT_BUFFER 256

typedef struct
{
	tZzcsSet	zzcsRd;
	tJdcsSet	jdcsRd;
	tYxcsSet	yxcsRd;
	tDwcsSet	dwcsRd;
	tTqcsSet	tqcsRd;
	tDccsSet	dccsRd;
	tDcjdSet	dcjdRd;
	tYkpzSet	ykpzRd;	/*ң������*/
	tYcpzSet	ycpzRd;
	tMa420Set    ma420dataRd;
	tDdcsSet	ddcsRd;
	tCktxcsSet	cktxcsRd;
	
	tZzcsSet	zzcsWr;
	tJdcsSet	jdcsWr;
	tYxcsSet	yxcsWr;
	tTqcsSet	tqcsWr;
	tDwcsSet	dwcsWr;
	tDccsSet	dccsWr;
	tDcjdSet	dcjdWr;
	tYkpzSet	ykpzWr;
	tYcpzSet	ycpzWr;
	tDdcsSet	ddcsWr;
	tCktxcsSet	cktxcsWr;
	
	tZzcs5700Data	zzcs;
	tYkpz5700Data	ykpz;		/*ң�����ò���*/
	tYcpz5700Data	ycpz;		/*ң�����ò���*/
	tJdcs5700Data	jdcs;
	tJdtz5700Data	jdtz;
	tYxcs5700Data	yxcs;
	tDwcs5700Data	dwcs;
	tTqcs5700Data	tqcs;
	tDccs5700Data	dccs;		/*ֱ������*/
	tCktxcs5700Data	cktxcs;
	tDctf		dctf;		/*ֱ�����ȵ���*/
	tDcjd5700Data	dcjd;		/*ֱ�����Ȳ���*/
	tDeltYccs	deltYccs;	/*ң��Խ�����Ͷ�ֵ,����װ�ò����趨,����,��ѹ0.2%,Ƶ��0.01Hz,����0.5%*/
	tDeltBhcs5771   deltBhcs;
	tParamSet	xtcs;
	tParamSet	txcs;
	tRelaySet 	dzBuf;
	tSoftYb		softYb;		/*��ѹ�嶨ֵ*/		
	tCrc		crc;
	tVersion	version;
	tDdcs5700Data      ddcs;
	
	
	unsigned char	hdbstr;			/*Ӳ������Ͷ��*/
	unsigned short	data[MAX_EDIT_BUFFER];		/*����LCD���ò����Ĺ�������*/
	unsigned long	edit_data[MAX_EDIT_BUFFER];	/*����LCD���ò����Ĺ�������*/

	unsigned char 	arm_mac_addr[NET_BYTES];
	unsigned char 	dsp_mac_addr[NET_BYTES];
	unsigned char 	para_bs;		/*װ�ó�ʼ��ΪTRUE,��װ�ý�����DSP��ŵĸ��ֲ���,��ΪFALSE����Һ����װ�ò��������޸Ļ��̨�Բ��������޸ģ���ΪTRUE*/
	unsigned long	bsDispTimer;
	unsigned char	relay_bs;
	
	unsigned short	gooseAddr;		/*��xsl��mac e2prom������ֵ*/
	unsigned short	addr;			/*��xsl��mac e2prom������ֵ*/
	unsigned short	ipA1;			/*��xsl��mac e2prom������ֵ*/
	unsigned short	ipA2;			/*��xsl��mac e2prom������ֵ*/
	unsigned short	ipB1;			/*��xsl��mac e2prom������ֵ*/
	unsigned short	ipB2;			/*��xsl��mac e2prom������ֵ*/
	
	unsigned char   arm_write_txpara;
	unsigned char   arm_write_relay;
	unsigned char   arm_write_soft;
}tDevParam;

typedef struct
{
	unsigned char	readyForSend;
	unsigned char	quality;		/*Ʒ��*/
	unsigned char	dataChg;		/*ң������Խ��5%*/
	
	unsigned long	sendTimer;
	unsigned short 	phase_uaia;
	unsigned short 	phase_ubib;
	unsigned short 	phase_ucic;
	unsigned short 	phase_uaub;
	unsigned short 	phase_ubuc;
	unsigned short 	phase_ucua;
	unsigned short 	phase_uxua;
	unsigned short 	phase_u0i0;
	unsigned short 	phase_iaib;
	unsigned short 	phase_ibic;
	unsigned short 	phase_icia;
	unsigned short 	phase_uaiam;
	unsigned short 	phase_ucicm;
	
	unsigned short 	iaabs;
	unsigned short 	ibabs;
	unsigned short 	icabs;
	unsigned short 	i0abs;
	unsigned short 	uababs;		/*������ѹ�б�*/
	unsigned short 	u0abs;		/**/
	unsigned short 	u1abs;
	unsigned short 	u2abs;		/**/
	unsigned short	uxabs;
	unsigned short	ux;
	unsigned short	u0;		/*���������ѹֱ�Ӳ���*/
	
	unsigned long 	fabs;
	unsigned long 	fabs_soft;	
	unsigned long 	fre_sum;
	unsigned long 	fre_sum_soft;
	unsigned short 	dfdt;
	unsigned long 	fre_count;
	unsigned long 	fre_bas[6];
	unsigned long 	fre_bas_x[6];
	unsigned short 	imax;
	unsigned short 	upmax;
	unsigned short 	uppmin;
	unsigned short	cnt;
	unsigned short	num;
	unsigned short	err;
}tYccs;

typedef struct
{			
	union
	{
		tJdtz5771Data       d5771;
		unsigned short	data[DZ_LEN];           
	}data;
}tJdtzSet;

typedef struct
{
	unsigned long 	clientId;
	unsigned long	ctlVal;
	unsigned char	no;		/*ң�غ�*/
	unsigned char	type;		/*SEL,EXE,CAN*/
	unsigned char	reason;
}tOrder;

typedef struct
{
	unsigned short  gin;
	unsigned char 	armSelect;
	unsigned char 	armCancel;
	unsigned char 	armExecute;
}tJdstData;		/*�ӵ�����*/
typedef struct
{
	unsigned char   timer_buf[8];				/*��ǰʱ��ֵ*/
	unsigned char	modiTimer[8];				/*ʱ�仺��,���������޸Ļ��ֶ��޸ĵ�ʱ��ֵ*/
	unsigned char 	hour;
	unsigned char   wrDspLocal;
	unsigned char   wrDspRmt;
	unsigned char	timeFlag;				/*ʱ�ӱ�ʶ�����Ƿ���Ӳ����ʱ*/
	unsigned long	rcvNetTimer;				/*���յ������ʱ���Ķ�ʱ����Ӧ���˼��1s���յ��Ķ�ʱ����*/
	unsigned long	rcvDspTime;				/*��¼���յ�DSPʱ��ʱARM��sysMSecond,���ڲ���ARM���ms*/
	unsigned long	ms_timer;
	
}tTimeData;

typedef struct
{
	unsigned short   mA420Data;
	unsigned char    select_flag;
	unsigned char    execute_flag;
	unsigned short    select_val;
	unsigned short    execute_val;
	unsigned long     time;
	unsigned char    complete; 
}tMA420Data;

typedef struct
{
	unsigned char	ua_l;
	unsigned char	ua_h;
	unsigned char	ub_l;
	unsigned char	ub_h;
	unsigned char	uc_l;
	unsigned char	uc_h;
	unsigned char	uab_l;
	unsigned char	uab_h;
	unsigned char	ubc_l;
	unsigned char	ubc_h;
	unsigned char	uca_l;
	unsigned char	uca_h;
	unsigned char	u0_l;
	unsigned char	u0_h;
	/*	unsigned char	f_l;
	unsigned char	f_h;*/
	unsigned char	ia_l;
	unsigned char	ia_h;
	unsigned char	ib_l;
	unsigned char	ib_h;
	unsigned char	ic_l;
	unsigned char	ic_h;
	unsigned char	i0_l;
	unsigned char	i0_h;
	unsigned char	pam_l;
	unsigned char	pam_h;
	unsigned char	sam_l;
	unsigned char	sam_h;
	unsigned char	qam_l;
	unsigned char	qam_h;
	unsigned char	pbm_l;
	unsigned char	pbm_h;
	unsigned char	sbm_l;
	unsigned char	sbm_h;
	unsigned char	qbm_l;
	unsigned char	qbm_h;
	unsigned char	pcm_l;
	unsigned char	pcm_h;
	unsigned char	scm_l;
	unsigned char	scm_h;
	unsigned char	qcm_l;
	unsigned char	qcm_h;
	unsigned char	pa_l;
	unsigned char	pa_h;
	unsigned char	sa_l;
	unsigned char	sa_h;
	unsigned char	cosa_l;
	unsigned char	cosa_h;
	unsigned char	qa_l;
	unsigned char	qa_h;
	unsigned char	pb_l;
	unsigned char	pb_h;
	unsigned char	sb_l;
	unsigned char	sb_h;
	unsigned char	cosb_l;
	unsigned char	cosb_h;
	unsigned char	qb_l;
	unsigned char	qb_h;
	unsigned char	pc_l;
	unsigned char	pc_h;
	unsigned char	sc_l;
	unsigned char	sc_h;
	unsigned char	cosc_l;
	unsigned char	cosc_h;
	unsigned char	qc_l;
	unsigned char	qc_h;
	unsigned char	p_l;
	unsigned char	p_h;
	unsigned char	s_l;
	unsigned char	s_h;
	unsigned char	cosq_l;
	unsigned char	cosq_h;
	unsigned char	q_l;
	unsigned char	q_h;
	unsigned char xb11_l;
	unsigned char xb11_h;
	unsigned char xb13_l;
	unsigned char xb13_h;
	unsigned char xb15_l;
	unsigned char xb15_h;
	unsigned char xb17_l;
	unsigned char xb17_h;
	unsigned char xb19_l;
	unsigned char xb19_h;
	unsigned char xb1b_l;
	unsigned char xb1b_h;
	unsigned char xb1d_l;
	unsigned char xb1d_h;
	unsigned char xb1f_l;
	unsigned char xb1f_h;
	unsigned char xb21_l;
	unsigned char xb21_h;
	unsigned char xb23_l;
	unsigned char xb23_h;
	unsigned char xb25_l;
	unsigned char xb25_h;
	unsigned char xb27_l;
	unsigned char xb27_h;
	unsigned char xb29_l;
	unsigned char xb29_h;
	unsigned char xb2b_l;
	unsigned char xb2b_h;
	unsigned char xb2d_l;
	unsigned char xb2d_h;
	unsigned char xb2f_l;
	unsigned char xb2f_h;
	unsigned char xb31_l;
	unsigned char xb31_h;
	unsigned char xb33_l;
	unsigned char xb33_h;
	unsigned char xb35_l;
	unsigned char xb35_h;
	unsigned char xb37_l;
	unsigned char xb37_h;
	unsigned char xb39_l;
	unsigned char xb39_h;
	unsigned char xb3b_l;
	unsigned char xb3b_h;
	unsigned char xb3d_l;
	unsigned char xb3d_h;
	unsigned char xb3f_l;
	unsigned char xb3f_h;
}tOneYc;
typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char	  quality;
	
	tOneYc    l1;
	tOneYc    l2;
	tOneYc    l3;
	tOneYc    l4;
	tOneYc    l5;
	tOneYc    l6;
	tOneYc    l7;
	tOneYc    l8;
		tOneYc    l9;
		tOneYc    l10;
		tOneYc    l11;
		tOneYc    l12;
		tOneYc    l13;
			tOneYc    l14;
			tOneYc    l15;
			tOneYc    l16;
			
	unsigned char	f1_l;
	unsigned char	f1_h;
	unsigned char	f2_l;
	unsigned char	f2_h;
	
	unsigned char	f3_l;
		unsigned char	f3_h;
		unsigned char	f4_l;
		unsigned char	f4_h;
	unsigned char	phaseU1aU2a_l;
	unsigned char	phaseU1aU2a_h;
	
	unsigned char	zl1m_l;
	unsigned char	zl1m_h;
	unsigned char	zl2m_l;
	unsigned char	zl2m_h;
	unsigned char	zl3m_l;
	unsigned char	zl3m_h;
	unsigned char	zl4m_l;
	unsigned char	zl4m_h;
	
	unsigned char    ground_i5abs1_l;
	unsigned char    ground_i5abs1_h;
	unsigned char    ground_i5abs2_l;
	unsigned char    ground_i5abs2_h;
	unsigned char    ground_i5abs3_l;
	unsigned char    ground_i5abs3_h;
	unsigned char    ground_i5abs4_l;
	unsigned char    ground_i5abs4_h;
	unsigned char    ground_i5abs5_l;
	unsigned char    ground_i5abs5_h;
	unsigned char    ground_i5abs6_l;
	unsigned char    ground_i5abs6_h;
	unsigned char	sum;
}tYc5771;

typedef  struct
{
	union
	{
		tYc5771		d5771;
	}yc;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
	
}tJkyc;

typedef struct
{     
	long	ua;
	long	ub;
	long	uc;
	long	uab;
	long	ubc;
	long	uca;
	long	u0;
	/*	long	f;*/
	long	ia;
	long	ib;
	long	ic;
	long	i0;
	long	pam;
	long	sam;
	long	qam;
	long	pbm;
	long	sbm;
	long	qbm;
	long	pcm;
	long	scm;
	long	qcm;
	long	pa;
	long	sa;
	long	cosa;
	long	qa;
	long	pb;
	long	sb;
	long	cosb;
	long	qb;
	long	pc;
	long	sc;
	long	cosc;
	long	qc;
	long	p;
	long	s;
	long	cosq;
	long	q;
}tOneLineYcData;

typedef struct
{     
	unsigned short	ua;
	unsigned short	ub;
	unsigned short	uc;
	unsigned short	uab;
	unsigned short	ubc;
	unsigned short	uca;
	unsigned short	u0;
	unsigned short	ia;
	unsigned short	ib;
	unsigned short	ic;
	unsigned short	i0;
	unsigned short	pam;
	unsigned short	sam;
	unsigned short	qam;
	unsigned short	pbm;
	unsigned short	sbm;
	unsigned short	qbm;
	unsigned short	pcm;
	unsigned short	scm;
	unsigned short	qcm;
	unsigned short	pa;
	unsigned short	sa;
	unsigned short	cosa;
	unsigned short	qa;
	unsigned short	pb;
	unsigned short	sb;
	unsigned short	cosb;
	unsigned short	qb;
	unsigned short	pc;
	unsigned short	sc;
	unsigned short	cosc;
	unsigned short	qc;
	unsigned short	p;
	unsigned short	s;
	unsigned short	cosq;	
	unsigned short	q;
}tOneLineYcPzData;

typedef struct
{
	
	tOneLineYcData	l1;
	tOneLineYcData	l2;
	tOneLineYcData	l3;
	tOneLineYcData	l4;
	tOneLineYcData	l5;
	tOneLineYcData	l6;
	
	tOneLineYcData	l7;
		tOneLineYcData	l8;
		tOneLineYcData	l9;
		tOneLineYcData	l10;
		tOneLineYcData	l11;
		tOneLineYcData	l12;
		tOneLineYcData	l13;
			tOneLineYcData	l14;
			tOneLineYcData	l15;
			tOneLineYcData	l16;
			
	long	f1;
	long	f2;
	long	f3;
		long	f4;
	long phaseU1aU2a;
	long	zlm[4];
	long fifthxb[6];
}tYc5771Val;

typedef struct
{
	
	tOneLineYcPzData	l1;
	tOneLineYcPzData	l2;
	tOneLineYcPzData	l3;
	tOneLineYcPzData	l4;
	tOneLineYcPzData	l5;
	tOneLineYcPzData	l6;
	tOneLineYcPzData	l7;
		tOneLineYcPzData	l8;
		tOneLineYcPzData	l9;
		tOneLineYcPzData	l10;
		tOneLineYcPzData	l11;
		tOneLineYcPzData	l12;
		tOneLineYcPzData	l13;
			tOneLineYcPzData	l14;
			tOneLineYcPzData	l15;
			tOneLineYcPzData	l16;
			
	unsigned short	f1;
	unsigned short	f2;
	unsigned short	f3;
	unsigned short	f4;
	unsigned short       phaseU1aU2a;
	unsigned short	zlm[4];
	unsigned short       fifthxb[6];
}tYc5771PzVal;

typedef struct
{
	unsigned char 	year;	
	unsigned char 	month;
	unsigned char 	date;	
	unsigned char 	hour;	
	unsigned char 	minu;	
	unsigned char 	secd;	
	unsigned char 	ms_l;
	unsigned char 	ms_h;
	unsigned char	quality;
	unsigned char	deltFlag;	/*ң��仯��־*/
	union
	{
		unsigned long	data[MAX_YC_NUM];	/*���ݵĵ�1λ��ʾ�Ƿ�deltˢ�£�1��ˢ�£�0��δˢ��*/
		tYc5771Val	instVal;
	}dd;
	union
	{
		unsigned short	YcQuality[MAX_YC_NUM];	/*���ݵĵ�1λ��ʾ�Ƿ�deltˢ�£�1��ˢ�£�0��δˢ��*/
		tYc5771PzVal	       instVal;
	}pz;
	tYc5771Val	val;
	tYc5771Val	valm;	/*δ�������ȵ���ֵ*/
}tYc5771Data;

typedef  union
{
	tYc5771Data		d5771;
}tSendYc;
typedef struct
{
	short		xb11;
	short		xb13;
	short		xb15;
	short		xb17;
	short		xb19;
	short		xb1b;
	short		xb1d;
	short		xb1f;
	short		xb21;
	short		xb23;
	short		xb25;
	short		xb27;
	short		xb29;
	short		xb2b;
	short		xb2d;
	short		xb2f;
}tXb5700Val;

typedef struct
{
	unsigned short xb1;
	unsigned short xb3;
	unsigned short xb5;
	unsigned short xb7;
	unsigned short xb9;
	unsigned short xbb;
	unsigned short xbd;
	unsigned short xbf;
}tOnePhaseXbData;

typedef struct
{
	short		xb11;
	short		xb13;
	short		xb15;
	short		xb17;
	short		xb19;
	short		xb1b;
	short		xb1d;
	short		xb1f;
	short		xb21;
	short		xb23;
	short		xb25;
	short		xb27;
	short		xb29;
	short		xb2b;
	short		xb2d;
	short		xb2f;
	short		xb31;
	short		xb33;
	short		xb35;
	short		xb37;
	short		xb39;
	short		xb3b;
	short		xb3d;
	short		xb3f;
}tOneLineXb5771Val;

typedef struct
{
	tOneLineXb5771Val  l1;
	tOneLineXb5771Val  l2;
	tOneLineXb5771Val  l3;
	tOneLineXb5771Val  l4;
	tOneLineXb5771Val  l5;
	tOneLineXb5771Val  l6;
	tOneLineXb5771Val  l7;
		tOneLineXb5771Val  l8;
		tOneLineXb5771Val  l9;
		tOneLineXb5771Val  l10;
		tOneLineXb5771Val  l11;
		tOneLineXb5771Val  l12;
		
		tOneLineXb5771Val  l13;
			tOneLineXb5771Val  l14;
			tOneLineXb5771Val  l15;
			tOneLineXb5771Val  l16;
}tXb5771Val;

typedef union
{
	unsigned short	data[32];
	tXb5700Val	val;
	tXb5771Val	val5771;
}tXb5700Data;
typedef struct
{	
	unsigned char    ground_i5abs_l;
	unsigned char    ground_i5abs_h;
	unsigned char    ground_5dir_l;
	unsigned char    ground_5dir_h;
}tOneFifthxb;

typedef struct
{	
	tOneFifthxb l1;
	tOneFifthxb l2;
	tOneFifthxb l3;
	tOneFifthxb l4;
	tOneFifthxb l5;
	tOneFifthxb l6; 
	tOneFifthxb l7;
		tOneFifthxb l8;
		tOneFifthxb l9;
		tOneFifthxb l10;
		tOneFifthxb l11;
		tOneFifthxb l12; 
		tOneFifthxb l13;
			tOneFifthxb l14;
			tOneFifthxb l15;
			tOneFifthxb l16;
		
	unsigned char     sum;
}tFifthxb;

typedef	struct
{
	tFifthxb   recv;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
}tJdxx;
typedef	struct
{
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
}tPha;
typedef struct
{
	unsigned char setFlag;
	unsigned char	setSerno;
	unsigned char	setType;
	unsigned char clrFlag;
	unsigned char	clrSerno;
	unsigned char	clrType;
}tYxzw;
typedef struct
{
	unsigned char	soebptr;
	unsigned char	soenum_l;
	unsigned char	soenum_h;
	unsigned char	trbptr;
	unsigned char	trnum;
	unsigned char	tsbptr;
	unsigned char	tsnum;
	unsigned char	opbptr;
	unsigned char	opnum_l;
	unsigned char	opnum_h;
	unsigned char	grpbptr;
	unsigned char	grpnum;
	unsigned char	zjbptr;
	unsigned char	zjnum;
}tXxjbFrame;
typedef struct
{
	tXxjbFrame	data;
	unsigned long	rdTimer;	/*��DSP��ʱ*/
	unsigned char	rdPc;
	unsigned char	complete;
}tXxjb;				/*װ����Ϣ��*/
typedef struct
{
	unsigned char 	recType;/*��������*/
	unsigned char	no;	/*ȷ�ϵ�ַ*/
}tRecAck;
typedef struct
{
	unsigned char	curDw;		/*�ܵ�λ*/
	tTimeForm	curTime;
	unsigned char	curDw1;		/*1�൵λ*/
	unsigned char	curDw2;		/*2�൵λ*/
	unsigned char	curDw3;		/*3�൵λ*/
	/*	unsigned char	oldDw;*/
	unsigned long	dwyx;
	unsigned long	phsdwTimer;	/*���൵λ��ʱ��*/
	unsigned long	hdctlTimer;	/*��λ���ڶ�ʱ��,��ʱ����,�������ƽ���*/
	unsigned char	mask[4];	/*��λ����*/
	unsigned char	tempDw;
	unsigned char	tapDisOrd;	/*���಻һ�±�־*/
	unsigned char	opResult;
	unsigned char	tapChgCnt;	/*��λ�仯����*/
	tTimeForm	tempTime;
	unsigned char	tempPosVal;
	unsigned char	oldPosVal;
	unsigned char	strtd;		/*�ѳ�ʼ��*/
}tSendDw;

typedef struct
{
	unsigned char  kxfh_char[96];
	unsigned char  sum;
}tkxfh_char;

typedef struct
{
	tkxfh_char     data5771;
	unsigned long	rdTimer;
	unsigned short	rcvPtr;
	unsigned char	complete;
	unsigned char	rdPc;
}tKxfh;


typedef struct
{
	unsigned short   data[12];
	unsigned short   data_bak[12];
	unsigned short   Quality[12];
	unsigned char    flag;
	unsigned long    timer;
	unsigned char    deltFlag;
}tJdxxData;

/*typedef struct
{
unsigned short    grond_i5abs;
unsigned short    grond_5dir;
}tOneFifthxbData;

  typedef struct
  {	 
  tOneFifthxbData l1;
  tOneFifthxbData l2;
  tOneFifthxbData l3;
  tOneFifthxbData l4;
  tOneFifthxbData l5;
  tOneFifthxbData l6;
}tFifthxbData;*/

typedef	struct 
{
	unsigned char	headEb;
	unsigned char	head90;
	unsigned char	len;		/*����(19*N+4)*/
	unsigned char	r21;		/*code*/
	unsigned char	frameNo;
	unsigned char	num;		/*��Ŀ*/
	unsigned char	bptr;		/*��ʼ��ַ*/
	unsigned char	total_l;
	unsigned char	total_h;
}tSoeHead;
typedef	struct 
{
	unsigned char	headEb;
	unsigned char	head90;
	unsigned char	len;		/*����(19*N+4)*/
	unsigned char	r26;		/*code*/
	unsigned char	frameNo;
	unsigned char	num;		/*��Ŀ*/
	unsigned char	bptr;		/*��ʼ��ַ*/
	unsigned char	total;
}tDwRecHead;
typedef struct
{
	unsigned char	no;		/*��ŵ�ַ*/
	unsigned char	serno;		/*����˳���*/
	unsigned char	year;
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	minute;
	unsigned char	second;
	unsigned char	msecond_l;
	unsigned char	msecond_h;
	unsigned char	ob;		/*��λ��*/
	unsigned char	newPos;
	unsigned char	oldPos;
	unsigned char	com_flag;
}tDwRecFr;

typedef	struct 
{
	unsigned char	headEb;
	unsigned char	head90;
	unsigned char	len;		/*����28*/
	unsigned char	r22;		/*code*/
	unsigned char	frameNo;
	unsigned char	bptr;		/*��ʼ��ַ*/
	unsigned char	total_l;
	unsigned char	total_h;
	unsigned char	no;
}tCmdHead;
typedef struct
{
	unsigned char	headEb;
	unsigned char	head90;
	unsigned char	len;		/*����28*/
	unsigned char	code;		/*code*/
	unsigned char	frameNo;
	unsigned char	data[1];	/*��ʼ��ַ*/
}tDspCommHd;

typedef struct
{
	unsigned short	stationAddr;
	unsigned short	devAddr;
	unsigned long	addr;
	unsigned short	len;
	unsigned char	sin;
	char		code;
	char		buf[512];
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
	unsigned char   bLcdCmd; /*Һ����������*/
}tMemRead;
typedef struct
{
	unsigned long	addr;
	unsigned short	len;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tMemWriteByte;
typedef struct
{
	unsigned long	addr;
	unsigned short	len;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tMemWriteWord;
typedef struct
{
	unsigned long	addr;
	unsigned short	len;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tMemWriteLong;
typedef struct
{
	unsigned long	addr;
	unsigned short	len;
	unsigned long	rdTimer;		/*����ʱ*/
	unsigned short	rcvPtr;
	unsigned char	rdPc;
	unsigned char	complete;		/*��ʼ����ɱ�־*/
}tWaveRead;
/****************down is not review by gjian****************************/
enum 	COMM_STATE_ENUM {	COMST_J,
COMST_A,COMST_B,COMST_C,
COMST_D,COMST_E,COMST_F,
COMST_G,COMST_H,COMST_I,
COMST_CLOSE,COMST_K,COMST_L,COMST_NULL};
/*��COMST_CLOSE������Close�����ת�Ƶ�COMST_NULL*/
enum FRN_ENUM{N_S,N_R};
enum DATA_TYPE_ENUM {NO_DATA,I_DATA,T_DATA,STRTDT_DATA,STPDT_DATA,S_DATA};
enum TCP_SEND_ID_ENUM{TCP_MAINTAIN,TCP_CLOSE_SOCKET,TCP_DATA};
typedef struct{
	unsigned short	no;
	unsigned long	data;
}temp_dz_type;


typedef struct
{
	unsigned char		dataType;
	unsigned char     	synHeadRev;
	unsigned short    	recv_count;
	unsigned short   	recv_bytes;
	unsigned long		socketId;		/*�ؼ��ֽ�id���͸���·��������*/
	unsigned long		clientIp;		/*�ؼ���*/
	unsigned char     	data[1];
}tSaveData;
typedef struct
{
	unsigned char	 head_flag;
	unsigned char	 length[2];
	unsigned char	 ctrl[4];
	unsigned char	 srcStationAddr[1];
	unsigned char	 srcDevAddr[2];
	unsigned char	 destStationAddr[1];
	unsigned char	 destDevAddr[2];
	unsigned char    resv[2];
	/*	unsigned char    resv[3];*/
}tApciPacket;
typedef struct
{
	unsigned long	sendType;
	unsigned long	socketId;
	unsigned long	clientIp;
}tTcpSendHead;
typedef	struct
{
	tTcpSendHead	head;
	tApciPacket	data;
}tTcpUSFr;
typedef struct 	
{
	unsigned short	datano;
	unsigned long	sndTimer;
	unsigned char	data[1];
}tSendFifoMsg;
typedef struct
{
	unsigned short	len;
	unsigned char	data[1];
}tAsduData;

typedef char c6_type[6];

typedef struct	
{
	unsigned int 		secs;      			/* Number of seconds since January 1, 1970	*/
	unsigned int 		usecs;  			/* Fraction of a second				*/
	unsigned int		qflags;				/* Quality flags, 8 least-significant bits only	*/
}tTimeStamp;
typedef struct
{
	unsigned char 	yy;
	unsigned char 	mm;
	unsigned char 	dd;
	unsigned char 	hh;
   	unsigned char 	minute;
	unsigned char 	second;				/*��*/
	unsigned char 	ms_h;				/*�����*/
	unsigned char 	ms_l;				/*�����*/
	tTimeStamp	t_utc;
}tTime7;
typedef struct
{
	unsigned short	len;
	unsigned char	data;
}tBVSTRING8;

union c_i
{
	unsigned short	i;
	char	c[2];
};
union c_l {
	unsigned long l;
	unsigned char c[4];
};
union i_l {
	unsigned long l;
	unsigned short i[2];
};
union c_s
{
	short   i;
	char	c[2];
};



typedef struct 
{
	unsigned short  gin; //ID
	unsigned char 	pjd; //����
	unsigned char 	plc; //����
	unsigned char 	plg; //����
	unsigned char 	status;
}tDataType;

typedef struct 
{
	unsigned char type;
	unsigned char num;
	unsigned short  ptr;    	/*point val��ָ��lascii_type����*/
}tDmsType;				/*������������*/

typedef struct 
{
	unsigned char type;
	unsigned char width;
	unsigned char num;
	unsigned char db;
	unsigned short ptr;    /*point val*/
	//begin wusenlin
	unsigned char nDataType; //��������0��ֵ��1ö��
	void*         pDataDef;  //������չ������������Ҫ����ö��ֵ
	//end wusenlin
}tDvalType;

typedef struct
{
	tDataType 	data;
	tDmsType  	dms;
	tDvalType 	dval;
}tGenTable;

typedef struct 
{
	unsigned char type;
	unsigned char width;
	unsigned char num;
	unsigned char p1;    /*point val */ //����λ��
	unsigned char p2;    /*point val */ //С��λ��
}tDjdType;

typedef struct 
{
	unsigned char type;
	unsigned char width;
	unsigned char num;
	unsigned short p1;    /*point val*/ //min
	unsigned short p2;    /*point val*/ //max
	unsigned short p3;    /*point val*/ //step
}tDlcType;

typedef struct 
{
	unsigned char type;
	unsigned char width;
	unsigned char num;
	unsigned char ptr;    /*point val*/
}tDlgType;
/***************for iec103********************/
typedef struct	
{
	unsigned char	flag;
	unsigned short	no;
}tLvl13;	/*���ڱ�ʶһ������/��������*/
typedef struct
{
	unsigned char	flag;
	unsigned short	addr;
	unsigned short	stationAddr;
	unsigned char	type;
	unsigned char	len;
	unsigned char	data[20];
}tAsdu5;
typedef struct 
{
	unsigned short  begin_add;
	unsigned short  length;
	unsigned char 	pre_link;
	unsigned char 	next_link;
}tFrameManagement;


typedef struct
{
	unsigned char	wy;	
	unsigned char	tq;
	unsigned char	wyBak;
	unsigned char	tqBak;
	unsigned long	wyTimer;
}tRsynCtl;	/*��ͬ����ص����ݽṹ*/
typedef struct	{
	unsigned char	no;
	unsigned short	i;
}funinf_type;


#define	NET103_CON_NUM         	16 //���������Ŀ
#define MAX_PER_DEV		        6		/*ÿ̨װ���������6������*/


typedef struct
{
	unsigned long	devAddr;
	unsigned long 	stationAddr;
	unsigned char	netAstate;
	unsigned char	netBstate;
}tOneComm;

#define	NET103_TCP_PORT         5000 
#define IEC104_TCP_PORT         2404

#define APCI_PACKET_LEN 	sizeof(tApciPacket)
typedef struct
{
	unsigned char	devName[20];
	unsigned char	t[20];
	unsigned char	version[20];
	unsigned char	code[20];
}tArmCrc;
typedef struct
{
	char	inf[8][20];
}tDevInf;
typedef struct
{
	unsigned short gin;
	unsigned short position;
	unsigned short type;
	unsigned short resv;
}tComponent;
typedef struct
{
	unsigned char  buf[4096];
	unsigned short bitMapLen;
	unsigned char  *bitMapData;
	unsigned short componentNum;
	tComponent     *component;
}tLcdData;


#endif
