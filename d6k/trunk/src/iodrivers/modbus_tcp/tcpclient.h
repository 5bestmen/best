
//ACE TCP¿Í»§¶Ë(ÒÔÌ«Íø)

#ifndef _TCP_CLIENT_H
#define _TCP_CLIENT_H


#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"


class CTcpClient
{
public:
	CTcpClient(void);
	~CTcpClient(void);
public:
	virtual bool Open(char* addr, int port);
	virtual void Close();

public:
	virtual bool SendMsg(char* pBuff, int nSize);
	virtual int  RecvMsg(char* pBuff, int nSize);

private:
	ACE_SOCK_Connector m_connector;
	ACE_SOCK_Stream m_peer;
};


#endif //_TCP_CLIENT_H

