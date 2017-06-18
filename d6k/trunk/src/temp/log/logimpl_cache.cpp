#include "LogImpl.h"

#include "ace\OS_NS_unistd.h"
#include "ace/Thread_Manager.h"

#include "Utils/misc.h"


//线程函数
void CLogImpl::LogCacheThreadProc(void * lpParameter)
{
	CLogImpl * pthis = (CLogImpl *)lpParameter;

	pthis->m_bIsCacheStopped = false;

	while ( pthis->m_bIsCacheing )
	{
		pthis->DoLogCache();

		Utils::USleep(1);//暂停1毫秒
	}

	pthis->m_bIsCacheStopped = true;
}

bool CLogImpl::DoLogCache()
{
	LOGDATA logdata;
	if(!m_logQueue.PopObject((char*)(&logdata),sizeof(LOGDATA),10))
		return false;

	m_logCache.PushCache(logdata);

	return true;
}

bool CLogImpl::StartCache()
{
	if(!CreateCache())
		return false;

	m_bIsCacheing = true;
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)LogCacheThreadProc,this);

	return true;
}

void CLogImpl::StopCache()
{
	m_bIsCacheing = false;
	while(!m_bIsCacheStopped)
	{
		Utils::USleep(1);//暂停1毫秒
	}

	CloseCache();
}

bool CLogImpl::CreateCache()
{
	//1、创建共享内存日志队列
	return m_logQueue.CreateQueue(m_nLogID,LOG_QUEUE_SIZE);
}

bool CLogImpl::OpenCache()
{
	return m_logQueue.OpenQueue(m_nLogID);
}

void CLogImpl::CloseCache()
{
	return m_logQueue.CloseQueue();
}
