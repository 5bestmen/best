
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

    // 保存入参
    void Init(NET_CONFIG* pConf, void (*NodePrompt)(char* str, int32u nLenvel), const int nNodeType, const int nStartMode);

    // 启动服务
    bool StartService();

    // 节点校验
    bool CheckNodeConf();

    // 获取本地IP
    bool GetAllLocalIpAddr();

    // 判断是否是本地IP
    bool IsLocalIP(unsigned long nAddr)
    {
        return (m_setIP.find(nAddr) == 0);
    }

public:
    void (*NodeStateCallBack)(char* str, int32u nLevel);    // 回调函数，用于写日志等.

protected:
private:

    bool m_bDouble;             // 是否双网 (false：单网，true：双网)
    int m_nNodeType;            // 当前节点类型
    int m_nCurNodeIndex;        // 当前节点下标
    int m_nSlaveNodeIndex;      // 备机节点下标

    int m_nStartMode;           // 启动模式

    NET_CONFIG* m_pNetConf;

    unsigned char m_chIP[2][4];                  // IP
    ACE_Unbounded_Set<unsigned long> m_setIP;    // 存放本地IP

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

    // 创建连接器
    //typedef ACE_Connector<MyServiceHandler,ACE_SOCK_CONNECTOR> Connector;

private:

    CNetService();

    ~CNetService();

};

#endif