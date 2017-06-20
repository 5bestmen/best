/*! @file fescnst.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  fescnst.h
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
	IDD_AIN = 1,  //!< ģ����
	IDD_DIN = 2,  //!< ������
	IDD_AOUT = 3, //!< ģ���� 
	IDD_DOUT = 4, //!< ����
	IDD_USERVAR = 5, //!< �û��Զ������
	IDD_SYSVAR = 6,  //!< ϵͳ����


	IDD_NODE = 7,
	IDD_CHANNEL = 10,
	IDD_DEVICE = 11,
	IDD_OBJECT = 12,  //!< ����
	IDD_OBJECT_VAR = 13, //!< �����ڲ�����

	IDD_AINALARM = 15,
	IDD_DINALARM = 16,

	IDD_AINALARMLIMIT = 17,
	IDD_DINALARMLIMIT = 18,

	IDD_TRANSLINEAR = 19,
	IDD_TRANSNONLINEAR = 20,

	IDD_FTCHANNEL = 21,  //!< ����ǰ�õ�Զ��ת��
	IDD_FAPP = 22, //!< ����ǰ�õ�Ӧ��ģ��
	IDD_SAPP = 23, //!< ����SCADA��Ӧ��ģ��

	MAX_IDD = IDD_SAPP,
};

#if 0
enum DATA_TYPE 
{
	DT_NULL = 0,
	DT_BOOLEAN = 1,	  //!< ������8λ��ȱʡֵ0
	DT_CHAR = 2,      //!< �з����ֽڣ�8λ��
	DT_BYTE = 3,      //!< �з����ֽڣ�8λ��
	DT_SHORT = 4,     //!< �޷��Ŷ����� ��16λ�� 
	DT_WORD = 5,      //!< �޷������� ��16λ
	DT_INT = 6,       //!< �з��ų����� 32λ
	DT_DWORD = 7,      //!< �޷��ų����� 32λ
	DT_LLONG = 8,      //!< �з��ų�����  64λ
	DT_LWORD = 9,      //!< �޷��ų�����  64λ
	DT_FLOAT = 10,
	DT_DOUBLE = 11,
	DT_STRING = 12,
	DT_BLOCK = 13,
};
#endif // 0

enum IO_DATASOURCE
{
	IO_LCU = 1,         //!< ͨ���вɼ��õ�
	IO_HOSTCALU = 2,	//!< ��̨���Ը�ֵ 
};

enum NODE_TYPE
{
	NODE_NULL =   0 , //!< ��Ч�ڵ�
	NODE_SVR    = 1,  //!< ������
	NODE_FES    = 2,  //!< ǰ�û�
	NODE_CLIENT = 3,  //!< ����վ
};

/*! \enum NODE_STATE  fescnst.h  "fescnst.h"
*  \brief �ڵ�Ĺ���״̬.
*/
enum NODE_STATE
{
	STATE_UNKOWN = 0, //!< δ��״̬
	STATE_MAIN  = 1,  //!< ��������
	STATE_SLAVE = 2,  //!< �ӻ�����
};

enum  ALARM_TYPE
{
	OPER_LOG = 1,  //!< ����
	DIAG_LOG = 2,  //!< �����
	CTRL_LOG = 3, //!< ���Ƶ���
	RELAYDIAG_LOG = 4  //!< ���������
};

enum CHANNEL_PRINT 
{
	PRINT_OFF = 0, //!< ����ӡ���ģ�Ĭ�������Ϊ��ֵ
	PRINT_ON = 1,  //!< ��ӡ���ģ�һ��ֻ���ڵ���ʱ�򿪴�ѡ��
};

enum OCCNO_LIMIT
{
	INVALID_OCCNO = 0,
	MAX_OCCNO = 2000000,

	MAX_NODE_COUNT = 2048,
	MAX_NODE_OCCNO = MAX_NODE_COUNT,

	MAX_CHANNEL_COUNT = 100000,   //!< ���ͨ������
	MAX_CHANNEL_OCCNO = MAX_CHANNEL_COUNT,

	MAX_DEVICE_COUNT = 100000,    //!< ���װ������
	MAX_DEVICE_OCCNO = MAX_DEVICE_COUNT,

	MAX_AIN_ALARMLIMIT_BITCOUNT = 5,  //! ��λ���㣬5*32 = 160
	MAX_AIN_ALARM_LIMIT_COUNT = 160,  //! һ��ALARM��LIMIT����������
	MAX_DIN_ALARM_LIMIT_COUNT = 2,    //! һ������������ֻ����2��
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
	DEADBAND_RELATIVE = 0,  //!< ������̣�Ĭ��ֵ
	DEADBAND_ABSOLUTE = 1,  //!< ����ֵ
};

enum AIN_ALARM_LIMIT_CONDITION
{
	CONDITION_GE  = 0,    //!< ���ڵ���
	CONDITION_LE  = 1,    //!< С�ڵ���
	CONDITION_EQUAL = 3,  //!< ����
	CONDITION_NEQUAL = 4,  //!< ������
	CONDITION_RATE_INC = 5, //!< �仯������
	CONDITION_RATE_DEC = 6
};

enum DIN_ALARM_LIMIT_CONDITION
{
	CONDITION_021 = 0,    //!< ��0��1�仯
	CONDITION_120 = 1,    //!< ��1��0�仯
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

	SCAN_OUT = 0, //����˳�
	SCAN_IN = 1, //���Ͷ��
  	DEFAULT_SCAN = SCAN_IN,
 	DEFAULT_SCANTIME = 10,
	QUALITY_BAD = 1,
	QUALITY_OK = 0,
	DEFAULT_QUA = QUALITY_OK,

	MANSET = 1,//����ģ���� �����˹�����̬
	NORMAL = 0,//

	AIN_ALARM_ON = 1 , //! Խ��
	AIN_ALARM_OFF = 0, //! ����

};

enum LANG
{
	LANG_NONE = 1,
	LANG_EN,
	LANG_zh_CN,
};

//��������
enum
{
	DRIVER_TYPE_DLL,
	DRIVER_TYPE_EXE,
};

//����״ֵ̬
enum
{
	DRIVER_STATE_UNKNOWN,
	DRIVER_STATE_LOAD,
	DRIVER_STATE_UNLOAD,
};


/*! \enum TAG_ATT  fescnst.h  "fescnst.h"
*  \brief ͨ������.
*/
enum TAG_ATT
{
	ATT_IN_OUT = 1,      // Ͷ�˲�����1��Ͷ�룻 0���˳��� ���õ�LCUǰ�û�����ӦScanEnable��      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATT_VALUE = 2,	     // ʵ��ֵ   ������                          (AI/DI/VAR)   �˹���ʵ��ֵ ����Ͷ�˱�־��1ʱ���������õ�LCUǰ�û���(AI/DI)
	ATT_QUA = 3,         // Ʒ�ʣ�0���ã�1������  
	ATT_STATE = 4,       // ״̬��        (AI/DI/DO/AO ...)
	ATT_MANSET = 5,      // �˹����ñ�־��0���ޣ� 1���˹����ã�      (AI/DI)
	ATT_ALMDIS = 6,      // ���Ƹ澯��0������ 1�����ƣ�            (AI/DI)
	ATT_CTRLENA = 7,     // ң���������OK��0�������� 1��OK��        (CTRL)

	ATT_MINOUTPUT = 8,   // Low�źŹ���ֵ��Χ 
	ATT_MAXOUTPUT = 9,   // High�źŹ���ֵ��Χ                       (AI/AO��

	ATT_HIQUA = 10,      // ������������ֵ����ƫ��ϵ��               (AI/AO)
	ATT_LOQUA = 11,      // ������������ֵ����ƫ��ϵ��               (AI/AO)
	ATT_UNIT = 12,       // ������λ                                 (AI/AO/)

	ATT_SIGNAL_VALUE = 13,
	ATT_RAW_VALUE = 14,
	ATT_OLD_VALUE = 15,

	ATT_PINLABEL = 16,        // ���Ӻ�˵��  
  	
	ATT_DESCRIPTION = 17,     // ��������                                (AI/DI/DO/AO/PID/CTRL/PI)
	ATT_STATE0 = 18,          // 0̬����                                 (DI) 
	ATT_STATE1 = 19,          // 1̬����                                 (DI)
	ATT_NETA = 20,            // ģ����A��״̬                           (MODULE/NODE)  
	ATT_NETB = 21,            // ģ����B��״̬                           (MODULE/NODE)
	ATT_DOUT = 22,            // �������                                (DO)
	ATT_AOUT = 23,            // ģ�����                                (AO)
	ATT_MAXRANGE = 24,        // ���������ֵ��Χ                        (AI/AO/)
	ATT_MINRANGE = 25,        // ��С������ֵ��Χ                        (AI/AO/)
	ATT_ISMAIN  = 26,          // �ڵ�����״̬                            (NODE)

	ATT_MAINT = 27,           // ������  ��1�����ƣ� 0��ժ�ƣ� ���õ����ݷ�������     (CTRL/EQUIP)
	ATT_START = 28,           // �������ƣ�1��������           ���õ�LCUǰ�û���      (CTRL)
	ATT_ALMCHECK = 29,        // �澯ȷ�ϣ�1��ȷ�ϣ�           ���õ����ݷ�������     (AI/DI)
	ATT_REDSWITCH = 30,       // �ڵ������л�                                         (NODE)
	ATT_GETRELAYVALUE = 31,   // �ٻ���ֵ                                             (MODULE)
	ATT_SETRELAYVALUE = 32,   // �趨ֵ                                               (MODULE)
	ATT_CONFIRMSET = 33,      // ȷ���޸Ķ�ֵ                                         (MODULE)
	ATT_CANCELSET  = 34,       // ȡ���޸Ķ�ֵ                                         (MODULE)
	ATT_RESETRELAY = 35,      // ��������                                             (MODULE)
	ATT_TEST       = 36,      // ���ݲ���                                             (AI/DI)��̨ʹ��
	ATT_TESTCANCEL = 37,      // ȡ�����ݲ���                                         (AI/DI)��̨ʹ��
	ATT_MAX        = 38
};
  
/*! \enum TAG_ATTR  fescnst.h  "fescnst.h"
*  \brief �ɶ�����. FieldID
*/
enum TAG_ATTR
{
	ATTR_IN_OUT = 1,      // Ͷ�˲�����1��Ͷ�룻 0���˳��� ���õ�LCUǰ�û�����ӦScanEnable��      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATTR_VALUE = 2,	     // ʵ��ֵ   ������                        (AI/DI/VAR)   
	ATTR_QUA = 3,         // Ʒ�ʣ�0���ã�1������  
	ATTR_STATE = 4,       // ״̬��        (AI/DI/DO/AO ...)
	ATTR_MANSET = 5,      //�˹����ñ�־��0���ޣ� 1���˹����ã�      (AI/DI)
	ATTR_ALMDIS = 6,      //���Ƹ澯��0������ 1�����ƣ�            (AI/DI)
	ATTR_CTRLENA = 7,     //ң���������OK��0�������� 1��OK��        (CTRL)

	ATTR_MINOUTPUT = 8,   //Low�źŹ���ֵ��Χ 
	ATTR_MAXOUTPUT = 9,   //High�źŹ���ֵ��Χ                      (AI/AO

	ATTR_HIQUA = 10,      //������������ֵ����ƫ��ϵ��              (AI/AO)
	ATTR_LOQUA = 11,      //������������ֵ����ƫ��ϵ��              (AI/AO)
	ATTR_UNIT = 12,       //������λ                                (AI/AO/)

	ATTR_SIGNAL_VALUE,
	ATTR_RAW_VALUE,
	ATTR_OLD_VALUE,

	ATTR_PINLABEL,             //���Ӻ�˵��  
	ATTR_DESCRIPTION,     //��������                                (AI/DI/DO/AO/PID/CTRL/PI)
	ATTR_STATE0,          //0̬����                                 (DI) 
	ATTR_STATE1,          //1̬����                                 (DI)
	ATTR_NETA,            //ģ����A��״̬                           (MODULE/NODE)  
	ATTR_NETB,            //ģ����B��״̬                           (MODULE/NODE)
	ATTR_DOUT,            //�������                                (DO)
	ATTR_AOUT,            //ģ�����                                (AO)
	ATTR_MAXRANGE,        //���������ֵ��Χ                        (AI/AO/)
	ATTR_MINRANGE,        //��С������ֵ��Χ                        (AI/AO/)
	ATTR_ISMAIN,          //�ڵ�����״̬                            (NODE)

	ATTR_MAINT,           // ������  ��1�����ƣ� 0��ժ�ƣ� ���õ����ݷ�������     (CTRL/EQUIP)
	ATTR_START,           // �������ƣ�1��������           ���õ�LCUǰ�û���      (CTRL)
	ATTR_ALMCHECK,        // �澯ȷ�ϣ�1��ȷ�ϣ�           ���õ����ݷ�������     (AI/DI)
	ATTR_REDSWITCH,       // �ڵ������л�                                         (NODE)
	ATTR_GETRELAYVALUE,   // �ٻ���ֵ                                             (MODULE)
	ATTR_SETRELAYVALUE,   // �趨ֵ                                               (MODULE)
	ATTR_CONFIRMSET,      // ȷ���޸Ķ�ֵ                                         (MODULE)
	ATTR_CANCELSET,       // ȡ���޸Ķ�ֵ                                         (MODULE)
	ATTR_RESETRELAY,      // ��������                                             (MODULE)
	ATTR_TEST,            // ���ݲ���                                             (AI/DI)��̨ʹ��
	ATTR_TESTCANCEL,      // ȡ�����ݲ���                                         (AI/DI)��̨ʹ��
	ATTR_MAX
};
 
/*! \enum TAG_ATTW  fescnst.h  "fescnst.h"
*  \brief �ɶ�д����. FieldID
*/
enum TAG_ATTW
{
	ATTW_IN_OUT = 1,      // Ͷ�˲�����1��Ͷ�룻 0���˳��� ���õ�LCUǰ�û�����ӦScanEnable��      (AI/DI/DO/AO/VAR/DEVICE/CHANNEL/NODE)
	ATTW_VALUE = 2,	      // �˹���ʵ��ֵ ����Ͷ�˱�־��1ʱ���������õ�LCUǰ�û���(AI/DI)
	ATTW_QUA = 3,         // Ʒ�ʣ�0���ã�1������  
	ATTW_MANSET = 5,      //�˹����ñ�־��0���ޣ� 1���˹����ã�      (AI/DI)
	ATTW_ALMDIS = 6,      //���Ƹ澯��0������ 1�����ƣ�            (AI/DI)
	ATTW_CTRLENA = 7,     //ң���������OK��0�������� 1��OK��        (CTRL)

	ATTW_DOUT = 22,            //�������                                (DO)
	ATTW_AOUT = 23,            //ģ�����                                (AO)

	ATTW_MAINT = 27,           // ������  ��1�����ƣ� 0��ժ�ƣ� ���õ����ݷ�������     (CTRL/EQUIP)
	ATTW_START = 28,           // �������ƣ�1��������           ���õ�LCUǰ�û���      (CTRL)
	ATTW_ALMCHECK = 29,        // �澯ȷ�ϣ�1��ȷ�ϣ�           ���õ����ݷ�������     (AI/DI)
	ATTW_REDSWITCH = 30,       // �ڵ������л�                                         (NODE)
	ATTW_GETRELAYVALUE = 31,   // �ٻ���ֵ                                             (MODULE)
	ATTW_SETRELAYVALUE,   // �趨ֵ                                               (MODULE)
	ATTW_CONFIRMSET,      // ȷ���޸Ķ�ֵ                                         (MODULE)
	ATTW_CANCELSET,       // ȡ���޸Ķ�ֵ                                         (MODULE)
	ATTW_RESETRELAY,      // ��������                                             (MODULE)
	ATTW_TEST,            // ���ݲ���                                             (AI/DI)��̨ʹ��
	ATTW_TESTCANCEL,      // ȡ�����ݲ���                                         (AI/DI)��̨ʹ��
	ATTW_MAX
};

#endif // FES_DATA_CONST_H


/** @}*/

