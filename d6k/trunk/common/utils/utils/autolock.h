#ifndef __AUTO_LOCK_H__
#define __AUTO_LOCK_H__

template <class LOCKCLASS>
class CAutoLock
{
public:
	CAutoLock (LOCKCLASS* pLock);
	~CAutoLock (void);

protected:
	LOCKCLASS* m_pLock;
};

#include "autolock.cpp"
#endif //__AUTO_LOCK_H__

