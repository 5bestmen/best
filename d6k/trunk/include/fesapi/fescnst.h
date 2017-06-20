/*! @file fescnst.h
<PRE>
********************************************************************************
模块名       :
文件名       :  fescnst.h
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
#ifndef FES_DATA_CONST_H
#define FES_DATA_CONST_H

#include "../datatypes.h"
#include "../variant.h"
#include "../public.h"

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


enum IDD_TYPE : int32u
{
	IDD_NULL = 0,
	IDD_AIN = 1,  //!< 模入量
	IDD_DIN = 2,  //!< 开入量
	IDD_AOUT = 3, //!< 模出量 
	IDD_DOUT = 4, //!< 开出
	IDD_USERVAR = 5, //!< 用户自定义变量
	IDD_SYSVAR = 6,  //!< 系统变量


	IDD_NODE = 7,
	IDD_CHANNEL = 10,
	IDD_DEVICE = 11,
	IDD_OBJECT = 12,  //!< 对象
	IDD_OBJECT_VAR = 13, //!< 对象内部变量

	IDD_AINALARM = 15,
	IDD_DINALARM = 16,

	IDD_AINALARMLIMIT = 17,
	IDD_DINALARMLIMIT = 18,

	IDD_TRANSLINEAR = 19,
	IDD_TRANSNONLINEAR = 20,

	IDD_FTCHANNEL = 21,  //!< 基于前置的远动转发
	IDD_FAPP = 22, //!< 基于前置的应用模块
	IDD_SAPP = 23, //!< 基于SCADA的应用模块

	MAX_IDD = IDD_SAPP,
};

#if 0
enum DATA_TYPE 
{
	DT_NULL = 0,
	DT_BOOLEAN = 1,	  //!< 布尔，8位，缺省值0
	DT_CHAR = 2,      //!< 有符号字节（8位）
	DT_BYTE = 3,      //!< 有符号字节（8位）
	DT_SHORT = 4,     //!< 无符号短整数 ，16位， 
	DT_WORD = 5,      //!< 无符短整数 ，16位
	DT_INT = 6,       //!< 有符号长整数 32位
	DT_DWORD = 7,      //!< 无符号长整数 32位
	DT_LLONG = 8,      //!< 有符号长整数  64位
	DT_LWORD = 9,      //!< 无符号长整数  64位
	DT_FLOAT = 10,
	DT_DOUBLE = 11,
	DT_STRING = 12,
	DT_BLOCK = 13,
};
#endif // 0

enum IO_DATASOURCE
{
	IO_LCU = 1,         //!< 通道中采集得到
	IO_HOSTCALU = 2,	//!< 后台语言赋值 
};

enum NODE_TYPE
{
	NODE_NULL =   0 , //!< 无效节点
	NODE_SVR    = 1,  //!< 服务器
	NODE_FES    = 2,  //!< 前置机
	NODE_CLIENT = 3,  //!< 工作站
};

/*! \enum NODE_STATE  fescnst.h  "fescnst.h"
*  \brief 节点的工作状态.
*/
enum NODE_STATE
{
	STATE_UNKOWN = 0, //!< 未定状态
	STATE_MAIN  = 1,  //!< 主机运行
	STATE_SLAVE = 2,  //!< 从机运行
};

enum  ALARM_TYPE
{
	OPER_LOG = 1,  //!< 操作
	DIAG_LOG = 2,  //!< 自诊断
	CTRL_LOG = 3, //!< 控制调节
	RELAYDIAG_LOG = 4  //!< 保护自诊断
};

enum CHANNEL_PRINT 
{
	PRINT_OFF = 0, //!< 不打印报文，默认情况下为此值
	PRINT_ON = 1,  //!< 打印报文，一般只有在调试时打开此选项
};

enum OCCNO_LIMIT
{
	INVALID_OCCNO = 0,
	MAX_OCCNO = 2000000,

	MAX_NODE_COUNT = 2048,
	MAX_NODE_OCCNO = MAX_NODE_COUNT,

	MAX_CHANNEL_COUNT = 100000,   //!< 最多通道数量
	MAX_CHANNEL_OCCNO = MAX_CHANNEL_COUNT,

	MAX_DEVICE_COUNT = 100000,    //!< 最多装置数量
	MAX_DEVICE_OCCNO = MAX_DEVICE_COUNT,

	MAX_AIN_ALARMLIMIT_BITCOUNT = 5,  //! 按位计算，5*32 = 160
	MAX_AIN_ALARM_LIMIT_COUNT = 160,  //! 一个ALARM内LIMIT的数量限制
	MAX_DIN_ALARM_LIMIT_COUNT = 2,    //! 一个开关量报警只能有2个
	MAX_ALARM_OCCNO = MAX_OCCNO,
};

#define  STRING_PAD  sizeof(size_t)

enum  TEXT_LENGTH:size_t
{
	MAX_ADDRESS_LENGTH = 128,
	MAX_ATTNAME_LENGTH = 128,
	MAX_FILENAME_LENGTH = 128,
	MAX_NAME_LENGTH = 512,
};

enum AIN_TRANSFORM
{
	TRANSFORM_TYPE_LINEAR = 0,
	TRANSFORM_TYPE_NONLINEAR = 1,
};

enum
{
	MAX_NON_LINEAR_NUM = 64,
};

enum DEADBAND_TYPE
{
	DEADBAND_RELATIVE = 0,  //!< 相对量程，默认值
	DEADBAND_ABSOLUTE = 1,  //!< 绝对值
};

enum AIN_ALARM_LIMIT_CONDITION
{
	CONDITION_GE  = 0,    //!< 大于等于
	CONDITION_LE  = 1,    //!< 小于等于
	CONDITION_EQUAL = 3,  //!< 等于
	CONDITION_NEQUAL = 4,  //!< 不等于
	CONDITION_RATE_INC = 5, //!< 变化率增加
	CONDITION_RATE_DEC = 6
};

enum DIN_ALARM_LIMIT_CONDITION
{
	CONDITION_021 = 0,    //!< 从0到1变化
	CONDITION_120 = 1,    //!< 从1到0变化
};

enum FB_ENUM :int8u
{
	FES_NO = 0,
	FES_YES = 1,

	UNINITED = 0,
	INITED = 1,
	DEFAULT_INIT = UNINITED,

	NORMAL_OPEN = 0,
	NORMAL_CLOSE = 1,

	SCAN_OUT = 0, //测点退出
	SCAN_IN = 1, //测点投入
  	DEFAULT_SCAN = SCAN_IN,
 	DEFAULT_SCANTIME = 10,
	QUALITY_BAD = 1,
	QUALITY_OK = 0,
	DEFAULT_QUA = QUALITY_OK,

	MANSET = 1,//开入模入测点 进入人工设置态
	NORMAL = 0,//

	AIN_ALARM_ON = 1 , //! 越限
	AIN_ALARM_OFF = 0, //! 复限

};

enum LANG
{
	LANG_NONE = 1,
	LANG_EN,
	LANG_zh_CN,
};

//驱动类型
enum
{
	DRIVER_TYPE_DLL,
	DRIVER_TYPE_EXE,
};

//驱动状态值
enum
{
	DRIVER_STATE_UNKNOWN,
	DRIVER_STATE_LOAD,
	DRIVER_STATE_UNLOAD,
};


/*! \enum TAG_ATT  fescnst.h  "fescnst.h"
*  \brief 通用属性.
*/
enum TAG_ATT
{
	ATT_IN_OUT = 1,      // 投退操作（1：投入； 0：退出； 设置到LCU前置机，对应ScanEnable）      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATT_VALUE = 2,	     // 实测值   熟数据                          (AI/DI/VAR)   人工置实测值 （当投退标志＝1时，允许设置到LCU前置机）(AI/DI)
	ATT_QUA = 3,         // 品质（0：好；1：坏）  
	ATT_STATE = 4,       // 状态字        (AI/DI/DO/AO ...)
	ATT_MANSET = 5,      // 人工设置标志（0：无； 1：人工设置）      (AI/DI)
	ATT_ALMDIS = 6,      // 抑制告警（0：允许； 1：抑制）            (AI/DI)
	ATT_CTRLENA = 7,     // 遥控条件检查OK（0：闭锁； 1：OK）        (CTRL)

	ATT_MINOUTPUT = 8,   // Low信号工程值范围 
	ATT_MAXOUTPUT = 9,   // High信号工程值范围                       (AI/AO）

	ATT_HIQUA = 10,      // 工程正常允许值正向偏移系数               (AI/AO)
	ATT_LOQUA = 11,      // 工程正常允许值负向偏移系数               (AI/AO)
	ATT_UNIT = 12,       // 计量单位                                 (AI/AO/)

	ATT_SIGNAL_VALUE = 13,
	ATT_RAW_VALUE = 14,
	ATT_OLD_VALUE = 15,

	ATT_PINLABEL = 16,        // 端子号说明  
  	
	ATT_DESCRIPTION = 17,     // 变量描述                                (AI/DI/DO/AO/PID/CTRL/PI)
	ATT_STATE0 = 18,          // 0态描述                                 (DI) 
	ATT_STATE1 = 19,          // 1态描述                                 (DI)
	ATT_NETA = 20,            // 模件的A网状态                           (MODULE/NODE)  
	ATT_NETB = 21,            // 模件的B网状态                           (MODULE/NODE)
	ATT_DOUT = 22,            // 开出输出                                (DO)
	ATT_AOUT = 23,            // 模出输出                                (AO)
	ATT_MAXRANGE = 24,        // 最大允许设值范围                        (AI/AO/)
	ATT_MINRANGE = 25,        // 最小允许设值范围                        (AI/AO/)
	ATT_ISMAIN  = 26,          // 节点主从状态                            (NODE)

	ATT_MAINT = 27,           // 检修牌  （1：挂牌； 0：摘牌； 设置到数据服务器）     (CTRL/EQUIP)
	ATT_START = 28,           // 启动控制（1：启动；           设置到LCU前置机）      (CTRL)
	ATT_ALMCHECK = 29,        // 告警确认（1：确认；           设置到数据服务器）     (AI/DI)
	ATT_REDSWITCH = 30,       // 节点主从切换                                         (NODE)
	ATT_GETRELAYVALUE = 31,   // 召唤定值                                             (MODULE)
	ATT_SETRELAYVALUE = 32,   // 设定值                                               (MODULE)
	ATT_CONFIRMSET = 33,      // 确认修改定值                                         (MODULE)
	ATT_CANCELSET  = 34,       // 取消修改定值                                         (MODULE)
	ATT_RESETRELAY = 35,      // 保护复归                                             (MODULE)
	ATT_TEST       = 36,      // 数据测试                                             (AI/DI)后台使用
	ATT_TESTCANCEL = 37,      // 取消数据测试                                         (AI/DI)后台使用
	ATT_MAX        = 38
};
  
/*! \enum TAG_ATTR  fescnst.h  "fescnst.h"
*  \brief 可读属性. FieldID
*/
enum TAG_ATTR
{
	ATTR_IN_OUT = 1,      // 投退操作（1：投入； 0：退出； 设置到LCU前置机，对应ScanEnable）      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATTR_VALUE = 2,	     // 实测值   熟数据                        (AI/DI/VAR)   
	ATTR_QUA = 3,         // 品质（0：好；1：坏）  
	ATTR_STATE = 4,       // 状态字        (AI/DI/DO/AO ...)
	ATTR_MANSET = 5,      //人工设置标志（0：无； 1：人工设置）      (AI/DI)
	ATTR_ALMDIS = 6,      //抑制告警（0：允许； 1：抑制）            (AI/DI)
	ATTR_CTRLENA = 7,     //遥控条件检查OK（0：闭锁； 1：OK）        (CTRL)

	ATTR_MINOUTPUT = 8,   //Low信号工程值范围 
	ATTR_MAXOUTPUT = 9,   //High信号工程值范围                      (AI/AO

	ATTR_HIQUA = 10,      //工程正常允许值正向偏移系数              (AI/AO)
	ATTR_LOQUA = 11,      //工程正常允许值负向偏移系数              (AI/AO)
	ATTR_UNIT = 12,       //计量单位                                (AI/AO/)

	ATTR_SIGNAL_VALUE,
	ATTR_RAW_VALUE,
	ATTR_OLD_VALUE,

	ATTR_PINLABEL,             //端子号说明  
	ATTR_DESCRIPTION,     //变量描述                                (AI/DI/DO/AO/PID/CTRL/PI)
	ATTR_STATE0,          //0态描述                                 (DI) 
	ATTR_STATE1,          //1态描述                                 (DI)
	ATTR_NETA,            //模件的A网状态                           (MODULE/NODE)  
	ATTR_NETB,            //模件的B网状态                           (MODULE/NODE)
	ATTR_DOUT,            //开出输出                                (DO)
	ATTR_AOUT,            //模出输出                                (AO)
	ATTR_MAXRANGE,        //最大允许设值范围                        (AI/AO/)
	ATTR_MINRANGE,        //最小允许设值范围                        (AI/AO/)
	ATTR_ISMAIN,          //节点主从状态                            (NODE)

	ATTR_MAINT,           // 检修牌  （1：挂牌； 0：摘牌； 设置到数据服务器）     (CTRL/EQUIP)
	ATTR_START,           // 启动控制（1：启动；           设置到LCU前置机）      (CTRL)
	ATTR_ALMCHECK,        // 告警确认（1：确认；           设置到数据服务器）     (AI/DI)
	ATTR_REDSWITCH,       // 节点主从切换                                         (NODE)
	ATTR_GETRELAYVALUE,   // 召唤定值                                             (MODULE)
	ATTR_SETRELAYVALUE,   // 设定值                                               (MODULE)
	ATTR_CONFIRMSET,      // 确认修改定值                                         (MODULE)
	ATTR_CANCELSET,       // 取消修改定值                                         (MODULE)
	ATTR_RESETRELAY,      // 保护复归                                             (MODULE)
	ATTR_TEST,            // 数据测试                                             (AI/DI)后台使用
	ATTR_TESTCANCEL,      // 取消数据测试                                         (AI/DI)后台使用
	ATTR_MAX
};
 
/*! \enum TAG_ATTW  fescnst.h  "fescnst.h"
*  \brief 可读写属性. FieldID
*/
enum TAG_ATTW
{
	ATTW_IN_OUT = 1,      // 投退操作（1：投入； 0：退出； 设置到LCU前置机，对应ScanEnable）      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATTW_VALUE = 2,	      // 人工置实测值 （当投退标志＝1时，允许设置到LCU前置机）(AI/DI)
	ATTW_QUA = 3,         // 品质（0：好；1：坏）  
	ATTW_MANSET = 5,      //人工设置标志（0：无； 1：人工设置）      (AI/DI)
	ATTW_ALMDIS = 6,      //抑制告警（0：允许； 1：抑制）            (AI/DI)
	ATTW_CTRLENA = 7,     //遥控条件检查OK（0：闭锁； 1：OK）        (CTRL)

	ATTW_DOUT = 22,            //开出输出                                (DO)
	ATTW_AOUT = 23,            //模出输出                                (AO)

	ATTW_MAINT = 27,           // 检修牌  （1：挂牌； 0：摘牌； 设置到数据服务器）     (CTRL/EQUIP)
	ATTW_START = 28,           // 启动控制（1：启动；           设置到LCU前置机）      (CTRL)
	ATTW_ALMCHECK = 29,        // 告警确认（1：确认；           设置到数据服务器）     (AI/DI)
	ATTW_REDSWITCH = 30,       // 节点主从切换                                         (NODE)
	ATTW_GETRELAYVALUE = 31,   // 召唤定值                                             (MODULE)
	ATTW_SETRELAYVALUE,   // 设定值                                               (MODULE)
	ATTW_CONFIRMSET,      // 确认修改定值                                         (MODULE)
	ATTW_CANCELSET,       // 取消修改定值                                         (MODULE)
	ATTW_RESETRELAY,      // 保护复归                                             (MODULE)
	ATTW_TEST,            // 数据测试                                             (AI/DI)后台使用
	ATTW_TESTCANCEL,      // 取消数据测试                                         (AI/DI)后台使用
	ATTW_MAX
};

#endif // FES_DATA_CONST_H


/** @}*/

