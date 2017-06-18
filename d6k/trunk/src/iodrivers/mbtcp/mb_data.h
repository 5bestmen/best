 

#ifndef MB_DATA_H
#define MB_DATA_H

#include <memory>
#include <vector>

#include <datatypes.h>

#include <fesapi\fesapi.h>


class CMBEReg
{
public:
	CMBEReg();
	~CMBEReg();
public:
	enum REG_TYPE
	{
		COIL = 1,
		HOLDING_REG = 2,
	};

private:

	int32u m_nRegAddr;    //! �Ĵ����ĵ�ַ
	int32u m_nOccNo;      //! �ڴ���е����к�
	int32u m_nPointType;  //! �ڴ���еĲ������
};

class CMBDevice
{
public:
	CMBDevice();
	CMBDevice(int32u nOccNO);

	~CMBDevice();
public:
	//! ��ȡ��������
	bool InitDeviceData(int32u nOccNO);

private:
	int32u m_nDeviceOccNo;

};

  

class CMBChannel
{
public:
	CMBChannel(void);
	~CMBChannel(void);
public:
	//! ��ȡ��������
	bool InitChannelData(int32u nOccNO);

public: 

	void StartScan();
	void StopScan();
private:
	//! �ٻ�����
	void PollingData();

private:
	
	int32u m_nChannelOccNo;  //! �ڴ���е�ͨ����

	std::vector<  CMBDevice* > m_arrDevices;

	 
 
};


#endif //MB_DATA_H

