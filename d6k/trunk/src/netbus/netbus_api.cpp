
#include <netbus/nbdef.h>

#define NBAPI_LIB
#include <netbus/nbapi.h>

#include "task_timer.h"
#include "node_manager.h"

#include "net_service.h"

#include "thread_manager.h"

#include "heartbeat.h"

CNetService& g_netService = CNetService::GetInstance();

NBAPI_EXPORT bool StartNetBus(NET_CONFIG * pConf, LogMsgCallBack pFunc, int32u nNodeType, int32u nStartMode)
{
    // ��ʼ��
    g_netService.Init(pConf, pFunc, nNodeType, nStartMode);

    // ��������
    g_netService.StartService();

    // ���net config
    return true;
}