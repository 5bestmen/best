#ifndef SCADAVIRABLE_DATA_H
#define SCADAVIRABLE_DATA_H

#include <QObject>

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

#include <unordered_map>
#include <vector>
#include <QDomDocument>
#include <QXmlStreamReader>

class QXmlStreamWriter;
class IMainModuleInterface;

namespace Config
{	
	class CScadaSystemVaraible :public CBaseData
	{
	public:
		CScadaSystemVaraible();
		~CScadaSystemVaraible();

		virtual void Save()
		{

		}

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

	public:
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述
		int32u m_nID;
		int32u m_nType;			//变量类型

	private:
		int32u m_nOccNo;
	};

	class CScadaUserVariable : public CBaseData
	{
	public:
		CScadaUserVariable();
		~CScadaUserVariable();

		virtual void Save()
		{

		}

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

	public:
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述
		int32u m_nID;
		int32u m_nType;			//变量类型
		/*
		enum DATA_TYPE
		{
			DT_NULL = 0,
			DT_BOOLEAN = 1,	  //!< 布尔，8位，缺省值0
			DT_CHAR = 2,      //!< 有符号字节（8位）
			DT_BYTE = 3,      //!< 无符号字节（8位）
			DT_SHORT = 4,     //!< 无符号短整数 ，16位，
			DT_WORD = 5,      //!< 无符短整数 ，16位
			DT_INT = 6,       //!< 有符号长整数 32位
			DT_DWORD = 7,      //!< 无符号长整数 32位
			DT_LLONG = 8,      //!< 有符号长整数  64位
			DT_LWORD = 9,      //!< 无符号长整数  64位
			DT_FLOAT = 10,
			DT_DOUBLE = 11,
			DT_STRING = 12,
			DT_BLOCK = 13,
			DT_MAX
};
		*/


		fp64 m_dbInitValue;
		int8u m_nInitialQua;

		QString m_strAddress;
		QString m_strPinLabel;

		QString m_strSourceTagName;
		QString m_strScaleTagName;
		int32u m_nScaleType;
		int32u m_nScaleOccNo;
		//bool m_bNewScalePoint;

		QString m_strAlarmTagName;
		int32u m_nAlarmType;
		int32u m_nAlarmOccNo;
	private:
		int32u m_nOccNo;
	};

	class CScadaUserVariableGroup : public CBaseData
	{
	public:
		CScadaUserVariableGroup();
		~CScadaUserVariableGroup();

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

		QString GetGroupName()
		{
			return m_strGroupName;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		bool AddUserVariableGroup(CScadaUserVariableGroup *pGroup);
		bool AddUserVariableItem(CScadaUserVariable *pItem);

		CScadaUserVariableGroup *CreateGroup();
		bool CheckGroupNameExist(QString &strTagName);

		std::vector<CScadaUserVariable *> &GetItem()
		{
			return m_arrItem;
		}

		bool DeleteGroup(CScadaUserVariableGroup *pGroup);

	public:
		//组包含的组
		std::vector<CScadaUserVariableGroup *> m_arrGroup;
		std::vector<CScadaUserVariable *> m_arrItem;
	private:
		QString m_strGroupName;
	};

	class CScadaAinAlarmLimit :public CBaseData
	{
	public:
		CScadaAinAlarmLimit()
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
			m_bLog = false;
		}
		~CScadaAinAlarmLimit()
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

	class CScadaAnalogAlarm :public CBaseData
	{
	public:
		CScadaAnalogAlarm();
		~CScadaAnalogAlarm();

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

		//bool AddAinAlarmLimit(CAinAlarmLimit *pAinAlarmLimit);
		//bool DeleteItem(CAinAlarmLimit *pAinAlarmLimit);
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

		std::vector<CScadaAinAlarmLimit *> m_arrAlarmOfflimit;		//限值vec

	private:
		void Clear();
	private:
		int32u m_nOccNo;				//大排行号
	};


	class CScadaAnalogAlarmGroup : public CBaseData
	{
	public:
		CScadaAnalogAlarmGroup();
		~CScadaAnalogAlarmGroup();

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

		bool AddAnalogGroup(CScadaAnalogAlarmGroup *pGroup);
		bool AddAnalogItem(CScadaAnalogAlarm *pItem);

		std::vector<CScadaAnalogAlarmGroup *> &GetGroup()
		{
			return m_arrAnalogGroup;
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

		std::vector<CScadaAnalogAlarm *> &GetItem()
		{
			return m_arrAnalogItem;
		}

		bool CheckModifyGroupNameExist(CScadaAnalogAlarmGroup *pGroup, QString &strTagName);

		CScadaAnalogAlarmGroup *CreateGroup();

		bool CheckGroupNameExist(QString &strTagName);
		bool DeleteGroup(CScadaAnalogAlarmGroup *pChildGroup);
		bool DeleteItem(CScadaAnalogAlarm *pItem);
	private:
		//组包含的组
		std::vector<CScadaAnalogAlarmGroup *> m_arrAnalogGroup;
		std::vector<CScadaAnalogAlarm *> m_arrAnalogItem;

		QString m_strGroupName;
	};

	class CScadaDinAlarmLimit :public CBaseData
	{
	public:
		CScadaDinAlarmLimit()
		{
			OccNo = 0;
			BlockNo = 0;
			memset(TagName, 0, sizeof(TagName));
			Description = "";
			ID = 0;
			Condition = ONETOZERO;
			Category = AlarmCategory::COMMON;
			DelayAlarm = 0;
			Priority = 0;
			SupportAck = true;
			SupportDelete = true;
			AckType = AlarmAckType::DELETE;
			SoundFile = "";
			PlaySoundTimes = 0;
			SpeechAlarmText = "";
			Beep = false;
			PushGraph = "";
			BlinkGraph = false;
			Log = false;
			BackColor = 0;
			TextColor = 0;
			BlinkBackColor = 0;
			BlinkTextColor = 0;
			DispGuide = "";
			Commands = "";
		}

		~CScadaDinAlarmLimit()
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

		//接口
		virtual int32u GetOccNo() const
		{
			return 0;
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

		int32u BlockNo;				//小排行号 同一个告警下的限值号

		//报警通知
		//接收者
		//QString Acceptor;
		//发送文本
		//电子邮件
		//语音电话
		//短信
		//传真
		void SetOccNo(int nOccNo)
		{
			OccNo = nOccNo;
		}

		int32u GetOccNo()
		{
			return OccNo;
		}
	private:

		int32u OccNo;					//大排行号

	};

	class CScadaDigitalAlarm :public CBaseData
	{
	public:
		CScadaDigitalAlarm();
		~CScadaDigitalAlarm();

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

		//bool AddDinAlarmLimit(CDinAlarmLimit *pDinAlarmLimit);
		//bool DeleteItem(CDinAlarmLimit *pDinAlarmLimit);
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

		std::vector<CScadaDinAlarmLimit *> m_arrDigtallimit;		//限值vec
	private:
		int32u m_nOccNo;				//大排行号

	};

	class CScadaDigitalAlarmGroup : public CBaseData
	{
	public:
		CScadaDigitalAlarmGroup();
		~CScadaDigitalAlarmGroup();

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

		CScadaDigitalAlarmGroup *CreateGroup();

		void SetGroupName(QString &strGroupName)
		{
			m_strGroupName = strGroupName;
		}

		QString GetGroupName()
		{
			return m_strGroupName;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		std::vector<CScadaDigitalAlarmGroup *> &GetGroup()
		{
			return m_arrDigitalGroup;
		}

		std::vector<CScadaDigitalAlarm *> &GetItem()
		{
			return m_arrDigitalItem;
		}

		bool AddDigitalGroup(CScadaDigitalAlarmGroup *pGroup);
		bool AddDigitalItem(CScadaDigitalAlarm *pItem);

		bool CheckGroupNameExist(QString &strTagName);
		bool DeleteGroup(CScadaDigitalAlarmGroup *pGroup);
		bool DeleteItem(CScadaDigitalAlarm *pItem);

		bool CheckModifyGroupNameExist(CScadaDigitalAlarmGroup *pGroup, QString &strTagName);

	private:

	private:
		//组包含的组
		std::vector<CScadaDigitalAlarmGroup *> m_arrDigitalGroup;
		std::vector<CScadaDigitalAlarm *> m_arrDigitalItem;

		QString m_strGroupName;
	};


	struct SCADA_NONLINEAR_SCALE
	{
		fp64 RawValue;
		fp64 ScaleValue;
	};


	class CScadaLinearTransform :public CBaseData
	{
	public:
		CScadaLinearTransform();
		~CScadaLinearTransform();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		//void SetOccNo(int32u nOccNo)
		//{
		//	m_nOccNo = nOccNo;
		//}

	public:
		//uint m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		bool m_bEnableInverse;	//是否采用反比
		fp64 m_dbMaxRaw;			//最大原始值
		fp64 m_dbMinRaw;			//最小原始值
		fp64 m_dbMaxScale;			//最大值
		fp64 m_dbMinScale;			//最小值
		//fp64 m_dbInverse;
		bool m_bEnableFactor;		//启用因子
		bool m_bEnableInverseFactor;		//取反因子



		fp64 m_dbGain;				//增益	
		fp64 m_dbOffset;			//偏置

		int m_nType;		//类型
	private:
		//uint m_nOccNo;				//大排行号
	};

	class CScadaNonlinearTransform :public CBaseData
	{
	public:
		CScadaNonlinearTransform();
		~CScadaNonlinearTransform();

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}

		//void SetOccNo(int32u nOccNo)
		//{
		//	m_nOccNo = nOccNo;
		//}
	public:
		//uint m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		int m_nType;		//类型

		std::vector<SCADA_NONLINEAR_SCALE *> m_arrPNonliear;

	private:
		void Clear();
	private:
		//uint m_nOccNo;				//大排行号
	};

	//转换
	class CScadaTransform :public CBaseData
	{
	public:
		CScadaTransform()
			:m_pLinearScale(NULL), m_pNonLinearScale(NULL)
		{
			memset(m_szTagName, 0, sizeof(m_szTagName));

			m_nOccNo = 0;
		}
		~CScadaTransform()
		{
			if (m_pLinearScale)
			{
				delete m_pLinearScale;
				m_pLinearScale = nullptr;
			}

			if (m_pNonLinearScale)
			{
				delete m_pNonLinearScale;
				m_pNonLinearScale = nullptr;
			}
		}

		virtual void Save()
		{

		}

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
		{
			return m_bModifyFlag;
		}

		CScadaLinearTransform *GetLinear()
		{
			if (!m_pLinearScale)
			{
				m_pLinearScale = new CScadaLinearTransform;
			}

			Q_ASSERT(m_pLinearScale);

			return m_pLinearScale;
		}

		void SetLinear(CScadaLinearTransform *pLinear)
		{
			m_pLinearScale = pLinear;

			Q_ASSERT(pLinear);
		}

		CScadaNonlinearTransform *GetNonLinear()
		{
			if (!m_pNonLinearScale)
			{
				m_pNonLinearScale = new CScadaNonlinearTransform;
			}

			Q_ASSERT(m_pNonLinearScale);

			return m_pNonLinearScale;
		}

		void SetNonLinear(CScadaNonlinearTransform *pNonLinear)
		{
			m_pNonLinearScale = pNonLinear;

			Q_ASSERT(pNonLinear);
		}

		void GetType(int &nType)
		{
			nType = m_nType;
		}

		bool CopyLinearToNon()
		{
			//线性转换为非线性
			if (m_pLinearScale == NULL && m_pNonLinearScale == NULL)
			{
				return false;
			}

			if (m_pLinearScale == NULL)
			{
				m_pNonLinearScale = new CScadaNonlinearTransform;
			}

			memset(m_pNonLinearScale->m_szTagName, 0, sizeof(m_pNonLinearScale->m_szTagName));
			int nSize = sizeof(m_pLinearScale->m_szTagName);
			strncpy(m_pNonLinearScale->m_szTagName, m_pLinearScale->m_szTagName, qMin((int)MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			m_pNonLinearScale->m_strDescription = m_pLinearScale->m_strDescription;
			m_pNonLinearScale->m_strGroupName = m_pNonLinearScale->m_strGroupName;

			memset(this->m_szTagName, 0, sizeof(this->m_szTagName));
			strncpy(this->m_szTagName, m_pLinearScale->m_szTagName, qMin((int)MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			return true;
		}

		bool CopyNonToLinear()
		{
			//非线性转换为线性
			if (m_pLinearScale == NULL && m_pNonLinearScale == NULL)
			{
				return false;
			}

			if (m_pLinearScale == NULL)
			{
				m_pLinearScale = new CScadaLinearTransform;
			}

			memset(m_pLinearScale->m_szTagName, 0, sizeof(m_pLinearScale->m_szTagName));
			strncpy(m_pLinearScale->m_szTagName, m_pNonLinearScale->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, (int)sizeof(m_pLinearScale->m_szTagName)));
			m_pLinearScale->m_strDescription = m_pNonLinearScale->m_strDescription;
			m_pNonLinearScale->m_strGroupName = m_pNonLinearScale->m_strGroupName;

			memset(this->m_szTagName, 0, sizeof(this->m_szTagName));
			strncpy(this->m_szTagName, m_pNonLinearScale->m_szTagName, qMin(MAX_TAGNAME_LEN_SCADASTUDIO, (int)sizeof(this->m_szTagName)));

			return true;
		}

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		//int32u m_nOccNo;				//大排行号
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//主键
		//QString m_strScaleName;	//描述

		//uint m_nID;

		//QString m_strGroupName;		//分组名称
		//QString m_strExtention;		//扩展参数

		int m_nType;		//类型

		//LINEAR_SCALE m_LinearScale;

		//std::vector<NONLINEAR_SCALE *> m_arrPNonliear;

		CScadaLinearTransform *m_pLinearScale;			//线性

		CScadaNonlinearTransform *m_pNonLinearScale;		//非线性

	private:
		int32u m_nOccNo;				//大排行号
	};

	class CScadaTransformGroup : public CBaseData
	{
	public:
		CScadaTransformGroup();
		~CScadaTransformGroup();

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

		CScadaTransformGroup *Find(QString &strName);

		CScadaTransformGroup *CreateGroup();

		bool AddTransformGroup(CScadaTransformGroup *pGroup);
		bool AddTransformItem(CScadaTransform *pItem);

		bool SetGroupName(QString &strGroupName)
		{
			Q_ASSERT(!strGroupName.isEmpty());
			if (strGroupName.isEmpty())
			{
				return false;
			}

			m_strGroupName = strGroupName;

			return true;
		}

		QString &GetGroupName()
		{
			return m_strGroupName;
		}

		std::vector<CScadaTransformGroup *> &GetGroup()
		{
			return m_arrTransformGroup;
		}

		std::vector<CScadaTransform *> &GetItem()
		{
			return m_arrTransformItem;
		}

		virtual int32u GetOccNo() const
		{
			return 0;
		}


		int32u GetItemCount();

		bool CheckModifyGroupNameExist(CScadaTransformGroup *pGroup, QString strGroupName);
		bool DeletGroup(CScadaTransformGroup *pChildGroup);
		bool DeleteItem(CScadaTransform *pChildItem);
	private:
		//组包含的组
		std::vector<CScadaTransformGroup *> m_arrTransformGroup;
		std::vector<CScadaTransform *> m_arrTransformItem;

		QString m_strGroupName;
	};

	class CScadaData :public CBaseData
	{
	public:
		CScadaData(IMainModuleInterface *pCore);
		~CScadaData();

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

		//校验数据
		bool CheckData();

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		bool SaveScadaData(const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//告警
		bool SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//转换
		bool SaveTransform(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveLinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveNonlinear(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//
		bool SaveVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveScadaSystemVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveScadaUserVariable(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveScadaUserVariableChildNode(QXmlStreamWriter &writer, Config::CScadaUserVariableGroup *pGroup, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);


		bool LoadScadaData(QXmlStreamReader &reader);
		bool LoadVariable(QXmlStreamReader &reader);
		bool LoadSystemVarialbeNode(QXmlStreamReader &xml);
		bool LoadUserVarialbeNode(QXmlStreamReader &xml, Config::CScadaUserVariableGroup *pGroup);

		bool LoadLinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadL(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);

		bool LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadNonLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadNl(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool Loadnl(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData);
		bool LoadP(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData);

		bool LoadAINAlarm(QXmlStreamReader &reader, CScadaAnalogAlarmGroup *pGroup);
		bool LoadAINAlarmLimit(QXmlStreamReader &reader, Config::CScadaAnalogAlarm *pAlarm);

		bool LoadDINAlarm(QXmlStreamReader &reader, CScadaDigitalAlarmGroup *pGroup);
		bool LoadDINAlarmLimit(QXmlStreamReader &reader, CScadaDigitalAlarm *pAlarm);

		bool PushTagNameHashMap(int nIDD, const QString &strTagName, CBaseData *pData);
		bool DeleteTagNameHashMap(int nIDD, const QString &strChannelTagnname);
		bool CheckTagNameIsExist(const QString &strTagName);
		bool ModifyHashMap(int32u nIDD, const QString &strTagName, const QString &strLastTagName);

		int32u GetArrCount(int nIDD);
		CScadaUserVariable *AddScadaUserVariable();

		//添加告警
		CScadaAnalogAlarm *AddAnalogAlarm();
		CScadaAinAlarmLimit *AddAnalogAlarmLimit();

		CScadaDigitalAlarm *AddDigitalAlarm();
		CScadaDinAlarmLimit *AddDigitalAlarmLimit();

		CScadaTransform *AddLinearTransform();
		CScadaTransform *AddNonLinearTransform();

		bool AddScadaVariableRelationAlarmArr(std::string &tagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableRelationAlarmArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName);
		bool ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteUserVariableAlarmArr(std::string &tagName);
		bool DeleteUserVariableRelationAlarmArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);

		bool AddScadaVariableRelationScaleArr(std::string &tagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableRelationScaleArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName, int type);
		bool ModifyUserVariableScaleArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		bool DeleteUserVariableScaleArr(std::string &tagName);
		bool DeleteUserVariableRelationScaleArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);


		//SourceTagName关联
		bool AddUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableRelationSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName);
		bool DeleteUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);
		bool DeleteUserVariableSourceTagNameArr(const std::string &szTagName);

		bool DeleteUserVariableCombineRelation(const std::string &szTagName, CScadaUserVariable *pVairalbe);

		//
		bool ChangeTagNameNodify(const QString &stTagName, const QString &strLastTagName, int nType);
	private:
		void InitScadaSystemVariable();

		CBaseData *GetData(QString &strTagName, int nType);

		bool ChangeNodeServerTagNameNodifyScadaVariable(const std::string &stTagName, const std::string &strLastTagName, int nType);

	public:
		//系统变量
		std::vector<CScadaSystemVaraible *> m_arrSystemVariable;
		//用户变量根组
		CScadaUserVariableGroup *m_pScadaUserGroup;

		//模拟量告警组
		CScadaAnalogAlarmGroup *m_pScadaAnalogAlarmGroup;
		CScadaDigitalAlarmGroup *m_pScadaDigitalAlarmGroup;
		//线性转换
		CScadaTransformGroup *m_pScadaLinearGroup;
		//非线性转换
		CScadaTransformGroup *m_pScadaNonLinearGroup;
	public:
		//int32u m_nOccNo;				//大排行号
		//QString m_szTagName;		//主键
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述

		uint m_nID;

		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

		QString m_strScadaVersion;	//scada版本

									//bool m_bModifiedFlag;			//修改标志
	private:
		int32u m_nOccNo;				//大排行号
		IMainModuleInterface *m_pCore;

		using TagNameMap = std::unordered_map<std::string, CBaseData *>;
		TagNameMap m_arrMaps[SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM];

		//tagname对应scada用户变量 hash
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrScaleToUserVariableMap;
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrAlarmToUserVariableMap;
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrSourceTagNameToUserVariableMap;
};

	class CScadaGroup
	{
	public:
		CScadaGroup();
		~CScadaGroup();

		bool CheckModifyGroupNameExist(CScadaGroup *pGroup, QString strGroupName);

		// 创建组
		CScadaGroup* CreateGroup();

		//修改组
		bool ModifyGroup(const QString &strTagName, const QString &strLastTagName);

		//删除组
		bool DeletGroup(CScadaGroup *pGroup, const QString &strTagGroup);
		bool DeletItem(CScadaData *pScada, const QString &strTagGroup);

		//创建前置item
		//CFesData *CreateFesItem();

		//修改项
		bool ModifyItem(const QString &strTagName, const QString &strLastTagName);

		QString m_strGroup;

		//组包含的组
		std::unordered_map<std::string, CScadaGroup *> m_arrGroup;
		std::unordered_map<std::string, CScadaData *> m_arrItem;
	};


	class CScadaVariableConfigData
	{
	public:
		CScadaVariableConfigData();
		~CScadaVariableConfigData();

	public:
		enum SAVE_TYPE
		{
			TO_LOCAL_FILE,
			TO_SQLITE,
			TO_MSSQL,
			TO_MYSQL,
		};

		enum NODE_TYPE
		{
			SERVER = 1, FES, WORKSTATION
		};
	public:
		bool SaveData(int nSaveType)
		{
			switch (nSaveType)
			{
			case TO_LOCAL_FILE:
				SaveDataToFile();
			default:
				break;
			}
		}

		//bool LoadFesFile(CFesData *pFes, const QString &filePath);

		bool Clear();
		bool Create();


		bool CheckModifyScadaTagNameExist(const QString &strTagName);
		bool ModifyScadaHashMap(const QString &strScadaTagName, const QString &strLastScadaTagName);
		bool DeleteScadaHahsMap(const QString &strScadaTagName);

		bool SaveProjectScadavarialbeNode(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveChildNode(QDomDocument *pXml, QDomElement &e, CScadaGroup *pGroup);
		bool SaveChildItem(CScadaGroup *pFesGroup, const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool SaveScadaSystemVariable(QXmlStreamWriter &writer);
		bool SaveScadaUserVariable(QXmlStreamWriter &writer);
		bool SaveScadaUserVariableChildNode(QXmlStreamWriter &writer, Config::CScadaUserVariableGroup *pGroup);

		bool SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo);
		bool SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo);

		//加载工程文件
		bool LoadProjectScadaNode(QDomElement &e, const QString &szRoot);
		bool LoadChildGroup(CScadaGroup *pScadaGroup, QDomElement &e, const QString &szRoot);
		bool LoadScadaItem(CScadaData *pScada, const QString &szRoot);

		bool LoadScadaVariableProjectFile(QString &strFile);
		bool LoadSystemVarialbeNode(QXmlStreamReader &xml);
		bool LoadUserVarialbeNode(QXmlStreamReader &xml, Config::CScadaUserVariableGroup *pGroup);
		bool LoadAINAlarm(QXmlStreamReader &reader, CScadaAnalogAlarmGroup *pGroup);
		bool LoadAINAlarmLimit(QXmlStreamReader &reader, Config::CScadaAnalogAlarm *pAlarm);
		bool LoadDINAlarm(QXmlStreamReader &reader, CScadaDigitalAlarmGroup *pGroup);
		bool LoadDINAlarmLimit(QXmlStreamReader &reader, CScadaDigitalAlarm *pAlarm);
		bool LoadLinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadL(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadNonlinear(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool LoadNl(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);
		bool Loadnl(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData);
		bool LoadP(QXmlStreamReader &reader, Config::CScadaNonlinearTransform *pData);
		bool LoadNonLinearGroupNode(QXmlStreamReader &reader, CScadaTransformGroup *pGroup);

		//添加scada项
		CScadaData *AddNewScadaData(CScadaGroup *pGroup);

		bool SaveTransform(QXmlStreamWriter &writer);
		bool SaveLinear(QXmlStreamWriter &writer);
		bool SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo);

		bool SaveNonlinear(QXmlStreamWriter &writer);
		bool SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo);

		//转换tagname对应的scada用户变量

		//告警tagname对应的scada用户变量
		//bool AddScadaVariableRelationAlarmArr(std::string &tagName, CScadaUserVariable *pVairalbe);
		//bool ModifyUserVariableRelationAlarmArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName);
		//bool ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		//bool DeleteUserVariableAlarmArr(std::string &tagName);



		CBaseData *GetData(QString &strTagName, int nType);

		std::unordered_map<std::string, CScadaData *> &GetScadaArr()
		{
			return m_arrHashTagNameMap;
		}

		//点tagname修改,通知绑定点的模块做出相应的修改 删除tagname为空
		bool ChangeTagNameNodify(QString &tagName, QString &lastTagName, int nType);
	private:
		bool SaveDataToFile()
		{
			//auto i = m_arrNodes.begin();
			//for (; i != m_arrNodes.end(); ++i)
			//{
			//	//	i->
			//}
		}
	public:
		//根组
		CScadaGroup *m_pGroup;
		
		
		//std::vector<CScadaSystemVaraible *> m_arrSystemVariable;	//系统变量

		using TagNameMap = std::unordered_map<std::string, CBaseData *>;

		TagNameMap m_arrMaps[SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM];

		//CScadaUserVariableGroup *m_pScadaUserGroup;

		////模拟量告警组
		//CScadaAnalogAlarmGroup *m_pScadaAnalogAlarmGroup;

		//CScadaDigitalAlarmGroup *m_pScadaDigitalAlarmGroup;

		//线性转换
		//CScadaTransformGroup *m_pScadaLinearGroup;
		//非线性转换
		//CScadaTransformGroup *m_pScadaNonLinearGroup;

		//所有前置map hash前置,查找快速 统计个数,添加前置
		//std::unordered_map<std::string, CFesData *> m_arrHashTagNameMap;


		//tagname对应scada用户变量 hash
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrScaleToUserVariableMap;
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrAlarmToUserVariableMap;

		//所有scada map hash前置,查找快速 统计个数,添加scada
		std::unordered_map<std::string, CScadaData *> m_arrHashTagNameMap;
		std::vector<CScadaData*> m_arrScadaDatas;

};
}

#endif // SCADAVIRABLE_DATA_H

