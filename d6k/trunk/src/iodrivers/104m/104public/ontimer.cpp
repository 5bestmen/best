
#include "ontimer.h"

#include <ace/Date_Time.h> 
#include <ace/OS_NS_sys_time.h>

#include "104impl.h"

COnTimer::COnTimer(IHost* pHost, C104Impl* pImpl)
{
	m_pHost = pHost;
	m_pImpl = pImpl;
}

COnTimer::~COnTimer()
{

}


bool COnTimer::OnTimer(int nTimerID)
{
	bool bRet = true;
	switch (nTimerID)
	{
	case TIMER_ID_T1:
		bRet = OnTimerT1();
		break;
	case TIMER_ID_T2:
		bRet = OnTimerT2();
		break;
	case TIMER_ID_T3:
		bRet = OnTimerT3();
		break;
	}

	return bRet;
}

//ȷ�ϳ�ʱ//����false��ʾ�ر�����(��ʱ��ֻ��T1��ʱ�Ź���)
bool COnTimer::OnTimerT1()
{
	m_pHost->RemoveTimer(TIMER_ID_T1);

	m_pHost->LogCallback104(MCB_TIMER, TYPE_T, "OnTimerT1");

	return false;
}

//ʼ�շ���true
bool COnTimer::OnTimerT2()
{
	if (m_pImpl->m_w > 0)
	{
		m_pImpl->GetSender()->Send_S();
	}

	return true;
}

//��·���м�ʱ��T3��ʱ������ TESTFR����
//ʼ�շ���true
bool COnTimer::OnTimerT3()
{
	//���Ͳ��Ա���
	m_pImpl->GetSender()->Send_U(TESTFR_ACT);

	m_pHost->LogCallback104(MCB_TIMER, TYPE_T, "OnTimerT3");

	return true;
}
