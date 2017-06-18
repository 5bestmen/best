/*! @file scada_db.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  scada_db.h
�ļ�ʵ�ֹ��� :  SCADA�ڵ���ڴ����ݿ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �������ڵ���ڴ����ݿ�
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef SCD_DB_MODULE_H
#define SCD_DB_MODULE_H

#include "mem_db.h"

#include "scadaapi/scdapp_def.h"

class CScadaApp;
 
class CScadaDB : public CMemDB
{
public:
	CScadaDB( );
	virtual ~CScadaDB(void);
public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
public:
	void CreateMailBoxs();
	void DestroyMailBoxs();

protected:

	// ����SCADA��APP�Ĺ���
	SAPP  * m_pScadaApps;
	unsigned int m_nAppCount;
//	std::vector<SAPP *>     m_arrApps;  //! �ڹ����ڴ��еĵ�ַ

	std::vector<std::shared_ptr<CScadaApp>> m_arrApps;
};

#endif // SCD_DB_MODULE_H


/** @}*/

