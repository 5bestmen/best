
#ifndef _THREAD_MANAGER_H_
#define _THREAD_MANAGER_H_

#include "ace/Task.h"
#include "ace/Reactor.h"

class CWorkBase
{
public:
    virtual int DoWork() = 0;
};

// 线程间数据需要传输换成（ACE_Task<ACE_MT_SYNCH>）
class CThreadCreate : public ACE_Task_Base
{
public:

	CThreadCreate(CWorkBase* pWork) : m_pWork(pWork)
    {
		ACE_ASSERT(m_pWork);
    }

	~CThreadCreate()
	{

	}

    int ThreadCreate()
    {
        return activate();
    }

protected:

    virtual int svc()
    {
		m_pWork->DoWork();
        return 1;
    }

private:

    CWorkBase* m_pWork;
};

#endif
