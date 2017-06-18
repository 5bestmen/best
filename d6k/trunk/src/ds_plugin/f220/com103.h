/************************************************************************/
/** \file
* \n  文 件 名 : com103.h     
* \n  文件功能 : 串口103定义    
* \n  作    者 : DAQO
* \n  创建时间 : 2012.9.26
* \n  备    注 : 
* \n  历史记录 :
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


/********************************类型标示的语义**********************************/
/*在监视方向上的过程信息                                                        */
/*TYPE IDENTIFICATION：UI8[1~8]<0..44>                                          */
#define M_SP_NA_1       1       /*单点信息                                      */ //wu
#define M_SP_TA_1       2       /*带时标的单点信息                              */
#define M_DP_NA_1       3       /*双点信息                                      */ //wu
#define M_DP_TA_1       4       /*带时标的双点信息                              */
#define M_ST_NA_1       5       /*步位置信息                                    */
#define M_ST_TA_1       6       /*带时标的步位置信息                            */
#define M_BO_NA_1       7       /*32比特串                                      */
#define M_BO_TA_1       8       /*带时标的32比特串  		                        */
#define M_ME_NA_1       9       /*测量值，归一化值                              *///wu
#define M_ME_TA_1      10       /*带时标的测量值，归一化值                      */
#define M_ME_NB_1      11       /*测量值，标度化值                              *///wu
#define M_ME_TB_1      12       /*带时标的测量值，标度化值                      */
#define M_ME_NC_1      13       /*测量值，短浮点数            				          */
#define M_ME_TC_1      14       /*带时标的测量值，短浮点数    				          */
#define M_IT_NA_1      15       /*累计量									    				          */
#define M_IT_TA_1      16       /*带时标的累计量              				          */
#define M_EP_TA_1      17       /*带时标的继电保护设备事件                      */
#define M_EP_TB_1      18       /*带时标的继电保护成组启动事件                  */
#define M_EP_TC_1      19       /*带时标的继电保护设备成组输出电路信息          */
#define M_PS_NA_1      20       /*带变位检出成组单点信息                        */
#define M_ME_ND_1      21       /*测量值，不带品质描述次的归一化值              */                                                               
#define M_SP_TB_1      30       /*带CP56Time2a时标的单点信息                    *///wu
#define M_DP_TB_1      31       /*带CP56Time2a时标的双点信息                    */
#define M_ST_TB_1      32       /*带CP56Time2a时标的步位信息                    */
#define M_BO_TB_1      33       /*带CP56Time2a时标的32比特串                    */
#define M_ME_TD_1      34       /*带CP56Time2a时标的测量值，归一化值            */
#define M_ME_TE_1      35       /*带CP56Time2a时标的测量值，标度化值            */
#define M_ME_TF_1      36       /*带CP56Time2a时标的测量值，短浮点数            */
#define M_IT_TB_1      37       /*带CP56Time2a时标的累计量                      *///wu
#define M_EP_TD_1      38       /*带CP56Time2a时标的继电保护装置事件            */
#define M_EP_TE_1      39       /*带CP56Time2a时标的继电保护装置成组启动事件    */
#define M_EP_TF_1      40       /*带CP56Time2a时标的继电保护装置成组输出电路信息*/
/*在控制方向的过程信息     CON                                                        */
/*TYPE IDENTIFICATION:UI8[1~8]<45..69>                                          */
#define C_SC_NA_1      45       /*单点命令                                      */
#define C_DC_NA_1      46       /*双点命令                                      */
#define C_RC_NA_1      47       /*调节步命令                                    */
#define C_SE_NA_1      48       /*设定值命令，归一化值                          */
#define C_SE_NB_1      49       /*设定值命令，标度化值                          */
#define C_SE_NC_1      50       /*设定值命令，短浮点数                          */
#define C_BO_NA_1      51       /*32比特串                                      */
#define C_SC_TA_1      58       /*带时标CP56Time2a单点命令 */
#define C_DC_TA_1      59       /*带时标CP56Time2a双点命令 */
#define C_RC_TA_1      60        /*带时标CP56Time2a步调节命令 */
#define C_SE_TA_1      61        /*带时标的设定值，归一化值*/
#define C_SE_TB_1      62
#define C_SE_TC_1      63
/*在监视方向的系统命令                                                          */
/*TYPE IDENTIFICATION:UI8[1~8]<70..99>                                          */
#define M_EI_NA_1      70       /*初始化结束   
/*在控制方向的系统命令     CON                                                  */
/*TYPE IDENTIFICATION:UI8[1~8]<100..109>                                        */
#define C_IC_NA_1     100       /*总召唤命令                                    */
#define C_CI_NA_1     101       /*计数量召唤命令                                */
#define C_RD_NA_1     102       /*读命令                                        */
#define C_CS_NA_1     103       /*时钟同步命令                                  */
#define C_TS_NA_1     104       /*测试命令，平衡方式下才有的功能。              */
#define C_RP_NA_1     105       /*复位进程命令                                  */
#define C_CD_NA_1     106       /*延时获得命令                                  */
#define C_TS_TA_1     107       /*带CP56Time2a时标的测试命令                    */
/*在控制方向的参数命令     CON                                                  */
/*TYPE IDENTIFICATION:UI8[1~8]<110..119>                                        */
#define P_ME_NA_1     110       /*测量值参数，归一化值                          */
#define P_ME_NB_1     111       /*测量值参数，标度化值                          */
#define P_ME_NC_1     112       /*测量值参数，短浮点数                          */
#define P_AC_NA_1     113       /*参数激活                                      */
/*文件传输                                                                      */
/*TYPE IDENTIFICATION:UI8[1~8]<120..127>                                        */
#define F_FR_NA_1     120       /*文件准备就绪                                  */
#define F_SR_NA_1     121       /*节准备就绪                                    */
#define F_SC_NA_1     122       /*召唤目录，选择文件，召唤文件，召唤节          */
#define F_LS_NA_1     123       /*最后的节，最后的段                            */
#define F_SG_NA_1     124       /*段                                            */
#define F_DR_NA_1     125       /*目录                                          */
#define C_SE_ND_1     136
/********************************************************************************/


/**************************传送原因定义******************************************/
#define COT_PER_CYC         1       /*周期、循环                                */
#define COT_BACK            2       /*背景扫描                                  */
#define COT_SPONT           3       /*自发、突发                                */
#define COT_INIT            4       /*初始化                                    */
#define COT_REQ             5       /*请求或者被请求                            */
#define COT_ACT             6       /*激活                                      */
#define COT_ACTCON          7       /*确认激活                                  */
#define COT_DEACT           8       /*停止激活                                  */
#define COT_DEACTCON        9       /*停止激活确认                              */
#define COT_ACTTERM        10       /*激活终止                                  */
#define COT_RETREM         11       /*远方命令引起的反送信息                    */
#define COT_RETLOG         12       /*当地命令引起的反送信息                    */
#define COT_FILE           13       /*文件传输                                  */
#define COT_INRO           20       /*响应站召唤                                */
#define COT_INRO1          21       /*响应第1组召唤                             */
#define COT_INRO2          22       /*响应第2组召唤                             */
#define COT_INRO3          23       /*响应第3组召唤                             */
#define COT_INRO4          24       /*响应第4组召唤                             */
#define COT_INRO5          25       /*响应第5组召唤                             */
#define COT_INRO6          26       /*响应第6组召唤                             */
#define COT_INRO7          27       /*响应第7组召唤                             */  
#define COT_INRO8          28       /*响应第8组召唤                             */  
#define COT_INRO9          29       /*响应第9组召唤                             */  
#define COT_INRO10         30       /*响应第10组召唤                            */  
#define COT_INRO11         31       /*响应第11组召唤                            */  
#define COT_INRO12         32       /*响应第12组召唤                            */  
#define COT_INRO13         33       /*响应第13组召唤                            */
#define COT_INRO14         34       /*响应第14组召唤                            */
#define COT_INRO15         35       /*响应第15组召唤                            */
#define COT_INRO16         36       /*响应第16组召唤                            */
#define COT_REQCOGEN       37       /*响应计数量召唤                            */
#define COT_REQCO1         38       /*响应第1组计数量召唤                       */
#define COT_REQCO2         39       /*响应第2组计数量召唤                       */
#define COT_REQCO3         40       /*响应第3组计数量召唤                       */
#define COT_REQCO4         41       /*响应第4组计数量召唤                       */
#define COT_UNKNOW_TYPE    44       /*未知的类型标示                            */ 
#define COT_UNKNOW_COT     45       /*未知的传送原因                            */
#define COT_UNKNOW_ADDR    46       /*未知的应用服务数据单元公共地址            */
#define COT_UNKNOW_INF     47       /*未知的信息体对象地址                      */
#define COT_ACTCON_NO       0x47      /*激活否定确认*/
#define COT_DEACTCON_NO   0x49       //激活结束否定确认
/********************************************************************************/

#define NORMALIZED_VALUE       1  //规一化值
#define SCALE_VALUE            2  //标度化值
#define SHORT_FLOAT_VALUE      3  //短浮点值
#define NO_QUALITY_VALUE       4  //无品质规一化值

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
	unsigned short yydzBs;        /*ASDU地址字节数*/
	unsigned short csyyBs;        /*传送原因字节数*/
	unsigned short xxdzBs;        /*信息地址字节数*/
	unsigned short ydz;           /*源地址*/
	unsigned short xxdz3;         /*信息地址第三字节值*/
	unsigned short ip1[4];        
	unsigned short ip2[4];        
	unsigned short wg1[4];        /*网关*/
	unsigned short wg2[4];
	unsigned short bclsyc;        /*是否保存历史数据文件标志*/
	unsigned short lsyczq;        /*历史遥测存储周期*/
	unsigned short yydz;         /*应用地址*/
	unsigned short ddyxSdz;      /*单点遥信始地址*/
	unsigned short sdyxSdz;      /*双点遥信始地址*/                   
	unsigned short ycSdz;        /*遥测始地址*/        
	unsigned short ykSdz;        /*遥控始地址*/         
	unsigned short dnSdz;        /*电能始地址*/                                    
	unsigned short bwzSdz;		 /*步位置始地址*/
	unsigned short csSdz;        /*参数始地址*/
	unsigned short sdSdz;        /*设定始地址*/
	unsigned short wjSdz;        /*文件始地址*/
	unsigned short lssjSdz;      /*历史事件始地址*/
	unsigned short yclx;         /*遥测类型*/
	unsigned short  yxsMg;       /*每组遥信数*/
	unsigned short  ycsMg;       /*每组遥测数*/
	unsigned short yxsMz;		 /*每帧遥信数*/
	unsigned short yxsjsMz;       /*每帧遥信事件数*/    
	unsigned short yxbwsMz;       /*每帧遥信变位数*/              
	unsigned short ycsMz;         /*每帧遥测数*/         
	unsigned short dnsMz;		 /*每帧电能数*/      
	unsigned short lxPy;          /*类型偏移*/
	unsigned short xdcPy;         /*类型偏移*/
	unsigned short csyyPy;	     /*传送原因偏移*/
	unsigned short yydzPy;        /*应用地址偏移*/
	unsigned short xxdzPy;        /*信息地址偏移*/
	unsigned short xxysPy;        /*信息元素偏移*/
	unsigned short cslx101;       /*101规约传输类型*/
} PROTOCOL_PARAM;

typedef union 
{
	unsigned short data[256];
	struct
	{
		unsigned short bclsyc;        /*是否保存历史数据文件标志*/
		unsigned short lsyczq;        /*历史遥测存储周期*/
		unsigned short yydz;         /*应用地址*/
		unsigned short ddyxSdz;      /*单点遥信始地址*/
		unsigned short sdyxSdz;      /*双点遥信始地址*/                   
		unsigned short ycSdz;        /*遥测始地址*/        
		unsigned short ykSdz;        /*遥控始地址*/         
		unsigned short dnSdz;        /*电能始地址*/                                    
		unsigned short bwzSdz;		 /*步位置始地址*/
		unsigned short csSdz;        /*参数始地址*/
		unsigned short sdSdz;        /*设定始地址*/
		unsigned short wjSdz;        /*文件始地址*/
		unsigned short lssjSdz;      /*历史事件始地址*/
		unsigned short yclx;         /*遥测类型*/
	}Param;
} YD_GTCS;

typedef union 
{
	unsigned short data[256];
	struct  
	{
		unsigned short yydzBs101;        /*ASDU地址字节数*/
		unsigned short csyyBs101;        /*传送原因字节数*/
		unsigned short xxdzBs101;        /*信息地址字节数*/
		unsigned short ydz101;           /*源地址*/
		unsigned short xxdz3101;         /*信息地址第三字节值*/
		unsigned short yydzBs104;        /*ASDU地址字节数*/
		unsigned short csyyBs104;        /*传送原因字节数*/
		unsigned short xxdzBs104;        /*信息地址字节数*/
		unsigned short ydz104;           /*源地址*/
		unsigned short xxdz3104;         /*信息地址第三字节值*/
		unsigned short ip1[4];        
		unsigned short ip2[4];        
		unsigned short wg1[4];        /*网关*/
		unsigned short wg2[4];
		unsigned short cslx101;          /*平衡/非平衡*/
	}Param;
} YD_TSCS;

typedef struct CFG_TOOL_TAG  
{
	unsigned char dbwjcz;    /*点表文件存在标志*/
	unsigned char dzwjcz;    /*定值文件存在标志*/
	unsigned char lsycwjcz;  /*历史遥测文件存在标志*/
	unsigned char lssoewjcz; /*历史SOE文件存在标志*/
	unsigned char dzwj;      /*读写定值文件标志*/
	unsigned char lswj;      /*读写历史文件标志*/
	unsigned char dbwj;      /*读写点表文件标志*/
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
	unsigned short  wdycNum;     /*温度遥测*/
	unsigned short  wcxbNum;     /*五次谐波*/
	unsigned short  ycTotalNum;
	unsigned short  ymNum;
	unsigned short  rybNum;
	unsigned short  dzyjNum;
	unsigned short  ykNum;
	unsigned char   fjtdwNum;
	
	unsigned short yx[YDYX_MAX_NUM];  /*远动顺序排列下的装置GIN号*/ 
	unsigned short yc[YDYC_MAX_NUM];   
	unsigned short ym[YDYM_MAX_NUM];   
	unsigned short yk[YDYK_MAX_NUM];  

	unsigned short yxqfFlag[YDYX_MAX_NUM];
	unsigned short yxSoeFlag[YDYX_MAX_NUM];
	
	unsigned short zzyx[YX_MAX_NUM];   /*gin排列下的远动序号*/
	unsigned short zzbhzt[BHZT_MAX_NUM];
	unsigned short zzyc[YC_MAX_NUM];
	unsigned short zzseczl[SECZL_MAX_NUM];
	unsigned short zzbhcy[BH_MAX_NUM];
	unsigned short zzkxfh[KXFH_MAX_NUM];
	unsigned short zzwd[WD_MAX_NUM];
	unsigned short zzwcxb[XB_MAX_NUM]; /*五次谐波10.5.21*/
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
// 	unsigned short  wdycNum;     /*温度遥测*/
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
// 	MSG_Q_ID        qSendMsg[2];//应用层发送报文邮箱
// 	YdBase          ydBase;
// } PORT_INF;        /*串口信息*/


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


/*************************控制域功能码定义***************************************/
/*主站向子站传输的功能码 */
#define C_RL_SEND      0              /*复位远方链路 */
#define C_PA           8              /*召唤要求访问位*/ 
#define C_RQ_LINKREQ   9              /*请求链路状态 */
#define C_P1           10             /*请求一级数据 */                               
#define C_P2           11             /*请求二级数据 */

/*子站向主站传输的功能码*/
#define S_CTRL_CODE_CONFIRM      0   /*确认                                     */
#define S_CTRL_CODE_LNKBUSY      1   /*链路忙，未接受报文                       */
#define S_STRL_CODE_DATA         8   /*以数据响应请求帧                         */
#define S_STRL_CODE_NODATA       9   /*无数据                                   */
#define S_STRL_CODE_LNKSTATUS   11   /*以链路状态或访问请求回答请求帧           */
#define S_STRL_CODE_LNKNOSERVER 14   /*链路服务未工作                           */
#define S_STRL_CODE_LNKNOFINISH 15   /*链路服务为完成                           */
/********************************************************************************/

typedef struct{
	/*单帧接收控制内部变量，链路层处理结构*/
	unsigned char	sm;		/*接收状态机，链路层接收完整帧，置0x16，应用层处理完毕置0xff*/
	unsigned char	typ;		/*帧类型*/
	unsigned char	len;
	unsigned char	bp;		/*buf栈指针*/
	unsigned char	fcb;
	unsigned char	dfc;
	unsigned char	err;		/*帧接收错误类型*/
	/*0-L,1-head,2-PRM,3-addr,4-cs,5-rear,6-send err,7-recv time out*/
	unsigned char	rts_dly;
	unsigned char	buf[256];	/*接收缓冲区*/
	unsigned char	send_buf[264];	/*发送队列*/
	unsigned short	send_front;	/*对头发送*/
	unsigned short	send_rear;	/*对尾加入*/
	
	unsigned short	rx_ch;		/*接收总字符数*/
	unsigned short	rx_frm;		/*接收总帧数*/
	unsigned short	tx_ch;		/*发送总字符数*/
	unsigned short	tx_frm;		/*发送总帧数*/
	
	unsigned char 	init_flag;	/*初始化禁止接收标志*/
	unsigned char 	rxfull_cnt; /*继电保护设备接收缓冲区满记数，超过3次强制清0*/
	unsigned short	except_cnt;	/*字符间,帧间及接收处理周期超时判断计数*/
	unsigned short	rxoff_cnt;	/*禁止接收超时判断计数*/
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

#define	RTS_DE		(1)		/*COM板RTS加非门时RTS_DE设为1,表示逻辑上直连*/

#define CLEAR_FILE  1
#define READ_FILE   2

typedef struct
{
	tZzcs5771Data	zzcs;       /*装置参数*/
	tYkpz5700Data	ykpz;		/*保护定值参数*/	
	tYcpz5700Data	ycpz;		/*遥测配置参数*/
	tJdcs5771Data	jdcs;       /*交流精度参数*/
	tJdtz5771Data	jdtz;       /*交流精度调整*/
	tYxcs5700Data	yxcs;       /*遥信参数*/
	tDwcs5700Data	dwcs;       /*未用*/
	tTqcs5700Data	tqcs;		/*未用*/
	tDccs5700Data	dccs;		/*直流参数*/
	tCktxcs5700Data	cktxcs;     /*串口通讯参数*/
	tDctf		dctf;		    /*直流调幅*/
	tDcjd5700Data	dcjd;		/*直流精度参数*/
}tDevPar;

typedef struct  
{
	tDevPar devParm;
    YD_GTCS ydgtcs;
	YD_TSCS ydtscs;
} tDownloadParam;


#endif//_COM103_H

















