
#ifndef _RTDB_MODULE_H
#define _RTDB_MODULE_H

#include "basemodule.h"

class CServer;
class CRtdbModule : public CBaseModule
{
public:
	CRtdbModule(CServer* pServer);
	virtual ~CRtdbModule(void);

public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
};

#endif // _RTDB_MODULE_H


