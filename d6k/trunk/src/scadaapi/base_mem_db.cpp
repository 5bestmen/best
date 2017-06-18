/*! @file mem_db.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  mem_db.cpp
�ļ�ʵ�ֹ��� :  �ڴ����ݿ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �ڴ����ݿ����
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
