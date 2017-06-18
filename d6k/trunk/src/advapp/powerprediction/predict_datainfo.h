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
*   \brief 测点基本信息 */
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
	QString m_szLinkedTagName; //! 实时库的测点TAGNAME
	int m_nType; //! 开关量、模拟量
};
/*! \class  CInverterPointInfo
*   \brief 逆变器基本信息 */
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
	QString m_szName; //! 逆变器的名称
	CPPPointInfo  m_OpenState; //! 逆变器分合状态点
	CPPPointInfo  m_ActPower;  //! 逆变器有功
	CPPPointInfo  m_ReactPower; //! 逆变器无功
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CPlantInfo
*   \brief 全厂的基本信息 */
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
	CPPPointInfo  m_ActPower;  //! 全厂有功
	CPPPointInfo  m_ReactPower; //! 全厂无功
	QString m_strName;          //! 厂站名称
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CInverterGroup
*   \brief 某一电厂下的所有逆变器信息 */
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
	//! 添加一个逆变器
	CInverterInfo* AddInverter();
	//! 删除一个逆变器
	bool DeleteInverter(CInverterInfo *pInverter);
	void Reset()
	{
		// todo 先回收资源

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
	QString m_strName;                  //!逆变器组名
	int m_nCount;
};
/*! \class  CWeatherData
*   \brief 某一电厂下气象仪信息 */
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
	CPPPointInfo  m_TotalRadiation;  //! 总辐射
	CPPPointInfo  m_DirectRadiation; //! 垂直辐射
	CPPPointInfo  m_ScattRadiation;  //! 散射辐射
	CPPPointInfo  m_AvergWindDirect; //! 平均风向
	CPPPointInfo  m_AvergWindSpeed;  //! 平均风速
	CPPPointInfo  m_AirTemperature;  //! 空气温度
	CPPPointInfo  m_RelativeHumdty;  //! 相对湿度
	CPPPointInfo  m_AirPressure;     //! 气压
	QString m_strName;               //! 气象仪名
	QVector<CPPPointInfo > m_vecTableInfo;
};
/*! \class  CPredictData
*   \brief 某一电厂的发电预测输出值 */
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
	CPPPointInfo  m_4Hour;  //! 4H预测输出值
	CPPPointInfo  m_72Hour; //! 72H预测输出值
	
	QString m_strName;      //! 预测输出名

	QVector<CPPPointInfo > m_vecTableInfo;
};
class CPredictGroup;

/*! \class  CStationData
*   \brief 某一电厂的配置信息 */
class CStationData
{
public:
	explicit CStationData(CPredictGroup *pGrp):m_pParent(pGrp)
	{
		
	}
	~CStationData();

	CStationData();
public:
	// 获取全站数据
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
	QString m_strStationName;  //! 厂站名
	CPredictGroup * m_pParent;  //! 所属的组
	QVector<CPlantInfo> m_vecPlantInfo;
	QVector<CInverterGroup> m_vecInverterGroup;
	QVector<CWeatherData> m_vecWeatherData;
	QVector<CPredictData> m_vecPredictData;
private:
	CPlantInfo     m_PlantValue;     //! 电厂的基本信息
	CInverterGroup  m_Inverters;     //! 全厂的所有逆变器
	CWeatherData  m_WeatherValue;    //! 天气数据
	CPredictData  m_PredictValue;    //! 预测数据
	
	
};
/*! \class  CPredictGroup
*   \brief 电厂组
    \details 组下面还可以有组
 */
class CPredictGroup
{
public:
	CPredictGroup();
	~CPredictGroup();
public:
	//创建组,分配内存
	CPredictGroup* CreateGroup();

	CPredictGroup* CreateGroup(CPredictGroup* pPrdtGrp);

	CPredictGroup* CreateGroup(QString& grpName);

	//删除组,释放内存
	bool DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName);

	//删除item,释放内存
	bool DeleteItem(const QString& strTagItem);

	bool DeleteItem(CStationData *pStation);
public:
	//组名
	QString m_strGrpName;

	//父组下建子组,存放新建子组,根节点下新建组,即存入该map
	QMap<QString, CPredictGroup* > m_mapStrGrps;

	//组下添加item，存放该组下所有item
	QMap<QString, CStationData* > m_mapStations;
private:
};
/*! \class  CPredictMgr
*   \brief 功率预测的文档（document）
    \detail 组下面还可以有组
*/
class CPredictMgr
{	
public:
	CPredictMgr();
	~CPredictMgr();
public:
	//删除预测项hash
	bool DeleteHashPredictItem(const QString& strPredictTag);

	//新建预测厂站,分配内存
	CStationData* CreateNewPredictItem(CPredictGroup* pPredictGroup);

	//CPredictGroup* CreateNewPredictGrp(CPredictGroup* pPredictGroup);

	bool SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot);
	bool SaveChildNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPrdtGroup);
	bool SaveChildItem(CPredictGroup* pPrdtGroup, const QString & szRoot);
	void Init();
	void Reset();

	//加载工程文件中预测节点
	bool LoadPredictNode(QDomElement& elm, const QString& szRoot);

	bool LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot);

	bool LoadChildGroup(CPredictGroup* pPrdtGrp, QDomElement& elm, const QString& szRoot);

public:
	//预测根组,名为powerpredict
	CPredictGroup* m_pRootPrdtGrp;

	//所有预测项map,不论在根组或子组下添加item都存入该map
	QMap<QString, CStationData* > m_mapRootGrpStrPrdtItem;

	//QMap<QString, CInverterInfo* > m_mapRootStrInvters;

	//预测项vector
	QVector<CStationData*> m_arrPrdtDatas;

private:
	enum 
	{
		MAX_STATION_NUM = 256,
	};

};
#endif  //PREDICT_DATAINFO_H
