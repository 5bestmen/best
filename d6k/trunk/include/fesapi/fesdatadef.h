/*! @file fesdatadef.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fesdatadef.h
�ļ�ʵ�ֹ��� :  ǰ�����ݶ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     ǰ�����ݶ���
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
*   \brief ǰ�û�����Ϣ */
struct FesInfo
{
	int FesId;                                             //!ǰ��ID
	char FesDesc[MAX_NAME_LENGTH + STRING_PAD];           //!ǰ������
	char FesTagName[MAX_NAME_LENGTH + STRING_PAD];         //!ǰ��TagName
	char FesVersion[MAX_NAME_LENGTH + STRING_PAD];         //!ǰ�ð汾
	char FesExt[MAX_NAME_LENGTH + STRING_PAD];             //!ǰ����չ��Ϣ
	char FesGrpName[MAX_NAME_LENGTH + STRING_PAD];         //!ǰ������
};

/*! \struct  StringUnit
*   \brief �ַ���������Ԫ */
struct StringUnit
{
	int32u OccNo;
	char Desc[MAX_NAME_LENGTH + STRING_PAD];
	bool IsTranslate;
	char Language[MAX_NAME_LENGTH + STRING_PAD];
};

/*! \struct  NODE
*   \brief ϵͳ�ڵ㶨�� */
struct NODE
{
	int32u  OccNo;                //! �ؼ��� Ψһ Id ��1��ʼ
	int32u  GrpNo;                //! ���
	int32u  NameOccNo;            //! ֻ���ַ������ڵ�ID���
	int32u  SlaveOccNo;           //! ��������ӻ������ס������OccNo

	int32u  Prority;               //! ���ȼ� Ϊ����N-1���෽ʽ��չ׼��

	/* ʵʱ */
	int8u IsDefined;      /* YES : is defined, NO: spare */
	int8u ScanEnable;     /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;           /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;        /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */


	int8u RedState;	      /* 0 ��ͨ�������ţ��� 1 ������ */
	int8u NodeType;       /* �ڵ����� */
	int32u State;         /* ����״̬  2-�� 7���� */
	int16u  Net1State;      /* ���� 1 ;�Ͽ� 0��ȱʡֵ�� */
	int16u  Net2State;      /* ���� 1 ;�Ͽ� 0��ȱʡֵ��*/

	int32u PointCount;

	/* ���� */
	int32u NetAIPAddr;     /* IP address of net1, filled by cfg. */
	int32u NetBIPAddr;     /* IP address of net2, filled by cfg. */

	int32u NetAIpV6Addr[4];  //! IPV6 ��A����ַ
	int32u NetBIpV6Addr[4];  //! IPV6 ��B����ַ

	int8s TagName[MAX_NAME_LENGTH + STRING_PAD];   //! group name of nodes   == TagName
	int8s HostName[MAX_NAME_LENGTH + STRING_PAD];     /* name of a node */
	int8s GroupName[MAX_NAME_LENGTH + STRING_PAD];   //! ����������ǵ��������ֶ�Ϊ�գ���������������ֶ�һ����
};

/*! \union  COMM_PARA
*   \brief  ͨ������ */
union COMM_PARA
{
	struct SERIAL_PORT
	{
		int32u  MasterComPort;                     /*���ں�  1��32��Ӧ��COM1��COM32*/
		int32u  SlaveComPort;                      /*���ô��ں�*/
		int32u  BaudRate;                          /*�����ʲ����ʣ�110��300��600��1200��2400��4800��9600��14400��19200��28800��38400��56000��57600��115200��128000��256000��500000*/

		int8u  Parity;                            /*У�飺0����У��  1��żУ�� 2����У��*/
		int8u  DataBit;                           /*����λ��4��5��6��7��8*/
		int8u  StopBit;                           /*ֹͣλ��0��1λ  1��1.5λ  2��2λ*/
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
*   \brief  ͨ������ */
struct CHANNEL
{
	int32u  OccNo;                //! �ؼ��� Ψһ Id ��1��ʼ
	int32u  NameOccNo;            //! ֻ���ַ������ڵ�ID���

	int8u   IsDefined;                         /* YES : is defined, NO: spare */
	int8u   ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u   Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u   Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u   PeerQuality;                       /* ���ö˿ڵ�״̬ */
	int8u   Heartbeat;                         /* ���� */
	int8u   PrintOn;                           //���Կ��أ�1�����ͨ��ͨѶ���ģ�0�������
	int8u   WorkNet;                           /*��ǰ��������*/

	COMM_PARA CommPara;

	int32u State;
	int8u  Prority;
	int8u  SetTimeAttr;                       /* �˿ڶ�ʱ���� 0/1/2 ʱ���޹�/ʱ��Դ/��ʱ */

	// ���غ�����������Ϣ
	int32u  DeviceCount;  //! ��ͨ����װ�õ�����

	int32u  NodeOccNo;  //! �������ĸ��ڵ�

	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //ͨ������
	char    DriverName[MAX_NAME_LENGTH + STRING_PAD]; //��������

	int8u	DriverType;	  //��������

	int8u	StateCore;	//ͨ���ں�״̬
	int8u	StateDriver; //ͨ������״̬

	int32s  MailBoxID;  //! ͨ��������ID
};

/*! \struct  DEVICE
*   \brief  װ�ö���
	\ Ԥ�����Ƕ�ֵ����
*/
struct DEVICE
{
	int32u  OccNo;

	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���
	int32u  NodeOccNo;     //! �������ĸ��ڵ�
	int32u  ChannelOccNo;  //! �������ĸ�ͨ��

	int32u  ModelOccNo;     //! װ������ʱ�õ�ģ��ID �ͺű�ʶ:DA-R771��772??

	int8u  Type;           //! װ������   1���������  2-����  3����� 4������� 5-����
	int8u  Pad1;
	int16u Pad2;

	int32u Heartbeat;      //! �����ź� ������ά��

	char   Address[MAX_ADDRESS_LENGTH + STRING_PAD];   //! װ�õ�ַ
	char   Extention[MAX_ADDRESS_LENGTH + STRING_PAD]; //! ��չ����
	// ʵʱֵ��
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                        /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
};

/*! \struct  DIN
*   \brief  ����������
*/
struct DIN
{
	int32u  OccNo;
	int32u  BlockNo;       //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���

	int8u   NeedTranmitFlag;  //! �Ƿ���Ҫת����־����ֵ���ڽ����������ã��ɳ����жϣ����true������ֵ�ı仯����ǰ��Զ��
	int16u  Pad2;
	int8u   IsSpare;       //! �Ƿ��õ㣬������SCADA; �������������ܴ������õı��õ�

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //���ַ

	int8u InitValEnable;    //! �Ƿ����ó�ʼֵ�ͳ�ʼƷ��
	int8u InitValue;
	int8u InitQua;

	/*˫�ӵ㴦����*/
	int32u SlaveOccNo;					  /*˫�ӵ��λ�ڵ�ID*/
	int8u  DDIFilter; 					  /*˫�ӵ�λ�ò���Ӧ�˲�����*/
	int8u  DDIFlag;   					  /*˫�ӵ�λ�ò���Ӧ��־*/
	int8u  DDIInUse;  					  /*˫�ӵ㴦��*/

	/*--------------------------------------ʵʱ��Ϣ------------------------------------------*/
	int32u State;

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/

	int8u Value;            			    /* DI's Output: 0/1                  */
	int8u NegValue;         		        /* DI's complementery of Output: 1/0 */
	int8u RawValue;                     	//ԭʼֵ   /* Data collects from inst. , produces */  /* Output/NotOutput based on SiglaType */


	int8u  CtrlByte;                          /* ������ */
	int8u  IsSOE;                           /* 0: Non-SOE, 1: SOE **/

	int8u StartCtrl;                          /* filled by cfg. to initilized a sequence*/
	int8u SignalType;                         /* 0 : NORMAL_OPEN; 1: NORMAL_CLOSE  */


	// ͳ�Ƽ����������
	int32u  NodeOccNo;     //! �����ڵ�
	int32u  ChannelOccNo;  //! ����ͨ��
	int32u  DeviceOccNo;   //! ����װ��


	int32u  AlarmOccNo;    //! �����ĸ澯ID
	int32u  DataSource;    //! ������Դ ����̬���ɣ�

	int32u  ExpressOccNo;  //! ������ʶ


	int32u  Desc0_OccNo;
	int32u  Desc1_OccNo;

	int32u  PinLabelOccNo;   //! ���Ӻ�����(�ַ���)

	TIMEPAK LastUpdateTime;  //! ���һ�θ���ʱ��
};

/*! \struct  TRANSFORM_LINEAR
*   \brief  ����ת������ */
struct TRANSFORM_LINEAR
{
	int32u  OccNo;
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���

	int8u EnableInverse; //! �Ƿ񷴱�����ת��
	int8u Pad;

	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	int8u EnableFactor;       //! ���ü���ϵ��
	int8u InverseFactor;      //! ���÷���ϵ��

	fp64  Gain;               //! ����ϵ��
	fp64  Offset;             //! ƫ��ϵ��
};

/*! \struct  TRANSFORM_NONLINEAR
*   \brief  ������ת������ */
struct TRANSFORM_NONLINEAR
{
	struct LINEAR_ITEM
	{
		fp64  RawValue;     //! ԭʼֵ
		fp64  ScaleValue;   //! ת�����ֵ
	};

	int32u  OccNo;
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���
	int32u  DataCount;    //! ������������Ч��Ŀ
	LINEAR_ITEM NonLinearData[MAX_NON_LINEAR_NUM];
};

/*! \struct  ALARM_COLOR
*   \brief  ������ɫ���� */
struct ALARM_COLOR
{
	int32u  BackColor;
	int32u  TextColor;
	int32u  BlinkBackColor;
	int32u  BlinkTextColor;
};

/*! \struct  AIN_ALARM
*   \brief  ģ�����ı������� */
struct  AIN_ALARM
{
	int32u  OccNo;
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���

	int8u   ScanEnable;     //! �Ƿ�����
	int8u   OnQualityGood;  //! ��ֵƷ�ʺòŸ澯
	int8u   DeadBandType;   //! ���������ͣ�0�����ֵ��1�Ǿ���ֵ

	fp64    DeadBand;        //! ����ֵ
	int32u  DescriptionOccNo;  //! ����
	int32u  LimitCount;      //! ��������ֵ�澯����
};
/*! \struct  DIN_ALARM
*   \brief  �������ı������� */
struct  DIN_ALARM
{
	int32u  OccNo;
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���

	int8u   ScanEnable;     //! �Ƿ�����
	int8u   OnQualityGood;  //! ��ֵƷ�ʺòŸ澯
	int32u  DescriptionOccNo;  //! ����
	int32u  LimitCount;      //! ��������ֵ�澯����
};
/*! \struct  AIN_ALARM_LIMIT
*   \brief  ģ����������ֵ */
struct  AIN_ALARM_LIMIT
{
	int32u  OccNo;
	int32u  BlockNo;      //! С���к� (LSN) ���ڸ澯�ڵ�˳��ţ�ֵ��0
	int32u  AlarmOccNo;   //! �����ĸ���������
	int32u  NameOccNo;    //! ֻ���ַ������ڵ�ID���

	int8u   ScanEnable;     //! �Ƿ�����
	int8u   Condition;      //! ����
	int8u   Category;       //! ���ࣺ�¹ʡ����ϡ���ͨ
	int32u  Priority;       //! ���ȼ�	

	int32u  DelayAlarm;     //! ��ʱ����λΪ��
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

	int32u  DescriptionOccNo;  //! ����


	fp64    LimitValue;       //! ������ֵ
	fp64    LowValue;         //! �ڱ�������Ϊ����xx��xx֮��ʱ����Ч
};

struct  DIN_ALARM_LIMIT
{
	int32u  OccNo;
	int32u  BlockNo;       //! С���к� (LSN) ���ڸ澯�ڵ�˳��ţ�ֵ��0
	int32u  AlarmOccNo;  //! �����ĸ���������
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���

	int8u   ScanEnable;     //! �Ƿ�����
	int8u   Condition;      //! ����
	int8u   Category;       //! ���ࣺ�¹ʡ����ϡ���ͨ
	int32u  Priority;       //! ���ȼ�

	int32u  DelayAlarm;     //! ��ʱ����λΪ��
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

	int32u  DescriptionOccNo;  //! ����
};
/*! \struct  AIN
*   \brief  ģ���� ���� */
struct AIN
{
	int32u  OccNo;
	int32u  BlockNo;       //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���

	int8u   IsSpare;       //! �Ƿ��õ㣬������SCADA; �������������ܴ������õı��õ�
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //���ַ

	/*--------------------------------------ʵʱ��Ϣ------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/
											//�˹������㣬����װ�ø���ֵ

	fp64  SignalValue;            /* Output referred signal, assigned by drive */
	fp64  RawValue;	              //�� ԭʼֵ
	fp64  Value;                  //�� ����ת��ֵ
	fp64  OldValue;
	//ʵʱֵ
	int32u   AlarmState[MAX_AIN_ALARMLIMIT_BITCOUNT];              //!  ����״̬ ��λ����
	int32u   AlarmFlag[MAX_AIN_ALARMLIMIT_BITCOUNT];               //!  ������ʶ���������α���

	fp64    InitValue;                         /* ���̳�ʼֵ */
	int32u  DataSource;                        //! ������Դ ����̬���ɣ�

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*������������ֵ����ƫ��ϵ�� �ٷ���*/
	fp64    LowQua;                             /*������������ֵ����ƫ��ϵ�� �ٷ���*/

	// ת����
	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	// ͳ�Ƽ����������
	int32u  NodeOccNo;      //! �����ڵ�
	int32u  ChannelOccNo;   //! ����ͨ��
	int32u  DeviceOccNo;    //! ����ͨ��	

	int32u  AlarmOccNo;     //! �����ĸ澯��һ���澯�ڲ��ж����ֵ�澯
	int32u  ExpressOccNo;   //! ������ʶ

	int8u   TransformType;  //! ת������(���ԣ�������)
	int32u  TransformOccNo; //! ת����ϵ��ʶ

	int32u  DescriptionOccNo;  //! �������
	int32u  PinLabelOccNo;  //! ���Ӻ�
	int32u  UnitNameOccNo;  //! ��λ����

	// ��ȱ ת����ϵ����Чֵ��Χ�ȵ�

	TIMEPAK LastUpdateTime;  //! ���һ�θ���ʱ��
};
/*! \struct  DOUT
*   \brief  ң�� ���� */
struct DOUT
{
	int32u  OccNo;
	int32u  BlockNo;       //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���

	int8u   IsSpare;       //! �Ƿ��õ㣬������SCADA; �������������ܴ������õı��õ�
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //! ���ַ

	/*--------------------------------------ʵʱ��Ϣ------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */

	int8u Output;                            /* DI's Output: 0/1                  */
	int8u RelayState;                        /* collected from modules specifying real output */
	int8u CtrlType;                          //0: Select  1:Sel Ok / Exec  2:Cancel
	int8u Filter;                            //�˲�
	int32u DataSource;                //! ������Դ ����̬���ɣ�

	int8u  IsSBO;                     //! SBO���ͣ�
	int8u  BlockSignalType;           //! �����ź�����
	int32u BlockingSignalOccNo;       //! �����źţ�������IO��Ҳ������������������Ǳ����Ǳ��ڵ�

	int32u  FeedbackOccNo;   //! �����ķ���ң���ź�
	int8u   FeedbackType;    //! ����0������1���϶�ң�سɹ���

	int32u BlockingProgramOccNo;      //! ���ڵ��ϱ�����������к� 
	int32u CheckOK;

	int32u StartProgramOccNo;         //! ����һ�γ��򣬶���ʵ��ң��

	// ͳ�Ƽ����������
	int32u  NodeOccNo;     //! �����ڵ�
	int32u  ChannelOccNo;  //! ����ͨ��
	int32u  DeviceOccNo;   //! ����ͨ��

	int32u  AlarmOccNo;    //! �����ĸ澯ID
	int32u  ExpressionOccNo;  //! ������ʶ

	int32u  PinLabelOccNo;   //! ���Ӻ�

	TIMEPAK LastUpdateTime;  //! ���һ�θ���ʱ��
};
/*! \struct  AOUT
*   \brief  ģ�� ���� */
struct AOUT
{
	int32u  OccNo;
	int32u  BlockNo;       //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;   //! ֻ���ַ������ڵ�ID���

	int8u   IsSpare;       //! �Ƿ��õ㣬������SCADA; �������������ܴ������õı��õ�
	int8u   Pad1;
	int16u  Pad2;

	char    Address[MAX_ADDRESS_LENGTH + STRING_PAD]; //! ���ַ
	/*--------------------------------------ʵʱ��Ϣ------------------------------------------*/
	int32u State;
	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */

	fp64    Output;                            /* AIN's output, caculated by drive or exp. */
	fp64    RawData;                           /* data collects from instrument  */
//	fp64         SignalValue;                       /* Output referred signal, assigned by drive */
	int32u  DataSource;     //! ������Դ ����̬���ɣ�

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*������������ֵ����ƫ��ϵ�� �ٷ���*/
	fp64    LowQua;                             /*������������ֵ����ƫ��ϵ�� �ٷ���*/

	fp64   MaxRaw;
	fp64   MinRaw;

	fp64   MaxScale;
	fp64   MinScale;

	// ͳ�Ƽ����������
	int32u  NodeOccNo;      //! �����ڵ�
	int32u  ChannelOccNo;   //! ����ͨ��
	int32u  DeviceOccNo;    //! ����ͨ��

	int32u  AlarmOccNo;     //! �����ĸ澯ID
	int32u  ExpressOccNo;   //! ������ʶ

	int8u   TransformType;    //! ת������(���ԣ�������)
	int32u  TransformOccNo;   //! ת����ϵ��ʶ

	int32u  PinLabelOccNo;     //! ���Ӻ�
	int32u  UnitNameOccNo;     //! ��λ����

	TIMEPAK LastUpdateTime;    //! ���һ�θ���ʱ��
};
/*! \struct  EXP
*   \brief  ���ʽ ���� */
struct EXP
{
	int32u  OccNo;
	int32u  BlockNo;       //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;     //! ֻ���ַ������ڵ�ID���

	int32u  VarID;                           /*���㷵�ز��ID(��̨DIN.ID)*/
	int32u  TableType;                      /*���㷵�ز����(��̨IDD_DIN...)*/
	int32u  MidCodeLen;                      /*����ű�����*/
	char*	MidCode;				           /*����ű�ִ����*/

};

struct TAGITEM
{
	int32u NodeOccNo;  //! �ڵ�
	int32u IddType;    //! ������� 
	int32u TagOccNo;   //! �������
	int32u FiledID;    //! �������
};
/*! \struct  VARDATA
*   \brief  �ڴ���� ���� */
struct VARDATA
{
	int32u  OccNo;
	int32u  BlockNo;        //! ���ڲ�����к� (LSN) ����װ���ڵ�˳��ţ����Բ�������ֵ��0
	int32u  NameOccNo;      //! ֻ���ַ������ڵ�ID���
	int32u  NodeOccNo;      //! �����ڵ�

	int32u  AlarmOccNo;     //! �����ĸ澯ID�����������IddType�жϣ�����ǿ��������������ģ�����ĸ澯������ǿ�������������������ĸ澯
	int32u  ExpressOccNo;   //! ������ʶ

	int8u   DataType;       //! �������ͣ�bool��byte��char...
	int8u   IddType;        //! DIN\AIN\AOUT\DOUT\  ���߿�

	int32u  SrcNodeOccNo;
	int32u  SrcOccNo;
	int8u   SrcIddType;
	int8u   IsRefTag;     //! �Ƿ�Ϊ���ñ���

	IO_VARIANT InitValue;  //! ��ʼֵ
	int8u      InitQua;    //! ��ʼƷ��
	int8u      IsSOE;                            /* 0: Non-SOE, 1: SOE **/

	/*--------------------------------------ʵʱ��Ϣ------------------------------------------*/
	int32u State;

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u ManSet;                            /* 0: Normal, 1: has been Manual set **/

	IO_VARIANT Value;            			    /* DI's Output: 0/1                  */
	IO_VARIANT RawValue;                     	//ԭʼֵ   /* Data collects from inst. , produces */  /* Output/NotOutput based on SiglaType */

	int8u  NegValue;         		        /* DI's complementery of Output: 1/0 */
	int8u  CtrlByte;                          /* ������ */
	
	int8u StartCtrl;                          /* filled by cfg. to initilized a sequence*/
	int8u SignalType;                         /* 0 : NORMAL_OPEN; 1: NORMAL_CLOSE  */

	int32u  DataSource;                       //! ������Դ ����̬���ɣ�

	int32u  Desc0OccNo;
	int32u  Desc1OccNo;

	fp64    RangeL;                             /* low end value of transmitter */
	fp64    RangeH;                             /* high end value of transmitter */
	fp64	HighQua;                            /*������������ֵ����ƫ��ϵ�� �ٷ���*/
	fp64    LowQua;                             /*������������ֵ����ƫ��ϵ�� �ٷ���*/

	fp64 MaxRaw;
	fp64 MinRaw;

	fp64 MaxScale;
	fp64 MinScale;

	TIMEPAK LastUpdateTime;  //! ���һ�θ���ʱ��
};

/*! \struct  FT_CHANNEL
*   \brief  ת��ͨ������ */
struct FT_CHANNEL
{
	int32u  OccNo;                //! �ؼ��� Ψһ Id ��1��ʼ
	int32u  NameOccNo;            //! ֻ���ַ������ڵ�ID���

	int8u IsDefined;                         /* YES : is defined, NO: spare */
	int8u ScanEnable;                           /* SCAN_OUT / SCAN_IN, DEFAULT_SCAN */
	int8u Init;                              /* UNINITED / INITED ,DEFAULT_INIT  */
	int8u Quality;                           /* QUALITY_OK /QUALITY_BAD, DEFAULT_QUA */
	int8u PeerQuality;                       /* ���ö˿ڵ�״̬ */
	int8u Heartbeat;                         /* ���� */
	int8u PrintOn;                           //���Կ��أ�1�����ͨ��ͨѶ���ģ�0�������
	int8u WorkNet;                           /*��ǰ��������*/

	COMM_PARA CommPara;

	int32u State;
	int8u Prority;

	// ���غ�����������Ϣ

	int32u  NodeOccNo;  //! �������ĸ��ڵ�

	char    ChannelName[MAX_NAME_LENGTH + STRING_PAD]; //ͨ������
	char    DriverName[MAX_NAME_LENGTH + STRING_PAD]; //��������

	int8u	DriverType;	  //��������

	int8u	StateCore;	//ͨ���ں�״̬
	int8u	StateDriver; //ͨ������״̬

	int32s  MailBoxID;  //! ͨ��������ID
};


/*! \struct  ATTDEF
*   \brief  �����������Զ��� */
struct ATTDEF
{
	int32u      Index;       //! ���
	int32u      ByteOffset;  //! �ṹ���ڵ�ƫ��
	int16u      DataType;   //! �������� 
	int16u      Att;        //! ���Ժ�

	int16u      ArraySize;  //! ���鳤��
	int16u      RW;         //! ��д���� 
	char        AttName[MAX_ATTNAME_LENGTH];
	char        Description[MAX_NAME_LENGTH];
};

enum MSG_CODE
{
	MSG_BASE = 0,       //!
	MSG_ALARM = 1,      //! ͨ�ø澯
	MSG_AIN_ALARM = 2,
	MSG_DIN_ALARM = 3,
	MSG_DIAG,   //! ����ϸ澯
	MSG_OPER,   //! �����澯
	MSG_SETVAL,        //! ��ֵ


};

enum
{
	MSG_LEN = 1024,
};
/*! \struct  BASE_MSG
*   \brief  ǰ���ڲ�����Ϣ���壬���ڽڵ��ڲ��ĸ���ģ�����Ϣ����
*   \details ÿ����Ϣ���1024�����Լ�mail.h�ж���
*/
struct BASE_MSG
{
	int16u MsgCode;   //! ��������������һ�����͵���Ϣ
	int16u Len;       //! ������Ϣ�ĳ��� <= 1024

	int8u  Data[MSG_LEN - sizeof(int16u) * 2];
};

/*! \struct  ALARM_MSG_H
*   \brief  ͨ�ø澯�¼�����  */
struct ALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u	EvtCode;      //! ������Ϣ����
	int32u  NodeOccNo;

	int32u	OccNo;

	int16u	AlarmFlag;         //���λΪ1��������,Ϊ0������
	int16u  AlarmState;        //�Կ�����������;��ģ����,
	IO_VARIANT	Value;
};
/*! \struct  ALARM_MSG
*   \brief  ͨ�ø澯�¼����� */
struct ALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u	EvtCode;      //! ������Ϣ����
	int32u  NodeOccNo;

	int32u	OccNo;

	int16u	AlarmFlag;         //���λΪ1��������,Ϊ0������
	int16u  AlarmState;        //�Կ�����������;��ģ����,
	IO_VARIANT	Value;

	int8u  ExtraData[MSG_LEN - sizeof(ALARM_MSG_H)];
};

/*! \struct  AINALARM_MSG_H
*   \brief  ģ�����澯�¼����� */
struct AINALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! ������Ϣ����
	int32u	OccNo;        //! ģ�������

	int32u  AlarmOccNo;     //! ģ�����澯�����к�
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! ���λΪ1��������,Ϊ0������
	int16u  AlarmState;    //! �Կ�����������;��ģ����,
	fp64	Value;         //! ��ǰ��ֵ	 
};
/*! \struct  AINALARM_MSG
*   \brief  ģ�����澯�¼����� */
struct AINALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! ������Ϣ����
	int32u	OccNo;        //! ģ�������

	int32u  AlarmOccNo;     //! ģ�����澯�����к�
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! ���λΪ1��������,Ϊ0������
	int16u  AlarmState;    //! �Կ�����������;��ģ����,
	fp64	Value;         //! ��ǰ��ֵ

	int8u  ExtraData[MSG_LEN - sizeof(AINALARM_MSG_H)];
};

/*! \struct  DINALARM_MSG_H
*   \brief  �������澯�¼����� */
struct DINALARM_MSG_H
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! ������Ϣ����
	int32u	OccNo;        //! ģ�������

	int32u  AlarmOccNo;     //! ģ�����澯�����к�
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! ���λΪ1��������,Ϊ0������
	int16u  AlarmState;    //! �Կ�����������;��ģ����,
	int8u	Value;         //! ��ǰ��ֵ
};
/*! \struct  DINALARM_MSG
*   \brief  �������澯�¼����� */
struct DINALARM_MSG
{
	int16u	MsgCode;
	int16u	Len;

	TIMEPAK Tm;

	int32u  NodeOccNo;
	int32u	EvtCode;      //! ������Ϣ����
	int32u	OccNo;        //! ģ�������

	int32u  AlarmOccNo;     //! ģ�����澯�����к�
	int32u  AlarmLimitOccNo;

	int16u	AlarmFlag;     //! ���λΪ1��������,Ϊ0������
	int16u  AlarmState;    //! �Կ�����������;��ģ����,
	int8u	Value;         //! ��ǰ��ֵ

	int8u  ExtraData[MSG_LEN - sizeof(DINALARM_MSG_H)];
};

/*! \struct  DIAG_MSG_H
*   \brief  ����ϸ澯�¼����� */
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
*   \brief  ����ϸ澯�¼����� */
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
*   \brief  �����澯�¼����� */
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
*   \brief  �����澯�¼����� */
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
*   \brief  ��ֵ �� ��ֵ������� */
struct SETVAL_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u	IddType;
	int8u	Att;  //! �ĸ�����
	int16u  Pad1;

	int32u	NodeOccNo;
	int32u	Occno;

	int8u	Datatype;
	int8u   Pad2;     /*MSG_SETVLUDONE 0:ѡ������ɹ� 1:ѡ�����ʧ�� */
	int8u   Source1; /*** see lcucnst.h ***/
	int8u   Source2;
	IO_VARIANT	Value[2];/*CTRL: 1ִ�У�2ѡ��3ѡ���ִ�У�4����*/
};

/*! \struct  REQDATA_MSG
*   \brief  �����ģ����ڶ�ֵ�ٻ���¼���ٻ��ȵ� */
struct REQDATA_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u   ExtraData[MSG_LEN - sizeof(int16u) * 2];

};
/*! \struct  RESPDATA_MSG
*   \brief  Ӧ���� */
struct RESPDATA_MSG
{
	int16u	MsgCode;
	int16u	Len;
	int8u   ExtraData[MSG_LEN - sizeof(int16u) * 2];
};

enum NBM_MSG_TYPES
{
	NBMT_MSG_SYNCH = 0,			 // ��Ϣͬ������
	NBMT_MSG_EVENT,				 // ������Ϣ����
	NBMT_CONTROL_EVENT,			 // ң����Ϣ����

	SEND_SWITCH_CMD_REQ = 10010, // �����л�����
	RECV_SWITCH_CMD_RES = 20010, // ������Ӧ
};

enum FE_MCODE
{
	MT_SETVALUE = 0X03,       //!< ��ֵ
	MT_SETVALUE_DONE = 0X04,  //!< ��ֵ���
	MT_NORMAL_EVENT = 0X50,   //!< ��ͨ�¼�
	MT_OPER = 0X5A,    //!< ���Ʋ�������
	MT_DIAG = 0X5B,    //!< �������Ϣ
	MT_RELAY = 0X5D,    //!< �����¼�
	MT_RELAY_EXT = 0X5E, //!< ������չ�¼� 
	MT_RELAY_DIAG = 0X5F, //!< ���������
};

/*! \struct  FES_CMD
*   \brief  ң�ء�ң������ */
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

