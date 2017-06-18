
#ifndef _CORE_MODULE_H
#define _CORE_MODULE_H

#include "basemodule.h"

class CServer;
class CCoreModule : public CBaseModule
{
public:
	CCoreModule(CServer* pServer);
	virtual ~CCoreModule(void);

public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
};

#endif // _CORE_MODULE_H


