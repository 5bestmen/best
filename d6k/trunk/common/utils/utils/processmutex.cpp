
#include "ProcessMutex.h"
#include <stdio.h>

#ifdef WIN32

CProcessMutex::CProcessMutex()
{
	/*
	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
	strncpy(m_cMutexName, name, min);
	m_pMutex = CreateMutex(NULL, false, m_cMutexName);
	*/
	m_pMutex = NULL;
}

CProcessMutex::~CProcessMutex()
{
	/*
	CloseHandle(m_pMutex);
	*/
}

void CProcessMutex::Create(const char* name)
{
	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
	strncpy(m_cMutexName, name, min);
	m_pMutex = CreateMutex(NULL, false, m_cMutexName);
}

void CProcessMutex::Create(int nID)
{
	char szName[64] = {'\0'};
	sprintf(szName, "%d", nID);

	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(szName)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(szName);
	strncpy(m_cMutexName, szName, min);
	m_pMutex = CreateMutex(NULL, false, m_cMutexName);
}

void CProcessMutex::Destroy()
{
	ReleaseMutex(m_pMutex);
	CloseHandle(m_pMutex);
}

bool CProcessMutex::Lock()
{
	//»¥³âËø´´½¨Ê§°Ü
	if (NULL == m_pMutex)
	{
		return false;
	}

	DWORD nRet = WaitForSingleObject(m_pMutex, INFINITE);
	//if(WaitForSingleObject(m_hResMutex, waitTime) == WAIT_OBJECT_0)

	if (nRet != WAIT_OBJECT_0)
	{
		return false;
	}

	return true;
}

bool CProcessMutex::UnLock()
{
	return ReleaseMutex(m_pMutex);
}

#endif

#ifdef linux

CProcessMutex::CProcessMutex(const char* name)
{
	/*
	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
	strncpy(m_cMutexName, name, min);
	m_pSem = sem_open(name, O_RDWR | O_CREAT, 0644, 1);
	*/
}

CProcessMutex::~CProcessMutex()
{
	/*
	int ret = sem_close(m_pSem);
	if (0 != ret)
	{
		printf("sem_close error %d\n", ret);
	}
	sem_unlink(m_cMutexName);
	*/
}
void CProcessMutex::Create(const char* name)
{
	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(name)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(name);
	strncpy(m_cMutexName, name, min);
	m_pSem = sem_open(name, O_RDWR | O_CREAT, 0644, 1);
}
void CProcessMutex::Create(int nID)
{
	char szName[64] = {'\0'};
	sprintf(szName, "%d", nID);

	memset(m_cMutexName, 0 ,sizeof(m_cMutexName));
	int min = strlen(szName)>(sizeof(m_cMutexName)-1)?(sizeof(m_cMutexName)-1):strlen(szName);
	strncpy(m_cMutexName, szName, min);
	m_pSem = sem_open(szName, O_RDWR | O_CREAT, 0644, 1);
}
void CProcessMutex::Destroy()
{
	int ret = sem_close(m_pSem);
	if (0 != ret)
	{
		printf("sem_close error %d\n", ret);
	}
	sem_unlink(m_cMutexName);
}
bool CProcessMutex::Lock()
{
	int ret = sem_wait(m_pSem);
	if (ret != 0)
	{
		return false;
	}
	return true;
}

bool CProcessMutex::UnLock()
{
	int ret = sem_post(m_pSem);
	if (ret != 0)
	{
		return false;
	}
	return true;
}

#endif
