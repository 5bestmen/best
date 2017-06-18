
#ifndef KERNEL_H
#define KERNEL_H


#include <map>
#include <vector>

#include "fesapi/fesdatadef.h"

//#include "utils/queue.h"
//#include "utils/array.h"
//#include "tinyxml/tinyxml.h"

#include "public.h"


class CShareMem;

class CKernel
{
public:
	CKernel();
	~CKernel();
	
	//////////////////////////////////////////////////////////////////////////
	//核心结构(kernel.cpp)
public:
	bool CreateDataPool();
	void DestroyDataPool();
	bool OpenDataPool(int32u nChannelOccNo);
	bool CloseDataPool(int32u nChannelOccNo);


private:
	// 共享内存
	CShareMem *m_pShmPimpl;

#if 0
	//////////////////////////////////////////////////////////////////////////
	//AIN
public:
	bool CreateAINTable(TiXmlElement* pParentElement);
	bool OpenAINTable();
	void DestroyAINTable();
public:
	AIN* GetAIN(int32u occNO);
	bool IoSetAINValue(int32u nChannelOccNo, int32u nOccNO, fp64 fValue, int8u nQuality);
	bool AppSetAINValue(int32u nAppOccNo, int32u nOccNO, fp64 fValue, int8u nQuality);
	fp64 GetAINValue(int32u occNO);
	int32u GetAinCount(int32u nChannelOccNo, int32u nDeviceOccNo);
	void GetAinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo, std::vector<int32u>& vecAinOccNos);
	int32u GetAinAddress(int32u nAinOccNo);
public:
	fp64 Transform(AIN* pAIN,fp64 fValue);
	void AINUpdate(AIN* pAIN,fp64 fValue,fp64 fTransValue,int8u nQuality);
	bool AINAlarm(AIN* pAIN);
	bool AINEvent(int32u nChannelOccNo, AIN* pAIN, bool IsManSet = false);
private:
	CArray<AIN> m_poolAIN;

	//DIN
public:
	bool CreateDINTable(TiXmlElement* pParentElement);
	bool OpenDINTable();
	void DestroyDINTable();
public:
	DIN* GetDIN(int32u occNO);
	bool SetDINValue(int32u nChannelOccNo, int32u occNO, int8u nValue, int8u nQuality);
	UINT8 GetDINValue(int32u occNO);
	int32u GetDinCount(int32u nChannelOccNo, int32u nDeviceOccNo);
	void GetDinOccNos(int32u nChannelOccNo, int32u nDeviceOccNo, std::vector<int32u>& vecDinOccNos);
	int32u GetDinAddress(int32u nDinOccNo);
private:
	CArray<DIN> m_poolDIN;
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//AOUT
public:
	bool CreateAOUTTable(TiXmlElement* pParentElement);
	bool OpenAOUTTable();
	void DestroyAOUTTable();
public:
	AOUT* GetAOUT(int32u occNO);
private:
	CArray<AOUT> m_poolAOUT;

	//////////////////////////////////////////////////////////////////////////
	//DOUT
public:
	bool CreateDOUTTable(TiXmlElement* pParentElement);
	bool OpenDOUTTable();
	void DestroyDOUTTable();
public:
	DOUT* GetDOUT(int32u occNO);
private:
	CArray<DOUT> m_poolDOUT;
	
	//////////////////////////////////////////////////////////////////////////
	//channel表(kernel_channel.cpp)
public:
	bool CreateChannelTable(TiXmlElement* pParentElement);
	bool OpenChannelTable();
	void DestroyChannelTable();
	CHANNEL* GetChannel(int32u occNo);
private:
	bool SetChannelCoreState(int32u nChannelOccNo,int8u nState);
	bool SetChannelDriverState(int32u nChannelOccNo,int8u nState);
	int8u GetChannelCoreState(int32u nChannelOccNo);
	int8u GetChannelDriverState(int32u nChannelOccNo);
private:
	CArray<CHANNEL> m_poolChannel;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//device表(kernel_device.cpp)
public:
	bool CreateDeviceTable(TiXmlElement* pParentElement);
	bool OpenDeviceTable();
	void DestroyDeviceTable();
	DEVICE* GetDevice(int32u occNo);
	int32u GetDeviceCount(int32u nChannelOccNo);
	int32u GetDeviceAddress(int32u nDeviceOccNO);
	void   GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u>& vecDeviceOccNos);
private:
	CArray<DEVICE> m_poolDevice;
	
	//////////////////////////////////////////////////////////////////////////
	//驱动管理
public:
	bool IsDriverQuit(int32u nChannelOccNo);
	bool LoadDrivers();
	void UnLoadDrivers();
private:
	bool IsDriversQuit();
	bool LoadExeDriver(int32u nChannelOccNo,const char* strDriverName);
	bool LoadDllDriver(int32u nChannelOccNo,const char* strDriverName);

	//////////////////////////////////////////////////////////////////////////
	//辅助函数(kernel_utils.cpp)
public:
	int GetElementChildCount(TiXmlElement* pElement, std::string strTag);

	//////////////////////////////////////////////////////////////////////////
	//线性转换
public:
	bool CreateTransformLinearTable(TiXmlElement* pParentElement);
	bool OpenTransformLinearTable();
	void DestroyTransformLinearTable();
private:
	fp64 LinearTransform(int32u nTransformOccNo, fp64 fValue);
	TRANSFORM_LINEAR* GetTransformLinear(int32u occNO);
private:
	CArray<TRANSFORM_LINEAR> m_poolTransformLinear;

	//////////////////////////////////////////////////////////////////////////
	//非线性转换
public:
	bool CreateTransformNonLinearTable(TiXmlElement* pParentElement);
	bool OpenTransformNonLinearTable();
	void DestroyTransformNonLinearTable();
private:
	fp64 NonLinearTransform(int32u nTransformOccNo, fp64 fValue);
	TRANSFORM_NONLINEAR* GetTransformNonLinear(int32u occNO);

private:
	CArray<TRANSFORM_NONLINEAR> m_poolTransformNonLinear;


	//////////////////////////////////////////////////////////////////////////
	//AIN Alarm
public:
	bool CreateAinAlarmTable(TiXmlElement* pParentElement);
	bool OpenAinAlarmTable();
	void DestroyAinAlarmTable();
	AIN_ALARM* GetAinAlarm(int32u occNO);
	
private:
	CArray<AIN_ALARM> m_poolAinAlarm;


	//////////////////////////////////////////////////////////////////////////
	//DIN Alarm
public:
	bool CreateDinAlarmTable(TiXmlElement* pParentElement);
	bool OpenDinAlarmTable();
	void DestroyDinAlarmTable();
	DIN_ALARM* GetDinAlarm(int32u occNO);
private:
	CArray<DIN_ALARM> m_poolDinAlarm;
#endif
};

extern CKernel g_kernel;


#endif // KERNEL_H
