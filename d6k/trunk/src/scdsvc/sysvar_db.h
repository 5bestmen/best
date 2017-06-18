/*! @file sysvar_db.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sysvar_db.h
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
	// ��ȡ��ֵ
	bool GetValue(int32u nOccNo, CVariant & val, int8u &nQuality)const; 
	//�������߷�����ã����²�ֵ
	bool UpdateValue(int32u nOccNo, const CVariant & val, int8u nQuality); 

	int32u GetCount()const
	{
		return m_nVarCount;
	}

protected:
	void LogMsg(const char *szLogTxt, int nLevel); 
protected:


private:
	int32u m_nNodeOccNo; //! ��ӳ�����Ľڵ�����
 
	//! ϵͳ��������
	int32u m_nVarCount;
	//! �����ڴ���
	struct VARDATA *m_pSysVars;
	 

};

#endif // SYSVAR_MEMDB_MODULE_H


/** @}*/

