
#ifndef _IHOST_H
#define _IHOST_H
#include "datatypes.h"
#include "fesapi/fescnst.h"
//#include "channel.h"
#include <string>
#include <memory>
class C104Channel;

class IHost
{
public:
	IHost();
	virtual ~IHost();
public:
	virtual bool SetTimer(int nTimerID, int nSecond) { return true; }
	virtual void RemoveTimer(int nTimerID) {}
	virtual bool SendMessage(char* pBuff, int nLen, int nConnectID = 0) { return true; }

	virtual int  GetGroupID() { return 0; }
	virtual int  GetChannelID() { return 0; }
	virtual int  GetDeviceID() { return 0; }

	virtual int  getDeviceAddr() { return 0; }
	virtual int  GetParamID() { return 0; }
	virtual int32u GetChannelOccNo() { return INVALID_OCCNO; }
	virtual std::shared_ptr<C104Channel> getKernel() { return nullptr; }
public:
	virtual void LogCallback104(int nSendOrRecv, int nMsgType, std::string strMsg) {}
};


#endif//_IHOST_H
