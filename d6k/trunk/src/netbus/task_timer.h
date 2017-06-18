
#ifndef _TASK_TIMER_H_
#define _TASK_TIMER_H_

#include "ace/Task.h"
#include "ace/TP_Reactor.h"

#include "netbus/nbdef.h"

class CTimerEventHandler : public ACE_Event_Handler
{
public:

	CTimerEventHandler(NET_CONFIG *pNetConf, const int32u nMyNodeIdx, const int32u nAnotherIndex) : m_timerID(0)
    {
		m_pNetConfig = pNetConf;
		m_nMyNodeIdx = nMyNodeIdx;
		m_nAnotherIndex = nAnotherIndex;
    }

	~CTimerEventHandler()
	{
		if (m_pReactor != nullptr)
		{
			m_pReactor->cancel_timer(m_timerID);
		}		
	}

public:

    // arg1：参数
    // arg2：延迟多久后定时器开始启动
    // arg3：定时器继续触发的时间间隔，如果采用默认值，则表示定时器只在延迟后触发一次
	long StartTimer(const void* arg, const ACE_Time_Value &delay, const ACE_Time_Value &interval = ACE_Time_Value::zero);

public:

	virtual int handle_timeout(const ACE_Time_Value& current_time, const void* act = 0);

	virtual int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);

private:

	void DoWork();

private:


	NET_CONFIG* m_pNetConfig;
	int32u m_nMyNodeIdx;
	int32u m_nAnotherIndex;

    long m_timerID;

	ACE_Reactor* m_pReactor;
};


class CCheckNodeState : public ACE_Task_Base
{
public:

	CCheckNodeState(NET_CONFIG *pNetConf, const int32u nMyNodeIdx, const int32s nAnotherIndex, const int nType) : m_nType(nType)
	{
		m_pTtimer = nullptr;
		m_pNetConfig = pNetConf;
		m_nMyNodeIdx = nMyNodeIdx;
		m_nAnotherIndex = nAnotherIndex;
		m_pReactor = new ACE_Reactor();
	}

	~CCheckNodeState()
	{
		if (m_pReactor != nullptr)
		{
			delete m_pReactor;
			m_pReactor = nullptr;
		}

		if (m_pTtimer != nullptr)
		{
			delete m_pTtimer;
			m_pTtimer = nullptr;
		}
	}

	virtual int svc(void)
	{
		m_pReactor->owner(ACE_OS::thr_self());

		Run();

		return 0;
	}

	virtual int close(u_long flags = 0)
	{
		if (ACE_OS::thr_equal(ACE_Thread::self(), this->last_thread()))
		{
			// 释放对象
			// 这里是对象不用释放。
			//delete this;
		}

		

		return 0;
	}

public:

	long Run();

	CTimerEventHandler* m_pTtimer;

	NET_CONFIG* m_pNetConfig;
	int32u m_nMyNodeIdx;
	int32s m_nAnotherIndex;

	// 1：检查主备；2：检查整个网络节点状态
	const int m_nType;

	ACE_Reactor* m_pReactor;
};

#endif
