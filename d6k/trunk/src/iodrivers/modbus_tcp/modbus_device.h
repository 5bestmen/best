/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :
�ļ�ʵ�ֹ��� :  
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
	//! ��Ȧ��ַ��OccNo��Ӧ��
	std::unordered_multimap<int32u, int32u> m_mapCoils;

	std::unordered_multimap<int32u, int32u> m_mapHoldingRegs;

	//! ������Ȧ�ĵ�ַ
	//std::vector<int32u> m_arrCoilsAddr;
	std::set<int32u> m_setCoilsAddr;

private:


};


/** @}*/

#endif // _MODBUS_CHANNEL_H
