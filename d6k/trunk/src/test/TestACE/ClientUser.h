#pragma once

#include "acereactor.h"
#include "handler.h"

#include "connector.h"


class CClientUser
{
public:
	CClientUser();
	~CClientUser();
public:
	void Start();

private:
	//反应器管理
public:
	ACE_Reactor* GetReactor() { return m_reactor.GetReactor(); }
private:
	CAceReactor m_reactor;

	CConnector<CPeerHandler<ACE_SOCK_Stream>, ACE_SOCK_CONNECTOR> m_connector;
};

