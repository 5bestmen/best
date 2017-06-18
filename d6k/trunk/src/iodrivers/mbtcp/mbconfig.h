 

#ifndef MODBUS_CONFIG_H
#define MODBUS_CONFIG_H

#include <map>
#include <vector>
#include <algorithm>
#include <datatypes.h>

enum 
{
	DATATYPE_AIN,
	DATATYPE_DIN,
};

class CModbusParam
{
public:
	CModbusParam()
	{
		m_nPerFrameAIN = 120;
		m_nPerFrameDIN = 1920;

		m_nFunCodeAIN = 0x03;
		m_nFunCodeDIN = 0x01;
		
		m_nFunCodeTelectrlAIN = 0x06;
		m_nFunCodeTelectrlDIN = 0x05;
	}
public:
	int8u m_nFunCodeAIN;
	int8u m_nFunCodeDIN;
	int8u m_nFunCodeTelectrlAIN;
	int8u m_nFunCodeTelectrlDIN;
public:
	int32u m_nPerFrameAIN;
	int32u m_nPerFrameDIN;

};

class CFrameData
{
public:
	int32u m_nDataType;//֡��������
	int16u m_nStartAddr;//֡��ʼ��ַ
	int16u m_nCount;//֡������Ŀ
public:
	int32u m_nDeviceAddress;
	int32u m_nDeviceOccNo;
};

class CDeviceData
{
public:
	CDeviceData(int32u nChannelOccNo,int32u nDeviceOccNo)
	{
		m_nChannelOccNo = nChannelOccNo;
		m_nDeviceOccNo = nDeviceOccNo;
		m_nLastHaltTime = 0;
	}

public:
	bool LoadAIN();
	bool LoadDIN();
	bool LoadDeviceAddress(int32u nDeviceOccNo);

	//װ�������е��OccNo
private:
	std::vector<int32u> m_vecDeviceAINOccNo;
	std::vector<int32u> m_vecDeviceDINOccNo;

	//װ�������е��Address
private:
	std::vector<int32u> m_vecDeviceAINAddress;
	std::vector<int32u> m_vecDeviceDINAddress;

	//��Address����OccNo��ӳ��
public:
	int32u GetAinOccNoFromAddress(int32u nAddress);
	int32u GetDinOccNoFromAddress(int32u nAddress);
private:
	std::map<int32u, int32u> m_mapAINAddressToOccNo;
	std::map<int32u, int32u> m_mapDINAddressToOccNo;

	//��֡
public:
	bool Prepare(CModbusParam* pModbusParam);
public:
	std::vector<CFrameData>& GetFrameData() { return m_vecDeviceFrames; }
private:
	bool PrepareFrame(int32u nDataType, CModbusParam* pModbusParam, std::vector<int32u>& arrAddress);
	std::vector<CFrameData> m_vecDeviceFrames;

	//װ�õ�ͨ��OccNo,��װ��OccNo
private:
	int32u m_nChannelOccNo;
	int32u m_nDeviceOccNo;

	//װ�õ�ַ
private:
	int32u m_nDeviceAddress;

	//װ�ù���
public:
	bool IsDeviceHalt();
	void SetDeviceHalt();
private:
	enum 
	{
		MAX_DEVICE_HALT_TIME = 5,//��
	};
	int32u m_nLastHaltTime;//װ�ù���ʱ��
};


class CChannelData
{
public:
	CChannelData();
	~CChannelData();

public:
	bool LoadDevices(CModbusParam* pModbusParam,int nChannelOccNo);
	void DeleteDevices();

public:
	
public:
	int32u GetAinOccNo(int32u nDeviceOccNo, int32u nAddress);
	int32u GetDinOccNo(int32u nDeviceOccNo, int32u nAddress);

public:
	std::map<int32u, CDeviceData*>* GetMapDevices() { return &m_mapDevices; }
private:
	std::map<int32u, CDeviceData*> m_mapDevices;
};
#endif //MODBUS_CONFIG_H

