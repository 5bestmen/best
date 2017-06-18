#ifndef  PREDICT_DATABASE_H
#define  PREDICT_DATABASE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDateTime>
#include <QString>
#include <QMap>
#include "predictglobal.h"


class CPredictDatabase : public QObject
{
	Q_OBJECT

protected:

	explicit CPredictDatabase(QObject *parent = 0);

	~CPredictDatabase();

public:
	/************************************************************************/
	/*                      Ԥ�����ݿ��������ʷ�⣩                        */
	/************************************************************************/

	//4h����Ԥ���
	static QString Table_Weather_Predict_4h;

	//72h����Ԥ���
	static QString Table_Weather_Predict_72h;

	//4h����Ԥ���
	static QString Table_Power_Predict_4h;

	//72h����Ԥ���
	static QString Table_Power_Predict_72h;

	//����ֵ��
	static QString Table_MaintenanceCapacity;

	//����������
	static QString Table_RunningCapacity;

	//��ֵ��
	static QString Table_LimitCapacity;

	//���һ��15minԤ��,4h����Ԥ���
	static QString Table_Weather_Predict_4h_Final_15m;

	//���һ��24hԤ��,72h����Ԥ���
	static QString Table_Weather_Predict_72h_Final_24h;

	//���һ��15minԤ��,4h����Ԥ���
	static QString Table_Power_Predict_4h_Final_15m;

	//���һ��24hԤ��,72h����Ԥ���
	static QString Table_Power_Predict_72h_Final_24h;

	//���һ��15minԤ��,�ֶ�4h����Ԥ���
	static QString Table_Power_Predict_4h_Final_Manual_15m;

	//���һ��24hԤ��,�ֶ�24h����Ԥ���
	static QString Table_Power_Predict_72h_Final_Manual_24h;

	//��ʷ���ݱ�
	static QString Table_Hdb;

	//�����״̬��
	static QString Table_Inverter_Status;

	//4h��72h����Ԥ����һ��ֵ�ֶ�λ��
	static const int Predict_Value_First_Field = 4;

	//��ʷ���ݱ��һ��ֵ�ֶ�λ��
	static const int Yesterday_Value_First_Field = 7;

	#define Is_Debug_Sql_Operation 1
	#define Is_Debug_Sql_String 0 

public:
	enum Interval
	{
		Minute15 = 15
	};

	enum Weather4h_Source
	{
		Weather4h_Source_FtpPredict72h = 0,
		Weather4h_Source_ActualPredict4h
	};

public:

	static CPredictDatabase& GetInstance();

	//��ʼ��
	static void Init();

public:
	/************************************************************************/
	/*                              Test                                    */
	/************************************************************************/

	//������������������ֵ����ֵ���������ڲ��ԣ�
	void CreateTable_Running_Maintenance_Limit();

	//д���ݵ���������������ֵ����ֵ���������ڲ��ԣ�
	void InsertData_Running_Mantenance_Limit();

	//����һϵ�б����ڲ��ԣ�
	void CreateTable_For_Test();

	//�������15m��24hһϵ�б����ڲ��ԣ�
	void CreateTable_Final_15m_Or_24h();

	//д���ݵ����15m��24hһϵ�б����ڲ��ԣ�
	//void InsertData_Final_15m_Or_24h(const QDateTime& time);

	//����Ԥ��4h���ʱ����ڲ��ԣ�
	void CreateTable_PowerPredict4h();

	//����Ԥ��72h���ʱ����ڲ��ԣ�
	void CreateTable_PowerPredict72h();

	//����Ԥ��4h��������ڲ��ԣ�
	void CreateTable_WeatherPredict4h();

	//����Ԥ��72h��������ڲ��ԣ�
	void CreateTable_WeatherPredict72h();

	//�����������ͱ����ڲ��ԣ�
	void CreateTable_WeatherType();

	//д���ݵ��������ͱ����ڲ��ԣ�
	void InsertData_WeatherType();

	//����������ԴΪ4h,д��������������
	void InsertDataWeatherPredict4h_Final_15m(Weather_Predict_Data4h& data, const QDateTime& insertTime);

	//дԤ��72h��������ʱ,д��������������
	void InsertDataWeatherPredict72h_Final_24h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//������ʷ����,�����ڲ��ԣ�
	void InsertData_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info);

	//void InsertData_All_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info);
	
	//ÿ15����Ԥ��һ�Σ�����Ҫ��һ�θñ���4h��16����Ϊ��λ
	void InsertDataPowerPredict4h_Final_15m(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//ÿ15����Ԥ��һ�Σ�����Ҫ��һ�θñ���24h��96����Ϊ��λ
	void InsertDataPowerPredict72h_Final_24h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//void InsertDataPowerPredict4h_Final_Manual_15m(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//void InsertDataPowerPredict72h_Final_Manual_24h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//δ��4hԤ������ͨ���˺���д��Ԥ�⹦��4h�����ڲ��ԣ�
	//void InsertDataPowerPredict4h(const QDateTime& insertTime);

	//δ��4hԤ������ͨ���˺���д��Ԥ�⹦��4h��
	void InsertDataPowerPredict4h(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//δ��72hԤ������ͨ���˺���д��Ԥ�⹦��72h�����ڲ��ԣ�
	//void InsertDataPowerPredict72h(const QDateTime& insertTime);

	//δ��72hԤ������ͨ���˺���д��Ԥ�⹦��72h��
	void InsertDataPowerPredict72h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//д���ݵ�Ԥ��4h�����
	//void InsertDataWeatherPredict4h(const QDateTime& insertTime);

	//д�����ݵ�Ԥ��4h�����
	void InsertDataWeatherPredict4h(Weather_Predict_Data4h& data, const QDateTime& insertTime);

	//4h����Ԥ������Դ,�ֱ�д�����ݵ���
	void InsertDataWeatherPredict4h_From_Ftp72h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//д���ݵ�Ԥ��72h�����
	//void InsertDataWeatherPredict72h(const QDateTime& insertTime);

	//д���ݵ�Ԥ��72h�����
	void InsertDataWeatherPredict72h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//��Ԥ�����ݿ��л�ȡԤ��4h��������
	bool GetData_WeatherPredict4h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime);

	//��Ԥ�����ݿ��л�ȡԤ��72h��������
    bool GetData_WeatherPredict72h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime);

	//��Ԥ�����ݿ��л�ȡ����ֵ
	QVector<data_type> GetMaintenanceValue(QDateTime startTime, QDateTime endTime);

	//��Ԥ�����ݿ��ж�ȡ��ȥ4h��ʷֵ�������ܷ��䡢�ܹ��ʣ�
	bool GetYesterday_Value_4h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_4h);

	//��Ԥ�����ݿ��ж�ȡ��ȥ72h��ʷֵ�������ܷ��䡢�ܹ��ʣ�
	bool GetYesterday_Value_72h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_72h);

	//��Ԥ�����ݿ��л�ȡ4h����Ԥ��ֵ
	bool GetData_PowerPredict4h(const QDateTime& time, QVector<data_type>& vecValue);

	//��Ԥ�����ݿ��л�ȡ24h����Ԥ��ֵ
	bool GetData_PowerPredict24h(const QDateTime& time, QVector<data_type>& vecValue);

	//��ʵʱ���ȡģ������Ϣ
	bool GetAnalogue_Info(const Analogues_alias& alias, Analogues_Info& a_Info);

	//��ʵʱ���ȡ�����״̬��Ϣ
	bool GetInverters_Status_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos);

	//��ʵʱ���ȡ�����������Ϣ
	bool GetInverters_Power_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos);

	//��ȡģ������Ϣmap
	QMap<Analogues_alias, Analogues_Info> GetAnalogueInfoMap(QList<Analogues_alias>& alias);

	//��ȡ�����״̬map
	bool GetInverters_StatusMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	//��ȡ������й�map
	bool GetInverters_ActivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	//��ȡ������޹�map
	bool GetInverters_ReactivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	int randValue(int value);

private:
	//����sql�������һ�ֲ���
	QString Find_Key_Operator(const QString sql);

	//����ʱ����ӡ���sql��Ϣ
	void Print_Debug_String(const QString sql);

	//ִ��sql��䲢���ؽ��
	void Query_Debug_Error(const QString sql);

	//���̶���ʽ��ʾ�����գ�yyyy-MM-dd��
	QString Set_Date_Format(const QDateTime& dateTime);

	//���̶���ʽ��ʾ��ǰʱ�䣨yyyy-MM-dd hh:mm:00)
	QString Set_Time_Format();

	//���̶���ʽ��ʾ��ǰʱ�䣨yyyy-MM-dd hh:mm:ss)
	QString Set_Time_Format_Actual();

	//���̶���ʽ��ʾ����ʱ��dateTime��yyyy-MM-dd hh:mm:00)
	QString Set_Time_Format(const QDateTime& dateTime);

	//���̶���ʽ��ʾ����ʱ��dateTime��yyyy-MM-dd hh:mm:ss)
	QString Set_Time_Format_Actual(const QDateTime& dateTime);

	//��������������Ԥ��ʱ��ȷ������ʱ��
	QString Set_Time_Format_72h(const QDateTime& dateTime);

	//�����ֶ�����
	QString SetFieldsName(const QString& para, const int fieldNum);

	//�����ֶ�ֵ
	QString SetFieldsValue(const data_type& value, const int flag, const int fieldIndex);

private:
	
	QSqlQuery *m_pPredictQuery;

	QSqlDatabase m_predictDb;

	//Power_Predict_Data4h powerPredictData4h;

	//Power_Predict_Data72h powerPredictData72h;

	//Weather_Predict_Data4h weatherPredictData4h;

	//Weather_Predict_Data72h weatherPredictData72h;

	Weather4h_Source m_weather4hSource;

	static CPredictDatabase *s_Instance;

};

#endif  //PREDICT_DATABASE_H