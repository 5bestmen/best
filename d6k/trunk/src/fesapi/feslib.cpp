

#include "feslib.h"
#include "mem_db.h"
#include "fesapi/fesdatadef.h"
#include "tagname_pool.h"
#include "stringpool.h"
#include <QDebug>


CFesApi::CFesApi(void)
{
	m_nRefCount = 0;
	m_pMemDB = std::make_shared<CMemDB>(); 
	m_pTagNamePool = std::make_shared<CTagNamePool>();
	m_pStringPool = std::make_shared<CStringPool>();
}


CFesApi::~CFesApi(void)
{
	 
}

bool CFesApi::Initialize(const char *pszDataPath, int32u nChannelOccNO, unsigned int nMode)
{
	// 如果初始化过，则不再初始化
	if (m_nRefCount > 0)
	{
		m_nRefCount++;
		return true;
	}
	else
	{
		m_nRefCount++;
	}

	if (m_pMemDB)
	{
		m_pMemDB->Initialize(pszDataPath, nMode);
	}

	if (m_pTagNamePool)
	{
		m_pTagNamePool->Initialize(pszDataPath, nMode);
	}

	if (m_pStringPool)
	{
		m_pStringPool->Initialize(pszDataPath, nMode);
	}

	if (nChannelOccNO!=INVALID_OCCNO)
	{
		//打开内存之后 打开驱动邮箱
		if (!m_pMemDB->OpenChannelMailBox(nChannelOccNO))
		{
			return false;
		}
	}	
	return true;
}

void CFesApi::Run()
{

}

void CFesApi::Shutdown(int32u nChannelOccNO )
{
	m_nRefCount--;
	if (m_nRefCount <= 0)
	{// 准备退出
		if (m_pTagNamePool)
		{
			m_pTagNamePool->Shutdown();
		}
		if (nChannelOccNO!=INVALID_OCCNO)
		{
			m_pMemDB->CloseChannelMailBox(nChannelOccNO);
		}
		if (m_pMemDB)
		{			
			m_pMemDB->Shutdown( );
		}
		if (m_pStringPool)
		{
			m_pStringPool->Shutdown();
		}
	}
} 
