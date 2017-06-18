#ifndef _SM_ALLOCATOR_H
#define _SM_ALLOCATOR_H

#include "smapi.h"

class CSMAllocator
{
public:
	CSMAllocator(void);
	~CSMAllocator(void);

public:
	char* Create(SMKey key, int nSize);
		
	char* Open(SMKey key);

	void  Close();

private:
	SMHandle m_SMHandle;
	SMKey    m_Key;
private:
	char*    m_pBlock;
};

#endif //_SM_ALLOCATOR_H
