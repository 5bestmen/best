
#include "net_service.h"

CNetService::CNetService()
{
    m_bDouble = false;
    m_nCurNodeIndex = m_nSlaveNodeIndex = -1;

    m_pServer = NULL;
}

CNetService::~CNetService()
{
    if (m_pServer != NULL)
    {
        delete m_pServer;
        m_pServer = NULL;
    }
}

void CNetService::Init(NET_CONFIG* pConf, void(*NodePrompt)(char* str, int32u nLenvel), const int nNodeType, const int nStartMode)
{
    m_pNetConf = pConf;
    NodeStateCallBack = NodePrompt;
    m_nNodeType = nNodeType;
    m_nStartMode = nStartMode;
}

bool CNetService::StartService()
{
    // 节点信息校验
    if (!CheckNodeConf())
    {
        return false;
    }

    // 当前启动模式进行判断
    // client
    // server
    // client、server
    switch (m_nStartMode)
    {
//     case SERVER:
//         break;
//     case CLIENT:
//         break;
//     case SERVER_CLIENT:
//         break;
    default:
        return false;
    }

    // 心跳任务


    return true;
}

bool CNetService::CheckNodeConf()
{
    if (!GetAllLocalIpAddr())
    {
        return false;
    }

	if (m_pNetConf->NodeCount <= 0)
    {
        return false;
    }

    bool bRes = false;
    int32u i = 0;
	for (; i < m_pNetConf->NodeCount; ++i)
    {
        unsigned long ulAddrA = inet_addr(m_pNetConf->pNodeConfig[i].chIP[0]);
        unsigned long ulAddrB = inet_addr(m_pNetConf->pNodeConfig[i].chIP[1]);
        if ((m_pNetConf->pNodeConfig[i].NodeType == m_nNodeType) && (IsLocalIP(ulAddrA) || IsLocalIP(ulAddrB)))
        {
            m_nCurNodeIndex = i;
            memcpy(m_chIP[0], (u_char*)(&ulAddrA), 4);
            memcpy(m_chIP[1], (u_char*)(&ulAddrB), 4);
            bRes = true;
            break;
        }
    }

    if (!bRes)
    {
        // log
        NodeStateCallBack("节点与网络节点表定义不匹配", NULL);
        return false;
    }

	for (i = 0; i < m_pNetConf->NodeCount; ++i)
    {
        if ((m_pNetConf->pNodeConfig[i].NodeType == m_nNodeType) && (i != m_nCurNodeIndex)
            && (_stricmp(m_pNetConf->pNodeConfig[i].chNickName, m_pNetConf->pNodeConfig[m_nCurNodeIndex].chNickName) == 0))
        {
            m_bDouble = true;
            m_nSlaveNodeIndex = i;

            break;
        }
    }

    return true;
}

bool CNetService::GetAllLocalIpAddr()
{
    ACE_INET_Addr* pAddrArray;
    size_t count = 0;
    if (ACE::get_ip_interfaces (count , pAddrArray) != 0)
    {
        return false;
    }

    if (count <= 0)
    {
        return false;
    }

    for (size_t i = 0; i < count; i++)
    {
        // IPv4
        if (pAddrArray[i].get_type () != PF_INET || pAddrArray[i].is_loopback ())
        {
            continue;
        }

        char addr[256] = {0};

        pAddrArray[i].get_host_addr(addr, sizeof(addr));

        unsigned long lAddr = ACE_OS::inet_addr(addr);

        m_setIP.insert(lAddr);
    }

    return true;
}