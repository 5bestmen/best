/*! @file dbg_svc.h
<PRE>
********************************************************************************
模块名       :
文件名       :  dbg_svc.h
文件实现功能 :  调试代理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   调试代理
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef _DBGSVR_MODULE_H
#define _DBGSVR_MODULE_H

#include "base_module.h"

class CServer;
class CDbgSvc : public CBaseModule
{
public:
	CDbgSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &MailID);
	virtual ~CDbgSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
};

#endif // _DBGSVR_MODULE_H


/** @}*/

