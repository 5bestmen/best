/*! @file queue_impl.h
<PRE>
********************************************************************************
模块名       :
文件名       :  queue_impl.h
文件实现功能 :  跨进程队列的处理，
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  因为考虑到跨平台的技术的未确定性，故采用pimpl模式
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   跨进程队列的处理
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.15
*******************************************************************************/

#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

#include <boost/interprocess/ipc/message_queue.hpp>
#include <memory>

 
class CQueue
{
public:
	CQueue();
	explicit CQueue(int nID) :m_nQueueID(nID), m_nOpenCount(0), m_nSentMailID(0)
	{
		m_bIsOpened = false;
	}
	~CQueue();
public:
	bool Create(const char *pszName,size_t nNum,size_t nSize);
	void Destroy();

	bool Open(const char *pszName);
	void Close( );

	bool Send(const unsigned char *pData, size_t nSize, unsigned int nPriority);
	bool TrySend(const unsigned char *pData, size_t nSize, unsigned int nPriority);
	bool Send(const unsigned char *pData, size_t nSize, unsigned int nPriority,unsigned int nTimeOut);
	
	bool Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority);
	bool TryReceive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority);
	bool Receive(unsigned char * pBuff, size_t nBuffSize, size_t & nRecvdSize, unsigned int & nPriority, unsigned int nTimeOut);

	std::string &GetName()
	{
		return m_szName;
	}

	int GetQueueID()const
	{
		return m_nQueueID;
	}
	unsigned int GetMailID()
	{
		m_nSentMailID++;
		return m_nSentMailID;
	}
	bool IsOpened()
	{
		return m_bIsOpened;
	}
private:
	//! 是否打开过
	bool m_bIsOpened;
	std::shared_ptr<boost::interprocess::message_queue> m_pQue;
	std::string m_szName;
	size_t m_nOpenCount;
	int m_nQueueID; //! 队列的标识
	unsigned int m_nSentMailID; //! 邮件的标识
};
 


#endif // QUEUE_IMPL_H


/** @}*/
