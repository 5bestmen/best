#pragma once

#include "threadsafedata.h"
#include <string>
#include <memory>
enum  AckType
{
	AFTER_CONFIRM_DELETE =1,       //!<1 播报后留存待确认后删除
	AFTER_PLAY_DELETE    =2,       //!<2 播报后立即删除 
};



class CAudioBaseData
{
	enum
	{
		MAX_FILENAME_LENGTH = 128,
	};
public:
	std::string m_szTagName;       //主标识符
public:
	bool m_bAck;
	int m_nAckType;                //播报后确认处理方式 
	int m_nPlayTimes;              //播报次数
	int m_nDelayPlayTime;          //延时播报时间
	int m_nPriority;               //播报优先级
//!<文件播报
public:
	bool m_bSoundEnable;
	char m_szSoundFile[MAX_FILENAME_LENGTH];     //播报文件路径
//!<TTS文本播报
public:
	bool m_bSpeechTextEnable;      //是否播报文本
	char m_szSpeechText[MAX_FILENAME_LENGTH*4];    //播报文本内容
//!<beep响声
public:
	bool m_bBeepEnable;            //是否beep
};

typedef std::shared_ptr < CAudioBaseData > CAudioBaseData_DEF;
typedef CSafeDeque < CAudioBaseData_DEF > CAudioBaseDataDeque;


class CAudioAiAlarmDataUnit:public CAudioBaseData
{
public:
	CAudioAiAlarmDataUnit();
	~CAudioAiAlarmDataUnit();
};

class CAudioDiAlarmDataUnit :public CAudioBaseData
{
public:
	CAudioDiAlarmDataUnit();
	~CAudioDiAlarmDataUnit();
};

