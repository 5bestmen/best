/*! @file reportdefine.h
<PRE>
********************************************************************************
模块名       :
文件名       :  reportdefine.h
文件实现功能 :  报表一些基本属性定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   报表一些基本属性定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef REPORT_DEFINE_H
#define REPORT_DEFINE_H




#define REPORT_ADD_GROUP      QStringLiteral("新建表组")
#define REPORT_DELETE_GROUP   QStringLiteral("删除表组")
#define REPORT_NEW_CREATE   QStringLiteral("新建报表")
#define IMPORT_REPORT_ITEM  QStringLiteral("导入报表")

#define DELETE_REPORT_ITEM  QStringLiteral("删除报表")
#define MODITY_NAME_ITEM    QStringLiteral("修改名称")
#define MODITY_TYPE_ITEM    QStringLiteral("修改属性")


//报表
#define REPORT_ROOT 23
//报表组
#define REPORT_GROUP_ITEM 231
//报表单元
#define REPORT_ITEM 232
//报表组最大数目
#define MAX_REPORT_GROUP_COUNT 3

//日报表
#define REPORT_DAY_ITEM 235               //作为组节点
#define REPORT_CHILD_DAY_GROUPITEM 2351   //作为子节点
#define REPORT_CHILD_DAY_ITEM 2352
//月报表
#define REPORT_MONTH_ITEM 236
#define REPORT_CHILD_MONTH_GROUPITEM 2361
#define REPORT_CHILD_MONTH_ITEM 2362
//年报表
#define REPORT_YEAR_ITEM 237
#define REPORT_CHILD_YEAR__GROUPITEM 2371
#define REPORT_CHILD_YEAR__ITEM 2372
//自定义报表
#define REPORT_CUSTOM_ITEM 238
#define REPORT_CHILD_CUSTOM_GROUPITEM 2381
#define REPORT_CHILD_CUSTOM_ITEM 2382

#define REPORT_CHILD_TIME_ITEM 239

#define  REPORT_USER_DATA_ROLE  (Qt::UserRole + 100 )  

#define REPORT_DES (char *)tr("report").toStdString().c_str()

enum REPROTTYPE
{
	DAY_REPORT = 0,            //！<日报表
	WEEK_REPORT = 1,           //！<周报表
	TEN_DAYS_REPORT = 2,       //！<旬报表
	MONTH_REPORT = 3,          //！<月报表
	QUARTER_REPORT = 4,        //！<季度报表
	YEAR_REPORT = 5,           //！<年报表
	SPECIAL_REPORT = 6,        //！<特殊报表
	HOUR_STATICS_REPORT = 7,   //！<时段统计报表	
	UNKNOWN_REPORT = 100,      //! <未知类型报表
};

#endif