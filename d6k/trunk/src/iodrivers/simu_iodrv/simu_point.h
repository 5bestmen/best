/*! @file
<PRE>
********************************************************************************
模块名       : 测点定义
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

#ifndef CSIMU_POINT_H
#define CSIMU_POINT_H
#pragma once

#include "datatypes.h"
#include "fesapi/fesdatadef.h"
#include "tag.h"
 
#include <memory>


#if 0
class CCoilOutput : public CIoDout
{
public:
	CCoilOutput()
	{

	}
	virtual ~CCoilOutput()
	{

	}
private:

};

class CDigitalInput : public CIoDin
{
public:
	CDigitalInput()
	{

	}
	virtual ~CDigitalInput()
	{

	}
private:

};


class CHoldingRegister : public  CIoAin
{
public:
	CHoldingRegister()
	{

	}
	virtual ~CHoldingRegister()
	{

	}
private:

};

class CAnalogueInput : public CIoAin
{
public:
	CAnalogueInput()
	{

	}
	virtual ~CAnalogueInput()
	{

	}
private:

};

#endif

/** @}*/

#endif // MODBUS_POINT_H
