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
*  @date    2010.03.16
*******************************************************************************/

#ifndef DATA_SIZE_H
#define DATA_SIZE_H

#include "variant.h"

#include <array>

class CIoDataType
{
public:
	CIoDataType()
	{

	}
	~CIoDataType()
	{

	}
public:
	static int32u GetDataSize(INT32U dt);

private:
	static std::array<int32u, DT_MAX> m_arrDataSize;

};

  

#endif // DATA_SIZE_H

/** @}*/  

