
#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/OS.h"

#include "netbus/nbdef.h"

class CMsgQueue
{
public:

    static CMsgQueue& GetInstance()
    {
        static CMsgQueue instance;
        return instance;
    }
	
	int EnqueuMsgToNormalMQ(const void* pData, size_t len)
    {
		ACE_ASSERT(pData && len <= MAX_EMSG_L);

		if (pData == nullptr || len > MAX_EMSG_L)
        {
            return 0;
        }

        // 构造消息块
        ACE_Message_Block *mb = new ACE_Message_Block(len + 1);

        // 将数据拷贝进消息块
        mb->copy((char*)pData, len);

		mb->rd_ptr()[len] = '\0';

        ACE_Time_Value nowait(ACE_OS::gettimeofday());

        // 向 ACE_Message_Queue 中添加新数据块
        if (-1 == m_mqNormal.enqueue_tail(mb, &nowait))
        {
            mb->release (); // 释放消息块
            return 0;
        }

        return 1;
    }

	int DequeuMsgFromNormalMQ(void* pData, unsigned long nDataLen, ACE_Time_Value* timeout = 0)
    {
		ACE_ASSERT(pData);

        ACE_Message_Block *mb = NULL;

        // 从 ACE_Message_Queue 中取出消息块
		if (m_mqNormal.dequeue_head(mb, timeout) != -1)
        {   
			size_t length = mb->length();
			if (length > nDataLen)
			{
				length = nDataLen;
			}

            // 从消息块中读数据
			ACE_OS::memcpy((char*)pData, mb->base(), length);
            mb->release();

			return static_cast<int> (length);
        }

        return 0;
    }

	int EnqueuMsgToUrgentMQ(const void* pData, size_t len, const int priority = 100)
    {
		ACE_ASSERT(pData && len <= MAX_EMSG_L);

		if (pData == nullptr || len > MAX_EMSG_L)
        {
            return 0;
        }

        // 构造消息块
        ACE_Message_Block *mb = new ACE_Message_Block(len + 1);

        // 将数据拷贝进消息块
        mb->copy((char*)pData, len);

		mb->rd_ptr()[len] = '\0';

		// 优先级
		mb->msg_priority(priority);

        ACE_Time_Value nowait(ACE_OS::gettimeofday());

        // 向 ACE_Message_Queue 中添加新数据块
        if (-1 == m_mqUrgent.enqueue_tail(mb, &nowait))
        {
            mb->release (); // 释放消息块
            return 0;
        }

        return 1;
    }

	size_t DequeuMsgFromUrgentMQ(void* pData, unsigned long nDataLen, ACE_Time_Value* timeout = 0)
    {
		ACE_ASSERT(pData && nDataLen > 0);
		if (pData == nullptr || nDataLen <= 0)
		{
			return 0;
		}

        ACE_Message_Block *mb = NULL;

        // 从 ACE_Message_Queue 中取出消息块
		if (m_mqUrgent.dequeue_head(mb, timeout) != -1)
        {   
            size_t length = mb->length();
			if (length > nDataLen)
			{
				length = nDataLen;
			}

            // 从消息块中读数据
			ACE_OS::memcpy((char*)pData, mb->base(), length);
            mb->release();

			return length;
        }

        return 0;
    }

	int SetToSwitchMQ(const void* pData, size_t len)
	{
		ACE_ASSERT(pData && len <= MAX_EMSG_L);

		if (pData == nullptr || len > MAX_EMSG_L)
		{
			return 0;
		}

		// 构造消息块
		ACE_Message_Block *mb = new ACE_Message_Block(len + 1);

		// 将数据拷贝进消息块
		mb->copy((char*)pData, len);

		mb->rd_ptr()[len] = '\0';

		ACE_Time_Value nowait(ACE_OS::gettimeofday());

		// 向 ACE_Message_Queue 中添加新数据块
		if (-1 == m_mqSwitch.enqueue_tail(mb, &nowait))
		{
			mb->release(); // 释放消息块
			return 0;
		}

		return 1;
	}

	int GetFromSwitchMQ(void* pData, unsigned long nDataLen, ACE_Time_Value* timeout = 0)
	{
		ACE_ASSERT(pData);

		ACE_Message_Block *mb = NULL;

		// 从 ACE_Message_Queue 中取出消息块
		if (m_mqSwitch.dequeue_head(mb, timeout) != -1)
		{
			size_t length = mb->length();
			if (length > nDataLen)
			{
				length = nDataLen;
			}

			// 从消息块中读数据
			ACE_OS::memcpy((char*)pData, mb->base(), length);
			mb->release();

			return static_cast<int> (length);
		}

		return 0;
	}

	ACE_Message_Queue<ACE_MT_SYNCH>& GetNormalQueue()
	{
		return m_mqNormal;
	}

	ACE_Message_Queue<ACE_MT_SYNCH>& GetUrgentQueue()
	{
		return m_mqUrgent;
	}

	ACE_Message_Queue<ACE_MT_SYNCH>& GetSwitchQueue()
	{
		return m_mqSwitch;
	}

private:
    // 普通消息
    ACE_Message_Queue<ACE_MT_SYNCH> m_mqNormal;

    // 紧急消息
    ACE_Message_Queue<ACE_MT_SYNCH> m_mqUrgent;

	// 主备切换命令
	ACE_Message_Queue<ACE_MT_SYNCH> m_mqSwitch;

protected:

    CMsgQueue()
    {
        m_mqNormal.high_water_mark(100 * 1024 * 1024);   // 高水平，存放1G的数据。
        m_mqNormal.low_water_mark(20 * 1024 * 1024);     // 低水位，存放20M的数据。

        m_mqUrgent.high_water_mark(1 * 1024 * 1024);
        m_mqUrgent.low_water_mark(1 * 1024);
    }

};



class CMsgQueueLite
{
public:

	CMsgQueueLite()
	{
		m_msgQueue.high_water_mark(100 * 1024 * 1024);   // 高水平，存放100M的数据。
		m_msgQueue.low_water_mark(20 * 1024 * 1024);     // 低水位，存放20M的数据。
	}

	~CMsgQueueLite()
	{
		m_msgQueue.deactivate();
	}

	int EnqueuMsgToMQ(const void* pData, size_t len, const int priority = GENERAL_MSG)
	{
		ACE_ASSERT(pData && len <= MAX_EMSG_L);

		if (pData == nullptr || m_msgQueue.is_full() || len > MAX_EMSG_L)
		{
			return 0;
		}

		// 构造消息块
		ACE_Message_Block *mb = new ACE_Message_Block(len + 1);

		// 将数据拷贝进消息块
		mb->copy((char*)pData, len);

		mb->rd_ptr()[len] = '\0';

		mb->msg_priority(priority);

		ACE_Time_Value nowait(ACE_OS::gettimeofday());

		// 向 ACE_Message_Queue 中添加新数据块
		if (-1 == m_msgQueue.enqueue_tail(mb, &nowait))
		{
			mb->release(); // 释放消息块
			return 0;
		}

		return 1;
	}

	size_t DequeuMsgFromMQ(void* pData, size_t nDataLen, ACE_Time_Value* timeout = 0)
	{
		ACE_ASSERT(pData);

		if (m_msgQueue.is_empty())
		{
			return 0;
		}

		ACE_Message_Block *mb = NULL;

		// 从 ACE_Message_Queue 中取出消息块
		if (m_msgQueue.dequeue_head(mb, timeout) != -1)
		{
			size_t length = mb->length();
			if (length > nDataLen)
			{
				length = nDataLen;
			}

			// 从消息块中读数据
			ACE_OS::memcpy((char*)pData, mb->base(), length);
			mb->release();

			return length;
		}

		return 0;
	}

	int DequeuMsgFromMQ(ACE_Message_Block *&mb, ACE_Time_Value* timeout = 0)
	{
		/*ACE_Message_Block *mb1 = nullptr;
		if (m_msgQueue.is_empty())
		{
			return -1;
		}*/

		// 从 ACE_Message_Queue 中取出消息块
		if (m_msgQueue.dequeue_head(mb, timeout) != -1)
		{
			//size_t length = mb->length();

			return 0;
		}

		return -1;
	}

	int SetToSwitchMQ(const void* pData, size_t len)
	{
		ACE_ASSERT(pData && len <= MAX_EMSG_L);

		if (pData == nullptr || len > MAX_EMSG_L)
		{
			return 0;
		}

		// 构造消息块
		ACE_Message_Block *mb = new ACE_Message_Block(len + 1);

		// 将数据拷贝进消息块
		mb->copy((char*)pData, len);

		mb->rd_ptr()[len] = '\0';

		ACE_Time_Value nowait(ACE_OS::gettimeofday());

		// 向 ACE_Message_Queue 中添加新数据块
		if (-1 == m_mqSwitch.enqueue_tail(mb, &nowait))
		{
			mb->release(); // 释放消息块
			return 0;
		}

		return 1;
	}

	int GetFromSwitchMQ(void* pData, unsigned long nDataLen, ACE_Time_Value* timeout = 0)
	{
		ACE_ASSERT(pData);

		ACE_Message_Block *mb = NULL;

		// 从 ACE_Message_Queue 中取出消息块
		if (m_mqSwitch.dequeue_head(mb, timeout) != -1)
		{
			size_t length = mb->length();
			if (length > nDataLen)
			{
				length = nDataLen;
			}

			// 从消息块中读数据
			ACE_OS::memcpy((char*)pData, mb->base(), length);
			mb->release();

			return static_cast<int> (length);
		}

		return 0;
	}

	ACE_Message_Queue<ACE_MT_SYNCH>& GetMsgQueue()
	{
		return m_msgQueue;
	}

	ACE_Message_Queue<ACE_MT_SYNCH>& GetSwitchQueue()
	{
		return m_mqSwitch;
	}

private:
	// 消息队列
	ACE_Message_Queue<ACE_MT_SYNCH> m_msgQueue;

	// 主备切换命令
	ACE_Message_Queue<ACE_MT_SYNCH> m_mqSwitch;
	
};


#endif
