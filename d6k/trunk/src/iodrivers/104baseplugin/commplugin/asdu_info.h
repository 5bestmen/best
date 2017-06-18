/********************************************************************************
模块名       :  APDU相关结构定义
文件名       :  asdu_info.h
文件实现功能 :  APDU结构
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期 2016/05/27       版本  v1.0   修改人      ww        修改内容

********************************************************************************/
#ifndef ASDU_INFO_H
#define ASDU_INFO_H
#include <QString>
#include <QList>
#include <QDateTime>

#pragma execution_character_set("utf-8")

enum
{
    STARTCHAR = 0x68,
};

enum { DEFAULT_MAX_K_VALUE = 12, DEFAULT_MAX_W_VALUE = 8 };

enum
{
    TYPE_I    = 1,
    TYPE_S   = 2,
    TYPE_U  = 4,
    TYPE_N  = 8,     //序列号类型(辅助)
    TYPE_T   = 16,   //定时器类型(辅助)
    TYPE_ERROR  = 32,//错误信息
    TYPE_K = 64, //K值信息
    TYPE_W = 128,//W值信息
};

enum
{
    TESTFR_CON=0x83,
    TESTFR_ACT = 0x43,
    STOPDT_CON=0x23,
    STOPDT_ACT=0x13,
    STARTDT_CON=0x0b,
    STARTDT_ACT=0x07,
};

enum
{
    ERROR_NONE,//无错误
    ERROR_SERIALNUMBER,//序列号错误
};

enum 
{
	FILE_CATALOG_REQUEST = 1,         //目录召唤
	E_FILE_CATALOG_RESPOND = 2,         //目录召唤相应
	FILE_READ_ACTION = 3,             //读取激活
	FILE_READ_ACTION_ACT = 4,         //读取激活确认
	FILE_READ_TRANSPORT = 5,          //读取传输
	FILE_READ_TRANSPORT_ACT = 6,      //读取传输确认
	FILE_WRITE_ACTION = 7,            //写入激活
	FILE_WRITE_ACTION_ACT = 8,        //写入激活确认
	FILE_WRITE_TRANSPORT  =9,         //写入传输
	FILE_WRITE_TRANSPORT_ACT = 10,    //写入传输确认

};

enum
{
    /*在监视方向的过程信息*/
    M_SP_NA_1 = 1,      // 单点信息
    M_SP_TA_1 = 2,      // 带时标的单点信息
    M_DP_NA_1 = 3,      // 双点信息
    M_DP_TA_1 = 4,      // 带时标的双点信息
    M_ST_NA_1 = 5,      // 步位置信息
    M_ST_TA_1 = 6,      // 带时标的步位置信息
    M_BO_NA_1 = 7,      // 32比特串
    M_BO_TA_1 = 8,      // 带时标的32比特串
    M_ME_NA_1 = 9,      // 测量值，规一化值
    M_ME_TA_1 = 10,     // 带时标的测量值，规一化值
    M_ME_NB_1 = 11,     // 测量值，标度化值
    M_ME_TB_1 = 12,     // 带时标的测量值，标度化值
    M_ME_NC_1 = 13,     // 测量值，短浮点数
    M_ME_TC_1 = 14,     // 带时标的测量值，短浮点数
    M_IT_NA_1 = 15,     // 累计量
    M_IT_TA_1 = 16,     // 带时标的累计量
    M_EP_TA_1 = 17,     // 带时标的继电保护装置事件
    M_EP_TB_1 = 18,     // 带时标的继电保护装置成组启动事件
    M_EP_TC_1 = 19,     // 带时标的继电保护装置成组出口信息
    M_PS_NA_1 = 20,     // 带状态检出的成组单点信息
    M_ME_ND_1 = 21,     // 不带品质描述的规一化测量值
    /*在监视方向的过程信息息，带时标的ASDU*/
    M_SP_TB_1 = 30,     // 带时标CP56Time2a的单点信息
    M_DP_TB_1 = 31,     // 带时标CP56Time2a的双点信息
    M_ST_TB_1 = 32,     // 带时标CP56Time2a的步位置信息
    M_BO_TB_1 = 33,     // 带时标CP56Time2a的32比特串
    M_ME_TD_1 = 34,     // 带时标CP56Time2a的测量值，规一化值
    M_ME_TE_1 = 35,     // 带时标CP56Time2a的测量值，标度化值
    M_ME_TF_1 = 36,     // 带时标CP56Time2a的测量值，短浮点数
    M_IT_TB_1 = 37,     // 带时标CP56Time2a的累计量
    M_EP_TD_1 = 38,     // 带时标CP56Time2a的继电保护装置事件
    M_EP_TE_1 = 39,     // 带时标CP56Time2a的继电保护装置成组启动事件
    M_EP_TF_1 = 40,     // 带时标CP56Time2a的继电保护装置成组出口信息
    /*在监视方向的系统信息*/
    M_EI_NA_1 = 70,     // 初始化结束
    /*在控制方向的过程信息*/
    C_SC_NA_1 = 45,     // 单命令
    C_DC_NA_1 = 46,     // 双命令
    C_RC_NA_1 = 47,     // 升降命令
    C_SE_NA_1 = 48,     // 设点命令，规一化值
    C_SE_NB_1 = 49,     // 设点命令，标度化值
    C_SE_NC_1 = 50,     // 设点命令，短浮点数
    C_BO_NA_1 = 51,     // 32比特串
    /*在控制方向的过程信息，带时标的ASDU*/
    C_SC_TA_1 = 58,     // 带时标CP56Time2a的单命令
    C_DC_TA_1 = 59,     // 带时标CP56Time2a的双命令
    C_RC_TA_1 = 60,     // 带时标CP56Time2a的升降命令
    C_SE_TA_1 = 61,     // 带时标CP56Time2a的设点命令，规一化值
    C_SE_TB_1 = 62,     // 带时标CP56Time2a的设点命令，标度化值
    C_SE_TC_1 = 63,     // 带时标CP56Time2a的设点命令，短浮点数
    C_BO_TA_1 = 64,     // 带时标CP56Time2a的32比特串

    /*在控制方向的系统信息*/
    C_IC_NA_1 = 100,    // 总召唤命令
    C_CI_NA_1 = 101,    // 电能脉冲召唤命令
    C_RD_NA_1 = 102,    // 读命令
    C_CS_NA_1 = 103,    // 时钟同步命令
    C_TS_NA_1 = 104,    // 测试命令
    C_RP_NA_1 = 105,    // 复位进程命令
    C_CD_NA_1 = 106,    // 收集传输延时
    /*控制方向的参数*/
    P_ME_NA_1 = 110,    // 测量值参数，规一化值
    P_ME_NB_1 = 111,    // 测量值参数，标度化值
    P_ME_NC_1 = 112,    // 测量值参数，短浮点数
    P_AC_NA_1 = 113,    // 参数激活
    /*文件传输*/
    F_FR_NA_1 = 120,    // 文件已准备好
    F_SR_NA_1 = 121,    // 节已准备好
    F_SC_NA_1 = 122,    // 召唤目录，选择文件，召唤文件，召唤节
    F_LS_NA_1 = 123,    // 最后的节，最后的段
    F_AF_NA_1 = 124,    // 确认文件，确认节
    F_SG_NA_1 = 125,    // 段
    F_DR_TA_1 = 126,    // 目录
	//扩展  
	D_DV_RD_1 = 132,   //定值读取反馈  多值
	D_DV_WR_1 = 136,   //参数设置确认

    C_GZ_SJ_1 = 140,     //故障事件  TODO

	D_FIX_SWITCH = 200,   //切换定值区
	D_FIX_SEARCH = 201,   //获取定值区号
	D_FIX_READ = 202,     //定值读取
	D_FIX_WRITE = 203,    //定值写入

	D_FILE_TRANSPORT = 210,   //文件服务


    /*************************************
        传送原因COT定义
        **************************************/
    COT_NOUSED   = 0x00,   // 未使用
    COT_PERCYC   = 0x01,   // 周期、循环
    COT_BACK     = 0x02,   // 背景扫描
    COT_SPONT    = 0x03,   // 突发（自发）
    COT_INIT     = 0x04,   // 初始化完成
    COT_REQ      = 0x05,   // 请求或被请求
    COT_ACT      = 0x06,   // 激活
    COT_ACTCON   = 0x07,   // 激活确认
    COT_DEACT    = 0x08,   // 停止激活
    COT_DEACTCON = 0x09,   // 停止激活确认
    COT_ACTTERM  = 0x0a,   // 激活终止
    COT_RETREM   = 0x0b,   // 远方命令引起的返送信息
    COT_RETLOC   = 0x0c,   // 当地命令引起的返送信息
    COT_FILE     = 0x0d,   // 文件传输
    COT_INTROGEN = 0x14,   // 响应站召唤
    COT_INTRO1   = 0x15,   //响应第1组召唤
    COT_INTRO2   = 0x16,   //响应第2组召唤
    COT_INTRO3   = 0x17,   //响应第3组召唤
    COT_INTRO4   = 0x18,   //响应第4组召唤
    COT_INTRO5   = 0x19,   //响应第5组召唤
    COT_INTRO6   = 0x1a,   //响应第6组召唤
    COT_INTRO7   = 0x1b,   //响应第7组召唤
    COT_INTRO8   = 0x1c,   //响应第8组召唤
    COT_INTRO9   = 0x1d,   //响应第9组召唤
    COT_INTRO10  = 0x1e,   //响应第10组召唤
    COT_INTRO11  = 0x1f,   //响应第11组召唤
    COT_INTRO12  = 0x20,   //响应第12组召唤
    COT_INTRO13  = 0x21,   //响应第13组召唤
    COT_INTRO14  = 0x22,   //响应第14组召唤
    COT_INTRO15  = 0x23,   //响应第15组召唤
    COT_INTRO16  = 0x24,   //响应第16组召唤
    COT_REQCOGEN = 0x25,   // 响应计数量总召唤
    COT_REQCO1   = 0x26,   // 响应第1组计数量召唤
    COT_REQCO2   = 0x27,   // 响应第2组计数量召唤
    COT_REQCO3   = 0x28,   // 响应第3组计数量召唤
    COT_REQCO4   = 0x29,   // 响应第4组计数量召唤

    COT_UNKNOW_TYPE = 0x2c,   //未知的类型标示
    COT_UNKNOW_COT  = 0x2d,   //未知的传送原因
    COT_UNKNOW_ADDR = 0x2e,   //未知的应用服务数据单元地址
    COT_UNKNOW_INF  = 0x2f,   //未知的信息体对象地址
};
enum
{
    MAX_ASDU_SIZE = 249,
};

enum TELETRCL_TYPES
{
    TELECTRL_REQUEST_SELECT=0,//遥控预置请求
    TELECTRL_ACK_SELECT=1,//遥控预置确认
    TELECTRL_REQUEST_EXECUTE=2,//遥控执行请求
    TELECTRL_ACK_EXECUTE=3,//遥控执行确认
    TELECTRL_REQUEST_UNSELECT=4,//遥控撤销请求
    TELECTRL_ACK_UNSELECT=5,//遥控撤销确认

    TELECTRL_REQUEST_GENERALCALL,//总召请求
    TELECTRL_ACK_GENERALCALL,//总召确认
    TELECTRL_ACK_GENERALCALL_OVER,//总召结束
    TELECTRL_REQUEST_SYNCTIME,//对时请求
    TELECTRL_ACK_SYNCTIME,//对时确认
    TELECTRL_REQUEST_KWHGENERALCALL,//电镀总召请求
    TELECTRL_ACK_KWHGENERALCALL,//电镀总召确认
    TELECTRL_RESET_PROCESSCALL,  //复位进程请求
    TELECTRL_ACK_PROCESSCALL     //复位进程确认
};

enum
{
    SEND_ERROR_K, //发送失败，k > DEFAULT_K
    SEND_ERROR_SOCKET,//发送失败，socket错误
    SEND_OK,//发送成功
};

enum
{
	DATATYPE_ANALOG,
	DATATYPE_BINARY,
	DATATYPE_KWH,
	DATATYPE_DEVICE,
	DATATYPE_CHANNEL,
	DATATYPE_GROUP,
	DATATYPE_LAST,
};

enum
{
	TIMER_INTERVAL_T1=15000, //发送方在发送一个I报文或U报文后 必须在t1时间内得到接收方的认可 否则认为TCP连接出现问题并重新建立连接 
	TIMER_INTERVAL_T2=10000,//接收方在接收到I格式报文后 
	TIMER_INTERVAL_T3=20000,//长期空闲t3>t1状态下发送测试帧的超时
	TIMER_INTERVAL_TG=600000 //十分钟定时发送一次总召和时间对时
};

enum
{
	//数据库文档定义
	QUALITY_NORMAL = 0,//正常
	QUALITY_MANUAL = 1, //置入
	QUALITY_ILLEGAL = 1 << 1,//非法
	QUALITY_DEAD = 1 << 2,//死数
	QUALITY_UNCERTAIN = 1 << 3,//不可靠
	QUALITY_BLOCK = 1 << 4,//封锁
	QUALITY_DOUBT = 1 << 5,//可疑
	QUALITY_BAD = 1 << 6,//坏数据
	QUALITY_UNINIT = 1 << 7,//未初始化
	QUALITY_CALCDATA = 1 << 8,//计算量
	QUALITY_UNREASONABLE = 1 << 9,//不合理
	QUALITY_INTERRUPT = 1 << 10,//采集中断
	QUALITY_REPLACE = 1 << 11,//替代


	//规约定义
	QUALITY_OVERFLOW = 1 << 12,//溢出
	QUALITY_BLOCKED = 1 << 13,//被闭锁
	QUALITY_REPLACED = 1 << 14,//被取代
	QUALITY_UNCURRENT = 1 << 15,//非当前值
	QUALITY_INVALID = 1 << 16,//无效
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

//品质位描述词
struct QDS
{
	unsigned char OV : 1;    //0: 未溢出 1：溢出
	unsigned char RES : 3;
	unsigned char BL : 1;     //0: 未被闭锁 1：被闭锁
	unsigned char SB : 1;     //0: 未被取代 1：被取代
	unsigned char NT : 1;     //0: 当前值   1：非当前值
	unsigned char IV : 1;     //0: 有效     1：无效
};

//三个八位位组二进制时间
struct CP24Time2a
{
	unsigned char msL;     //毫秒低
	unsigned char msH;     //毫秒高
	unsigned char minute;  //分钟(0-59)
public:
	QString Dump()
	{
		int nMilliSec = MYMAKE16(msL, msH);
		char buf[128];
		sprintf(buf, "%.2d %d", minute, nMilliSec);
		return buf;
	}
	void Initialize(int cminute, int csecond, int cmillisec)
	{
		unsigned short ms = csecond * 1000 + cmillisec;
		msL = MYLO8(ms);
		msH = MYHI8(ms);
		minute = cminute & 0x3f;
	}
};

class COT2
{
public:
    unsigned char m_cot0;
    unsigned char m_cot1;
public:
    void SetCot(unsigned short cot)
    {
        m_cot0 = MYLO8(cot);
        m_cot1 = MYHI8(cot);
    }
    unsigned short GetCot()
    {
        return MYMAKE16(m_cot0,m_cot1);
    }
};

class   ASDUADDR2
{
public:
    unsigned char m_asduaddr0;
    unsigned char m_asduaddr1;
public:
    void SetAddr(unsigned short addr)
    {
        m_asduaddr0 = MYLO8(addr);
        m_asduaddr1 = MYHI8(addr);
    }
    unsigned short GetAddr()
    {
        return MYMAKE16(m_asduaddr0,m_asduaddr1);
    }
};

class ASDU_BASE
{
public:
    unsigned char type;         //类型标识
    unsigned char vsq;          //可变结构限定词
    COT2 cot;                     //传送原因
    ASDUADDR2 asduAddr;  //应用服务数据单元公共地址

public:
    bool IsSQ1()
    {
        if((vsq & 0x80) == 0x80)
        {
            return true;
        }
        return false;
    }
};

//3字节表示的信息对象地址
class INFOADDR3
{
public:
    unsigned char m_infoaddr0;
    unsigned char m_infoaddr1;
    unsigned char m_infoaddr2;
public:
    void SetAddr(int addr)
    {
        unsigned short  lo16 = MYLO16(addr);
        unsigned short  hi16 = MYHI16(addr);

        m_infoaddr0 = MYLO8(lo16);
        m_infoaddr1 = MYHI8(lo16);
        m_infoaddr2 = MYLO8(hi16);
    }
    int GetAddr()
    {
        unsigned short lo16 = MYMAKE16(m_infoaddr0,m_infoaddr1);
        unsigned short hi16 = MYMAKE16(m_infoaddr2,0);

        return MYMAKE32(lo16,hi16);
    }
};

//四字节表示的信息对象地址
class INFOADDR4
{
public:
	unsigned char m_infoaddr0;
	unsigned char m_infoaddr1;
	unsigned char m_infoaddr2;
	unsigned char m_infoaddr3;
public:
	void SetAddr(int addr)
	{
		unsigned short  lo16 = MYLO16(addr);
		unsigned short  hi16 = MYHI16(addr);

		m_infoaddr0 = MYLO8(lo16);
		m_infoaddr1 = MYHI8(lo16);

		m_infoaddr2 = MYLO8(hi16);
		m_infoaddr3 = MYHI8(hi16);
	}

	int GetAddr()
	{
		unsigned short lo16 = MYMAKE16(m_infoaddr0, m_infoaddr1);
		unsigned short hi16 = MYMAKE16(m_infoaddr2, m_infoaddr3);

		return MYMAKE32(lo16, hi16);
	}
};

//带品质描述词的单点信息
struct SIQ
{
    unsigned char SPI:1;    //值
    unsigned char RES:3;    //备用
    unsigned char BL:1;     //0: 未被闭锁 1：被闭锁
    unsigned char SB:1;     //0: 未被取代 1：被取代
    unsigned char NT:1;     //0: 当前值   1：非当前值
    unsigned char IV:1;     //0: 有效     1：无效
};

//定值参数特征标识
struct DVFLAG
{
	unsigned char CONT : 1;    //后续标识
	unsigned char RES : 5;    //reserve
	unsigned char CR : 1;     //0: 未用 1：取消预置
	unsigned char SE : 1;     //0: 固化 1：预置
};

//////////////////////////////////////////////////////////////////////////
//M_SP_NA_1:单点遥信(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU1_SQ1 : public ASDU_BASE
{
public:
    enum { MAX_DATA_PER_ASDU1_SQ1=(MAX_ASDU_SIZE-sizeof(ASDU_BASE)-sizeof(INFOADDR3))/sizeof(SIQ),};
public:
    INFOADDR3 infoaddr;
    SIQ m_data[MAX_DATA_PER_ASDU1_SQ1];
public:
    void SetItemCount(int nCount)
    {
        vsq = nCount | 0x80;
    }

    int GetItemCount() const
    {
        return  vsq & 0x7f; //应用数据单元数目
    }

public:
    void SetValue(int nIndex,int nValue)
    {
        if(nIndex<0 || nIndex>=MAX_DATA_PER_ASDU1_SQ1)
        {
            return;
        }

        m_data[nIndex].SPI = nValue;
    }

    int GetValue(int nIndex)
    {
        if(nIndex<0 || nIndex>=MAX_DATA_PER_ASDU1_SQ1)
        {
            return 0;
        }

        return m_data[nIndex].SPI;
    }

    SIQ GetSIQ(int nIndex)
    {
        int a = 0;
        if(nIndex<0 || nIndex>=MAX_DATA_PER_ASDU1_SQ1)
        {
            return (*(SIQ*)(&a));
        }

        return m_data[nIndex];
    }

public:
    int  GetAsdu1_SQ1Length()
    {
        int nSize = sizeof(ASDU_BASE) + sizeof(INFOADDR3) + GetItemCount() * sizeof(unsigned char);
        return nSize;
    }
};

//////////////////////////////////////////////////////////////////////////
//M_SP_NA_1:单点遥信(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU1_SQ0 : public ASDU_BASE
{
private:
    class ASDU1_SQ0_DATA //离散的信号量
    {
    public:
        INFOADDR3 infoaddr;
        SIQ siq;
    };
public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
public:
    void SetValue(int nIndex,int nValue);
    int GetValue(int nIndex);
    SIQ GetSIQ(int nIndex);
public:
    enum { MAX_DATA_PER_ASDU1_SQ0=(MAX_ASDU_SIZE-sizeof(ASDU_BASE))/sizeof(ASDU1_SQ0_DATA),};
    ASDU1_SQ0_DATA m_data[MAX_DATA_PER_ASDU1_SQ0];
};

//带品质描述词的双点信息
struct DIQ
{
    unsigned char DPI:2;    //值
    unsigned char RES:2;    //备用
    unsigned char BL:1;     //0: 未被闭锁 1：被闭锁
    unsigned char SB:1;     //0: 未被取代 1：被取代
    unsigned char NT:1;     //0: 当前值   1：非当前值
    unsigned char IV:1;     //0: 有效     1：无效
};

//CP56Time2a时间结构
struct CP56Time2a
{
	unsigned char msL;     //毫秒低
	unsigned char msH;     //毫秒高
	unsigned char minute;  //分钟(0-59)
	unsigned char hour;    //小时(0-23)
	unsigned char day;     //日期 0-4位: 日  5-7位:星期
	unsigned char month;   //月份(1-12)
	unsigned char year;    //年份(0-99)
public:
	QString Dump()
	{
		int nMilliSec = MYMAKE16(msL, msH);
		char buf[128];
		sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d %d", year + 2000, month, day, hour, minute, nMilliSec / 1000, nMilliSec % 1000);
		return buf;
	}
	void Initialize(int cyear, int cmonth, int cday, int chour, int cminute, int csecond, int cmillisec)
	{
		unsigned short ms = csecond * 1000 + cmillisec;
		msL = MYLO8(ms);
		msH = MYHI8(ms);
		minute = cminute & 0x3f;
		hour = chour & 0x1f;
		day = cday & 0x1f;
		month = cmonth & 0x0f;
		year = (cyear - 2000) & 0x7f;
	}
};

//////////////////////////////////////////////////////////////////////////
//M_DP_NA_1:双点遥信(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU3_SQ1 : public ASDU_BASE
{
public:
    enum { MAX_DATA_PER_ASDU3_SQ1=(MAX_ASDU_SIZE-sizeof(ASDU_BASE)-sizeof(INFOADDR3))/sizeof(DIQ),};
public:
    INFOADDR3 infoaddr;
    DIQ m_data[MAX_DATA_PER_ASDU3_SQ1];
public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
public:
    void SetValue(int nIndex,int nValue);
    int GetValue(int nIndex);
public:
    DIQ GetDIQ(int nIndex);
public:
    int GetAsdu3_SQ1Length();
};

//////////////////////////////////////////////////////////////////////////
//M_DP_NA_1:双点遥信(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU3_SQ0 : public ASDU_BASE
{
	class ASDU3_SQ0_DATA //离散的信号量
	{
	public:
		INFOADDR3      infoaddr;
		DIQ                 diq;
	};
	enum { MAX_DATA_PER_ASDU3_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU3_SQ0_DATA), };
public:
	void SetItemCount(int nCount);
	int    GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);

public:
	DIQ GetDIQ(int nIndex);
public:
	ASDU3_SQ0_DATA m_data[MAX_DATA_PER_ASDU3_SQ0];
};
//////////////////////////////////////////////////////////////////////////

//netbus消息头
struct NBM_BASE
{
public:
    NBM_BASE()
    {
        m_nMsgType  = 0;
        m_nMsgLen    = 0;
    }
public:
    QString Dump(){return "";}
public:
    int m_nMsgType;
    int m_nMsgLen;
};

//遥控命令
struct NBM_TELECTRL : public NBM_BASE
{
public:
    int m_nStationID;
    int m_nDeviceID;//二次设备ID
    int m_nEquipmenID;//一次设备ID
    int m_nDataID;
    int m_nRelateDataID;

public:
    int m_nUserID;
    int m_nNodeID;

public:
    int m_nCommandType;//遥控命令类型,45,46,48,49,50?

public:
    int m_nDataType;//模拟量?开关量?
    float m_fValue;    //点值

public:
    int m_nCtrlType; // 遥控类型(总召，总召确认，对时,对时确认，预置，预置确认，执行，执行确认，撤销，撤销确认....)

public:
    QString Dump()
    {
        char buf[256];

        sprintf(buf,"Type:%d Length:%d StationID:%d DeviceID:%d DataID:%d UserID:%d NodeID:%d DataType:%d fValue:%f CtrlType:%d",
                m_nMsgType,m_nMsgLen,m_nStationID,m_nDeviceID,m_nDataID,m_nUserID,m_nNodeID,m_nDataType,m_fValue,m_nCtrlType);

        return buf;
    }
};


//////////////////////////////////////////////////////////////////////////
//M_ME_NA_1:归一化测量值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU9_SQ0 : public ASDU_BASE
{
	class ASDU9_SQ0_DATA
	{
	public:
		INFOADDR3      infoaddr;
		short nValue;
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU9_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU9_SQ0_DATA), };
public:
	ASDU9_SQ0_DATA        m_data[MAX_DATA_PER_ASDU9_SQ0];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int  GetAsdu9_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_NA_1:归一化测量值(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU9_SQ1 : public ASDU_BASE
{
public:
	class ASDU9_SQ1_DATA
	{
	public:
		short m_nValue;
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU9_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU9_SQ1_DATA), };
public:
	INFOADDR3      infoaddr;
	ASDU9_SQ1_DATA        m_data[MAX_DATA_PER_ASDU9_SQ1];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int  GetAsdu9_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_DP_TA_1:带时标双点遥信(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU4_SQ0 : public ASDU_BASE
{
private:
	class ASDU4_SQ0_DATA //离散的信号量
	{
	public:
		INFOADDR3      m_infoaddr;
		DIQ                  m_diq;
		CP24Time2a    m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);
public:
	DIQ GetDIQ(int nIndex);

public:
	int  GetAsdu4_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU4_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU4_SQ0_DATA), };
	ASDU4_SQ0_DATA m_data[MAX_DATA_PER_ASDU4_SQ0];
};

//////////////////////////////////////////////////////////////////////////
//M_ME_TA_1:测量值，带时标的归一化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU10_SQ0 : public ASDU_BASE
{
private:
	class ASDU10_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		short  m_nva;
		QDS m_qds;
		CP24Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu10_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU10_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU10_SQ0_DATA), };
	ASDU10_SQ0_DATA m_data[MAX_DATA_PER_ASDU10_SQ0];
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_ME_NB_1:标度化测量值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU11_SQ0 : public ASDU_BASE
{
	class ASDU11_SQ0_DATA
	{
	public:
		INFOADDR3 infoaddr;
		short m_nValue;
		QDS m_qds;
	};
	enum { MAX_DATA_PER_ASDU11_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU11_SQ0_DATA), };
public:
	ASDU11_SQ0_DATA m_data[MAX_DATA_PER_ASDU11_SQ0];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int GetAsdu11_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_NB_1:标度化测量值(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU11_SQ1 : public ASDU_BASE
{
public:
	class ASDU11_SQ1_DATA
	{
	public:
		short m_nValue;
		QDS m_qds;
	};
	enum { MAX_DATA_PER_ASDU11_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU11_SQ1_DATA), };
public:
	INFOADDR3 infoaddr;
	ASDU11_SQ1_DATA m_data[MAX_DATA_PER_ASDU11_SQ1];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int GetAsdu11_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_TB_1:测量值，带时标的标度化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU12_SQ0 : public ASDU_BASE
{
private:
	class ASDU12_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		short m_sva;
		QDS m_qds;
		CP24Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);

public:
	QDS GetQDS(int nIndex);

public:
	int GetAsdu12_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU12_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU12_SQ0_DATA), };
	ASDU12_SQ0_DATA m_data[MAX_DATA_PER_ASDU12_SQ0];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_NC_1:短浮点数测量值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU13_SQ0 : public ASDU_BASE
{
	class ASDU13_SQ0_DATA
	{
	public:
		INFOADDR3 infoaddr;
		float m_fValue;
		QDS m_qds;
	};
	enum { MAX_DATA_PER_ASDU13_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU13_SQ0_DATA), };
public:
	ASDU13_SQ0_DATA m_data[MAX_DATA_PER_ASDU13_SQ0];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	float GetValue(int nIndex);
	void SetValue(int nIndex, float fValue);

public:
	QDS GetQDS(int nIndex);

public:
	int GetAsdu13_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_NC_1:短浮点数测量值(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU13_SQ1 : public ASDU_BASE
{
public:
	class ASDU13_SQ1_DATA
	{
	public:
		float m_fValue;
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU13_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU13_SQ1_DATA), };
public:
	INFOADDR3      infoaddr;
	ASDU13_SQ1_DATA        m_data[MAX_DATA_PER_ASDU13_SQ1];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	float GetValue(int nIndex);
	void SetValue(int nIndex, float fValue);

public:
	QDS GetQDS(int nIndex);

public:
	int GetAsdu13_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_TC_1:测量值，带时标的短浮点数(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU14_SQ0 : public ASDU_BASE
{
private:
	class ASDU14_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		float m_std;
		QDS m_qds;
		CP24Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, float fValue);
	float GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu14_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU14_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU14_SQ0_DATA), };
	ASDU14_SQ0_DATA m_data[MAX_DATA_PER_ASDU14_SQ0];
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_SP_TA_1:带时标单点遥信(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU2_SQ0 : public ASDU_BASE
{
private:
	class ASDU2_SQ0_DATA //离散的信号量
	{
	public:
		INFOADDR3      m_infoaddr;
		SIQ                  m_siq;
		CP24Time2a    m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);
public:
	SIQ GetSIQ(int nIndex);
public:
	int  GetAsdu2_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU2_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU2_SQ0_DATA), };
	ASDU2_SQ0_DATA m_data[MAX_DATA_PER_ASDU2_SQ0];
};

//////////////////////////////////////////////////////////////////////////
//M_IT_NA_1:累计量(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU15_SQ0 : public ASDU_BASE
{
private:
	class ASDU15_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		int m_nValue;
		char m_bcrparam;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	int GetAsdu15_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU15_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU15_SQ0_DATA), };
	ASDU15_SQ0_DATA m_data[MAX_DATA_PER_ASDU15_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_IT_NA_1:召唤目录响应
//////////////////////////////////////////////////////////////////////////
class ASDUMLXY : public ASDU_BASE
{
private:
    //信息体地址（目录）
    INFOADDR3 m_CatalogIndex;

    class ASDUMLXY_DATA
    {
    public:
        //文件名称编号
        ASDUADDR2 m_infoFileIndex;
        //文件长度编号
        INFOADDR3 m_CatalogIndex;
        //文件状态
        unsigned char m_bFileStatus;
        //时间
        CP56Time2a m_time;
    };
public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
public:
    //获取文件编号信息
    unsigned short GetValue(int nIndex);
public:
    CP56Time2a GetTime(int nIndex);
public:
    enum { MAX_DATA_ASDUMLXY = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDUMLXY_DATA), };
    ASDUMLXY_DATA m_data[MAX_DATA_ASDUMLXY];
};


//////////////////////////////////////////////////////////////////////////
//M_IT_NA_1:累计量(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU15_SQ1 : public ASDU_BASE
{
	class ASDU15_SQ1_DATA //连续的累计量
	{
	public:
		int m_nValue;
		char  m_bcrparam;
	};
public:
	enum { MAX_DATA_PER_ASDU15_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU15_SQ1_DATA), };
public:
	INFOADDR3 infoaddr;
	ASDU15_SQ1_DATA m_data[MAX_DATA_PER_ASDU15_SQ1];

public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	int GetAsdu15_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_DP_NA_1:死区设置
//////////////////////////////////////////////////////////////////////////
class ASDU112_ZOOM : public ASDU_BASE
{
public:
        INFOADDR3      infoaddr;
        float          m_value;
        unsigned char  uType;
public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
};


//////////////////////////////////////////////////////////////////////////
//M_IT_TA_1:带时标的累计量(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU16_SQ0 : public ASDU_BASE
{
private:
	class ASDU16_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		float m_fValue;
		char m_bcrparam;
		CP24Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, float fValue);
	float GetValue(int nIndex);
public:
	void SetTime(int nIndex);
	CP24Time2a GetTime(int nIndex);
public:
	int GetAsdu16_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU16_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU16_SQ0_DATA), };
	ASDU16_SQ0_DATA m_data[MAX_DATA_PER_ASDU16_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//M_PS_NA_1:带变位检出的成组单点信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU20_SQ0 : public ASDU_BASE
{
private:
	class ASDU20_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		int m_nSCD;
		QDS m_qds;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);

public:
	QDS GetQDS(int nIndex);

public:
	enum { MAX_DATA_PER_ASDU20_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU20_SQ0_DATA), };
	ASDU20_SQ0_DATA m_data[MAX_DATA_PER_ASDU20_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_PS_NA_1:带变位检出的成组单点信息(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU20_SQ1 : public ASDU_BASE
{
	class ASDU20_SQ1_DATA
	{
	public:
		int m_nSCD;
		QDS m_qds;
	};
	enum { MAX_DATA_PER_ASDU20_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU20_SQ1_DATA), };
public:
	INFOADDR3 m_infoaddr;
	ASDU20_SQ1_DATA m_data[MAX_DATA_PER_ASDU20_SQ1];

public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu20_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_ND_1:测量值，不带品质描述词的归一化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU21_SQ0 : public ASDU_BASE
{
private:
	class ASDU21_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		short m_nva;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	int GetAsdu21_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU21_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU21_SQ0_DATA), };
	ASDU21_SQ0_DATA m_data[MAX_DATA_PER_ASDU21_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_ME_ND_1:测量值，不带品质描述词的归一化值(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU21_SQ1 : public ASDU_BASE
{
	class ASDU21_SQ1_DATA
	{
	public:
		short m_nva;
	};
	enum { MAX_DATA_PER_ASDU21_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDU21_SQ1_DATA), };
public:
	INFOADDR3 m_infoaddr;
	ASDU21_SQ1_DATA m_data[MAX_DATA_PER_ASDU21_SQ1];

public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	int GetAsdu21_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//M_SP_TB_1:带长时标的单点信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU30_SQ0 : public ASDU_BASE
{
	class ASDU30_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		SIQ m_siq;
		CP56Time2a    m_time;
	};
public:
	enum { MAX_DATA_PER_ASDU30_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU30_SQ0_DATA), };
	ASDU30_SQ0_DATA        m_data[MAX_DATA_PER_ASDU30_SQ0];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	void SetAddress(int nIndex, int nAddr);
	int GetAddress(int nIndex);
public:
	void SetTime(int nIndex);
	CP56Time2a GetTime(int nIndex);
public:
	SIQ GetSIQ(int nIndex);
public:
	int  GetAsdu30_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//M_DP_TB_1:带长时标的双点信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU31_SQ0 : public ASDU_BASE
{
	class ASDU31_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		DIQ m_diq;
		CP56Time2a m_time;
	};
public:
	enum { MAX_DATA_PER_ASDU31_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU31_SQ0_DATA), };
	ASDU31_SQ0_DATA m_data[MAX_DATA_PER_ASDU31_SQ0];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	void SetAddress(int nIndex, int nAddr);
	int GetAddress(int nIndex);
public:
	void SetTime(int nIndex);
	CP56Time2a GetTime(int nIndex);
public:
	DIQ GetDIQ(int nIndex);
public:
	int GetAsdu31_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ST_TB_1:带时标的步位置信息(离散)
//////////////////////////////////////////////////////////////////////////
//带瞬变状态指示的值
struct VTI
{
	unsigned short value : 15;     //值
	unsigned short BS : 1;     //瞬变状态 0:未在瞬变状态 1:处于瞬变状态
};

class ASDU32_SQ0 : public ASDU_BASE
{
private:
	class ASDU32_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		VTI m_vti;
		QDS m_qds;
		CP56Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	enum { MAX_DATA_PER_ASDU32_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU32_SQ0_DATA), };
	ASDU32_SQ0_DATA m_data[MAX_DATA_PER_ASDU32_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_TD_1:测量值，带时标CP56Time2a的归一化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU34_SQ0 : public ASDU_BASE
{
private:
	class ASDU34_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		short m_nva;
		QDS m_qds;
		CP56Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu34_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU34_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU34_SQ0_DATA), };
	ASDU34_SQ0_DATA m_data[MAX_DATA_PER_ASDU34_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_TE_1:测量值，带时标CP56Time2a的标度化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU35_SQ0 : public ASDU_BASE
{
private:
	class ASDU35_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		short m_sva;
		QDS m_qds;
		CP56Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu35_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU35_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU35_SQ0_DATA), };
	ASDU35_SQ0_DATA m_data[MAX_DATA_PER_ASDU35_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_TF_1:测量值，带时标CP56Time2a的短浮点数(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU36_SQ0 : public ASDU_BASE
{
private:
	class ASDU36_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		float m_fValue;
		QDS m_qds;
		CP56Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, float fValue);
	float GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int GetAsdu36_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU36_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU36_SQ0_DATA), };
	ASDU36_SQ0_DATA m_data[MAX_DATA_PER_ASDU36_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_IT_TB_1:带时标CP56Time2a的累计量(离散)
//////////////////////////////////////////////////////////////////////////

//二进制计数器读数
struct BCR_PARAM
{
	unsigned char  SQ : 5;     //顺序号
	unsigned char  CY : 1;     //0:未溢出 1:溢出
	unsigned char  CA : 1;     //0:未被调整 1:被调整
	unsigned char  IV : 1;     //0:有效 1:无效
};
struct BCR
{
	unsigned int number;     //计数器读数
	BCR_PARAM param;
};
class ASDU37_SQ0 : public ASDU_BASE
{
private:
	class ASDU37_SQ0_DATA
	{
	public:
		INFOADDR3 m_infoaddr;
		BCR m_bcr;
		CP56Time2a m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	int GetAsdu37_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU37_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU37_SQ0_DATA), };
	ASDU37_SQ0_DATA m_data[MAX_DATA_PER_ASDU37_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//C_SC_NA_1:单点命令(遥控：控分控合)
//////////////////////////////////////////////////////////////////////////

//单命令
struct SCO
{
	unsigned char SCS : 1;   //值
	unsigned char RES : 1;   //备用
	unsigned char QU : 5;   //备用
	unsigned char SelectOrExcute : 1; //0:执行 1:选择
};

//双命令
struct DCO
{
	unsigned char DCS : 2;   //0:不允许 1:开 2:合 3:不允许
	unsigned char QU : 5;   //备用
	unsigned char SelectOrExcute : 1; //0:执行 1:选择
};

struct Q0S//Q零S，QOS在windows内部已用
{
	unsigned char QL : 7;
	unsigned char SelectOrExcute : 1; //0:执行 1:选择
};
class ASDU45 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	SCO m_sco;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//C_DC_NA_1:双点命令
//////////////////////////////////////////////////////////////////////////
class ASDU46 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	DCO m_dco;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//C_SE_NA_1:归一化设点命令(遥控：设模拟量)
//////////////////////////////////////////////////////////////////////////
class ASDU48 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	short m_nValue;
	Q0S m_qos;
public:
	short GetValue();
	void SetValue(short nValue);
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_SE_NB_1:标度化设点命令(遥控：设模拟量)
//////////////////////////////////////////////////////////////////////////
class ASDU49 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	short m_nValue;
	Q0S m_qos;
public:
	short GetValue();
	void SetValue(short nValue);
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_SE_NC_1:浮点数设点命令(遥控：设模拟量)
//////////////////////////////////////////////////////////////////////////
class ASDU50 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	float m_fValue;
	Q0S m_qos;
public:
	float GetValue();
	void SetValue(float fValue);
};
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//C_IC_NA_1:总召唤命令
//////////////////////////////////////////////////////////////////////////
class ASDU100 : public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
    unsigned char m_qoi;
};

//////////////////////////////////////////////////////////////////////////
//C_CI_NA_1:电能脉冲召唤命令
//////////////////////////////////////////////////////////////////////////
class ASDU101 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	unsigned char m_qcc;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//C_CS_NA_1:复位进程
//////////////////////////////////////////////////////////////////////////
class ASDU105 : public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
    unsigned char m_qcc;

};

//故障事件
class ASDUGZ : public ASDU_BASE
{
private:
    class ASDU_GZ_DATA
    {
    public:
        INFOADDR3 m_infoaddr;  //遥测信息体地址
        short m_AnalogValue;             //遥测数据
    };

public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
public:
    void SetValue(int nIndex, int nValue);
    int GetValue(int nIndex);
public:
    int GetAsduGZ_SQ0Length();


public:
    unsigned char m_BinaryType;    //遥信类型
    INFOADDR3 m_BinaryAddr;        //遥信点号
    unsigned char m_BinaryValue;   //遥信值
    CP56Time2a m_BinaryTime;       //故障事件

    unsigned char m_AnalogType;    //遥测类型

public:
    enum { MAX_DATA_PER_ASDU_GZ = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3) - sizeof(CP56Time2a) - 2) / sizeof(ASDU_GZ_DATA), };
    ASDU_GZ_DATA m_data[MAX_DATA_PER_ASDU_GZ];


};
 

//////////////////////////////////////////////////////////////////////////
//C_CS_NA_1:时钟同步命令
//////////////////////////////////////////////////////////////////////////
class ASDU103 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;
	CP56Time2a m_time;
};
//////////////////////////////////////////////////////////////////////////

//文件召唤
//召唤目录
class ASDUZGML : public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
//     unsigned char m_reserve1;  //保留字段1
//     unsigned char m_reserve2;  //保留字段2
    ASDUADDR2 m_infoFileIndex;    //文件名称编号
    unsigned char m_reserve3;  //保留字段3
    unsigned char m_reserve4;  //保留字段4   1表示选择文件
};

//选择文件响应
class ASDUCHRQ : public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
    ASDUADDR2 m_infoFileIndex;    //文件名称编号
    INFOADDR3 m_FileLeg;          //文件长度
    unsigned char m_frq;          //0表示肯定确认
};

//请求文件准备就绪
class ASDUQQWJ :public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
    ASDUADDR2 m_infoFileIndex;    //文件名称编号
    unsigned char m_nodeName;     //节名称
    INFOADDR3 m_FileLeg;          //文件长度
    unsigned char m_frq;          //0表示准备好
};

//最后一节
class ASDULASTNODE :public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;
    ASDUADDR2 m_infoFileIndex;    //文件名称编号
    unsigned char m_nodeName;     //节名称
    unsigned char m_reserve;          //保留
    unsigned char m_frq;          //和校验
};

//节数据
class ASDUFILENODE : public ASDU_BASE
{
public:
    INFOADDR3 m_infoaddr;       //信息体地址
    ASDUADDR2 m_infoFileIndex;    //文件名称编号
    unsigned char m_NodeNameIndex;     //节名称
    unsigned char m_NodeLegth;         //段长度
    enum { MAX_DATA_FILE_NODE = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)), };
    char m_data[MAX_DATA_FILE_NODE];

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//切换定值区
class ASDU200 : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;          //信息体地址
	ASDUADDR2 m_infAreaIndex;     //定值区号
};

//读取当前定值区号 控制方向
class ASDU201_C : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;          //信息体地址 
};

//最大定制区 区号
class   ASDUADDR7
{
public:
	unsigned char m_asduaddr0;
	unsigned char m_asduaddr1;
	unsigned char m_asduaddr2;
	unsigned char m_asduaddr3;
	unsigned char m_asduaddr4;
	unsigned char m_asduaddr5;
	unsigned char m_asduaddr6;

};

//读取当前定值区号   监视方向
class ASDU201_M : public ASDU_BASE
{
public:
	INFOADDR3 m_infoaddr;          //信息体地址 

	ASDUADDR2 m_infoCurrentAreaIndex;     //当前定值区区号
	ASDUADDR2 m_infoMinAreaIndex;         //最小定值区区号

	ASDUADDR7 m_infoMaxAreaIndex;         //最大定值区区号
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//定值获取
//////////////////////////////////////////////////////////////////////////
//单命令
struct DZQOS
{
	unsigned char QOS : 1;   // 1:预置   0：激活
	unsigned char RES : 1;   //备用
	unsigned char QU : 5;   //备用
	unsigned char DQ : 1; //备用
};

//定值读取  控制方向
class ASDUDZ : public ASDU_BASE
{
public:
	class ASDUDZ_DATA
	{
	public:
		INFOADDR3      infoaddr;		
	};

	enum { MAX_DATA_PER_ASDUDZ_RD = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDUDZ_DATA), };
public:
    int GetAsduDzLength();

public:
	//定值区号
	ASDUADDR2      infoFixCode;
	//
	ASDUDZ_DATA m_data[MAX_DATA_PER_ASDUDZ_RD];

public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
};

//定值设置   bool类型
class ASDU203_B : public ASDU_BASE
{
public:
	//定值区号
	ASDUADDR2      m_infoFixCode;
    //参数特征标识
	unsigned char m_featureCode;
	//
	class ASDUDZ_SET
	{
	public:
		//信息体地址
		INFOADDR3      infoaddr;
		//tag类型
		unsigned char tagType;
		//数据长度
		unsigned char dataLength;
		//value
		bool m_bValue;
	};
	enum { MAX_DATA_PER_ASDUDZ_RD = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(ASDUADDR2) - 1) / sizeof(ASDUDZ_SET), };
public:
	int GetAsduDzLength();
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;

	ASDUDZ_SET m_data[MAX_DATA_PER_ASDUDZ_RD];

};

//定值设置  unsigned short
class ASDU203_U : public ASDU_BASE
{
public:
	//定值区号
	ASDUADDR2      m_infoFixCode;
	//参数特征标识
	unsigned char m_featureCode;
	//
	class ASDUDZ_SET
	{
	public:
		//信息体地址
		INFOADDR3      infoaddr;
		//tag类型
		unsigned char tagType;
		//数据长度
		unsigned char dataLength;
		//value
		unsigned short m_uValue;
	};
	enum { MAX_DATA_PER_ASDUDZ_RD = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(ASDUADDR2) - 1) / sizeof(ASDUDZ_SET), };
public:
	int GetAsduDzLength();
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;

	ASDUDZ_SET m_data[MAX_DATA_PER_ASDUDZ_RD];

};

//定值int
class ASDU203_N : public ASDU_BASE
{
public:
	//定值区号
	ASDUADDR2      m_infoFixCode;
	//参数特征标识
	unsigned char m_featureCode;
	//
	class ASDUDZ_SET
	{
	public:
		//信息体地址
		INFOADDR3      infoaddr;
		//tag类型
		unsigned char tagType;
		//数据长度
		unsigned char dataLength;
		//value
		float m_nValue;
	};
	enum { MAX_DATA_PER_ASDUDZ_RD = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(ASDUADDR2) - 1) / sizeof(ASDUDZ_SET), };
public:
	int GetAsduDzLength();
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;

	ASDUDZ_SET m_data[MAX_DATA_PER_ASDUDZ_RD];


};

//定值修改固化
class ASDU203_GH : public ASDU_BASE
{
public:
	//
	//定值区号
	ASDUADDR2      m_infoFixCode;
	//参数特征标识
	DVFLAG m_featureCode;

};

class ASDUDZ2 : public ASDU_BASE
{
public:
    class ASDUDZ2_DATA
    {
    public:
        INFOADDR3      infoaddr;
    };

    enum { MAX_DATA_PER_ASDUDZ2_RD = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR3)) / sizeof(ASDUDZ2_DATA), };
public:
    int GetAsduDzLength();

public:
    ASDUDZ2_DATA m_data[MAX_DATA_PER_ASDUDZ2_RD];
public:
    void SetItemCount(int nCount);
    int GetItemCount() const;
};

//数据
class DEV_DATA
{
public:
	//地址
	int nAddress;
	//tag类型
	int nTagType;
	//数据类型长度
	int nLength;
// 	//值
// 	float nValue;
// 	//
// 	bool bValue;
// 	//
// 	unsigned short uValue;
	//字符串 value
	QString strValue;

	int iQos;
};

//定值命令
struct  DEV_BASE
{
public:


public:
	int m_nCommandType;    // 102/132  48/136
	unsigned short m_nCto;            //传送原因
	int m_nQos;            //  1:选择预置参数   0:执行激活参数
	unsigned short m_nAsduID;       //ASDU公共地址
	//数据
	QList<DEV_DATA> m_lstData;
};

// 死区
struct ZOOM_BASE
{
public:
    int nReason;        //发送原因
    int nNumber;        //发送数量 
    int nBaseAddr;      //装置地址
    int nAddr;           //信息对象地址
    float fValue;       //
    unsigned char qpm;  //参数
};

//录波文件
struct LB_DATA
{
public:
    int m_nCatalogIndex;   //目录标号
    unsigned short m_nFilenameIndex;  //文件名称编号
    bool m_bValidFlag;     //有效标示
    QString m_strFileName;   //文件名
};


class FILE_BASE :public ASDU_BASE
{
public:
	//信息对象定义
	INFOADDR3 m_asduObjID;
	//附加数据包类型
	unsigned char m_addrType;

};

class FILE_BASE_HEADER : public FILE_BASE
{
public:
	//操作标识
	unsigned char m_nOperatorType;

};

struct Catalog_Info
{
public:
	//文件名称
	QString m_strFileName;
	//文件属性
	unsigned char m_cFileAttr;   //1:目录  0:文件
	//
	int m_FileSize;
	//文件名称长度
	unsigned char m_cFileNameLength;

};

//文件属性信息
struct FILE_ATTR_INFO : public FILE_BASE
{
public:
	//文件名称长度
	unsigned char m_cFileNameLength;
	//文件名称
	QString m_strFileName;
	//文件属性
	unsigned char m_cFileAttr;   //1:目录  0:文件
								 //文件大小
	int m_FileSize;
	//文件时间
	CP56Time2a m_timeFile;

	//附加属性

	//操作标识
	unsigned char m_nOperatorType;   //1:
									 //文件id
	int m_nFileID;
	//文件内容
	QByteArray m_btArrayData;

};

//召唤文件目录  
class  FILE_CATALOG_REQUEST_1 : public FILE_BASE
{
public: 
	enum { MAX_DATA_FILENAME_LENGTH = (MAX_ASDU_SIZE - sizeof(FILE_BASE) - 7 - sizeof(CP56Time2a)*2) , };
public:
	//操作标识
	unsigned char m_nOperatorType;
	//目录ID
	INFOADDR4 m_nCatalogID;
	//目录名长度
	unsigned char m_uCatalogLength;
    //目录名
	char m_cCatalogName[MAX_DATA_FILENAME_LENGTH];

	//下面三个字段只做传输数据 
	//召唤标志
	unsigned char m_nCallFlag;
	//查询起始时间
	CP56Time2a m_dateStart;
	//查询结束时间
	CP56Time2a m_dataEnd;

	//文件信息
	FILE_ATTR_INFO m_fileInfo;

public:
	int GetAsduLength()
	{
		return sizeof(FILE_BASE) + 2 + sizeof(INFOADDR4) + m_uCatalogLength;
	}

};

//召唤文件目录2
class FILE_CATALOG_REQUEST_2
{
public:
	//召唤标志
	unsigned char m_nCallFlag;
	//查询起始时间
	CP56Time2a m_dateStart;
	//查询结束时间
	CP56Time2a m_dataEnd;

};



//召唤目录 确认
struct FILE_CATALOG_RESPOND : public FILE_BASE
{
public:
	//操作标识
	unsigned char m_nOperatorType;
	//结果字描述
	unsigned char m_nResultFlag;
	//目录ID
	int m_nCatalogID;
	//后续标识
	unsigned char m_uNextFlag;

	//文件数量
	unsigned char m_uFileCount;
	//文件属性信息ds
	//QList<FILE_ATTR_INFO> m_arrFiles;
public:
	int GetLength()
	{
		return sizeof(FILE_BASE) + 8;
	}
};



//读文件激活
// struct READ_ACTION
// {
// public:
// 	//操作标识
// 	unsigned char m_nOperatorType;
//     //文件名长度
// 	unsigned char m_nFileNameLegth;
// 	//文件名
// 	unsigned char m_nFileName;
// };

//写文件激活
// struct  WRITE_ACTION : public READ_ACTION
// {
// public:
// 	//文件ID
// 	int m_nFileID;
// 	//文件大小
// 	int m_nFileSize;
// };

//读（写）文件激活确认
// struct READ_ACTION_CONFORM
// {
// public:
// 	//
// 	//操作标识
// 	unsigned char m_nOperatorType;
// 	//结果字描述
// 	unsigned char m_nResultFlag;
//     //文件名长度
// 	unsigned char m_nFileNameLength;
// 	//文件名
// 	QString strFilename;
// 	//文件标识
// 	int m_nFileID;
// 	//文件大小
// 	int m_nFileSize;
// 	//
// 
// };

//读(写)文件传输
struct READ_NODE_FILE
{
public:
	//操作标识
	unsigned char m_nOperatorType;
	//文件ID
	int m_nFileID;
	//数据段号
	int m_nFileNodeID;
	//后续标志
	unsigned char m_cNextFlag;
	//文件数据
	QByteArray m_byFileData;
	//验证码
	unsigned char m_cCheckCode;
};

//读(写)文件数据传输确认
struct READ_NODE_FILE_CONFORM : public FILE_BASE
{
public:
	//操作标识
	unsigned char m_nOperatorType;
	//文件ID
	INFOADDR4  m_nFileID;
	//数据段号
	INFOADDR4 m_nFileNodeID;
	//结果描述子
	unsigned char m_cEndFlag;

};

//

#pragma pack(pop)











#endif // ASDU_INFO_H
