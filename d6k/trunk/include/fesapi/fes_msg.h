/*! @file mem_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.h
文件实现功能 :  实时数据库模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  需要注意是否在一个进程空间内
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   实时数据库模块
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/
#ifndef FES_MSG_DEF_H
#define FES_MSG_DEF_H

#include "fesdatadef.h"

enum MSG_TYPES
{
	MT_AIN,
	MT_DIN,
	MT_SOE,
	MT_NET,
	MT_SYS,
	MT_CTRL,
	MT_MAX,
};

struct MSG_BASE
{
	int16u Type;
	int16u Len;
	TIMEPAK Time;
};


//模拟量越限、复限、梯度限
struct MSG_LIMIT : public MSG_BASE
{
	int32u FesOccNo; //前置OccNo
	int32u AinOccNo; //测点OccNo
	int32u LimitType;//越限类型
	
	fp64 AinValue;   //当时测值
};

//开关量变位
struct MSG_BREAK : public MSG_BASE
{
	int32u FesOccNo; //前置OccNo
	int32u DinOccNo; //测点OccNo

	int8u DinValue;   //当时测值
};

//SOE
struct MSG_SOE : public MSG_BASE
{
	int32u FesOccNo; //前置OccNo
	int32u DinOccNo; //测点OccNo

	int8u DinValue;   //当时测值
};


//遥控命令
struct MSG_CTRL : public MSG_BASE
{
public:
	int32u OccNo;//遥控点OccNo

public:
	int32u UserOccNo;

public:
	int32u CommandType;//遥控命令类型,45,46,48,49,50?

public:
	int32u DataType;//模拟量?开关量?
	fp64   Value;    //点值

public:
	int  CtrlType; // 遥控类型(总召，总召确认，对时,对时确认，预置，预置确认，执行，执行确认，撤销，撤销确认....)
};





struct MSG_EVENT : public MSG_BASE
{
	int32u	EvtCode;    //! 报警信息代码
	int32u	Occno;

	int16u	AlarmFlag;         //最高位为1不允许报警,为0允许报警
	int16u  AlarmState;        //对开关量无意义;对模拟量,
	IO_VARIANT	Value;
};

struct MSG_SETVAL : public MSG_BASE
{
	int8u	Att;
	int16u  Pad1;

	int32u	NodeOccNo;
	int32u	Occno;

	int8u	Datatype;
	int8u  Pad2;     /*MSG_SETVLUDONE 0:选择或撤销成功 1:选择或撤销失败 */
	int8u  Source1; /*** see lcucnst.h ***/
	int8u  Source2;
	IO_VARIANT	Value[2];/*CTRL: 1执行；2选择；3选择后执行；4撤销*/
};


enum FE_MCODE
{
	MT_SETVALUE = 0X03,       //!< 设值
	MT_SETVALUE_DONE = 0X04,  //!< 设值完成
	MT_NORMAL_EVENT = 0X50,   //!< 普通事件
	MT_OPER  = 0X5A,    //!< 控制操作调节
	MT_DIAG  = 0X5B,    //!< 自诊断信息
	MT_RELAY = 0X5D,    //!< 保护事件
	MT_RELAY_EXT= 0X5E, //!< 保护扩展事件 
	MT_RELAY_DIAG = 0X5F, //!< 保护自诊断
};

struct FES_CMD
{

};

#endif // FES_MSG_DEF_H


/** @}*/

