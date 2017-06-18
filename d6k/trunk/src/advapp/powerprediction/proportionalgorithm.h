#ifndef  PROPORTION_ALGORITHM_H
#define  PROPORTION_ALGORITHM_H

#include "basealgorithm.h"

class CProportionAlgorithm : public CBaseAlgorithm
{
	Q_OBJECT

public:
	CProportionAlgorithm();

	~CProportionAlgorithm();
	
	//4h功率预测算法
	void PowerPredict4h(const QDateTime &currTime);
	
	//72h功率预测算法
	void PowerPredict72h(const QDateTime &currTime);

private:
	
	void InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type);

	void InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType);

	void InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type);

	void InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType);

private:
	//模拟量名列表
	QList<Analogues_alias> a_Alias;

	//根据模拟量名列表,生成map<模拟量名,模拟量属性>
	QMap<Analogues_alias, Analogues_Info> a_Info;

	//根据模拟量名列表,生成map<模拟量名,过去4h数据>
	QMap<Analogues_alias, QVector<data_type> > map_yesterday_4h;

	//根据模拟量名列表.生成map<模拟量名,过去72h数据>
	QMap<Analogues_alias, QVector<data_type> > map_yesterday_72h;

	//根据气象类型,生成map<气象类型名,4h气象数据>
	QMap<Weather_Type, Weather_Predict_Data> map_predict_weather_4h;

	//根据气象类型,生成map<气象类型名,72h气象数据>
	QMap<Weather_Type, Weather_Predict_Data> map_predict_weather_72h;

	//map<气象类型名,气象别名>, 气象类型名--枚举, 气象别名--如DaqoWeather_TotalRadiation
	QMap<Weather_Type, Analogues_alias> map_weather_analogues_alias;

	//4h功率数据
	Power_Predict_Data4h power_predict_data_4h;

	//72h功率数据
	Power_Predict_Data72h power_predict_data_72h;

};






#endif  //PROPORTION_ALGORITHM_H