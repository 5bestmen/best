#pragma once

#include "threadsafedata.h"
#include <string>
#include <memory>
enum  AckType
{
	AFTER_CONFIRM_DELETE =1,       //!<1 �����������ȷ�Ϻ�ɾ��
	AFTER_PLAY_DELETE    =2,       //!<2 ����������ɾ�� 
};



class CAudioBaseData
{
	enum
	{
		MAX_FILENAME_LENGTH = 128,
	};
public:
	std::string m_szTagName;       //����ʶ��
public:
	bool m_bAck;
	int m_nAckType;                //������ȷ�ϴ���ʽ 
	int m_nPlayTimes;              //��������
	int m_nDelayPlayTime;          //��ʱ����ʱ��
	int m_nPriority;               //�������ȼ�
//!<�ļ�����
public:
	bool m_bSoundEnable;
	char m_szSoundFile[MAX_FILENAME_LENGTH];     //�����ļ�·��
//!<TTS�ı�����
public:
	bool m_bSpeechTextEnable;      //�Ƿ񲥱��ı�
	char m_szSpeechText[MAX_FILENAME_LENGTH*4];    //�����ı�����
//!<beep����
public:
	bool m_bBeepEnable;            //�Ƿ�beep
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

