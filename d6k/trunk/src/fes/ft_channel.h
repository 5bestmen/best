/*! @file ft_channel.h
<PRE>
********************************************************************************
ģ����       :  ǰ�õ�Զ��ת��ģ��
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
	int32u m_nOccNo;          //! ͨ�������к�
	std::string m_szChannel;  //! ͨ������
	FT_CHANNEL * m_pData;
	int m_nMailBoxID;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
