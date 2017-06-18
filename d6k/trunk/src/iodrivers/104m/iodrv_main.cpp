
#include "iodriver_main.h"
#include "fesapi/fesapi.h"
#include "log/log.h"
#include "dll_lib.h"
#include <QObject>

static CIoDriverLib s_IoDrvLib;

extern "C"
{
	
	/*! \fn M_EXPORT int StartIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	********************************************************************************************************* 
	** \brief StartIoDriver 
	** \details 104驱动启动函数
	** \param pszName 
	** \param nMyNodeOccNo 
	** \param nChannelOccNo 
	** \return M_EXPORT int 
	** \author xingzhibing
	** \date 2017年2月16日 
	** \note 
	********************************************************************************************************/
	IODRV  int StartIoDriver(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo)
	{
		Q_ASSERT(nChannelOccNo > 0 && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == 0 || nChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			return 0;
		}

		Q_ASSERT(pszName && strlen(pszName) > 0);
		if (!pszName || (strlen(pszName) == 0))
		{
			return 0;
		}

		// 连接前置引擎
		bool bRet = OpenIO(pszName, nChannelOccNo,0);
		if (bRet == false)
		{
			LogMsg("104_master", QObject::tr("Open fes kernel failed...").toStdString().c_str(), 2, nullptr);
			return 0;
		}
	//	bRet = OpenChannelMailBox(nChannelOccNo);
	//	Q_ASSERT(bRet);

		LogMsg("104_master", QObject::tr("Open fes kernel successed...").toStdString().c_str(), 0, nullptr);

		s_IoDrvLib.RegisterChannel(nChannelOccNo);

		if (s_IoDrvLib.Start(nChannelOccNo))
		{
			return 1;
		}

		return 0;
	}


	/*! \fn M_EXPORT int StopIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	********************************************************************************************************* 
	** \brief StopIoDriver 
	** \details  104驱动关闭函数
	** \param pszName 
	** \param nMyNodeOccNo 
	** \param nChannelOccNo 
	** \return M_EXPORT int 
	** \author xingzhibing
	** \date 2017年2月16日 
	** \note 
	********************************************************************************************************/
	IODRV int StopIoDriver(const char * pszName, int32u nMyNodeOccNo, int32u nChannelOccNo)
	{
		Q_ASSERT(nChannelOccNo > 0 && nChannelOccNo <= MAX_CHANNEL_OCCNO);
		if (nChannelOccNo == 0 || nChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			return 0;
		}

		if (s_IoDrvLib.Stop(nChannelOccNo))
		{
			return 0;
		}
		// 关闭与前置引擎的连接
		bool bRet = CloseIO(nChannelOccNo);
		if (bRet == false)
		{
			LogMsg("104_master", QObject::tr("Close fes kernel failed...").toStdString().c_str(), 2, nullptr);
			return 0;
		}

		return 1;
	}
}
