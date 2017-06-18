/************************************************************************/
/** \file
* \n  �� �� �� : com103.h     
* \n  �ļ����� : ����103����    
* \n  ��    �� : DAQO
* \n  ����ʱ�� : 2012.9.26
* \n  ��    ע : 
* \n  ��ʷ��¼ :
*/
/************************************************************************/
#ifndef _COM103_H
#define _COM103_H

#include "stru.h"
/******************   yd define *******************/
#define YEAR_OFFSET 0
#define MON_OFFSET  1
#define DAY_OFFSET  2
#define HOUR_OFFSET 3
#define MIN_OFFSET  4
#define SEC_OFFSET  5
#define MSH_OFFSET  6
#define MSL_OFFSET  7

#define YEAR_OFFSET_CP56 6
#define MON_OFFSET_CP56  5
#define DAY_OFFSET_CP56  4
#define HOUR_OFFSET_CP56 3
#define MIN_OFFSET_CP56  2


#define NOF_BYTES  2
#define NOS_BYTES  1
//#define SJJG_BYTES 2
//#define JLBS_BYTES 2
//#define XXYS_BYTES 2
//#define CP56_BYTES 7
#define LOS_BYTES  1
#define LOF_BYTES  3


/********************************���ͱ�ʾ������**********************************/
/*�ڼ��ӷ����ϵĹ�����Ϣ                                                        */
/*TYPE IDENTIFICATION��UI8[1~8]<0..44>                                          */
#define M_SP_NA_1       1       /*������Ϣ                                      */ //wu
#define M_SP_TA_1       2       /*��ʱ��ĵ�����Ϣ                              */
#define M_DP_NA_1       3       /*˫����Ϣ                                      */ //wu
#define M_DP_TA_1       4       /*��ʱ���˫����Ϣ                              */
#define M_ST_NA_1       5       /*��λ����Ϣ                                    */
#define M_ST_TA_1       6       /*��ʱ��Ĳ�λ����Ϣ                            */
#define M_BO_NA_1       7       /*32���ش�                                      */
#define M_BO_TA_1       8       /*��ʱ���32���ش�  		                        */
#define M_ME_NA_1       9       /*����ֵ����һ��ֵ                              *///wu
#define M_ME_TA_1      10       /*��ʱ��Ĳ���ֵ����һ��ֵ                      */
#define M_ME_NB_1      11       /*����ֵ����Ȼ�ֵ                              *///wu
#define M_ME_TB_1      12       /*��ʱ��Ĳ���ֵ����Ȼ�ֵ                      */
#define M_ME_NC_1      13       /*����ֵ���̸�����            				          */
#define M_ME_TC_1      14       /*��ʱ��Ĳ���ֵ���̸�����    				          */
#define M_IT_NA_1      15       /*�ۼ���									    				          */
#define M_IT_TA_1      16       /*��ʱ����ۼ���              				          */
#define M_EP_TA_1      17       /*��ʱ��ļ̵籣���豸�¼�                      */
#define M_EP_TB_1      18       /*��ʱ��ļ̵籣�����������¼�                  */
#define M_EP_TC_1      19       /*��ʱ��ļ̵籣���豸���������·��Ϣ          */
#define M_PS_NA_1      20       /*����λ������鵥����Ϣ                        */
#define M_ME_ND_1      21       /*����ֵ������Ʒ�������εĹ�һ��ֵ              */                                                               
#define M_SP_TB_1      30       /*��CP56Time2aʱ��ĵ�����Ϣ                    *///wu
#define M_DP_TB_1      31       /*��CP56Time2aʱ���˫����Ϣ                    */
#define M_ST_TB_1      32       /*��CP56Time2aʱ��Ĳ�λ��Ϣ                    */
#define M_BO_TB_1      33       /*��CP56Time2aʱ���32���ش�                    */
#define M_ME_TD_1      34       /*��CP56Time2aʱ��Ĳ���ֵ����һ��ֵ            */
#define M_ME_TE_1      35       /*��CP56Time2aʱ��Ĳ���ֵ����Ȼ�ֵ            */
#define M_ME_TF_1      36       /*��CP56Time2aʱ��Ĳ���ֵ���̸�����            */
#define M_IT_TB_1      37       /*��CP56Time2aʱ����ۼ���                      *///wu
#define M_EP_TD_1      38       /*��CP56Time2aʱ��ļ̵籣��װ���¼�            */
#define M_EP_TE_1      39       /*��CP56Time2aʱ��ļ̵籣��װ�ó��������¼�    */
#define M_EP_TF_1      40       /*��CP56Time2aʱ��ļ̵籣��װ�ó��������·��Ϣ*/
/*�ڿ��Ʒ���Ĺ�����Ϣ     CON                                                        */
/*TYPE IDENTIFICATION:UI8[1~8]<45..69>                                          */
#define C_SC_NA_1      45       /*��������                                      */
#define C_DC_NA_1      46       /*˫������                                      */
#define C_RC_NA_1      47       /*���ڲ�����                                    */
#define C_SE_NA_1      48       /*�趨ֵ�����һ��ֵ                          */
#define C_SE_NB_1      49       /*�趨ֵ�����Ȼ�ֵ                          */
#define C_SE_NC_1      50       /*�趨ֵ����̸�����                          */
#define C_BO_NA_1      51       /*32���ش�                                      */
#define C_SC_TA_1      58       /*��ʱ��CP56Time2a�������� */
#define C_DC_TA_1      59       /*��ʱ��CP56Time2a˫������ */
#define C_RC_TA_1      60        /*��ʱ��CP56Time2a���������� */
#define C_SE_TA_1      61        /*��ʱ����趨ֵ����һ��ֵ*/
#define C_SE_TB_1      62
#define C_SE_TC_1      63
/*�ڼ��ӷ����ϵͳ����                                                          */
/*TYPE IDENTIFICATION:UI8[1~8]<70..99>                                          */
#define M_EI_NA_1      70       /*��ʼ������   
/*�ڿ��Ʒ����ϵͳ����     CON                                                  */
/*TYPE IDENTIFICATION:UI8[1~8]<100..109>                                        */
#define C_IC_NA_1     100       /*���ٻ�����                                    */
#define C_CI_NA_1     101       /*�������ٻ�����                                */
#define C_RD_NA_1     102       /*������                                        */
#define C_CS_NA_1     103       /*ʱ��ͬ������                                  */
#define C_TS_NA_1     104       /*�������ƽ�ⷽʽ�²��еĹ��ܡ�              */
#define C_RP_NA_1     105       /*��λ��������                                  */
#define C_CD_NA_1     106       /*��ʱ�������                                  */
#define C_TS_TA_1     107       /*��CP56Time2aʱ��Ĳ�������                    */
/*�ڿ��Ʒ���Ĳ�������     CON                                                  */
/*TYPE IDENTIFICATION:UI8[1~8]<110..119>                                        */
#define P_ME_NA_1     110       /*����ֵ��������һ��ֵ                          */
#define P_ME_NB_1     111       /*����ֵ��������Ȼ�ֵ                          */
#define P_ME_NC_1     112       /*����ֵ�������̸�����                          */
#define P_AC_NA_1     113       /*��������                                      */
/*�ļ�����                                                                      */
/*TYPE IDENTIFICATION:UI8[1~8]<120..127>                                        */
#define F_FR_NA_1     120       /*�ļ�׼������                                  */
#define F_SR_NA_1     121       /*��׼������                                    */
#define F_SC_NA_1     122       /*�ٻ�Ŀ¼��ѡ���ļ����ٻ��ļ����ٻ���          */
#define F_LS_NA_1     123       /*���Ľڣ����Ķ�                            */
#define F_SG_NA_1     124       /*��                                            */
#define F_DR_NA_1     125       /*Ŀ¼                                          */
#define C_SE_ND_1     136
/********************************************************************************/


/**************************����ԭ����******************************************/
#define COT_PER_CYC         1       /*���ڡ�ѭ��                                */
#define COT_BACK            2       /*����ɨ��                                  */
#define COT_SPONT           3       /*�Է���ͻ��                                */
#define COT_INIT            4       /*��ʼ��                                    */
#define COT_REQ             5       /*������߱�����                            */
#define COT_ACT             6       /*����                                      */
#define COT_ACTCON          7       /*ȷ�ϼ���                                  */
#define COT_DEACT           8       /*ֹͣ����                                  */
#define COT_DEACTCON        9       /*ֹͣ����ȷ��                              */
#define COT_ACTTERM        10       /*������ֹ                                  */
#define COT_RETREM         11       /*Զ����������ķ�����Ϣ                    */
#define COT_RETLOG         12       /*������������ķ�����Ϣ                    */
#define COT_FILE           13       /*�ļ�����                                  */
#define COT_INRO           20       /*��Ӧվ�ٻ�                                */
#define COT_INRO1          21       /*��Ӧ��1���ٻ�                             */
#define COT_INRO2          22       /*��Ӧ��2���ٻ�                             */
#define COT_INRO3          23       /*��Ӧ��3���ٻ�                             */
#define COT_INRO4          24       /*��Ӧ��4���ٻ�                             */
#define COT_INRO5          25       /*��Ӧ��5���ٻ�                             */
#define COT_INRO6          26       /*��Ӧ��6���ٻ�                             */
#define COT_INRO7          27       /*��Ӧ��7���ٻ�                             */  
#define COT_INRO8          28       /*��Ӧ��8���ٻ�                             */  
#define COT_INRO9          29       /*��Ӧ��9���ٻ�                             */  
#define COT_INRO10         30       /*��Ӧ��10���ٻ�                            */  
#define COT_INRO11         31       /*��Ӧ��11���ٻ�                            */  
#define COT_INRO12         32       /*��Ӧ��12���ٻ�                            */  
#define COT_INRO13         33       /*��Ӧ��13���ٻ�                            */
#define COT_INRO14         34       /*��Ӧ��14���ٻ�                            */
#define COT_INRO15         35       /*��Ӧ��15���ٻ�                            */
#define COT_INRO16         36       /*��Ӧ��16���ٻ�                            */
#define COT_REQCOGEN       37       /*��Ӧ�������ٻ�                            */
#define COT_REQCO1         38       /*��Ӧ��1��������ٻ�                       */
#define COT_REQCO2         39       /*��Ӧ��2��������ٻ�                       */
#define COT_REQCO3         40       /*��Ӧ��3��������ٻ�                       */
#define COT_REQCO4         41       /*��Ӧ��4��������ٻ�                       */
#define COT_UNKNOW_TYPE    44       /*δ֪�����ͱ�ʾ                            */ 
#define COT_UNKNOW_COT     45       /*δ֪�Ĵ���ԭ��                            */
#define COT_UNKNOW_ADDR    46       /*δ֪��Ӧ�÷������ݵ�Ԫ������ַ            */
#define COT_UNKNOW_INF     47       /*δ֪����Ϣ������ַ                      */
#define COT_ACTCON_NO       0x47      /*�����ȷ��*/
#define COT_DEACTCON_NO   0x49       //���������ȷ��
/********************************************************************************/

#define NORMALIZED_VALUE       1  //��һ��ֵ
#define SCALE_VALUE            2  //��Ȼ�ֵ
#define SHORT_FLOAT_VALUE      3  //�̸���ֵ
#define NO_QUALITY_VALUE       4  //��Ʒ�ʹ�һ��ֵ

/****************** com103 define ***********************************/
#define YM_PER_FRAME     44
#define MAX_YM_NUM       88
#define MAX_YC_NUM_ZX    220
#define YC_PER_FRAME     110

#define YM_CLASS     0x6
#define QFCS_CLASS   0xa0
#define SOECS_CLASS  0xa1


typedef struct YK_INF_TAG
{
	unsigned char fun;
	unsigned char inf;
	unsigned char value;
	unsigned char selFlag;
	unsigned char ctrlFlag;
	unsigned char holdTime;
	unsigned short infoAddr;
} YK_INF;

typedef struct 
{
	unsigned short yydzBs;        /*ASDU��ַ�ֽ���*/
	unsigned short csyyBs;        /*����ԭ���ֽ���*/
	unsigned short xxdzBs;        /*��Ϣ��ַ�ֽ���*/
	unsigned short ydz;           /*Դ��ַ*/
	unsigned short xxdz3;         /*��Ϣ��ַ�����ֽ�ֵ*/
	unsigned short ip1[4];        
	unsigned short ip2[4];        
	unsigned short wg1[4];        /*����*/
	unsigned short wg2[4];
	unsigned short bclsyc;        /*�Ƿ񱣴���ʷ�����ļ���־*/
	unsigned short lsyczq;        /*��ʷң��洢����*/
	unsigned short yydz;         /*Ӧ�õ�ַ*/
	unsigned short ddyxSdz;      /*����ң��ʼ��ַ*/
	unsigned short sdyxSdz;      /*˫��ң��ʼ��ַ*/                   
	unsigned short ycSdz;        /*ң��ʼ��ַ*/        
	unsigned short ykSdz;        /*ң��ʼ��ַ*/         
	unsigned short dnSdz;        /*����ʼ��ַ*/                                    
	unsigned short bwzSdz;		 /*��λ��ʼ��ַ*/
	unsigned short csSdz;        /*����ʼ��ַ*/
	unsigned short sdSdz;        /*�趨ʼ��ַ*/
	unsigned short wjSdz;        /*�ļ�ʼ��ַ*/
	unsigned short lssjSdz;      /*��ʷ�¼�ʼ��ַ*/
	unsigned short yclx;         /*ң������*/
	unsigned short  yxsMg;       /*ÿ��ң����*/
	unsigned short  ycsMg;       /*ÿ��ң����*/
	unsigned short yxsMz;		 /*ÿ֡ң����*/
	unsigned short yxsjsMz;       /*ÿ֡ң���¼���*/    
	unsigned short yxbwsMz;       /*ÿ֡ң�ű�λ��*/              
	unsigned short ycsMz;         /*ÿ֡ң����*/         
	unsigned short dnsMz;		 /*ÿ֡������*/      
	unsigned short lxPy;          /*����ƫ��*/
	unsigned short xdcPy;         /*����ƫ��*/
	unsigned short csyyPy;	     /*����ԭ��ƫ��*/
	unsigned short yydzPy;        /*Ӧ�õ�ַƫ��*/
	unsigned short xxdzPy;        /*��Ϣ��ַƫ��*/
	unsigned short xxysPy;        /*��ϢԪ��ƫ��*/
	unsigned short cslx101;       /*101��Լ��������*/
} PROTOCOL_PARAM;

typedef union 
{
	unsigned short data[256];
	struct
	{
		unsigned short bclsyc;        /*�Ƿ񱣴���ʷ�����ļ���־*/
		unsigned short lsyczq;        /*��ʷң��洢����*/
		unsigned short yydz;         /*Ӧ�õ�ַ*/
		unsigned short ddyxSdz;      /*����ң��ʼ��ַ*/
		unsigned short sdyxSdz;      /*˫��ң��ʼ��ַ*/                   
		unsigned short ycSdz;        /*ң��ʼ��ַ*/        
		unsigned short ykSdz;        /*ң��ʼ��ַ*/         
		unsigned short dnSdz;        /*����ʼ��ַ*/                                    
		unsigned short bwzSdz;		 /*��λ��ʼ��ַ*/
		unsigned short csSdz;        /*����ʼ��ַ*/
		unsigned short sdSdz;        /*�趨ʼ��ַ*/
		unsigned short wjSdz;        /*�ļ�ʼ��ַ*/
		unsigned short lssjSdz;      /*��ʷ�¼�ʼ��ַ*/
		unsigned short yclx;         /*ң������*/
	}Param;
} YD_GTCS;

typedef union 
{
	unsigned short data[256];
	struct  
	{
		unsigned short yydzBs101;        /*ASDU��ַ�ֽ���*/
		unsigned short csyyBs101;        /*����ԭ���ֽ���*/
		unsigned short xxdzBs101;        /*��Ϣ��ַ�ֽ���*/
		unsigned short ydz101;           /*Դ��ַ*/
		unsigned short xxdz3101;         /*��Ϣ��ַ�����ֽ�ֵ*/
		unsigned short yydzBs104;        /*ASDU��ַ�ֽ���*/
		unsigned short csyyBs104;        /*����ԭ���ֽ���*/
		unsigned short xxdzBs104;        /*��Ϣ��ַ�ֽ���*/
		unsigned short ydz104;           /*Դ��ַ*/
		unsigned short xxdz3104;         /*��Ϣ��ַ�����ֽ�ֵ*/
		unsigned short ip1[4];        
		unsigned short ip2[4];        
		unsigned short wg1[4];        /*����*/
		unsigned short wg2[4];
		unsigned short cslx101;          /*ƽ��/��ƽ��*/
	}Param;
} YD_TSCS;

typedef struct CFG_TOOL_TAG  
{
	unsigned char dbwjcz;    /*����ļ����ڱ�־*/
	unsigned char dzwjcz;    /*��ֵ�ļ����ڱ�־*/
	unsigned char lsycwjcz;  /*��ʷң���ļ����ڱ�־*/
	unsigned char lssoewjcz; /*��ʷSOE�ļ����ڱ�־*/
	unsigned char dzwj;      /*��д��ֵ�ļ���־*/
	unsigned char lswj;      /*��д��ʷ�ļ���־*/
	unsigned char dbwj;      /*��д����ļ���־*/
} FILE_FLAG;

typedef struct 
{
	unsigned long  hstrySoeNum[31];
	unsigned long  soetotalNum;
	unsigned char  oneSoeLen;
	long           ycSectionLen;
	unsigned char  soeFileCs;
	unsigned char  ycFileCs;
	FILE_FLAG      flag;
} FileInfo;



#if 0
	#define YX_FUN         YX_CLASS
	#define YC_FUN         YC_CLASS
	#define YK_FUN         YK_CLASS
	#define FJT_FUN        FJT_CLASS
	#define YM_FUN         YM_CLASS
	#define YK_START_INF   1
	#define YX_START_INF   1
	#define YC_START_INF   1
	#define YM_START_INF   1
	#define FJT_START_INF  1
#else
	#define YX_FUN         1
	#define YC_FUN         1
	#define YK_FUN         1
	#define FJT_FUN        1
	#define YM_FUN         1

	#define YK_START_INF   48
	#define YX_START_INF   149
	#define YC_START_INF   92
	#define YM_START_INF   6
	#define FJT_START_INF  76
#endif

#define MAX_LEN       32

#define YDYC_MAX_NUM  512
#define YDYX_MAX_NUM  512
#define YDYM_MAX_NUM  100
#define YDYK_MAX_NUM  32

#define YX_MAX_NUM    255
#define BHZT_MAX_NUM  255
#define YC_MAX_NUM    255
#define SECZL_MAX_NUM  32
#define BH_MAX_NUM    255
#define KXFH_MAX_NUM   64
#define WD_MAX_NUM     4
#define XB_MAX_NUM     10
#define YM_MAX_NUM    64
#define YK_MAX_NUM    32
typedef struct YdBase_TAG 
{
	unsigned short  yxNum;
	unsigned short  bhztNum;
	unsigned short  yxTotalNum;
	unsigned short  ycNum;
	unsigned short  secondZLNum;
	unsigned short  kxycNum;
	unsigned short  BhycNum;
	unsigned short  wdycNum;     /*�¶�ң��*/
	unsigned short  wcxbNum;     /*���г��*/
	unsigned short  ycTotalNum;
	unsigned short  ymNum;
	unsigned short  rybNum;
	unsigned short  dzyjNum;
	unsigned short  ykNum;
	unsigned char   fjtdwNum;
	
	unsigned short yx[YDYX_MAX_NUM];  /*Զ��˳�������µ�װ��GIN��*/ 
	unsigned short yc[YDYC_MAX_NUM];   
	unsigned short ym[YDYM_MAX_NUM];   
	unsigned short yk[YDYK_MAX_NUM];  

	unsigned short yxqfFlag[YDYX_MAX_NUM];
	unsigned short yxSoeFlag[YDYX_MAX_NUM];
	
	unsigned short zzyx[YX_MAX_NUM];   /*gin�����µ�Զ�����*/
	unsigned short zzbhzt[BHZT_MAX_NUM];
	unsigned short zzyc[YC_MAX_NUM];
	unsigned short zzseczl[SECZL_MAX_NUM];
	unsigned short zzbhcy[BH_MAX_NUM];
	unsigned short zzkxfh[KXFH_MAX_NUM];
	unsigned short zzwd[WD_MAX_NUM];
	unsigned short zzwcxb[XB_MAX_NUM]; /*���г��10.5.21*/
	unsigned short zzym[YM_MAX_NUM];   
	unsigned short zzyk[YK_MAX_NUM];   
} YdBase;

// 
// typedef struct PORT_INF_TAG
// {
// 	unsigned short  LinkAddr;
// 	unsigned short  yxNum;
// 	unsigned short  bhztNum;
// 	unsigned short  yxTotalNum;
// 	unsigned short  ycNum;
// 	unsigned short  secondZLNum;
// 	unsigned short  kxycNum;
// 	unsigned short  BhycNum;
// 	unsigned short  wdycNum;     /*�¶�ң��*/
// 	unsigned short  wcxbNum;
// 	unsigned short  ycTotalNum;
// 	unsigned short  ymNum;
// 	unsigned short  rybNum;
// 	unsigned short  dzyjNum;
// 	unsigned short  ykNum;
// 	unsigned char   fjtdwNum;
// 	unsigned char   yxValue[MAX_LEN];
// 	unsigned char   fjtdwValue[4];
// 	unsigned char   time[4];
// 	unsigned char   qSendMsgNum[2];
// 	unsigned char   commState[2];    
// 	MSG_Q_ID        qRcvMsg[2];//
// 	MSG_Q_ID        qSendMsg[2];//Ӧ�ò㷢�ͱ�������
// 	YdBase          ydBase;
// } PORT_INF;        /*������Ϣ*/


#ifndef YES
	#define YES 1
#endif

#ifndef NO
	#define NO 0
#endif 


#ifndef COM_EMPTY
	#define COM_EMPTY 0
#endif

#ifndef COM_FULL
	#define COM_FULL 0xff
#endif


/*************************���������붨��***************************************/
/*��վ����վ����Ĺ����� */
#define C_RL_SEND      0              /*��λԶ����· */
#define C_PA           8              /*�ٻ�Ҫ�����λ*/ 
#define C_RQ_LINKREQ   9              /*������·״̬ */
#define C_P1           10             /*����һ������ */                               
#define C_P2           11             /*����������� */

/*��վ����վ����Ĺ�����*/
#define S_CTRL_CODE_CONFIRM      0   /*ȷ��                                     */
#define S_CTRL_CODE_LNKBUSY      1   /*��·æ��δ���ܱ���                       */
#define S_STRL_CODE_DATA         8   /*��������Ӧ����֡                         */
#define S_STRL_CODE_NODATA       9   /*������                                   */
#define S_STRL_CODE_LNKSTATUS   11   /*����·״̬���������ش�����֡           */
#define S_STRL_CODE_LNKNOSERVER 14   /*��·����δ����                           */
#define S_STRL_CODE_LNKNOFINISH 15   /*��·����Ϊ���                           */
/********************************************************************************/

typedef struct{
	/*��֡���տ����ڲ���������·�㴦��ṹ*/
	unsigned char	sm;		/*����״̬������·���������֡����0x16��Ӧ�ò㴦�������0xff*/
	unsigned char	typ;		/*֡����*/
	unsigned char	len;
	unsigned char	bp;		/*bufջָ��*/
	unsigned char	fcb;
	unsigned char	dfc;
	unsigned char	err;		/*֡���մ�������*/
	/*0-L,1-head,2-PRM,3-addr,4-cs,5-rear,6-send err,7-recv time out*/
	unsigned char	rts_dly;
	unsigned char	buf[256];	/*���ջ�����*/
	unsigned char	send_buf[264];	/*���Ͷ���*/
	unsigned short	send_front;	/*��ͷ����*/
	unsigned short	send_rear;	/*��β����*/
	
	unsigned short	rx_ch;		/*�������ַ���*/
	unsigned short	rx_frm;		/*������֡��*/
	unsigned short	tx_ch;		/*�������ַ���*/
	unsigned short	tx_frm;		/*������֡��*/
	
	unsigned char 	init_flag;	/*��ʼ����ֹ���ձ�־*/
	unsigned char 	rxfull_cnt; /*�̵籣���豸���ջ�����������������3��ǿ����0*/
	unsigned short	except_cnt;	/*�ַ���,֡�估���մ������ڳ�ʱ�жϼ���*/
	unsigned short	rxoff_cnt;	/*��ֹ���ճ�ʱ�жϼ���*/
	unsigned char  appRxFlag;
	unsigned char  appTxFlag;
	
}sci_dev_stu;

#define WD_CLASS    0x8a
#define WCXB_CLASS  0x8b

#define	FCV		(0x10)
#define	FCB		(0x20)
#define	DFC		(0x10)
#define	ACD		(0x20)
#define	PRM		(0x40)

#define	RTS_DE		(1)		/*COM��RTS�ӷ���ʱRTS_DE��Ϊ1,��ʾ�߼���ֱ��*/

#define CLEAR_FILE  1
#define READ_FILE   2

typedef struct
{
	tZzcs5771Data	zzcs;       /*װ�ò���*/
	tYkpz5700Data	ykpz;		/*������ֵ����*/	
	tYcpz5700Data	ycpz;		/*ң�����ò���*/
	tJdcs5771Data	jdcs;       /*�������Ȳ���*/
	tJdtz5771Data	jdtz;       /*�������ȵ���*/
	tYxcs5700Data	yxcs;       /*ң�Ų���*/
	tDwcs5700Data	dwcs;       /*δ��*/
	tTqcs5700Data	tqcs;		/*δ��*/
	tDccs5700Data	dccs;		/*ֱ������*/
	tCktxcs5700Data	cktxcs;     /*����ͨѶ����*/
	tDctf		dctf;		    /*ֱ������*/
	tDcjd5700Data	dcjd;		/*ֱ�����Ȳ���*/
}tDevPar;

typedef struct  
{
	tDevPar devParm;
    YD_GTCS ydgtcs;
	YD_TSCS ydtscs;
} tDownloadParam;


#endif//_COM103_H

















