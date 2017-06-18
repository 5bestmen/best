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

		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];	//主键
		int32u m_nID;				//ID
		int32u m_nBlockOccNo;		//小排行号
									//int m_nDataType;			//变量的数据类型
		QString m_strDescription;	//描述
		int8u m_nInitialQua;		//初始品质
		QString m_strAddress;		//测点地址
		QString m_strPinLabel;		//端子名称

									/*
									模拟量的转换方式默认采用该表内的转换，如果定义了采用外部的转换方式，则采用外部的转换方式进行数据的转换。
									*/
									//转换
		double m_dbInitValue;		//初始值
		QString m_strFormat;		//格式		x xx xxx xxxx xxxxx
		QString m_strUnit;		//Unit
		bool m_bEnable;
		double m_dbMaxRaw;
		double m_dbMinRaw;
		double m_dbMaxConvert;
		double m_dbMinConvert;
		//Advaneced
		QString m_strScaleTagName;	//转换tag name
		int32u m_nScaleOccNo;
		int32u m_nScaleType;		//转换类型
		QString m_strScaleDesc;		//转换描述

									/*
									*LowQua  通常为 - 1.2  HighQua 通常为1.2
									OutputL = RangeH + (RangeH - RangeL) * LowQua; （有效值范围低值）
									OutputH = RangeL + (RangeH - RangeL) * HighQua; （有效值范围高值）

									如果在此有效值范围外，则认为测值品质不可信。
									通常来说：
									OutputH > RangeH > HH > H

									*/
		double m_dblRangeL;			//有限范围低
		double m_dblRangeH;			//有限范围高
		double m_dblHighQua;
		double m_dblLowQua;

		bool m_bSaveDisk;			//是否存盘 (掉电保存)	
		int m_nSaveDiskPeriod;	    //存盘周期

		bool m_bSaveDB;				//是否存库
		int32u m_nSaveDBPeriod;		//存库周期

		double m_dblSensitivity;		//变化灵敏度
		int32u m_nSensitivityType;	//0 相对值			1 相对量程

		QString m_strAlarmTagName;	//告警tag name
		int32u m_nAlarmOccNo;
		int32u m_nAlarmType;		//告警类型
		QString m_strAlarmDesc;		//告警描述

		int32u m_nChannelOccNo;		//通道大排行号
		int32u m_nDeviceOccNo;			//装置大排行号
		int32u m_nReferenceCount;	//引用次数
		int32u m_nDataSource;		//数据来源
		QString m_strExpress;		//表达式
									//QString m_strGroupName;		//组名

									////////////////////////////////////////////////////////
									// 程序内部用，不用保存
		bool m_bNewPointScale;		//转换新加点
		bool m_bNewPointAlarm;		//告警新加点
	protected:
		void InitDefault()
		{

		}
	private:
		int32u m_nOccNo;			//大排行号

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
		char m_szDoubleDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];	//双位遥信的次遥信TagName 双位遥信（DDI），在主遥信中定义次遥信的TagName，次遥信中不做定义。
		int32u m_nID;					//ID
										//int32u m_nOccNo;				//大排行号
		int32u m_nBlockOccNo;			//小排行号
										//int8u m_nDataType;			//变量的数据类型
										//QString m_szTagName;		//tag name
		QString m_strDescription;	//描述
		bool m_bInitValue;        //初始值
		int8u m_nInitialQua;		//初始品质

		QString m_strAddress;		//测点地址
		QString m_strPinLabel;		//端子名称

		int32u m_nSignalType;		//信号类型 常开 常闭
		bool m_bSOE;				//SOE

		QString m_strBitCloseString;	//位闭合字符串
		QString m_strBitOpenString;		//位打开字符串
		QString m_strAlarmTag;		//告警tag name
		int32u m_nAlarmType;		//告警类型
		QString m_strAlarmDesc;		//告警描述
		int32u m_nAlarmOccNo;

		bool m_bSaveDisk;			//是否存盘
		int32u m_nSaveDiskPeriod;	//存盘周期
		bool m_bSaveDB;				//是否存库
		int32u m_nSaveDBPeriod;		//存库周期

		int m_nReferenceCount;		//引用次数
									//QString m_strGroupName;		//分组

		int32u m_nDataSource;		//数据来源

		int32u m_nChannelOccNo;		//QString m_strChannelOccNo;
		int32u m_nDeviceOccNo;

		QString m_strExpress;		//表达式

		bool m_bNewPointAlarm;
		//bool m_bNewPointScale;
	private:
		int32u m_nOccNo;			//大排行号
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
										//int32u m_nOccNo;				//大排行号
		int32u m_nBlockOccNo;				//小排行号
		int8u m_nDataType;			//变量的数据类型
		QString m_strDescription;	//描述	
		int8u m_nInitialQua;		//初始品质
		QString m_strAddress;		//测点地址
		QString m_strPinLabel;		//端子名称

									//PROJECT DATA
									/*
									模拟量的转换方式默认采用该表内的转换，如果定义了采用外部的转换方式，则采用外部的转换方式进行数据的转换。
									*/
									//转换
		double m_dbInitValue;		//初始值
		QString m_strFormat;		//格式		x xx xxx xxxx xxxxx
		QString m_strUnit;		//Unit
		bool m_bEnable;
		double m_dbMaxRaw;
		double m_dbMinRaw;
		double m_dbMaxConvert;
		double m_dbMinConvert;
		//Advaneced
		QString m_strScaleTagName;	//转换tag name
		int32u m_nScaleType;		//转换类型
		QString m_strScaleDesc;		//转换描述

									/*
									*LowQua  通常为 - 1.2  HighQua 通常为1.2
									OutputL = RangeH + (RangeH - RangeL) * LowQua; （有效值范围低值）
									OutputH = RangeL + (RangeH - RangeL) * HighQua; （有效值范围高值）

									如果在此有效值范围外，则认为测值品质不可信。
									通常来说：
									OutputH > RangeH > HH > H

									*/
		double m_dblRangeL;			//有限范围低
		double m_dblRangeH;			//有限范围高
		double m_dblHighQua;
		double m_dblLowQua;


		//fp64 m_dbSensitivity;		//变化灵敏度
		int32u m_nReferenceCount;	//引用次数

		int32u m_nChannelOccNo;		//通道大排行号
		int32u m_nDeviceOccNo;		//装置大排行号

		bool m_bNewPointAlarm;		//新加告警点
		bool m_bNewPointScale;
	private:
		int32u m_nOccNo;				//大排行号
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

		int32u m_nBlockOccNo;			//小排行号

		int32u m_nDataType;		//变量的数据类型

		QString m_strDescription;	//描述
		bool m_bInitValue;		//初始值
		int8u m_nInitialQua;		//初始品质

		QString m_strAddress;		//测点地址
		QString m_strPinLabel;		//端子名称

		bool m_bIsSBO;				//选择遥控

		double m_dblTelCtlWaitTime;	//遥控返校时间限
		double m_dblTelCtlSendTime;	//遥控发令时间限
		double m_dblTelCtlExecTime;	//遥控执行时间限

		char m_szAssociatedDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		int8u m_nAssociatedDIValType;

		//闭锁信号类型
		int8u m_nBlockSignalType;
		char m_szBlockingSignalTag[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];		//闭锁开入

		QString m_strBlockingProgram;		//控制闭锁程序

		QString m_strStartProgram;		//启动程序

		int m_nReferenceCount;		//引用次数

		int32u m_nChannelOccNo;		//QString m_strChannelOccNo;
		int32u m_nDeviceOccNo;

		bool m_bNewPointAlarm;		//告警新加点

	private:
		int32u m_nOccNo;				//大排行号
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
		bool SaveDO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nDOOccNo, std::unordered_map<std::string, int32u> *pHash
			, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo);
	private:
		void Clear();

	public:
		//int32u m_nOccNo;					//大排行号
		//QString m_szTagName;			//主键
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;		//描述

		int32u m_nID;
		int m_nPriority;				//优先级别

		QString m_strDeviceAddr;		//装置地址
		QString m_strExtentionParam;	//扩展参数
		int m_nDeviceModelID;		//装置模板ID

		int8u m_nType;					//装置类型
	public:
		std::vector <CAnalogInput *> m_arrAIs;		//AI vec
		std::vector <CDigitalInput *> m_arrDIs;		//DI vec
		std::vector <CAnalogOutput *> m_arrAOs;		//AO vec
		std::vector <CDigitalOutput *> m_arrDOs;		//DO vec
	private:
		int32u m_nOccNo;					//大排行号
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

	public:
		//int32u m_nOccNo;				//大排行号
		//QString m_szTagName;		//主键
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述

		COMM_PARA m_CommPara;
		QString m_strChannelPara;
		uint m_nID;
		//int m_nStation;				//?
		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

	public:
		std::vector <CDevice *> m_arrDevices;			//装置vec
	private:
		void Clear();
	private:
		int32u m_nOccNo;				//大排行号
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

		//组包含的组
		std::unordered_map<std::string, CChannelGroup *> m_arrChannelGroup;
		std::unordered_map<std::string, CChannelData *> m_arrChannelItem;
	};
	
	//转发遥测
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

		int32u m_nGroupNo;		//组号
		int32u m_nEntryNo;		//条目号

		QString m_strAddress;	//地址

		QString m_strSourceTagName;

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;
		fp64 m_dblMaxRaw;					//y=kx+b
		fp64 m_dblMinRaw;
		fp64 m_dblMaxConvert;
		fp64 m_dblMinConvert;

		QString m_strScaleTagName;
		int m_nScaleType;
		fp64 m_dblHLimit;
		fp64 m_dblLLimit;
		fp64 m_dblThreshold;		//不超过这个值不上送

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//大排行号

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

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;
		bool m_bInverse;				//取反

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//大排行号

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

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;

		char m_szAssociatedDI[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		char m_szBlockingSignalTag[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		bool m_bBlockingSignalCondition;

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//大排行号
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

		int32u m_nGroupNo;		//组号
		int32u m_nEntryNo;		//条目号

		QString m_strAddress;	//地址

		QString m_strSourceTagName;

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;

		fp64 m_dbMaxRaw;					//y=kx+b
		fp64 m_dbMinRaw;
		fp64 m_dbMaxConvert;
		fp64 m_dbMinConvert;

		QString m_strScaleTagName;
		int m_nScaleType;

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//大排行号
	};

	//调档
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

		int32u m_nGroupNo;		//组号
		int32u m_nEntryNo;		//条目号

		QString m_strAddress;	//地址

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;

		QString m_strSourceTagName;

		int32u m_nChannelOccNo;

	private:
		int32u m_nOccNo;				//大排行号
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

		int32u m_nGroupNo;		//组号
		int32u m_nEntryNo;		//条目号

		QString m_strAddress;	//地址

		QString m_strDescription;	//描述
		int32u m_nDescriptionOccNo;
		QString m_strFUN;			//串口 功能码
		QString m_strInfo;

		QString m_strSourceTagName;

		int32u m_nChannelOccNo;
	private:
		int32u m_nOccNo;				//大排行号
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
		//转发遥测
		bool ReadAin(QXmlStreamReader &reader, CFesData *pFesData);
		//转发遥信
		bool ReadDin(QXmlStreamReader &reader, CFesData *pFesData);
		//转发遥调
		bool ReadAout(QXmlStreamReader &reader, CFesData *pFesData);
		//转发遥调
		bool ReadCgout(QXmlStreamReader &reader, CFesData *pFesData);
		//转发遥控
		bool ReadDout(QXmlStreamReader &reader, CFesData *pFesData);
		//转发电度
		bool ReadKwh(QXmlStreamReader &reader, CFesData *pFesData);

	public:
		//int32u m_nOccNo;				//大排行号
		//QString m_szTagName;		//主键
		char m_szTagName[MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD];
		QString m_strDescription;	//描述

		COMM_PARA m_CommPara;
		QString m_strChannelPara;
		uint m_nID;
		//int m_nStation;				//?
		QString m_strGroupName;		//分组名称
		QString m_strExtention;		//扩展参数

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
		int32u m_nOccNo;				//大排行号
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
		//组包含的组
		std::unordered_map<std::string, CForwardingChannelGroup *> m_arrGroup;
		std::unordered_map<std::string, CForwardingChannelData *> m_arrItem;

		QString m_strGroupName;
	};

}


#endif

