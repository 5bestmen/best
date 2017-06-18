

#include "fesapi/iodriver_main.h"
#include "fesapi/fesapi.h"
#include "log/log.h"

#include "modbus_task.h"
#include "dll_lib.h"

#include <QObject>


static CIoDriverLib s_IoDrvLib;

extern "C"
{
	/*! \fn IODRV int StartIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	********************************************************************************************************* 
	** \brief StartIoDriver 
	** \details 启动驱动
	** \param pszName 共享内存名字，通常就是proj的全路径名称
	** \param nMyNodeOccNo 
	** \param nChannelOccNo 
	** \return IODRV int 
	** \author LiJin 
	** \date 2016年10月8日 
	** \note 
	********************************************************************************************************/
	IODRV int StartIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	{
		Q_ASSERT(nChannelOccNo > 0 && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == 0 || nChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			return 0;
		}

		Q_ASSERT(pszName && strlen(pszName) > 0);
		if (pszName || strlen(pszName) == 0)
		{
			return 0;
		}
 
		// 连接前置引擎
		bool bRet = OpenIO(pszName, nChannelOccNo);
		if (bRet == false)
		{
			// TODO : LOG
			LogMsg("modbus_tcp",QObject::tr("Open fes kernel failed...").toStdString().c_str() ,2);
			return 0;
		}
		// LOG 成功
		LogMsg("modbus_tcp", QObject::tr("Open fes kernel successed...").toStdString().c_str(), 0);

		s_IoDrvLib.RegisterChannel(nChannelOccNo);
 
		if (s_IoDrvLib.Start(nChannelOccNo))
		{
			return 1;
		}

		return 0;
	}


	IODRV int StopIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	{
		
		Q_ASSERT(nChannelOccNo > 0 && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == 0 || nChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			return 0;
		}

// 		Q_ASSERT(pszName && strlen(pszName) > 0);
// 		if (pszName || strlen(pszName) == 0)
// 		{
// 			return 0;
// 		}

		if (s_IoDrvLib.Stop(nChannelOccNo))
		{
			return 0;
		}
		// 关闭与前置引擎的连接
		bool bRet = CloseIO(nChannelOccNo);
		if (bRet == false)
		{
			// TODO : LOG
			LogMsg("modbus_tcp", QObject::tr("Close fes kernel failed...").toStdString().c_str(), 2);
			return 0;
		}
		
		return 1;
	}
}
 