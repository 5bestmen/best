 

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

	int32u m_nRegAddr;    //! 寄存器的地址
	int32u m_nOccNo;      //! 内存库中的排行号
	int32u m_nPointType;  //! 内存库中的测点类型
};

class CMBDevice
{
public:
	CMBDevice();
	CMBDevice(int32u nOccNO);

	~CMBDevice();
public:
	//! 获取配置数据
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
	//! 获取配置数据
	bool InitChannelData(int32u nOccNO);

public: 

	void StartScan();
	void StopScan();
private:
	//! 召唤数据
	void PollingData();

private:
	
	int32u m_nChannelOccNo;  //! 内存库中的通道号

	std::vector<  CMBDevice* > m_arrDevices;

	 
 
};


#endif //MB_DATA_H

