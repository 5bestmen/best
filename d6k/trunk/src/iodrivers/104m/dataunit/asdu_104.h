
//通用驱动定义：
//1:ASDU定义
//2:数据类型定义
//3:通用宏定义

#ifndef _ASDU_104_H
#define _ASDU_104_H

#include "asdu_base.h"


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

	/*************************************
	传送原因COT定义
	**************************************/
	COT_NOUSED = 0x00,   // 未使用                                            
	COT_PERCYC = 0x01,   // 周期、循环                                        
	COT_BACK = 0x02,   // 背景扫描                                          
	COT_SPONT = 0x03,   // 突发（自发）                                      
	COT_INIT = 0x04,   // 初始化完成                                        
	COT_REQ = 0x05,   // 请求或被请求                                      
	COT_ACT = 0x06,   // 激活                                              
	COT_ACTCON = 0x07,   // 激活确认                                          
	COT_DEACT = 0x08,   // 停止激活                                          
	COT_DEACTCON = 0x09,   // 停止激活确认                                      
	COT_ACTTERM = 0x0a,   // 激活终止                                          
	COT_RETREM = 0x0b,   // 远方命令引起的返送信息                            
	COT_RETLOC = 0x0c,   // 当地命令引起的返送信息                            
	COT_FILE = 0x0d,   // 文件传输                                          
	COT_INTROGEN = 0x14,   // 响应站召唤                                        
	COT_INTRO1 = 0x15,   //响应第1组召唤                                      
	COT_INTRO2 = 0x16,   //响应第2组召唤                                      
	COT_INTRO3 = 0x17,   //响应第3组召唤                                      
	COT_INTRO4 = 0x18,   //响应第4组召唤                                      
	COT_INTRO5 = 0x19,   //响应第5组召唤                                      
	COT_INTRO6 = 0x1a,   //响应第6组召唤                                      
	COT_INTRO7 = 0x1b,   //响应第7组召唤                                      
	COT_INTRO8 = 0x1c,   //响应第8组召唤                                      
	COT_INTRO9 = 0x1d,   //响应第9组召唤                                      
	COT_INTRO10 = 0x1e,   //响应第10组召唤                                     
	COT_INTRO11 = 0x1f,   //响应第11组召唤                                     
	COT_INTRO12 = 0x20,   //响应第12组召唤                                     
	COT_INTRO13 = 0x21,   //响应第13组召唤                                     
	COT_INTRO14 = 0x22,   //响应第14组召唤                                     
	COT_INTRO15 = 0x23,   //响应第15组召唤                                     
	COT_INTRO16 = 0x24,   //响应第16组召唤                                     
	COT_REQCOGEN = 0x25,   // 响应计数量总召唤                                  
	COT_REQCO1 = 0x26,   // 响应第1组计数量召唤                               
	COT_REQCO2 = 0x27,   // 响应第2组计数量召唤                               
	COT_REQCO3 = 0x28,   // 响应第3组计数量召唤                               
	COT_REQCO4 = 0x29,   // 响应第4组计数量召唤                               

	COT_UNKNOW_TYPE = 0x2c,   //未知的类型标示                                     
	COT_UNKNOW_COT = 0x2d,   //未知的传送原因                                     
	COT_UNKNOW_ADDR = 0x2e,   //未知的应用服务数据单元地址                         
	COT_UNKNOW_INF = 0x2f,   //未知的信息体对象地址                               
};


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

//继电保护设备事件的品质描述词
struct QDP
{
	unsigned char RES : 3;    //备用
	unsigned char EI : 1;     //0：动作时间有效 1：动作时间无效
	unsigned char BL : 1;     //0: 未被闭锁 1：被闭锁
	unsigned char SB : 1;     //0: 未被取代 1：被取代
	unsigned char NT : 1;     //0: 当前值   1：非当前值
	unsigned char IV : 1;     //0: 有效     1：无效
};

//带品质描述词的单点信息
struct SIQ
{
	unsigned char SPI : 1;    //值
	unsigned char RES : 3;    //备用
	unsigned char BL : 1;     //0: 未被闭锁 1：被闭锁
	unsigned char SB : 1;     //0: 未被取代 1：被取代
	unsigned char NT : 1;     //0: 当前值   1：非当前值
	unsigned char IV : 1;     //0: 有效     1：无效
};

//带品质描述词的双点信息
struct DIQ
{
	unsigned char DPI : 2;    //值
	unsigned char RES : 2;    //备用
	unsigned char BL : 1;     //0: 未被闭锁 1：被闭锁
	unsigned char SB : 1;     //0: 未被取代 1：被取代
	unsigned char NT : 1;     //0: 当前值   1：非当前值
	unsigned char IV : 1;     //0: 有效     1：无效
};

//带瞬变状态指示的值
struct VTI
{
	unsigned short value : 15;     //值
	unsigned short BS : 1;     //瞬变状态 0:未在瞬变状态 1:处于瞬变状态
};

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

//命令限定词
struct QOC
{
	unsigned char QU : 5;  //0:无另外的定义 1:短脉冲持续时间 2:长脉冲持续时间 3:持续输出
	unsigned char SE : 1;  //0:执行 1:选择
};

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


//步调节命令
struct RCO
{
	unsigned char SCS : 2;   //0:不允许 1:升一步 2:降一步 3:不允许
	struct QOC qoc; //命令限定词
};

//传送原因
struct REASON
{
	unsigned char cause : 6;   //传送原因
	unsigned char PN : 1;      //0:肯定确认 1:否定确认
	unsigned char T : 1;       //0:未试验 1:试验
};



//两个八位位组二进制时间
struct CP16Time2a
{
	unsigned char msL;     //毫秒低
	unsigned char msH;     //毫秒高
};


enum
{
	MAX_ASDU_SIZE = 249,
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
		INFOADDR      infoaddr;
		SIQ           siq;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
	SIQ GetSIQ(int nIndex);
public:
	enum { MAX_DATA_PER_ASDU1_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU1_SQ0_DATA), };
	ASDU1_SQ0_DATA m_data[MAX_DATA_PER_ASDU1_SQ0];
};

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_SP_NA_1:单点遥信(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU1_SQ1 : public ASDU_BASE
{
public:
	enum { MAX_DATA_PER_ASDU1_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(SIQ), };
public:
	INFOADDR    infoaddr;
	SIQ                m_data[MAX_DATA_PER_ASDU1_SQ1];
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
	SIQ GetSIQ(int nIndex);
public:
	int  GetAsdu1_SQ1Length();
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
		INFOADDR      m_infoaddr;
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


//////////////////////////////////////////////////////////////////////////
//M_DP_NA_1:双点遥信(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU3_SQ0 : public ASDU_BASE
{
	class ASDU3_SQ0_DATA //离散的信号量
	{
	public:
		INFOADDR      infoaddr;
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


//////////////////////////////////////////////////////////////////////////
//M_DP_NA_1:双点遥信(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU3_SQ1 : public ASDU_BASE
{
public:
	enum { MAX_DATA_PER_ASDU3_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(DIQ), };
public:
	INFOADDR      infoaddr;
	DIQ                  m_data[MAX_DATA_PER_ASDU3_SQ1];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	DIQ GetDIQ(int nIndex);
public:
	int  GetAsdu3_SQ1Length();
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
		INFOADDR      m_infoaddr;
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


//////////////////////////////////////////////////////////////////////////
//M_ST_NA_1:不带时标的步位置信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU5_SQ0 : public ASDU_BASE
{
private:
	class ASDU5_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		VTI                   m_vti;
		QDS                 m_qds;
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
	enum { MAX_DATA_PER_ASDU5_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU5_SQ0_DATA), };
	ASDU5_SQ0_DATA m_data[MAX_DATA_PER_ASDU5_SQ0];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ST_NA_1:不带时标的步位置信息(连续)
//////////////////////////////////////////////////////////////////////////
class ASDU5_SQ1 : public ASDU_BASE
{
public:
	class ASDU5_SQ1_DATA
	{
	public:
		VTI                   m_vti;
		QDS                 m_qds;
	};
	enum { MAX_DATA_PER_ASDU5_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU5_SQ1_DATA), };
public:
	INFOADDR      m_infoaddr;
	ASDU5_SQ1_DATA m_data[MAX_DATA_PER_ASDU5_SQ1];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);

public:
	QDS GetQDS(int nIndex);

public:
	int  GetAsdu5_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ST_TA_1:带时标的步位置信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU6_SQ0 : public ASDU_BASE
{
private:
	class ASDU6_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		VTI                   m_vti;
		QDS                 m_qds;
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
	QDS GetQDS(int nIndex);
public:
	enum { MAX_DATA_PER_ASDU6_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU6_SQ0_DATA), };
	ASDU6_SQ0_DATA m_data[MAX_DATA_PER_ASDU6_SQ0];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_BO_TA_1:带时标的32比特串(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU8_SQ0 : public ASDU_BASE
{
private:
	class ASDU8_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		int                   m_bsi;
		QDS                 m_qds;
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
	QDS GetQDS(int nIndex);

public:
	enum { MAX_DATA_PER_ASDU8_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU8_SQ0_DATA), };
	ASDU8_SQ0_DATA m_data[MAX_DATA_PER_ASDU8_SQ0];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ME_NA_1:归一化测量值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU9_SQ0 : public ASDU_BASE
{
	class ASDU9_SQ0_DATA
	{
	public:
		INFOADDR      infoaddr;
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
	enum { MAX_DATA_PER_ASDU9_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU9_SQ1_DATA), };
public:
	INFOADDR      infoaddr;
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
//M_ME_TA_1:测量值，带时标的归一化值(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU10_SQ0 : public ASDU_BASE
{
private:
	class ASDU10_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		short                m_nva;
		QDS                 m_qds;
		CP24Time2a    m_time;
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
	int  GetAsdu10_SQ0Length();
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
		INFOADDR      infoaddr;
		short m_nValue;
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU11_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU11_SQ0_DATA), };
public:
	ASDU11_SQ0_DATA        m_data[MAX_DATA_PER_ASDU11_SQ0];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int  GetAsdu11_SQ0Length();
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
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU11_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU11_SQ1_DATA), };
public:
	INFOADDR      infoaddr;
	ASDU11_SQ1_DATA        m_data[MAX_DATA_PER_ASDU11_SQ1];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	short GetValue(int nIndex);
	void SetValue(int nIndex, short nValue);
	void SetQDS(int nIndex, unsigned char nQds);
	QDS GetQDS(int nIndex);
public:
	int  GetAsdu11_SQ1Length();
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
		INFOADDR      m_infoaddr;
		short                m_sva;
		QDS                 m_qds;
		CP24Time2a    m_time;
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
	int  GetAsdu12_SQ0Length();
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
		INFOADDR      infoaddr;
		float m_fValue;
		QDS  m_qds;
	};
	enum { MAX_DATA_PER_ASDU13_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU13_SQ0_DATA), };
public:
	ASDU13_SQ0_DATA        m_data[MAX_DATA_PER_ASDU13_SQ0];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	float GetValue(int nIndex);
	void SetValue(int nIndex, float fValue);

public:
	QDS GetQDS(int nIndex);

public:
	int  GetAsdu13_SQ0Length();
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
	enum { MAX_DATA_PER_ASDU13_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU13_SQ1_DATA), };
public:
	INFOADDR      infoaddr;
	ASDU13_SQ1_DATA        m_data[MAX_DATA_PER_ASDU13_SQ1];
public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	float GetValue(int nIndex);
	void SetValue(int nIndex, float fValue);

public:
	QDS GetQDS(int nIndex);

public:
	int  GetAsdu13_SQ1Length();
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
		INFOADDR      m_infoaddr;
		float                m_std;
		QDS                 m_qds;
		CP24Time2a    m_time;
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
	int  GetAsdu14_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU14_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU14_SQ0_DATA), };
	ASDU14_SQ0_DATA m_data[MAX_DATA_PER_ASDU14_SQ0];
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_IT_NA_1:累计量(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU15_SQ0 : public ASDU_BASE
{
private:
	class ASDU15_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		int                m_nValue;
		char                 m_bcrparam;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	int  GetAsdu15_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU15_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU15_SQ0_DATA), };
	ASDU15_SQ0_DATA m_data[MAX_DATA_PER_ASDU15_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


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
	enum { MAX_DATA_PER_ASDU15_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU15_SQ1_DATA), };
public:
	INFOADDR      infoaddr;
	ASDU15_SQ1_DATA         m_data[MAX_DATA_PER_ASDU15_SQ1];

public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	int  GetAsdu15_SQ1Length();
};
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//M_IT_TA_1:带时标的累计量(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU16_SQ0 : public ASDU_BASE
{
private:
	class ASDU16_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		float                m_fValue;
		char                 m_bcrparam;
		CP24Time2a    m_time;
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
	int  GetAsdu16_SQ0Length();
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
		INFOADDR      m_infoaddr;
		int                    m_nSCD;
		QDS                 m_qds;
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
		int                    m_nSCD;
		QDS                 m_qds;
	};
	enum { MAX_DATA_PER_ASDU20_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU20_SQ1_DATA), };
public:
	INFOADDR      m_infoaddr;
	ASDU20_SQ1_DATA         m_data[MAX_DATA_PER_ASDU20_SQ1];

public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	QDS GetQDS(int nIndex);
public:
	int  GetAsdu20_SQ1Length();
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
		INFOADDR      m_infoaddr;
		short                m_nva;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	int  GetAsdu21_SQ0Length();
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
	enum { MAX_DATA_PER_ASDU21_SQ1 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE) - sizeof(INFOADDR)) / sizeof(ASDU21_SQ1_DATA), };
public:
	INFOADDR      m_infoaddr;
	ASDU21_SQ1_DATA         m_data[MAX_DATA_PER_ASDU21_SQ1];

public:
	void SetItemCount(int nCount);
	int   GetItemCount() const;
public:
	void SetValue(int nIndex, short nValue);
	short GetValue(int nIndex);
public:
	int  GetAsdu21_SQ1Length();
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
		INFOADDR       m_infoaddr;
		SIQ                   m_siq;
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
		INFOADDR       m_infoaddr;
		DIQ                   m_diq;
		CP56Time2a     m_time;
	};
public:
	enum { MAX_DATA_PER_ASDU31_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU31_SQ0_DATA), };
	ASDU31_SQ0_DATA        m_data[MAX_DATA_PER_ASDU31_SQ0];
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
	DIQ GetDIQ(int nIndex);
public:
	int  GetAsdu31_SQ0Length();
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_ST_TB_1:带时标的步位置信息(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU32_SQ0 : public ASDU_BASE
{
private:
	class ASDU32_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		VTI                   m_vti;
		QDS                 m_qds;
		CP56Time2a    m_time;
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
		INFOADDR      m_infoaddr;
		short                m_nva;
		QDS                 m_qds;
		CP56Time2a    m_time;
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
	int  GetAsdu34_SQ0Length();
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
		INFOADDR      m_infoaddr;
		short                m_sva;
		QDS                 m_qds;
		CP56Time2a    m_time;
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
	int  GetAsdu35_SQ0Length();
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
		INFOADDR      m_infoaddr;
		float                m_fValue;
		QDS                 m_qds;
		CP56Time2a    m_time;
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
	int  GetAsdu36_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU36_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU36_SQ0_DATA), };
	ASDU36_SQ0_DATA m_data[MAX_DATA_PER_ASDU36_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//M_IT_TB_1:带时标CP56Time2a的累计量(离散)
//////////////////////////////////////////////////////////////////////////
class ASDU37_SQ0 : public ASDU_BASE
{
private:
	class ASDU37_SQ0_DATA
	{
	public:
		INFOADDR      m_infoaddr;
		BCR                  m_bcr;
		CP56Time2a    m_time;
	};
public:
	void SetItemCount(int nCount);
	int GetItemCount() const;
public:
	void SetValue(int nIndex, int nValue);
	int GetValue(int nIndex);
public:
	int  GetAsdu37_SQ0Length();
public:
	enum { MAX_DATA_PER_ASDU37_SQ0 = (MAX_ASDU_SIZE - sizeof(ASDU_BASE)) / sizeof(ASDU37_SQ0_DATA), };
	ASDU37_SQ0_DATA m_data[MAX_DATA_PER_ASDU37_SQ0];
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//C_SC_NA_1:单点命令(遥控：控分控合)
//////////////////////////////////////////////////////////////////////////
class ASDU45 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	SCO  m_sco;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//C_DC_NA_1:双点命令
//////////////////////////////////////////////////////////////////////////
class ASDU46 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	DCO  m_dco;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//C_SE_NA_1:归一化设点命令(遥控：设模拟量)
//////////////////////////////////////////////////////////////////////////
class ASDU48 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
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
	INFOADDR m_infoaddr;
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
	INFOADDR m_infoaddr;
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
	INFOADDR m_infoaddr;
	unsigned char m_qoi;
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_CI_NA_1:电能脉冲召唤命令
//////////////////////////////////////////////////////////////////////////
class ASDU101 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	unsigned char m_qcc;
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_CS_NA_1:时钟同步命令
//////////////////////////////////////////////////////////////////////////
class ASDU103 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	CP56Time2a m_time;
};
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif//_ASDU_104_H
