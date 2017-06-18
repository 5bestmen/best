#ifndef RELAY_103_H
#define RELAY_103_H

#include <stdint.h>
#include <functional>

//可变结构限定词
#define VSQ_COMMON1				0x81
#define VSQ_COMMON2				0x01

//传送原因
//控制方向
#define COTC_TIMESYNC	8	// 时间同步
#define COTC_GENQUERY	9	// 总查询
#define COTC_COMMAND	20	// 一般命令
#define COTC_DTB_DATA	31	// 扰动数据
#define COTC_WAVE_DATA	32	// 波形文件
#define COTC_GRC_WRITE	40	// 通用分类写命令
#define COTC_GRC_READ	42  // 通用分类读命令
#define COTC_HIS		128	// 历史查询

//监视方向
#define COT_SPONTANEOUS	1	// 突发
#define COT_CYCLIC		2	// 循环
#define COT_RESET_FCB	3	// 复位帧计数器
#define COT_RESET_CU	4	// 复位
#define COT_START		5	// 启动	
#define COT_POWER_ON	6	// 上电
#define COT_TEST_MODE	7	// 测试模式	
#define COT_TIMESYNC	8	// 时间同步
#define COT_GI			9	// 总查询
#define COT_TERM_GI		10  // 总查询中止
#define COT_LOCAL_OP	11  // 当地操作
#define COT_REMOTE_OP	12  // 远方操作
#define COT_ACK			20	// 肯定确认
#define COT_NAK			21	// 否定确认
#define COT_DTB_DATA	31	// 扰动数据
#define COT_WAVE_DATA	32	// 波形文件
#define COT_GRCW_ACK_P	40  // 通用写肯定确认
#define COT_GRCW_ACK_N	41  // 通用写否定确认
#define COT_GRCR_VALID	42	// 通用读有效
#define COT_GRCR_INV	43  // 通用读无效
#define COT_GRCW_CFM	44	// 通用写确认
#define COT_HIS			128	// 历史查询
#define COT_HISOVER		129	// 历史查询结束
#define COT_TELECONTROL 200	// 突发

#define LONGTIME_MSEC	0
#define LONGTIME_MIN	2
#define LONGTIME_HOUR	3
#define LONGTIME_DAY	4
#define LONGTIME_MON	5
#define LONGTIME_YEAR	6

#define GENREQCOM_FUN	0
#define GENREQCOM_INF	1
#define GENREQCOM_RII	2
#define GENREQCOM_NOG	3
#define GENREQCOM_GIR	4

//ASDU 命令结构
// #define ASDU_TYPE	0
// #define ASDU_VSQ	1
// #define ASDU_COT	2
// #define ASDU_ADDR	3
// #define ASDU_FUN	4
// #define ASDU_INF	5

#define ASDU20_CC	6
#define ASDU20_RII	7

#define ASDU10_RII	6
#define ASDU10_NGD	7

#define ASDU21_RII	6
#define ASDU21_NOG	7

#define ASDU24_TOO	6
#define ASDU24_TOV	7
#define ASDU24_FAN	8
#define ASDU24_ACC	10

#define GENDATACOM_FUN	0
#define GENDATACOM_INF	1
#define GENDATACOM_RII	2
#define GENDATACOM_NGD	3
#define GENDATACOM_GDR	4

#define GENDATAREC_GIN	0
#define GENDATAREC_KOD	2
#define GENDATAREC_GDD	3
#define GENDATAREC_GID	6
#define GENDATAREC_SIZE 6


#define GDDTYPE_TYP	0
#define GDDTYPE_SIZ	1
#define GDDTYPE_NUM	2



#define NETDHEADSIZE		15
#define NETDCALHEADSIZE		12
#define NETDLEFTHEADSIZE	3

#define MAX_DATA_NUMBER		0x7fff


#pragma pack(push, 1)
struct Net103PacketHead
{
	unsigned char	Start;				// 0x68H
	unsigned short  Length;				// 实际报文长度
	unsigned char	ControlField1;		// 控制域1
	unsigned char	ControlField2;		// 控制域2
	unsigned char	ControlField3;		// 控制域3
	unsigned char	ControlField4;		// 控制域4
	unsigned char	SrcStationID;		// 源厂站号
	unsigned short	SrcDevID;			// 源地址
	unsigned char	DestStationID;		// 目标厂站号
	unsigned short	DestDevID;			// 目标地址
	unsigned char	Res1;				// 备用1
	unsigned char	Res2;				// 备用2
} ;

typedef struct tagIEC103AsduHead
{
	uint8_t asdu_type;    //!< 类型标识
	uint8_t vsq;          //!< 可变结构限定词
	uint8_t cot;          //!< 传送原因
	uint8_t asdu_addr;    //!< 应用服务数据单元公共地址
	uint8_t fun;          //!< 功能类型
	uint8_t inf;          //!< 信息序号
	uint8_t rii;          //!< 返回信息标识符
}IEC103_ASDU_HEAD;
#define ASDU_HEAD_LEN    sizeof(IEC103_ASDU_HEAD)

struct SGDDType
{
	uchar	byTYPE;
	uchar	bySIZE;
	uchar	byNUMBER : 7;
	uchar	byCONT   : 1;
};

struct SGINType
{
	uchar	byGROUP;
	uchar	byENTRY;
};
//通用分类数据记录
struct SGenDataRec
{
	union 
	{
		ushort		wGIN;
		SGINType	GIN;
	};
	uchar			byKOD;
	SGDDType		GDD;
	uchar*			GID;
};


#pragma pack(pop)


class C103Impl
{
public:
	C103Impl();
	~C103Impl();
public:
	enum
	{
		STARTCHAR = 0x68,
	};
	
	enum
	{
		TESTFR_CON = 0x83,  //!< 测试帧 
		TESTFR_ACT = 0x43,  //!< 测试帧 
		STOPDT_CON = 0x23,
		STOPDT_ACT = 0x13,
		STARTDT_CON = 0x0b,
		STARTDT_ACT = 0x07,
	};

	enum ASDU_OFFSET
	{
		ASDU_TYPE = 0,
		ASDU_VSQ  = 1,
		ASDU_COT  = 2,
		ASDU_ADDR = 3,
		ASDU_FUN  = 4,
		ASDU_INF  = 5,
	};

	enum FRM_TYPE
	{
		NETD_FORMAT_I = 0, // 用于编号的信息传输（I格式）数据帧
		NETD_FORMAT_S = 1,  // 编号的监视功能（S格式）    确认帧
		NETD_FORMAT_U = 2,  // 未编号的控制功能（U格式）  启动帧、测试帧
		NETD_FORMAT_ERR = 3,
	};

	//ASDU 应用类型定义 
	enum ASDU_TYPE
	{	
		//控制方向 
		TYPC_TIMESYNC		=	0x06,   //!< 时间同步
		TYPC_COMMAND		=	0x14,   //!< 一般命令
		TYPC_DTB_COMMAND	=	24,     //!< 扰动数据传输的命令 
		TYPC_DTB_CONFIRM	=	25,     //!< 扰动数据传输的认可 
		TYPC_GRC_DATA		=	10,     //!< 通用分类数据
		TYPC_GRC_COMMAND	=	21,     //!< 通用分类命令
		TYPC_WAVE_AFFIRM	=	231,
		TYPC_WAVE_TRANSCMD	=	233,
		TYPC_WAVE_OVER		=	235,
		//监视方向
		TYPW_MARK			=	5,      //!< 标识ASDU5
		TYPW_GRC_DATA		=	10,     //!< 通用分类数据ASDU10
		TYPW_GRC_MARK		=	11,
		TYPW_DTB_TABLE		=	23,
		TYPW_DTB_DATAREADY	=	26,
		TYPW_DTB_CHREADY	=	27,
		TYPW_DTB_STATEREADY	=	28,
		TYPW_DTB_TRANSSTATE	=	29,
		TYPW_DTB_TRANSDATA	=	30,
		TYPW_DTB_END		=	31,
		TYPW_WAVE_TABLE		=	230,
		TYPW_WAVE_DATA		=	232, 
	};

	//COT定义
	enum COT_TYPE
	{
		//监视方向
		M_COT_SPON				= 1,    //!< 自发(突发)
		M_COT_CYC				= 2,    //!< 循环
		M_COT_RST_FCB			= 3,    //!< 复位帧计数位
		M_COT_RST_CU		    = 4,    //!< 复位通信单元
		M_COT_RESTART			= 5,    //!< 启动/重新启动
		M_COT_POWER_UP			= 6,    //!< 电源合上
		M_COT_TEST				= 7,    //!< 测试模式
		M_COT_SYN				= 8,    //!< 时间同步
		M_COT_GI				= 9,    //!< 总召唤
		M_COT_TGI				= 10,   //!< 总召唤终止
		M_COT_LOCAL				= 11,   //!< 当地操作
		M_COT_REMOTE			= 12,   //!< 远方操作
		M_COT_POSITIVE_CMD		= 20,   //!< 命令的肯定认可
		M_COT_NEGATIVE_CMD		= 21,   //!< 命令的否定认可
		M_COT_DSTB				= 31,   //!< 扰动数据的传送
		M_COT_POSITIVE_GCWR		= 40,   //!< 通用分类写命令的肯定认可
		M_COT_NEGATIVE_GCWR		= 41,   //!< 通用分类写命令的否定认可
		M_COT_RSP_GCRD_VAL		= 42,   //!< 对通用分类读命令有效数据响应
		M_COT_RSP_GCRD_INV		= 43,   //!< 对通用分类读命令无效数据响应
		M_COT_GCWR_CON			= 44,   //!< 通用分类写确认
		//控制方向
		C_COT_CYC				= 2,    //!< 循环
		C_COT_SYN				= 8,    //!< 时间同步
		C_COT_GI				= 9,    //!< 总召唤
		C_COT_G_CMD				= 20,   //!< 一般命令
		C_COT_DSTB				= 31,   //!< 扰动数据的传输
		C_COT_GC_WR				= 40,   //!< 通用分类写命令
		C_COT_GC_RD				= 42,   //!< 通用分类读命令
	};

	//FUN定义
	enum FUN_TYPE
	{
		IEC103_FUN_TYP_TM   = 242,  //!< 扰动数据功能类型
		IEC103_FUN_TYP_GEN	= 254,  //!< 通用分类功能类型
		IEC103_FUN_TYP_GLB	= 255,  //!< 全局功能类型
	};

	//INF定义
	enum INF_TYPE
	{
		//控制方向
		INF_C_RD_ALL_GROUP_TLT		    = 240,     //!< 0xF0:读全部被定义的组的标题
		INF_C_RD_ONE_GROUP_ALL_ITM	    = 241,     //!< 0xF1:读一个组的全部条目的值或属性
		INF_C_RD_ONE_ITM_DIR		    = 243,     //!< 0xF3:读单个条目的目录
		INF_C_RD_ONE_ITM_ATTR		    = 244,     //!< 0xF4:读单个条目的值或属性
		INF_C_GI					    = 245,     //!< 0xF5:对通用分类数据的总查询
		INF_C_WR_ITEM				    = 248,     //!< 0xF8:写条目
		INF_C_WR_ITEM_CON			    = 249,     //!< 0xF9:带确认的写条目
		INF_C_WR_ITEM_EXE			    = 250,     //!< 0xFA:带执行的写条目
		INF_C_WR_ITEM_CANCEL		    = 251,     //!< 0xFB:写条目终止
		//监视方向
        INF_M_RD_ALL_GROUP_TLT		    = 240,     //!< 0xF0:读所有被定义组的标题
        INF_M_RD_ONE_GROUP_ALL_ITM	    = 241,     //!< 0xF1:读一个组的全部条目的值或属性
        INF_M_RD_ONE_ITM_DIR		    = 243,     //!< 0xF3:读单个条目的目录
        INF_M_RD_ONE_ITM_ATTR		    = 244,     //!< 0xF4:读单个条目的值或属性
        INF_M_GI_END				    = 245,     //!< 0xF5:通用分类数据的总查询终止
        INF_M_WR_ITEM_CON			    = 249,     //!< 0xF9:带确认的写条目
        INF_M_WR_ITEM_EXE			    = 250,     //!< 0xFA:带执行的写条目
        INF_M_WR_ITEM_CANCEL		    = 251,     //!< 0xFB:带终止的写条目
	};


public:
	// 对时
	void ISyncTime();
	// 总查询
	void IGeneralQuary( );
	// 启动帧
	unsigned int ISendStartDT(unsigned char *pAPDU,unsigned int nLen) const;
	// 读组目录
	unsigned int IReadGroupCatalog() ;

	bool HandleMsg(const char *pBuff, size_t nLen);

	bool IsReqestedCatalog()
	{
		return m_bReqCatalogFlag;
	}
	bool IsBuiltDB()
	{
		return m_bBuiltDBFlag;
	}
	
protected:
	void PackApci(unsigned char *pApci);
	void PackAsdu();
	void PackApdu();

	//
	unsigned int PackTimeSync(unsigned char *pAsdu);
	// 
	unsigned int PackGeneralQuary(unsigned char *pAsdu);

	unsigned int GetFrameType(Net103PacketHead *pHead);

	// U
	bool HandleUMsg(Net103PacketHead *pHead);
	// I
	bool HandleIMsg(const char *pBuff, size_t nLen);
	// S
	bool HandleSMsg(const char *pBuff, size_t nLen);

	void HandleASDU5();	 

	bool HandleASDU10(unsigned char *pAsdu, unsigned int nLen);
	bool AsduParser240(unsigned char *pAsdu, unsigned int nLen);
	bool AsduParser241(unsigned char *pAsdu, unsigned int nLen);
	void InitPacketHead(Net103PacketHead *pHead, unsigned int nFormat) const;

	void InitPacketHead(Net103PacketHead *pHead, unsigned int nFormat, unsigned char nSrcStationAddr, unsigned short nSrcDeviceAddr, 
		unsigned char nDestStationAddr, unsigned short nDestDeviceAddr);
 
	bool HandleReadCatalog(unsigned char *pData, unsigned int nLen);

	unsigned int CalcGrcDataLen(unsigned char *pGdBuf);

private:
	void DoSth();
	bool Log(const char *szLogTxt, int nLevel);
	std::string GetRemoteEndPoint()
	{
		if (m_fnGetRemoteEndPoint)
		{
			return m_fnGetRemoteEndPoint();
		}
		return "";
	}

	bool SendData(const char *pDataBuf, size_t nLen);
	bool SendData(Net103PacketHead *pHead, const char *pDataBuf);

	void IncreaseNumber(unsigned short& wNumber);
	
private:
	unsigned short	m_nTcpSendNum;
	unsigned short	m_nTcpRecvNum;

	unsigned char  m_nSrcStationAddr;
	unsigned short m_nSrcDeviceAddr;
	unsigned char  m_nDestStationAddr;
	unsigned short m_nDestDeviceAddr;

	bool m_bReqCatalogFlag;   //! 是否请求过
	bool m_bBuiltDBFlag;      //! 是否已经生成过

	unsigned char m_byRII;    //!< 返回信息标识符RII
public:
	// 发送接口
	std::function<bool(const char* const pstr[], const size_t len[], size_t num)  > m_fnSendData;

	std::function<std::string()  > m_fnGetRemoteEndPoint;
};



#endif // RELAY_103_H
