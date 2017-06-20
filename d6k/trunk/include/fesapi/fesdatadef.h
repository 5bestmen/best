/*! @file fesdatadef.h
<PRE>
********************************************************************************
模块名       :
文件名       :  fesdatadef.h
文件实现功能 :  前置数据定义
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief     前置数据定义
*  @author    LiJin
*  @version   1.0
*  @date      2016.04.24
*******************************************************************************/
#ifndef FES_DATA_DEF_H
#define FES_DATA_DEF_H

#include "../datatypes.h"
#include "../variant.h"
#include "../public.h"
#include "fescnst.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#pragma pack (1)

/*! \struct  FesInfo
*   \brief 前置基本信息 */
struct FesInfo
{
	int FesId;                                             //!前置ID
	char FesDesc[MAX_NAME_LENGTH + STRING_PAD];           //!前置描述
	char FesTagName[MAX_NAME_LENGTH + STRING_PAD];         //!前置TagName
	char FesVersion[MAX_NAME_LENGTH + STRING_PAD];         //!前置版本
	char FesExt[MAX_NAME_LENGTH + STRING_PAD];             //!前置扩展信息
	char FesGrpName[MAX_NAME_LENGTH + STRING_PAD];         //!前置组名
};

/*! \struct  StringUnit
*   \brief 字符串基本单元 */
struct StringUnit
{
	int32u OccNo;
	char Desc[MAX_NAME_LENGTH + STRING_PAD];
	bool IsTranslate;
	char Language[MAX_NAME_LENGTH + STRING_PAD];
};

/*! \struct  NODE
*   \brief 系统节点定义 */
struct NODE
{
	int32u  OccNo;                //! 关键字 唯一 Id 从1开始
	int32u  GrpNo;                //! 组号
	int32u  NameOccNo;            //! 只读字符串池内的ID序号
	int32u  SlaveOccNo;           //! 如果是主从机，则记住备机的OccNo

	int32u  Prority;               //! 优先级 为今后的N-1冗余方式扩展准备

	/* 实时 */
	int8u IsDefined;      /* YES : is defined, NO: spare */
	int8u ScanEnable;     /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;           /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;        /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */


	int8u RedState;	      /* 0 普通（左网桥）； 1 右网桥 */
	int8u NodeType;       /* 节点类型 */
	int32u State;         /* 主从状态  2-从 7－主 */
	int16u  Net1State;      /* 连接 1 ;断开 0（缺省值） */
	int16u  Net2State;      /* 连接 1 ;断开 0（缺省值）*/

	int32u PointCount;

	/* 加载 */
	int32u NetAIPAddr;     /* IP address of net1, filled by cfg. */
	int32u NetBIPAddr;     /* IP address of net2, filled by cfg. */

	int32u NetAIpV6Addr[4];  //! IPV6 的A网地址
	int32u NetBIpV6Addr[4];  //! IPV6 的B网地址

	int8s TagName[MAX_NAME_LENGTH + STRING_PAD];   //! group name of nodes   == TagName
	int8s HostName[MAX_NAME_LENGTH + STRING_PAD];     /* name of a node */
	int8s GroupName[MAX_NAME_LENGTH + STRING_PAD];   //! 组名，如果是单机，该字段为空，如果主备机，该字段一样。
};

/*! \union  COMM_PARA
*   \brief  通道定义 */
union COMM_PARA
{
	struct SERIAL_PORT
	{
		int32u  MasterComPort;                     /*串口号  1～32对应于COM1～COM32*/
		int32u  SlaveComPort;                      /*备用串口号*/
		int32u  BaudRate;                          /*波特率波特率：110、300、600、1200、2400、4800、9600、14400、19200、28800、38400、56000、57600、115200、128000、256000、500000*/

		int8u  Parity;                            /*校验：0－无校验  1－偶校验 2－奇校验*/
		int8u  DataBit;                           /*数据位：4，5，6，7，8*/
		int8u  StopBit;                           /*停止位：0－1位  1－1.5位  2－2位*/
		int8u  Pad;
	} S;
	struct ETH_IP
	{
		int32u MasterIP;
		int32u SlaveIP;
		int32u MasterPort;
		int32u SlavePort;
	} E;
};

/*! \struct  CHANNEL
*   \brief  通道定义 */
struct CHANNEL
{
	int32u  OccNo;                //! 关键字 唯一 Id 从1开始
	int32u  NameOccNo;            //! 只读字符串池内的ID序号

	int8u   IsDefined;                         /* YES : is defined, NO: spare */
	int8u   ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u   Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u   Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u   PeerQuality;                       /* 备用端口的状态 */
	int8u   Heartbeat;                         /* 心跳 */
	int8u   PrintOn;                           //调试开关：1：输出通道通讯报文，0：不输出
	int8u   WorkNet;                           /*当前工作网络*/

	COMM_PARA CommPara;

	int32u State;
	int8u  Prority;
	int8u  SetTimeAttr;                       /* 端口对时属性 0/1/2 时钟无关/时钟源/受时 */

	// 加载后计算出来的信息
	int32u  DeviceCount;  //! 该通道下装置的数量

	int32u  NodeOccNo;  //! 隶属于哪个节点

	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //通道名称
	char    DriverName[MAX_NAME_LENGTH + STRING_PAD]; //驱动名称

	int8u	DriverType;	  //驱动类型

	int8u	StateCore;	//通道内核状态
	int8u	StateDriver; //通道驱动状态

	int32s  MailBoxID;  //! 通道的邮箱ID
};

/*! \struct  DEVICE
*   \brief  装置定义
	\ 预留考虑定值管理
*/
struct DEVICE
{
	int32u  OccNo;

	int32u  NameOccNo;     //! 只读字符串池内的ID序号
	int32u  NodeOccNo;     //! 隶属于哪个节点
	int32u  ChannelOccNo;  //! 隶属于哪个通道

	int32u  ModelOccNo;     //! 装置引入时用的模板ID 型号标识:DA-R771、772??

	int8u  Type;           //! 装置类型   1－保护测控  2-保护  3－测控 4－管理机 5-其他
	int8u  Pad1;
	int16u Pad2;

	int32u Heartbeat;      //! 心跳信号 由驱动维护

	char   Address[MAX_ADDRESS_LENGTH + STRING_PAD];   //! 装置地址
	char   Extention[MAX_ADDRESS_LENGTH + STRING_PAD]; //! 扩展参数
	// 实时值区
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                        /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
};

/*! \struct  DIN
*   \brief  开关量定义
*/
struct DIN
{
	int32u  OccNo;
	int32u  BlockNo;       //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;     //! 只读字符串池内的ID序号

	int8u   NeedTranmitFlag;  //! 是否需要转发标志，该值不在界面上做配置，由程序判断，如果true，则数值的变化发给前置远动
	int16u  Pad2;
	int8u   IsSpare;       //! 是否备用点，不送往SCADA; 从其他点表导入可能存在无用的备用点

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //点地址

	int8u InitValEnable;    //! 是否启用初始值和初始品质
	int8u InitValue;
	int8u InitQua;

	/*双接点处理功能*/
	int32u SlaveOccNo;					  /*双接点分位节点ID*/
	int8u  DDIFilter; 					  /*双接点位置不对应滤波计数*/
	int8u  DDIFlag;   					  /*双接点位置不对应标志*/
	int8u  DDIInUse;  					  /*双接点处理*/

	/*--------------------------------------实时信息------------------------------------------*/
	int32u State;

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/

	int8u Value;            			    /* DI's Output: 0/1                  */
	int8u NegValue;         		        /* DI's complementery of Output: 1/0 */
	int8u RawValue;                     	//原始值   /* Data collects from inst. , produces */  /* Output/NotOutput based on SiglaType */


	int8u  CtrlByte;                          /* 控制字 */
	int8u  IsSOE;                           /* 0: Non-SOE, 1: SOE **/

	int8u StartCtrl;                          /* filled by cfg. to initilized a sequence*/
	int8u SignalType;                         /* 0 : NORMAL_OPEN; 1: NORMAL_CLOSE  */


	// 统计及计算后整理
	int32u  NodeOccNo;     //! 隶属节点
	int32u  ChannelOccNo;  //! 隶属通道
	int32u  DeviceOccNo;   //! 隶属装置


	int32u  AlarmOccNo;    //! 关联的告警ID
	int32u  DataSource;    //! 数据来源 由组态生成？

	int32u  ExpressOccNo;  //! 计算表标识


	int32u  Desc0_OccNo;
	int32u  Desc1_OccNo;

	int32u  PinLabelOccNo;   //! 端子号描述(字符串)

	TIMEPAK LastUpdateTime;  //! 最近一次更新时间
};

/*! \struct  TRANSFORM_LINEAR
*   \brief  线性转换定义 */
struct TRANSFORM_LINEAR
{
	int32u  OccNo;
	int32u  NameOccNo;   //! 只读字符串池内的ID序号

	int8u EnableInverse; //! 是否反比线性转换
	int8u Pad;

	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	int8u EnableFactor;       //! 启用计算系数
	int8u InverseFactor;      //! 启用反比系数

	fp64  Gain;               //! 增益系数
	fp64  Offset;             //! 偏置系数
};

/*! \struct  TRANSFORM_NONLINEAR
*   \brief  非线性转换定义 */
struct TRANSFORM_NONLINEAR
{
	struct LINEAR_ITEM
	{
		fp64  RawValue;     //! 原始值
		fp64  ScaleValue;   //! 转换后的值
	};

	int32u  OccNo;
	int32u  NameOccNo;   //! 只读字符串池内的ID序号
	int32u  DataCount;    //! 非线性数据有效数目
	LINEAR_ITEM NonLinearData[MAX_NON_LINEAR_NUM];
};

/*! \struct  ALARM_COLOR
*   \brief  报警颜色定义 */
struct ALARM_COLOR
{
	int32u  BackColor;
	int32u  TextColor;
	int32u  BlinkBackColor;
	int32u  BlinkTextColor;
};

/*! \struct  AIN_ALARM
*   \brief  模拟量的报警定义 */
struct  AIN_ALARM
{
	int32u  OccNo;
	int32u  NameOccNo;   //! 只读字符串池内的ID序号

	int8u   ScanEnable;     //! 是否启用
	int8u   OnQualityGood;  //! 测值品质好才告警
	int8u   DeadBandType;   //! 死区的类型，0是相对值，1是绝对值

	fp64    DeadBand;        //! 死区值
	int32u  DescriptionOccNo;  //! 描述
	int32u  LimitCount;      //! 所属的限值告警数量
};
/*! \struct  DIN_ALARM
*   \brief  开关量的报警定义 */
struct  DIN_ALARM
{
	int32u  OccNo;
	int32u  NameOccNo;   //! 只读字符串池内的ID序号

	int8u   ScanEnable;     //! 是否启用
	int8u   OnQualityGood;  //! 测值品质好才告警
	int32u  DescriptionOccNo;  //! 描述
	int32u  LimitCount;      //! 所属的限值告警数量
};
/*! \struct  AIN_ALARM_LIMIT
*   \brief  模拟量报警限值 */
struct  AIN_ALARM_LIMIT
{
	int32u  OccNo;
	int32u  BlockNo;      //! 小排行号 (LSN) ，在告警内的顺序号，值非0
	int32u  AlarmOccNo;   //! 属于哪个报警里面
	int32u  NameOccNo;    //! 只读字符串池内的ID序号

	int8u   ScanEnable;     //! 是否启用
	int8u   Condition;      //! 条件
	int8u   Category;       //! 分类：事故、故障、普通
	int32u  Priority;       //! 优先级	

	int32u  DelayAlarm;     //! 延时，单位为秒
	int8u   EnableAck;
	int8u   EnableDelete;
	int8u   AckType;

	char   SoundFile[MAX_FILENAME_LENGTH];
	int8u  PlaySoundTimes;

	int8u  SpeechAlarmText;
	int8u  Beep;
	char   PushGraphName[MAX_FILENAME_LENGTH];
	int8u  BlinkGraphObject;
	int8u  LogToAlarmWIndow;
	ALARM_COLOR Color;

	int8u  LightBoard;
	char   DispGuide[MAX_FILENAME_LENGTH];
	char   Commands[MAX_NAME_LENGTH];

	int32u  DescriptionOccNo;  //! 描述


	fp64    LimitValue;       //! 报警限值
	fp64    LowValue;         //! 在报警条件为介于xx与xx之间时才有效
};

struct  DIN_ALARM_LIMIT
{
	int32u  OccNo;
	int32u  BlockNo;       //! 小排行号 (LSN) ，在告警内的顺序号，值非0
	int32u  AlarmOccNo;  //! 属于哪个报警里面
	int32u  NameOccNo;   //! 只读字符串池内的ID序号

	int8u   ScanEnable;     //! 是否启用
	int8u   Condition;      //! 条件
	int8u   Category;       //! 分类：事故、故障、普通
	int32u  Priority;       //! 优先级

	int32u  DelayAlarm;     //! 延时，单位为秒
	int8u   EnableAck;
	int8u   EnableDelete;
	int8u   AckType;

	char   SoundFile[MAX_NAME_LENGTH];
	int8u  PlaySoundTimes;

	int8u  SpeechAlarmText;
	int8u  Beep;
	char   PushGraphName[MAX_NAME_LENGTH];
	int8u  BlinkGraphObject;
	int8u  LogToAlarmWIndow;
	ALARM_COLOR Color;

	int8u  LightBoard;
	char   DispGuide[MAX_NAME_LENGTH];
	char   Commands[MAX_NAME_LENGTH];

	int32u  DescriptionOccNo;  //! 描述
};
/*! \struct  AIN
*   \brief  模拟量 定义 */
struct AIN
{
	int32u  OccNo;
	int32u  BlockNo;       //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;     //! 只读字符串池内的ID序号

	int8u   IsSpare;       //! 是否备用点，不送往SCADA; 从其他点表导入可能存在无用的备用点
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //点地址

	/*--------------------------------------实时信息------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/
											//人工置数点，不从装置更新值

	fp64  SignalValue;            /* Output referred signal, assigned by drive */
	fp64  RawValue;	              //！ 原始值
	fp64  Value;                  //！ 工程转换值
	fp64  OldValue;
	//实时值
	int32u   AlarmState[MAX_AIN_ALARMLIMIT_BITCOUNT];              //!  报警状态 按位计算
	int32u   AlarmFlag[MAX_AIN_ALARMLIMIT_BITCOUNT];               //!  报警标识，用于屏蔽报警

	fp64    InitValue;                         /* 工程初始值 */
	int32u  DataSource;                        //! 数据来源 由组态生成？

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*工程正常允许值正向偏移系数 百分数*/
	fp64    LowQua;                             /*工程正常允许值负向偏移系数 百分数*/

	// 转换用
	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	// 统计及计算后整理
	int32u  NodeOccNo;      //! 隶属节点
	int32u  ChannelOccNo;   //! 隶属通道
	int32u  DeviceOccNo;    //! 隶属通道	

	int32u  AlarmOccNo;     //! 关联的告警，一个告警内部有多个限值告警
	int32u  ExpressOccNo;   //! 计算表标识

	int8u   TransformType;  //! 转换类型(线性，非线性)
	int32u  TransformOccNo; //! 转换关系标识

	int32u  DescriptionOccNo;  //! 测点描述
	int32u  PinLabelOccNo;  //! 端子号
	int32u  UnitNameOccNo;  //! 单位名称

	// 还缺 转换关系，有效值范围等等

	TIMEPAK LastUpdateTime;  //! 最近一次更新时间
};
/*! \struct  DOUT
*   \brief  遥控 定义 */
struct DOUT
{
	int32u  OccNo;
	int32u  BlockNo;       //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;     //! 只读字符串池内的ID序号

	int8u   IsSpare;       //! 是否备用点，不送往SCADA; 从其他点表导入可能存在无用的备用点
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //! 点地址

	/*--------------------------------------实时信息------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */

	int8u Output;                            /* DI's Output: 0/1                  */
	int8u RelayState;                        /* collected from modules specifying real output */
	int8u CtrlType;                          //0: Select  1:Sel Ok / Exec  2:Cancel
	int8u Filter;                            //滤波
	int32u DataSource;                //! 数据来源 由组态生成？

	int8u  IsSBO;                     //! SBO类型？
	int8u  BlockSignalType;           //! 闭锁信号类型
	int32u BlockingSignalOccNo;       //! 闭锁信号，可以是IO，也可以是虚拟变量，但是必须是本节点

	int32u  FeedbackOccNo;   //! 关联的反馈遥信信号
	int8u   FeedbackType;    //! 是用0，还是1来认定遥控成功？

	int32u BlockingProgramOccNo;      //! 本节点上闭锁程序的排行号 
	int32u CheckOK;

	int32u StartProgramOccNo;         //! 启动一段程序，而非实际遥控

	// 统计及计算后整理
	int32u  NodeOccNo;     //! 隶属节点
	int32u  ChannelOccNo;  //! 隶属通道
	int32u  DeviceOccNo;   //! 隶属通道

	int32u  AlarmOccNo;    //! 关联的告警ID
	int32u  ExpressionOccNo;  //! 计算表标识

	int32u  PinLabelOccNo;   //! 端子号

	TIMEPAK LastUpdateTime;  //! 最近一次更新时间
};
/*! \struct  AOUT
*   \brief  模出 定义 */
struct AOUT
{
	int32u  OccNo;
	int32u  BlockNo;       //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;   //! 只读字符串池内的ID序号

	int8u   IsSpare;       //! 是否备用点，不送往SCADA; 从其他点表导入可能存在无用的备用点
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //! 点地址
	/*--------------------------------------实时信息------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */

	fp64    Output;                            /* AIN's output, caculated by drive or exp. */
	fp64    RawData;                           /* data collects from instrument  */
//	fp64         SignalValue;                       /* Output referred signal, assigned by drive */
	int32u  DataSource;     //! 数据来源 由组态生成？

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*工程正常允许值正向偏移系数 百分数*/
	fp64    LowQua;                             /*工程正常允许值负向偏移系数 百分数*/

	fp64   MaxRaw;
	fp64   MinRaw;

	fp64   MaxScale;
	fp64   MinScale;

	// 统计及计算后整理
	int32u  NodeOccNo;      //! 隶属节点
	int32u  ChannelOccNo;   //! 隶属通道
	int32u  DeviceOccNo;    //! 隶属通道

	int32u  AlarmOccNo;     //! 关联的告警ID
	int32u  ExpressOccNo;   //! 计算表标识

	int8u   TransformType;    //! 转换类型(线性，非线性)
	int32u  TransformOccNo;   //! 转换关系标识

	int32u  PinLabelOccNo;     //! 端子号
	int32u  UnitNameOccNo;     //! 单位名称

	TIMEPAK LastUpdateTime;    //! 最近一次更新时间
};
/*! \struct  EXP
*   \brief  表达式 定义 */
struct EXP
{
	int32u  OccNo;
	int32u  BlockNo;       //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;     //! 只读字符串池内的ID序号

	int32u  VarID;                           /*计算返回测点ID(后台DIN.ID)*/
	int32u  TableType;                      /*计算返回测点表号(后台IDD_DIN...)*/
	int32u  MidCodeLen;                      /*计算脚本长度*/
	char*	MidCode;				           /*计算脚本执行码*/

};

struct TAGITEM
{
	int32u NodeOccNo;  //! 节点
	int32u IddType;    //! 测点类型 
	int32u TagOccNo;   //! 测点排行
	int32u FiledID;    //! 测点属性
};
/*! \struct  VARDATA
*   \brief  内存变量 定义 */
struct VARDATA
{
	int32u  OccNo;
	int32u  BlockNo;        //! 板内测点排行号 (LSN) ，在装置内的顺序号，可以不连续，值非0
	int32u  NameOccNo;      //! 只读字符串池内的ID序号
	int32u  NodeOccNo;      //! 隶属节点

	int32u  AlarmOccNo;     //! 关联的告警ID，根据下面的IddType判断，如果是开关量则关联的是模拟量的告警，如果是开关量，则关联开关量的告警
	int32u  ExpressOccNo;   //! 计算表标识

	int8u   DataType;       //! 数据类型：bool、byte、char...
	int8u   IddType;        //! DIN\AIN\AOUT\DOUT\  或者空

	int32u  SrcNodeOccNo;
	int32u  SrcOccNo;
	int8u   SrcIddType;
	int8u   IsRefTag;     //! 是否为引用变量

	IO_VARIANT InitValue;  //! 初始值
	int8u      InitQua;    //! 初始品质
	int8u      IsSOE;                            /* 0: Non-SOE, 1: SOE **/

	/*--------------------------------------实时信息------------------------------------------*/
	int32u State;

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/

	IO_VARIANT Value;            			    /* DI's Output: 0/1                  */
	IO_VARIANT RawValue;                     	//原始值   /* Data collects from inst. , produces */  /* Output/NotOutput based on SiglaType */

	int8u  NegValue;         		        /* DI's complementery of Output: 1/0 */
	int8u  CtrlByte;                          /* 控制字 */
	
	int8u StartCtrl;                          /* filled by cfg. to initilized a sequence*/
	int8u SignalType;                         /* 0 : NORMAL_OPEN; 1: NORMAL_CLOSE  */

	int32u  DataSource;                       //! 数据来源 由组态生成？

	int32u  Desc0OccNo;
	int32u  Desc1OccNo;

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*工程正常允许值正向偏移系数 百分数*/
	fp64    LowQua;                             /*工程正常允许值负向偏移系数 百分数*/

	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	TIMEPAK LastUpdateTime;  //! 最近一次更新时间
};

/*! \struct  FT_CHANNEL
*   \brief  转发通道定义 */
struct FT_CHANNEL
{
	int32u  OccNo;                //! 关键字 唯一 Id 从1开始
	int32u  NameOccNo;            //! 只读字符串池内的ID序号

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u PeerQuality;                       /* 备用端口的状态 */
	int8u Heartbeat;                         /* 心跳 */
	int8u PrintOn;                           //调试开关：1：输出通道通讯报文，0：不输出
	int8u WorkNet;                           /*当前工作网络*/

	COMM_PARA CommPara;

	int32u State;
	int8u Prority;

	// 加载后计算出来的信息

	int32u  NodeOccNo;  //! 隶属于哪个节点

	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //通道名称
	char    DriverName[MAX_NAME_LENGTH + STRING_PAD]; //驱动名称

	int8u	DriverType;	  //驱动类型

	int8u	StateCore;	//通道内核状态
	int8u	StateDriver; //通道驱动状态

	int32s  MailBoxID;  //! 通道的邮箱ID
};


/*! \struct  ATTDEF
*   \brief  基础对象属性定义 */
struct ATTDEF
{
	int32u      Index;       //! 序号
	int32u      ByteOffset;  //! 结构体内的偏移
	int16u      DataType;   //! 数据类型 
	int16u      Att;        //! 属性号

	int16u      ArraySize;  //! 数组长度
	int16u      RW;         //! 读写属性 
	char        AttName[MAX_ATTNAME_LENGTH];
	char        Description[MAX_NAME_LENGTH];
};

enum MSG_CODE
{
	MSG_BASE = 0,       //!
	MSG_ALARM = 1,      //! 通用告警
	MSG_AIN_ALARM = 2,
	MSG_DIN_ALARM = 3,
	MSG_DIAG,   //! 自诊断告警
	MSG_OPER,   //! 操作告警
	MSG_SETVAL,        //! 设值


};

enum
{
	MSG_LEN = 1024,
};
/*! \struct  BASE_MSG
*   \brief  前置内部的消息定义，用于节点内部的各个模块间消息交互
*   \details 每个消息最大1024，可以见mail.h中定义
*/
struct BASE_MSG
{
	int16u MsgCode;   //! 用于区分这是哪一种类型的消息
	int16u Len;       //! 整封消息的长度 <= 1024

	int8u  Data[MSG_LEN - sizeof(int16u) * 2];
};

/*! \struct  ALARM_MSG_H
*   \brief  通用告警事件信文  */
struct ALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u	EvtCode;      //! 报警信息代码
	int32u  NodeOccNo;

	int32u	OccNo;

	int16u	AlarmFlag;         //最高位为1不允许报警,为0允许报警
	int16u  AlarmState;        //对开关量无意义;对模拟量,
	IO_VARIANT	Value;
};
/*! \struct  ALARM_MSG
*   \brief  通用告警事件信文 */
struct ALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u	EvtCode;      //! 报警信息代码
	int32u  NodeOccNo;

	int32u	OccNo;

	int16u	AlarmFlag;         //最高位为1不允许报警,为0允许报警
	int16u  AlarmState;        //对开关量无意义;对模拟量,
	IO_VARIANT	Value;

	int8u  ExtraData[MSG_LEN - sizeof(ALARM_MSG_H)];
};

/*! \struct  AINALARM_MSG_H
*   \brief  模拟量告警事件信文 */
struct AINALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! 报警信息代码
	int32u	OccNo;        //! 模拟量点号

	int32u  AlarmOccNo;     //! 模拟量告警的排行号
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! 最高位为1不允许报警,为0允许报警
	int16u  AlarmState;    //! 对开关量无意义;对模拟量,
	fp64	Value;         //! 当前测值	 
};
/*! \struct  AINALARM_MSG
*   \brief  模拟量告警事件信文 */
struct AINALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! 报警信息代码
	int32u	OccNo;        //! 模拟量点号

	int32u  AlarmOccNo;     //! 模拟量告警的排行号
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! 最高位为1不允许报警,为0允许报警
	int16u  AlarmState;    //! 对开关量无意义;对模拟量,
	fp64	Value;         //! 当前测值

	int8u  ExtraData[MSG_LEN - sizeof(AINALARM_MSG_H)];
};

/*! \struct  DINALARM_MSG_H
*   \brief  开关量告警事件信文 */
struct DINALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! 报警信息代码
	int32u	OccNo;        //! 模拟量点号

	int32u  AlarmOccNo;     //! 模拟量告警的排行号
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! 最高位为1不允许报警,为0允许报警
	int16u  AlarmState;    //! 对开关量无意义;对模拟量,
	int8u	Value;         //! 当前测值
};
/*! \struct  DINALARM_MSG
*   \brief  开关量告警事件信文 */
struct DINALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! 报警信息代码
	int32u	OccNo;        //! 模拟量点号

	int32u  AlarmOccNo;     //! 模拟量告警的排行号
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! 最高位为1不允许报警,为0允许报警
	int16u  AlarmState;    //! 对开关量无意义;对模拟量,
	int8u	Value;         //! 当前测值

	int8u  ExtraData[MSG_LEN - sizeof(DINALARM_MSG_H)];
};

/*! \struct  DIAG_MSG_H
*   \brief  自诊断告警事件信文 */
struct DIAG_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u  ChannelOccNo;
	int32u  DeviceOccNo;
};
/*! \struct  DIAG_MSG
*   \brief  自诊断告警事件信文 */
struct DIAG_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u  ChannelOccNo;
	int32u  DeviceOccNo;

	int8u  ExtraData[MSG_LEN - sizeof(DIAG_MSG_H)];
};
/*! \struct  OPER_MSG
*   \brief  操作告警事件信文 */
struct OPER_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u  ChannelOccNo;
	int32u  DeviceOccNo;
};
/*! \struct  OPER_MSG
*   \brief  操作告警事件信文 */
struct OPER_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u  ChannelOccNo;
	int32u  DeviceOccNo;

	int8u  ExtraData[MSG_LEN - sizeof(OPER_MSG_H)];
};

/*! \struct  FE_MSG_SETVAL
*   \brief  设值 及 设值完成信文 */
struct SETVAL_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u	IddType;
	int8u	Att;  //! 哪个属性
	int16u  Pad1;

	int32u	NodeOccNo;
	int32u	Occno;

	int8u	Datatype;
	int8u   Pad2;     /*MSG_SETVLUDONE 0:选择或撤销成功 1:选择或撤销失败 */
	int8u   Source1; /*** see lcucnst.h ***/
	int8u   Source2;
	IO_VARIANT	Value[2];/*CTRL: 1执行；2选择；3选择后执行；4撤销*/
};

/*! \struct  REQDATA_MSG
*   \brief  请求报文，用于定值召唤、录波召唤等等 */
struct REQDATA_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u   ExtraData[MSG_LEN - sizeof(int16u) * 2];

};
/*! \struct  RESPDATA_MSG
*   \brief  应答报文 */
struct RESPDATA_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u   ExtraData[MSG_LEN - sizeof(int16u) * 2];
};

enum NBM_MSG_TYPES
{
	NBMT_MSG_SYNCH = 0,			 // 消息同步类型
	NBMT_MSG_EVENT,				 // 事项消息类型
	NBMT_CONTROL_EVENT,			 // 遥控消息类型

	SEND_SWITCH_CMD_REQ = 10010, // 发送切换报文
	RECV_SWITCH_CMD_RES = 20010, // 接收响应
};

enum FE_MCODE
{
	MT_SETVALUE = 0X03,       //!< 设值
	MT_SETVALUE_DONE = 0X04,  //!< 设值完成
	MT_NORMAL_EVENT = 0X50,   //!< 普通事件
	MT_OPER = 0X5A,    //!< 控制操作调节
	MT_DIAG = 0X5B,    //!< 自诊断信息
	MT_RELAY = 0X5D,    //!< 保护事件
	MT_RELAY_EXT = 0X5E, //!< 保护扩展事件 
	MT_RELAY_DIAG = 0X5F, //!< 保护自诊断
};

/*! \struct  FES_CMD
*   \brief  遥控、遥调命令 */
struct FES_CMD
{
	int16u	MsgCode;
	int16u	Len;

	int8u	IddType;

	int32u NodeOccNo;
	int32u ChannelOccNo;
	int32u OccNo;

	IO_VARIANT	Value;
};

#pragma pack ()

#endif // FES_DATA_DEF_H


/** @}*/

