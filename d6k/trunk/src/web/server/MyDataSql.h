#ifndef _MYDATASQL_H
#define _MYDATASQL_H

#define CHAR_NAME 24

#define  ANAOL_NORMAL           0		//正常
#define  ANAOL_HIGH             1		//越上限
#define  ANAOL_LOW              2		//越下限
#define  ANAOL_HIHI             3		//越上上限
#define  ANAOL_LOLO             4		//越下下限

#include <vector>
using namespace std;

#ifdef WIN32
#include <winsock2.h>
#include<windows.h>
#endif // WIN32


//#include "db/dbapi.h"
//#include "rdb/rdbapi.h"

#include "soapStub.h"

typedef struct tag_scd_dev{
	char   name[CHAR_NAME];//设备代码
	unsigned short dev_type;	//设备类型
}SCDDEV,*PSCDDEV;


typedef std::vector<PSCDDEV> SCDDEV_VECTOR;
typedef std::vector<_ns2__RequestType_ID > ID_STR;

class  Db_connector
{
public:
	/**
	@brief 打开数据库
	@return 成功返回0，失败返回-1
	*/
	static int open(void);
	/**
	@brief 打开数据库
	@return 成功返回行数，失败返回-1
	*/
	static int select(const char* sql, void**result_pt);
	/**
	@brief 关闭数据库
	*/
	static void close(void);
};

//0:失败 1：部分成功 2:成功
class RdbValueGet_B
{
public:
	RdbValueGet_B(size_t request_idnum);
	~RdbValueGet_B();
public:
	int Get_Data_From_Database( ID_STR* id_str,ns2__PayloadType* payload, std::string request_type );

private:
	int Get_ArryValues(ns3__ArrayValuesType* arry_value, std::string arry_name);

	int Get_DeviceValues(ns3__DeviceValuesType* dev_value, std::string dev_name);

	bool Get_CommonValues(ns3__CommonValuesType* commonvalue, std::string table_name, std::string name, std::string field);

	bool GetSingleData(ns3__CommonValuesType* commonvalue, char *tablename, char* name, char*field);

	bool GetMeasureData(ns3__DeviceValuesType* dev_value,char* devname);

	bool GetSignalData(ns3__DeviceValuesType* dev_value,char* devname);

	bool GetDegreeData(ns3__DeviceValuesType* dev_value,char* devname);

	int Get_ArryDevices(char* arry_code, SCDDEV_VECTOR* scddev_vector);	//获取光复矩阵的所有设备 以设备类型区分

	int split(const std::string& str, vector<std::string>& ret_, std::string sep);

	int Get_DataSet_Value(ns2__PayloadType* payload, int count);
private:
	//Rdb_QuickPolling* m_polling_read;
	//RDB_FIELD_STRU* m_rdbFields;

private:

	bool AnalyzeString(const std::string& strKey, int& nStation, int& nDevice, int& nID);

	// 从实时库中获取ai数据
	bool GetAiFromRDB(int nStation, int nDevice, int id, float& fValue);

	// 从实时库中获取di数据
	bool GetDiFromRDB(int nStation, int nDevice, int id, float& fValue);

	// 从实时库中获取dd数据
	bool GetDDFromRDB(int nStation, int nDevice, int id, float& fValue);

	// 从实时库中获取calc数据
	bool GetCalcFromRDB(int nStation, int nDevice, int id, float& fValue);
};

#endif


 