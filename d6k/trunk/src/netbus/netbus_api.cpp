
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
    // 初始化
    g_netService.Init(pConf, pFunc, nNodeType, nStartMode);

    // 启动服务
    g_netService.StartService();

    // 检查net config
    return true;
}