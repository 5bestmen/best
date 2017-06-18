/*! @file
<PRE>
********************************************************************************
模块名       :
文件名       :
文件实现功能 :  
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

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
	int32u m_nOccNo;          //! 通道的排行号
	std::string m_szChannel;  //! 通道名称

	//! 通道下面的装置
	std::vector<std::shared_ptr<CIoDevice>> m_arrDevices;
};
 
 


/** @}*/

#endif // _MODBUS_CHANNEL_H
