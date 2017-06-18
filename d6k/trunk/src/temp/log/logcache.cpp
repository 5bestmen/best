#include "LogCache.h"


CLogCache::CLogCache(void)
{

}


CLogCache::~CLogCache(void)
{
}


void CLogCache::PushCache(LOGDATA& logdata)
{
	CAutoLock<CThreadMutex> lock(&m_logCacheMutex);
	m_cache.push(logdata);
}
bool CLogCache::PopCache(LOGDATA& logdata)
{
	CAutoLock<CThreadMutex> lock(&m_logCacheMutex);

	int nSize= m_cache.size();

	if(m_cache.size() <=0)
		return false;

	logdata = m_cache.front();
	m_cache.pop();

	return true;
}

int CLogCache::GetSize()
{
	return m_cache.size();
}
