
#include "smallocator.h"

CSMAllocator::CSMAllocator(void)
{
	m_SMHandle  = SM_INVALID_HANDLE;
	
	m_pBlock     = NULL;
}

CSMAllocator::~CSMAllocator(void)
{
	Close();
}

char* CSMAllocator::Create(SMKey key, int nSize)
{
	m_SMHandle = SMAPI::CreateShareMemory( key , nSize );
	if(SM_INVALID_HANDLE == m_SMHandle)
	{
		return NULL;
	}

	m_pBlock = SMAPI::MapShareMemory(m_SMHandle);
	
	return m_pBlock;
}

char* CSMAllocator::Open(SMKey key)
{
	m_SMHandle = SMAPI::OpenShareMemory(key);
	if(SM_INVALID_HANDLE == m_SMHandle)
	{
		return NULL;
	}

	m_pBlock = SMAPI::MapShareMemory(m_SMHandle);
	
	return m_pBlock;
}

void CSMAllocator::Close()
{
	if(m_pBlock != NULL)
	{
		SMAPI::UnMapShareMemory(m_pBlock);
		m_pBlock = NULL;
	}

	if(m_SMHandle != SM_INVALID_HANDLE)
	{
		SMAPI::CloseSharememory(m_SMHandle);
		m_SMHandle = SM_INVALID_HANDLE;
	}
}
