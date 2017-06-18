/*! @file mem_db.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  mem_db.h
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
	// ��ȡ�����ݿ�Ľڵ����к�
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

