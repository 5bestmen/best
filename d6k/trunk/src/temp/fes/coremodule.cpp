
#include "coremodule.h"
#include "fesapi\fesapi.h"
#include "mail\mail.h"

CCoreModule::CCoreModule(CServer* pServer) : CBaseModule(pServer)
{
}

CCoreModule::~CCoreModule(void)
{
}

bool CCoreModule::Initialize()
{
	if (Mail::CreateMailBox(SMKEY_MAIL_CORE)==K_FALSE)
		return false;

	StartModule();

	return true;
}

void CCoreModule::Run()
{
	if (Mail::RecvMail(SMKEY_MAIL_CORE, m_mailbuf, MAIL_BUF_SIZE) == K_FALSE)
		return;

	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

	int nType = pMsgBase->Type;
}

void CCoreModule::Shutdown()
{
	StopModule();

	Mail::DestroyMailBox(SMKEY_MAIL_CORE);
}

