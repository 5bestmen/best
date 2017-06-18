#ifndef   _THREADSYNC_H
#define   _THREADSYNC_H

#ifdef _WIN32
#include<windows.h>
#else
#include <pthread.h>
#endif

#ifndef _WIN32
class CThreadMutex
{
private:
    pthread_mutex_t m_mutex;
	
public:
	//手动上锁，手动解锁
    CThreadMutex();
	//传入参数为true时，自动上锁、解锁；为false时，跟默认构造函数意义相同，
	CThreadMutex(bool isLocker);

    ~CThreadMutex();
    void Lock();
    void UnLock();
private:
	bool m_isLocker; //是否已经上锁
};

#else
class CThreadMutex
{
private:
    CRITICAL_SECTION m_mutex;
	
public:
	//手动上锁，手动解锁
    CThreadMutex()
    {
        ::InitializeCriticalSection(&m_mutex);
    };

	//传入参数为true时，自动上锁、解锁；为false时，跟默认构造函数意义相同，
	CThreadMutex(bool isLocker)
	{
		CThreadMutex();
		if(isLocker)
			Lock();
		m_isLocker = isLocker;
	}
    ~CThreadMutex()
    {
		if(m_isLocker)
			UnLock();
        ::DeleteCriticalSection(&m_mutex);
    };
    void Lock()
    {
		if(!m_isLocker)
			::EnterCriticalSection(&m_mutex);        
    };
    void UnLock()
    {
        ::LeaveCriticalSection(&m_mutex);        
    };
private:
	bool m_isLocker; //是否已经上锁
};
#endif

#endif  //_THREADSYNC_H