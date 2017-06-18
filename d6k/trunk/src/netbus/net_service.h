
#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#define NBAPI_LIB
#include <netbus/nbapi.h>

#include <ace/OS.h>
#include <ace/INET_Addr.h>
#include <ace/Unbounded_Set.h>
#include <ace/OS_Memory.h>

#include "acceptor.h"

class CNetService
{
public:

    static CNetService& GetInstance()
    {
        static CNetService instance;
        return instance;
    }

    // �������
    void Init(NET_CONFIG* pConf, void (*NodePrompt)(char* str, int32u nLenvel), const int nNodeType, const int nStartMode);

    // ��������
    bool StartService();

    // �ڵ�У��
    bool CheckNodeConf();

    // ��ȡ����IP
    bool GetAllLocalIpAddr();

    // �ж��Ƿ��Ǳ���IP
    bool IsLocalIP(unsigned long nAddr)
    {
        return (m_setIP.find(nAddr) == 0);
    }

public:
    void (*NodeStateCallBack)(char* str, int32u nLevel);    // �ص�����������д��־��.

protected:
private:

    bool m_bDouble;             // �Ƿ�˫�� (false��������true��˫��)
    int m_nNodeType;            // ��ǰ�ڵ�����
    int m_nCurNodeIndex;        // ��ǰ�ڵ��±�
    int m_nSlaveNodeIndex;      // �����ڵ��±�

    int m_nStartMode;           // ����ģʽ

    NET_CONFIG* m_pNetConf;

    unsigned char m_chIP[2][4];                  // IP
    ACE_Unbounded_Set<unsigned long> m_setIP;    // ��ű���IP

private:

    // Reactor single
    //typedef ACE_Singleton<ACE_Reactor, ACE_Null_Mutex> Reactor;

    int CreateServer()
    {
        //ACE_NEW_RETURN(m_pServer, CThreadCreate(CServerThread), -1);
        m_pServer = new CServerThread();

        CThreadCreate thread(m_pServer);
        thread.ThreadCreate();

        ACE_Reactor::instance()->run_reactor_event_loop();
        return 1;
    }

    CServerThread* m_pServer;
    
    //typedef ACE_Acceptor<MyServiceHandler,ACE_SOCK_ACCEPTOR> Acceptor;

    // ����������
    //typedef ACE_Connector<MyServiceHandler,ACE_SOCK_CONNECTOR> Connector;

private:

    CNetService();

    ~CNetService();

};

#endif