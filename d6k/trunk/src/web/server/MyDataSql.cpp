#include "MyDataSql.h"
#include<sstream>

#include "rdb_define.h"

/////////////////////////////////////////////Db_connector/////////////////////////////
//���ݿ�������־��0������-1�쳣
static int s_db_stat = 0;
//static DBApi s_db;

int Db_connector::open(void)
{
	/*s_db_stat = CS_SUCCEED == s_db.Open() ? 0 : -1;
	return s_db_stat;*/

	return -1;
}

int Db_connector::select(const char* sql, void**result_pt)
{
	int ret = -1;
	/*
	//ֻ�����ݿ�����ʱ�����������ݿ�select
	if (!s_db_stat && sql != NULL)
	{
		CS_DATAFMT *datafmt = NULL;
		FUNC_STR func;
		func.func = SEL_ISQL_RESULTS;
		strcpy(func.dbname, DB_NAME);
		strcpy(func.isql, sql);

		if (CS_SUCCEED == s_db.SelectIsqlResults(&func, &datafmt, result_pt))
		{
			ret = func.ret_roxnum;
		}

		if (datafmt)
		{
			free(datafmt);
			datafmt = NULL;
		}
	}//if (!s_db_stat && sql != NULL)
	*/
	return ret;
}

void Db_connector::close(void)
{
	/*s_db.Close();
	s_db_stat = -1;*/
}


//////////////////////////////////////////////////////////////////////////
RdbValueGet_B::RdbValueGet_B(size_t request_idnum)
{
	/*m_polling_read = new Rdb_QuickPolling();
	m_rdbFields =  new RDB_FIELD_STRU[request_idnum*2];//value����Ա�־*/
}

RdbValueGet_B::~RdbValueGet_B()
{
	/*delete m_polling_read;
	delete []m_rdbFields;*/
}

int RdbValueGet_B::Get_ArryValues(ns3__ArrayValuesType* arry_value, std::string arry_name)
{
	int result = 0;
	arry_value->ArrayCode = arry_name;
	SCDDEV_VECTOR scddev_vertor;
	char* arry_name_char = const_cast<char*>(arry_name.c_str());
	Get_ArryDevices(arry_name_char, &scddev_vertor);
	
	U32INT cur_type = -1;
	ns3__DeviceClassType* dev_class_type = NULL;
	for (int i = 0;i<scddev_vertor.size();i++)
	{
		if (cur_type != scddev_vertor[i]->dev_type) //����������
		{
			cur_type = scddev_vertor[i]->dev_type;
			if(dev_class_type)
				arry_value->DeviceClasses.push_back(dev_class_type);
			dev_class_type = new ns3__DeviceClassType;
			char classtype[200];
			sprintf(classtype,"%d",cur_type);
			dev_class_type->ClassName = classtype;
		}
		//����SCD�豸Ԫ�أ�ѡȡ�������������
		ns3__DeviceValuesType *dev_value_type = new ns3__DeviceValuesType;
		result += Get_DeviceValues(dev_value_type, scddev_vertor[i]->name);
		dev_class_type->Devices.push_back(dev_value_type);
	}
	if (result == 3*scddev_vertor.size())
	{
		result = 3;
	}
	else if (result > 0)
	{
		result = 2;
	}
	return result;
}

int RdbValueGet_B::Get_DeviceValues(ns3__DeviceValuesType* dev_value, std::string dev_name)
{
	bool b_res = false;
	int  result = 0;
	dev_value->Code = dev_name;

	char select_name[200];
	memset(select_name,0,200);
	strncpy(select_name,dev_name.c_str(),dev_name.length());

	//ʵʱ�����
	b_res = GetMeasureData(dev_value,select_name);
	if (b_res)
	{
		result++;
	}
 	b_res = GetSignalData(dev_value,select_name);
	if (b_res)
	{
		result++;
	}
	b_res = GetMeasureData(dev_value,select_name);
	if (b_res)
	{
		result++;
	}

	return result;
}

//���ѡȡ ѡȡ����Ϊ�豸����
bool RdbValueGet_B::GetMeasureData(ns3__DeviceValuesType* dev_value,char* devname)
{
	/*Rdb_SqlCli rl;
	rl.RdbClearSQLCmd();
	rl.RdbSelectFrom("analog");
	rl.RdbOrderBy("ycno");
	rl.RdbSelectField("name");//YC����
	rl.RdbSelectField("olgrade");//Խ�޵ȼ�
	rl.RdbSelectField("value");	//ֵ
	rl.RdbSelectField("deadf");	//������־
	rl.RdbSelectField("invalidf");//�Ƿ���־

	rl.RdbWhereCond("devname",RDB_DATATYPE_STRING,devname);
	int	ret = rl.RdbSelCmd(1,"scada","scada");
	if(ret != RDB_OK)	return FALSE;
	if(ret==RDB_OK) {
		rl.RdbDispRcd();
	}

	int	RecNumber=rl.rdbgetrcdcnt();
	if (RecNumber==0) return FALSE;
	ns3__CommonValuesType* temp_data = NULL;
	for(int i=0;i<RecNumber;i++) 
	{
		temp_data = new ns3__CommonValuesType;
		temp_data->Key = rl.RdbGetString(i,"name");
		uint olgrade=rl.RdbGetVal_uint(i,"olgrade");
		float value =rl.RdbGetVal_float(i,"value");
		bool deadf =rl.RdbGetVal_uint(i,"deadf");
		bool invalidf=rl.RdbGetVal_uint(i,"invalidf");

		temp_data->Value = new std::string;
		std::stringstream ss;
		ss<<value;
		ss>>*(temp_data->Value);
		ss.clear();


// 		Rdb_Vartype_value value_type;
// 		rl.RdbGetColumnValue(i,"value",&value_type);
// 		temp_data->Value = new std::string(value_type.RdbGetString());

// 		std::string strvalue;
// // 		sprintf(strvalue,"%f",value);
// 		temp_data->Value = &strvalue;
		temp_data->UpperLimit = new bool(false);
		temp_data->LowerLimit = new bool(false);
		temp_data->UpperUpperLimit = new bool(false);
		temp_data->LowerLowerLimit = new bool(false);
		switch(olgrade)
		{
		case ANAOL_NORMAL:
			break;
		case ANAOL_HIGH:
			*(temp_data->UpperLimit) = true;
			break;
		case ANAOL_LOW:
			*(temp_data->LowerLimit) = true;
			break;
		case ANAOL_HIHI:
			*(temp_data->UpperUpperLimit) = true;
			break;
		case ANAOL_LOLO:
			*(temp_data->LowerLowerLimit) = true;
			break;
		default:
			break;
		}
		temp_data->Dead = &deadf;
		temp_data->Invalid = &invalidf;
		dev_value->RemoteMeasure.push_back(temp_data);
	}*/
	return true;
}

bool RdbValueGet_B::GetSignalData(ns3__DeviceValuesType* dev_value,char* devname)
{
	/*int ret = 0;
	void *raddr = NULL;
	int tmpint=1;
	Rdb_SqlCli rl;
	rl.RdbClearSQLCmd();

	char* table[] = {"break","prot","switch","status"};
	char *tablename = NULL;
	int tab_cnt = 3;

	//��ͬ�ı�
	for (int i = 0;i<tab_cnt;i++)
	{
		tablename = table[i];
		rl.RdbSelectFrom(tablename);
		rl.RdbSelectField("name");
		rl.RdbSelectField("describe");
		rl.RdbSelectField("value");
		rl.RdbSelectField("invalidf");
		rl.RdbOrderBy("yxno");
		rl.RdbWhereCond("devname",RDB_DATATYPE_STRING,devname);
		ret = rl.RdbSelCmd(1,"scada","scada");
		if(ret != RDB_OK) return false;
		if(ret==RDB_OK) {
			rl.RdbDispRcd();
		}

		int	RecNumber=rl.rdbgetrcdcnt();
		if (RecNumber==0) continue;

		ns3__CommonValuesType* temp_data = NULL;

		for(int i=0;i<RecNumber;i++) 
		{
			temp_data = new ns3__CommonValuesType;
			temp_data->Key = rl.RdbGetString(i,"name");
			uint yxvalue = rl.RdbGetVal_uint(i,"value");
			bool invalidf=rl.RdbGetVal_uint(i,"invalidf");

			temp_data->Value = new std::string;
			std::stringstream ss;
			ss<<yxvalue;
			ss>>*(temp_data->Value);
			ss.clear();
			temp_data->Invalid = &invalidf;


			temp_data->UpperLimit = new bool(false);
			temp_data->LowerLimit = new bool(false);
			temp_data->UpperUpperLimit = new bool(false);
			temp_data->LowerLowerLimit = new bool(false);
			temp_data->Dead = new bool(false);


			dev_value->RemoteSignal.push_back(temp_data);
		}
	}*/


	return false;
}

bool RdbValueGet_B::GetDegreeData(ns3__DeviceValuesType* dev_value,char* devname)
{
	/*Rdb_SqlCli rl;
	rl.RdbClearSQLCmd();

	rl.RdbSelectFrom("kwh");
	rl.RdbSelectField("kwhno");
	rl.RdbOrderBy("kwhno");
	rl.RdbSelectField("name");
	rl.RdbSelectField("tabcode");
	rl.RdbWhereCond("devname",RDB_DATATYPE_STRING,devname);
	int	ret = rl.RdbSelCmd(1,"scada","scada");
	if(ret != RDB_OK)	return FALSE;
	if(ret==RDB_OK) {
		rl.RdbDispRcd();
	}

	int	RecNumber=rl.rdbgetrcdcnt();
	if (RecNumber==0) return FALSE;

	ns3__CommonValuesType* temp_data = NULL;

	for(int i=0;i<RecNumber;i++) 
	{
		temp_data = new ns3__CommonValuesType;
		temp_data->Key = rl.RdbGetString(i,"name");
		double value = rl.RdbGetVal_double(i,"tabcode");

		temp_data->Value = new std::string;
		std::stringstream ss;
		ss<<value;
		ss>>*(temp_data->Value);
		ss.clear();

		temp_data->UpperLimit = new bool(false);
		temp_data->LowerLimit = new bool(false);
		temp_data->UpperUpperLimit = new bool(false);
		temp_data->LowerLowerLimit = new bool(false);
		temp_data->Dead = new bool(false);
		temp_data->Invalid = new bool(false);

		dev_value->RemoteDegree.push_back(temp_data);
	}*/
	return false;
}

bool RdbValueGet_B::Get_CommonValues(ns3__CommonValuesType* commonvalue, std::string table_name, std::string name, std::string field)
{
	commonvalue->Key = name;
	bool b_res = false;

	char* tablename = const_cast<char*>(table_name.c_str());
	char* dataname = const_cast<char*>(name.c_str());
	char* keyfield = const_cast<char*>(field.c_str());

	char test_name[200];
	memset(test_name,0,200);
	strncpy(test_name,name.c_str(),name.length());

	printf("tablename:%s, dataname:%s, fieldname: %s\n",tablename,test_name,keyfield);

	b_res = GetSingleData(commonvalue,tablename,test_name,keyfield);
	return b_res;
}

bool RdbValueGet_B::GetSingleData(ns3__CommonValuesType* commonvalue, char *tablename, char* name, char*field)
{
	/*Rdb_SqlCli rl;
	rl.RdbClearSQLCmd();
	rl.RdbSelectFrom(tablename);
	rl.RdbSelectField("name");
	rl.RdbSelectField(field);	//ֵ
	if (strcmp(tablename,"analog")==0 )	//YCֵ�Ż�ȡ����
	{
		rl.RdbSelectField("olgrade");//Խ�޵ȼ�
		rl.RdbSelectField("deadf");	//������־
	}
	if (strcmp(tablename,"kwh"))	//���˵�ȶ�Ҫȡ�Ƿ���־
	{
		rl.RdbSelectField("invalidf");//�Ƿ���־	
	}

	rl.RdbWhereCond("name",RDB_DATATYPE_STRING,name);

	int	ret = rl.RdbSelCmd(1,"scada","scada");
	if (ret<0)
	{
		RdbPrintErr("\n",ret);
	}
	if(ret != RDB_OK)	return false;
	if(ret==RDB_OK) {
		rl.RdbDispRcd();
	}
	int	RecNumber=rl.rdbgetrcdcnt();
	if (RecNumber==0) return false;


	RDB_VALUETYPE_STRU value_type;
	rl.RdbGetColumnValue(0,field,&value_type);
	
	char value_str[40];
	switch(value_type.valuetype) {
	case RDB_DATATYPE_UCHAR:
		sprintf(value_str,"%4u",value_type.value.ucharval);
		break;  
	case RDB_DATATYPE_USHORT:
		sprintf(value_str,"%4u",value_type.value.ushortval);
		break;  
	case RDB_DATATYPE_UINT:
		sprintf(value_str,"%4u",value_type.value.uintval);
		break;  
	case RDB_DATATYPE_CHAR:
		sprintf(value_str,"%5d",value_type.value.charval);
		break;
	case RDB_DATATYPE_SHORT:
		sprintf(value_str,"%5d",value_type.value.shortval);
		break;
	case RDB_DATATYPE_INT:
		sprintf(value_str,"%5d",value_type.value.uintval);
		break;
	case RDB_DATATYPE_LONG:
		sprintf(value_str,"%5d",value_type.value.longval);
		break;
	case RDB_DATATYPE_ULONG:
		sprintf(value_str,"%4u",value_type.value.ulongval);
		break;
	case RDB_DATATYPE_FLOAT:
		sprintf(value_str,"%8.3f",value_type.value.floatval);
		break;
	case RDB_DATATYPE_DOUBLE:
		sprintf(value_str,"%8.3f",value_type.value.doubleval);
		break;
	}

	commonvalue->Value = new std::string;
	*(commonvalue->Value) = value_str;

// 	float value = rl.RdbGetVal_float(0,field);
// 	std::stringstream ss;
// 	ss<<value;
// 	ss>>*(commonvalue->Value);
// 	ss.clear();

	if (strcmp(tablename,"analog")==0)
	{
		uint olgrade=rl.RdbGetVal_uint(0,"olgrade");
		bool deadf =rl.RdbGetVal_uint(0,"deadf");
		commonvalue->UpperLimit = new bool(false);
		commonvalue->LowerLimit = new bool(false);
		commonvalue->UpperUpperLimit = new bool(false);
		commonvalue->LowerLowerLimit = new bool(false);
		switch(olgrade)
		{
		case ANAOL_NORMAL:
			break;
		case ANAOL_HIGH:
			*(commonvalue->UpperLimit) = true;
			break;
		case ANAOL_LOW:
			*(commonvalue->LowerLimit) = true;
			break;
		case ANAOL_HIHI:
			*(commonvalue->UpperUpperLimit) = true;
			break;
		case ANAOL_LOLO:
			*(commonvalue->LowerLowerLimit) = true;
			break;
		default:
			break;
		}
		commonvalue->Dead = new bool(false);
		*(commonvalue->Dead) = deadf;
	}

	if (strcmp(tablename,"kwh"))
	{
		bool invalidf = rl.RdbGetVal_uint(0,"invalidf");
		commonvalue->Invalid = new bool(false);
		*(commonvalue->Invalid) = invalidf;

	}*/
	return true;
}
//////////////////////////////////////////////////////////////////////////

int RdbValueGet_B::Get_Data_From_Database( ID_STR* id_str, ns2__PayloadType* payload, std::string request_type )
{
	const int DEFAULT_GROUP_NO = 1;
	int ret = 0;
	/*if (!m_rdbFields)
	{
		return ret;
	}*/
	for (int i=0; i < id_str->size(); i++)
	{
		// ʵʱ������key�����ݹ�������󣬴������Լ���ʵʱ���л�ȡʵʱ���ݡ�
		std::string strReqKeyId =  id_str->at(i).__item;

		//std::cout<<"req_data:"<<request_data.c_str()<<std::endl;

		if (request_type == "ArrayValues")
		{
			if (!payload->union_PayloadType.ArrayValues)
			{
				return 0;
			}
			ns3__ArrayValuesType* arry_value = new ns3__ArrayValuesType;
			ret += Get_ArryValues(arry_value, strReqKeyId);
			payload->union_PayloadType.ArrayValues->push_back(arry_value);

		}
		else if (request_type == "DeviceValues")
		{
			if (!payload->union_PayloadType.DeviceValues)
			{
				return 0;
			}
			ns3__DeviceValuesType* dev_value = new ns3__DeviceValuesType;
			ret += Get_DeviceValues(dev_value,strReqKeyId);
			payload->union_PayloadType.DeviceValues->push_back(dev_value);
		}
		else if (request_type == "CommonValues")
		{
			/*if (!payload->union_PayloadType.CommonValues)
			{
				return 0;
			}
			ns3__CommonValuesType* commonvalue_ptr = new ns3__CommonValuesType;
			std::vector<std::string> strlist;
			split(request_data, strlist,"+");
			if (strlist.size() < 3)
			{
				return 0;
			}
			memset(&m_rdbFields[2*i], 0, sizeof(RDB_FIELD_STRU));
			m_rdbFields[2*i].groupno = DEFAULT_GROUP_NO;
			strcpy(m_rdbFields[2*i].tabname, strlist[0].c_str() );
			strcpy(m_rdbFields[2*i].objname, strlist[1].c_str() );
			strcpy(m_rdbFields[2*i].fldname, strlist[2].c_str() );

			memset(&m_rdbFields[2*i + 1], 0, sizeof(RDB_FIELD_STRU));
			m_rdbFields[2*i +1].groupno = DEFAULT_GROUP_NO;
			strcpy( m_rdbFields[2*i + 1].tabname, strlist[0].c_str() );
			strcpy( m_rdbFields[2*i + 1].objname, strlist[1].c_str());
			if (strlist[0] == "analog")
			{
				strcpy( m_rdbFields[2*i + 1].fldname, "ycinconsistf" );
			}
			else
			{
				strcpy( m_rdbFields[2*i + 1].fldname, "yxinconsistf" );
			}
			commonvalue_ptr->Key = strlist[1];
			payload->union_PayloadType.CommonValues->push_back(commonvalue_ptr);*/
		}
		else if (request_type == "SimpleValues")
		{
			//std::string table_name,table_type;
			// "1-1-1-0100"��
			if (strReqKeyId.size() < 10)
			{
				continue;
			}
			/*table_type = request_data.substr(request_data.size()- 4,4);
			int type = atoi( table_type.c_str());
			if(type > 299 && type < 306)//break
			{
				table_name = "break";
			}
			else if (type > 399 && type < 499)
			{
				table_name = "status";
			}
			else if (type > 499 && type < 576)//switch
			{
				table_name = "switch";
			}
			else if (type > 599 && type <603)//prot
			{
				table_name = "prot";
			}
			else if (type >199 && type <207)	//kwh
			{
				table_name = "kwh";
			}
			else	//analog
			{
				table_name = "analog";
			}*/

			if (!payload->union_PayloadType.CommonValues)
			{
				return 0;
			}
			ns3__CommonValuesType* commonvalue_ptr = new ns3__CommonValuesType;
			commonvalue_ptr->Key = strReqKeyId;
			payload->union_PayloadType.CommonValues->push_back(commonvalue_ptr);

			/*memset(&m_rdbFields[2*i], 0, sizeof(RDB_FIELD_STRU));
			m_rdbFields[2*i].groupno = DEFAULT_GROUP_NO;
			strcpy( m_rdbFields[2*i].tabname, table_name.c_str() );
			strcpy( m_rdbFields[2*i].objname, request_data.c_str());
			strcpy( m_rdbFields[2*i].fldname, "value" );

			memset(&m_rdbFields[2*i + 1], 0, sizeof(RDB_FIELD_STRU));
			m_rdbFields[2*i +1].groupno = DEFAULT_GROUP_NO;
			strcpy( m_rdbFields[2*i + 1].tabname, table_name.c_str() );
			strcpy( m_rdbFields[2*i + 1].objname, request_data.c_str());
			if (table_name == "analog")
			{
				strcpy( m_rdbFields[2*i + 1].fldname, "ycinconsistf" );
			}
			else if (table_name == "kwh")
			{
				strcpy( m_rdbFields[2*i].fldname, "tabcode" );
				strcpy( m_rdbFields[2*i + 1].fldname, "terminalerror" );
			}
			else
			{
				strcpy( m_rdbFields[2*i + 1].fldname, "yxinconsistf" );
			}*/
		}	
	}

	//���ݼ���ʽ����polling��ȡ��ʽ
	if (request_type == "SimpleValues" || request_type == "CommonValues")
	{
		ret = Get_DataSet_Value(payload, id_str->size());
	}

	return ret;
}


int RdbValueGet_B::Get_DataSet_Value(ns2__PayloadType* payload, int count)
{
	std::string strDataType;
	//ns3__CommonValuesType* commonvalue_ptr = payload->union_PayloadType.CommonValues->at(0);
	for each (auto i in *(payload->union_PayloadType.CommonValues))
	{
		const std::string strReqKeyId = i->Key;
		if (strReqKeyId.size() < 10)
		{
			continue;
		}

		int nStation = -1;
		int nDevice = -1;
		int nID = 1;

		if (!AnalyzeString(strReqKeyId, nStation, nDevice, nID))
		{
			return -1;
		}

		strDataType = strReqKeyId.substr(strReqKeyId.size()- 4,4);

		float value = 0.0;

		int nType = atoi(strDataType.c_str());
		switch (nType)
		{		
		case DI_TYPE:
			{
				// ��ȡң��ʵʱ����
				if (!GetDiFromRDB(nStation, nDevice, nID, value))
				{
					return -1;
				}
				break;
			}
		case AI_TYPE:
			{
				// ��ȡң��ʵʱ����
				if (!GetAiFromRDB(nStation, nDevice, nID, value))
				{
					return -1;
				}

				break;
			}
		case DD_TYPE:
			{
				// ��ȡ�����ʵʱ����
				if (!GetDDFromRDB(nStation, nDevice, nID, value))
				{
					return -1;
				}

				break;
			}
		case CALC_TYPE:
			{
				// ��ȡ������ʵʱ����
				if (!GetCalcFromRDB(nStation, nDevice, nID, value))
				{
					return -1;
				}

				break;
			}
		default:
			break;
		}

		char value_str[40] = {0};
		sprintf(value_str,"%8.3f", value);

		std::cout<<"value:"<<value_str<<std::endl;

		i->Value = new std::string;
		*(i->Value) = value_str;
		i->UpperLimit = new bool(false);
		i->LowerLimit = new bool(false);
		i->UpperUpperLimit = new bool(false);
		i->LowerLowerLimit = new bool(false);
		i->Invalid = new bool(false);

		//��ȡ���ɱ�ʶ
		// todo: Ʒ��λ��������������������
		/*rdbNo = rdbValue.RdbGetValOrderno(i);
		if ( rdbNo >=count*2 || rdbNo < 0)
			continue ;
		valNo=rdbValue.RdbGetParaValno(rdbNo);

		value = (double)rdbValue.RdbGetVal_long (valNo);
		if (value)
		{
			bool inconsistf = true;
			*(i->Invalid) = inconsistf;
		}*/

		ns3__CommonValuesType* commonvalue_ptr1 = payload->union_PayloadType.CommonValues->at(0);
		std::cout<<"commonvalue_ptr->Value:"<<*(commonvalue_ptr1->Value)<<std::endl;
	}


	/*Rdb_MultiTypeValue rdbValue ;	
	if ( m_polling_read->RdbGetFieldValue( "scada","scada",count*2,m_rdbFields,&rdbValue ) <= 0 )
		return  0;

	int	RecNumber = rdbValue.RdbGetValNums();
	if (RecNumber==0) return false;

	unsigned char valueType = 0;
	int strLen;
	int j = 0;
	for (int i=0;  i< RecNumber; i++)
	{

		int rdbNo = rdbValue.RdbGetValOrderno(i);		//���ݲ������У���ӦRDB_FIELD_STRU
		if ( rdbNo >=count*2 || rdbNo < 0)
			continue ;

		ns3__CommonValuesType* commonvalue_ptr = payload->union_PayloadType.CommonValues->at(rdbNo/2);
		if (!commonvalue_ptr)
		{
			continue;
		}
		int valNo=rdbValue.RdbGetParaValno(rdbNo);
		if (rdbValue.RdbGetValInfo (valNo, &valueType, &strLen) == NULL)
		{
			continue;
		}
		double value;
		switch (valueType)
		{
		case RDB_DATATYPE_UCHAR :
		case RDB_DATATYPE_CHAR :
		case RDB_DATATYPE_USHORT :
		case RDB_DATATYPE_SHORT :
		case RDB_DATATYPE_UINT :
		case RDB_DATATYPE_INT :
		case RDB_DATATYPE_ULONG :
		case RDB_DATATYPE_LONG :
			value = (double)rdbValue.RdbGetVal_long (valNo);
			break;
		case RDB_DATATYPE_FLOAT :
			value = (double)rdbValue.RdbGetVal_float (valNo);
			break;
		case RDB_DATATYPE_DOUBLE :
			value = (double)rdbValue.RdbGetVal_double (valNo);
			break;
		default:
			break;
		}

		char value_str[40];
		sprintf(value_str,"%8.3f", value);


		commonvalue_ptr->Value = new std::string;
		*(commonvalue_ptr->Value) = value_str;
		commonvalue_ptr->UpperLimit = new bool(false);
		commonvalue_ptr->LowerLimit = new bool(false);
		commonvalue_ptr->UpperUpperLimit = new bool(false);
		commonvalue_ptr->LowerLowerLimit = new bool(false);
		commonvalue_ptr->Invalid = new bool(false);

		//��ȡ���ɱ�ʶ
		i++;
		rdbNo = rdbValue.RdbGetValOrderno(i);
		if ( rdbNo >=count*2 || rdbNo < 0)
			continue ;
		valNo=rdbValue.RdbGetParaValno(rdbNo);

		value = (double)rdbValue.RdbGetVal_long (valNo);
		if (value)
		{
			bool inconsistf = true;
			*(commonvalue_ptr->Invalid) = inconsistf;
		}
	}
	return RecNumber/2*3;*/
	return -1;
}

//count ���ݸ���,��ȡ��¼ʱ��¼������˫����ΪҪ��ȡ���ɱ�־
/*int RdbValueGet_B::Get_DataSet_Value(ns2__PayloadType* payload, int count)
{
	if (!m_rdbFields)
	{
		return 0;
	}

	Rdb_MultiTypeValue rdbValue ;	
	if ( m_polling_read->RdbGetFieldValue( "scada","scada",count*2,m_rdbFields,&rdbValue ) <= 0 )
		return  0;

	int	RecNumber = rdbValue.RdbGetValNums();
	if (RecNumber==0) return false;

	unsigned char valueType = 0;
	int strLen;
	int j = 0;
	for (int i=0;  i< RecNumber; i++)
	{
		
		int rdbNo = rdbValue.RdbGetValOrderno(i);		//���ݲ������У���ӦRDB_FIELD_STRU
		if ( rdbNo >=count*2 || rdbNo < 0)
			continue ;
		
		ns3__CommonValuesType* commonvalue_ptr = payload->union_PayloadType.CommonValues->at(rdbNo/2);
		if (!commonvalue_ptr)
		{
			continue;
		}
		int valNo=rdbValue.RdbGetParaValno(rdbNo);
		if (rdbValue.RdbGetValInfo (valNo, &valueType, &strLen) == NULL)
		{
			continue;
		}
		double value;
		switch (valueType)
		{
		case RDB_DATATYPE_UCHAR :
		case RDB_DATATYPE_CHAR :
		case RDB_DATATYPE_USHORT :
		case RDB_DATATYPE_SHORT :
		case RDB_DATATYPE_UINT :
		case RDB_DATATYPE_INT :
		case RDB_DATATYPE_ULONG :
		case RDB_DATATYPE_LONG :
			value = (double)rdbValue.RdbGetVal_long (valNo);
			break;
		case RDB_DATATYPE_FLOAT :
			value = (double)rdbValue.RdbGetVal_float (valNo);
			break;
		case RDB_DATATYPE_DOUBLE :
			value = (double)rdbValue.RdbGetVal_double (valNo);
			break;
		default:
			break;
		}

		char value_str[40];
		sprintf(value_str,"%8.3f", value);


		commonvalue_ptr->Value = new std::string;
		*(commonvalue_ptr->Value) = value_str;
		commonvalue_ptr->UpperLimit = new bool(false);
		commonvalue_ptr->LowerLimit = new bool(false);
		commonvalue_ptr->UpperUpperLimit = new bool(false);
		commonvalue_ptr->LowerLowerLimit = new bool(false);
		commonvalue_ptr->Invalid = new bool(false);

		//��ȡ���ɱ�ʶ
		i++;
		rdbNo = rdbValue.RdbGetValOrderno(i);
		if ( rdbNo >=count*2 || rdbNo < 0)
			continue ;
		valNo=rdbValue.RdbGetParaValno(rdbNo);

		value = (double)rdbValue.RdbGetVal_long (valNo);
		if (value)
		{
			bool inconsistf = true;
			*(commonvalue_ptr->Invalid) = inconsistf;
		}
	}
	return RecNumber/2*3;
}*/

int RdbValueGet_B::Get_ArryDevices(char* arry_code, SCDDEV_VECTOR* scddev_vector)	//��ȡ�⸴����������豸 ���豸��������
{
	/*Db_connector::open();
	char sql[500];
	sprintf(sql, "select ����,���� from SCD�豸������ where վ����ϵ���� = '%s' order by ����", arry_code);
	PSCDDEV scddev_ptr;
	int rcd_cnt = Db_connector::select(sql,(void **)&scddev_ptr);
	std::vector<char *> devices;
	if (rcd_cnt > 0)
	{	
		PSCDDEV temp = NULL;
		PSCDDEV iterator_ = scddev_ptr;
		//�������ն˺ŵ��ն˲�����ֵ����Ӧ���ն˲����ṹ��Ԫ��
		for (int i = 0; i < rcd_cnt; i++)
		{
			temp = new SCDDEV;
			strcpy(temp->name,iterator_->name);
			temp->dev_type = iterator_->dev_type;
			scddev_vector->push_back(temp);
			iterator_++;
		}
	}
	Db_connector::close();
	return rcd_cnt;*/

	return -1;
}

int RdbValueGet_B::split(const string& str, vector<string>& ret_, string sep)
{
	if (str.empty())
	{
		return 0;
	}
	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

/******************************************************* ��ȡʵʱ���� *************************************************************/

bool RdbValueGet_B::AnalyzeString(const std::string& strKey, int& nStation, int& nDevice, int& nID)
{
	if (strKey.empty())
	{
		return false;
	}

	const char CH = '-';

	int pos = strKey.find(CH);
	if (pos == 0 || pos == -1)
	{
		return false;
	}

	std::string strStation = strKey.substr(0, pos);

	int posDevice = strKey.find(CH, pos + 1);
	if (posDevice == -1)
	{
		return false;
	}

	std::string strDevice = strKey.substr(pos + 1, posDevice - (pos + 1));

	int posId = strKey.find(CH, posDevice + 1);
	if (posId == -1)
	{
		return false;
	}

	std::string strId = strKey.substr(posDevice + 1, posId - (posDevice + 1));

	nStation = atoi(strStation.c_str());
	nDevice = atoi(strDevice.c_str());
	nID = atoi(strId.c_str());

	return true;
}

bool RdbValueGet_B::GetAiFromRDB( int nStation, int nDevice, int id, float& fValue )
{
	if (nStation < 0 || nDevice < 0 || id < 0)
	{
		return false;
	}

	// todo:����tagername���ڴ���л�ȡʵʱ���ݡ�
	fValue = 100;

	return true;
}

bool RdbValueGet_B::GetDiFromRDB( int nStation, int nDevice, int id, float& fValue)
{
	if (nStation < 0 || nDevice < 0 || id < 0)
	{
		return false;
	}

	// todo:����tagername���ڴ���л�ȡʵʱ���ݡ�
	fValue = 100;

	return true;
}

bool RdbValueGet_B::GetDDFromRDB( int nStation, int nDevice, int id, float& fValue )
{
	if (nStation < 0 || nDevice < 0 || id < 0)
	{
		return false;
	}

	// todo:����tagername���ڴ���л�ȡʵʱ���ݡ�
	fValue = 100;

	return true;
}

bool RdbValueGet_B::GetCalcFromRDB( int nStation, int nDevice, int id, float& fValue )
{
	if (nStation < 0 || nDevice < 0 || id < 0)
	{
		return false;
	}

	// todo:����tagername���ڴ���л�ȡʵʱ���ݡ�
	fValue = 100;

	return true;
}

