#ifndef PREDICT_DEFINE_H
#define PREDICT_DEFINE_H

#include <QObject>

//日志等级
#define PREDICT_LOG_LEVEL_0   0

//预测模块名为powerpredict
#define  POWER_PREDICT_DESC (char *)QObject::tr("powerpredict").toStdString().c_str()

#define  POWER_PREDICT_ROOT_TYPE		 425                        //根类型
#define  POWER_PREDICT_GROUP_TYPE	     426					    //组类型
#define  POWER_PREDICT_ITEM_TYPE		 427					    //item类型
#define  POWER_PREDICT_STATION_TYPE		 501					    //整站数据类型
#define  POWER_PREDICT_INVERTER_GROUP_TYPE  502					    //逆变器组类型
#define  POWER_PREDICT_WEATHER_TYPE		 503					    //气象仪数据类型
#define  POWER_PREDICT_PREDICT_TYPE		 504					    //预测数据类型
#define  POWER_PREDICT_INVERTER_TYPE     505					    //逆变器数据类型

#define  PREDICT_ROOT_ROLE				 Qt::UserRole			    //根角色
#define  PREDICT_GROUP_ROLE				 Qt::UserRole + 425 + 110   //组角色
#define  PREDICT_ITEM_ROLE				 Qt::UserRole + 425 + 120   //item角色
#define  PREDICT_QUOTE_ITEM_ROLE		 Qt::UserRole + 425 + 130   //控件角色
#define  PREDICT_OLD_NAME_ROLE			 Qt::UserRole + 425 + 140   //最后命名角色
#define  PREDICT_PATH_ROLE				 Qt::UserRole + 425 + 150   //路径角色
#define  LASTVALUE						 Qt::UserRole + 425 + 160   //上次值角色
#define  POINTERDATA					 Qt::UserRole + 425 + 170   //指针角色
#define  GROUP_NUMBER_ROLE				 Qt::UserRole + 425 + 180   //组数量角色
#define  STATION_NAME_ROLE               Qt::UserRole + 425 + 190   //厂站名角色

#define  ROOT_GROUP						 0
#define  MAX_GROUP_NUM				  	 3                          //厂站组最多3层

//ActionName
#define  POWER_PREDICT_ROOT_NAME         QObject::tr("powerpredict")
#define  RENAME_ITEM					 QObject::tr("rename")
#define  DELETE_ITEM					 QObject::tr("delete")            //用于删item,也可删group
#define  GROUP_NAME						 QObject::tr("add group")
#define  ADD_POWER_FREDICT_ITEM			 QObject::tr("add predict item")
#define  ADD_INVERTER                    QObject::tr("add inverter")

#define  PREDICT_GROUP_NAME				 QObject::tr("group")
#define  PREDICT_ITEM_NAME				 QObject::tr("station")
#define  PREDICT_INVERTER_NAME		     QObject::tr("inverter")

//Icon
#define  POWER_PREDICT_PNG				 ":/new/prefix1/images/powerpredict.png"
#define  CLOSE_GROUP_PNG				 ":/new/prefix1/images/close_group.png"
#define  OPEN_GROUP_PNG					 ":/new/prefix1/images/open_group.png"
#define  PREDICT_ITEM_PNG	     		 ":/new/prefix1/images/powerpredictitem.png" 
#define  PREDICT_RENAME_PNG				 ":/new/prefix1/images/rename_1.png"
#define  PREDICT_DELETE_ITEM_PNG		 ":/new/prefix1/images/list-remove.png"
#define  PREDICT_GROUP_PNG				 ":/new/prefix1/images/group.jpg"
#define  PREDICT_DATA_PNG                ":/new/prefix1/images/data_pic.jpg"
#define  PREDICT_ADD_INVERTER_PNG		 ":/new/prefix1/images/list-add.png"
#define  PREDICT_DEVICE_PNG              ":/new/prefix1/images/device.png"

#define  PREDICT_LINKED_POINT              QStringLiteral("linked point")

enum PREDICT_WIDGET_TYPE
{
	STATION_DATA_TYPE,
	INVERTER_DATA_TYPE,
	WEATHER_DATA_TYPE,
	PREDICT_DATA_TYPE,
};

#define M_MAX_CONTENT_LEN   256

#endif  //PREDICT_DEFINE_H