#ifndef RELAY_103_H
#define RELAY_103_H

#include <stdint.h>
#include <functional>

//�ɱ�ṹ�޶���
#define VSQ_COMMON1				0x81
#define VSQ_COMMON2				0x01

//����ԭ��
//���Ʒ���
#define COTC_TIMESYNC	8	// ʱ��ͬ��
#define COTC_GENQUERY	9	// �ܲ�ѯ
#define COTC_COMMAND	20	// һ������
#define COTC_DTB_DATA	31	// �Ŷ�����
#define COTC_WAVE_DATA	32	// �����ļ�
#define COTC_GRC_WRITE	40	// ͨ�÷���д����
#define COTC_GRC_READ	42  // ͨ�÷��������
#define COTC_HIS		128	// ��ʷ��ѯ

//���ӷ���
#define COT_SPONTANEOUS	1	// ͻ��
#define COT_CYCLIC		2	// ѭ��
#define COT_RESET_FCB	3	// ��λ֡������
#define COT_RESET_CU	4	// ��λ
#define COT_START		5	// ����	
#define COT_POWER_ON	6	// �ϵ�
#define COT_TEST_MODE	7	// ����ģʽ	
#define COT_TIMESYNC	8	// ʱ��ͬ��
#define COT_GI			9	// �ܲ�ѯ
#define COT_TERM_GI		10  // �ܲ�ѯ��ֹ
#define COT_LOCAL_OP	11  // ���ز���
#define COT_REMOTE_OP	12  // Զ������
#define COT_ACK			20	// �϶�ȷ��
#define COT_NAK			21	// ��ȷ��
#define COT_DTB_DATA	31	// �Ŷ�����
#define COT_WAVE_DATA	32	// �����ļ�
#define COT_GRCW_ACK_P	40  // ͨ��д�϶�ȷ��
#define COT_GRCW_ACK_N	41  // ͨ��д��ȷ��
#define COT_GRCR_VALID	42	// ͨ�ö���Ч
#define COT_GRCR_INV	43  // ͨ�ö���Ч
#define COT_GRCW_CFM	44	// ͨ��дȷ��
#define COT_HIS			128	// ��ʷ��ѯ
#define COT_HISOVER		129	// ��ʷ��ѯ����
#define COT_TELECONTROL 200	// ͻ��

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

//ASDU ����ṹ
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
	unsigned short  Length;				// ʵ�ʱ��ĳ���
	unsigned char	ControlField1;		// ������1
	unsigned char	ControlField2;		// ������2
	unsigned char	ControlField3;		// ������3
	unsigned char	ControlField4;		// ������4
	unsigned char	SrcStationID;		// Դ��վ��
	unsigned short	SrcDevID;			// Դ��ַ
	unsigned char	DestStationID;		// Ŀ�곧վ��
	unsigned short	DestDevID;			// Ŀ���ַ
	unsigned char	Res1;				// ����1
	unsigned char	Res2;				// ����2
} ;

typedef struct tagIEC103AsduHead
{
	uint8_t asdu_type;    //!< ���ͱ�ʶ
	uint8_t vsq;          //!< �ɱ�ṹ�޶���
	uint8_t cot;          //!< ����ԭ��
	uint8_t asdu_addr;    //!< Ӧ�÷������ݵ�Ԫ������ַ
	uint8_t fun;          //!< ��������
	uint8_t inf;          //!< ��Ϣ���
	uint8_t rii;          //!< ������Ϣ��ʶ��
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
//ͨ�÷������ݼ�¼
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
		TESTFR_CON = 0x83,  //!< ����֡ 
		TESTFR_ACT = 0x43,  //!< ����֡ 
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
		NETD_FORMAT_I = 0, // ���ڱ�ŵ���Ϣ���䣨I��ʽ������֡
		NETD_FORMAT_S = 1,  // ��ŵļ��ӹ��ܣ�S��ʽ��    ȷ��֡
		NETD_FORMAT_U = 2,  // δ��ŵĿ��ƹ��ܣ�U��ʽ��  ����֡������֡
		NETD_FORMAT_ERR = 3,
	};

	//ASDU Ӧ�����Ͷ��� 
	enum ASDU_TYPE
	{	
		//���Ʒ��� 
		TYPC_TIMESYNC		=	0x06,   //!< ʱ��ͬ��
		TYPC_COMMAND		=	0x14,   //!< һ������
		TYPC_DTB_COMMAND	=	24,     //!< �Ŷ����ݴ�������� 
		TYPC_DTB_CONFIRM	=	25,     //!< �Ŷ����ݴ�����Ͽ� 
		TYPC_GRC_DATA		=	10,     //!< ͨ�÷�������
		TYPC_GRC_COMMAND	=	21,     //!< ͨ�÷�������
		TYPC_WAVE_AFFIRM	=	231,
		TYPC_WAVE_TRANSCMD	=	233,
		TYPC_WAVE_OVER		=	235,
		//���ӷ���
		TYPW_MARK			=	5,      //!< ��ʶASDU5
		TYPW_GRC_DATA		=	10,     //!< ͨ�÷�������ASDU10
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

	//COT����
	enum COT_TYPE
	{
		//���ӷ���
		M_COT_SPON				= 1,    //!< �Է�(ͻ��)
		M_COT_CYC				= 2,    //!< ѭ��
		M_COT_RST_FCB			= 3,    //!< ��λ֡����λ
		M_COT_RST_CU		    = 4,    //!< ��λͨ�ŵ�Ԫ
		M_COT_RESTART			= 5,    //!< ����/��������
		M_COT_POWER_UP			= 6,    //!< ��Դ����
		M_COT_TEST				= 7,    //!< ����ģʽ
		M_COT_SYN				= 8,    //!< ʱ��ͬ��
		M_COT_GI				= 9,    //!< ���ٻ�
		M_COT_TGI				= 10,   //!< ���ٻ���ֹ
		M_COT_LOCAL				= 11,   //!< ���ز���
		M_COT_REMOTE			= 12,   //!< Զ������
		M_COT_POSITIVE_CMD		= 20,   //!< ����Ŀ϶��Ͽ�
		M_COT_NEGATIVE_CMD		= 21,   //!< ����ķ��Ͽ�
		M_COT_DSTB				= 31,   //!< �Ŷ����ݵĴ���
		M_COT_POSITIVE_GCWR		= 40,   //!< ͨ�÷���д����Ŀ϶��Ͽ�
		M_COT_NEGATIVE_GCWR		= 41,   //!< ͨ�÷���д����ķ��Ͽ�
		M_COT_RSP_GCRD_VAL		= 42,   //!< ��ͨ�÷����������Ч������Ӧ
		M_COT_RSP_GCRD_INV		= 43,   //!< ��ͨ�÷����������Ч������Ӧ
		M_COT_GCWR_CON			= 44,   //!< ͨ�÷���дȷ��
		//���Ʒ���
		C_COT_CYC				= 2,    //!< ѭ��
		C_COT_SYN				= 8,    //!< ʱ��ͬ��
		C_COT_GI				= 9,    //!< ���ٻ�
		C_COT_G_CMD				= 20,   //!< һ������
		C_COT_DSTB				= 31,   //!< �Ŷ����ݵĴ���
		C_COT_GC_WR				= 40,   //!< ͨ�÷���д����
		C_COT_GC_RD				= 42,   //!< ͨ�÷��������
	};

	//FUN����
	enum FUN_TYPE
	{
		IEC103_FUN_TYP_TM   = 242,  //!< �Ŷ����ݹ�������
		IEC103_FUN_TYP_GEN	= 254,  //!< ͨ�÷��๦������
		IEC103_FUN_TYP_GLB	= 255,  //!< ȫ�ֹ�������
	};

	//INF����
	enum INF_TYPE
	{
		//���Ʒ���
		INF_C_RD_ALL_GROUP_TLT		    = 240,     //!< 0xF0:��ȫ�����������ı���
		INF_C_RD_ONE_GROUP_ALL_ITM	    = 241,     //!< 0xF1:��һ�����ȫ����Ŀ��ֵ������
		INF_C_RD_ONE_ITM_DIR		    = 243,     //!< 0xF3:��������Ŀ��Ŀ¼
		INF_C_RD_ONE_ITM_ATTR		    = 244,     //!< 0xF4:��������Ŀ��ֵ������
		INF_C_GI					    = 245,     //!< 0xF5:��ͨ�÷������ݵ��ܲ�ѯ
		INF_C_WR_ITEM				    = 248,     //!< 0xF8:д��Ŀ
		INF_C_WR_ITEM_CON			    = 249,     //!< 0xF9:��ȷ�ϵ�д��Ŀ
		INF_C_WR_ITEM_EXE			    = 250,     //!< 0xFA:��ִ�е�д��Ŀ
		INF_C_WR_ITEM_CANCEL		    = 251,     //!< 0xFB:д��Ŀ��ֹ
		//���ӷ���
        INF_M_RD_ALL_GROUP_TLT		    = 240,     //!< 0xF0:�����б�������ı���
        INF_M_RD_ONE_GROUP_ALL_ITM	    = 241,     //!< 0xF1:��һ�����ȫ����Ŀ��ֵ������
        INF_M_RD_ONE_ITM_DIR		    = 243,     //!< 0xF3:��������Ŀ��Ŀ¼
        INF_M_RD_ONE_ITM_ATTR		    = 244,     //!< 0xF4:��������Ŀ��ֵ������
        INF_M_GI_END				    = 245,     //!< 0xF5:ͨ�÷������ݵ��ܲ�ѯ��ֹ
        INF_M_WR_ITEM_CON			    = 249,     //!< 0xF9:��ȷ�ϵ�д��Ŀ
        INF_M_WR_ITEM_EXE			    = 250,     //!< 0xFA:��ִ�е�д��Ŀ
        INF_M_WR_ITEM_CANCEL		    = 251,     //!< 0xFB:����ֹ��д��Ŀ
	};


public:
	// ��ʱ
	void ISyncTime();
	// �ܲ�ѯ
	void IGeneralQuary( );
	// ����֡
	unsigned int ISendStartDT(unsigned char *pAPDU,unsigned int nLen) const;
	// ����Ŀ¼
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

	bool m_bReqCatalogFlag;   //! �Ƿ������
	bool m_bBuiltDBFlag;      //! �Ƿ��Ѿ����ɹ�

	unsigned char m_byRII;    //!< ������Ϣ��ʶ��RII
public:
	// ���ͽӿ�
	std::function<bool(const char* const pstr[], const size_t len[], size_t num)  > m_fnSendData;

	std::function<std::string()  > m_fnGetRemoteEndPoint;
};



#endif // RELAY_103_H
