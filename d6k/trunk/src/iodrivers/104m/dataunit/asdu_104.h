
//ͨ���������壺
//1:ASDU����
//2:�������Ͷ���
//3:ͨ�ú궨��

#ifndef _ASDU_104_H
#define _ASDU_104_H

#include "asdu_base.h"


enum
{
	/*�ڼ��ӷ���Ĺ�����Ϣ*/
	M_SP_NA_1 = 1,      // ������Ϣ                                          
	M_SP_TA_1 = 2,      // ��ʱ��ĵ�����Ϣ                                  
	M_DP_NA_1 = 3,      // ˫����Ϣ                                          
	M_DP_TA_1 = 4,      // ��ʱ���˫����Ϣ                                  
	M_ST_NA_1 = 5,      // ��λ����Ϣ                                        
	M_ST_TA_1 = 6,      // ��ʱ��Ĳ�λ����Ϣ                                
	M_BO_NA_1 = 7,      // 32���ش�                                          
	M_BO_TA_1 = 8,      // ��ʱ���32���ش�                                  
	M_ME_NA_1 = 9,      // ����ֵ����һ��ֵ                                  
	M_ME_TA_1 = 10,     // ��ʱ��Ĳ���ֵ����һ��ֵ                          
	M_ME_NB_1 = 11,     // ����ֵ����Ȼ�ֵ                                  
	M_ME_TB_1 = 12,     // ��ʱ��Ĳ���ֵ����Ȼ�ֵ                          
	M_ME_NC_1 = 13,     // ����ֵ���̸�����                                  
	M_ME_TC_1 = 14,     // ��ʱ��Ĳ���ֵ���̸�����                          
	M_IT_NA_1 = 15,     // �ۼ���                                            
	M_IT_TA_1 = 16,     // ��ʱ����ۼ���                                    
	M_EP_TA_1 = 17,     // ��ʱ��ļ̵籣��װ���¼�                          
	M_EP_TB_1 = 18,     // ��ʱ��ļ̵籣��װ�ó��������¼�                  
	M_EP_TC_1 = 19,     // ��ʱ��ļ̵籣��װ�ó��������Ϣ                  
	M_PS_NA_1 = 20,     // ��״̬����ĳ��鵥����Ϣ                          
	M_ME_ND_1 = 21,     // ����Ʒ�������Ĺ�һ������ֵ                        
	/*�ڼ��ӷ���Ĺ�����ϢϢ����ʱ���ASDU*/
	M_SP_TB_1 = 30,     // ��ʱ��CP56Time2a�ĵ�����Ϣ                        
	M_DP_TB_1 = 31,     // ��ʱ��CP56Time2a��˫����Ϣ                        
	M_ST_TB_1 = 32,     // ��ʱ��CP56Time2a�Ĳ�λ����Ϣ                      
	M_BO_TB_1 = 33,     // ��ʱ��CP56Time2a��32���ش�                        
	M_ME_TD_1 = 34,     // ��ʱ��CP56Time2a�Ĳ���ֵ����һ��ֵ                
	M_ME_TE_1 = 35,     // ��ʱ��CP56Time2a�Ĳ���ֵ����Ȼ�ֵ                
	M_ME_TF_1 = 36,     // ��ʱ��CP56Time2a�Ĳ���ֵ���̸�����                
	M_IT_TB_1 = 37,     // ��ʱ��CP56Time2a���ۼ���                          
	M_EP_TD_1 = 38,     // ��ʱ��CP56Time2a�ļ̵籣��װ���¼�                
	M_EP_TE_1 = 39,     // ��ʱ��CP56Time2a�ļ̵籣��װ�ó��������¼�        
	M_EP_TF_1 = 40,     // ��ʱ��CP56Time2a�ļ̵籣��װ�ó��������Ϣ        
	/*�ڼ��ӷ����ϵͳ��Ϣ*/
	M_EI_NA_1 = 70,     // ��ʼ������                                        
	/*�ڿ��Ʒ���Ĺ�����Ϣ*/
	C_SC_NA_1 = 45,     // ������                                            
	C_DC_NA_1 = 46,     // ˫����                                            
	C_RC_NA_1 = 47,     // ��������                                          
	C_SE_NA_1 = 48,     // ��������һ��ֵ                                
	C_SE_NB_1 = 49,     // ��������Ȼ�ֵ                                
	C_SE_NC_1 = 50,     // �������̸�����                                
	C_BO_NA_1 = 51,     // 32���ش�                                          
	/*�ڿ��Ʒ���Ĺ�����Ϣ����ʱ���ASDU*/
	C_SC_TA_1 = 58,     // ��ʱ��CP56Time2a�ĵ�����                          
	C_DC_TA_1 = 59,     // ��ʱ��CP56Time2a��˫����                          
	C_RC_TA_1 = 60,     // ��ʱ��CP56Time2a����������                        
	C_SE_TA_1 = 61,     // ��ʱ��CP56Time2a����������һ��ֵ              
	C_SE_TB_1 = 62,     // ��ʱ��CP56Time2a����������Ȼ�ֵ              
	C_SE_TC_1 = 63,     // ��ʱ��CP56Time2a���������̸�����              
	C_BO_TA_1 = 64,     // ��ʱ��CP56Time2a��32���ش�                        
	/*�ڿ��Ʒ����ϵͳ��Ϣ*/
	C_IC_NA_1 = 100,    // ���ٻ�����                                        
	C_CI_NA_1 = 101,    // ���������ٻ�����                                  
	C_RD_NA_1 = 102,    // ������                                            
	C_CS_NA_1 = 103,    // ʱ��ͬ������                                      
	C_TS_NA_1 = 104,    // ��������                                          
	C_RP_NA_1 = 105,    // ��λ��������                                      
	C_CD_NA_1 = 106,    // �ռ�������ʱ                                      
	/*���Ʒ���Ĳ���*/
	P_ME_NA_1 = 110,    // ����ֵ��������һ��ֵ                              
	P_ME_NB_1 = 111,    // ����ֵ��������Ȼ�ֵ                              
	P_ME_NC_1 = 112,    // ����ֵ�������̸�����                              
	P_AC_NA_1 = 113,    // ��������                                          
	/*�ļ�����*/
	F_FR_NA_1 = 120,    // �ļ���׼����                                      
	F_SR_NA_1 = 121,    // ����׼����                                        
	F_SC_NA_1 = 122,    // �ٻ�Ŀ¼��ѡ���ļ����ٻ��ļ����ٻ���              
	F_LS_NA_1 = 123,    // ���Ľڣ����Ķ�                                
	F_AF_NA_1 = 124,    // ȷ���ļ���ȷ�Ͻ�                                  
	F_SG_NA_1 = 125,    // ��                                                
	F_DR_TA_1 = 126,    // Ŀ¼                                              

	/*************************************
	����ԭ��COT����
	**************************************/
	COT_NOUSED = 0x00,   // δʹ��                                            
	COT_PERCYC = 0x01,   // ���ڡ�ѭ��                                        
	COT_BACK = 0x02,   // ����ɨ��                                          
	COT_SPONT = 0x03,   // ͻ�����Է���                                      
	COT_INIT = 0x04,   // ��ʼ�����                                        
	COT_REQ = 0x05,   // ���������                                      
	COT_ACT = 0x06,   // ����                                              
	COT_ACTCON = 0x07,   // ����ȷ��                                          
	COT_DEACT = 0x08,   // ֹͣ����                                          
	COT_DEACTCON = 0x09,   // ֹͣ����ȷ��                                      
	COT_ACTTERM = 0x0a,   // ������ֹ                                          
	COT_RETREM = 0x0b,   // Զ����������ķ�����Ϣ                            
	COT_RETLOC = 0x0c,   // ������������ķ�����Ϣ                            
	COT_FILE = 0x0d,   // �ļ�����                                          
	COT_INTROGEN = 0x14,   // ��Ӧվ�ٻ�                                        
	COT_INTRO1 = 0x15,   //��Ӧ��1���ٻ�                                      
	COT_INTRO2 = 0x16,   //��Ӧ��2���ٻ�                                      
	COT_INTRO3 = 0x17,   //��Ӧ��3���ٻ�                                      
	COT_INTRO4 = 0x18,   //��Ӧ��4���ٻ�                                      
	COT_INTRO5 = 0x19,   //��Ӧ��5���ٻ�                                      
	COT_INTRO6 = 0x1a,   //��Ӧ��6���ٻ�                                      
	COT_INTRO7 = 0x1b,   //��Ӧ��7���ٻ�                                      
	COT_INTRO8 = 0x1c,   //��Ӧ��8���ٻ�                                      
	COT_INTRO9 = 0x1d,   //��Ӧ��9���ٻ�                                      
	COT_INTRO10 = 0x1e,   //��Ӧ��10���ٻ�                                     
	COT_INTRO11 = 0x1f,   //��Ӧ��11���ٻ�                                     
	COT_INTRO12 = 0x20,   //��Ӧ��12���ٻ�                                     
	COT_INTRO13 = 0x21,   //��Ӧ��13���ٻ�                                     
	COT_INTRO14 = 0x22,   //��Ӧ��14���ٻ�                                     
	COT_INTRO15 = 0x23,   //��Ӧ��15���ٻ�                                     
	COT_INTRO16 = 0x24,   //��Ӧ��16���ٻ�                                     
	COT_REQCOGEN = 0x25,   // ��Ӧ���������ٻ�                                  
	COT_REQCO1 = 0x26,   // ��Ӧ��1��������ٻ�                               
	COT_REQCO2 = 0x27,   // ��Ӧ��2��������ٻ�                               
	COT_REQCO3 = 0x28,   // ��Ӧ��3��������ٻ�                               
	COT_REQCO4 = 0x29,   // ��Ӧ��4��������ٻ�                               

	COT_UNKNOW_TYPE = 0x2c,   //δ֪�����ͱ�ʾ                                     
	COT_UNKNOW_COT = 0x2d,   //δ֪�Ĵ���ԭ��                                     
	COT_UNKNOW_ADDR = 0x2e,   //δ֪��Ӧ�÷������ݵ�Ԫ��ַ                         
	COT_UNKNOW_INF = 0x2f,   //δ֪����Ϣ������ַ                               
};


#pragma pack(push, 1)

//Ʒ��λ������
struct QDS
{
	unsigned char OV : 1;    //0: δ��� 1�����
	unsigned char RES : 3;
	unsigned char BL : 1;     //0: δ������ 1��������
	unsigned char SB : 1;     //0: δ��ȡ�� 1����ȡ��
	unsigned char NT : 1;     //0: ��ǰֵ   1���ǵ�ǰֵ
	unsigned char IV : 1;     //0: ��Ч     1����Ч
};

//�̵籣���豸�¼���Ʒ��������
struct QDP
{
	unsigned char RES : 3;    //����
	unsigned char EI : 1;     //0������ʱ����Ч 1������ʱ����Ч
	unsigned char BL : 1;     //0: δ������ 1��������
	unsigned char SB : 1;     //0: δ��ȡ�� 1����ȡ��
	unsigned char NT : 1;     //0: ��ǰֵ   1���ǵ�ǰֵ
	unsigned char IV : 1;     //0: ��Ч     1����Ч
};

//��Ʒ�������ʵĵ�����Ϣ
struct SIQ
{
	unsigned char SPI : 1;    //ֵ
	unsigned char RES : 3;    //����
	unsigned char BL : 1;     //0: δ������ 1��������
	unsigned char SB : 1;     //0: δ��ȡ�� 1����ȡ��
	unsigned char NT : 1;     //0: ��ǰֵ   1���ǵ�ǰֵ
	unsigned char IV : 1;     //0: ��Ч     1����Ч
};

//��Ʒ�������ʵ�˫����Ϣ
struct DIQ
{
	unsigned char DPI : 2;    //ֵ
	unsigned char RES : 2;    //����
	unsigned char BL : 1;     //0: δ������ 1��������
	unsigned char SB : 1;     //0: δ��ȡ�� 1����ȡ��
	unsigned char NT : 1;     //0: ��ǰֵ   1���ǵ�ǰֵ
	unsigned char IV : 1;     //0: ��Ч     1����Ч
};

//��˲��״ָ̬ʾ��ֵ
struct VTI
{
	unsigned short value : 15;     //ֵ
	unsigned short BS : 1;     //˲��״̬ 0:δ��˲��״̬ 1:����˲��״̬
};

//�����Ƽ���������
struct BCR_PARAM
{
	unsigned char  SQ : 5;     //˳���
	unsigned char  CY : 1;     //0:δ��� 1:���
	unsigned char  CA : 1;     //0:δ������ 1:������
	unsigned char  IV : 1;     //0:��Ч 1:��Ч
};
struct BCR
{
	unsigned int number;     //����������
	BCR_PARAM param;
};

//�����޶���
struct QOC
{
	unsigned char QU : 5;  //0:������Ķ��� 1:���������ʱ�� 2:���������ʱ�� 3:�������
	unsigned char SE : 1;  //0:ִ�� 1:ѡ��
};

//������
struct SCO
{
	unsigned char SCS : 1;   //ֵ
	unsigned char RES : 1;   //����
	unsigned char QU : 5;   //����
	unsigned char SelectOrExcute : 1; //0:ִ�� 1:ѡ��
};

//˫����
struct DCO
{
	unsigned char DCS : 2;   //0:������ 1:�� 2:�� 3:������
	unsigned char QU : 5;   //����
	unsigned char SelectOrExcute : 1; //0:ִ�� 1:ѡ��
};

struct Q0S//Q��S��QOS��windows�ڲ�����
{
	unsigned char QL : 7;
	unsigned char SelectOrExcute : 1; //0:ִ�� 1:ѡ��
};


//����������
struct RCO
{
	unsigned char SCS : 2;   //0:������ 1:��һ�� 2:��һ�� 3:������
	struct QOC qoc; //�����޶���
};

//����ԭ��
struct REASON
{
	unsigned char cause : 6;   //����ԭ��
	unsigned char PN : 1;      //0:�϶�ȷ�� 1:��ȷ��
	unsigned char T : 1;       //0:δ���� 1:����
};



//������λλ�������ʱ��
struct CP16Time2a
{
	unsigned char msL;     //�����
	unsigned char msH;     //�����
};


enum
{
	MAX_ASDU_SIZE = 249,
};

//////////////////////////////////////////////////////////////////////////
//M_SP_NA_1:����ң��(��ɢ)
//////////////////////////////////////////////////////////////////////////
class ASDU1_SQ0 : public ASDU_BASE
{
private:
	class ASDU1_SQ0_DATA //��ɢ���ź���
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
//M_SP_NA_1:����ң��(����)
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
//M_SP_TA_1:��ʱ�굥��ң��(��ɢ)
//////////////////////////////////////////////////////////////////////////
class ASDU2_SQ0 : public ASDU_BASE
{
private:
	class ASDU2_SQ0_DATA //��ɢ���ź���
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
//M_DP_NA_1:˫��ң��(��ɢ)
//////////////////////////////////////////////////////////////////////////
class ASDU3_SQ0 : public ASDU_BASE
{
	class ASDU3_SQ0_DATA //��ɢ���ź���
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
//M_DP_NA_1:˫��ң��(����)
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
//M_DP_TA_1:��ʱ��˫��ң��(��ɢ)
//////////////////////////////////////////////////////////////////////////
class ASDU4_SQ0 : public ASDU_BASE
{
private:
	class ASDU4_SQ0_DATA //��ɢ���ź���
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
//M_ST_NA_1:����ʱ��Ĳ�λ����Ϣ(��ɢ)
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
//M_ST_NA_1:����ʱ��Ĳ�λ����Ϣ(����)
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
//M_ST_TA_1:��ʱ��Ĳ�λ����Ϣ(��ɢ)
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
//M_BO_TA_1:��ʱ���32���ش�(��ɢ)
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
//M_ME_NA_1:��һ������ֵ(��ɢ)
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
//M_ME_NA_1:��һ������ֵ(����)
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
//M_ME_TA_1:����ֵ����ʱ��Ĺ�һ��ֵ(��ɢ)
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
//M_ME_NB_1:��Ȼ�����ֵ(��ɢ)
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
//M_ME_NB_1:��Ȼ�����ֵ(����)
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
//M_ME_TB_1:����ֵ����ʱ��ı�Ȼ�ֵ(��ɢ)
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
//M_ME_NC_1:�̸���������ֵ(��ɢ)
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
//M_ME_NC_1:�̸���������ֵ(����)
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
//M_ME_TC_1:����ֵ����ʱ��Ķ̸�����(��ɢ)
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
//M_IT_NA_1:�ۼ���(��ɢ)
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
//M_IT_NA_1:�ۼ���(����)
//////////////////////////////////////////////////////////////////////////
class ASDU15_SQ1 : public ASDU_BASE
{
	class ASDU15_SQ1_DATA //�������ۼ���
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
//M_IT_TA_1:��ʱ����ۼ���(��ɢ)
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
//M_PS_NA_1:����λ����ĳ��鵥����Ϣ(��ɢ)
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
//M_PS_NA_1:����λ����ĳ��鵥����Ϣ(����)
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
//M_ME_ND_1:����ֵ������Ʒ�������ʵĹ�һ��ֵ(��ɢ)
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
//M_ME_ND_1:����ֵ������Ʒ�������ʵĹ�һ��ֵ(����)
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

//M_SP_TB_1:����ʱ��ĵ�����Ϣ(��ɢ)
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
//M_DP_TB_1:����ʱ���˫����Ϣ(��ɢ)
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
//M_ST_TB_1:��ʱ��Ĳ�λ����Ϣ(��ɢ)
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
//M_ME_TD_1:����ֵ����ʱ��CP56Time2a�Ĺ�һ��ֵ(��ɢ)
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
//M_ME_TE_1:����ֵ����ʱ��CP56Time2a�ı�Ȼ�ֵ(��ɢ)
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
//M_ME_TF_1:����ֵ����ʱ��CP56Time2a�Ķ̸�����(��ɢ)
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
//M_IT_TB_1:��ʱ��CP56Time2a���ۼ���(��ɢ)
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
//C_SC_NA_1:��������(ң�أ��طֿغ�)
//////////////////////////////////////////////////////////////////////////
class ASDU45 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	SCO  m_sco;
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//C_DC_NA_1:˫������
//////////////////////////////////////////////////////////////////////////
class ASDU46 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	DCO  m_dco;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//C_SE_NA_1:��һ���������(ң�أ���ģ����)
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
//C_SE_NB_1:��Ȼ��������(ң�أ���ģ����)
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
//C_SE_NC_1:�������������(ң�أ���ģ����)
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
//C_IC_NA_1:���ٻ�����
//////////////////////////////////////////////////////////////////////////
class ASDU100 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	unsigned char m_qoi;
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_CI_NA_1:���������ٻ�����
//////////////////////////////////////////////////////////////////////////
class ASDU101 : public ASDU_BASE
{
public:
	INFOADDR m_infoaddr;
	unsigned char m_qcc;
};
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//C_CS_NA_1:ʱ��ͬ������
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
