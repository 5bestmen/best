
#ifndef _QUEUE_CPP
#define _QUEUE_CPP

#include "queue.h"

template<typename T>
bool CQueue<T>::CreateQueue(int nQueueID,int nQueueSize)
{
	int nBlockSize = sizeof(TwithTag)*nQueueSize + sizeof(CQueueTag);
	char* pData = (char*)m_pSMAllocator->Create( nQueueID, nBlockSize );
	if(pData == NULL)
		return false;

	m_pQueueTag = (CQueueTag*)pData;
	m_pQueueTag->m_nMaxQueueObjCount = nQueueSize;
	m_pQueueTag->m_nQueueObjCount =0;

	m_pQueueTag->m_nPosHead = -1;
	m_pQueueTag->m_nPosTail = -1;

	m_pQueueData = (TwithTag*)(pData + sizeof(CQueueTag));
	for ( int i = 0; i < nQueueSize; i++ )
	{
		m_pQueueData[i].SetObjState(QOBJ_NONE);
	}

	char buf[32];
	sprintf(buf,"queue:%d",nQueueID);
	m_process_mutex.Create(buf);
	
	//If lpName matches the name of another kind of object in the same name space (such as an existing semaphore, mutex, waitable timer, job, or file-mapping object), the function fails and the GetLastError function returns ERROR_INVALID_HANDLE. This occurs because these objects share the same name space
	sprintf(buf,"event:%d",nQueueID);
	m_hEventHasData = CreateEvent(NULL, TRUE, FALSE, buf);//初始为无信号状态

	int nError = GetLastError();
	if(nError == ERROR_INVALID_HANDLE)
	{
		int asdasdasdasd = 0;
	}

	return true;
}

template<typename T>
bool CQueue<T>::OpenQueue(int nQueueID,int nWaitTime)
{
	if(m_pSMAllocator==NULL)
		return false;

	//打开有数据事件(等待主程序创建成功)
	char buf[32];
	sprintf(buf,"event:%d",nQueueID);

	int nStartTime = Utils::GetTickSecond();
	while(true)
	{
		m_hEventHasData = OpenEvent(EVENT_ALL_ACCESS, FALSE, buf);
		if(m_hEventHasData != NULL)
			break;

		Utils::USleep(1); //暂停1毫秒

		int nCurTime = Utils::GetTickSecond();
		if(nCurTime - nStartTime >= nWaitTime)
			return false;
	}
	
	//打开队列
	char* pData = m_pSMAllocator->Open(nQueueID);
	if(pData == NULL)
		return false;

	m_pQueueTag = (CQueueTag*)pData;
	m_pQueueData = (TwithTag*)( pData + sizeof(CQueueTag));
		
	//创建进程锁
	sprintf(buf,"queue:%d",nQueueID);
	m_process_mutex.Create(buf);
	
	return true;
}

template<typename T>
void CQueue<T>::CloseQueue()
{
	if(m_pSMAllocator!=NULL)
	{
		m_pSMAllocator->Close();
	}

	m_process_mutex.Destroy();

	CloseHandle(m_hEventHasData);
}

template<typename T>
bool CQueue<T>::PushObject(char* pBuff, int nSize, int nWaitTime)
{
	//1、初始判断
	if(m_pQueueData == NULL)  //共享内存没有映射
		return false;

	if(pBuff == NULL || nSize <= 0 || nSize > sizeof(T))
		return false;

	//2、加锁
	if(!m_process_mutex.Lock())
		return false;

	//3、队列头调整
	m_pQueueTag->m_nPosHead++;
	if(m_pQueueTag->m_nPosHead >= m_pQueueTag->m_nMaxQueueObjCount)
		m_pQueueTag->m_nPosHead =0;
	
	//4、队列尾调整
	if(m_pQueueTag->m_nPosTail == -1)
		m_pQueueTag->m_nPosTail = m_pQueueTag->m_nPosHead;

	//5、赋值
	TwithTag* pEmptyObject = m_pQueueData + m_pQueueTag->m_nPosHead;
	T* pDestBuff = &(pEmptyObject->m_T);
	memcpy((char*)pDestBuff,pBuff,nSize);
	pEmptyObject->SetObjState(QOBJ_DATA);
	m_pQueueTag->m_nQueueObjCount +=1;

	//6、发有数据信号
	SetEvent(m_hEventHasData);

	//7、解锁
	m_process_mutex.UnLock();

	return true;
}


template<typename T>
bool CQueue<T>::PopObject(char* pBuff, int nSize, int nWaitTime)
{
	//1、初始判断
	if(m_pQueueData == NULL) //共享内存没有映射
		return false;

	if(pBuff == NULL || nSize < sizeof(T))
		return false;

	//2、等待数据信号
	if(WaitForSingleObject(m_hEventHasData, nWaitTime) != WAIT_OBJECT_0)//毫秒 INFINITE
		return false;

	//3、加锁
	if(!m_process_mutex.Lock())
		return false;

	//4、读数据，并调整队尾位置
	TwithTag* pObject = m_pQueueData + m_pQueueTag->m_nPosTail;
	if(pObject->GetObjState() == QOBJ_DATA)
	{
		T* pSrcBuff = &(pObject->m_T);
		memcpy(pBuff,(char*)pSrcBuff,sizeof(T));
		pObject->SetObjState(QOBJ_NONE);
		m_pQueueTag->m_nQueueObjCount -=1;
		
		if(m_pQueueTag->m_nPosTail == m_pQueueTag->m_nPosHead )
		{
			m_pQueueTag->m_nPosTail = -1;
			ResetEvent(m_hEventHasData); //读完，重置有数据信号
		}
		else
		{
			m_pQueueTag->m_nPosTail++;
			if(m_pQueueTag->m_nPosTail >= m_pQueueTag->m_nMaxQueueObjCount)
				m_pQueueTag->m_nPosTail =0;
		}
	}
	
	//5、解锁
	m_process_mutex.UnLock();

	return true;
}



#endif //_QUEUE_CPP

