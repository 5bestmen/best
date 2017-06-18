#ifndef POWER_PREDICT_DEFINE_H
#define POWER_PREDICT_DEFINE_H

#include <QString>

//#define  POWER_PREDICT_ROOT_NAME     QStringLiteral("功率预测配置")
//#define  RENAME_ITEM   QStringLiteral("重命名")
//#define  DELETE_ITEM   QStringLiteral("删除")
//#define  GROUP_NAME    QStringLiteral("新建组")
//#define  POWER_FREDICT_NAME  QStringLiteral("新增预测项")
//#define  ADD_DEVICE    QStringLiteral("新建装置")
//#define  DELETE_DEVICE QStringLiteral("删除装置")

//Type
//#define POWER_PREDICT_CONFIG       425
//#define POWER_PREDICT_ITEM_GROUP   426
//#define POWER_PREDICT_DEVICE_ITEM  427
//#define POWER_PREDICT_ITEM         428

//ActionName
#define  POWER_PREDICT_ROOT_NAME   QObject::tr("功率预测")
#define  RENAME_ITEM			   QObject::tr("rename")
#define  DELETE_ITEM			   QObject::tr("delete")             //用于删item,也可删group
#define  GROUP_NAME				   QObject::tr("add group")
#define  ADD_STATION_NAME          QObject::tr("add station")
#define  ADD_POWER_FREDICT_ITEM	   QObject::tr("add predict item")
#define  ADD_DEVICE				   QObject::tr("add device")
#define  DELETE_DEVICE			   QObject::tr("delete device")


#define  PREDICT_GROUP_NAME        QObject::tr("group")
#define  PREDICT_ITEM_NAME         QObject::tr("item")
#define  PREDICT_DEVICE_NAME       QObject::tr("device")

//Icon
#define  POWER_PREDICT_PNG         ":/images/powerpredict.png"
#define  CLOSE_GROUP_PNG		   ":/images/close_group.png"
#define  OPEN_GROUP_PNG			   ":/images/open_group.png"
#define  POWERPREDICT_PNG		   ":/images/powerpredictitem.png"
#define  PREDICT_DEVICE_PNG		   ":/images/device.png"   
#define  PREDICT_RENAME_PNG		   ":/images/rename_1.png"
#define  PREDICT_DELETE_ITEM_PNG   ":/images/list-remove.png"
#define  PREDICT_GROUP_PNG         ":/images/group.jpg"

//HeadName
#define  HEAD_NAME_ID              QStringLiteral("ID")
#define  HEAD_NAME_TAGNAME         QStringLiteral("TagName")
#define  HEAD_NAME_DESCIBE         QStringLiteral("描述")
#define  HEAD_NAME_OCCNO           QStringLiteral("OccNo")
#define  LEFT_BRACKETS             QStringLiteral("(")
#define  RIGHT_BRACKETS            QStringLiteral(")")

#define PREDICT_LOG_LEVEL_0   0

#define	MIN_OCCNO_PREDICTION 0
#define	MAX_OCCNO_PREDICTION 20000000
#define	MIN_BLOCKNO_PREDICTION 0
#define	MAX_BLOCKNO_PREDICTION 20000000
#define	MAX_TAGNAME_LEN_PREDICTION 512
#define TABWIDGET_DESC (char *)QObject::tr("tab widget").toStdString().c_str()


//预测模块名为powerpredict
#define  POWER_PREDICT_DESC (char *)QObject::tr("powerpredict").toStdString().c_str()

enum HASHID
{
	POWERPREDICTHASHID, DEVICEHASHID, AIHASHID, DIHASHID
};


enum POWERPREDICTHASHTYPENUM
{
	MAX_IDD_NUM = 12
};


typedef struct PREDICT_AI_ATTRIBUTE
{
	QString desc;
	QString tagName;
}Predict_Attributes;

enum PREDICT_WIDGET_TYPE
{
	DEVICE_WIDGET_AI_TYPE, DEVICE_WIDGET_DI_TYPE, DEVICE_WIDGET,
};


#endif  //POWER_PREDICT_DEFINE_H