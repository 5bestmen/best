
#include "basemodule.h"

#include "fesapi/fesapi.h"
#include "mail/mail.h"
#include "utils/misc.h"
#include "ace/Thread_Manager.h"

CBaseModule::CBaseModule(CServer* pServer)
{
	m_pServer = pServer;
}


CBaseModule::~CBaseModule(void)
{
}

//Ïß³Ìº¯Êý
void CBaseModule::ModuleThreadProc(void * lpParameter)
{
	CBaseModule* pthis = (CBaseModule*)lpParameter;

	pthis->m_bIsThreadStopped = false;

	while (pthis->m_bIsThreadRunning)
	{
		pthis->Run();

		Utils::USleep(1);
	}

	pthis->m_bIsThreadStopped = true;
}

void CBaseModule::StartModule()
{
	m_bIsThreadRunning = true;

	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)ModuleThreadProc, this);

}
void CBaseModule::StopModule()
{
	m_bIsThreadRunning = false;

	while (!m_bIsThreadStopped)
	{
		Utils::USleep(1);
	}
}

