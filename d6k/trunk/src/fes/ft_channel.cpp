/*! @file ft_channel.cpp
<PRE>
********************************************************************************
模块名       :  前置的远动转发模块
文件名       :
文件实现功能 :
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2016.10.11
*/
#include "ft_channel.h"
#include "fesapi/fesdatadef.h"
#include "log/log.h"

#include <QDebug>
#include <QLibrary> 

CFTChannel::CFTChannel()
{
	m_nOccNo = 0;
	m_pData = nullptr;
	m_fnStartFtDriver = nullptr;
	m_fnStopFtDriver = nullptr;
}

CFTChannel::~CFTChannel()
{
	
}

CFTChannel::CFTChannel(FT_CHANNEL *pChannel) :m_pData(pChannel)
{
	Q_ASSERT(pChannel);
	m_nOccNo = m_pData->OccNo;

	m_fnStartFtDriver = nullptr;
	m_fnStopFtDriver = nullptr;
}

bool CFTChannel::Load()
{
	QString szExt;
#ifdef Q_OS_MAC
	// mac
#endif

#ifdef Q_OS_LINUX
	// linux
	szExt = ".so";
#endif

#ifdef Q_OS_WIN32
	// win
	szExt = ".dll";
#endif

	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	if (m_pData->DriverType == 0)
	{// 进程内
		Q_ASSERT(strlen(m_pData->DriverName));
		if (strlen(m_pData->DriverName) <= 0)
		{
			return false;
		}
		QString szDllName;
		QString szLog;
		szDllName = m_pData->DriverName + szExt;
		QLibrary dlllib(szDllName);
		if (dlllib.load())
		{
			m_fnStartFtDriver = (StartFtDriver)dlllib.resolve("StartFtDriver");
			m_fnStopFtDriver = (StopFtDriver)dlllib.resolve("StopFtDriver");
			Q_ASSERT(m_fnStartFtDriver);
			Q_ASSERT(m_fnStopFtDriver);

			if (m_fnStartFtDriver && m_fnStopFtDriver)
			{
				szLog = "Load [ " + szDllName + " ] success.";
				LogString(szLog.toStdString().c_str(), 0);
			}
			else
			{
				szLog = "Load [ " + szDllName + " ] failed, the callback function is bad.";
				LogString(szLog.toStdString().c_str(), 1);
			}
		}
		else
		{// log 打印
			szLog = "Load [ " + szDllName + " ] failed";
			LogString(szLog.toStdString().c_str(), 1);
		}
	}
	else
	{// 进程外
		// modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}


	return true;
}

bool CFTChannel::Start(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog;

	if (m_pData->DriverType == 0)
	{// 进程内 
	 
//		Q_ASSERT(m_fnStartIoDriver);	
		if (m_fnStartFtDriver )
		{
			m_fnStartFtDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
		}
		else
		{
			if (strlen(m_pData->ChannelName))
			{
				szLog = "Start [ " + QString(m_pData->ChannelName) + " ] failed.";
			}
			else
			{
				szLog = "Start [ " + QString("Channel:%1").arg(m_pData->OccNo) + " ] failed.";
			}
			
			LogString(szLog.toStdString().c_str(), 1);
		}
		 
	}
	else
	{// 进程外
		//进程启动格式： modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}


	return true;
}

bool CFTChannel::Stop(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog;

	if (m_pData->DriverType == 0)
	{// 进程内 
		Q_ASSERT(m_fnStopFtDriver);

		if ( m_fnStopFtDriver)
		{
			m_fnStopFtDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
		}
		else
		{
			if (strlen(m_pData->ChannelName))
			{
				szLog = "Stop [ " + QString(m_pData->ChannelName) + " ] failed.";
			}
			else
			{
				szLog = "Stop [ " + QString("Channel:%1").arg(m_pData->OccNo) + " ] failed.";
			}

			LogString(szLog.toStdString().c_str(), 1);
		}

	}
	else
	{// 进程外
		// modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}
	return true;
}

bool  CFTChannel::LogString(const char *szLogTxt, int nLevel)
{
	::LogMsg("FES", szLogTxt, nLevel, nullptr);
	return true;
}

void CFTChannel::SetMailBoxID(int nID)
{
	m_nMailBoxID = nID;
	if (m_pData)
	{
		m_pData->MailBoxID = nID;
	}
}
// std::string  CFTChannel::GetTagName()const
// {
// 	Q_ASSERT(false);
// 	return "";
// }

/** @}*/
