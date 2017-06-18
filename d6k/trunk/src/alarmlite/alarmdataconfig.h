#pragma once


#include "mail.h"
#include "datatypes.h"
#include "threadsafedata.h"
 
#include <string>
#include <memory>
#include <QDateTime>
#include <QMetaType>
#include "qglobal.h"

//�澯����״̬
enum ALARM_STATE
{
	alarm_unknown,       //״̬λ��
	alarm_confim,       //������ȷ��
	alarm_unconfim,     //����δȷ��
	alarm_delete,       //������ɾ��
};


enum 
{
	MAX_CONTENT_LENGTH=256,
}; 


struct DMSG;

//ÿ���澯���ݽṹ����
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


