/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :
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
*  @date    2007.09.11
*/

#ifndef _MODBUS_DEVICE_H
#define _MODBUS_DEVICE_H
#pragma once

#include "device.h"
#include "datatypes.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <set>
 
class CModbusDevice : public CIoDevice
{
public:
	CModbusDevice();
	explicit CModbusDevice(int32u nOccNo);

	virtual ~CModbusDevice();
public:
	virtual bool Start();
	virtual bool Stop() ;

	virtual bool LoadData(int32u nOccNo);
	int32u GetChannelOccNo() ;
	virtual void PollingData();
private:
	void SimuData();

	void SimuCoils();
protected:
	//! 线圈地址与OccNo对应表
	std::unordered_multimap<int32u, int32u> m_mapCoils;

	std::unordered_multimap<int32u, int32u> m_mapHoldingRegs;

	//! 所有线圈的地址
	//std::vector<int32u> m_arrCoilsAddr;
	std::set<int32u> m_setCoilsAddr;

private:


};


/** @}*/

#endif // _MODBUS_CHANNEL_H
