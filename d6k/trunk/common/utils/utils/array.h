
#ifndef _ARRAY_H
#define _ARRAY_H

#include "smallocator.h"

#include "..\utils\processmutex.h"

enum
{
	AOBJ_NONE,        //无数据
	AOBJ_READING,   //正读
	AOBJ_WRITING,   //正写
	AOBJ_DATA,        //有数据
};
//////////////////////////////////////////////////////////////////////////

//Array头部,记录队列状态数据
class CArrayTag
{     
public:
	int m_nMaxArrayObjCount;
	int m_nArrayObjCount;
};

template<typename T>
class CArray
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
	CArray()
	{
		m_pSMAllocator = new CSMAllocator;
		m_pArrayTag = NULL;
		m_pArrayData = NULL;     
	}                                         

	~CArray()
	{
		if(m_pSMAllocator!=NULL) 
		{
			delete m_pSMAllocator;
			m_pSMAllocator=NULL;
		} 
	}
public:
	bool CreateArray(int nArrayID,int nArraySize)
	{
		int nBlockSize = sizeof(TwithTag)*nArraySize + sizeof(CArrayTag);
		char* pData = (char*)m_pSMAllocator->Create( nArrayID, nBlockSize );
		if(pData == NULL)
			return false;

		m_pArrayTag = (CArrayTag*)pData;
		m_pArrayTag->m_nMaxArrayObjCount = nArraySize;
		m_pArrayTag->m_nArrayObjCount =0;

		m_pArrayData = (TwithTag*)(pData + sizeof(CArrayTag));
		for ( int i = 0; i < nArraySize; i++ )
		{
			m_pArrayData[i].SetObjState(AOBJ_NONE);
		}

		m_process_mutex.Create(nArrayID);

		return true;
	}
	bool OpenArray(int nArrayID)
	{
		if(m_pSMAllocator==NULL)
			return false;

		char* pData = m_pSMAllocator->Open(nArrayID);
		if(pData == NULL)
			return false;

		m_pArrayTag = (CArrayTag*)pData;
		m_pArrayData = (TwithTag*)( pData + sizeof(CArrayTag));

		m_process_mutex.Create(nArrayID);

		return true;
	}
	void CloseArray()
	{
		if(m_pSMAllocator!=NULL)
		{
			m_pSMAllocator->Close();
		}

		m_process_mutex.Destroy();
	}
		
public:
	T*     GetObject(int nIndex);
	int    GetArraySize();

private:
	TwithTag*    m_pArrayData;
	CArrayTag*   m_pArrayTag;

private:
	CSMAllocator* m_pSMAllocator;

public:
	CProcessMutex* GetProcessMutex() {return &m_process_mutex;}
private:
	CProcessMutex m_process_mutex; //进程锁
};

#include "array.cpp"
#endif //_ARRAY_H