#include "proportionalgorithm.h"
#include "predictdatabase.h"
#include <QDebug>


CProportionAlgorithm::CProportionAlgorithm()
{
	a_Alias.append(DaqoWeather_TotalRadiation);
	a_Alias.append(DaqoPower_TotalPower);

	a_Info = GetAnaloguesInfo(a_Alias);

	QVector<data_type> vec_4h(Power_Predict_4h_Points);
	QVector<data_type> vec_72h(Power_Predict_72h_Points);

	for each (const QString & str in a_Alias)
	{
		map_yesterday_4h.insert(str, vec_4h);
		map_yesterday_72h.insert(str, vec_72h);
	}

	QList<Weather_Type> listType;
	listType.append(Weather_Total_Radiation);
	InsertWeatherType4h(map_predict_weather_4h, listType);
	InsetWeatherType72h(map_predict_weather_72h, listType);

	map_weather_analogues_alias.insert(Weather_Total_Radiation, DaqoWeather_TotalRadiation);

}

CProportionAlgorithm::~CProportionAlgorithm()
{

}

void CProportionAlgorithm::InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type)
{
	Weather_Predict_Data  weatherPredictData4h(Weather_Predict_4h_Points);
	map.insert(type, weatherPredictData4h);
}


void CProportionAlgorithm::InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType)
{
	Weather_Predict_Data weatherPredictData4h(Weather_Predict_4h_Points);
	
	for each (Weather_Type type in listType)
	{
		map.insert(type, weatherPredictData4h);
	}
}

void CProportionAlgorithm::InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type)
{
	Weather_Predict_Data weatherPredictData72h(Weather_Predict_72h_Points);
	map.insert(type, weatherPredictData72h);
}

void CProportionAlgorithm::InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType)
{
	Weather_Predict_Data weatherPredictData72h(Weather_Predict_72h_Points);

	for each (Weather_Type type in listType)
	{
		map.insert(type, weatherPredictData72h);
	}
}

/*! \fn void CProportionAlgorithm::PowerPredict4h(const QDateTime &currTime)
********************************************************************************************************************************
** \brief PowerPredict4h
** \details 4h功率预测算法
** \return void
** \author GuoHaijun
** \date 2016年1月12日
** \note
*******************************************************************************************************************************/
void CProportionAlgorithm::PowerPredict4h(const QDateTime &currTime)
{
	QDateTime predictStartTime, predictEndTime;
	QVector<data_type> maintenanceValues;
	QString strTime;
	bool isFind4h = false;
	
	predictStartTime = currTime.addSecs(15 * 60);
	int hour = predictStartTime.time().hour();
	int minu = (predictStartTime.time().minute() / 15) * 15;

	strTime = QString("%1 %2:%3:00").arg(predictStartTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));

	predictStartTime = QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss");

	predictEndTime = predictStartTime.addSecs(60 * 60 * 4 - 15 * 60);

	if ((currTime.time().hour() <= PREDICTDAQO::Power_Predict_Morning_Hour) || (currTime.time().hour() >= PREDICTDAQO::Power_Predict_Afternon_Hour))
	{
		for (int i = 0; i < Power_Predict_4h_Points; i++)
		{
			power_predict_data_4h.data[i] = 0;
		}

		CPredictDatabase::GetInstance().InsertDataPowerPredict4h(power_predict_data_4h, currTime);
		return;
	}

	QDateTime time = currTime.addDays(-1).addSecs(15 * 60);

	if (CPredictDatabase::GetInstance().GetYesterday_Value_4h(time,a_Info,map_yesterday_4h))
	{
		if (CPredictDatabase::GetInstance().GetData_WeatherPredict4h(map_predict_weather_4h, predictStartTime))
		{
			maintenanceValues = CPredictDatabase::GetInstance().GetMaintenanceValue(predictStartTime, predictEndTime);

			for (int i = 0; i < Power_Predict_4h_Points; i++)
			{
				if ( map_yesterday_4h[DaqoWeather_TotalRadiation][i] != 0 )
				{
					power_predict_data_4h.data[i] = map_predict_weather_4h[Weather_Total_Radiation][i] / map_yesterday_4h[DaqoWeather_TotalRadiation][i] * map_yesterday_4h[DaqoPower_TotalPower][i];
					power_predict_data_4h.data[i] = power_predict_data_4h.data[i] - maintenanceValues[i];

					if (power_predict_data_4h.data[i] < 0)
					{
						power_predict_data_4h.data[i] = 0;
					}

					if (power_predict_data_4h.data[i] > capacity)
					{
						power_predict_data_4h.data[i] = capacity;
					}
				} 
				else
				{
					for (int j = 0; j < Power_Predict_4h_Points; j++)
					{
						if (map_yesterday_4h[DaqoWeather_TotalRadiation][j] != 0)
						{
							power_predict_data_4h.data[i] = map_predict_weather_4h[Weather_Total_Radiation][i] / map_yesterday_4h[DaqoWeather_TotalRadiation][j] * map_yesterday_4h[DaqoPower_TotalPower][j];
							power_predict_data_4h.data[i] = power_predict_data_4h.data[i] - maintenanceValues[i];

							if (power_predict_data_4h.data[i] < 0)
							{
								power_predict_data_4h.data[i] = 0;
							}

							if (power_predict_data_4h.data[i] > capacity)
							{
								power_predict_data_4h.data[i] = capacity;
							}

							isFind4h = true;
							break;
						}
					}
					if (!isFind4h)
					{
						power_predict_data_4h.data[i] = 0;
					}
				}
			}
			CPredictDatabase::GetInstance().InsertDataPowerPredict4h(power_predict_data_4h, predictStartTime);
		} 
		else
		{
			qDebug() << "get_4h_weather_predict_data error!";
			return;
		}		
	} 
	else
	{
		qDebug() << "get_yesterday_4h_data error!";
		return;
	}
	qDebug() << "4h proportion algorithm success...";
	return;
}

/*! \fn void CProportionAlgorithm::PowerPredict72h(const QDateTime &currTime)
********************************************************************************************************************************
** \brief PowerPredict72h
** \details 72h功率预测算法
** \return void
** \author GuoHaijun
** \date 2016年1月12日
** \note
*******************************************************************************************************************************/
void CProportionAlgorithm::PowerPredict72h(const QDateTime &currTime)
{
	QDateTime predictStartTime, predictEndTime;
	QVector<data_type> maintenanceValues;

	bool isFind72h = false;

	QDateTime time = QDateTime::fromString(currTime.toString("yyyy-MM-dd 00:00:00"), "yyyy-MM-dd hh:mm:ss");

	predictStartTime = time.addDays(1);
	predictEndTime = time.addDays(4);

	if (CPredictDatabase::GetInstance().GetYesterday_Value_72h(time, a_Info, map_yesterday_72h))
	{
		if (CPredictDatabase::GetInstance().GetData_WeatherPredict72h(map_predict_weather_72h, currTime))
		{
			maintenanceValues = CPredictDatabase::GetInstance().GetMaintenanceValue(predictStartTime, predictEndTime);

			for (int i = 0; i < Power_Predict_72h_Points; i++ )
			{
				if ( map_yesterday_72h[DaqoWeather_TotalRadiation][i] != 0 )
				{
					power_predict_data_72h.data[i] = map_predict_weather_72h[Weather_Total_Radiation][i] / map_yesterday_72h[DaqoWeather_TotalRadiation][i] * map_yesterday_72h[DaqoPower_TotalPower][i];
					power_predict_data_72h.data[i] = power_predict_data_72h.data[i] - maintenanceValues[i];

					if (power_predict_data_72h.data[i] < 0)
					{
						power_predict_data_72h.data[i] = 0;
					}

					if (power_predict_data_72h.data[i] > capacity)
					{
						power_predict_data_72h.data[i] = capacity;
					}
				} 
				else
				{
					for (int j = 0; j < Power_Predict_72h_Points; j++)
					{
						if (map_yesterday_72h[DaqoWeather_TotalRadiation][j] != 0)
						{
							power_predict_data_72h.data[i] = map_predict_weather_72h[Weather_Total_Radiation][i]/map_yesterday_72h[DaqoWeather_TotalRadiation][j] * map_yesterday_72h[DaqoPower_TotalPower][j];
							power_predict_data_72h.data[i] = power_predict_data_72h.data[i] - maintenanceValues[i];

							if (power_predict_data_72h.data[i] < 0)
							{
								power_predict_data_72h.data[i] = 0;
							}

							if (power_predict_data_72h.data[i] > capacity)
							{
								power_predict_data_72h.data[i] = capacity;
							}

							isFind72h = true;
							break;
						}
					}
					if (!isFind72h)
					{
						power_predict_data_72h.data[i] = 0;
					}
				}
			}
			CPredictDatabase::GetInstance().InsertDataPowerPredict72h(power_predict_data_72h, currTime);
		} 
		else
		{
			qDebug() << "get_72h_weather_predict_data error!";
			return;
		}
	} 
	else
	{
		qDebug()<< "get_yesterday_72h_data error!";
		return;
	}
	qDebug()<< "72h proportion algorithm success...";
	return;
}