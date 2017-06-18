/*! @file queue_impl.cpp
<PRE>
********************************************************************************
ģ����       :  ���ڹ����ڴ��IPC
�ļ���       :  queue_impl.cpp
�ļ�ʵ�ֹ��� :  ���ڹ����ڴ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ��Ҫע����SCADA������FES������ͬһ���̿ռ���
                              ��ģ������ģ��Ҳ����������ͬһ���̿ռ���
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ڹ����ڴ����
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/

#include "queue_impl.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/date_time.hpp>
#include <QDebug>

 
CQueue::CQueue() :m_nQueueID(0), m_nOpenCount(0), m_nSentMailID(0)
{
	m_nOpenCount = 0;
	m_bIsOpened = false;
}

CQueue::~CQueue()
{

} 

bool CQueue::Create(const char *pszName, size_t nNum, size_t nSize)
{
	Q_ASSERT(pszName);
	if (pszName == nullptr)
		return false;

	Q_ASSERT(strlen(pszName) > 0);
	if (strlen(pszName) <= 0)
		return false;
	
	m_szName = pszName;

	using namespace boost::interprocess;

	try
	{
		message_queue::remove(pszName);

		m_pQue = std::make_shared<message_queue>
			(create_only,
			pszName,
			nNum,
			nSize
			);
	}	
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);		
		//std::cout << ex.what() << std::endl;
		return false;
	}

	return true;
}

void  CQueue::Destroy()
{
	Q_ASSERT(m_szName.empty() == false);
	if (m_szName.empty() == true)
		return  ;

	using namespace boost::interprocess;
	try
	{
		message_queue::remove(m_szName.c_str());
		m_pQue.reset();
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return  ;
	}
}
 
bool CQueue::Open(const char *pszName)
{
	Q_ASSERT(pszName);
	if (pszName == nullptr)
		return false;

	Q_ASSERT(strlen(pszName) > 0);
	if (strlen(pszName) <= 0)
		return false;

	m_szName = pszName;

	if (m_nOpenCount>0)
	{
		return true;
	}
	using namespace boost::interprocess;
	try
	{
		// TODO:����ʱ��Ҫע�� ���removeҪ��Ҫ
		//message_queue::remove(pszName);

		m_pQue = std::make_shared<message_queue>
			(open_only,
			pszName
			);

		m_nOpenCount++;
		m_bIsOpened = true;
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}

	return true;
}

void CQueue::Close( )
{
	Q_ASSERT(m_szName.empty() == false);
	if (m_szName.empty() == true)
		return;
	m_nOpenCount--;
	if (m_nOpenCount > 0)
	{
		return;
	}

	using namespace boost::interprocess;
	try
	{
		message_queue::remove(m_szName.c_str());
		m_pQue.reset();
		m_bIsOpened = false;
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return;
	}
}
/*! \fn bool  CQueue::Send(const unsigned char *pData, size_t nSize, unsigned int nPriority)
********************************************************************************************************* 
** \brief CQueue::Send 
** \details ������Ϣ�������������������
** \param pData 
** \param nSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016��9��14�� 
** \note ��Ҫ�����£����û�д��Ƿ�������
********************************************************************************************************/
bool  CQueue::Send(const unsigned char *pData, size_t nSize, unsigned int nPriority)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pData && nSize);
	if (pData == nullptr || nSize == 0)
		return false;

	using namespace boost::interprocess;
	try
	{
		//progress_timer pt;//��¼ʱ�䣬���ڲ���
		m_pQue->send(pData, nSize, nPriority);	 
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}
/*! \fn bool  CQueue::TrySend(const unsigned char *pData, size_t nSize, unsigned int nPriority)
********************************************************************************************************* 
** \brief CQueue::TrySend 
** \details ������Ϣ�������������������������false
** \param pData 
** \param nSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016��9��14�� 
** \note 
********************************************************************************************************/
bool  CQueue::TrySend(const unsigned char *pData, size_t nSize, unsigned int nPriority)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pData && nSize);
	if (pData == nullptr || nSize == 0)
		return false;

	using namespace boost::interprocess;
	try
	{
		return m_pQue->try_send(pData, nSize, nPriority);
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}
/*! \fn bool CQueue::Send(const unsigned char *pData, size_t nSize, unsigned int nPriority, unsigned int nTimeOut)
********************************************************************************************************* 
** \brief CQueue::Send 
** \details ������Ϣ�����������������������ʱ
** \param pData 
** \param nSize 
** \param nPriority 
** \param nTimeOut ����
** \return bool 
** \author LiJin 
** \date 2016��9��14�� 
** \note 
********************************************************************************************************/
bool CQueue::Send(const unsigned char *pData, size_t nSize, unsigned int nPriority, unsigned int nTimeOut)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pData && nSize);
	if (pData == nullptr || nSize == 0)
		return false;

	using namespace boost::interprocess;
 
	try
	{
		boost::posix_time::ptime  abs_time(boost::posix_time::microsec_clock::universal_time());
		boost::posix_time::time_duration td = boost::posix_time::milliseconds(nTimeOut);
		abs_time += td;	 

		return m_pQue->timed_send(pData, nSize, nPriority, abs_time);
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}
/*! \fn bool CQueue::Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority)
********************************************************************************************************* 
** \brief CQueue::Receive 
** \details ��������
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016��9��15�� 
** \note 
********************************************************************************************************/
bool CQueue::Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pBuff && nBuffSize);
	if (pBuff == nullptr || nBuffSize == 0)
		return false;

	using namespace boost::interprocess;

	try
	{
		m_pQue->receive(pBuff, nBuffSize, nRecvdSize, nPriority);
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}
/*! \fn bool CQueue::TryReceive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority)
********************************************************************************************************* 
** \brief CQueue::TryReceive 
** \details ����������
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016��9��15�� 
** \note 
********************************************************************************************************/
bool CQueue::TryReceive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pBuff && nBuffSize);
	if (pBuff == nullptr || nBuffSize == 0)
		return false;

	using namespace boost::interprocess;

	try
	{
		return m_pQue->try_receive(pBuff, nBuffSize, nRecvdSize, nPriority);
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}

/*! \fn bool CQueue::Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority, unsigned int nTimeOut)
********************************************************************************************************* 
** \brief CQueue::Receive 
** \details ����ʱ����������
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \param nTimeOut 
** \return bool 
** \author LiJin 
** \date 2016��9��15�� 
** \note 
********************************************************************************************************/
bool CQueue::Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority, unsigned int nTimeOut)
{
	Q_ASSERT(m_pQue != nullptr);
	if (m_pQue == nullptr)
		return false;
	Q_ASSERT(pBuff && nBuffSize);
	if (pBuff == nullptr || nBuffSize == 0)
		return false;

	using namespace boost::interprocess;

	try
	{
		boost::posix_time::time_duration td = boost::posix_time::milliseconds(nTimeOut);
	//	boost::posix_time::ptime  abs_time(boost::posix_time::second_clock::local_time());
		boost::posix_time::ptime  abs_time(boost::posix_time::microsec_clock::universal_time());
		abs_time += td;

		return m_pQue->timed_receive(pBuff, nBuffSize, nRecvdSize, nPriority, abs_time);
	}
	catch (interprocess_exception &ex)
	{
		qDebug() << ex.what();
		Q_ASSERT(false);
		//std::cout << ex.what() << std::endl;
		return false;
	}
	return true;
}

/** @}*/
