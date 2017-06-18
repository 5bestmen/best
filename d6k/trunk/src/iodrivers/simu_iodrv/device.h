/*! @file
<PRE>
********************************************************************************
模块名       :  装置定义
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

#ifndef  FES_DEVICE_H
#define  FES_DEVICE_H
#pragma once
 

#include "datatypes.h"

#include <memory>
#include <string>
#include <vector>
 
class CIoAin;
class CIoDin;
class CIoAout;
class CIoDout;
 
class CIoDevice
{
public:
	CIoDevice();
	virtual ~CIoDevice();

public:
	int32u GetOccNo()
	{
		return m_nOccNo;
	}
	int32u GetChannelOccNo()
	{
		return m_nChannelOccNo;
	}
	virtual bool LoadData(int32u nOccNo) = 0;
	virtual void PollingData() = 0;
protected:
	int32u m_nOccNo;                 //! 排行号
	int32u m_nChannelOccNo;          //! 排行号

	std::string m_szDevice;          //! 名称
 
	//! 通道下面的装置
	std::vector<std::shared_ptr<CIoAin>>  m_arrAins;
	std::vector<std::shared_ptr<CIoDin>>  m_arrDins;
	std::vector<std::shared_ptr<CIoAout>> m_arrAouts;
	std::vector<std::shared_ptr<CIoDout>> m_arrDouts;

};


/** @}*/

#endif // FES_DEVICE_H
