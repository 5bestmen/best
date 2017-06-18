#include "ThreadMutex.h"

#ifndef _WIN32
CThreadMutex::CThreadMutex() 
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&m_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
	m_isLocker = false;
	
}
CThreadMutex::CThreadMutex(bool isLocker)
{
	CThreadMutex();
	if(isLocker)
		Lock();
	m_isLocker = isLocker;
}

CThreadMutex::~CThreadMutex() 
{
	if(m_isLocker)
		UnLock();
    pthread_mutex_destroy(&m_mutex);
}    

void CThreadMutex::Lock() 
{
	if(!m_isLocker)
		pthread_mutex_lock(&m_mutex);
}

void CThreadMutex::UnLock() 
{
    pthread_mutex_unlock(&m_mutex);
}

#endif