
#include "task_timer.h"


long CCheckNodeState::Run()
{
	if (CHECK_HOST_SLAVE == m_nType)
	{
		// 延迟多久后定时器开始启动
		ACE_Time_Value t1(1);

		// 定时器继续触发的时间间隔，如果采用默认值，则表示定时器只在延迟后触发一次
		ACE_Time_Value t2(1);

		m_pTtimer = new CTimerEventHandler(m_pNetConfig, m_nMyNodeIdx, m_nAnotherIndex);

		m_pTtimer->StartTimer(m_pReactor, t1, t2);

		m_pReactor->run_reactor_event_loop();
	}
	else if (CHECK_NODE_NET_STATE == m_nType)
	{
		ACE_ASSERT(m_pNetConfig != nullptr && m_pNetConfig->pNodeConfig != nullptr);
		if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr)
		{
			return -1;
		}

		int32u nCnt = m_pNetConfig->NodeCount;
		for (int32u i = 0; i < nCnt; i++)
		{
			ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
			ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[i].TimeNetA;
			ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
			// 5秒收不到对侧心跳则认为该节点下线
			if (subTime > NODE_ALIVE_TIME)
			{
				m_pNetConfig->pNodeConfig[i].CardStatus[0] = 0;
			}
			else
			{
				m_pNetConfig->pNodeConfig[i].CardStatus[0] = 1;
			}

			// 双网
			if (m_pNetConfig->IsDoubleNet)
			{
				ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
				ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[i].TimeNetB;
				ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
				// 5秒收不到对侧心跳则认为该节点下线
				if (subTime > NODE_ALIVE_TIME)
				{
					m_pNetConfig->pNodeConfig[i].CardStatus[1] = 0;
				}
				else
				{
					m_pNetConfig->pNodeConfig[i].CardStatus[1] = 1;
				}
			}
		}

		m_pReactor->run_reactor_event_loop();
	}	

	return 0;
}

long CTimerEventHandler::StartTimer(const void* arg, const ACE_Time_Value &delay, const ACE_Time_Value &interval /*= ACE_Time_Value::zero*/)
{
	m_pReactor = (ACE_Reactor*)arg;

	m_timerID = m_pReactor->schedule_timer(this, arg, delay, interval);

	return m_timerID;
}

int CTimerEventHandler::handle_timeout(const ACE_Time_Value& current_time, const void* act /*= 0*/)
{
	DoWork();

	return 0;
}

int CTimerEventHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
	if (m_timerID)
	{
		m_pReactor->cancel_timer(this);
	}

	return 0;
}

void CTimerEventHandler::DoWork()
{
	ACE_ASSERT(m_pNetConfig != nullptr && m_pNetConfig->pNodeConfig != nullptr);
	ACE_ASSERT(m_nAnotherIndex >= 0 && m_nAnotherIndex < m_pNetConfig->NodeCount);
	ACE_ASSERT(m_nMyNodeIdx >= 0 && m_nMyNodeIdx < m_pNetConfig->NodeCount);
	ACE_ASSERT(m_nAnotherIndex != m_nMyNodeIdx);

	if (m_pNetConfig == nullptr || m_pNetConfig->pNodeConfig == nullptr ||
		m_nAnotherIndex < 0 || m_nAnotherIndex >= m_pNetConfig->NodeCount || m_nAnotherIndex == m_nMyNodeIdx)
	{
		return;
	}

	ACE_UINT64 nowTime = ACE_OS::gettimeofday().get_msec();
	ACE_UINT64 oldTime = m_pNetConfig->pNodeConfig[m_nAnotherIndex].SwitchTime;
	ACE_UINT64 subTime = (nowTime - oldTime) / 1000;
	// 5秒收到对侧心跳则进行主备切换
	if (subTime > HEARTBEAT_TIMEOUT)
	{
		if (m_pNetConfig->pNodeConfig[m_nMyNodeIdx].HostSlave != MAIN)
		{
			m_pNetConfig->pNodeConfig[m_nMyNodeIdx].HostSlave = MAIN;
		}

		m_pNetConfig->pNodeConfig[m_nAnotherIndex].HostSlave = SLAVE;
	}

	return;
}

