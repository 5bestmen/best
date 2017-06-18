#ifndef DEFINE_H_
#define DEFINE_H_

//////////////////////////////////////////////////////
#define FES_CHANNEL_ITEM 15

#define FES_ALARM_ITEM 16

#define FES_SCALE_ITEM 17

#define FES_VARIABLE_ITEM 18
#define FES_SYSTEM_VARIABLE_ITEM 55
#define FES_USER_VARIABLE_ITEM 56
#define FES_USER_VARIBALE_ROOT_ITEM 57

#define FES_HIS_SAVE_ITEM 19

#define FES_PDR_ITEM 20

///////////////////////////////////////////////////////

#define FES_TYPE_RANGE_MIN 1

#define FES_TYPE_RANGE_MAX 200

//通道组
#define FES_CHANNEL_GROUP_ITEM 155
//通道
#define FES_CHANNEL_CHILD_ITEM 156
//装置
#define FES__CHANNEL_CHILD_DEVICE_ITEM 157

//报警
#define FES_TYPE_ALARM_GROUP_ITEM 165
#define FES_TYPE_ALARM_CHILD_ITEM 166
//转换
#define FES_TYPE_SCALE_GROUP_ITEM 175
#define FES_TYPE_SCALE_CHILD_ITEM 176
//变量
#define FES_TYPE_VARIABLE_GROUP_ITEM 185
//历史存储
#define FES_TYPE_HIS_AI_REPORT_ITEM 192
#define FES_TYPE_HIS_AI_CURVE_ITEM 193
#define FES_TYPE_HIS_AI_MAX_MIN_ITEM 194
#define FES_TYPE_HIS_AI_LIMIT_ITEM 195
#define FES_TYPE_HIS_AI_CUM_ITEM 196
#define FES_TYPE_HIS_DI_CUM_ITEM 197
#define FES_TYPE_HIS_KWH_CUM_ITEM 198


//对象
#define EQUIPMENT_ITEM 21
#define EQUIPMENT_GROUP_CHILD_ITEM 215
#define EQUIPMENT_CHILD_ITEM 216
//图形
#define GRAPH_ITEM 22
#define GRAPH_GROUP_CHILD_ITEM 225
#define GRAPH_CHILD_ITEM 226
//报表
#define REPORT_ITEM 23
#define REPORT_CHILD_DAY_ITEM 235
#define REPORT_CHILD_MONTH_ITEM 236
#define REPORT_CHILD_YEAR_ITEM 237
#define REPORT_CHILD_CUSTOM_ITEM 238
#define REPORT_CHILD_TIME_ITEM 239
//用户
#define USER_ITEM 25
#define USER_GROUP_CHILD_ITEM 255
#define USER_CHILD_ITEM 256
//节点
#define NODE_ITEM 26
//顺控与脚本
#define SEQ_SCRIPT_ITEM 27
#define SEQ_CHILD_ITEM 275
#define SCRIPT_CHILD_ITEM 276

#define MAX_GROUP_NUM 3

#define ROW_NOMAL 0
#define ROW_MODIFY 1
#define ROW_INSERT 2
#define ROW_ADD 3

typedef struct CHANNEL
{
	QString fieldName;

	QString fieldAlasName;

	QString fieldType;

	int fieldWidth;

	int fieldFlag;

	bool keyFlag;

	QString searchTable;

	QString valueField;

	QString showDesc;

	QString defaultValue;
} CHANNEL;

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

#define MYIPPORTREG "^(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\.(\\d{1,2}|1\\d\\d|2[0-4]\\d|25[0-5])\\:\\d{1,10}$"

#define MYCOMREG "^COM\\d+$"

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

//表
#define FES_CHANNEL "FES_CHANNEL"
#define ANALOG_INPUT "ANALOG_INPUT"
#define DIGITAL_INPUT "DIGITAL_INPUT"
#define FES_DEVICE "FES_DEVICE"
#define DIGITAL_OUTPUT "DIGITAL_OUTPUT"
#define ANALOG_OUTPUT "ANALOG_OUTPUT"

//组图标
#define CLOSE_GROUP_PNG ":images/close_group.png"
#define GROUP_NAME QStringLiteral("添加组")
//通道
#define CHANNEL_PNG ":images/channel.png"
#define CHANNEL_NAME QStringLiteral("添加通道")
#define CHANNEL_CONFIGURATION QStringLiteral("通道配置")
#define CHANNEL_GROUP_NAME QStringLiteral("添加通道组")
//装置
#define DEVICE_PNG ":images/device.png"
#define DEVICE_NAME QStringLiteral("添加装置") 
#define DEVICE_CONFIGURATION QStringLiteral("装置配置")

//报警
#define ALARM_GROUP_NAME QStringLiteral("添加报警组")
#define ALARM_NAME QStringLiteral("添加报警")
//转换
#define SCALE_GROUP_NAME QStringLiteral("添加转换组")
#define SCALE_NAME QStringLiteral("添加转换")
//变量
#define VARIABLE_GROUP_NAME QStringLiteral("添加变量组")
//设备
#define EQUIPMENT_GROUP_NAME QStringLiteral("添加设备组")
#define EQUIPMENT_NAME QStringLiteral("添加设备")
#define EQUIPMENT_PNG ":images/equipment.png"
//图形
#define GRAPH_GROUP_NAME QStringLiteral("添加图形组")
#define GRAPH_NAME QStringLiteral("添加图形")
#define GRAPH_PNG ":images/graph.png"
//用户
#define USER_GROUP_NAME QStringLiteral("添加用户组")
#define USER_NAME QStringLiteral("添加用户")
//报表
#define REPORT_PNG ":images/report.png"
//系统
#define SYSTEM_PNG ":images/system.png"
//节点
#define NODE_PNG ":images/node.png"
//顺控与脚本
#define SEQ_SCRIPT_PNG ":images/seqscript.png"
#endif