/*! @file sysvar_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sysvar_db.h
文件实现功能 :  系统变量内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   系统变量内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.16
*******************************************************************************/
#ifndef SYSVAR_MEMDB_MODULE_H
#define SYSVAR_MEMDB_MODULE_H

#include "datatypes.h"
#include "../scdpub/variant_data.h"

#include "fesapi/fesdatadef.h"
#include "base_mem_db.h"

#include <vector>
#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  
#include <atomic>

 
class CSysVarMemDB  : public CBaseMemDB
{
public:
	CSysVarMemDB();
	virtual ~CSysVarMemDB();
public:
	// 获取测值
	bool GetValue(int32u nOccNo, CVariant & val, int8u &nQuality)const; 
	//网络总线服务调用，更新测值
	bool UpdateValue(int32u nOccNo, const CVariant & val, int8u nQuality); 

	int32u GetCount()const
	{
		return m_nVarCount;
	}

protected:
	void LogMsg(const char *szLogTxt, int nLevel); 
protected:


private:
	int32u m_nNodeOccNo; //! 本映射区的节点排行
 
	//! 系统变量数量
	int32u m_nVarCount;
	//! 共享内存区
	struct VARDATA *m_pSysVars;
	 

};

#endif // SYSVAR_MEMDB_MODULE_H


/** @}*/

