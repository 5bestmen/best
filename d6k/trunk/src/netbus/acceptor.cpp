
#include "acceptor.h"

#include <iostream>

#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"
#include "ace/SOCK_SEQPACK_Association.h"

#include "msg_queue.h"

#include "fesapi/fesdatadef.h"

CMsgQueue& g_msgQ = CMsgQueue::GetInstance();

CAcceptorTask::CAcceptorTask(ACE_INET_Addr& addr) : m_addrAcceptor(addr), m_pProactor(nullptr), m_lock(), m_sem((unsigned int)0)
{
	m_nStopState = false;
}

CAcceptorTask::~CAcceptorTask()
{
	if (!m_nStopState)
	{
		Stop();
	}

	DeleteProactor();
}
int CAcceptorTask::Start(int numThreads/* = 1*/)
{
	ACE_ASSERT(numThreads > 0);
	if (numThreads > 4)
	{
		numThreads = 4;
	}

	if (CreateProactor() == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("Unable to create proactor")), -1);
	}

	if (this->activate(THR_NEW_LWP, numThreads) == -1)
	{
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("unable to activate thread pool")), -1);
	}

	for (; numThreads > 0; numThreads--)
	{
		m_sem.acquire();
	}

	return 0;
}

int CAcceptorTask::Stop(void)
{
	if (m_nStopState)
	{
		return true;
	}

	if (m_pProactor != nullptr)
	{
		ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Calling End Proactor event loop\n")));

		ACE_Proactor::instance()->proactor_end_event_loop();
		//m_pProactor->proactor_end_event_loop();
	}

	if (this->wait() == -1)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p.\n"), ACE_TEXT("unable to stop thread pool")));
	}

	m_nStopState = true;

	return 0;
}

int CAcceptorTask::close(u_long flags /*= 0*/)
{
	ACE_UNUSED_ARG(flags);

	if (ACE_OS::thr_equal(ACE_Thread::self(), this->last_thread()))
	{
		// 释放对象
		//delete this;
	}

	return 0;
}

int CAcceptorTask::svc()
{
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) CAcceptorTask started.\n")));

	DisableSignal(ACE_SIGRTMIN, ACE_SIGRTMAX);
	DisableSignal(SIGPIPE, SIGPIPE);	

	// signal that we are ready  
	m_sem.release(1);
	
	m_acceptor.open(m_addrAcceptor);
	// Ace proactor多路分离。

	//ACE_Proactor::run_event_loop();
	ACE_Proactor::instance()->proactor_run_event_loop();
	//m_pProactor->proactor_run_event_loop();

	//ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) CAcceptorTask finished.\n")));

	return 0;
}

// 创建proactor模式
int CAcceptorTask::CreateProactor(void)
{
	ACE_GUARD_RETURN(ACE_SYNCH_RECURSIVE_MUTEX, monitor, m_lock, -1);

	ACE_ASSERT(m_pProactor == nullptr);

	ACE_NEW_RETURN(m_pProactor, ACE_Proactor(), -1);

	// Set new singleton and delete it in close_singleton()
	// 思考：添加到单例中会不会对其他proactor服务产生影响.
	ACE_Proactor::instance(m_pProactor, 1);

	return 0;
}

// 删除proactor模式
int CAcceptorTask::DeleteProactor(void)
{
	ACE_GUARD_RETURN(ACE_SYNCH_RECURSIVE_MUTEX, monitor, m_lock, -1);

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) Delete Proactor\n")));

	// 底层自动释放m_pReactor指针
	if (m_pProactor != nullptr)
	{
		ACE_Proactor::instance()->close_singleton();
	}	

	return 0;
}

CProactiveService::CProactiveService() : m_handle(ACE_INVALID_HANDLE)
{
	m_cnt = 0;
	m_size = 0;

	m_strRemainFlag.clear();
}

CProactiveService::CProactiveService(int nID) : m_nServerID(nID) , m_handle(ACE_INVALID_HANDLE)
{
	m_cnt = 0;
	m_size = 0;

	m_strRemainFlag.clear();
}

CProactiveService::~CProactiveService()
{
	if (this->handle() != ACE_INVALID_HANDLE)
	{
		ACE_OS::closesocket(this->handle());

		ACE_DEBUG((LM_WARNING, ACE_TEXT("(%t) 客户(%s)断开连接.\n"), m_addr.get_host_addr()));

		UnbindNodeInfo(m_addr.get_host_addr());
	}
}

void CProactiveService::open(ACE_HANDLE h, ACE_Message_Block& message_block)
{
	this->handle(h);

	ACE_SOCK_SEQPACK_Association ass = ACE_SOCK_SEQPACK_Association(h);

	size_t addrSize = 1;
	ass.get_remote_addrs(&m_addr, addrSize);

	ACE_DEBUG((LM_WARNING, ACE_TEXT("(%t)有客户端连接(%s).\n"), m_addr.get_host_addr()));

	// 构造读流 写流
	if (this->m_asynchReader.open(*this) != 0 || this->m_asynchWriter.open(*this) != 0)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),ACE_TEXT("HA_Proactive_Service open")));
		delete this;
		return;
	}

	//如果客户连接时同时提交了数据，需要伪造一个结果，然后呼叫读事件
	if (message_block.length() != 0)
	{
		// 复制消息块
		ACE_Message_Block &duplicate = *message_block.duplicate();
		// 伪造读结果，以便进行读完成回调
		ACE_Asynch_Read_Stream_Result_Impl *fake_result =
			ACE_Proactor::instance()->create_asynch_read_stream_result(this->proxy(),
			m_handle,
			duplicate,
			ACE_DEFAULT_MAX_SOCKET_BUFSIZ,
			0,
			ACE_INVALID_HANDLE,
			0,
			0);

		size_t bytes_transferred = message_block.length();

		// Accept事件处理完成，wr_ptr指针会被向前移动，将其移动到开始位置
		duplicate.wr_ptr(duplicate.wr_ptr() - bytes_transferred);
		// 这个方法将调用回调函数
		fake_result->complete(message_block.length(), 1, 0);
		// 销毁伪造的读结果
		delete fake_result;
	}

	ACE_Message_Block *mb = 0;
	ACE_NEW_NORETURN(mb, ACE_Message_Block(ACE_DEFAULT_MAX_SOCKET_BUFSIZ));
	if (this->m_asynchReader.read(*mb, mb->space()) != 0)
	{
		ACE_ERROR((LM_ERROR, ACE_TEXT("%p\n"),	ACE_TEXT("HA_Proactive_Service begin read")));
		mb->release();
		delete this;
		return;
	}

	// 保存节点信息
	BindNodeInfo(m_addr.get_host_addr());

	// mb is now controlled by Proactor framework.
	return;
}

// 当用户要读的数据读好了后，调用这个方法
void  CProactiveService::handle_read_stream(const ACE_Asynch_Read_Stream::Result &result)
{
	ACE_GUARD(ACE_SYNCH_MUTEX, monitor, m_synchMutex);

	ACE_Message_Block &mb = result.message_block();
	if (!result.success() || result.bytes_transferred() == 0)
	{// 读取失败？？
		mb.release();
		delete this;

		return;
	}

	// Reset pointers.
	size_t size = result.bytes_transferred();
	mb.rd_ptr()[size] = '\0';

	// 统计接收到的字节数
	//m_size += size;
	//std::cout << m_size << std::endl;
	//ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) CProactiveService %d: read %B bytes\n"), m_nServerID, result.bytes_transferred()));

	HandleMsg(mb.rd_ptr(), size);
	/*char bufData[4096] = { 0 };
	//ACE_OS::memcpy(bufData, mb.base() + 8, size - 8);

	ACE_InputCDR cdr(&mb);
	ACE_CDR::ULong length;
	cdr >> length;
	cdr.reset_byte_order(length);
	cdr >> length;

	cdr >> *bufData;

	char* data = mb.rd_ptr() + 8;*/
	/*ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("(%t) **** Server %d: handle_read_stream() ****\n"),
		m_nServerID));
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("%s = %B\n"), ACE_TEXT("bytes_to_read"), result.bytes_to_read()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %d\n"),
		ACE_TEXT("handle"),
		result.handle()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %B\n"),
		ACE_TEXT("bytes_transfered"),
		result.bytes_transferred()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %@\n"),
		ACE_TEXT("act"),
		result.act()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %d\n"),
		ACE_TEXT("success"),
		result.success()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %@\n"),
		ACE_TEXT("completion_key"),
		result.completion_key()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %d\n"),
		ACE_TEXT("error"),
		result.error()));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("%s = %s\n"),
		ACE_TEXT("message_block"),
		mb.rd_ptr() + 8));
	ACE_DEBUG((LM_DEBUG,
		ACE_TEXT("**** end of message ****************\n")));*/

	// 处理接收的数据
	//  todo:

	// 处理完释放内存
	mb.release();

	// 继续读
	Initiate_Read_Stream();
}


int CProactiveService::HandleMsg(ACE_Message_Block* mb)
{
	/*ACE_ASSERT(mb);
	if (mb == nullptr)
	{
		return -1;
	}

	char* data = mb->rd_ptr();
	size_t len = mb->length();

	EMSG_BUF *pBuf = (EMSG_BUF*)(data);

	char* temp = new char[pBuf->MsgDataSize + 1];
	memset(temp, 0, pBuf->MsgDataSize + 1);

	ACE_OS::memcpy(temp, pBuf->buffData, pBuf->MsgDataSize);*/

	return 0;
}

bool CProactiveService::HandleMsg(char* pData, size_t nLen)
{
	//ACE_ASSERT(pData && nLen > 0);

	if (pData == nullptr || nLen <= 0)
	{		
		return false;
	}

	if (!m_msgCache.IsEmpty())
	{ // 存在冗余数据

		// 存在冗余数据时，冗余数据的长度和再次接收的长度是否超过包头
		if ((nLen + m_msgCache.GetCurByteLength()) < EMSG_BUF_HEAD_SIZE + 1)
		{ // 没有超过包头长度，保存继续接收

			// 此时接收到的包小于整个包头，保存数据继续接收。
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else
		{ // 超过包头长度

			if (m_msgCache.GetCurByteLength() < EMSG_BUF_HEAD_SIZE + 1)
			{
				// 复制包头
				// 包头冗余值
				size_t nHeadSize = (EMSG_BUF_HEAD_SIZE + 1) - m_msgCache.GetCurByteLength();
				m_msgCache.CopyData(pData, nHeadSize);

				pData += nHeadSize;
				nLen -= nHeadSize;
			}			
		}

		size_t nNeedBytes = m_msgCache.GetNeedByteLength();
		if (nNeedBytes <= 0 || nNeedBytes > MAX_EMSG_L)
		{
			m_msgCache.Clear();
			return false;
		}

		if (nNeedBytes > nLen)//需要的长度大于接收到的长度
		{
			m_msgCache.CopyData(pData, nLen);
			return true;
		}
		else//接收到的长度大于需要的长度
		{
			//复制需要的部分
			m_msgCache.CopyData(pData, nNeedBytes);
			pData += nNeedBytes;
			nLen -= nNeedBytes;

			m_cnt++;
			if (m_cnt % 10000 == 0)
			{
				std::cout << "接收num:" << m_cnt << std::endl;
			}

			char* data = m_msgCache.GetDataBuff();
			size_t len = m_msgCache.GetDataLen();

			EMSG_BUF* msg = (EMSG_BUF*)data;
			
			int nMsgType = static_cast<int>(msg->MsgType);
			
			// 把 m_msgCache.GetDataBuff() 这个完整的包放到消息队列中。
			switch (nMsgType)
			{
			case NBMT_MSG_SYNCH:
			{
				if (0 == g_msgQ.EnqueuMsgToNormalMQ(data, len))
				{
					// log
				}

				// 回响应
				if (0)
				{
					ACE_Message_Block *mb = nullptr;
					ACE_NEW_RETURN(mb, ACE_Message_Block(2 + 1), false);
					mb->wr_ptr("OK");
					ACE_OS::memcpy(mb->base(), "OK", nLen);

					Initiate_Write_Stream(*mb, 2);
				}

				break;
			}
			case NBMT_MSG_EVENT:
			{
				if (0 == g_msgQ.EnqueuMsgToUrgentMQ(data, len, NBMT_MSG_EVENT))
				{
					// log
				}

				// 回响应
				if (0)
				{
					ACE_Message_Block *mb = nullptr;
					ACE_NEW_RETURN(mb, ACE_Message_Block(2 + 1), false);
					mb->wr_ptr("OK");
					ACE_OS::memcpy(mb->base(), "OK", nLen);

					Initiate_Write_Stream(*mb, 2);
				}

				break;
			}
			case NBMT_CONTROL_EVENT:
			{
				if (0 == g_msgQ.EnqueuMsgToUrgentMQ(data, len, (NBMT_CONTROL_EVENT + NBMT_MSG_EVENT + 1)))
				{
					// log					
				}

				// 回响应
				if (0)
				{
					ACE_Message_Block *mb = nullptr;
					ACE_NEW_RETURN(mb, ACE_Message_Block(2 + 1), false);
					mb->wr_ptr("OK");
					ACE_OS::memcpy(mb->base(), "OK", nLen);

					Initiate_Write_Stream(*mb, 2);
				}

				break;
			}
			case RECV_SWITCH_CMD_RES: // 接收到切换响应
			{
				// 将收到的切换响应报文放到消息队列
				g_msgQ.SetToSwitchMQ(data, len);

				break;
			}
			default:
				break;
			}
			
			m_msgCache.Clear();
			
			//解析剩余的部分
			return HandleMsg(pData, nLen);
		}
	}
	else
	{ // 没有冗余数据

		//在接收区寻找启动字符
		char* pNewBuff = std::find(pData, pData + nLen, ORIGIN_FLAG);
		if (pNewBuff == pData + nLen)
		{
			return false;//没有找到启动字符
		}

		nLen = nLen - (pNewBuff - pData);
		pData = pNewBuff;

		//复制启动字符至缓冲区
		m_msgCache.CopyData(pData, 1);
		pData += 1;
		nLen -= 1;

		return HandleMsg(pData, nLen);
	}

	return true;
}

bool CProactiveService::HandleSwitch(char* pData, size_t nLen)
{
	return true;
}

// 当用户要写的数据在网卡上发送成功后，Proactor会回调这个方法
void CProactiveService::handle_write_stream(const ACE_Asynch_Write_Stream::Result &result)
{
	ACE_GUARD(ACE_SYNCH_MUTEX, monitor, m_synchMutex);

	ACE_Message_Block & mb = result.message_block();

	ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%t) CProactiveService %d: wrote %B bytes ok\n"), m_nServerID, result.bytes_transferred()));
	

	mb.release();

	// 继续读
	Initiate_Read_Stream();
}


int CProactiveService::Initiate_Read_Stream(void)
{	
	ACE_Message_Block *mb = 0;
	ACE_NEW_RETURN(mb, ACE_Message_Block(ACE_DEFAULT_MAX_SOCKET_BUFSIZ), -1);

	// 开始读操作
	if (m_asynchReader.read(*mb, mb->size()/* - 1*/) == -1)
	{
		mb->release();
#if defined (ACE_WIN32)
		// On peer close, ReadFile will yield ERROR_NETNAME_DELETED; won't get
		// a 0-byte read as we would if underlying calls used WSARecv.
		if (ACE_OS::last_error() == ERROR_NETNAME_DELETED)
		{
			ACE_ERROR_RETURN((LM_DEBUG, ACE_TEXT("(%t) Server %d, peer closed\n"), m_nServerID), -1);
		}
#endif /* ACE_WIN32 */
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%t) Server %d, %p\n"), m_nServerID, ACE_TEXT("read")), -1);
	}

	return 0;
}

int CProactiveService::Initiate_Write_Stream(ACE_Message_Block &mb, size_t nbytes)
{
	if (this->handle_ == ACE_INVALID_HANDLE)
	{
		mb.release();
		return -1;
	}

	if (nbytes == 0)
	{
		mb.release();
		ACE_ERROR_RETURN((LM_ERROR,	ACE_TEXT("(%t) CProactiveService::ACE_Asynch_Write_Stream::write nbytes <=0 ")), -1);
	}

	// 发送
	if (m_asynchWriter.write(mb, nbytes) == -1)
	{
		mb.release();
#if defined (ACE_WIN32)
		// On peer close, WriteFile will yield ERROR_NETNAME_DELETED.
		if (ACE_OS::last_error() == ERROR_NETNAME_DELETED)
		{
			ACE_ERROR_RETURN((LM_DEBUG, ACE_TEXT("(%t) Server %d, peer gone\n"), m_nServerID), -1);
		}
#endif /* ACE_WIN32 */
		ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("(%t) Server %d, %p\n"), m_nServerID, ACE_TEXT("write")), -1);
	}

	return 0;
}

void CProactiveService::BindNodeInfo(const char* ip)
{
	ACE_ASSERT(ip);

	ACE_UINT64 addr = ACE_OS::inet_addr(ip);
	CNodeManager* pNodeManager = nullptr;
	int nRes = M_SINGLETON::instance()->m_connMap.find(addr, pNodeManager);
	if (nRes != -1)
	{
		M_SINGLETON::instance()->m_connMap.unbind(addr);
		delete pNodeManager;
		pNodeManager = nullptr;
	}

	pNodeManager = new CNodeManager(&m_asynchWriter);
	pNodeManager->SetNetUsing(true);
	pNodeManager->SetConnCnt();
	pNodeManager->SetConnectTime(ACE_OS::gettimeofday().get_msec());
	M_SINGLETON::instance()->m_connMap.bind(addr, pNodeManager);
}

void CProactiveService::UnbindNodeInfo(const char* ip)
{
	ACE_ASSERT(ip);

	ACE_UINT64 addr = ACE_OS::inet_addr(ip);
	CNodeManager* pNodeManager = nullptr;
	int nRes = M_SINGLETON::instance()->m_connMap.find(addr, pNodeManager);
	if (nRes != -1)
	{
		delete pNodeManager;
		pNodeManager = nullptr;

		M_SINGLETON::instance()->m_connMap.unbind(addr);
	}
}
