#ifndef  PREDICT_DATAINFO_H
#define  PREDICT_DATAINFO_H

#include "fesapi/fescnst.h"

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>
#include <QDomDocument>
#include <QDomElement>
#include <QXmlStreamWriter>


/*! \class  CPPPointInfo
*   \brief ��������Ϣ */
class CPPPointInfo
{
public:	
	CPPPointInfo()
	{

	}
	~CPPPointInfo()
	{

	}
public:
	int m_nID;
	QString m_szName;
	QString m_szLinkedTagName; //! ʵʱ��Ĳ��TAGNAME
	int m_nType; //! ��������ģ����
};
/*! \class  CInverterPointInfo
*   \brief �����������Ϣ */
class CInverterInfo
{
public:
	enum InverterType
	{
		Inverter_Active_Power = 1,
		Inverter_Reactive_Power,
		Inverter_Open_State,
	};
	CInverterInfo()
	{
		m_ActPower.m_szName = QObject::tr("Inverter ActivePower");
		m_ActPower.m_nType = IDD_AIN;
		m_ActPower.m_nID = Inverter_Active_Power;
		m_ReactPower.m_szName = QObject::tr("Inverter ReActivePower");
		m_ReactPower.m_nType = IDD_AIN;
		m_ReactPower.m_nID = Inverter_Reactive_Power;
		m_OpenState.m_szName = QObject::tr("Inverter Open Close State");
		m_OpenState.m_nType = IDD_DIN;
		m_OpenState.m_nID = Inverter_Open_State;
		Init();
	}
	~CInverterInfo()
	{

	}
	void Init();
	bool SaveADIData(QXmlStreamWriter& writer);
	bool SaveInverterData(QXmlStreamWriter& writer);

	bool LoadData(QXmlStreamReader& reader, CInverterInfo* pInvterInfo);
	bool ReadADIN(QXmlStreamReader& reader, CInverterInfo* pInvterInfo);
	bool ReadAi(QXmlStreamReader& reader, CInverterInfo* pInvterInfo);
	bool ReadDi(QXmlStreamReader& reader, CInverterInfo* pInvterInfo);
	QString & GetName()
	{
		return m_szName;
	}

public:
	int m_nID;
	QString m_szName; //! �����������
	CPPPointInfo  m_OpenState; //! ������ֺ�״̬��
	CPPPointInfo  m_ActPower;  //! ������й�
	CPPPointInfo  m_ReactPower; //! ������޹�
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CPlantInfo
*   \brief ȫ���Ļ�����Ϣ */
class CPlantInfo
{
public:
	enum PlantType
	{
		ActivePower = 1,
		ReactivePower,
	};

	CPlantInfo()
	{
		m_ActPower.m_szName = QObject::tr("Plant Active Power");
		m_ActPower.m_nType = IDD_AIN;
		m_ActPower.m_nID = ActivePower;
		m_ReactPower.m_szName = QObject::tr("Plant Reactive Power");
		m_ReactPower.m_nType = IDD_AIN;
		m_ReactPower.m_nID = ReactivePower;
		Init();
	}

	CPlantInfo(CPlantInfo &pInfo)
	{
		this->m_ActPower.m_nID = pInfo.m_ActPower.m_nID;
		this->m_ActPower.m_szName = pInfo.m_ActPower.m_szName;
		this->m_ActPower.m_szLinkedTagName = pInfo.m_ActPower.m_szLinkedTagName;
		this->m_ActPower.m_nType = pInfo.m_ActPower.m_nType;
	}

	~CPlantInfo()
	{

	}
	void Init();
	bool SaveADIData(QXmlStreamWriter& writer);
	bool SavePlantData(QXmlStreamWriter& writer);
	bool LoadData(QXmlStreamReader& reader, CPlantInfo* pPlntInfo);
	bool ReadADIN(QXmlStreamReader& reader, CPlantInfo* pPlntInfo);
	bool ReadAi(QXmlStreamReader& reader, CPlantInfo* pPlntInfo);

	
	QString & GetName()
	{
		return m_strName;
	}
public:
	CPPPointInfo  m_ActPower;  //! ȫ���й�
	CPPPointInfo  m_ReactPower; //! ȫ���޹�
	QString m_strName;          //! ��վ����
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CInverterGroup
*   \brief ĳһ�糧�µ������������Ϣ */
class CInverterGroup
{
public:
	CInverterGroup()
	{
		m_strName = "Inverter Groups";
	}
	~CInverterGroup()
	{
		Reset();
	}
public:
	//! ���һ�������
	CInverterInfo* AddInverter();
	//! ɾ��һ�������
	bool DeleteInverter(CInverterInfo *pInverter);
	void Reset()
	{
		// todo �Ȼ�����Դ

		m_arrInverters.clear();
	}
	bool SaveInverterGrp(QXmlStreamWriter& writer, CInverterGroup& invertGrp);

	bool LoadInvertersGrp(QXmlStreamReader& reader);

	QString & GetName()
	{
		return m_strName;
	}

public:	
	QVector<CInverterInfo > m_arrInverters;
	QMap<QString, CInverterInfo* > m_mapInverters;
	QString m_strName;                  //!���������
	int m_nCount;
};
/*! \class  CWeatherData
*   \brief ĳһ�糧����������Ϣ */
class CWeatherData
{
public:

	enum WeatherTpye
	{
		TotalRadiation = 1,
		DirectRadiation,
		ScattRadiation,
		AvergWindDirect,
		AvergWindSpeed,
		AirTemperature,
		RelativeHumdty,
		AirPressure,
	};

	CWeatherData()
	{
		m_TotalRadiation.m_szName = QObject::tr("Weather Meter Total Radiation");
		m_TotalRadiation.m_nType = IDD_AIN;
		m_TotalRadiation.m_nID = TotalRadiation;

		m_DirectRadiation.m_szName = QObject::tr("Weather Meter Direct Radiation");
		m_DirectRadiation.m_nType = IDD_AIN;
		m_DirectRadiation.m_nID = DirectRadiation;

		m_ScattRadiation.m_szName = QObject::tr("Weather Meter Scattering Radiation");
		m_ScattRadiation.m_nType = IDD_AIN;
		m_ScattRadiation.m_nID = ScattRadiation;

		m_AvergWindDirect.m_szName = QObject::tr("Weather Meter Average Wind Direction");
		m_AvergWindDirect.m_nType = IDD_AIN;
		m_AvergWindDirect.m_nID = AvergWindDirect;

		m_AvergWindSpeed.m_szName = QObject::tr("Weather Meter Average Wind Speed");
		m_AvergWindSpeed.m_nType = IDD_AIN;
		m_AvergWindSpeed.m_nID = AvergWindSpeed;

		m_AirTemperature.m_szName = QObject::tr("Weather Meter Air Temperature");
		m_AirTemperature.m_nType = IDD_AIN;
		m_AirTemperature.m_nID = AirTemperature;

		m_RelativeHumdty.m_szName = QObject::tr("Weather Meter Relative Humidity");
		m_RelativeHumdty.m_nType = IDD_AIN;
		m_RelativeHumdty.m_nID = RelativeHumdty;

		m_AirPressure.m_szName = QObject::tr("Weather Meter Air Pressure");
		m_AirPressure.m_nType = IDD_AIN;
		m_AirPressure.m_nID = AirPressure;

		Init();
	}
	~CWeatherData()
	{

	}
	void Init();
	bool SaveADIData(QXmlStreamWriter& writer);
	bool SaveWeatherData(QXmlStreamWriter& writer);

	bool LoadData(QXmlStreamReader& reader, CWeatherData* pWeaInfo);
	bool ReadADIN(QXmlStreamReader& reader, CWeatherData* pWeaInfo);
	bool ReadAi(QXmlStreamReader& reader, CWeatherData* pWeaInfo);

	QString & GetName()
	{
		return m_strName;
	}

public:
	CPPPointInfo  m_TotalRadiation;  //! �ܷ���
	CPPPointInfo  m_DirectRadiation; //! ��ֱ����
	CPPPointInfo  m_ScattRadiation;  //! ɢ�����
	CPPPointInfo  m_AvergWindDirect; //! ƽ������
	CPPPointInfo  m_AvergWindSpeed;  //! ƽ������
	CPPPointInfo  m_AirTemperature;  //! �����¶�
	CPPPointInfo  m_RelativeHumdty;  //! ���ʪ��
	CPPPointInfo  m_AirPressure;     //! ��ѹ
	QString m_strName;               //! ��������
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CPredictData
*   \brief ĳһ�糧�ķ���Ԥ�����ֵ */
class CPredictData
{
public:
	enum PredictType
	{
		predict4h = 1,
		predict72h,
	};

	CPredictData()
	{
		m_4Hour.m_szName = QObject::tr("Plant 4 Hour Predication Value");
		m_4Hour.m_nType = IDD_AOUT;
		m_4Hour.m_nID = predict4h;
		
		m_72Hour.m_szName = QObject::tr("Plant 72 Hour Predication Value");
		m_72Hour.m_nType = IDD_AOUT;
		m_72Hour.m_nID = predict72h;
		Init();
	}
	~CPredictData()
	{
		 
	}
	void Init();
	bool SaveADIData(QXmlStreamWriter& writer);
	bool SavePredictData(QXmlStreamWriter& writer);

	bool LoadData(QXmlStreamReader& reader, CPredictData* pPrdtInfo);
	bool ReadADIN(QXmlStreamReader& reader, CPredictData* pPrdtInfo);
	bool ReadAO(QXmlStreamReader& reader, CPredictData* pPrdtInfo);

	QString & GetName()
	{
		return m_strName;
	}

public:
	CPPPointInfo  m_4Hour;  //! 4HԤ�����ֵ
	CPPPointInfo  m_72Hour; //! 72HԤ�����ֵ
	
	QString m_strName;      //! Ԥ�������

	QVector<CPPPointInfo > m_vecTableInfo;
};
class CPredictGroup;

/*! \class  CStationData
*   \brief ĳһ�糧��������Ϣ */
class CStationData
{
public:
	explicit CStationData(CPredictGroup *pGrp):m_pParent(pGrp)
	{
		
	}
	~CStationData();

	CStationData();
public:
	// ��ȡȫվ����
	CPlantInfo * GetPlantInfo()
	{
		return &m_PlantValue;
	}

	const CPlantInfo &GetPlantInfo()const
	{
		return  m_PlantValue;
	}

	CWeatherData * GetWeatherInfo()
	{
		return &m_WeatherValue;
	}
	CPredictData * GetPredictInfo()
	{
		return &m_PredictValue;
	}
	CInverterGroup * GetInverterGrpInfo()
	{
		return &m_Inverters;
	}
	const QString &GetName()const
	{
		return m_strStationName;
	}
	void SetName(const QString & szName)
	{
		m_strStationName = szName;
	}
	CPredictGroup *GetParentGroup()
	{
		return m_pParent;
	}
	bool SaveStationData(const QString& szRoot, CStationData* pStnData);
	bool LoadStationData(QXmlStreamReader &reader);
	bool ReadPlant(QXmlStreamReader& reader);
	bool Readp(QXmlStreamReader& reader);

	bool LoadPlantData(QXmlStreamReader& reader);
	bool LoadWeatherData(QXmlStreamReader& reader);
	bool LoadPredictData(QXmlStreamReader& reader);
	bool LoadInvertersData(QXmlStreamReader& reader);

public:
	QString m_strStationName;  //! ��վ��
	CPredictGroup * m_pParent;  //! ��������
	QVector<CPlantInfo> m_vecPlantInfo;
	QVector<CInverterGroup> m_vecInverterGroup;
	QVector<CWeatherData> m_vecWeatherData;
	QVector<CPredictData> m_vecPredictData;
private:
	CPlantInfo     m_PlantValue;     //! �糧�Ļ�����Ϣ
	CInverterGroup  m_Inverters;     //! ȫ�������������
	CWeatherData  m_WeatherValue;    //! ��������
	CPredictData  m_PredictValue;    //! Ԥ������
	
	
};
/*! \class  CPredictGroup
*   \brief �糧��
    \details �����滹��������
 */
class CPredictGroup
{
public:
	CPredictGroup();
	~CPredictGroup();
public:
	//������,�����ڴ�
	CPredictGroup* CreateGroup();

	CPredictGroup* CreateGroup(CPredictGroup* pPrdtGrp);

	CPredictGroup* CreateGroup(QString& grpName);

	//ɾ����,�ͷ��ڴ�
	bool DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName);

	//ɾ��item,�ͷ��ڴ�
	bool DeleteItem(const QString& strTagItem);

	bool DeleteItem(CStationData *pStation);
public:
	//����
	QString m_strGrpName;

	//�����½�����,����½�����,���ڵ����½���,�������map
	QMap<QString, CPredictGroup* > m_mapStrGrps;

	//�������item����Ÿ���������item
	QMap<QString, CStationData* > m_mapStations;
private:
};
/*! \class  CPredictMgr
*   \brief ����Ԥ����ĵ���document��
    \detail �����滹��������
*/
class CPredictMgr
{	
public:
	CPredictMgr();
	~CPredictMgr();
public:
	//ɾ��Ԥ����hash
	bool DeleteHashPredictItem(const QString& strPredictTag);

	//�½�Ԥ�⳧վ,�����ڴ�
	CStationData* CreateNewPredictItem(CPredictGroup* pPredictGroup);

	//CPredictGroup* CreateNewPredictGrp(CPredictGroup* pPredictGroup);

	bool SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot);
	bool SaveChildNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPrdtGroup);
	bool SaveChildItem(CPredictGroup* pPrdtGroup, const QString & szRoot);
	void Init();
	void Reset();

	//���ع����ļ���Ԥ��ڵ�
	bool LoadPredictNode(QDomElement& elm, const QString& szRoot);

	bool LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot);

	bool LoadChildGroup(CPredictGroup* pPrdtGrp, QDomElement& elm, const QString& szRoot);

public:
	//Ԥ�����,��Ϊpowerpredict
	CPredictGroup* m_pRootPrdtGrp;

	//����Ԥ����map,�����ڸ�������������item�������map
	QMap<QString, CStationData* > m_mapRootGrpStrPrdtItem;

	//QMap<QString, CInverterInfo* > m_mapRootStrInvters;

	//Ԥ����vector
	QVector<CStationData*> m_arrPrdtDatas;

private:
	enum 
	{
		MAX_STATION_NUM = 256,
	};

};
#endif  //PREDICT_DATAINFO_H
