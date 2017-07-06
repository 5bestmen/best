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
		QString m_strDescription;	//����
		int32u m_nID;
		int32u m_nType;			//��������

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
		QString m_strDescription;	//����
		int32u m_nID;
		int32u m_nType;			//��������
		/*
		enum DATA_TYPE
		{
			DT_NULL = 0,
			DT_BOOLEAN = 1,	  //!< ������8λ��ȱʡֵ0
			DT_CHAR = 2,      //!< �з����ֽڣ�8λ��
			DT_BYTE = 3,      //!< �޷����ֽڣ�8λ��
			DT_SHORT = 4,     //!< �޷��Ŷ����� ��16λ��
			DT_WORD = 5,      //!< �޷������� ��16λ
			DT_INT = 6,       //!< �з��ų����� 32λ
			DT_DWORD = 7,      //!< �޷��ų����� 32λ
			DT_LLONG = 8,      //!< �з��ų�����  64λ
			DT_LWORD = 9,      //!< �޷��ų�����  64λ
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
		QString m_strSourceOccNo;

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
		//���������
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

		std::vector<CScadaAinAlarmLimit *> m_arrAlarmOfflimit;		//��ֵvec

	private:
		void Clear();
	private:
		int32u m_nOccNo;				//�����к�
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

		//�ӿ�
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
		//���������
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

		//�ӿ�
		virtual int32u GetOccNo() const
		{
			return 0;
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

		int32u BlockNo;				//С���к� ͬһ���澯�µ���ֵ��

		//����֪ͨ
		//������
		//QString Acceptor;
		//�����ı�
		//�����ʼ�
		//�����绰
		//����
		//����
		void SetOccNo(int nOccNo)
		{
			OccNo = nOccNo;
		}

		int32u GetOccNo()
		{
			return OccNo;
		}
	private:

		int32u OccNo;					//�����к�

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
		QString m_strDescription;	//����


		int32u m_nID;					//ID

		QString m_strGroupName;
		bool m_bEnable;				//����
		bool m_bOnQualityGood;		//Ʒ��
		int32u m_nDelayAlarm;		//��ʱ����

		int m_nAlarmType;

		std::vector<CScadaDinAlarmLimit *> m_arrDigtallimit;		//��ֵvec
	private:
		int32u m_nOccNo;				//�����к�

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
		//���������
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
		//uint m_nOccNo;				//�����к�
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//����
		QString m_strDescription;	//����

		uint m_nID;

		QString m_strGroupName;		//��������
		QString m_strExtention;		//��չ����

		bool m_bEnableInverse;	//�Ƿ���÷���
		fp64 m_dbMaxRaw;			//���ԭʼֵ
		fp64 m_dbMinRaw;			//��Сԭʼֵ
		fp64 m_dbMaxScale;			//���ֵ
		fp64 m_dbMinScale;			//��Сֵ
		//fp64 m_dbInverse;
		bool m_bEnableFactor;		//��������
		bool m_bEnableInverseFactor;		//ȡ������



		fp64 m_dbGain;				//����	
		fp64 m_dbOffset;			//ƫ��

		int m_nType;		//����
	private:
		//uint m_nOccNo;				//�����к�
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
		//uint m_nOccNo;				//�����к�
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//����
		QString m_strDescription;	//����

		uint m_nID;

		QString m_strGroupName;		//��������
		QString m_strExtention;		//��չ����

		int m_nType;		//����

		std::vector<SCADA_NONLINEAR_SCALE *> m_arrPNonliear;

	private:
		void Clear();
	private:
		//uint m_nOccNo;				//�����к�
	};

	//ת��
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
			//����ת��Ϊ������
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
			//������ת��Ϊ����
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
		//int32u m_nOccNo;				//�����к�
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//����
		//QString m_strScaleName;	//����

		//uint m_nID;

		//QString m_strGroupName;		//��������
		//QString m_strExtention;		//��չ����

		int m_nType;		//����

		//LINEAR_SCALE m_LinearScale;

		//std::vector<NONLINEAR_SCALE *> m_arrPNonliear;

		CScadaLinearTransform *m_pLinearScale;			//����

		CScadaNonlinearTransform *m_pNonLinearScale;		//������

	private:
		int32u m_nOccNo;				//�����к�
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
		//���������
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

		//У������
		bool CheckData();

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		bool SaveScadaData(const QString &szRoot, int nOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//�澯
		bool SaveAlarm(QXmlStreamWriter &writer, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAnalogAlarm(QXmlStreamWriter &writer, CScadaAnalogAlarmGroup *pGroup, int &nAanalogAlarmOccNo, int &nAanalogAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDigtalAlarm(QXmlStreamWriter &writer, CScadaDigitalAlarmGroup *pGroup, int &nDigitalAlarmOccNo, int &nDigitalAlarmLimitOccNo
			, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		//ת��
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

		//��Ӹ澯
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


		//SourceTagName����
		bool AddUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableRelationSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName, CScadaUserVariable *pVairalbe);
		bool ModifyUserVariableSourceTagNameArr(const std::string &szTagName, const std::string &szLastTagName);
		bool DeleteUserVariableRelationSourceTagNameArr(const std::string &szTagName, CScadaUserVariable *pVairalbe);
		bool DeleteUserVariableSourceTagNameArr(const std::string &szTagName);

		bool DeleteUserVariableCombineRelation(const std::string &szTagName, CScadaUserVariable *pVairalbe);

		//
		bool ChangeTagNameNodify(const QString &stTagName, const QString &strLastTagName, int nType);

		bool GetUserVariableRelatedSourceOccNo(QString &strTagName, QString &strRelatedTagName, QString &strOutput);
	private:
		void InitScadaSystemVariable();

		CBaseData *GetData(QString &strTagName, int nType);

		bool ChangeNodeServerTagNameNodifyScadaVariable(const std::string &stTagName, const std::string &strLastTagName, int nType);

	public:
		//ϵͳ����
		std::vector<CScadaSystemVaraible *> m_arrSystemVariable;
		//�û���������
		CScadaUserVariableGroup *m_pScadaUserGroup;

		//ģ�����澯��
		CScadaAnalogAlarmGroup *m_pScadaAnalogAlarmGroup;
		CScadaDigitalAlarmGroup *m_pScadaDigitalAlarmGroup;
		//����ת��
		CScadaTransformGroup *m_pScadaLinearGroup;
		//������ת��
		CScadaTransformGroup *m_pScadaNonLinearGroup;
	public:
		//int32u m_nOccNo;				//�����к�
		//QString m_szTagName;		//����
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//����

		uint m_nID;

		QString m_strGroupName;		//��������
		QString m_strExtention;		//��չ����

		QString m_strScadaVersion;	//scada�汾

									//bool m_bModifiedFlag;			//�޸ı�־
	private:
		int32u m_nOccNo;				//�����к�
		IMainModuleInterface *m_pCore;

		using TagNameMap = std::unordered_map<std::string, CBaseData *>;
		TagNameMap m_arrMaps[SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM];

		//tagname��Ӧscada�û����� hash
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

		// ������
		CScadaGroup* CreateGroup();

		//�޸���
		bool ModifyGroup(const QString &strTagName, const QString &strLastTagName);

		//ɾ����
		bool DeletGroup(CScadaGroup *pGroup, const QString &strTagGroup);
		bool DeletItem(CScadaData *pScada, const QString &strTagGroup);

		//����ǰ��item
		//CFesData *CreateFesItem();

		//�޸���
		bool ModifyItem(const QString &strTagName, const QString &strLastTagName);

		QString m_strGroup;

		//���������
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

		//���ع����ļ�
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

		//���scada��
		CScadaData *AddNewScadaData(CScadaGroup *pGroup);

		bool SaveTransform(QXmlStreamWriter &writer);
		bool SaveLinear(QXmlStreamWriter &writer);
		bool SaveLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo);

		bool SaveNonlinear(QXmlStreamWriter &writer);
		bool SaveNonLinearTransformChildNode(QXmlStreamWriter &writer, Config::CScadaTransformGroup *pGroup, int &nOccNo);

		//ת��tagname��Ӧ��scada�û�����

		//�澯tagname��Ӧ��scada�û�����
		//bool AddScadaVariableRelationAlarmArr(std::string &tagName, CScadaUserVariable *pVairalbe);
		//bool ModifyUserVariableRelationAlarmArr(CScadaUserVariable *pVairalbe, std::string &tagName, std::string &lastTagName);
		//bool ModifyUserVariableAlarmArr(std::string &tagName, std::string &lastTagName, std::string &desc, int type);
		//bool DeleteUserVariableAlarmArr(std::string &tagName);



		CBaseData *GetData(QString &strTagName, int nType);

		std::unordered_map<std::string, CScadaData *> &GetScadaArr()
		{
			return m_arrHashTagNameMap;
		}

		//��tagname�޸�,֪ͨ�󶨵��ģ��������Ӧ���޸� ɾ��tagnameΪ��
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
		//����
		CScadaGroup *m_pGroup;
		
		
		//std::vector<CScadaSystemVaraible *> m_arrSystemVariable;	//ϵͳ����

		using TagNameMap = std::unordered_map<std::string, CBaseData *>;

		TagNameMap m_arrMaps[SCADA_VARIABLE_HASH_TYPE_NUM::MAX_NUM];

		//CScadaUserVariableGroup *m_pScadaUserGroup;

		////ģ�����澯��
		//CScadaAnalogAlarmGroup *m_pScadaAnalogAlarmGroup;

		//CScadaDigitalAlarmGroup *m_pScadaDigitalAlarmGroup;

		//����ת��
		//CScadaTransformGroup *m_pScadaLinearGroup;
		//������ת��
		//CScadaTransformGroup *m_pScadaNonLinearGroup;

		//����ǰ��map hashǰ��,���ҿ��� ͳ�Ƹ���,���ǰ��
		//std::unordered_map<std::string, CFesData *> m_arrHashTagNameMap;


		//tagname��Ӧscada�û����� hash
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrScaleToUserVariableMap;
		std::unordered_map<std::string, std::vector<CScadaUserVariable *>> m_arrAlarmToUserVariableMap;

		//����scada map hashǰ��,���ҿ��� ͳ�Ƹ���,���scada
		std::unordered_map<std::string, CScadaData *> m_arrHashTagNameMap;
		std::vector<CScadaData*> m_arrScadaDatas;

};
}

#endif // SCADAVIRABLE_DATA_H

