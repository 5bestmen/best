/*! @file
<PRE>
********************************************************************************
ģ����       :
�ļ���       :
�ļ�ʵ�ֹ��� :  
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  
*  @author xingzhibing
*  @version 1.0
*  @date    2016.10.24
*/

#ifndef _CSIMU_DEVICE_H
#define _CSIMU_DEVICE_H
#pragma once

#include "device.h"
#include "datatypes.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <set>
 
class CSimuDevice : public CIoDevice
{
public:
	CSimuDevice();
	explicit CSimuDevice(int32u nOccNo);

	virtual ~CSimuDevice();
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
	//! ������Ȧ�ĵ�ַ
	//std::vector<int32u> m_arrCoilsAddr;
	std::set<int32u> m_setCoilsAddr;

private:


};


/** @}*/

#endif // _MODBUS_CHANNEL_H
