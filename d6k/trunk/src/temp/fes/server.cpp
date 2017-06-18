
#include "Server.h"

#include "fesapi\fesapi.h"
#include "Utils/misc.h"

#include "rtdbmodule.h"
#include "dbgsvrmodule.h"
#include "netbusmodule.h"

#include "coremodule.h"

CServer::CServer(void)
{
	m_pRtDbModule	= new CRtdbModule(this);
	m_pDbgSvrModule = new CDbgSvrModule(this);
	m_pNetBusModule = new CNetBusModule(this);

	m_pCoreModule = new CCoreModule(this);
}


CServer::~CServer(void)
{
	delete m_pRtDbModule;
	delete m_pDbgSvrModule;
	delete m_pNetBusModule;

	delete m_pCoreModule;
}


bool CServer::Initialize()
{
	if (!m_pCoreModule->Initialize())
		return false;

	if (!m_pRtDbModule->Initialize())
		return false;

	if(!m_pDbgSvrModule->Initialize())
		return false;

	if (!m_pNetBusModule->Initialize())
		return false;

	return true;
}

void CServer::Run()
{
	while (m_bQuit)
	{
		Utils::USleep(1);
	}
}

void CServer::Shutdown()
{
	m_pNetBusModule->Shutdown();
	m_pDbgSvrModule->Shutdown();
	m_pRtDbModule->Shutdown();

	m_pCoreModule->Shutdown();
}
