/*! @file
<PRE>
********************************************************************************
ģ����       :  �ڲ������������ݽṹ����
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author  LiJin
*  @version 1.0
*  @date    2015.09.1
*/
#ifndef NETBUS_DEF_H
#define NETBUS_DEF_H 

#include "../datatypes.h"

#include <string>

extern "C"
{ 

#define  NAME_SIZE  (512+4)  //!TODO ����ͳһ�ŵ�һ�������ļ�

#define GET_MSG_TIMEOUT		10   // ��ȡ��Ϣ��ʱʱ��(��)

	//�˿ں�
#define SERVER_PORT		95271 // ���������˿ں�
#define FES_PORT		95275 // ǰ�ö˿ں�
#define CLIENT_PORT		95277 // �ͻ��˶˿ں�

#define NETBUS_INLINE inline	// ����

#define START_WAIT_TIME			3 // ���������������ȴ�ʱ��

#define HEARTBEAT_TIMEOUT		5 // �����л���ʱʱ��

#define NODE_ALIVE_TIME			5 // �ڵ��ʱʱ��

#define HOST_NAME_SIZE			20 // ������������������

#define CHECK_HOST_SLAVE		1 // �������
#define CHECK_NODE_NET_STATE	2 // �����������ڵ�״̬

#define HEARTBEAT_TIMER 1*1000


	// ����������
	const unsigned char  ORIGIN_FLAG = 0x5a;    // ��ʾ��ʼ

	enum : unsigned int
	{
		MAIN  = 1,		 // ��
		SLAVE = 2,		 // ��
	};

	// ��������
	enum :int
	{
		NET_A = 0, // A��
		NET_B = 1, // B��
	};

	// ��Ϣ����(����Խ�����ȼ�Խ��)
	enum : int32u
	{
		GENERAL_MSG = 1,		// ��ͨ��Ϣ����,��������Ϣȫ������.

		URGENT_MSG = 100,		// ������Ϣ����,�����ڱ仯����.

		MATTER_MSG = 1000,		// ������Ϣ����,���������.

		CONTROL_MSG = 10000		// ������Ϣ����,����ң�ء�ң��.
	};

 	/*! \struct  NODE_CONFIG
	*   \brief ϵͳ�ڵ㶨�� */
	struct NODE_CONFIG
	{
		int32u OccNo;							//!  �ڵ������ݿ��е�ID
		int32u SlaveOccNo;
		int32u NodeType;						//!  �ڵ�����
		int32u NetAIPAddr;                      //! IP address of net1, filled by cfg. 
		int32u NetBIPAddr;                      //! IP address of net2, filled by cfg. 

		char chIP[2][NAME_SIZE + 1];		    //! AB��IP

		int32u NetAIpV6Addr[4];                 //! IPV6 ��A����ַ
		int32u NetBIpV6Addr[4];                 //! IPV6 ��B����ַ

	    int8u IsCheckTime;						//�� �Ƿ��ʱ
		int8u IsDefaultHost;					//�� �Ƿ�Ĭ��Ϊ��
		
		char HostName[NAME_SIZE + 1];		    //�� ��������
		char TagName[NAME_SIZE + 1];			//�� ����
		char GrpName[NAME_SIZE + 1];            //!  ����
		// ʵʱֵ
		int32u HostSlave;						//�� ����״̬
		int8u CardStatus[2];					//�� AB��״̬

		unsigned long long SwitchTime;			//�� ׼���л���׼ʱ��
		unsigned long long TimeNetA;			//�� A����׼ʱ��
		unsigned long long TimeNetB;			//�� B����׼ʱ��
	} ;

	typedef struct NODE_CONFIG CNodeConfig;
//	using CNodeConfig = struct  NODE_CONFIG;

	/*! \struct  NET_CONFIG
	*   \brief ����ڵ㶨�� */
	struct NET_CONFIG
	{
		int32u MyNodeOccNo;                 //! ���ڵ�����к�
		bool IsDoubleNet;					//! �Ƿ�˫��
		bool bCheckTime;					//! �Ƿ��ʱ
		int32u CheckTimeNodeOccNo;	        //! ��ʱ�ڵ����к�
		int32u NodeCount;					//! �ڵ���
		CNodeConfig* pNodeConfig;			//! �ڵ��ָ��
	} ;
//	typedef struct NET_CONFIG CNetConfig;
//	using CNetConfig = struct NET_CONFIG;

	const int32u MAX_EMSG_L = (399 * 1024);

	enum EMSG_PATH
	{
		TO_SERVER = 1,      //!< ���͵�������
		TO_FES = 2,         //!< ���͵�ǰ��
		TO_MASTER_FES = 3,
		TO_CLIENT = 4,
		TO_PEER = 5,        //!< ���͵���飬�Բ�����ڵ�
		TO_SELF = 6,		
	};
	/*! \struct  EMSG_BUF
	*   \brief ��̫������ */
	struct EMSG_BUF_HEAD
	{
		int32u  MsgPath;        //! ��Ϣ·��

		int32u  MsgType;        //! ��Ϣ����
		int32u  SrcOccNo;       //! Դ�ڵ��OccNo
		int32u  DestOccNo;		//! Ŀ�ĵ�OccNo
		int32u  MsgDataSize;    //! MsgData �е���Ч����
		int32u  FuncCode;
		int32u  SeqNum;         //! ��ţ������ۼ�	
	};

	/*! \struct  EMSG_BUF
	*   \brief ��̫������ */
	struct EMSG_BUF
	{
		int32u  MsgPath;        //! ��Ϣ·��

		int32u  MsgType;        //! ��Ϣ����
		int32u  SrcOccNo;       //! Դ�ڵ��OccNo
		int32u  DestOccNo;		//! Ŀ�ĵ�OccNo
		int32u  MsgDataSize;    //! MsgData �е���Ч����
		int32u  FuncCode;
		int32u  SeqNum;         //! ��ţ������ۼ�

		//void* data;
		char BuffData[MAX_EMSG_L];
		//unsigned char MsgData[MAX_EMSG_L]; 
	};

	const int32u EMSG_BUF_HEAD_SIZE = sizeof(EMSG_BUF_HEAD);

	const int32u EMSG_BUF_SIZE = sizeof(EMSG_BUF);

	//���͹�����
	enum class FunCode:int
	{
		//����������Ϣ
		 M_AI_NA_1 = 1,          //AI��Ϣ
		 M_AI_TA_1 = 2,          //��ʱ��AI��Ϣ
		 M_AI_ND_1 = 3,          //����Ʒ�ʵ�AI
		 M_DI_NA_1 = 4,          //DI��Ϣ
		 M_DI_TA_1 = 5,          //��ʱ��DI��Ϣ
		 M_DI_ND_1 = 6,          //����Ʒ�ʵ�DI
		
		 //�仯������Ϣ
		 M_AI_NC_1 = 7,          //�����仯����AI
		 M_AI_SC_1 = 8,          //�쳣�仯����AI���������ݣ��澯���ݣ�
		 M_DI_NC_1 = 9,          //�����仯����DI
		 M_DI_SC_1 = 10,         //�쳣�仯����DI���������ݣ��澯���ݣ�

		 //ͳ����������
		 M_AI_SD_1 = 11,	     //AIͳ����������
		 M_DI_SD_1 = 12,         //DIͳ����������

		 //�澯��������
		 M_AI_AM_1 = 13,         //AI�澯��������
		 M_DI_AM_1 = 14,         //DI�澯��������

		 //�¼�����
		 M_AI_AE_1 = 15,         //AI�¼�����
		 M_DI_AE_1 = 16,         //DI�¼�����
	};

	//����ԭ��
	enum  MSG_TYPE
	{
		COT_PERCYC = 1 ,        //������������
		COT_SPONT  = 2 ,        //����ͻ������
		COT_ALARM  = 3 ,        //�澯��Ϣ����
		COT_EVENT  = 4 ,        //�¼���Ϣ����
		COT_SUB    = 5 ,        //������Ϣ����

		COT_SETVAL = 6 ,        //��ֵ�����·�
		COT_REDATA = 7 ,        //��������
	};

	

	/************************************** ���� **************************************/
	enum
	{
		HEARTBEAT_CODE = 0x6f,	// ����������
	};

	static const unsigned int g_nSize = 9 + HOST_NAME_SIZE + 1 + 2;

	/*#�鲥���ಥ����ַ
	#224.0.0.0 -- 224.0.0.255���ر�������֪��Э��ʹ�á�����224.0.0.1�Ǳ��������������ܣ�224.0.0.2�Ǳ�������·�������ա�
	#224.0.1.0 -- 238.255.255.255Ԥ���鲥��ַ���ಥ��ַӦ�Ӵ˷�Χ��ѡ��
	#239.0.0.0 -- 239.255.255.255˽���鲥��ַ��
	#232.0.0.0 -- 232.255.255.255�ض�Դ�鲥��*/
	#define MULTICAST_ADDR		"234.5.6.8"

	// #�㲥�˿�
	#define DGRAM_PORT_A			59527	 // A�������˿�
	#define DGRAM_PORT_B			69527	 // B�������˿�
}



/** @}*/
#endif // NETBUS_DEF_H