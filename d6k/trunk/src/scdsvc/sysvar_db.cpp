/*! @file sysvar_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  sysvar_db.cpp
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
#include "sysvar_db.h"
#include "fesapi/fesdatadef.h"

#include "log/log.h"
#include <QObject> 
#include <QString> 
#include <QDebug>


CSysVarMemDB::CSysVarMemDB()  
{ 
	m_nVarCount = 0;
	m_pSysVars = nullptr;
}

CSysVarMemDB::~CSysVarMemDB(void)
{ 

}

void CSysVarMemDB::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("SCADA", szLogTxt, nLevel, nullptr);
}

// 获取测值
bool CSysVarMemDB::GetValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(nOccNo > INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	Q_ASSERT(nOccNo <= m_nVarCount);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nVarCount)
		return false;



	return true;
}

bool  CSysVarMemDB::UpdateValue(int32u nOccNo, const CVariant & val, int8u nQuality)
{
	Q_ASSERT(nOccNo > INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	Q_ASSERT(nOccNo <= m_nVarCount);

	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nVarCount)
		return false;


	return true;
}


/** @}*/
