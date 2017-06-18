/*! @file mem_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.h
文件实现功能 :  内存数据库基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.2
*******************************************************************************/
#ifndef BASE_MEMDB_MODULE_H
#define BASE_MEMDB_MODULE_H

#include "datatypes.h"
#include "../scdpub/variant_data.h"
 
 
class CBaseMemDB  
{
public:
	CBaseMemDB();
	virtual ~CBaseMemDB();
public:	  
	// 获取该数据库的节点排行号
	int32u GetNodeOccNo()const
	{
		return m_nMyNodeOccNo;
	}

protected:
	void LogMsg(const char *szLogTxt, int nLevel); 

protected:
	int32u m_nMyNodeOccNo;

};

#endif // BASE_MEMDB_MODULE_H


/** @}*/

