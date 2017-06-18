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
	//�ֶ��������ֶ�����
    CThreadMutex();
	//�������Ϊtrueʱ���Զ�������������Ϊfalseʱ����Ĭ�Ϲ��캯��������ͬ��
	CThreadMutex(bool isLocker);

    ~CThreadMutex();
    void Lock();
    void UnLock();
private:
	bool m_isLocker; //�Ƿ��Ѿ�����
};

#else
class CThreadMutex
{
private:
    CRITICAL_SECTION m_mutex;
	
public:
	//�ֶ��������ֶ�����
    CThreadMutex()
    {
        ::InitializeCriticalSection(&m_mutex);
    };

	//�������Ϊtrueʱ���Զ�������������Ϊfalseʱ����Ĭ�Ϲ��캯��������ͬ��
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
	bool m_isLocker; //�Ƿ��Ѿ�����
};
#endif

#endif  //_THREADSYNC_H