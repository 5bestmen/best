
#include "handler.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/CDR_Base.h"
#include "ace/CDR_Stream.h"



#define MAX_MSG_SOCKETBUFF    1024*1024    //SOCKET数据缓存大小

int CHandler::open(void *p)
{
	if (m_pPeer == NULL)
	{
		return -1; //在开启连接之前，必须设置该值
	}

	if (super::open(p) == -1)
		return -1;

	if (this->peer().get_remote_addr(m_addrRemote) == -1)
		return -1;

	//设为非阻塞
	if (this->peer().enable(ACE_NONBLOCK) == -1)
		return -1;

	//设置缓冲区
	int nBuffSize = MAX_MSG_SOCKETBUFF;
	ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_RCVBUF, (char*)&nBuffSize, sizeof(nBuffSize));
	ACE_OS::setsockopt(this->get_handle(), SOL_SOCKET, SO_SNDBUF, (char*)&nBuffSize, sizeof(nBuffSize));

	//         m_pPeer->AddConnect(this);
	// 
	//         m_pPeer->OnConnectOpen(GetConnectID(),m_addrRemote);

	return 0;
}


int CHandler::handle_input(ACE_HANDLE handle)
{
	int nLen = peer().recv(m_buff_recv, MAX_HANDLER_BUFF_SIZE);

	if (nLen > 0)
	{
		//     m_pPeer->RecvMsg(GetConnectID(),m_buff_recv,nLen);
	}
	else if (nLen == 0) // Socket was closed by server
	{
		ACE_ERROR_RETURN((LM_ERROR,
			ACE_TEXT("%t: client h %d: closed by server\n"),
			handle),
			-1);
	}
	else if (errno == EWOULDBLOCK) // no data ready on socket
	{
		ACE_ERROR_RETURN((LM_DEBUG,
			ACE_TEXT("%t: client h %d: recv no data\n"),
			handle),
			0);
	}
	else
	{
		ACE_ERROR_RETURN((LM_ERROR,
			ACE_TEXT("%t: client h %d: %p\n"),
			handle,
			ACE_TEXT("send")),
			-1);
	}

	return 0;
}

bool CHandler::SendMessage(char* pBuff, int nMsgLen)
{
	if (pBuff == NULL || nMsgLen <= 0)
		return false;

	ssize_t send_cnt = this->peer().send(pBuff, nMsgLen);
	if (send_cnt < nMsgLen)
		return false;

	return true;
}


int CHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask)
{
	/*
	//m_connectedclients.clr_bit (handle);
	m_pPeer->RemoveConnect(GetConnectID());

	m_pPeer->OnConnectClose(m_addrRemote);

	return ACE_OS::closesocket (handle);
	*/
	//         m_pPeer->OnConnectClose(GetConnectID(),m_addrRemote);
	// 
	//         m_pPeer->RemoveConnect(GetConnectID());


	return 0;
}