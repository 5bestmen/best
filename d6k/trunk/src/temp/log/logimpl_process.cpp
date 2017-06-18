#include "LogImpl.h"

#include "ace\OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/CDR_Stream.h"

#include "Utils/misc.h"

//�̺߳���
void CLogImpl::LogProcessThreadProc(void * lpParameter)
{
	CLogImpl * pthis = (CLogImpl *)lpParameter;

	pthis->m_bIsProcessStopped = false;

	while ( pthis->m_bIsProcessing )
	{
		pthis->DoLogProcess();

		Utils::USleep(1);//��ͣ1����
	}

	pthis->m_bIsProcessStopped = true;
}

bool CLogImpl::DoLogProcess()
{
	m_nCurrTime = Utils::GetTickSecond();

	int nCacheSize = m_logCache.GetSize();
	if( nCacheSize <= 0 )
		return false;
		
	//δ�ﵽ��������
	if( (m_nCurrTime - m_nLastTime < LOG_HANDLE_CYCLE) && (m_logCache.GetSize() < LOG_HANDLE_COUNT) )
		return false;
	
	if(m_bIsLog2File)
	{
		Log2File();
	}
	else
	{
		Log2Sqlite();
	}
	
	m_nLastTime = Utils::GetTickSecond();

	return true;
}

void CLogImpl::StartProcess()
{
	m_bIsProcessing = true;
	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)LogProcessThreadProc,this);
}

void CLogImpl::StopProcess()
{
	//�˳�ʱ��ȷ�����ػ������־�Ѿ�ȫ��д�����ݿ�
	while(true)
	{
		if(m_logCache.GetSize()<=0)
		{
			m_bIsProcessing = false;
			break;
		}

		Utils::USleep(1);//��ͣ1����
	}
	
	while(!m_bIsProcessStopped)
	{
		Utils::USleep(1);//��ͣ1����
	}
}


