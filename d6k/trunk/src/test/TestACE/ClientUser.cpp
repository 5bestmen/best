#include "stdafx.h"
#include "ClientUser.h"


CClientUser::CClientUser()
{
}


CClientUser::~CClientUser()
{
	m_reactor.StopReactor();
}


void CClientUser::Start()
{
	m_connector.reactor(this->GetReactor());

	m_connector.init(0, nullptr);
	m_reactor.StartReactor();

}
