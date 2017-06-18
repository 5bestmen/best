
#ifndef _DBGSVR_MODULE_H
#define _DBGSVR_MODULE_H

#include "basemodule.h"

class CServer;
class CDbgSvrModule : public CBaseModule
{
public:
	CDbgSvrModule(CServer* pServer);
	virtual ~CDbgSvrModule(void);

public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
};

#endif // _DBGSVR_MODULE_H


