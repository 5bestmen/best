#include "LogImpl.h"

#include <ace/ACE.h>
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"
#include "ace/CDR_Stream.h"

#include "Utils/misc.h"

CLogImpl g_LogImpl;

CLogImpl::CLogImpl(void)
{
	m_strLogName="";
	m_nLogID = -1;

	m_nLastTime = Utils::GetTickSecond();
	m_nCurrTime = Utils::GetTickSecond();

	m_bIsLog2File = false;

	ACE::init();
}


CLogImpl::~CLogImpl(void)
{
	ACE::fini();
}


bool CLogImpl::CreateLog(char* szLogName)
{
	if(!PrepareLog(szLogName))
		return false;

	InitSqlite();

	//1���������ػ���
	StartProcess();

	//2�����������߳�
	StartCache();

	return true;
}

bool CLogImpl::DestroyLog()
{
	//1���رս����߳�
	StopCache();

	//2���رձ��ػ���
	StopProcess();

	UnInitSqlite();

	return true;
}

bool CLogImpl::OpenLog(char* szLogName)
{
	if(!PrepareLog(szLogName))
		return false;

	return  OpenCache();
}

bool CLogImpl::CloseLog()
{
	CloseCache();

	return true;
}

bool CLogImpl::LogMsg(char* szMoudleName,char *szLogTxt,int nLevel)
{
	//1�������Ϣ��
	CMsgLog log;
	log.m_nLogLevel = nLevel;
	log.m_strLogModule.SetMsg(szMoudleName);
	log.m_strLogText.SetMsg(szLogTxt);

	//2����Ϣд������
	ACE_OutputCDR payload(sizeof(LOGDATA));
	payload << log;

	int nLogSize = payload.total_length();
	if(nLogSize > sizeof(LOGDATA))
		nLogSize = sizeof(LOGDATA);

	//3�������й���logdata
	LOGDATA logdata;
	::memcpy(((char*)(&logdata)),payload.begin()->rd_ptr(),nLogSize);

	//4��д�빲���ڴ���־����
	m_logQueue.PushObject((char*)(&logdata),nLogSize,10);

	return true;
}

bool CLogImpl::PrepareLog(char* szLogName)
{
	if( (!m_strLogName.empty()) || m_nLogID != -1 )
		return false;

	m_strLogName = szLogName;
	
	//ȫ��תΪСд
	transform(m_strLogName.begin(),m_strLogName.end(),m_strLogName.begin(),tolower);
	
	char buf[128];
	sprintf(buf,"CLogImpl_%s",(char*)m_strLogName.c_str());

	m_nLogID = Utils::BKDRHash(buf);

	return true;
}
