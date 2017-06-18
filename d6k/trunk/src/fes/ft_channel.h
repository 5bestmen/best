/*! @file ft_channel.h
<PRE>
********************************************************************************
模块名       :  前置的远动转发模块
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
*  @date    2017.05.16
*/

#ifndef FT_CHANNEL_H
#define FT_CHANNEL_H
#pragma once

#include "datatypes.h"

#include <memory>
#include <string>

struct FT_CHANNEL;

typedef int(*StartFtDriver)(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);
typedef int(*StopFtDriver)(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);
 
class CFTChannel
{
public:
	CFTChannel();
	explicit CFTChannel(FT_CHANNEL *pChannel);
	~CFTChannel();
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

	StartFtDriver  m_fnStartFtDriver;
	StopFtDriver   m_fnStopFtDriver;

protected:
	int32u m_nOccNo;          //! 通道的排行号
	std::string m_szChannel;  //! 通道名称
	FT_CHANNEL * m_pData;
	int m_nMailBoxID;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
