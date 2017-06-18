
#ifndef _AUTO_LOCK_CPP_
#define _AUTO_LOCK_CPP_

#include "AutoLock.h"

template <class LOCKCLASS> 
CAutoLock<LOCKCLASS>::CAutoLock (LOCKCLASS* pLock) : m_pLock(pLock)
{
	m_pLock->Lock ();
}

template <class LOCKCLASS> 
CAutoLock<LOCKCLASS>::~CAutoLock (void)
{
	m_pLock->UnLock();
}

#endif //_AUTO_LOCK_CPP_

