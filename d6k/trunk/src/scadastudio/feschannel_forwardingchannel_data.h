#ifndef FES_FORWARDING_CONF_DATA_H
#define FES_FORWARDING_CONF_DATA_H

#include "datatypes.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "base_data.h"

#include <vector>
#include <QString>
#include <QMap>
#include <unordered_map>
#include <set>

class QXmlStreamWriter;
class CFesData;
class QXmlStreamReader;

class IMainModuleInterface;

namespace Config
{
	class CFesData;
	
	class CAnalogInput :public CBaseData
	{
	public:
		CAnalogInput();
		explicit  CAnalogInput(int32u nOccNo)
		{
			memset(m_szTagName, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD, 0);
			InitDefault();
			Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
			m_nOccNo = nOccNo;
		}

		~CAnalogInput();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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

		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];	//����
		int32u m_nID;				//ID
		int32u m_nBlockOccNo;		//С���к�
									//int m_nDataType;			//��������������
		QString m_strDescription;	//����
		int8u m_nInitialQua;		//��ʼƷ��
		QString m_strAddress;		//����ַ
		QString m_strPinLabel;		//��������

									/*
									ģ������ת����ʽĬ�ϲ��øñ��ڵ�ת������������˲����ⲿ��ת����ʽ��������ⲿ��ת����ʽ�������ݵ�ת����
									*/
									//ת��
		double m_dbInitValue;		//��ʼֵ
		QString m_strFormat;		//��ʽ		x xx xxx xxxx xxxxx
		QString m_strUnit;		//Unit
		bool m_bEnable;
		double m_dbMaxRaw;
		double m_dbMinRaw;
		double m_dbMaxConvert;
		double m_dbMinConvert;
		//Advaneced
		QString m_strScaleTagName;	//ת��tag name
		int32u m_nScaleOccNo;
		int32u m_nScaleType;		//ת������
		QString m_strScaleDesc;		//ת������

									/*
									*LowQua  ͨ��Ϊ - 1.2  HighQua ͨ��Ϊ1.2
									OutputL = RangeH + (RangeH - RangeL) * LowQua; ����Чֵ��Χ��ֵ��
									OutputH = RangeL + (RangeH - RangeL) * HighQua; ����Чֵ��Χ��ֵ��

									����ڴ���Чֵ��Χ�⣬����Ϊ��ֵƷ�ʲ����š�
									ͨ����˵��
									OutputH > RangeH > HH > H

									*/
		double m_dblRangeL;			//���޷�Χ��
		double m_dblRangeH;			//���޷�Χ��
		double m_dblHighQua;
		double m_dblLowQua;

		bool m_bSaveDisk;			//�Ƿ���� (���籣��)	
		int m_nSaveDiskPeriod;	    //��������

		bool m_bSaveDB;				//�Ƿ���
		int32u m_nSaveDBPeriod;		//�������

		double m_dblSensitivity;		//�仯������
		int32u m_nSensitivityType;	//0 ���ֵ			1 �������

		QString m_strAlarmTagName;	//�澯tag name
		int32u m_nAlarmOccNo;
		int32u m_nAlarmType;		//�澯����
		QString m_strAlarmDesc;		//�澯����

		int32u m_nChannelOccNo;		//ͨ�������к�
		int32u m_nDeviceOccNo;			//װ�ô����к�
		int32u m_nReferenceCount;	//���ô���
		int32u m_nDataSource;		//������Դ
		QString m_strExpress;		//���ʽ
									//QString m_strGroupName;		//����

									////////////////////////////////////////////////////////
									// �����ڲ��ã����ñ���
		bool m_bNewPointScale;		//ת���¼ӵ�
		bool m_bNewPointAlarm;		//�澯�¼ӵ�
	protected:
		void InitDefault()
		{

		}
	private:
		int32u m_nOccNo;			//�����к�

	};

	class CDigitalInput :public CBaseData
	{
	public:
		CDigitalInput();
		~CDigitalInput();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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
		char m_szDoubleDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];	//˫λң�ŵĴ�ң��TagName ˫λң�ţ�DDI��������ң���ж����ң�ŵ�TagName����ң���в������塣
		int32u m_nID;					//ID
										//int32u m_nOccNo;				//�����к�
		int32u m_nBlockOccNo;			//С���к�
										//int8u m_nDataType;			//��������������
										//QString m_szTagName;		//tag name
		QString m_strDescription;	//����
		bool m_bInitValue;        //��ʼֵ
		int8u m_nInitialQua;		//��ʼƷ��

		QString m_strAddress;		//����ַ
		QString m_strPinLabel;		//��������

		int32u m_nSignalType;		//�ź����� ���� ����
		bool m_bSOE;				//SOE

		QString m_strBitCloseString;	//λ�պ��ַ���
		QString m_strBitOpenString;		//λ���ַ���
		QString m_strAlarmTag;		//�澯tag name
		int32u m_nAlarmType;		//�澯����
		QString m_strAlarmDesc;		//�澯����
		int32u m_nAlarmOccNo;

		bool m_bSaveDisk;			//�Ƿ����
		int32u m_nSaveDiskPeriod;	//��������
		bool m_bSaveDB;				//�Ƿ���
		int32u m_nSaveDBPeriod;		//�������

		int m_nReferenceCount;		//���ô���
									//QString m_strGroupName;		//����

		int32u m_nDataSource;		//������Դ

		int32u m_nChannelOccNo;		//QString m_strChannelOccNo;
		int32u m_nDeviceOccNo;

		QString m_strExpress;		//���ʽ

		bool m_bNewPointAlarm;
		//bool m_bNewPointScale;
	private:
		int32u m_nOccNo;			//�����к�
	};

	class CAnalogOutput :public CBaseData
	{
	public:
		CAnalogOutput();
		~CAnalogOutput();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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
		int32u m_nID;					//ID
										//int32u m_nOccNo;				//�����к�
		int32u m_nBlockOccNo;				//С���к�
		int8u m_nDataType;			//��������������
		QString m_strDescription;	//����	
		int8u m_nInitialQua;		//��ʼƷ��
		QString m_strAddress;		//����ַ
		QString m_strPinLabel;		//��������

									//PROJECT DATA
									/*
									ģ������ת����ʽĬ�ϲ��øñ��ڵ�ת������������˲����ⲿ��ת����ʽ��������ⲿ��ת����ʽ�������ݵ�ת����
									*/
									//ת��
		double m_dbInitValue;		//��ʼֵ
		QString m_strFormat;		//��ʽ		x xx xxx xxxx xxxxx
		QString m_strUnit;		//Unit
		bool m_bEnable;
		double m_dbMaxRaw;
		double m_dbMinRaw;
		double m_dbMaxConvert;
		double m_dbMinConvert;
		//Advaneced
		QString m_strScaleTagName;	//ת��tag name
		int32u m_nScaleType;		//ת������
		QString m_strScaleDesc;		//ת������

									/*
									*LowQua  ͨ��Ϊ - 1.2  HighQua ͨ��Ϊ1.2
									OutputL = RangeH + (RangeH - RangeL) * LowQua; ����Чֵ��Χ��ֵ��
									OutputH = RangeL + (RangeH - RangeL) * HighQua; ����Чֵ��Χ��ֵ��

									����ڴ���Чֵ��Χ�⣬����Ϊ��ֵƷ�ʲ����š�
									ͨ����˵��
									OutputH > RangeH > HH > H

									*/
		double m_dblRangeL;			//���޷�Χ��
		double m_dblRangeH;			//���޷�Χ��
		double m_dblHighQua;
		double m_dblLowQua;


		//fp64 m_dbSensitivity;		//�仯������
		int32u m_nReferenceCount;	//���ô���

		int32u m_nChannelOccNo;		//ͨ�������к�
		int32u m_nDeviceOccNo;		//װ�ô����к�

		bool m_bNewPointAlarm;		//�¼Ӹ澯��
		bool m_bNewPointScale;
	private:
		int32u m_nOccNo;				//�����к�
	};

	class CDigitalOutput :public CBaseData
	{
	public:
		CDigitalOutput();
		~CDigitalOutput();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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
		uint m_nID;					//ID

		int32u m_nBlockOccNo;			//С���к�

		int32u m_nDataType;		//��������������

		QString m_strDescription;	//����
		bool m_bInitValue;		//��ʼֵ
		int8u m_nInitialQua;		//��ʼƷ��

		QString m_strAddress;		//����ַ
		QString m_strPinLabel;		//��������

		bool m_bIsSBO;				//ѡ��ң��

		double m_dblTelCtlWaitTime;	//ң�ط�Уʱ����
		double m_dblTelCtlSendTime;	//ң�ط���ʱ����
		double m_dblTelCtlExecTime;	//ң��ִ��ʱ����

		char m_szAssociatedDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		int8u m_nAssociatedDIValType;

		//�����ź�����
		int8u m_nBlockSignalType;
		char m_szBlockingSignalTag[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//��������

		QString m_strBlockingProgram;		//���Ʊ�������

		QString m_strStartProgram;		//��������

		int m_nReferenceCount;		//���ô���

		int32u m_nChannelOccNo;		//QString m_strChannelOccNo;
		int32u m_nDeviceOccNo;

		bool m_bNewPointAlarm;		//�澯�¼ӵ�

	private:
		int32u m_nOccNo;				//�����к�
	};

	class CDevice :public CBaseData
	{
	public:
		CDevice(IMainModuleInterface *pCore);
		~CDevice();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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

		void AddAnalogInput(CAnalogInput *pAnalogInput);

		void AddDigitalInput(CDigitalInput *pDigitalInput);

		void AddAnalogOutput(CAnalogOutput *pAnalogOutput);

		void AddDigitalOutput(CDigitalOutput *pDigitalOutput);

		bool LoadData(QXmlStreamReader &reader, CFesData *pFesData);
		//bool ReadData(QXmlStreamReader &reader);
		bool ReadAIN(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadAi(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadDIN(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadDi(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadDOUT(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadDo(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadAOUT(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadAo(QXmlStreamReader &reader, CFesData *pFesData);

		bool SaveDeviceData(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nDOOccNo
			, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nDIOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nDOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool LogToFile(const char* pFilename, const char* pLog);
	private:
		void Clear();

	public:
		//int32u m_nOccNo;					//�����к�
		//QString m_szTagName;			//����
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;		//����

		int32u m_nID;
		int m_nPriority;				//���ȼ���

		QString m_strDeviceAddr;		//װ�õ�ַ
		QString m_strExtentionParam;	//��չ����
		int m_nDeviceModelID;		//װ��ģ��ID

		int8u m_nType;					//װ������
	public:
		std::vector <CAnalogInput *> m_arrAIs;		//AI vec
		std::vector <CDigitalInput *> m_arrDIs;		//DI vec
		std::vector <CAnalogOutput *> m_arrAOs;		//AO vec
		std::vector <CDigitalOutput *> m_arrDOs;		//DO vec
	private:
		int32u m_nOccNo;					//�����к�
		IMainModuleInterface *m_pCore;
	};

	class CChannelData :public CBaseData
	{
	public:
		CChannelData();
		~CChannelData();

		virtual void Save();

		void AddDevice(CDevice *pDevice);
		bool DeleteDevice(CDevice *pDevice);

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

		bool LoadDevicessData(QXmlStreamReader &reader, CFesData *pFesData);
		bool ReadD(QXmlStreamReader &reader);

		bool SaveChannelData(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nDOOccNo,
			CFesData *pFes, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool LogToFile(const char* filename, const char* log);

	public:
		//int32u m_nOccNo;				//�����к�
		//QString m_szTagName;		//����
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//����

		COMM_PARA m_CommPara;
		QString m_strChannelPara;
		uint m_nID;
		//int m_nStation;				//?
		QString m_strGroupName;		//��������
		QString m_strExtention;		//��չ����

	public:
		std::vector <CDevice *> m_arrDevices;			//װ��vec
	private:
		void Clear();
	private:
		int32u m_nOccNo;				//�����к�
	};


	class CChannelGroup
	{
	public:
		CChannelGroup();
		~CChannelGroup();

		CChannelGroup * CreateGroup();

		bool ModifyItem(const QString &strTagName, const QString &strLastTagName);

		CChannelGroup * CreateItem(CChannelGroup *pChannelGroup);

		bool DeleteItem(QString &strLastTagName);

		bool DeleteGroup(QString &strLastTagName);

		bool CheckModifyGroupNameExist(CChannelGroup *pChannelGroup, const QString &strGroupName);

		bool ModifyGroup(const QString &strTagName, const QString &strLastTagName);

		QString m_strGroup;

		//���������
		std::unordered_map<std::string, CChannelGroup *> m_arrChannelGroup;
		std::unordered_map<std::string, CChannelData *> m_arrChannelItem;
	};
	
	//ת��ң��
	class CForwardingAnalogInputData :public CBaseData
	{
	public:
		CForwardingAnalogInputData();
		~CForwardingAnalogInputData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;		//���
		int32u m_nEntryNo;		//��Ŀ��

		QString m_strAddress;	//��ַ

		QString m_strSourceTagName;

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;
		fp64 m_dblMaxRaw;					//y=kx+b
		fp64 m_dblMinRaw;
		fp64 m_dblMaxConvert;
		fp64 m_dblMinConvert;

		QString m_strScaleTagName;
		int m_nScaleType;
		fp64 m_dblHLimit;
		fp64 m_dblLLimit;
		fp64 m_dblThreshold;		//���������ֵ������

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//�����к�

	};

	class CForwardingDigitalInputData :public CBaseData
	{
	public:
		CForwardingDigitalInputData();
		~CForwardingDigitalInputData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;
		int32u m_nEntryNo;

		QString m_strAddress;

		QString m_strSourceTagName;

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;
		bool m_bInverse;				//ȡ��

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//�����к�

	};

	class CForwardingDigitalOutputData :public CBaseData
	{
	public:
		CForwardingDigitalOutputData();
		~CForwardingDigitalOutputData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

	public:
		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;
		int32u m_nEntryNo;

		QString m_strAddress;

		QString m_strSourceTagName;

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;

		char m_szAssociatedDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		char m_szBlockingSignalTag[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		bool m_bBlockingSignalCondition;

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//�����к�
	};

	class CForwardingAnalogOutputData :public CBaseData
	{
	public:
		CForwardingAnalogOutputData();
		~CForwardingAnalogOutputData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;		//���
		int32u m_nEntryNo;		//��Ŀ��

		QString m_strAddress;	//��ַ

		QString m_strSourceTagName;

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;

		fp64 m_dbMaxRaw;					//y=kx+b
		fp64 m_dbMinRaw;
		fp64 m_dbMaxConvert;
		fp64 m_dbMinConvert;

		QString m_strScaleTagName;
		int m_nScaleType;

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//�����к�
	};

	//����
	class CForwardingControlGearData :public CBaseData
	{
	public:
		CForwardingControlGearData();
		~CForwardingControlGearData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;		//���
		int32u m_nEntryNo;		//��Ŀ��

		QString m_strAddress;	//��ַ

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;

		QString m_strSourceTagName;

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//�����к�
	};

	class CForwardingKWHData :public CBaseData
	{
	public:
		CForwardingKWHData();
		~CForwardingKWHData();

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

		int32u GetOccNo() const
		{
			return m_nOccNo;
		}

		void SetOccNo(int32u nOccNo)
		{
			m_nOccNo = nOccNo;
		}

		int32u m_nID;
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];

		int32u m_nBlockNo;

		int32u m_nGroupNo;		//���
		int32u m_nEntryNo;		//��Ŀ��

		QString m_strAddress;	//��ַ

		QString m_strDescription;	//����
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//���� ������
		QString m_strInfo;

		QString m_strSourceTagName;

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//�����к�
	};

	class CForwardingChannelData : public CBaseData
	{
	public:
		CForwardingChannelData();
		~CForwardingChannelData();

		virtual void Save();

		virtual void SetModifyFlag(bool bFlag)
		{
			m_bModifyFlag = bFlag;
		}

		virtual bool GetModifyFlag() const
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

		bool SaveForwardingChannelData(QXmlStreamWriter &writer, int &nOccNoChannel, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nCGOccNo, int &nDOOccNo, int &nKwhOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nAIOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nDIOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveAO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nAOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveCG(QXmlStreamWriter &writer, int &nOccNoChannel, int &nCGOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveDO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nDOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
		bool SaveKwh(QXmlStreamWriter &writer, int &nOccNoChannel, int &nKwhOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);

		bool LoadData(QXmlStreamReader & reader, CFesData * pFes);
		//ת��ң��
		bool ReadAin(QXmlStreamReader &reader, CFesData *pFesData);
		//ת��ң��
		bool ReadDin(QXmlStreamReader &reader, CFesData *pFesData);
		//ת��ң��
		bool ReadAout(QXmlStreamReader &reader, CFesData *pFesData);
		//ת��ң��
		bool ReadCgout(QXmlStreamReader &reader, CFesData *pFesData);
		//ת��ң��
		bool ReadDout(QXmlStreamReader &reader, CFesData *pFesData);
		//ת�����
		bool ReadKwh(QXmlStreamReader &reader, CFesData *pFesData);

	public:
		//int32u m_nOccNo;				//�����к�
		//QString m_szTagName;		//����
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//����

		COMM_PARA m_CommPara;
		QString m_strChannelPara;
		uint m_nID;
		//int m_nStation;				//?
		QString m_strGroupName;		//��������
		QString m_strExtention;		//��չ����

	public:
		std::vector <CForwardingAnalogInputData *> m_arrAIs;
		std::vector <CForwardingDigitalInputData *> m_arrDIs;
		std::vector <CForwardingDigitalOutputData *> m_arrDOs;
		std::vector <CForwardingAnalogOutputData *> m_arrAOs;
		std::vector <CForwardingControlGearData *> m_arrCGs;
		std::vector <CForwardingKWHData *> m_arrKWHs;
	private:
		void Clear();
	private:
		int32u m_nOccNo;				//�����к�
	};

	class CForwardingChannelGroup : public CBaseData
	{
	public:
		CForwardingChannelGroup();
		~CForwardingChannelGroup();

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
			return 0;
		}

		CForwardingChannelGroup *Find(QString &strName);

		CForwardingChannelGroup *CreateGroup();

		bool AddForwardingGroup(CForwardingChannelGroup *pGroup);
		bool ModifyItem(const QString &strTagName, const QString &strLastTagName);

		bool AddForwardingItem(CForwardingChannelData *pItem);

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

		std::unordered_map<std::string, CForwardingChannelGroup *> &GetGroup()
		{
			return m_arrGroup;
		}

		std::unordered_map<std::string, CForwardingChannelData *> &GetItem()
		{
			return m_arrItem;
		}

		bool AddItem(std::string &strTagName, CForwardingChannelData *pItem);


		int32u GetItemCount();

		bool CheckModifyGroupNameExist(CForwardingChannelGroup *pGroup, QString strGroupName);
		bool DeletGroup(const QString &strTagName);
		bool DeleteItem(const QString &strTagName);
	public:
		//���������
		std::unordered_map<std::string, CForwardingChannelGroup *> m_arrGroup;
		std::unordered_map<std::string, CForwardingChannelData *> m_arrItem;

		QString m_strGroupName;
	};

}


#endif

