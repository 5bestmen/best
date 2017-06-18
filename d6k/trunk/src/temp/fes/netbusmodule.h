
#ifndef _NETBUS_MODULE_H
#define _NETBUS_MODULE_H

#include "basemodule.h"

class CServer;
class CNetBusModule : public CBaseModule
{
public:
	CNetBusModule(CServer* pServer);
	virtual ~CNetBusModule(void);

public:
	virtual bool Initialize();
	virtual void Run();
	virtual void Shutdown();
};

#endif // _NETBUS_MODULE_H


