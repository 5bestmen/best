#ifndef SCADA_STUDIO_DEFINE_H_
#define SCADA_STUDIO_DEFINE_H_

#include "datatypes.h"

#include <QString>


#define	MIN_OCCNO_SCADASTUDIO 0
#define	MAX_OCCNO_SCADASTUDIO 20000000
#define	MIN_BLOCKNO_SCADASTUDIO 0
#define	MAX_BLOCKNO_SCADASTUDIO 20000000
#define	MAX_TAGNAME_LEN_SCADASTUDIO 512
#define MAX_NAME_LENGTH_SCADASTUDIO 512
#define MAX_ADDRESS_LENGTH_SCADASTUDIO 128	//����ַ

#define FES_ROOT_NAME QObject::tr("fesconfig")

#define FES_CONFIG 5

#define FES_ITEM 13
#define FES_ITEM_GROUP 14

#define FES_CHANNEL_ITEM 15

#define FES_ALARM_ITEM 16
#define FES_ANALOG_ALARM_ITEM 6
//#define FES_ANALOG_ALARM_GROUP 21
#define FES_DIGTAL_ALARM_ITEM 7
//#define FES_DIGTAL_ALARM_GROUP 22

#define FES_SCALE_ITEM 17
#define FES_SCALE_ITEM_LINEAR 21
#define FES_SCALE_ITEM_NONLINEAR 25

#define FES_VARIABLE_ITEM 18
#define FES_SYSTEM_VARIABLE_ITEM 55
#define FES_USER_VARIABLE_ITEM 56
//#define FES_USER_VARIBALE_CHILD_ITEM 57
#define FES_USER_VARIBALE_GROUP_ITEM 58
//#define FES_TYPE_VARIABLE_GROUP_ITEM 185

#define FES_HIS_SAVE_ITEM 19

#define FES_PDR_ITEM 20

#define FES_FORWARDING_CHANNEL_LIST 22
///////////////////////////////////////////////////////

#define FES_TYPE_RANGE_MIN 1

#define FES_TYPE_RANGE_MAX 200

//ͨ����
#define FES_CHANNEL_GROUP_ITEM 155
//ͨ��
#define FES_CHANNEL_CHILD_ITEM 156
//װ��
#define FES__CHANNEL_CHILD_DEVICE_ITEM 157
//#define FES_DEVICE_AI_TAGNAME	158
//#define FES_DEVICE_DI_TAGNAME	159
//#define FES_DEVICE_AO_TAGNAME	160
//#define FES_DEVICE_DO_TAGNAME	161
#define FES_DEVICE_AI	162
#define FES_DEVICE_DI	163
#define FES_DEVICE_AO	164
#define FES_DEVICE_DO	165
//����
#define FES_TYPE_ANALOG_ALARM_GROUP_ITEM 166
#define FES_TYPE_ALARM_ANALOG_ITEM 167
#define FES_TYPE_ALARM_ANALOG_LIMIT_ITEM 1665
#define FES_TYPE_DIGTAL_ALARM_GROUP_ITEM 168
#define FES_TYPE_ALARM_DIGITAL_ITEM 169
#define FES_TYPE_ALARM_DIGITAL_LIMIT_ITEM 1675

//ת��
#define FES_TYPE_SCALE_LINEAR_GROUP_ITEM 175
#define FES_TYPE_SCALE_NONLINEAR_GROUP_ITEM 176
#define FES_TYPE_SCALE_LINEAR_CHILD_ITEM 177
#define FES_TYPE_SCALE_NONLINEAR_CHILD_ITEM 178
//����

//��ʷ�洢
#define FES_TYPE_HIS_AI_REPORT_ITEM 192
#define FES_TYPE_HIS_AI_CURVE_ITEM 193
#define FES_TYPE_HIS_AI_MAX_MIN_ITEM 194
#define FES_TYPE_HIS_AI_LIMIT_ITEM 195
#define FES_TYPE_HIS_AI_CUM_ITEM 196
#define FES_TYPE_HIS_DI_CUM_ITEM 197
#define FES_TYPE_HIS_KWH_CUM_ITEM 198

//ת��
#define FES_FORWARDING_CHANNEL_ITEM 216
#define FES_FORWARDING_CHANNEL_GROUP_ITEM 217
#define FES_FORWARDING_CHANNEL_AI_ITEM 2154 //#define FES_FORWARDING_CHANNEL_KWH_ITEM 2159
#define FES_FORWARDING_CHANNEL_DI_ITEM 2155
#define FES_FORWARDING_CHANNEL_AO_ITEM 2156
#define FES_FORWARDING_CHANNEL_CG_ITEM 2157					//����
#define FES_FORWARDING_CHANNEL_DO_ITEM 2158
#define FES_FORWARDING_CHANNEL_KWH_ITEM 2159


//������
#define REPORT_ADD_GROUP      QStringLiteral("�½�����")
#define REPORT_DELETE_GROUP   QStringLiteral("ɾ������")
#define REPORT_NEW_CREATE   QStringLiteral("�½�����")
#define IMPORT_REPORT_ITEM  QStringLiteral("���뱨��")

#define DELETE_REPORT_ITEM  QStringLiteral("ɾ������")
#define MODITY_NAME_ITEM    QStringLiteral("�޸�����")
#define MODITY_TYPE_ITEM    QStringLiteral("�޸�����")


///////////////////////////////////////////////////////

#define FES_TYPE_RANGE_MIN 1

#define FES_TYPE_RANGE_MAX 200

//ͨ����
#define FES_CHANNEL_GROUP_ITEM 155
//ͨ��
#define FES_CHANNEL_CHILD_ITEM 156
//װ��
#define FES__CHANNEL_CHILD_DEVICE_ITEM 157


//�û�
#define USER_MANAGE_ITEM 25
#define USER_MANAGE_GROUP_ITEM 255
#define USER_MANAGE_CHILD_ITEM 256
//�ڵ�
#define NODE_ITEM 26
#define NODE_ITEM_SERVER 265
#define NODE_ITEM_FES 266
#define NODE_ITEM_FES_CHILD_GROUP 268
#define NODE_ITEM_FES_CHILD_ITEM 269
#define NODE_ITEM_WORKSTATION 267
#define NODE_ITEM_WORKSTATION_CHILD_ITEM 270
#define NODE_ITEM_WORKSTATION_CHILD_GROUP 271
//˳����ű�
#define SEQ_SCRIPT_ITEM 27
#define SEQ_CHILD_ITEM 275
#define SCRIPT_CHILD_ITEM 276
//ϵͳ
#define SYSTEM_CONFIG_ITEM 28

#define EXTEND_SERVICE_ITEM 30

#define SCADAVARIABLE_CONFIG 31
#define SCADA_ITEM 315
#define SCADA_ITEM_GROUP 316

#define SCADAVARIABLE_ITEM_VARIABLE 3155
//scada����_ϵͳ����
#define SCADAVARIABLE_ITEM_VARIABLE_SYSTEM 31555
#define SCADAVARIABLE_ITEM_VARIABLE_USER 315556
//scada����_������
#define SCADAVARIABLE_ITEM_VARIABLE_USER_GROUP 31557
//scada�����澯
#define SCADAVARIABLE_ITEM_ALARM 3158
#define SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM 31585
#define SCADAVARIABLE_ITEM_ANALOG_ALARM_ITEM_CHILD 31586
#define SCADAVARIABLE_ITEM_ANALOG_ALARM_LIMIT_ITEM 31587
#define SCADAVARIABLE_ITEM_ANALOG_ALARM_GROUP_ITEM 31588
#define SCADAVARIABLE_ITEM_DIGTAL_ALARM_ITEM 31589
#define SCADAVARIABLE_ITEM_DIGITAL_ALARM_ITEM_CHILD 31595
#define SCADAVARIABLE_ITEM_DIGITAL_ALARM_LIMIT_ITEM 31596
#define SCADAVARIABLE_ITEM_DIGITAL_ALARM_GROUP_ITEM 31597
//scadaת��
#define SCADAVARIABLE_ITEM_SCALE 3160
//scada����ת��
#define SCADAVARIABLE_ITEM_SCALE_LINEAR_ROOT 31604
#define SCADAVARIABLE_ITEM_SCALE_LINEAR_GROUP 31605
#define SCADAVARIABLE_ITEM_SCALE_LINEAR_CHILD_ITEM 31606
//scada������ת��
#define SCADAVARIABLE_ITEM_SCALE_NONLINEAR_ROOT 31607
#define SCADAVARIABLE_ITEM_SCALE_NONLINEAR_GROUP 31608
#define SCADAVARIABLE_ITEM_SCALE_NONLINEAR_CHILD_ITEM 31609

#define ROW_NOMAL 0
#define ROW_MODIFY 1
#define ROW_INSERT 2
#define ROW_ADD 3

typedef struct CHANNELSCADASTUDIO
{
	QString fieldName;

	QString fieldAlasName;

	QString fieldType;

	int fieldWidth;

	int fieldFlag;

	bool keyFlag;		//����

	QString searchTable;

	QString valueField;

	QString showDesc;

	QString defaultValue;
} CHANNELSCADASTUDIO;

#define MYUNCHAR "^([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])$"
#define MYUSHORTREG "^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|([1-5][0-9][0-9][0-9][0-9])|(6[0-4][0-9][0-9][0-9])|(65[0-4][0-9][0-9])|(655[0-2][0-9])|(6553[0-5]))$"
#define MYUINTREG "^([0-9]|[1-9][0-9]|[1-9][0-9][0-9]|[1-9][0-9][0-9][0-9]|[1-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
[1-3][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]|\
(4[0-1][0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
(42[0-8][0-9][0-9][0-9][0-9][0-9][0-9][0-9])|\
(429[0-3][0-9][0-9][0-9][0-9][0-9][0-9])|\
(4294[0-8][0-9][0-9][0-9][0-9][0-9])|\
(42949[0-5][0-9][0-9][0-9][0-9])|\
(429496[0-6][0-9][0-9][0-9])|\
(4294967[0-1][0-9][0-9])|\
(42949672[0-8][0-9])|\
(429496729[0-5]))$"
#define MYSTRINGREG "^[\\s\\S]{0,%1}$"

#define MYIPPORTREG "((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))\\:\\d{1,10}$"
//#define MYIPREG "^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])$"
#define MYIPREG "^((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))$"

#define MYCOMREG "^COM\\d+$"

#define TAGNAMEREG "^[0-9a-zA-Z#]{1,%1}$"
#define GROUPNAMEREG "^[0-9a-zA-Z#\u4e00-\u9fa5]{1,%1}$"

typedef struct IDDESC
{
	QString ID;
	QString desc;
} IDDESC;

#define UNINT "UNINT"
#define UNSHORT "UNSHORT"
#define UNCHAR "UNCHAR"
#define MYSTRING "STRING"
#define MYFLOAT "FLOAT"
#define MYIPPORT "IPPORT"
#define MYCOM "COM"

#define COMPARE_TRUE 0

//��
#define FES_CHANNEL "FES_CHANNEL"
#define ANALOG_INPUT "ANALOG_INPUT"
#define DIGITAL_INPUT "DIGITAL_INPUT"
#define FES_DEVICE "FES_DEVICE"
#define DIGITAL_OUTPUT "DIGITAL_OUTPUT"
#define ANALOG_OUTPUT "ANALOG_OUTPUT"

#define NEW_TREE_ITEM  QStringLiteral("�½�")
#define NEW_GROUP_TREE_ITEM  QStringLiteral("�½���")

#define DELETE_TREE_ITEM  QStringLiteral("ɾ��")
#define COPY_TREE_ITEM    QStringLiteral("����")
#define CUT_TREE_ITEM     QStringLiteral("����")
#define PASTE_TREE_ITEM   QStringLiteral("ճ��")
#define RENAME_TREE_ITEM  QStringLiteral("������")
#define ATTR_TREE_ITEM  QStringLiteral("����")



#define DELETE_TREE_ITEM QStringLiteral("ɾ��")

//����
#define PROJECT_PNG ":images//project.png"


#define RENAME_ITEM	QObject::tr("rename")
#define EXPAND_ITEM	QObject::tr("expand")
#define COLLAPSE_ITEM	QObject::tr("collapse")
#define GROUP_ITEM QObject::tr("add group")
#define DELETE_ITEM	QObject::tr("delete")

//ǰ��
#define FES_PNG ":images/fes.png"
#define GROUP_NAME QObject::tr("group")
#define FES_NAME QObject::tr("add fes")
#define FES_MODIFY_ITEM tr("modify fes")
#define FES_GROUP_MODIFY tr("modify group")

//��ͼ��
#define CLOSE_GROUP_PNG ":images/close_group.png"
#define OPEN_GROUP_PNG ":images/open_group.png"

//ͨ��
#define CHANNEL_PNG ":images/channel.png"
#define CHANNEL_NAME QObject::tr("add channel")
#define CHANNEL_MODIFY QObject::tr("modify channel")
#define CHANNEL_GROUP_MODIFY QObject::tr("modify group channel")
#define CHANNEL_GROUP_NAME QObject::tr("add channel group")
//װ��
#define DEVICE_PNG ":images/device.png"
#define DEVICE_NAME QObject::tr("add device") 
#define DEVICE_CONFIGURATION QStringLiteral("װ������")
#define DEVICE_MODIFY tr("modify device")

//����
#define ADD_ALARM_GROUP_NAME QObject::tr("add alarm group")
#define ADD_ALARM_NAME QObject::tr("add alarm")
#define ALARM_OFFLIMIT_NAME tr("add analog offlimit")
#define ALARM_PNG ":images/alarm.png"
//ת��
#define SCALE_GROUP_NAME QObject::tr("group")
#define SCALE_NAME QObject::tr("add scale")
#define SCALE_PNG ":images/scale.png"
//����
#define VARIABLE_GROUP_NAME QObject::tr("add group")
#define VARIABLE_PNG ":images/variablelist.png"
#define VARIABLE_SYSTEM_PNG ":images/systemvariable.png"
#define VARIABLE_USER_PNG ":images/variablelist.png"
//��ʷ�洢
#define HIS_SAVE_PNG ":images/hissave.png"
//PDR
#define PDR_PNG ":images/pdr.png"
//ת��
#define FES_FORWARDING_ADDCHANNELGROUP_NAME QObject::tr("add forwarding channel group")
#define FES_FORWARDING_ADDCHANNELITEM_NAME QObject::tr("add forwarding channel item")
#define PDR_PNG ":images/pdr.png"

//�豸
#define EQUIPMENT_GROUP_NAME QStringLiteral("����豸��")
#define EQUIPMENT_NAME QStringLiteral("����豸")
#define EQUIPMENT_PNG ":images/equipment.png"
//ͼ��
#define GRAPH_GROUP_NAME QStringLiteral("�½���")
#define GRAPH_NAME QStringLiteral("�½�")
#define GRAPH_PNG ":images/graph.png"
//�û�
#define USER_GROUP_NAME QObject::tr("add user group")
#define USER_NAME QObject::tr("add user")
#define USER_MANAGER_PNG ":images/uservariable.png"
//����
#define REPORT_PNG ":images/report.png"
//ϵͳ
#define SYSTEM_PNG ":images/system.png"
//�ڵ�
#define NODE_PNG ":images/node.png"
#define ADD_NODE_FES_GROUP tr("add node fes group")
#define ADD_NODE_FES_ITEM tr("add node fes item")
#define ADD_NODE_WORKSTATION_GROUP tr("add node workstation group")
#define ADD_NODE_WORKSTATION tr("add node workstation")
//˳����ű�
#define SEQ_SCRIPT_PNG ":images/seqscript.png"
//scada����
#define ADD_SCADA_DESC QObject::tr("add scada")

#define ADD_USER_VARIABLE_GROUP QObject::tr("add user virable group")
#define ADD__VARIABLE_GROUP QObject::tr("add user virable group")
//#define ADD_ALARM_NAME QStringLiteral("add alarm")

#define MODIFYROWCOLOR QColor(Qt::blue)
#define INSERTROWCOLOR QColor(Qt::darkGreen)
#define DELETEROWCOLOR QColor(Qt::red)
#define NORMALROWCOLOR QColor(Qt::black)

#define SAVE_PNG ":images/save.png"

#define DELETE_PNG ":images/delete.png"

#define NEW_PNG ":images/new.png"
#define CLOSE_PROJECT_PNG ":images/closeproject.png"
typedef struct CONFIG_PARA
{
	QString desc;
	QString tagname;
} CONFIG_PARA;

#define DRIVE_104 "104"
#define DRIVE_101 "101"
#define DRIVE_MODBUS "modbus"
#define DEVICEIP "127.0.0.1"
#define DEVICEPORT 2404

//#define TAGNAME_SIZE 40
#define CHANNELNAME_SIZE 128

//============================ͨ��===========================
#define CHANNEL_DESC_ADD "ͨ��"
#define CHANNEL_TAGNAME_ADD "channel"
//ͨ�������С
//#define CHANNEL_TAGNAME_SIZE 128
//#define CHANNEL_CHANNELNAME_SIZE 128
#define UINT_HIGH 2147483647
#define UINT_LOW 1

#define CHANNEL_TYPE "ͨ��"
#define FES_TYPE "ǰ��"
#define DEVICE_TYPE "װ��"

struct FLAG
{
	bool F_MODIFY;
};

#define FES_DESC (char *)QObject::tr("fes").toStdString().c_str()
#define CHANNEL_DESC (char *)QObject::tr("channel").toStdString().c_str()
#define AI_ALARM_DESC "ai alarm"
#define AI_ALARM_LIMIT_DESC "ai alarm limit"
#define DI_ALARM_DESC "di alarm"
#define DI_ALARM_LIMIT_DESC "di alarm limit"
#define DEVICE_DESC (char *)QObject::tr("device").toStdString().c_str()
#define AI_DESC (char *)QObject::tr("ai").toStdString().c_str()
#define DI_DESC (char *)QObject::tr("di").toStdString().c_str()
#define AO_DESC (char *)QObject::tr("ao").toStdString().c_str()
#define DO_DESC (char *)QObject::tr("do").toStdString().c_str()
#define LINEAR_SCALE_DESC (char *)QObject::tr("linear scale").toStdString().c_str()
#define NONLINEAR_SCALE_DESC (char *)QObject::tr("nonlinear scale").toStdString().c_str()
#define SCADASTUDIO_DESC (char *)QObject::tr("scadastudio").toStdString().c_str()
#define NODE_CONFIG_DESC (char *)QObject::tr("node").toStdString().c_str()
#define EXTEND_SERVICE_DESC (char *)QObject::tr("extendservice").toStdString().c_str()
#define USER_MANAGE_DESC (char *)QObject::tr("user").toStdString().c_str()
#define SCADAVARIABLE_DESC (char *)QObject::tr("scadavariable").toStdString().c_str()

#define TABWIDGET_DESC (char *)QObject::tr("tab widget").toStdString().c_str()

//device
enum DeviceType
{
	PROTECTIONANDCONTROL = 1, PROTECTION, MEASUREMENTANDCONTROL, COMMUNICATIONMANAGEMENTMACHINE, OTHERS
	//1���������  2 - ����  3����� 4������� 5 - ����
};

//variant.hһ��
//enum VariableDataType
//{
//	BooleanType,			//λ						������(ң�š�ң��)
//	CharType,				//�з����ֽ�(8λ)
//	ByteType,				//�޷����ֽ�(8λ)
//	ShortType,				//�з�����(16λ)
//	WordType,				//�޷�����(16λ)
//	LongType,				//�з���˫��(32λ)
//	DWordType,				//�޷���˫��(32λ)
//	LLongType,				//�з���˫��(64λ)
//	LWordType,				//�޷���˫��(64λ)
//	FloatType,				//�����ȸ���(32λ)
//	DoubleType,				//˫���ȸ���(64λ)			ģ����(ң�⡢ң����ң��)
//	StringType				//�ַ���(0������)
//
//};

enum InitialQua
{
	//��ʼƷ��
	NormalQua, UndefiedQua, UnnormalQua, UnconnectQua
};


enum SignalType
{
	NormallyOpen,			//���� 0
	NormallyClosed			//���� 1			
};

//ǰ��ת����scadaת������һ��һ����
enum ScaleType
{
	LINEAR, NONLINEAR, SCALETYPENULL
};

//ǰ�ø澯��scada�澯����һ��һ����
enum AlarmType
{
	AIALARM, DIALARM, ALARMTYPENULL
};

enum SelectScaleType
{
	AISELECTSCALE, AOSELECTSCALE
};

enum AlarmCategory
{
	//�¹� 0
	//���� 1
	//��ͨ 2
	ACCIDENT, FAULT, COMMON
};



enum AlarmAckType
{
	//ȷ�Ϻ���ʾ��ʽ
	//1.�澯�¼�ȷ�Ϻ��ڸ澯������ʧ
	//2.�澯�¼�ȷ�Ϻ����ڸ澯����
	//3.�澯�¼�ȷ�Ϻ�ȷ���¼��ŵ��Ѿ�ȷ�ϵĸ澯�����С�
	DELETE=1, KEEP, CONFORM
};

////�澯����
//enum CMP_CONDITION
//{
//	CONDITION_EQ_STUDIO = 0, //!< ����
//	CONDITION_NE_STUDIO,     //!< ������
//	CONDITION_GE_STUDIO,     //!< ���ڵ���
//	CONDITION_GT_STUDIO,     //!< ����
//	CONDITION_LE_STUDIO,     //!< С�ڵ���
//	CONDITION_LT_STUDIO,     //!< С��
//};

enum DeadType
{
	ABS, Range
};



//fes��scadavariable����ģ�����澯����
//ģ�����澯
enum AnalogAlarmDeadArea
{
	VALUE, PERCENT
};

enum AlarmContion
{
	GREATEREQUAL, LESSEQUAL, EQUAL, NOTEQUAL, BWTEEN, RATEREDUCE, RATEINCREASE
};

//�������澯
enum DIAlarmContion
{
	ZEROTOONE, 	//0 0->1
	ONETOZERO	//1 1->0
};

enum NODETYPE
{
	SERVER = 1, FEP, WORKSTAIOTN, HISTORYSERVER, WEBSERVER
	//1-��������(�����������ݴ洢��SCADA�����)
	//2-FEPǰ�û�
	//3 - ����վ
	//4 - ��ʷ������
	//5 - WEB������
};


//Fes�����ļ�
class FES_PROJECT_INFO
{
public:
	FES_PROJECT_INFO()
	{

	}
	~FES_PROJECT_INFO()
	{

	}

	QString TagName;
	QString OldTagName;
	int FesState;
};

//#define LOG_STRING(x) (char *)QStringLiteral(x).toStdString().c_str()

#define LOG_STRING(x) (char *)tr(x).toStdString().c_str()

//=================================widget type=====================================
enum REPROTTYPE
{
	DAY_REPORT = 0,            //��<�ձ���
	WEEK_REPORT = 1,           //��<�ܱ���
	TEN_DAYS_REPORT = 2,       //��<Ѯ����
	MONTH_REPORT = 3,          //��<�±���
	QUARTER_REPORT = 4,        //��<���ȱ���
	YEAR_REPORT = 5,           //��<�걨��
	SPECIAL_REPORT = 6,        //��<���ⱨ��
	HOUR_STATICS_REPORT = 7,   //��<ʱ��ͳ�Ʊ���	
};

#define CHECK_LESS_ZERO_INT(x) x >= 0 ? true : false

enum HASHID
{
	FESHASHID,
	CHANNELHASHID, 
	DEVICEHASHID, 
	AIHASHID, 
	DIHASHID,
	AOHASHID, 
	DOHASHID, 
	TRANSFORMHASHID,
	ANALOGALARMHASHID,
	ANALOGALARMLIMITHASHID,
	DIGITALALARMHASHID, 
	DIGITALALARMLIMITHASHID,
	SYSTEMVARIABLEID,
	USERVIRIABLEGROUPID,
	USERVIRIABLEID, 
	FORWARDINGCHANNEL, 
	FORWARDINGAIHASHID,
	FORWARDINGDIHASHID,
	FORWARDINGDOHASHID	,
	FORWARDINGAOHASHID,
	FORWARDINGCGHASHID,
	FORWARDINGKWHHASHID
};

enum FES_HASH_TYPE_NUM
{
	MAX_IDD_NUM = 22
};

enum SCADA_VARIABLE_HASH_ID
{
	SCADAID,
	SYSTEMID, 
	USERID,
	ANALOGALARMID,
	ANALOGALARMLIMITID,
	DIGITALALARMID, 
	DIGITALALARMLIMITID,
	LINEARTRANSFORMID, 
	NONLINEARTRANSFORMID
};

enum SCADA_VARIABLE_HASH_TYPE_NUM
{
	MAX_NUM = 9
};

//�ڵ����з���ģ���� ������
#define NORMAL_SERVER "NORMAL_SERVER"
#define NORMAL_WORKSTATION "NORMAL_WORKSTATION"
#define OPTION_TRUE "true"
#define OPTION_FALSE "false"
#define ServiceConfig "ServiceConfig"

struct DESC_STRING_POOL
{
	DESC_STRING_POOL()
	{
		strDesc = "";
		nOccNo = 0;
		bTranslation = false;
		strTranslation = "";
	}
	
	std::string strDesc;
	int32u nOccNo;
	bool bTranslation;
	std::string strTranslation;
};

#define LOG_FES_LOG "log/fes_log.log"
#define LOG_NODE_LOG "log/node_log.log"
#define LOG_SCADA_VARAIBLE_LOG "log/scada_vairaible_log.log"

#endif