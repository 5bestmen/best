
#include "netbusmodule.h"
#include "fesapi\fesapi.h"
#include "mail\mail.h"

CNetBusModule::CNetBusModule(CServer* pServer) : CBaseModule(pServer)
{
}

CNetBusModule::~CNetBusModule(void)
{
}

bool CNetBusModule::Initialize()
{
	if (Mail::CreateMailBox(SMKEY_MAIL_NETBUS)==K_FALSE)
		return false;

	StartModule();

	return true;
}

void CNetBusModule::Run()
{
	if (Mail::RecvMail(SMKEY_MAIL_NETBUS, m_mailbuf, MAIL_BUF_SIZE)==K_FALSE)
		return;

	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

	int nType = pMsgBase->Type;
}

void CNetBusModule::Shutdown()
{
	StopModule();

	Mail::DestroyMailBox(SMKEY_MAIL_NETBUS);
}

