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
*  @date    2016.09.11
*/

#ifndef FES_IO_CHANNEL_H
#define FES_IO_CHANNEL_H
#pragma once

#include "datatypes.h"

#include <memory>
#include <string>

struct CHANNEL;

typedef int(*StartIoDriver)(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);
typedef int(*StopIoDriver)(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);
 
class CIoChannel
{
public:
	CIoChannel();
	explicit CIoChannel(CHANNEL *pChannel);
	~CIoChannel();
public:
	bool Load();
	bool Start(const char * pszName);
	bool Stop(const char * pszName);
// 	virtual bool LoadData(int32u nOccNo) = 0;

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}
//	std::string GetTagName()const;
	void SetMailBoxID(int nID);

	int GetMailBoxID() const
	{
		return m_nMailBoxID;
	}
protected:
	bool LogString(const char *szLogTxt, int nLevel);

	StartIoDriver  m_fnStartIoDriver;
	StopIoDriver   m_fnStopIoDriver;

protected:
	int32u m_nOccNo;          //! ͨ�������к�
	std::string m_szChannel;  //! ͨ������
	CHANNEL * m_pData;
	int m_nMailBoxID;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
