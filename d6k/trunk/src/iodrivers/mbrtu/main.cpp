
#include "fesapi\fesapi.h"

#include "modbus_task.h"

extern "C" __declspec(dllexport) int drivermain(int nParam)
{
//	int nChannelID = nParam;
//	OpenIO(nChannelID);

	if (nParam <= 0 )
	{
		return 0;
	}
#if 0
	// 连接前置引擎
	bool bRet = ConnectFes();
	if (bRet == false)
	{
		return 0;
	}

	int nChannelNo = nParam;
	bRet = OpenFes(nChannelNo);
	if (bRet == false)
	{
		CloseFes();
		return 0;
	}
#endif
	

	return 0;
}
