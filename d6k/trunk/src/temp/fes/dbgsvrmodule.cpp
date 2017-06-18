
#include "dbgsvrmodule.h"

#include "fesapi\fesapi.h"
#include "mail\mail.h"

CDbgSvrModule::CDbgSvrModule(CServer* pServer) : CBaseModule(pServer)
{
}

CDbgSvrModule::~CDbgSvrModule(void)
{
}

bool CDbgSvrModule::Initialize()
{
	if (Mail::CreateMailBox(SMKEY_MAIL_DBGSVR)==K_FALSE)
		return false;

	StartModule();

	return true;
}

void CDbgSvrModule::Run()
{
	if (Mail::RecvMail(SMKEY_MAIL_DBGSVR, m_mailbuf, MAIL_BUF_SIZE)==K_FALSE)
		return;

	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

	int nType = pMsgBase->Type;
}

void CDbgSvrModule::Shutdown()
{
	StopModule();

	Mail::DestroyMailBox(SMKEY_MAIL_DBGSVR);
}


