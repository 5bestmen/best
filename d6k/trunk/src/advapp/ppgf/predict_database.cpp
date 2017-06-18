#include "predict_database.h"

#include <QMap>
#include <QDateTime>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlError>


QString CPredictDatabase::Table_Weather_Predict_4h = "weather_predict_4h";
QString CPredictDatabase::Table_Weather_Predict_72h = "weather_predict_72h";
QString CPredictDatabase::Table_Weather_Predict_4h_Final_15m = "weather_predict_4h_final_15m";
QString CPredictDatabase::Table_Weather_Predict_72h_Final_24h = "weather_predict_72h_final_24h";

QString CPredictDatabase::Table_Power_Predict_4h = "power_predict_4h";
QString CPredictDatabase::Table_Power_Predict_72h = "power_predict_72h";
QString CPredictDatabase::Table_Power_Predict_4h_Final_15m = "power_predict_4h_final_15m";
QString CPredictDatabase::Table_Power_Predict_72h_Final_24h = "power_predict_72h_final_24h";
QString CPredictDatabase::Table_Power_Predict_4h_Final_Manual_15m = "power_predict_4h_final_manual_15m";
QString CPredictDatabase::Table_Power_Predict_72h_Final_Manual_24h = "power_predict_72h_final_manual_24h";

QString CPredictDatabase::Table_MaintenanceCapacity = "maintenancecapacity";
QString CPredictDatabase::Table_RunningCapacity = "runningcapacity";
QString CPredictDatabase::Table_LimitCapacity = "limitcapacity";

QString CPredictDatabase::Table_Hdb = "hdb";
QString CPredictDatabase::Table_Inverter_Status = "inverter_status";

CPredictDatabase* CPredictDatabase::s_Instance(0);

CPredictDatabase::CPredictDatabase(QObject *parent)
	:QObject(parent)
{
	m_weather4hSource = Weather4h_Source_FtpPredict72h;

	m_predictDb = QSqlDatabase::addDatabase("QMYSQL", "predictMySql");

	//从配置文件读取host、user、paswd、port后设置相应参数,目前先写死，后续增加从配置文件读取
	QString host = "localhost";
	QString user = "root";
	QString pass = "123456";
	int     port = 3306;
	m_predictDb.setHostName(host);
	m_predictDb.setUserName(user);
	m_predictDb.setPassword(pass);
	m_predictDb.setPort(port);
	m_predictDb.setDatabaseName("history");

	if(!m_predictDb.open())
	{
		qDebug() << "MYSQL connect fail: " << m_predictDb.lastError().text();
	}

#ifdef Q_OS_LINUX
	m_predictDb.exec("SET NAMES 'Latin1' ");
#endif // Q_OS_LINUX

	m_pPredictQuery = new QSqlQuery(m_predictDb);
	Q_CHECK_PTR(m_pPredictQuery);

}

CPredictDatabase::~CPredictDatabase()
{
	m_predictDb.close();

	delete m_pPredictQuery;

	m_pPredictQuery = nullptr;
}

CPredictDatabase& CPredictDatabase::GetInstance()
{
	static CPredictDatabase s_PredictDb;
	return s_PredictDb;
}

/*! \fn void CPredictDatabase::Init()
*********************************************
** \brief   Init
** \details 初始化
** \return  bool
** \author  GuoHaijun
** \date    2017年5月27日
** \note
********************************************/
void CPredictDatabase::Init()
{
	GetInstance();
}

/*! \fn bool CPredictDatabase::GetYesterday_Value_4h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_4h)
****************************************************************************************************************************************************************************************
** \brief    GetYesterday_Value_4h
** \details  从预测数据库中读取过去4h历史值（包括总辐射、总功率）
** \return   bool
** \author   GuoHaijun
** \date     2017年5月27日
** \note
****************************************************************************************************************************************************************************************/
bool CPredictDatabase::GetYesterday_Value_4h(const QDateTime& time, QMap<Analogues_Alias, Analogues_Info>& a_Info, QMap<Analogues_Alias, QVector<data_type> >& map_yesterday_4h)
{
	QString sql = "";
	bool isOk = false;

	int pos = ((time.time().hour() * 60 + time.time().minute()) / 15) * 15;
	int tick = (pos / 5) * 2;

	for each (const QString &key in a_Info.keys())
	{
		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND point = %4 AND time = '%5' ").arg(Table_Hdb).arg(a_Info[key].station).arg(a_Info[key].device).arg(a_Info[key].point).arg(Set_Date_Format(time));

		(*m_pPredictQuery).exec(sql);
		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			for (int i = 0; i < Power_Predict_4h_Points; i++)
			{
				map_yesterday_4h[key][i] = (*m_pPredictQuery).value(Yesterday_Value_First_Field + tick + i * 6).toFloat();
			}
		} 
		else
		{
			isOk = false;
		}
	}
	return isOk;
}

/*! \fn bool CPredictDatabase::GetYesterday_Value_72h(const QDateTime& time, QMap<Analogues_alias, Analogues_Info>& a_Info, QMap<Analogues_alias, QVector<data_type> >& map_yesterday_72h)
****************************************************************************************************************************************************************************************
** \brief    GetYesterday_Value_72h
** \details  从预测数据库中读取过去72h历史值（包括总辐射、总功率）
** \return   bool
** \author   GuoHaijun
** \date     2017年5月27日
** \note
****************************************************************************************************************************************************************************************/
bool CPredictDatabase::GetYesterday_Value_72h(const QDateTime& time, QMap<Analogues_Alias, Analogues_Info>& a_Info, QMap<Analogues_Alias, QVector<data_type> >& map_yesterday_72h)
{
	bool isOk = true;
	QString sql = "";

	int index = 0;
	for each (const QString &key in a_Info.keys())
	{
		index = 0;
		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND point = %4 AND time = '%5' ").arg(Table_Hdb).arg(a_Info[key].station).arg(a_Info[key].device).arg(a_Info[key].point).arg(Set_Date_Format(time.addDays(-1)));
		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);

			for (int i = 0; i < Power_Predict_24h_Points; i++)
			{
				map_yesterday_72h[key][index++] = (*m_pPredictQuery).value(Yesterday_Value_First_Field + i * 6).toFloat();
			}
		}
		else
		{
			isOk = false;
		}

		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND point = %4 AND time = '%5' ").arg(Table_Hdb).arg(a_Info[key].station).arg(a_Info[key].device).arg(a_Info[key].point).arg(Set_Date_Format(time.addDays(-2)));

		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);

			for (int i = 0; i < Power_Predict_24h_Points; i++)
			{
				map_yesterday_72h[key][index++] = (*m_pPredictQuery).value(Yesterday_Value_First_Field + i*6).toFloat();
			}
		} 
		else
		{
			isOk = false;
		}

		sql = QString("SELECT * FROM %1 WHERE station = %2 AND device = %3 AND point = %4 AND time = '%5' ").arg(Table_Hdb).arg(a_Info[key].station).arg(a_Info[key].device).arg(a_Info[key].point).arg(Set_Date_Format(time.addDays(-3)));

		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);

			for (int i = 0; i < Power_Predict_24h_Points; i++)
			{
				map_yesterday_72h[key][index++] = (*m_pPredictQuery).value(Yesterday_Value_First_Field + i * 6).toFloat();
			}
		} 
		else
		{
			isOk = false;
		}
	}

	return isOk;
}

/*! \fn void CPredictDatabase::CreateTable_PowerPredict4h()
******************************************************************
** \brief     CreateTable_PowerPredict4h
** \details   创建预测4h功率表（用于测试）
** \return    void
** \author    GuoHaijun
** \date      2017年5月27日
** \note
*****************************************************************/
void CPredictDatabase::CreateTable_PowerPredict4h()
{
	QString sql;
	Query_Debug_Error(QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_4h));

	sql = QString("create table if not exists %1 ( "
		"id integer primary key auto_increment,"
		"predict_time timestamp default '2017-01-01 00:00:00' not null, "
		"insert_time timestamp default '2017-01-01' 00:00:00' not null, ").arg(Table_Power_Predict_4h);

	sql += SetFieldsName("v%1 float default 0, v%2_falt tinyint default 0,", Power_Predict_4h_Points);

	sql.chop(1);
	sql.append(")");

	Print_Debug_String(sql);

	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::CreateTable_PowerPredict72h()
******************************************************************
** \brief     CreateTable_PowerPredict72h
** \details   创建预测72h功率表（用于测试）
** \return    void
** \author    GuoHaijun
** \date      2017年5月27日
** \note
*****************************************************************/
void CPredictDatabase::CreateTable_PowerPredict72h()
{	
	QString sql;
	Query_Debug_Error(QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_72h));

	sql = QString("create table if not exists %1 ( "
		"id integer primary key auto_increment,"
		"predict_time timestamp default '2017-01-01 00:00:00' not null,"
		"insert_time timestamp default '2017-01-01 00:00:00' not null, ").arg(Table_Power_Predict_72h);

	sql += SetFieldsName("v%1 float default 0, v%2_flag tinyint default 0,", Power_Predict_72h_Points);

	sql.chop(1);
	sql.append(")");

	Print_Debug_String(sql);

	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::CreateTable_WeatherPredict4h()
******************************************************************
** \brief    CreateTable_WeatherPredict4h
** \details  创建预测4h气象表（用于测试）
** \return   void
** \author   GuoHaijun
** \date     2017年5月27日
** \note
*****************************************************************/
void CPredictDatabase::CreateTable_WeatherPredict4h()
{
	QString sql;
	Query_Debug_Error(QString("DROP TABLE IF EXISTS %1").arg(Table_Weather_Predict_4h));

	sql = QString("create table if not exists %1 ( "
		"id integer primary key auto_increment,"
		"predict_time timestamp default '2017-01-01 00:00:00' not null,"
		"insert_time timestamp default '2017-01-01 00:00:00' not null, "
		"weather_type tinyint not null, ").arg(Table_Weather_Predict_4h);

	sql += SetFieldsName("v%1 float default 0, v%2_flag tinyint default 0,", Weather_Predict_4h_Points);

	sql += "FOREIGN KEY (weather_type) REFERENCES weather_type(type) ON DELETE RESTRICT)";

	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::CreateTable_WeatherPredict72h()
******************************************************************
** \brief    CreateTable_WeatherPredict72h
** \details  创建预测72h气象表（用于测试）
** \return   void
** \author   GuoHaijun
** \date    2017年5月27日
** \note
*****************************************************************/
void CPredictDatabase::CreateTable_WeatherPredict72h()
{
	QString sql;
	Query_Debug_Error(QString("DROP TABLE IF EXISTS %1").arg(Table_Weather_Predict_72h));

	sql = QString("create table if not exists %1 ( "
		"id integer primary key auto_increment,"
		"predict_time timestamp default '2017-01-01 00:00:00' not null,"
		"insert_time timestamp default '2017-01-01 00:00:00' not null, "
		"weather_type tinyint not null, ").arg(Table_Weather_Predict_4h);

	sql += SetFieldsName("v%1 float default 0, v%2_flag tinyint default 0,", Weather_Predict_72h_Points);

	sql += "FOREIGN KEY (weather_type) REFERENCES weather_type(type) ON DELETE RESTRICT)";

	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::CreateTable_WeatherType()
********************************************************
** \brief   CreateTable_WeatherType
** \details 创建气象类型表（用于测试）
** \return  void
** \author  GuoHaijun
** \date    2017年5月27日
** \note
*******************************************************/
void CPredictDatabase::CreateTable_WeatherType()
{
	Query_Debug_Error("CREATE TABLE IF NOT EXISTS weather_type (type tinyint primary key,"
		"name varchar(32) not null"
		")");
}

/*! \fn void CPredictDatabase::CreateTable_Running_Maintenance_Limit()
**********************************************************************
** \brief    CreateTable_Running_Maintenance_Limit
** \details  创建运行容量、检修值、限值三个表（用于测试）
** \return   void
** \author   GuoHaijun
** \date     2017年5月27日
** \note
**********************************************************************/
void CPredictDatabase::CreateTable_Running_Maintenance_Limit()
{
	Query_Debug_Error(QString("DROP TABLE IF EXISTS %1").arg(Table_RunningCapacity));

	QString sql = QString("create table if not exists %1 ( "
		"id int primary key auto_increment,"
		"starttime datetime default '2017-01-01 00:00:00' not null, "
		"endtime datetime default '2017-01-01 00:00:00' not null, "
		"value float default 0 not null) ").arg(Table_RunningCapacity);

	Print_Debug_String(sql);

	Query_Debug_Error(sql);

	Query_Debug_Error(QString("DROP TABLE IF EXIST %1").arg(Table_MaintenanceCapacity));

	sql = QString("create table if not exists %1 ( "
		"id int primary key auto_increment,"
		"starttime datetime default '2017-01-01 00:00:00' not null,"
		"endtime datetime default '2017-01-01 00:00:00' not null,"
		"description varchar(255) default 'change' not null,"
		"value float default 0 not null)").arg(Table_MaintenanceCapacity);
	
	Print_Debug_String(sql);

	Query_Debug_Error(sql);

	Query_Debug_Error(QString("DROP TABLE IF EXIST %1").arg(Table_LimitCapacity));

	sql = QString("create table if not exists %1 ( "
		"id int primary key auto_increment,"
		"starttime datetime default '2017-01-01 00:00:00' not null, "
		"endtime datetime default '2017-01-01 00:00:00' not null, "
		"value float default 0 not null) ").arg(Table_LimitCapacity);

	Print_Debug_String(sql);

	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::InsertData_Running_Mantenance_Limit()
**********************************************************************
** \brief    InsertData_Running_Mantenance_Limit
** \details  写数据到运行容量、检修值、限值三个表（用于测试）
** \return   void
** \author   GuoHaijun
** \date     2017年5月27日
** \note
**********************************************************************/
void CPredictDatabase::InsertData_Running_Mantenance_Limit()
{
	Query_Debug_Error("INSERT INTO runningcapacity(starttime,endtime,value) values('2017-01-01 00:00:00','2017-01-07 23:55:59',4.25)");
	Query_Debug_Error("INSERT INTO maintenancecapacity(starttime,endtime,descripation,value) values('2017-01-01 00:00:00','2017-01-07 23:55:59','检修4.25',4.25)");
	Query_Debug_Error("INSERT INTO limitcapacity(starttime,endtime,value) values('2017-01-01 00:00:00','2017-01-07 23:55:59',4.25)");
}

/*! \fn void CPredictDatabase::CreateTable_For_Test()
********************************************************
** \brief    CreateTable_For_Test
** \details  创建一系列表（用于测试）
** \return   void
** \author   GuoHaijun
** \date     2017年5月27日
** \note
********************************************************/
void CPredictDatabase::CreateTable_For_Test()
{
	CreateTable_Running_Maintenance_Limit();
	CreateTable_Final_15m_Or_24h();
	CreateTable_WeatherPredict4h();
	CreateTable_WeatherPredict72h();
	CreateTable_PowerPredict4h();
	CreateTable_PowerPredict72h();
}

/*! \fn void CPredictDatabase::CreateTable_Final_15m_Or_24h()
*************************************************************
** \brief    CreateTable_Final_15m_Or_24h
** \details  创建最后15m或24h一系列表（用于测试）
** \return   void
** \author   GuoHaijun
** \date     2017年5月27日
** \note
*************************************************************/
void CPredictDatabase::CreateTable_Final_15m_Or_24h()
{
	QString sql;

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Weather_Predict_4h_Final_15m);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists %1 ( "
				  "id integer primary key auto_increment, "
				  "predict_time datetime default '2017-01-01 00:00:00' not null, "
				  "weather_type tinyint not null, "
				  "v1 float default 0, v1_flag tinyint default 0)"
				  ).arg(Table_Weather_Predict_4h_Final_15m);
	Print_Debug_String(sql);
	Query_Debug_Error(sql);

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_4h_Final_15m);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists ("
				  "id integer primary key auto_increment, "
				  "predict_time datetime default '2017-01-01 00:00:00' not null, "
				  "v1 float default 0, v1_flag tinyint default 0)"
				  ).arg(Table_Power_Predict_4h_Final_15m);
	Print_Debug_String(sql);
	Query_Debug_Error(sql);

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_4h_Final_Manual_15m);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists ( "
				  "id integer primary key auto_increment, "
				  "predict_time datetime default '2017-01-01 00:00:00' not null, "
				  "v1 tinyint default 0, v1_flag tinyint default 0)"
				  ).arg(Table_Power_Predict_4h_Final_Manual_15m);
	Print_Debug_String(sql);
	Query_Debug_Error(sql);

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Weather_Predict_72h_Final_24h);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists %1 ("
				  "id integer primary key auto_increment, "
				  "predict_time date default '2017-01-01' not null, "
				  "weather_type tinyint not null, "
		).arg(Table_Weather_Predict_72h_Final_24h);

	sql += SetFieldsName("v%1 float default 0, v%2_flat tinyint default 0,", Weather_Predict_24h_Points);
	sql.chop(1);
	sql.append(")");
	Print_Debug_String(sql);
	Query_Debug_Error(sql);

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_72h_Final_24h);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists %1 ( "
				  "id integer primary key auto_increment, "
				  "predict_time date default '2017-01-01' not null, "
				  ).arg(Table_Power_Predict_72h_Final_24h);

	sql += SetFieldsName("v%1 float default 0, v%2 tinyint default 0,",Power_Predict_24h_Points);
	sql.chop(1);
	sql.append(")");
	Print_Debug_String(sql);
	Query_Debug_Error(sql);

	sql = QString("DROP TABLE IF EXISTS %1").arg(Table_Power_Predict_72h_Final_Manual_24h);
	Query_Debug_Error(sql);
	sql = QString("create table if not exists %1"
				  "id integer primary key auto_increment, "
				  "predict_time date default '2017-01-01' not null,"
				  ).arg(Table_Power_Predict_72h_Final_Manual_24h);

	sql += SetFieldsName("v%1 float default 0, v%2_flag tinyint default 0,", Power_Predict_24h_Points);
	sql.chop(1);
	sql.append(")");
	Print_Debug_String(sql);
	Query_Debug_Error(sql);
}

/*! \fn void CPredictDatabase::Query_Debug_Error(const QString sql)
**********************************************************************
** \brief Query_Debug_Error
** \details 执行sql语句并返回结果
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*********************************************************************/
void CPredictDatabase::Query_Debug_Error(const QString sql)
{
	if ((*m_pPredictQuery).exec(sql))
	{
#if Is_Debug_Sql_Operation		
		qDebug() << Find_Key_Operator(sql) << "ok,success";
#endif
	} 
	else
	{
		qDebug()<< Find_Key_Operator(sql) << (*m_pPredictQuery).lastError();
	}
}


/*! \fn QString CPredictDatabase::Find_Key_Operator(const QString sql)
**********************************************************************
** \brief Find_Key_Operator
** \details 遍历sql语句是哪一种操作
** \return QString
** \author GuoHaijun
** \date   2017年5月27日
** \note
*********************************************************************/
QString CPredictDatabase::Find_Key_Operator(const QString sql)
{
	if (sql.indexOf("SELECT",0,Qt::CaseInsensitive) >= 0)
	{
		return "SELECT";
	} 
	else if(sql.indexOf("INSERT INTO",0,Qt::CaseInsensitive) >= 0)
	{
		return "INSERT INTO";
	}
	else if(sql.indexOf("UPDATE",0,Qt::CaseInsensitive) >= 0)
	{
		return "UPDATE";
	}
	else if(sql.indexOf("CREATE",0,Qt::CaseInsensitive) >= 0)
	{
		return "CREATE";
	}
	else if(sql.indexOf("DROP",0,Qt::CaseSensitive) >= 0)
	{
		return "DROP";
	}
	else if(sql.indexOf("DELETE",0,Qt::CaseInsensitive) >=0)
	{
		return "DELETE";
	}
	else
	{
		return "not in my define key operator";
	}
}

/*! \fn void CPredictDatabase::Print_Debug_String(const QString sql)
**********************************************************************
** \brief Print_Debug_String
** \details  调试时,打印输出sql信息
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*********************************************************************/
void CPredictDatabase::Print_Debug_String(const QString sql)
{
#if Is_Debug_Sql_String
	qDebug() << sql;
#endif
}

/*! \fn QString CPredictDatabase::SetFieldsName(const QString &para, const int fieldNum)
****************************************************************************************
** \brief SetFieldsName
** \details  设置字段名字
** \return QString
** \author GuoHaijun
** \date   2017年5月27日
** \note
***************************************************************************************/
QString CPredictDatabase::SetFieldsName(const QString &para, const int fieldNum)
{
	QString sql;
	int num = para.count('%');

	switch (num)
	{
		case 1:
			for (int i = 1; i <= fieldNum; i++)
			{
				sql += para.arg(i);
			}
			break;
		case 2:
			for (int i = 1; i <= fieldNum; i++)
			{
				sql += para.arg(i).arg(i);
			}
			break;
		default:
			;
	}
	return sql;
}

/*! \fn QString CPredictDatabase::SetFieldsValue(const data_type& value, const int flag, const int fieldIndex)
**************************************************************************************************************
** \brief SetFieldsValue
** \details  设置字段值
** \return QString
** \author GuoHaijun
** \date   2017年5月27日
** \note
*************************************************************************************************************/
QString CPredictDatabase::SetFieldsValue(const data_type& value, const int flag, const int fieldIndex)
{
	return QString(" v%1 = %2, v%3 = %4 ,").arg(fieldIndex).arg(value).arg(fieldIndex).arg(flag);
}

/*! \fn bool CPredictDatabase::GetAnalogue_Info(const Analogues_alias& alias, Analogues_Info& a_Info)
********************************************************************************************************************************
** \brief GetAnalogue_Info
** \details 从实时库获取模拟量信息
** \return bool
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetAnalogue_Info(const Analogues_Alias& alias, Analogues_Info& a_Info)
{
	//从实时库获取模拟量信息
	Q_UNUSED(a_Info);
	Q_UNUSED(alias);

	return true;
}


/*! \fn void CPredictDatabase::InsertData_WeatherType()
********************************************************************************************************************************
** \brief InsertData_WeatherType
** \details 写数据到气象类型表（用于测试）
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertData_WeatherType()
{
	Query_Debug_Error(QString("DELETE FORM weather_type"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Total_Radiation).arg("Total_Radiation"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Scattered_Radiation).arg("Scattered_Radiation"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Direct_Radiation).arg("Direct_Radiation"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Wind_Speed).arg("Wind_Speed"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Wind_Direction).arg("Wind_Direction"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Humidity).arg("Humidity"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Temperature).arg("Temperature"));
	Query_Debug_Error(QString("INSERT INTO weather_type values (%1,'%2')").arg(Weather_Air_Pressure).arg("Air_Pressure"));
}

/*! \fn void CPredictDatabase::InsertDataWeatherPredict4h_Final_15m(Weather_Predict_Data4h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict4h_Final_15m
** \details 天气数据来源为4h, 写入天气气象数据
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataWeatherPredict4h_Final_15m(Weather_Predict_Data4h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dateTimeFormat;
	QString dateTimeFormatActual = Set_Time_Format_Actual(insertTime);

	for (int j = 0; j < Weather_Predict_4h_Points; j++)
	{
		calDateTime = insertTime.addSecs(15 * 60 * j);
		dateTimeFormat = Set_Time_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' AND weather_type = %3 ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(dateTimeFormat).arg(data.type);
		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			int id = (*m_pPredictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%1', weather_type = %3, ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(dateTimeFormat).arg(data.type);

			sql.append(QString("%1").arg(SetFieldsValue(data.data[j], 0, 1)));

			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_4h_Final_15m).arg(dateTimeFormat);
			sql.append(QString("%1,").arg(data.data[j]));
			sql.append(QString("%1,").arg(0));

			sql.chop(1);
			sql.append(")");
		}

		Print_Debug_String(sql);

		Query_Debug_Error(sql);
	}
}

/*! \fn void CPredictDatabase::InsertDataWeatherPredict72h_Final_24h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict72h_Final_24h
** \details 写预测72h天气数据时,写入天气气象数据
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataWeatherPredict72h_Final_24h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dateTimeFormat;

	for (int day = 1; day <= 3; day++)
	{
		calDateTime = insertTime.addDays(day);
		dateTimeFormat = Set_Date_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time ='%2' ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			int id = (*m_pPredictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Power_Predict_24h_Points; index++)
			{
				sql.append(QString("%1").arg(SetFieldsValue(data.data[(day - 1)*Power_Predict_24h_Points + index - 1], 0, index)));
			}
			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Power_Predict_24h_Points; index++)
			{
				sql.append(QString("%1,").arg(data.data[(day - 1)*Power_Predict_24h_Points + index - 1]));
				sql.append(QString("%1,").arg(0));
			}

			sql.chop(1);
			sql.append(")");
		}

		Print_Debug_String(sql);

		Query_Debug_Error(sql);
	}
}

/*! \fn void CPredictDatabase::InsertData_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info)
********************************************************************************************************************************
** \brief InsertData_Hdb
** \details 插入历史数据,（用于测试）
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertData_Hdb(const QDateTime& insertTime, const QMap<Analogues_Alias, Analogues_Info>& map_Info)
{
	data_type hdb_day_288[288];
	int maxId = 0;

	for each(Analogues_Alias alias in map_Info.keys())
	{
		for (int i = 0; i < 288; i++)
		{
			hdb_day_288[i] = randValue(i);
		}

		maxId = 0;
		QString dataTimeFormat = Set_Date_Format(insertTime);

		QString sql = QString("SELECT * FROM %1 WHERE time = '%2' AND station = %3 AND device = %4 AND point = %5 ").arg(Table_Hdb).arg(dataTimeFormat).arg(map_Info[alias].station).arg(map_Info[alias].device).arg(map_Info[alias].point);
		(*m_pPredictQuery).exec(sql);

		if((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			int id = (*m_pPredictQuery).value(0).toInt();

			sql = QString("DELETE FROM %1 WHERE id = %2 ").arg(Table_Hdb).arg(id);
			(*m_pPredictQuery).exec(sql);

			if (alias == DaqoWeather_TotalRadiation)
			{
				
			} 
			else
			{
			}
		} 
		else
		{
		}

	}

}

/*
void CPredictDatabase::InsertData_All_Hdb(const QDateTime& insertTime, const QMap<Analogues_alias, Analogues_Info>& map_Info)
{


}*/

/*! \fn void CPredictDatabase::InsertDataPowerPredict4h_Final_15m(Power_Predict_Data4h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict4h_Final_15m
** \details 每15分钟预测一次，都需要存一次该表，以4h的16个点为单位
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataPowerPredict4h_Final_15m(Power_Predict_Data4h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dataTimeFormat;

	for (int j = 0; j < Weather_Predict_4h_Points; j++)
	{
		calDateTime = insertTime.addSecs(15 * 60 * j);
		dataTimeFormat = Set_Time_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_4h_Final_15m).arg(dataTimeFormat);
		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			int id = (*m_pPredictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(Table_Power_Predict_4h_Final_15m).arg(dataTimeFormat);
			sql.append(QString("%1").arg(SetFieldsValue(data.data[j], 0, 1)));

			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		}
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_4h_Final_15m).arg(dataTimeFormat);
			sql.append(QString("%1,").arg(data.data[j]));
			sql.append(QString("%1,").arg(0));

			sql.chop(1);
			sql.append(")");
		}
		Print_Debug_String(sql);
		Query_Debug_Error(sql);
	}
}

/*! \fn void CPredictDatabase::InsertDataPowerPredict72h_Final_24h(Power_Predict_Data72h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict72h_Final_24h
** \details 每15分钟预测一次，都需要存一次该表，以24h的96个点为单位
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataPowerPredict72h_Final_24h(Power_Predict_Data72h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dateTimeFormat;

	for (int day = 1; day <= 3; day++)
	{
		calDateTime = insertTime.addDays(day);
		dateTimeFormat = Set_Date_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			int id = (*m_pPredictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Power_Predict_24h_Points; index++)
			{
				sql.append(QString("%1").arg(SetFieldsValue(data.data[(day - 1)*Power_Predict_24h_Points + index - 1], 0, index)));
			}
			
			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_72h_Final_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Power_Predict_24h_Points; index++)
			{
				sql.append(QString("%1,").arg(data.data[(day - 1)*Power_Predict_24h_Points + index - 1]));
				sql.append(QString("%1,").arg(0));
			}
			sql.chop(1);
			sql.append(")");
		}

		Print_Debug_String(sql);
		Query_Debug_Error(sql);
	}
}

/*
void CPredictDatabase::InsertDataPowerPredict4h_Final_Manual_15m(Power_Predict_Data4h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dateTimeFormat;

	for (int j = 0; j < Weather_Predict_4h_Points; j++)
	{
		calDateTime = insertTime.addSecs(15 * 60 * j);
		dateTimeFormat = Set_Time_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time ='%2' ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(dateTimeFormat);
		(*predictQuery).exec(sql);

		if ((*predictQuery).next() == true)
		{
			(*predictQuery).seek(0);
			int id = (*predictQuery).value(0).toInt();

			sql = QStrin("UPDATE %1 SET predict_time = '%2', ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(dateTimeFormat);

			sql.append(QString("%1").arg(SetFieldsValue(data.data[j], 0, 1)));
			
			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(dateTimeFormat);
			sql.append(QString("%1").arg(data.data[j]));
			sql.append(QString("%1").arg(0));

			sql.chop(1);
			sql.append(")");
		}

		Print_Debug_String(sql);

		Query_Debug_Error(sql);
	}
}*/

/*
void CPredictDatabase::InsertDataPowerPredict72h_Final_Manual_24h(Power_Predict_Data72h& data, const QDateTime& insertTime)
{
	QDateTime calDateTime;
	QString dateTimeFormat;

	for (int day = 1; day <= 3; day++)
	{
		calDateTime = insertTime.addDays(day);
		dateTimeFormat = Set_Time_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_72h_Final_Manual_24h).arg(dateTimeFormat);
		(*predictQuery).exec(sql);

		if ((*predictQuery).next() == true)
		{
			(*predictQuery).seek(0);
			int id = (*predictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', ").arg(Table_Power_Predict_72h_Final_Manual_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Power_Predict_24h_Points; index)
			{
				sql.append(QString("%1").arg(SetFieldsValue(data.data[(day - 1)*Power_Predict_24h_Points + index - 1], 0, index)));
			}
			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2', ").arg(Table_Power_Predict_72h_Final_Manual_24h).arg(dateTimeFormat);
			for (int index = 1; index <= Weather_Predict_24h_Points; index)
			{
				sql.append(QString("%1,").arg(data.data[(day - 1)*Power_Predict_24h_Points + index - 1]));
				sql.append(QString("%1,").arg(0);
			
			}

			sql.chop(1);
			sql.append(")");
		}
		
		Print_Debug_String(sql);

		Query_Debug_Error(sql);
	}
}*/

/*! \fn void CPredictDatabase::InsertDataPowerPredict4h(const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict4h
** \details 未来4h预测数据通过此函数写入预测功率4h表（用于测试）
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
/*
void CPredictDatabase::InsertDataPowerPredict4h(const QDateTime& insertTime)
{
	InsertDataPowerPredict4h(powerPredictData4h, insertTime);
}*/

/*! \fn void CPredictDatabase::InsertDataPowerPredict4h(Power_Predict_Data4h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict4h
** \details 未来4h预测数据通过此函数写入预测功率4h表
** \return void
** \author GuoHaijun
** \date   2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataPowerPredict4h(Power_Predict_Data4h& data, const QDateTime& insertTime)
{
	QString dateTimeFormat = Set_Time_Format(insertTime);
	QString dateTimeFormatActual = Set_Time_Format_Actual(insertTime);

	QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_4h).arg(dateTimeFormat);
	(*m_pPredictQuery).exec(sql);

	if ((*m_pPredictQuery).next() == true)
	{
		(*m_pPredictQuery).seek(0);
		int id = (*m_pPredictQuery).value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', ").arg(Table_Power_Predict_4h).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= Power_Predict_4h_Points; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i-1], 0, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	} 
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL, '%2','%3',").arg(Table_Power_Predict_4h).arg(dateTimeFormat).arg(dateTimeFormatActual);
		for (int i = 1; i <= Power_Predict_4h_Points; i++)
		{
			sql.append(QString("%1,").arg(data.data[i-1]));
			sql.append(QString("%1,").arg(0));
		}

		sql.chop(1);
		sql.append(")");
	}

	Print_Debug_String(sql);
	
	Query_Debug_Error(sql);

	InsertDataPowerPredict4h_Final_15m(data, insertTime);
}

/*! \fn void CPredictDatabase::InsertDataPowerPredict72h(const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict72h
** \details 未来72h预测数据通过此函数写入预测功率72h表（用于测试）
** \return void
** \author GuoHaijun
** \date  2017年5月27日
** \note
*******************************************************************************************************************************/
/*
void CPredictDatabase::InsertDataPowerPredict72h(const QDateTime& insertTime)
{
	InsertDataPowerPredict72h(powerPredictData72h, insertTime);
}*/

/*! \fn void CPredictDatabase::InsertDataPowerPredict72h(Power_Predict_Data72h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataPowerPredict72h
** \details 未来72h预测数据通过此函数写入预测功率72h表
** \return void
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataPowerPredict72h(Power_Predict_Data72h& data, const QDateTime& insertTime)
{
	QString dataTimeFormat = Set_Time_Format(insertTime);
	QString dataTimeFormatActual = Set_Time_Format_Actual(insertTime);

	QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_72h).arg(dataTimeFormat);
	(*m_pPredictQuery).exec(sql);

	if ((*m_pPredictQuery).next() == true)
	{
		(*m_pPredictQuery).seek(0);
		int id = (*m_pPredictQuery).value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', ").arg(Table_Power_Predict_72h).arg(dataTimeFormat).arg(dataTimeFormatActual);
		for (int i = 0; i < Power_Predict_72h_Points; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i - 1], 0, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	} 
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL, '%2', '%3',").arg(Table_Power_Predict_72h).arg(dataTimeFormat).arg(dataTimeFormatActual);
		for (int i = 0; i < Power_Predict_72h_Points; i++)
		{
			sql.append(QString("%1,").arg(data.data[i - 1]));
			sql.append(QString("%1,").arg(0));
		}

		sql.chop(1);
		sql.append(")");
	}

	Print_Debug_String(sql);

	Query_Debug_Error(sql);

	InsertDataPowerPredict72h_Final_24h(data, insertTime);
}

/*! \fn void CPredictDatabase::InsertDataWeatherPredict4h(const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict4h
** \details 写数据到预测4h气象表
** \return void
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
/*
void CPredictDatabase::InsertDataWeatherPredict4h(const QDateTime& insertTime)
{
	InsertDataWeatherPredict4h(weatherPredictData4h, insertTime);
}
*/

/*! \fn void CPredictDatabase::InsertDataWeatherPredict4h(Weather_Predict_Data4h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict4h
** \details 写入数据到预测4h气象表
** \return void
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataWeatherPredict4h(Weather_Predict_Data4h& data, const QDateTime& insertTime)
{
	QString dateTimeFormat;
	QString dateTimeFormatActual;
	QDateTime predictTime;

	if (m_weather4hSource == Weather4h_Source_ActualPredict4h)
	{
		dateTimeFormat = Set_Time_Format(insertTime.addSecs(15 * 60));
		dateTimeFormatActual = Set_Time_Format_Actual(insertTime);
		predictTime = insertTime.addSecs(15 * 60);
	} 
	else
	{
		dateTimeFormat = Set_Time_Format(insertTime);
		dateTimeFormatActual = Set_Time_Format_Actual(insertTime);
		predictTime = insertTime;
	}

	QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' AND weather_type = %3").arg(Table_Weather_Predict_4h).arg(dateTimeFormat).arg(data.type);
	(*m_pPredictQuery).exec(sql);

	if ((*m_pPredictQuery).next() == true)
	{
		(*m_pPredictQuery).seek(0);
		int id = (*m_pPredictQuery).value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', weather_type = %4, ").arg(Table_Weather_Predict_4h).arg(dateTimeFormat).arg(dateTimeFormatActual).arg(data.type);
		for(int i = 1; i <= Weather_Predict_4h_Points; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i-1], 0, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	} 
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3',%4,").arg(Table_Weather_Predict_4h).arg(dateTimeFormat).arg(dateTimeFormatActual).arg(data.type);
		for (int i = 1; i <= Weather_Predict_4h_Points;i++)
		{
			sql.append(QString("%1,").arg(data.data[i - 1]));
			sql.append(QString("%1,").arg(0));
		}
		
		sql.chop(1);
		sql.append(")");
	}

	Print_Debug_String(sql);

	Query_Debug_Error(sql);

	if(m_weather4hSource == Weather4h_Source_ActualPredict4h)
	{
		InsertDataWeatherPredict4h_Final_15m(data, predictTime);
	}
}

/*! \fn void CPredictDatabase::InsertDataWeatherPredict4h_From_Ftp72h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict4h_From_Ftp72h
** \details 4h天气预测两个源,分别写入数据到表
** \return void
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataWeatherPredict4h_From_Ftp72h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
{
	/*QDateTime calDateTime;
	QString dateTimeFormat;

	for (int day = 1; day <= 3; day++)
	{
		calDateTime = insertTime.addDays(day);
		dateTimeFormat = Set_Time_Format(calDateTime);

		QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' AND weather_type = %3 ").arg(Table_Weather_Predict_72h_Final_24h).arg(dateTimeFormat).arg(data.type);
		(*predictQuery).exec(sql);

		if ((*predictQuery).next() == true)
		{
			(*predictQuery).seek(0);
			int id = (*predictQuery).value(0).toInt();

			sql = QString("UPDATE %1 SET predict_time = '%2', weather_type = %3, ").arg(Table_Weather_Predict_72h_Final_24h).arg(dateTimeFormat).arg(data.type);
			for (int index = 1; index <= Weather_Predict_24h_Points; index++)
			{
				sql.append(QString("%1,").arg(SetFieldsValue(data.data[(day - 1)*Weather_Predict_24h_Points + index - 1], 0, index)));				
			}
			sql.chop(1);
			sql += QString("WHERE id = %1").arg(id);
		} 
		else
		{
			sql = QString("INSERT INTO %1 VALUES (NULL,'%2',%3,").arg(Table_Weather_Predict_72h_Final_24h).arg(dateTimeFormat).arg(data.type);
			for (int index = 1; index <= Weather_Predict_24h_Points; index++)
			{
				sql.append(QString("%1,").arg(data.data[(day - 1)*Weather_Predict_24h_Points + index - 1]));
				sql.append(QString("%1,").arg(0);
			}
			sql.chop(1);
			sql.append(")");
		}

		Print_Debug_String(sql);

		Query_Debug_Error(sql);
	}*/

	QDateTime predict4hStartTime = QDateTime::fromString(insertTime.toString("yyyy-MM-dd 00:00:00"), "yyyy-MM-dd hh:mm::ss").addDays(1);
	QDateTime predict4hEndTime = predict4hStartTime.addSecs(60 * 60 * 24 * 3 - 60 * 60 * 4);
	QDateTime calDateTime;
	int index = 0;
	Weather_Predict_Data4h data4h;

	for(calDateTime = predict4hStartTime; calDateTime <= predict4hEndTime; calDateTime=calDateTime.addSecs(15*60))
	{
		data4h.type = data.type;
		for (int i = 0; i < Weather_Predict_4h_Points; i++)
		{
			data4h.data[i] = data.data[index + i];
		}
		InsertDataWeatherPredict4h(data4h, calDateTime);
		index++;
	}
	
	index = 0;
	if (m_weather4hSource == Weather4h_Source_FtpPredict72h)
	{
		for (calDateTime = predict4hStartTime; calDateTime <= predict4hEndTime; calDateTime=calDateTime.addSecs(15*60))
		{
			index++;
			QString dateTimeFormat = Set_Time_Format(calDateTime);

			QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' AND weather_trpe = %3").arg(Table_Weather_Predict_4h_Final_15m).arg(dateTimeFormat).arg(data.type);
			(*m_pPredictQuery).exec(sql);

			if((*m_pPredictQuery).next() == true)
			{
				(*m_pPredictQuery).seek(0);
				int id = (*m_pPredictQuery).value(0).toInt();

				sql = QString("UPDATE %1 SET predict_time = '%2', weather_type = %3, ").arg(Table_Weather_Predict_4h_Final_15m).arg(dateTimeFormat).arg(data.type);
				sql.append(QString("%1").arg(SetFieldsValue(data.data[index],0,1)));
				sql.chop(1);
				sql += QString("WHERE id = %1").arg(id);
			} 
			else
			{
				sql = QString("INSERT INTO %1 VALUES(NULL, '%2', %3,").arg(Table_Weather_Predict_4h_Final_15m).arg(dateTimeFormat).arg(data.type);
				sql.append(QString("%1,").arg(data.data[index]));
				sql.append(QString("%1,").arg(0));
				sql.chop(1);
				sql.append(")");
			}

			Print_Debug_String(sql);
		
			Query_Debug_Error(sql);
		}
	}
}

/*! \fn void CPredictDatabase::InsertDataWeatherPredict72h(const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict72h
** \details 写数据到预测72h气象表
** \return void
** \author GuoHaijun
** \date  2017年5月27日
** \note
*******************************************************************************************************************************/
/*
void CPredictDatabase::InsertDataWeatherPredict72h(const QDateTime& insertTime)
{
	InsertDataWeatherPredict72h(weatherPredictData72h, insertTime);
}*/

/*! \fn void CPredictDatabase::InsertDataWeatherPredict72h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
********************************************************************************************************************************
** \brief InsertDataWeatherPredict72h
** \details 写数据到预测72h气象表
** \return void
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
void CPredictDatabase::InsertDataWeatherPredict72h(Weather_Predict_Data72h& data, const QDateTime& insertTime)
{
	QString dateTimeFormat = Set_Time_Format_72h(insertTime);
	QString dateTimeFormatActual = Set_Time_Format_Actual(insertTime);

	QString sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' AND weather_type = %3").arg(Table_Power_Predict_72h).arg(dateTimeFormat).arg(data.type);
	(*m_pPredictQuery).exec(sql);

	if ((*m_pPredictQuery).next() == true)
	{
		(*m_pPredictQuery).seek(0);
		int id = (*m_pPredictQuery).value(0).toInt();

		sql = QString("UPDATE %1 SET predict_time = '%2', insert_time = '%3', weather_type = %4, ").arg(Table_Power_Predict_72h).arg(dateTimeFormat).arg(dateTimeFormatActual).arg(data.type);
		for (int i = 1; i <= Weather_Predict_72h_Points; i++)
		{
			sql.append(QString("%1").arg(SetFieldsValue(data.data[i - 1], 0, i)));
		}

		sql.chop(1);
		sql += QString("WHERE id = %1").arg(id);
	} 
	else
	{
		sql = QString("INSERT INTO %1 VALUES (NULL,'%2','%3',%4,").arg(Table_Weather_Predict_72h).arg(dateTimeFormat).arg(dateTimeFormatActual).arg(data.type);
		for (int i = 1; i < Weather_Predict_72h_Points; i++)
		{
			sql.append(QString("%1,").arg(data.data[i-1]));
			sql.append(QString("%1,").arg(0));
		}

		sql.chop(1);
		sql.append(")");
	}

	Print_Debug_String(sql);

	Query_Debug_Error(sql);

	InsertDataWeatherPredict72h_Final_24h(data, insertTime);

	if (m_weather4hSource == Weather4h_Source_FtpPredict72h)
	{
		InsertDataWeatherPredict4h_From_Ftp72h(data, insertTime);
	}
}

/*! \fn QString CPredictDatabase::Set_Date_Format(const QDateTime& dateTime)
****************************************************************************
** \brief Set_Date_Format
** \details 按固定格式显示年月日（yyyy-MM-dd）
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
***************************************************************************/
QString CPredictDatabase::Set_Date_Format(const QDateTime& dateTime)
{
	return QString("%1").arg(dateTime.toString("yyyy-MM-dd"));
}

/*! \fn QString CPredictDatabase::Set_Time_Format()
**********************************************************************
** \brief Set_Time_Format
** \details 按固定格式显示当前时间（yyyy-MM-dd hh:mm:00)
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
*********************************************************************/
QString CPredictDatabase::Set_Time_Format()
{
	QDateTime date = QDateTime::currentDateTime();

	int hour = date.time().hour();
	int minu = (date.time().minute() / Minute15)*Minute15;

	return QString("%1 %2:%3:00").arg(date.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));
}

/*! \fn QString CPredictDatabase::Set_Time_Format_Actual()
**********************************************************************
** \brief Set_Time_Format_Actual
** \details 按固定格式显示当前时间（yyyy-MM-dd hh:mm:ss)
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
*********************************************************************/
QString CPredictDatabase::Set_Time_Format_Actual()
{
	QDateTime date = QDateTime::currentDateTime();

	int hour = date.time().hour();
	int minu = date.time().minute();
	int sec = date.time().second();

	return QString("%1 %2:%3:%4").arg(date.toString("yyyy-MM-dd")).arg(hour, 0, 10, QChar('0')).arg(minu, 0, 10, QChar('0')).arg(sec, 0, 10, QChar('0'));
}

/*! \fn QString CPredictDatabase::Set_Time_Format(const QDateTime& dateTime)
****************************************************************************
** \brief Set_Time_Format
** \details 按固定格式显示给定时间dateTime（yyyy-MM-dd hh:mm:00)
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
***************************************************************************/
QString CPredictDatabase::Set_Time_Format(const QDateTime& dateTime)
{
	int hour = dateTime.time().hour();
	int minu = (dateTime.time().minute() / Minute15)*Minute15;

	return QString("%1 %2:%3:00").arg(dateTime.toString("yyyy-MM-dd")).arg(hour, 2, 10, QChar('0')).arg(minu, 2, 10, QChar('0'));
}

/*! \fn QString CPredictDatabase::Set_Time_Format_Actual(const QDateTime& dateTime)
***********************************************************************************
** \brief Set_Time_Format_Actual
** \details 按固定格式显示给定时间dateTime（yyyy-MM-dd hh:mm:ss)
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
**********************************************************************************/
QString CPredictDatabase::Set_Time_Format_Actual(const QDateTime& dateTime)
{
	int hour = dateTime.time().hour();
	int minu = dateTime.time().minute();
	int sec = dateTime.time().second();

	return QString("%1 %2:%3:%4").arg(dateTime.toString("yyyy-MM-dd")).arg(hour, 0, 10, QChar('0')).arg(minu, 0, 10, QChar('0')).arg(sec, 0, 10, QChar('0'));
}

/*! \fn QString CPredictDatabase::Set_Time_Format_72h(const QDateTime& dateTime)
***********************************************************************************
** \brief Set_Time_Format_72h
** \details 根据上下午天气预测时间确定更新时间
** \return QString
** \author GuoHaijun
** \date 2017年5月27日
** \note
**********************************************************************************/
QString CPredictDatabase::Set_Time_Format_72h(const QDateTime& dateTime)
{
	int hour = dateTime.time().hour();

	if (hour < Power_Predict_Afternon_Hour)
	{
		return QString("%1 08:00:00").arg(dateTime.toString("yyyy-MM-dd"));
	} 
	else
	{
		return QString("%1 16:00:00").arg(dateTime.toString("yyyy-MM-dd"));
	}
}

/*! \fn bool CPredictDatabase::GetData_WeatherPredict4h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime)
********************************************************************************************************************************
** \brief GetData_WeatherPredict4h
** \details 从预测数据库中获取预测4h气象数据
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetData_WeatherPredict4h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime)
{
	bool isOk = true;
	QString sql = "";

	for each (const Weather_Type& key in map.keys())
	{
		sql = QString("SELECT * FROM %1 WHERE weather_type = %2 AND predict_time = '%3' ORDER BY predict_time DESC ").arg(Table_Weather_Predict_4h).arg(key).arg(Set_Time_Format(getTime));

		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			for (int i = 0; i < Weather_Predict_4h_Points; i++)
			{
				map[key][i] = (*m_pPredictQuery).value(Predict_Value_First_Field + i * 2).toInt();
			}
		} 
		else
		{
			isOk = false;
		}
	}
	return isOk;
}

/*! \fn bool CPredictDatabase::GetData_WeatherPredict72h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime)
********************************************************************************************************************************
** \brief GetData_WeatherPredict72h
** \details 从预测数据库中获取预测72h气象数据
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetData_WeatherPredict72h(QMap<Weather_Type, Weather_Predict_Data>& map, const QDateTime &getTime)
{
	bool isOk = true;
	QString sql = "";

	for each (const Weather_Type& key in map.keys())
	{
		sql = QString("SELECT * FROM %1 WHERE weather_type = %2 AND predict_time = '%3' ORDER BY predict_time DESC ").arg(Table_Weather_Predict_72h).arg(key).arg(Set_Time_Format(getTime));

		(*m_pPredictQuery).exec(sql);

		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			for (int i = 0; i < Weather_Predict_72h_Points; i++)
			{
				map[key][i] = (*m_pPredictQuery).value(Predict_Value_First_Field + i * 2).toInt();
			}
		} 
		else
		{
			isOk = false;
		}

		return isOk;
	}

	return true;
}

/*! \fn QVector<data_type> CPredictDatabase::GetMaintenanceValue(QDateTime startTime, QDateTime endTime)
********************************************************************************************************************************
** \brief GetMaintenanceValue
** \details 从预测数据库中获取检修值
** \return QVector<data_type>
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
QVector<data_type> CPredictDatabase::GetMaintenanceValue(QDateTime startTime, QDateTime endTime)
{
	QVector<data_type> vecMaintenanceValue;
	QString sql;

	QDateTime tmp = startTime;

	while (tmp <= endTime)
	{
		sql = QString("SELECT * FROM %1 WHERE starttime <= '%2' and endtime >= '%3' ").arg(Table_RunningCapacity).arg(tmp.toString("yyyy-MM-dd hh:mm:ss")).arg(tmp.toString("yyyy-MM-dd hh:mm:ss"));

		(*m_pPredictQuery).exec(sql);
		if ((*m_pPredictQuery).next() == true)
		{
			QSqlRecord record = (*m_pPredictQuery).record();
			float value = record.value("value").toFloat();
			vecMaintenanceValue.append(value);
		} 
		else
		{
			vecMaintenanceValue.append(0);
		}

		tmp = tmp.addSecs(60 * 15);
	}
	return vecMaintenanceValue;
}

/*! \fn bool CPredictDatabase::GetData_PowerPredict4h(const QDateTime& time, QVector<data_type>& vecValue)
********************************************************************************************************************************
** \brief GetData_PowerPredict4h
** \details 从预测数据库中获取4h功率预测值
** \return bool
** \author GuoHaijun
** \date  2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetData_PowerPredict4h(const QDateTime& time, QVector<data_type>& vecValue)
{
	bool isOk = true;
	QDateTime nextDay;

	QString sql;

	for (int i = 0; i < Power_Predict_4h_Points; i++)
	{
		nextDay = time.addSecs(60 * 15 * i);
		sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_4h_Final_Manual_15m).arg(Set_Time_Format(nextDay));

		(*m_pPredictQuery).exec(sql);
		if ((*m_pPredictQuery).next() == true)
		{
			(*m_pPredictQuery).seek(0);
			vecValue.append((*m_pPredictQuery).value(2).toFloat());
		} 
		else
		{
			isOk = false;
		}
	}
	return isOk;
}

/*! \fn bool CPredictDatabase::GetData_PowerPredict24h(const QDateTime& time, QVector<data_type>& vecValue)
********************************************************************************************************************************
** \brief GetData_PowerPredict24h
** \details 从预测数据库中获取24h功率预测值
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetData_PowerPredict24h(const QDateTime& time, QVector<data_type>& vecValue)
{
	bool isOK = true;
	QDateTime nextDay = time.addDays(1);

	QString sql;
	sql = QString("SELECT * FROM %1 WHERE predict_time = '%2' ").arg(Table_Power_Predict_72h_Final_Manual_24h).arg(Set_Time_Format(nextDay));

	(*m_pPredictQuery).exec(sql);

	if ((*m_pPredictQuery).next() == true)
	{
		(*m_pPredictQuery).seek(0);

		for (int i=0; i < Power_Predict_24h_Points; i++)
		{
			vecValue.append((*m_pPredictQuery).value(2 + i * 2).toFloat());
		}
	} 
	else
	{
		isOK = false;
	}

	return isOK;
}

/*! \fn bool CPredictDatabase::GetInverters_Status_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos)
********************************************************************************************************************************
** \brief GetInverters_Status_Info
** \details 从实时库获取逆变器状态信息
** \return bool
** \author GuoHaijun
** \date  2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetInverters_Status_Info(const Analogues_Alias& alias, QList<Inverter_Info>& a_infos)
{
	//从实时库获取逆变器状态信息
	Q_UNUSED(alias);
	Q_UNUSED(a_infos);

	return true;
}

/*! \fn bool CPredictDatabase::GetInverters_Power_Info(const Analogues_alias& alias, QList<Inverter_Info>& a_infos)
********************************************************************************************************************************
** \brief GetInverters_Status_Info
** \details 从实时库获取逆变器功率信息
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetInverters_Power_Info(const Analogues_Alias& alias, QList<Inverter_Info>& a_infos)
{
	//从实时库获取逆变器功率信息
	Q_UNUSED(alias);
	Q_UNUSED(a_infos);
	return true;
}

/*! \fn bool CPredictDatabase::GetAnalogueInfoMap(QList<Analogues_alias>& alias)
********************************************************************************************************************************
** \brief GetAnalogueInfoMap
** \details 获取模拟量信息map
** \return QMap<Analogues_alias, Analogues_Info>
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
QMap<Analogues_Alias, Analogues_Info> CPredictDatabase::GetAnalogueInfoMap(QList<Analogues_Alias>& alias)
{
	QMap<Analogues_Alias, Analogues_Info> map;
	Analogues_Info a_info;

	for each (const Analogues_Alias& str in alias)
	{
		if (CPredictDatabase::GetInstance().GetAnalogue_Info(str, a_info))
		{
			map.insert(str, a_info);
		} 
		else
		{
			qDebug() << "enable to get " << str << "information.";
		}
	}

	return map;
}

/*! \fn bool CPredictDatabase::GetInverters_StatusMap(QMap<Analogues_alias, QList<Inverter_Info> >& map)
********************************************************************************************************************************
** \brief GetInverters_StatusMap
** \details 获取逆变器状态map
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetInverters_StatusMap(QMap<Analogues_Alias, QList<Inverter_Info> >& map)
{
	QList<Inverter_Info> a_infos;

	if (GetInverters_Status_Info(DaqoInverter_Status, a_infos))
	{
		map.insert(DaqoInverter_Status, a_infos);

		return true;
	}

	return false;
}

/*! \fn bool CPredictDatabase::GetInverters_ActivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map)
********************************************************************************************************************************
** \brief GetInverters_ActivePowerMap
** \details 获取逆变器有功map
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetInverters_ActivePowerMap(QMap<Analogues_Alias, QList<Inverter_Info> >& map)
{
	QList<Inverter_Info> a_infos;

	if (GetInverters_Power_Info(DaqoInverter_ActivePoewer, a_infos))
	{
		map.insert(DaqoInverter_ActivePoewer, a_infos);
		return true;
	}

	return false;
}

/*! \fn bool CPredictDatabase::GetInverters_ReactivePowerMap(QMap<Analogues_alias, QList<Inverter_Info> >& map)
********************************************************************************************************************************
** \brief GetInverters_ReactivePowerMap
** \details 获取逆变器无功map
** \return bool
** \author GuoHaijun
** \date 2017年5月27日
** \note
*******************************************************************************************************************************/
bool CPredictDatabase::GetInverters_ReactivePowerMap(QMap<Analogues_Alias, QList<Inverter_Info> >& map)
{
	QList<Inverter_Info> a_infos;

	if (GetInverters_Power_Info(DaqoInverter_ReactivePower, a_infos))
	{
		map.insert(DaqoInverter_ReactivePower, a_infos);

		return true;
	}

	return false;
}


int CPredictDatabase::randValue(int value)
{
	bool isRand = false;

	return (isRand ? (qrand() % 100) : (value + 1));
}