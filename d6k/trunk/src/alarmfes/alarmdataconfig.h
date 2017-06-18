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
	MAX_CONTENT_LENGTH=1024
}; 


struct DMSG;

//每条告警数据结构定义
class CAlarmDataConfig
{
public:
	CAlarmDataConfig()
	{
		m_nAlarmState = 0;
		m_nAlarmID = 0;
		memset(&m_Msg, 0, sizeof(DMSG));

		memset(&m_szContent, 0, sizeof(m_szContent));
	}
	~CAlarmDataConfig()
	{

	}
public:
	int     m_nAlarmState;
	int     m_nAlarmID;
	QString m_szTimeTag;
	DMSG    m_Msg;
	float   m_fVal;
	int8s   m_szContent[MAX_CONTENT_LENGTH*2];
public:
	std::string Dump()
	{
		char buf[2048];
		sprintf_s(buf,
			"alarmInfo:ID:%d,STATE:%d,RECV:TIME:%s,CONTENT:%s",
			m_nAlarmID, 
			m_nAlarmState,
			m_szTimeTag.toStdString().c_str(), 
			m_Msg.Buff);
		return buf;
	}
};



typedef std::shared_ptr < CAlarmDataConfig >  AlarmInfo_PTR;

typedef CSafeDeque<std::shared_ptr < CAlarmDataConfig > >  CAlarmInfoQueue;

//typedef	typename  CAlarmQueueInfo::ITERATOR iterator;


