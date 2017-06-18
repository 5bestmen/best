#ifndef ALARM_CONFIG_DATA_H
#define ALARM_CONFIG_DATA_H

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

namespace Config
{
	//�澯���
	struct ALARM_STYLE
	{
		bool PushPicture;			//��ͼ
		bool Pdr;					//Pdr
		bool PlaySound;				//��������
		bool Blink;					//��˸
		bool LogAlarmWindow;		//��¼�澯��
		bool AccidentHandle;		//�¹ʴ���
		bool AccidengPrint;			//�¹ʴ�ӡ
		bool LightPlate;			//������
	};

	/*! \class CAinAlarmLimit config_data.h  "config_data.h"
	*  \brief ģ����������ֵ
	*  \details  һ�������п��԰��������ֵ��������ޡ��߸���....
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
		// ����������ֽ���У����
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
		QString m_strDescription;		//����

		int32u m_nID;						//ID

		int8u m_nCondition;	//�澯����

		int8u m_nCategory;	//�¹�
							//����
							//��ͨ


		fp64 m_dbValue;
		fp64 m_dbLowValue;
		int32u m_nDelayAlarm;
		int32u m_nPriority;

		bool m_bSupportAck;		//֧��ȷ��
		bool m_bSupportDelete;		//֧��ɾ��

		int8u m_nAckType;		//ȷ�Ϻ���ʾ��ʽ

		QString m_strSoundFile;		//�����ļ�
		int32u m_nPlaySoundTimes;	//���Ŵ���
		QString m_strSpeechAlarmText;


		//int32u ReadAccessLevel;	//16 levels
		//int32u WriteAccessLevel;		//16 levels

		bool m_bBeep;
		QString m_strPushGraph;
		bool m_bBlinkGraph;
		bool m_bLog;

		int32u m_nBackColor;						//����ɫ
		int32u m_nTextColor;						//�ı�ɫ
		int32u m_nBlinkBackColor;					//��˸����ɫ
		int m_nBlinkTextColor;						//��˸�ı�ɫ
		bool m_bLightBoard;							//����������
		QString m_strDispGuide;						//��ʾ����ָ��
		QString m_strCommands;


		//QString TimeFormat;			//ʱ���ʽ

		//0 >=		1 <=		2 =			3 �仯�ʽ���		4�仯�����
		//5������	6 ����


		//int8u AlarmStyle;		//0 �ƻ���	1 �¹�׷��	2 ��������	3 ��˸	4 ��¼������
		////5 �¹ʴ���	6 �¹ʴ�ӡ	7 ������

		//����֪ͨ
		//������
		QString Acceptor;
		//�����ı�
		//�����ʼ�
		//�����绰
		//����
		//����
	private:
		int32u m_nOccNo;					//�����к� ͬһ��ǰ�����и澯��ֵ
		int32u m_nBlockNo;					//С���к� ͬһ���澯�µ���ֵ��
	};

	//ģ�����澯����
	struct ALARM_CONDITION_AI
	{
		float HiHiValue;	//�߸���ֵ
		float HiValue;		//����ֵ
		float LoValue;		//����ֵ
		float LoLoValue;	//�͵���ֵ
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
		//int32u m_nOccNo;				//�����к�
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//tag name
		QString m_strDescription;	//����

		int32u m_nID;					//ID

		QString m_strGroupName;
		bool m_bEnable;				//����
		bool m_bOnQualityGood;		//Ʒ��


		int8u m_nDeadType;			//��������
		fp64 m_dbDeadArea;			//����

		int32u m_nDelayAlarm;		//��ʱ����

		int m_nAlarmType;

		//bool m_bSupportAck;
		//bool m_bSupportDelete;
		//QString m_strSoundFile;
		//QString m_strPlaySoundTimes;

		std::vector<CAinAlarmLimit *> m_arrAlarmOfflimit;		//��ֵvec

	private:
		void Clear();
	private:
		int32u m_nOccNo;				//�����к�
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

		//ֻ�ǽӿ� û��ʵϰ�ô�
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
		//���������
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
		QString Description;		//����

		int32u ID;						//ID

		int8u Condition;			//����
		//0->1 �澯	0 0->1
		//1->0 �澯	1 1->0
		int8u Category;					//�¹�
		//����
		//��ͨ״̬�仯

		int32u DelayAlarm;			//��ʱ����
		int32u Priority;				//���ȼ���

		bool SupportAck;			//֧��ȷ��
		bool SupportDelete;		//֧��ɾ��

		int8u AckType;					//ȷ�Ϻ���ʾ��ʽ

		QString SoundFile;			//�����ļ�
		int32u PlaySoundTimes;		//���Ŵ���

		QString SpeechAlarmText;

		bool Beep;					//������
		QString PushGraph;
		bool BlinkGraph;
		bool Log;

		int32u BackColor;		//����ɫ
		int32u TextColor;			//�ı�ɫ
		int32u BlinkBackColor;		//��˸����ɫ
		int32u BlinkTextColor;					//��˸�ı�ɫ

		QString DispGuide;

		QString Commands;			//����

		//����֪ͨ
		//������
		//QString Acceptor;
		//�����ı�
		//�����ʼ�
		//�����绰
		//����
		//����

		int32u OccNo;					//�����к�
		int32u BlockNo;				//С���к� ͬһ���澯�µ���ֵ��
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
		QString m_strDescription;	//����


		int32u m_nID;					//ID

		QString m_strGroupName;
		bool m_bEnable;				//����
		bool m_bOnQualityGood;		//Ʒ��
		int32u m_nDelayAlarm;		//��ʱ����

		int m_nAlarmType;

		std::vector<CDinAlarmLimit *> m_arrDigtalOfflimit;		//��ֵvec
	private:
		int32u m_nOccNo;				//�����к�

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

		//�ӿ�
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
		//���������
		std::vector<CDigitalAlarmGroup *> m_arrDigitalGroup;
		std::vector<CDigitalAlarm *> m_arrDigitalItem;

		QString m_strGroupName;
	};
}

#endif

