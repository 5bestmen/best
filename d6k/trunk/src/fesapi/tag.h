/*! @file tag.h
<PRE>
********************************************************************************
模块名       :  测点定义
文件名       :  tag.h
文件实现功能 :  
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author LiJin
*  @version 1.0
*  @date    2016.09.11
*/

#ifndef FES_TAG_H
#define FES_TAG_H
#pragma once

#include "datatypes.h"
#include <memory>
#include <functional>
#include <vector>
 
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
// struct AIN_ALARM;
// struct DIN_ALARM;
// struct AIN_ALARM_LIMIT;
// struct DIN_ALARM_LIMIT;
class CAinAlarm;
class CDinAlarm;
struct TRANSFORM_LINEAR;
struct TRANSFORM_NONLINEAR;
struct CHANNEL;
struct TIMEPAK;
struct DEVICE;

template<typename T>
class CIoTag
{
public:
	CIoTag()
	{
 	}
 
	virtual ~CIoTag()
	{

	}
public:
protected:
	T * m_pTag;       //! 内存数据库指针 
};

class CFesTag
{
public:
	CFesTag()
	{

	}
	virtual~CFesTag()
	{

	}
public:
	void SetTimePack(TIMEPAK &dstTm, TIMEPAK *pTm);

public:
	std::function< bool(int32u nChannelOccNo, CHANNEL** ppChannel) > m_fnGetChannel;
	std::function< bool(int32u nDeviceOccNo, DEVICE **ppDevice) > m_fnGetDevice;
protected:
	DEVICE * m_pDevcie;  //! 所属的装置
	CHANNEL *m_pChannel; //! 所属的通道
	int m_nChannelMailBoxID;
};

class CFesAin : public CFesTag
{
public:
	explicit CFesAin(AIN *pAin);
	virtual~CFesAin();
public:
	bool Update(fp64 fpValue, int8u nQua);
	bool Update(fp64 fpValue, int8u nQua, TIMEPAK * pTm);

	void SetQuality(int8u nQua);
	void GetValue(fp64 &fpValue, int8u &nQua)const;

	void ClearAlarmState();

	void LoadConfig();
	AIN *GetAinPtr()
	{
		return m_pFB;
	}
protected:
	CFesAin();

	bool CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput);
	bool CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput);
	bool CalcOutput(AIN *pFB, fp64 fpRaw, fp64 & fpOutput);
public:
	//! 获取告警的occno
	std::function< std::shared_ptr<CAinAlarm>(int32u nAlarmOccNo) > m_fnGetAlarm;

	std::function< TRANSFORM_LINEAR*(int32u nOccNo) > m_fnGetTransformLinear;
	std::function< TRANSFORM_NONLINEAR*(int32u nOccNo) > m_fnGetTransformNonLinear;

//	std::function< bool(int32u nChannelOccNo, CHANNEL& ch) > m_fnGetChannel;

private:
	//! 共享内存中的地址
	AIN * m_pFB;
	TRANSFORM_LINEAR *m_pLinear;
	TRANSFORM_NONLINEAR *m_pNonLinear;
	std::shared_ptr<CAinAlarm> m_pAlarm;

};

class CFesDin : public CFesTag
{
public:
	CFesDin();
	explicit CFesDin(DIN *pDin);

	virtual~CFesDin();
public:
	bool Update(int8u nValue, int8u nQua);
	bool Update(int8u nValue, int8u nQua, bool bCpuEnable);
	bool Update(int8u nValue, int8u nQua, bool bCpuEnable, TIMEPAK * pTm);

	bool Alarm(int8u nValue, int8u nQua, TIMEPAK * pTm);
	void SetQuality(int8u nQua);

	void GetValue(int8u &nValue, int8u &nQua)const;

	void LoadConfig();
protected:
	bool NormalUpdate(int8u nValue, int8u nQua, bool bCpuEnable, TIMEPAK * pTm);

public:
	//! 获取告警的occno
	std::function< std::shared_ptr<CDinAlarm>(int32u nAlarmOccNo) > m_fnGetAlarm;
private:
	DIN * m_pFB;
	std::shared_ptr<CDinAlarm> m_pAlarm;
};

class CFesAout : public CFesTag
{
public:
	CFesAout();
	explicit CFesAout(AOUT *pAout);
	virtual~CFesAout();
public:
	bool Update(fp64 fpValue, int8u nQua);
	void SetQuality(int8u nQua);

	std::function< TRANSFORM_LINEAR*(int32u nOccNo) > m_fnGetTransformLinear;
	std::function< TRANSFORM_NONLINEAR*(int32u nOccNo) > m_fnGetTransformNonLinear; 
protected:
	bool Output(fp64 fpValue, fp64 *pRetValue);

	bool CalcLinearOutput(TRANSFORM_LINEAR *pLinear, fp64 fpRaw, fp64 & fpOutput);
	bool CalcNonLinearOutput(TRANSFORM_NONLINEAR *pNonLinear, fp64 fpRaw, fp64 & fpOutput);
	bool CalcOutput(AOUT *pFB, fp64 fpRaw, fp64 & fpOutput);
private:
	AOUT * m_pFB;
	TRANSFORM_LINEAR *m_pLinear;
	TRANSFORM_NONLINEAR *m_pNonLinear;
	std::shared_ptr<CAinAlarm> m_pAlarm; 
};

class CFesDout : public CFesTag
{
public:
	CFesDout();
	explicit CFesDout(DOUT *pDout);
	virtual ~CFesDout();
public:
	bool Update(int8u nValue, int8u nQua);
	void SetQuality(int8u nQua);
private:
	DOUT * m_pFB;
};


/** @}*/

#endif // FES_TAG_H
