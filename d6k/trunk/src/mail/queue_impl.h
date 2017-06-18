/*! @file queue_impl.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  queue_impl.h
�ļ�ʵ�ֹ��� :  ����̶��еĴ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  ��Ϊ���ǵ���ƽ̨�ļ�����δȷ���ԣ��ʲ���pimplģʽ
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����̶��еĴ���
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
	//! �Ƿ�򿪹�
	bool m_bIsOpened;
	std::shared_ptr<boost::interprocess::message_queue> m_pQue;
	std::string m_szName;
	size_t m_nOpenCount;
	int m_nQueueID; //! ���еı�ʶ
	unsigned int m_nSentMailID; //! �ʼ��ı�ʶ
};
 


#endif // QUEUE_IMPL_H


/** @}*/
