/*! @file mem_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.cpp
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
#include "base_mem_db.h"

#include "fesapi/fesdatadef.h"

#include "log/log.h"
#include <QObject> 
#include <QString> 
#include <QDebug>


CBaseMemDB::CBaseMemDB()  
{
	m_nMyNodeOccNo = INVALID_OCCNO;
}

CBaseMemDB::~CBaseMemDB(void)
{ 

}

void CBaseMemDB::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("SCADA", szLogTxt, nLevel, nullptr);
}

 


/** @}*/
