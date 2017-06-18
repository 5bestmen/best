
#ifndef _ARRAY_CPP
#define _ARRAY_CPP


#include "array.h"

//////////////////////////////////////////////////////////////////////////
template<typename T>
T* CArray<T>::GetObject(int nIndex)
{ 
	if( (m_pArrayTag == NULL) || (m_pArrayData ==NULL) )
		return NULL;

	if(nIndex <0 || nIndex >= m_pArrayTag->m_nMaxArrayObjCount )
		return NULL;

	TwithTag* pObject = m_pArrayData + nIndex;

	return &(pObject->m_T);
}

template<typename T>
int CArray<T>::GetArraySize()
{
	if(m_pArrayTag == NULL)
		return 0;

	return m_pArrayTag->m_nMaxArrayObjCount;
}

#endif //_ARRAY_CPP

