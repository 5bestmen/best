#ifndef _MYDATASQL_H
#define _MYDATASQL_H

#define CHAR_NAME 24

#define  ANAOL_NORMAL           0		//����
#define  ANAOL_HIGH             1		//Խ����
#define  ANAOL_LOW              2		//Խ����
#define  ANAOL_HIHI             3		//Խ������
#define  ANAOL_LOLO             4		//Խ������

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
	char   name[CHAR_NAME];//�豸����
	unsigned short dev_type;	//�豸����
}SCDDEV,*PSCDDEV;


typedef std::vector<PSCDDEV> SCDDEV_VECTOR;
typedef std::vector<_ns2__RequestType_ID > ID_STR;

class  Db_connector
{
public:
	/**
	@brief �����ݿ�
	@return �ɹ�����0��ʧ�ܷ���-1
	*/
	static int open(void);
	/**
	@brief �����ݿ�
	@return �ɹ�����������ʧ�ܷ���-1
	*/
	static int select(const char* sql, void**result_pt);
	/**
	@brief �ر����ݿ�
	*/
	static void close(void);
};

//0:ʧ�� 1�����ֳɹ� 2:�ɹ�
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

	int Get_ArryDevices(char* arry_code, SCDDEV_VECTOR* scddev_vector);	//��ȡ�⸴����������豸 ���豸��������

	int split(const std::string& str, vector<std::string>& ret_, std::string sep);

	int Get_DataSet_Value(ns2__PayloadType* payload, int count);
private:
	//Rdb_QuickPolling* m_polling_read;
	//RDB_FIELD_STRU* m_rdbFields;

private:

	bool AnalyzeString(const std::string& strKey, int& nStation, int& nDevice, int& nID);

	// ��ʵʱ���л�ȡai����
	bool GetAiFromRDB(int nStation, int nDevice, int id, float& fValue);

	// ��ʵʱ���л�ȡdi����
	bool GetDiFromRDB(int nStation, int nDevice, int id, float& fValue);

	// ��ʵʱ���л�ȡdd����
	bool GetDDFromRDB(int nStation, int nDevice, int id, float& fValue);

	// ��ʵʱ���л�ȡcalc����
	bool GetCalcFromRDB(int nStation, int nDevice, int id, float& fValue);
};

#endif


 