 

#ifndef  _THREAD_H
#define  _THREAD_H


#include <string.h>

#ifndef ULONG
typedef unsigned long ULONG;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif

#ifdef WIN32
#define _AFXDLL
#include <process.h>
#include <afx.h>

#define THFNRETURN unsigned int
#define STDCALL __stdcall

#define THREAD_HANDLE HANDLE

#else //!WIN32
#include <pthread.h>
#define THFNRETURN void*
#define STDCALL

#define THREAD_HANDLE pthread_t
#endif

typedef THFNRETURN (STDCALL *ThreadFun)(void*);

#define MAX_THREAD 5


class CThread  
{
public:
	CThread();
	virtual ~CThread();

public:
    //线程创建,分配资源，对象消亡时必须调用DestroyThread，
	//否则会导致线程内的资源无法释放，导致内存泄露
	virtual bool CreateThread(ThreadFun, void* pThParam);
	
	//线程退出，释放资源
	virtual void DestroyThread();
private:

    //线程句柄
	THREAD_HANDLE m_ThreadPoll[MAX_THREAD];
	int m_iCurThreadCount;
	
};
#endif //_THREAD_H
