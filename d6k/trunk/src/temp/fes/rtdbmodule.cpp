
#include "rtdbmodule.h"

#include "fesapi\fesapi.h"
#include "mail\mail.h"
#include "utils\misc.h"
#include "ace/Thread_Manager.h"

CRtdbModule::CRtdbModule(CServer* pServer) : CBaseModule(pServer)
{
}

CRtdbModule::~CRtdbModule(void)
{
}

bool CRtdbModule::Initialize()
{
	if(CreateIO()==K_FALSE)
		return false;

	if (LoadDrivers() == K_FALSE)
		return false;

	if (Mail::CreateMailBox(SMKEY_MAIL_RTDB)==K_FALSE)
		return false;

	StartModule();

	return true;
}

void CRtdbModule::Run()
{
	if (Mail::RecvMail(SMKEY_MAIL_RTDB, m_mailbuf, MAIL_BUF_SIZE) == K_FALSE)
		return;

	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

	int nType = pMsgBase->Type;
}

void CRtdbModule::Shutdown()
{
	StopModule();

	UnLoadDrivers();

	DestroyIO();

	Mail::DestroyMailBox(SMKEY_MAIL_RTDB);
}
