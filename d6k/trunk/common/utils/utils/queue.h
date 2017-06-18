
#ifndef _QUEUE_H
#define _QUEUE_H

#include "smallocator.h"

#include "..\utils\processmutex.h"
#include "..\utils\misc.h"

enum
{
	QOBJ_NONE,      //������
	QOBJ_READING,   //����
	QOBJ_WRITING,   //��д
	QOBJ_DATA,      //������
};

//Queueͷ��,��¼����״̬����
class CQueueTag
{     
public:
	int m_nMaxQueueObjCount;
	int m_nQueueObjCount;
public:
	int m_nPosHead;//ͷ
	int m_nPosTail;//β
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
	bool OpenQueue(int nQueueID,int nWaitTime=5);//��ʱʱ��5����
	void CloseQueue();

public:
	bool PushObject(char* pBuff, int nSize, int nWaitTime=10);
	bool PopObject(char* pBuff, int nSize, int nWaitTime=10);

private:
	TwithTag*  m_pQueueData;		//����������
	CQueueTag* m_pQueueTag;			//���б�ʶ��

private:
	CSMAllocator* m_pSMAllocator;	//�����ڴ������
	
public:
	CProcessMutex* GetProcessMutex() { return &m_process_mutex; }
private:
	CProcessMutex m_process_mutex;  //������
	HANDLE m_hEventHasData;		    //�������¼�

};

#include "queue.cpp"
#endif //_QUEUE_H
