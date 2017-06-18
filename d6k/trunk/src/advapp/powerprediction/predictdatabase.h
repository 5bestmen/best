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
	/*                      预测数据库表名（历史库）                        */
	/************************************************************************/

	//4h气象预测表
	static QString Table_Weather_Predict_4h;

	//72h气象预测表
	static QString Table_Weather_Predict_72h;

	//4h功率预测表
	static QString Table_Power_Predict_4h;

	//72h功率预测表
	static QString Table_Power_Predict_72h;

	//检修值表
	static QString Table_MaintenanceCapacity;

	//运行容量表
	static QString Table_RunningCapacity;

	//限值表
	static QString Table_LimitCapacity;

	//最后一次15min预测,4h气象预测表
	static QString Table_Weather_Predict_4h_Final_15m;

	//最后一次24h预测,72h气象预测表
	static QString Table_Weather_Predict_72h_Final_24h;

	//最后一次15min预测,4h功率预测表
	static QString Table_Power_Predict_4h_Final_15m;

	//最后一次24h预测,72h功率预测表
	static QString Table_Power_Predict_72h_Final_24h;

	//最后一次15min预测,手动4h功率预测表
	static QString Table_Power_Predict_4h_Final_Manual_15m;

	//最后一次24h预测,手动24h功率预测表
	static QString Table_Power_Predict_72h_Final_Manual_24h;

	//历史数据表
	static QString Table_Hdb;

	//逆变器状态表
	static QString Table_Inverter_Status;

	//4h和72h气象预测表第一个值字段位置
	static const int Predict_Value_First_Field = 4;

	//历史数据表第一个值字段位置
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

	//初始化
	static void Init();

public:
	/************************************************************************/
	/*                              Test                                    */
	/************************************************************************/

	//创建运行容量、检修值、限值三个表（用于测试）
	void CreateTable_Running_Maintenance_Limit();

	//写数据到运行容量、检修值、限值三个表（用于测试）
	void InsertData_Running_Mantenance_Limit();

	//创建一系列表（用于测试）
	void CreateTable_For_Test();

	//创建最后15m或24h一系列表（用于测试）
	void CreateTable_Final_15m_Or_24h();

	//写数据到最后15m或24h一系列表（用于测试）
	//void InsertData_Final_15m_Or_24h(const QDateTime& time);

	//创建预测4h功率表（用于测试）
	void CreateTable_PowerPredict4h();

	//创建预测72h功率表（用于测试）
	void CreateTable_PowerPredict72h();

	//创建预测4h气象表（用于测试）
	void CreateTable_WeatherPredict4h();

	//创建预测72h气象表（用于测试）
	void CreateTable_WeatherPredict72h();

	//创建气象类型表（用于测试）
	void CreateTable_WeatherType();

	//写数据到气象类型表（用于测试）
	void InsertData_WeatherType();

	//天气数据来源为4h,写入天气气象数据
	void InsertDataWeatherPredict4h_Final_15m(Weather_Predict_Data4h& data, const QDateTime& insertTime);

	//写预测72h天气数据时,写入天气气象数据
	void InsertDataWeatherPredict72h_Final_24h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//插入历史数据,（用于测试）
	void InsertData_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info);

	//void InsertData_All_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info);
	
	//每15分钟预测一次，都需要存一次该表，以4h的16个点为单位
	void InsertDataPowerPredict4h_Final_15m(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//每15分钟预测一次，都需要存一次该表，以24h的96个点为单位
	void InsertDataPowerPredict72h_Final_24h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//void InsertDataPowerPredict4h_Final_Manual_15m(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//void InsertDataPowerPredict72h_Final_Manual_24h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//未来4h预测数据通过此函数写入预测功率4h表（用于测试）
	//void InsertDataPowerPredict4h(const QDateTime& insertTime);

	//未来4h预测数据通过此函数写入预测功率4h表
	void InsertDataPowerPredict4h(Power_Predict_Data4h& data, const QDateTime& insertTime);

	//未来72h预测数据通过此函数写入预测功率72h表（用于测试）
	//void InsertDataPowerPredict72h(const QDateTime& insertTime);

	//未来72h预测数据通过此函数写入预测功率72h表
	void InsertDataPowerPredict72h(Power_Predict_Data72h& data, const QDateTime& insertTime);

	//写数据到预测4h气象表
	//void InsertDataWeatherPredict4h(const QDateTime& insertTime);

	//写入数据到预测4h气象表
	void InsertDataWeatherPredict4h(Weather_Predict_Data4h& data, const QDateTime& insertTime);

	//4h天气预测两个源,分别写入数据到表
	void InsertDataWeatherPredict4h_From_Ftp72h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//写数据到预测72h气象表
	//void InsertDataWeatherPredict72h(const QDateTime& insertTime);

	//写数据到预测72h气象表
	void InsertDataWeatherPredict72h(Weather_Predict_Data72h& data, const QDateTime& insertTime);

	//从预测数据库中获取预测4h气象数据
	bool GetData_WeatherPredict4h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime);

	//从预测数据库中获取预测72h气象数据
    bool GetData_WeatherPredict72h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime);

	//从预测数据库中获取检修值
	QVector<data_type> GetMaintenanceValue(QDateTime startTime, QDateTime endTime);

	//从预测数据库中读取过去4h历史值（包括总辐射、总功率）
	bool GetYesterday_Value_4h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_4h);

	//从预测数据库中读取过去72h历史值（包括总辐射、总功率）
	bool GetYesterday_Value_72h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_72h);

	//从预测数据库中获取4h功率预测值
	bool GetData_PowerPredict4h(const QDateTime& time, QVector<data_type>& vecValue);

	//从预测数据库中获取24h功率预测值
	bool GetData_PowerPredict24h(const QDateTime& time, QVector<data_type>& vecValue);

	//从实时库获取模拟量信息
	bool GetAnalogue_Info(const Analogues_alias& alias, Analogues_Info& a_Info);

	//从实时库获取逆变器状态信息
	bool GetInverters_Status_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos);

	//从实时库获取逆变器功率信息
	bool GetInverters_Power_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos);

	//获取模拟量信息map
	QMap<Analogues_alias, Analogues_Info> GetAnalogueInfoMap(QList<Analogues_alias>& alias);

	//获取逆变器状态map
	bool GetInverters_StatusMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	//获取逆变器有功map
	bool GetInverters_ActivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	//获取逆变器无功map
	bool GetInverters_ReactivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map);

	int randValue(int value);

private:
	//遍历sql语句是哪一种操作
	QString Find_Key_Operator(const QString sql);

	//调试时，打印输出sql信息
	void Print_Debug_String(const QString sql);

	//执行sql语句并返回结果
	void Query_Debug_Error(const QString sql);

	//按固定格式显示年月日（yyyy-MM-dd）
	QString Set_Date_Format(const QDateTime& dateTime);

	//按固定格式显示当前时间（yyyy-MM-dd hh:mm:00)
	QString Set_Time_Format();

	//按固定格式显示当前时间（yyyy-MM-dd hh:mm:ss)
	QString Set_Time_Format_Actual();

	//按固定格式显示给定时间dateTime（yyyy-MM-dd hh:mm:00)
	QString Set_Time_Format(const QDateTime& dateTime);

	//按固定格式显示给定时间dateTime（yyyy-MM-dd hh:mm:ss)
	QString Set_Time_Format_Actual(const QDateTime& dateTime);

	//根据上下午天气预测时间确定更新时间
	QString Set_Time_Format_72h(const QDateTime& dateTime);

	//设置字段名字
	QString SetFieldsName(const QString& para, const int fieldNum);

	//设置字段值
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