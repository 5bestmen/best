/*! @file sysvar_db.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sysvar_db.cpp
�ļ�ʵ�ֹ��� :  ϵͳ�����ڴ����ݿ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ�����ڴ����ݿ�
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

// ��ȡ��ֵ
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
