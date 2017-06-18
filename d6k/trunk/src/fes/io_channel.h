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
	int32u m_nOccNo;          //! 通道的排行号
	std::string m_szChannel;  //! 通道名称
	CHANNEL * m_pData;
	int m_nMailBoxID;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
