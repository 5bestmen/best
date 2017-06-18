
#ifndef _NETBUS_HEARTBEAT_H_
#define _NETBUS_HEARTBEAT_H_

#include "thread_manager.h"

class CHeartbeat : public CWorkBase
{
public:
    virtual int do_work()
    {
        return 1;
    }
};

#endif