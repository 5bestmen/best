
#include "ace/OS_NS_errno.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/Time_Value.h"

#include "tcpclient.h"


CTcpClient::CTcpClient()
{
}

CTcpClient::~CTcpClient(void)
{

}

bool CTcpClient::Open(char* addr, int port)
{
	ACE_INET_Addr peeraddr(port, addr);
	ACE_Time_Value timeout(10);

	if (m_connector.connect(m_peer, peeraddr, &timeout) == -1)
	{
		if (ACE_OS::last_error() == ETIME)
		{
			ACE_DEBUG((LM_DEBUG,
				ACE_TEXT("(%P|%t) Timeout while ")
				ACE_TEXT("connecting to log server\n")));
		}
		else
		{
			ACE_ERROR((LM_ERROR,
				ACE_TEXT("%p\n"),
				ACE_TEXT("log")));
		}
		return false;
	}

	return true;
}

void CTcpClient::Close()
{
	m_peer.close();

}

bool CTcpClient::SendMsg(char* pBuff, int nSize)
{
	ACE_Time_Value sendTimeout(0, 5);
	if (m_peer.send_n(pBuff, nSize, &sendTimeout) == -1)
	{
		if (ACE_OS::last_error() == ETIME)
		{
			ACE_DEBUG((LM_DEBUG,
				ACE_TEXT("(%P|%t) Timeout while sending ")
				ACE_TEXT("query to status server\n")));
		}
		else
		{
			ACE_ERROR((LM_ERROR,
				ACE_TEXT("%p\n"),
				ACE_TEXT("send_n")));
		}
		return false;
	}

	return true;
}

int CTcpClient::RecvMsg(char* pBuff, int nSize)
{
	ssize_t bc;
	ACE_Time_Value recvTimeout(2, 100);
	if ((bc = m_peer.recv(pBuff, nSize, &recvTimeout)) == -1)
	{
		ACE_ERROR((LM_ERROR,
			ACE_TEXT("%p\n"),
			ACE_TEXT("recv")));
		return 0;
	}
	return bc;
}

