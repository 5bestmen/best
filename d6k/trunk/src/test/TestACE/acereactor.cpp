
#include "ace/Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "acereactor.h"
#include "ace/OS_NS_unistd.h"

CAceReactor::CAceReactor()
{
	ACE_TP_Reactor* tp_reactor = new ACE_TP_Reactor;
//	ACE_WFMO_Reactor* tp_reactor = new ACE_WFMO_Reactor;

	m_pReactor = new ACE_Reactor(tp_reactor, 1);
	ACE_Reactor::instance(m_pReactor);
	m_bIsLooping = false;
}

CAceReactor::~CAceReactor(void)
{
	delete m_pReactor;
}

int CAceReactor::svc()
{
	m_bIsLooping = true;

	//设置reactor所属线程
	m_pReactor->owner(ACE_OS::thr_self());

	static bool IsReactorActive = false;
	if (!IsReactorActive)
	{
		IsReactorActive = true;
		int nret = m_pReactor->run_reactor_event_loop();
	}
	else
	{
		m_pReactor->reset_reactor_event_loop();
		int nret = m_pReactor->run_reactor_event_loop();
	}

	m_bIsLooping = false;

	return 0;
}
bool CAceReactor::StartReactor()
{
	this->activate();

	return true;
}

bool CAceReactor::StopReactor()
{
	if (m_pReactor && m_bIsLooping)
	{
		m_pReactor->end_reactor_event_loop();
	}

	int nRet = this->wait();

#if 0
	//等到reactor线程结束
	while (m_bIsLooping)
	{
		ACE_OS::sleep(ACE_Time_Value(0, 1000)); //暂停1毫秒,
	}
#endif

	return true;
}
