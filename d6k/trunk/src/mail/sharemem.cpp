/*! @file sharemem.cpp
<PRE>
********************************************************************************
ģ����       :  
�ļ���       : sharemem.cpp
�ļ�ʵ�ֹ��� :
����         :
�汾         :
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����ڴ�.
*  @author  LiJin
*  @version 1.0
*  @date    2016.04.26
*******************************************************************************/

#include "sharemem.h" 

#include <QSharedMemory>
#include <QDebug>
 

CShareMem::CShareMem()
{
	
}

CShareMem::~CShareMem()
{
	m_pShm->detach();
}
/*! \fn void *CShareMem::CreateShareMem(const char *szMemName, int nSize)
********************************************************************************************************* 
** \brief CShareMem::CreateShareMem 
** \details 
** \param szMemName 
** \param nSize 
** \return void * 
** \author LiJin
** \date 2016��4��24�� 
** \note  ��Ҫ�����£���ͬƽ̨������ܹ�֧�ֶ��Ĺ����ڴ�
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
** \details �ͻ�ģ���ȡ�����ڴ�
** \param szMemName 
** \return void * 
** \author LiJin 
** \date 2016��4��26�� 
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
