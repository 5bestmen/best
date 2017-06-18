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

#ifndef FES_CHANNEL_H
#define FES_CHANNEL_H
#pragma once

#include "datatypes.h"

#include <vector>
#include <memory>
#include <string>


class CIoDevice;

class CIoChannel
{
public:
	CIoChannel();
	CIoChannel(int32u nOccNo);

	virtual ~CIoChannel();
public:
	virtual bool Start() = 0;
	virtual bool Stop() = 0;
	virtual bool LoadData(int32u nOccNo) = 0;

	int32u GetOccNo()
	{
		return m_nOccNo;
	}
protected:
	void LogMsg(const char *szLogTxt, int nLevel);

protected:
	int32u m_nOccNo;          //! ͨ�������к�
	std::string m_szChannel;  //! ͨ������

	//! ͨ�������װ��
	std::vector<std::shared_ptr<CIoDevice>> m_arrDevices;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
