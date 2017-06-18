#ifndef  PROPORTION_ALGORITHM_H
#define  PROPORTION_ALGORITHM_H

#include "basealgorithm.h"

class CProportionAlgorithm : public CBaseAlgorithm
{
	Q_OBJECT

public:
	CProportionAlgorithm();

	~CProportionAlgorithm();
	
	//4h����Ԥ���㷨
	void PowerPredict4h(const QDateTime &currTime);
	
	//72h����Ԥ���㷨
	void PowerPredict72h(const QDateTime &currTime);

private:
	
	void InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type);

	void InsertWeatherType4h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType);

	void InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, Weather_Type type);

	void InsetWeatherType72h(QMap<Weather_Type, Weather_Predict_Data> & map, QList<Weather_Type> listType);

private:
	//ģ�������б�
	QList<Analogues_alias> a_Alias;

	//����ģ�������б�,����map<ģ������,ģ��������>
	QMap<Analogues_alias, Analogues_Info> a_Info;

	//����ģ�������б�,����map<ģ������,��ȥ4h����>
	QMap<Analogues_alias, QVector<data_type> > map_yesterday_4h;

	//����ģ�������б�.����map<ģ������,��ȥ72h����>
	QMap<Analogues_alias, QVector<data_type> > map_yesterday_72h;

	//������������,����map<����������,4h��������>
	QMap<Weather_Type, Weather_Predict_Data> map_predict_weather_4h;

	//������������,����map<����������,72h��������>
	QMap<Weather_Type, Weather_Predict_Data> map_predict_weather_72h;

	//map<����������,�������>, ����������--ö��, �������--��DaqoWeather_TotalRadiation
	QMap<Weather_Type, Analogues_alias> map_weather_analogues_alias;

	//4h��������
	Power_Predict_Data4h power_predict_data_4h;

	//72h��������
	Power_Predict_Data72h power_predict_data_72h;

};






#endif  //PROPORTION_ALGORITHM_H