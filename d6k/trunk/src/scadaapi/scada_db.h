/*! @file scada_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  scada_db.h
文件实现功能 :  SCADA节点的内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   服务器节点的内存数据库
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

	// 基于SCADA的APP的管理
	SAPP  * m_pScadaApps;
	unsigned int m_nAppCount;
//	std::vector<SAPP *>     m_arrApps;  //! 在共享内存中的地址

	std::vector<std::shared_ptr<CScadaApp>> m_arrApps;
};

#endif // SCD_DB_MODULE_H


/** @}*/

