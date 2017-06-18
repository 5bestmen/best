#ifndef   PREDICT_GLOBAL_H
#define   PREDICT_GLOBAL_H

#include <QString>
#include <QVector>

namespace  PREDICTDAQO
{
	const int Weather_Predict_4h_Points = 16;
	const int Weather_Predict_24h_Points = 96;
	const int Weather_Predict_72h_Points = 288;

	const int Power_Predict_4h_Points = 16;
	const int Power_Predict_24h_Points = 96;
	const int Power_Predict_72h_Points = 288;


	const int Weather_Predict_Morning_Hour = 8;
	const int Weather_Predict_Afternon_Hour = 16;

	const int Power_Predict_Morning_Hour = 6;
	const int Power_Predict_Afternon_Hour = 18;

	typedef float data_type;

	typedef QString analogues_name;

	typedef QVector<data_type> Weather_Predict_Data;

	enum Weather_Type
	{
		Weather_Total_Radiation = 0,
		Weather_Scattered_Radiation,
		Weather_Direct_Radiation,
		Weather_Wind_Speed,
		Weather_Wind_Direction,
		Weather_Temperature,
		Weather_Humidity,
		Weather_Air_Pressure,
		Weather_Type_Count
	};

	enum Wind_Direction_Type
	{
		Wind_Direction_N = 0,
		Wind_Direction_NNW,
		Wind_Direction_WN,
		Wind_Direction_WWN,

		Wind_Direction_W,
		Wind_Direction_WWS,
		Wind_Direction_SW,
		Wind_Direction_SSW,

		Wind_Direction_S,
		Wind_Direction_SSE,
		Wind_Direction_ES,
		Wind_Direction_EES,

		Wind_Direction_E,
		Wind_Direction_EEN,
		Wind_Direction_NE,
		Wind_Direction_NNE,

		Wind_Direction_Count,
		Wind_Direction_Error
	};

	#define  DaqoWeather_TotalRadiation            "DaqoWeather.TotalRadiation"
	#define  DaqoWeather_ScatteredRadiation        "DaqoWeather.ScatteredRadiation"
	#define  DaqoWeather_DirectRadiation           "DaqoWeather.DirectRadiation" 
	#define  DaqoWeather_WindSpeed                 "DaqoWeather.WindSpeed"   
	#define  DaqoWeather_WindDirection             "DaqoWeather.WindDirection"
	#define  DaqoWeather_Temperature               "DaqoWeather.Temperature"
	#define  DaqoWeather_Humidity                  "DaqoWeather.Humidity"
	#define  DaqoWeather_AirPressure               "DaqoWeather.AirPressure"

	#define  DaqoPower_TotalPower                  "DaqoPower.TotalPower"
	#define  DaqoPower_Capacity                    "DaqoPower.Capacity"

	#define  DaqoInverter_Status                   "DaqoInverter.Status"
	#define  DaqoInverter_ReactivePower            "DaqoInverter.ReactivePower"
	#define  DaqoInverter_ActivePoewer             "DaqoInverter.ActivePower"

	#define  FTP_CONFIG_FILE                       "ftpconfig.ini"

	typedef struct weather_predict_from_local
	{
		Weather_Type type;
		data_type    data[Weather_Predict_4h_Points];
	}Weather_Predict_Data4h;

	typedef struct weather_predict_from_ftp
	{
		Weather_Type type;
		data_type    data[Weather_Predict_72h_Points];
	}Weather_Predict_Data72h;

	typedef struct power_predict_4h
	{
		data_type data[Power_Predict_4h_Points];
	}Power_Predict_Data4h;

	typedef struct power_predict_72h
	{
		data_type data[Power_Predict_72h_Points];
	}Power_Predict_Data72h;

	typedef QString Analogues_alias;

	typedef struct analogues_infomation
	{
		int station;
		int device;
		int point;
	}Analogues_Info; //模拟量属性

	typedef struct inverter_information
	{
		int station;
		int device;
		int point;
		QString name; //装置名
	}Inverter_Info;  //装置属性

}

using namespace PREDICTDAQO;

#endif  //PREDICT_GLOBAL_H