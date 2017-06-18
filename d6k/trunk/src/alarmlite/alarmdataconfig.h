#pragma once


#include "mail.h"
#include "datatypes.h"
#include "threadsafedata.h"
 
#include <string>
#include <memory>
#include <QDateTime>
#include <QMetaType>
#include "qglobal.h"

//告警数据状态
enum ALARM_STATE
{
	alarm_unknown,       //状态位置
	alarm_confim,       //报警已确认
	alarm_unconfim,     //报警未确认
	alarm_delete,       //报警待删除
};


enum 
{
	MAX_CONTENT_LENGTH=256,
}; 


struct DMSG;

//每条告警数据结构定义
struct CAlarmDataConfig
{
public:
	int     m_nAlarmState;
	int     m_nAlarmID;
	int     m_nType;
	int8s   m_szTimeTag[MAX_CONTENT_LENGTH];
	float   m_fVal;
	int8s   m_szContent[MAX_CONTENT_LENGTH*2];
};



typedef std::shared_ptr < CAlarmDataConfig >  AlarmInfo_PTR;

typedef CSafeDeque<std::shared_ptr < CAlarmDataConfig > >  CAlarmInfoQueue;

//typedef	typename  CAlarmQueueInfo::ITERATOR iterator;


