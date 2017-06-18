#ifndef ALARM_CONFIG_DATA_H
#define ALARM_CONFIG_DATA_H

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

namespace Config
{
	//告警风格
	struct ALARM_STYLE
	{
		bool PushPicture;			//推图
		bool Pdr;					//Pdr
		bool PlaySound;				//播放音乐
		bool Blink;					//闪烁
		bool LogAlarmWindow;		//登录告警窗
		bool AccidentHandle;		//事故处理
		bool AccidengPrint;			//事故打印
		bool LightPlate;			//光字牌
	};

	/*! \class CAinAlarmLimit config_data.h  "config_data.h"
	*  \brief 模拟量报警限值
	*  \details  一个报警中可以包含多个限值，比如高限、高高限....
	*/
	class CAinAlarmLimit:public CBaseData
	{
	public:
		CAinAlarmLimit()
		{
			memset(m_szTagName, 0, sizeof(m_szTagName));
			m_strDescription = "";
			//ReadAccessLevel = 0;
			//WriteAccessLevel = 0;
			m_nID = 0;
			//Condition = 0;
			m_dbValue = 0;
			m_dbLowValue = 0;
			m_nDelayAlarm = 0;
			m_nPriority = 0;
			m_bSupportAck = true;
			m_bSupportDelete = true;
			m_nBlockNo = 1;
			m_nOccNo = 0;
			m_nPlaySoundTimes = 0;
			m_nBackColor = 0;
			m_nBlinkBackColor = 0;
			m_nBlinkTextColor = 0;
			m_nTextColor = 0;
		}
		~CAinAlarmLimit()
		{

		}
		
		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			Q_UNUSED(bFlag);
		}

		virtual bool GetModifyFlag() const
		{
			return false;
		}
		// 对输入的名字进行校验检查
		bool CheckTagName(std::string & szTagName) const
		{
			Q_UNUSED(szTagName);
			
			return true;
		}

		void SetOccNo(int nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		virtual int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetBlockNo(int nBlockNo)
		{
			m_nBlockNo = nBlockNo;
		}

		int GetBlockNo()
		{
			return m_nBlockNo;
		}

		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];			//Tag name
		QString m_strDescription;		//描述

		int32u m_nID;						//ID

		int8u m_nCondition;	//告警条件

		int8u m_nCategory;	//事故
							//故障
							//普通


		fp64 m_dbValue;
		fp64 m_dbLowValue;
		int32u m_nDelayAlarm;
		int32u m_nPriority;

		bool m_bSupportAck;		//支持确认
		bool m_bSupportDelete;		//支持删除

		int8u m_nAckType;		//确认后显示方式

		QString m_strSoundFile;		//声音文件
		int32u m_nPlaySoundTimes;	//播放次数
		QString m_strSpeechAlarmText;


		//int32u ReadAccessLevel;	//16 levels
		//int32u WriteAccessLevel;		//16 levels

		bool m_bBeep;
		QString m_strPushGraph;
		bool m_bBlinkGraph;
		bool m_bLog;

		int32u m_nBackColor;						//背景色
		int32u m_nTextColor;						//文本色
		int32u m_nBlinkBackColor;					//闪烁背景色
		int m_nBlinkTextColor;						//闪烁文本色
		bool m_bLightBoard;							//点亮光字牌
		QString m_strDispGuide;						//显示处理指导
		QString m_strCommands;


		//QString TimeFormat;			//时间格式

		//0 >=		1 <=		2 =			3 变化率降低		4变化率提高
		//5不等于	6 介于


		//int8u AlarmStyle;		//0 推画面	1 事故追忆	2 播放音乐	3 闪烁	4 登录报警窗
		////5 事故处理	6 事故打印	7 光字牌

		//报警通知
		//接收者
		QString Acceptor;
		//发送文本
		//电子邮件
		//语音电话
		//短信
		//传真
	private:
		int32u m_nOccNo;					//大排行号 同一个前置所有告警限值
		int32u m_nBlockNo;					//小排行号 同一个告警下的限值号
	};

	//模拟量告警条件
	struct ALARM_CONDITION_AI
	{
		float HiHiValue;	//高高限值
		float HiValue;		//高限值
		float LoValue;		//低限值
		float LoLoValue;	//低低限值
	};

	class CAnalogAlarm :public CBaseData
	{
	public:
		CAnalogAlarm();
		~CAnalogAlarm();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}


		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		bool AddAinAlarmLimit(CAinAlarmLimit *pAinAlarmLimit);
		bool DeleteItem(CAinAlarmLimit *pAinAlarmLimit);
	public:
		//int32u m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//tag name
		QString m_strDescription;	//描述

		int32u m_nID;					//ID

		QString m_strGroupName;
		bool m_bEnable;				//启用
		bool m_bOnQualityGood;		//品质


		int8u m_nDeadType;			//死区类型
		fp64 m_dbDeadArea;			//死区

		int32u m_nDelayAlarm;		//延时报警

		int m_nAlarmType;

		//bool m_bSupportAck;
		//bool m_bSupportDelete;
		//QString m_strSoundFile;
		//QString m_strPlaySoundTimes;

		std::vector<CAinAlarmLimit *> m_arrAlarmOfflimit;		//限值vec

	private:
		void Clear();
	private:
		int32u m_nOccNo;				//大排行号
	};


	class CAnalogAlarmGroup : public CBaseData
	{
	public:
		CAnalogAlarmGroup();
		~CAnalogAlarmGroup();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			Q_UNUSED(bFlag);
		}

		virtual bool GetModifyFlag() const
		{
			return true;
		}

		void SetGroupName(QString &strGroupName)
		{
			m_strGroupName = strGroupName;
		}

		//只是接口 没有实习用处
		virtual int32u GetOccNo() const
		{
			return 0;
		}

		bool AddAnalogGroup(CAnalogAlarmGroup *pGroup);
		bool AddAnalogItem(CAnalogAlarm *pItem);

		std::vector<CAnalogAlarmGroup *> &GetGroup()
		{
			return m_arrAnalogGroup;
		}

		QString GetGroupName()
		{
			return m_strGroupName;
		}

		std::vector<CAnalogAlarm *> &GetItem()
		{
			return m_arrAnalogItem;
		}

		bool CheckModifyGroupNameExist(CAnalogAlarmGroup *pGroup, QString &strTagName);

		CAnalogAlarmGroup *CreateGroup();

		bool CheckGroupNameExist(QString &strTagName);
		bool DeleteGroup(CAnalogAlarmGroup *pChildGroup);
		bool DeleteItem(CAnalogAlarm *pItem);
	private:
		//组包含的组
		std::vector<CAnalogAlarmGroup *> m_arrAnalogGroup;
		std::vector<CAnalogAlarm *> m_arrAnalogItem;

		QString m_strGroupName;
	};

	class CAnalogAlarmNotify :public CBaseData
	{
	public:
		CAnalogAlarmNotify();
		~CAnalogAlarmNotify();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

	private:

	};

	class CDinAlarmLimit :public CBaseData
	{
	public:
		CDinAlarmLimit();
		~CDinAlarmLimit()
		{

		}

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			Q_UNUSED(bFlag);
		}

		virtual bool GetModifyFlag() const
		{
			return true;
		}

		virtual int32u GetOccNo() const
		{
			return OccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			OccNo = nOccNo;
		}

		char TagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];			//tag name
		QString Description;		//描述

		int32u ID;						//ID

		int8u Condition;			//条件
		//0->1 告警	0 0->1
		//1->0 告警	1 1->0
		int8u Category;					//事故
		//故障
		//普通状态变化

		int32u DelayAlarm;			//延时报警
		int32u Priority;				//优先级别

		bool SupportAck;			//支持确认
		bool SupportDelete;		//支持删除

		int8u AckType;					//确认后显示方式

		QString SoundFile;			//声音文件
		int32u PlaySoundTimes;		//播放次数

		QString SpeechAlarmText;

		bool Beep;					//蜂鸣器
		QString PushGraph;
		bool BlinkGraph;
		bool Log;

		int32u BackColor;		//背景色
		int32u TextColor;			//文本色
		int32u BlinkBackColor;		//闪烁背景色
		int32u BlinkTextColor;					//闪烁文本色

		QString DispGuide;

		QString Commands;			//命令

		//报警通知
		//接收者
		//QString Acceptor;
		//发送文本
		//电子邮件
		//语音电话
		//短信
		//传真

		int32u OccNo;					//大排行号
		int32u BlockNo;				//小排行号 同一个告警下的限值号
	};

	class CDigitalAlarm :public CBaseData
	{
	public:
		CDigitalAlarm();
		~CDigitalAlarm();

		virtual void Save();

		void Clear();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag()const
		{
			return m_bModifyFlag;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		bool AddDinAlarmLimit(CDinAlarmLimit *pDinAlarmLimit);
		bool DeleteItem(CDinAlarmLimit *pDinAlarmLimit);
	public:
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		//QString m_szTagName;		//tag name
		QString m_strDescription;	//描述


		int32u m_nID;					//ID

		QString m_strGroupName;
		bool m_bEnable;				//启用
		bool m_bOnQualityGood;		//品质
		int32u m_nDelayAlarm;		//延时报警

		int m_nAlarmType;

		std::vector<CDinAlarmLimit *> m_arrDigtalOfflimit;		//限值vec
	private:
		int32u m_nOccNo;				//大排行号

	};


	class CDigitalAlarmGroup : public CBaseData
	{
	public:
		CDigitalAlarmGroup();
		~CDigitalAlarmGroup();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			Q_UNUSED(bFlag);
		}

		virtual bool GetModifyFlag() const
		{
			return true;
		}

		CDigitalAlarmGroup *CreateGroup();

		void SetGroupName(QString &strGroupName)
		{
			m_strGroupName = strGroupName;
		}

		QString GetGroupName()
		{
			return m_strGroupName;
		}

		//接口
		virtual int32u GetOccNo() const
		{
			return 0;
		}

		std::vector<CDigitalAlarmGroup *> &GetGroup()
		{
			return m_arrDigitalGroup;
		}

		std::vector<CDigitalAlarm *> &GetItem()
		{
			return m_arrDigitalItem;
		}

		bool AddDigitalGroup(CDigitalAlarmGroup *pGroup);
		bool AddDigitalItem(CDigitalAlarm *pItem);
		
		bool CheckModifyGroupNameExist(CDigitalAlarmGroup *pGroup, QString &strTagName);

		bool DeleteGroup(CDigitalAlarmGroup *pGroup);
		bool DeleteItem(CDigitalAlarm *pItem);

	private:
		bool CheckGroupNameExist(QString &strTagName);
	private:
		//组包含的组
		std::vector<CDigitalAlarmGroup *> m_arrDigitalGroup;
		std::vector<CDigitalAlarm *> m_arrDigitalItem;

		QString m_strGroupName;
	};
}

#endif

