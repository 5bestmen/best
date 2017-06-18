#ifndef RELAY_103_GEN_H
#define RELAY_103_GEN_H

#include <stdint.h>
#include <functional>

/************************************************************************/
//		GDD Data Type定义
/************************************************************************/
#define IEC103_GDD_TYPE_NONE			0    //!< 无数据
#define IEC103_GDD_TYPE_ASCII			1    //!< ASCII8位码
#define IEC103_GDD_TYPE_BS1			    2    //!< 成组8位串
#define IEC103_GDD_TYPE_UINT			3    //!< 无符号整数
#define IEC103_GDD_TYPE_INT			    4    //!< 整数
#define IEC103_GDD_TYPE_UFLOAT			5    //!< 无符号浮点数
#define IEC103_GDD_TYPE_FLOAT			6    //!< 浮点数
#define IEC103_GDD_TYPE_R3223			7    //!< IEEE标准754短实数
#define IEC103_GDD_TYPE_R6453			8    //!< IEEE标准754实数
#define IEC103_GDD_TYPE_DPI			    9    //!< 双点信息
#define IEC103_GDD_TYPE_SPI			    10   //!< 单点信息
#define IEC103_GDD_TYPE_DPI_T_R		    11   //!< 带瞬变和差错的双点信息
#define IEC103_GDD_TYPE_MEA_Q			12   //!< 带品质描述词的被测值

#define IEC103_GDD_TYPE_BIN_TIME		14   //!< 二进制时间
#define IEC103_GDD_TYPE_GIN			    15   //!< 通用分类标识序号
#define IEC103_GDD_TYPE_REL_TIME		16   //!< 相对时间
#define IEC103_GDD_TYPE_FUN_INF		    17   //!< 功能类型和信息序号
#define IEC103_GDD_TYPE_M_TM_TA		    18   //!< 带时标的报文
#define IEC103_GDD_TYPE_M_TMR_TA		19   //!< 带相对时间的时标报文
#define IEC103_GDD_TYPE_M_TME_TA		20   //!< 带相对时间的时标的被测值
#define IEC103_GDD_TYPE_EXT_TEXT_NUM	21   //!< 外部文本序号
#define IEC103_GDD_TYPE_GRC		        22   //!< 通用分类回答码
#define IEC103_GDD_TYPE_DATA_STRUCT  	23   //!< 数据结构
#define IEC103_GDD_TYPE_INDEX			24   //!< 索引

#define IEC103_GDD_TYPE_PULSE           201  //!< 脉冲量
#define IEC103_GDD_TYPE_TAP_SHORT       202  //!< 档位(短时标)
#define IEC103_GDD_TYPE_CTLCMD	        206	 //!< 外扩 - 控制命令
#define IEC103_GDD_TYPE_TAP             207  //!< 档位

#define IEC103_GDD_MAX_LEN              256  //!< 通用分类数据最大长度

//四个8位位组时间
typedef struct _CP32TIME2a
{
	uint8_t msL;   //!< 毫秒L
	uint8_t msH;   //!< 毫秒H
	uint8_t min;   //!< 分
	uint8_t hour;  //!< 小时
}CP32TIME2a;

//七个8位位组时间
typedef struct _CP56TIME2a
{
	uint8_t msL;   //!< 毫秒L
	uint8_t msH;   //!< 毫秒H
	uint8_t min;   //!< 分
	uint8_t hour;  //!< 小时
	uint8_t day;   //!< 日
	uint8_t month; //!< 月
	uint8_t year;  //!< 年
}CP56TIME2a;

enum KOD_TYPE
{
	IEC103_KOD_NONE             = 0,    //!< 无所制定的描述类别
	IEC103_KOD_ACTUAL_V         = 1,    //!< 实际值
	IEC103_KOD_DEFAULT_V        = 2,    //!< 缺省值
	IEC103_KOD_RANGE            = 3,    //!< 量程

	IEC103_KOD_PRECISION        = 5,    //!< 精度
	IEC103_KOD_FACTOR           = 6,    //!< 因子
	IEC103_KOD_REF_PERCENT      = 7,    //!< %参比
	IEC103_KOD_ENUM             = 8,    //!< 列表
	IEC103_KOD_UNIT             = 9,    //!< 量纲
	IEC103_KOD_DESCRIPTION      = 10,   //!< 描述

	IEC103_KOD_PASSWORD         = 12,   //!< 口令条目
	IEC103_KOD_READ_ONLY        = 13,   //!< 只读
	IEC103_KOD_WRITE_ONLY       = 14,   //!< 只写

	IEC103_KOD_FUN_INF          = 19,   //!< 相应的功能类型和信息序号
	IEC103_KOD_EVENT            = 20,   //!< 相应的事件
	IEC103_KOD_ENUM_TEXT_ARRAY  = 21,   //!< 列表的文本阵列
	IEC103_KOD_ENUM_VALUE_ARRAY = 22,   //!< 列表的值阵列
	IEC103_KOD_RELATED          = 23,   //!< 相关联的条目
	MAX_IEC103_KOD_TYPE
};

/************************************************************************/
//		通用分类回答码
/************************************************************************/
enum GEN_REPLY_CODE
{
	IEC103_GRC_OK,             //!< 认可
	IEC103_GRC_GIN_UNAVAI,     //!< 无效的通用分类标识序号(GIN)
	IEC103_GRC_DATA_UNEXIST,   //!< 不存在所请求的数据
	IEC103_GRC_DATA_UNAVAI,    //!< 数据不能用过后再来一次
	IEC103_GRC_CONFIRM_ERR,    //!< 改变设定时确认出错
	IEC103_GRC_OVERRANGE,      //!< 改变设定时超出量程
	IEC103_GRC_ENERY,          //!< 条目的范围太大
	IEC103_GRC_COMMAND,        //!< 太多的命令
	IEC103_GRC_READONLY,       //!< 条目是只读
	IEC103_GRC_PASSWORD,       //!< 设定受口令保护
	IEC103_GRC_MODIFYING,      //!< 当地设定在进行中
	IEC103_GRC_ERR_DESC        //!< 带有下面所描述的差错
};

//通用分类标识序号
typedef struct tagGin
{
	uint8_t group;    //!< 组号
	uint8_t entry;    //!< 条目号
}GIN;

//通用分类数据描述
typedef struct tagGdd
{
	uint8_t dataType; //!< 数据类型
	uint8_t dataSize; //!< 数据宽度
	uint8_t contNum;  //!< 数据数目(低7位为数目，最高位为后续状态)
}GDD;

//通用分类数据集
typedef struct tagNgd
{
	GIN     gin;        //!< 通用分类标识序号
	uint8_t kod;        //!< 描述类别
	GDD     gdd;        //!< 通用分类数据描述
	uint8_t gid[IEC103_GDD_MAX_LEN];    //!< 通用分类标识数据
}NGD;

//通用分类数据集
typedef struct tagNgdData
{
	uint8_t ngd_num;              //!< 通用分类数据数据集数目
	NGD     ngd[256];             //!< 通用分类数据
}NGD_DATA;

//带时标的报文
typedef struct tagGddType18
{
	uint8_t    dpi;               //!< 双点信息
	CP32TIME2a cp32time2a;        //!< 4个八位位组时间
	uint8_t    sin;               //!< 附加信息
}GDD_TYPE_18;

//带相对时间的时标报文
typedef struct tagGddType19
{
	uint8_t    dpi;               //!< 双点信息
	uint8_t    relative_time[2];  //!< 相对时间
	uint8_t    fan[2];            //!< 故障序号
	CP32TIME2a cp32time2a;        //!< 4个八位位组时间
	uint8_t    sin;               //!< 附加信息
}GDD_TYPE_19;

//带相对时间的被测值
typedef struct tagGddType20
{
	uint8_t    val[4];            //!< 被测值 
	uint8_t    relative_time[2];  //!< 相对时间
	uint8_t    fan[2];            //!< 故障序号
	CP32TIME2a cp32time2a;        //!< 4个八位位组时间
}GDD_TYPE_20;

//二进制计数器读数用于电能脉冲
typedef struct tagGddType201
{
	int32_t  iValue;              //!< 计算器读数
	int8_t   id;                  //!< 顺序号
}GDD_TYPE_201;

//带四字节时标的BCD码
typedef struct tagGddType202
{
	uint8_t    bcd[4];            //!< BCD码
	CP32TIME2a cp32time2a;        //!< 4个八位位组时间
}GDD_TYPE_202;

//带长时标的报文
typedef struct tagGddType203
{
	uint8_t    dpi;               //!< 双点信息
	CP56TIME2a cp56time2a;        //!< 7个八位位组时间
	uint8_t    sin;               //!< 附加信息
}GDD_TYPE_203;

//带相对时间的长时标报文
typedef struct tagGddType204
{
	uint8_t    dpi;               //!< 双点信息
	uint8_t    relative_time[2];  //!< 相对时间
	uint8_t    fan[2];            //!< 故障序号
	CP56TIME2a cp56time2a;        //!< 7个八位位组时间
	uint8_t    sin;               //!< 附加信息
}GDD_TYPE_204;

//带相对时间七字节时标的被测值
typedef struct tagGddType205
{
	uint8_t    val[4];            //!< 被测值 
	uint8_t    relative_time[2];  //!< 相对时间
	uint8_t    fan[2];            //!< 故障序号
	CP56TIME2a cp56time2a;        //!< 7个八位位组时间
}GDD_TYPE_205;

//带七字节时标的BCD码
typedef struct tagGddType207
{
	uint8_t    bcd[4];            //!< BCD码
	CP56TIME2a cp56time2a;        //!< 7个八位位组时间
}GDD_TYPE_207;

/*数据属性项结构,参照IEC103定义*/
typedef struct GDD_DATA GDD_DATA;
struct GDD_DATA
{
	GDD_DATA *next;
	uint8_t  flag;      //!< 有效标志
	uint8_t  kod;       //!< 描述类别
	GDD      gdd;       //!< 通用分类数据描述
	uint8_t  *gid;      //!< 通用分类标识数据
};

#endif // RELAY_103_GEN_H
