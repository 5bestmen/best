

#include "channel.h"

#include "fesapi/fesapi.h"
#include "log/log.h"

CIoChannel::CIoChannel()
{
	m_nOccNo = 0;
}

CIoChannel::~CIoChannel()
{
	m_arrDevices.clear();
}


void CIoChannel::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("simu_iodrv", szLogTxt, nLevel,nullptr);
}
