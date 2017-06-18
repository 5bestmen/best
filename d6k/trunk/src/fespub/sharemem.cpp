/*! @file sharemem.cpp
<PRE>
********************************************************************************
模块名       :  
文件名       : sharemem.cpp
文件实现功能 :
作者         :
版本         :
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   共享内存.
*  @author  LiJin
*  @version 1.0
*  @date    2016.04.26
*******************************************************************************/

#include "sharemem.h" 
#include <QDebug>
#ifdef USE_QT_SHM

#include <QSharedMemory>

 

CShareMem::CShareMem()
{
	
}

CShareMem::~CShareMem()
{

}
/*! \fn void *CShareMem::CreateShareMem(const char *szMemName, int nSize)
********************************************************************************************************* 
** \brief CShareMem::CreateShareMem 
** \details 
** \param szMemName 
** \param nSize 
** \return void * 
** \author LiJin
** \date 2016年4月24日 
** \note  需要测试下，不同平台，最大能够支持多大的共享内存
********************************************************************************************************/
void *CShareMem::CreateShareMem(const char *szMemName, int nSize)
{
	Q_ASSERT(szMemName != nullptr);
	Q_ASSERT(strlen(szMemName) > 0);
	Q_ASSERT(nSize > 0);

	if (szMemName == nullptr || strlen(szMemName) == 0)
		return nullptr;

	if (nSize <= 0)
		return nullptr;

	if (m_pShm)
	{
		m_pShm.reset();
	}

	m_pShm = std::make_shared<QSharedMemory>(szMemName);
//	QString szRet = m_pShm->key();
//	m_pShm->setKey(szMemName);

	bool bRet = m_pShm->create(nSize);
	Q_ASSERT(bRet);
	if (bRet)
	{
		m_pShm->unlock();
		return m_pShm->data();
	}
	else
	{// TODO :  LOG
		QString szErr = m_pShm->errorString();
		qDebug() << szErr;
	}

	return nullptr;
}

/*! \fn void *CShareMem::GetShareMem(const char * szMemName)
********************************************************************************************************* 
** \brief CShareMem::GetShareMem 
** \details 客户模块获取共享内存
** \param szMemName 
** \return void * 
** \author LiJin 
** \date 2016年4月26日 
** \note 
********************************************************************************************************/
void *CShareMem::GetShareMem(const char * szMemName)
{
	Q_ASSERT(szMemName != nullptr);
	Q_ASSERT(strlen(szMemName) > 0);

	if (szMemName == nullptr || strlen(szMemName) == 0)
		return nullptr;

	if (m_pShm)
	{
		m_pShm.reset();
	}

	m_pShm = std::make_shared<QSharedMemory>(szMemName);

	bool bRet = m_pShm->attach();
	Q_ASSERT(bRet);
//	void *p = m_pShm->data();
	if (bRet)
	{
		return m_pShm->data();
	}
	else
	{// TODO :  LOG
		QString szErr = m_pShm->errorString();
		qDebug() << szErr;
	}

	return nullptr;
}

bool CShareMem::Lock()
{
	if (m_pShm)
	{
		return m_pShm->lock();
	}
	return false;
}

bool CShareMem::UnLock()
{
	if (m_pShm)
	{
		return m_pShm->unlock();
	}
	return false;
}
/*@}*/


#else

CShareMem::CShareMem()
{
	m_pAllocator = NULL;
}

/*! \fn void * CShareMem::CreateShareMem(const char *szMemName, int nSize)
********************************************************************************************************* 
** \brief CShareMem::CreateShareMem 
** \details  ACE 分配器开辟共享内存
** \param szMemName 
** \param nSize 
** \return void * 
** \author xingzhibing
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void * CShareMem::CreateShareMem(const char *szMemName, int nSize)
{
	Q_ASSERT(szMemName && strlen(szMemName) > 0);
	if (!szMemName || strlen(szMemName) <= 0)
	{
		return false;
	}

	Q_ASSERT( nSize > 0 );
	if (nSize <=0)
	{
		return NULL;
	}

	if (!Create(szMemName,MemOptions::NEVER_FIXED))
	{
		return NULL;
	}

	Q_ASSERT(m_pAllocator);
	if (m_pAllocator==NULL)
	{
		return NULL;
	}

	char* pPool = (char*)m_pAllocator->malloc(nSize);
	if (pPool == NULL)
		ACE_ERROR_RETURN((LM_ERROR,
		ACE_TEXT("%p\n"),
		ACE_TEXT("Unable to malloc")),
		NULL);

	if (m_pAllocator->bind(szMemName, pPool) == -1)
		ACE_ERROR_RETURN((LM_ERROR,
		ACE_TEXT("%p\n"),
		ACE_TEXT("bind failed")),
		NULL);

	Q_ASSERT(pPool);
	return pPool;


}

/*! \fn void * CShareMem::GetShareMem(const char * szMemName)
********************************************************************************************************* 
** \brief CShareMem::GetShareMem 
** \details 
** \param szMemName 
** \return void * 
** \author xingzhibing
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void * CShareMem::GetShareMem(const char * szMemName)
{
	Q_ASSERT(szMemName && strlen(szMemName) > 0);
	if (!szMemName || strlen(szMemName) <= 0)
	{
		return NULL;
	}
	
	if (!Create(szMemName,MemOptions::ALWAYS_FIXED))
	{
		return NULL;
	}

	Q_ASSERT(m_pAllocator);
	if (m_pAllocator == NULL)
		return NULL;

	void* pdata = NULL;
	if (m_pAllocator->find(szMemName, pdata) == -1 || pdata == NULL)
		return NULL;

	return (char*)pdata;
}

/*! \fn bool  CShareMem::Create(const char* szMemName)
********************************************************************************************************* 
** \brief CShareMem::Create 
** \details 创建共享内存分配器
** \param szMemName 
** \return void 
** \author xingzhibing
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
bool  CShareMem::Create(const char* szMemName,MemOptions nMode)
{
	Q_ASSERT(szMemName && strlen(szMemName)>0);

	if (!szMemName ||strlen(szMemName)<=0  )
	{
		return false;
	}
	//创建共享内存分配器
	std::string strPath(szMemName);
	
	switch (nMode)
	{
	case CShareMem::MemOptions::ALWAYS_FIXED:
	{
		ACE_MMAP_Memory_Pool_Options options(0, ACE_MMAP_Memory_Pool_Options::ALWAYS_FIXED);

		if (m_pAllocator == NULL)
		{
			ACE_NEW_RETURN(m_pAllocator,
				ALLOCATOR(strPath.c_str(),
				strPath.c_str(),
				&options),
				false);
		}
		break;
	}		
	case CShareMem::MemOptions::NEVER_FIXED:
	{
		remove(strPath.c_str());
		ACE_MMAP_Memory_Pool_Options options(0, ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);

		if (m_pAllocator == NULL)
		{
			ACE_NEW_RETURN(m_pAllocator,
				ALLOCATOR(strPath.c_str(),     //pool_name
				strPath.c_str(),               //lock_name
				&options),                     //options
				false);
		}
		break;
	}		
	case CShareMem::MemOptions::DEFAULT:
	{
		ACE_MMAP_Memory_Pool_Options options(0, ACE_MMAP_Memory_Pool_Options::FIRSTCALL_FIXED);

		if (m_pAllocator == NULL)
		{
			ACE_NEW_RETURN(m_pAllocator,
				ALLOCATOR(strPath.c_str(),
				strPath.c_str(),
				&options),
				false);
		}
		break;
	}	
	default:
		break;
	}
	ACE_MMAP_Memory_Pool_Options options(0, ACE_MMAP_Memory_Pool_Options::NEVER_FIXED);
	
	if (m_pAllocator == NULL)
	{
		ACE_NEW_RETURN(m_pAllocator,
			ALLOCATOR(strPath.c_str(),
			strPath.c_str(),
			&options),
			false);
	}

	return true;
}

/*! \fn  void CShareMem::Destroy()
********************************************************************************************************* 
** \brief CShareMem::Destroy 
** \details 销毁共享内存
** \return void 
** \author xingzhibing
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
void CShareMem::Destroy()
{
	if (m_pAllocator != NULL)
	{
		m_pAllocator->remove();
		delete m_pAllocator;
		m_pAllocator = NULL;
	}
}

#endif // _ACE_MEM