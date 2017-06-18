#ifndef __PROCESS_MUTEX_H__
#define __PROCESS_MUTEX_H__

#ifdef WIN32
#include <Windows.h>
#endif

#ifdef linux
#include <unistd.h>
#include <semaphore.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#endif

class CProcessMutex
{
public:
	/* 默认创建匿名的互斥 */
	CProcessMutex();
	~CProcessMutex();
	
public:
	void Create(const char* name = NULL);
	void Create(int nID);
	void Destroy();
public:
	bool Lock();
	bool UnLock();

private:

#ifdef WIN32
	void* m_pMutex;
#endif

#ifdef linux
	sem_t* m_pSem;
#endif
	char m_cMutexName[30];
};

#endif

