
#ifndef _QUEUE_H
#define _QUEUE_H

#include "smallocator.h"

#include "..\utils\processmutex.h"
#include "..\utils\misc.h"

enum
{
	QOBJ_NONE,      //无数据
	QOBJ_READING,   //正读
	QOBJ_WRITING,   //正写
	QOBJ_DATA,      //有数据
};

//Queue头部,记录队列状态数据
class CQueueTag
{     
public:
	int m_nMaxQueueObjCount;
	int m_nQueueObjCount;
public:
	int m_nPosHead;//头
	int m_nPosTail;//尾
};

template<typename T>
class CQueue
{
	class TwithTag
	{
	public:
		T m_T;
	private:
		int m_nObjState;
	public:
		int   GetObjState() {  return m_nObjState; }
		void  SetObjState(int nNewState) {  m_nObjState = nNewState; }
	};

public:
	CQueue()
	{
		m_pSMAllocator = new CSMAllocator;
		m_pQueueTag = NULL;
		m_pQueueData = NULL; 
		m_hEventHasData = NULL;
	}
	~CQueue()
	{
		if(m_pSMAllocator!=NULL) 
		{
			delete m_pSMAllocator;
			m_pSMAllocator=NULL;
		} 
	}
public:
	bool CreateQueue(int nQueueID,int nQueueSize);
	bool OpenQueue(int nQueueID,int nWaitTime=5);//超时时间5秒钟
	void CloseQueue();

public:
	bool PushObject(char* pBuff, int nSize, int nWaitTime=10);
	bool PopObject(char* pBuff, int nSize, int nWaitTime=10);

private:
	TwithTag*  m_pQueueData;		//队列数据区
	CQueueTag* m_pQueueTag;			//队列标识区

private:
	CSMAllocator* m_pSMAllocator;	//共享内存分配器
	
public:
	CProcessMutex* GetProcessMutex() { return &m_process_mutex; }
private:
	CProcessMutex m_process_mutex;  //进程锁
	HANDLE m_hEventHasData;		    //有数据事件

};

#include "queue.cpp"
#endif //_QUEUE_H
