/*! @file queue_impl.cpp
<PRE>
********************************************************************************
模块名       :  基于共享内存的IPC
文件名       :  queue_impl.cpp
文件实现功能 :  基于共享内存队列
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  需要注意今后，SCADA可能与FES运行在同一进程空间内
                              主模块与子模块也可能运行在同一进程空间内
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基于共享内存队列
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
		// TODO:调试时需要注意 这个remove要不要
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
** \details 发送消息，如果队列满，则阻塞
** \param pData 
** \param nSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016年9月14日 
** \note 需要测试下，如果没有打开是否有问题
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
		//progress_timer pt;//记录时间，用于测试
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
** \details 发送消息，如果队列满，不阻塞，返回false
** \param pData 
** \param nSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016年9月14日 
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
** \details 发送消息，如果队列满，则阻塞到超时
** \param pData 
** \param nSize 
** \param nPriority 
** \param nTimeOut 毫秒
** \return bool 
** \author LiJin 
** \date 2016年9月14日 
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
** \details 阻塞接收
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016年9月15日 
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
** \details 非阻塞接收
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \return bool 
** \author LiJin 
** \date 2016年9月15日 
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
** \details 带超时的阻塞接收
** \param pBuff 
** \param nBuffSize 
** \param nRecvdSize 
** \param nPriority 
** \param nTimeOut 
** \return bool 
** \author LiJin 
** \date 2016年9月15日 
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
