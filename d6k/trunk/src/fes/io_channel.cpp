/*! @file
<PRE>
********************************************************************************
模块名       :
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
#include "io_channel.h"
#include "fesapi/fesdatadef.h"

#include <QDebug>
#include <QLibrary> 

CIoChannel::CIoChannel()
{
	m_nOccNo = 0;
	m_pData = nullptr;
	m_fnStartIoDriver = nullptr;
	m_fnStopIoDriver = nullptr;
}

CIoChannel::~CIoChannel()
{
	
}

CIoChannel::CIoChannel(CHANNEL *pChannel) :m_pData(pChannel)
{
	Q_ASSERT(pChannel);
	m_nOccNo = m_pData->OccNo;

	m_fnStartIoDriver = nullptr;
	m_fnStopIoDriver = nullptr;
}

bool CIoChannel::Load()
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
			m_fnStartIoDriver = (StartIoDriver)dlllib.resolve("StartIoDriver");
			m_fnStopIoDriver = (StopIoDriver)dlllib.resolve("StopIoDriver");
			Q_ASSERT(m_fnStartIoDriver);
			Q_ASSERT(m_fnStopIoDriver);

			if (m_fnStartIoDriver && m_fnStopIoDriver)
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

bool CIoChannel::Start(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog;

	if (m_pData->DriverType == 0)
	{// 进程内 
	 
//		Q_ASSERT(m_fnStartIoDriver);	
		if (m_fnStartIoDriver )
		{
			m_fnStartIoDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
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

bool CIoChannel::Stop(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog;

	if (m_pData->DriverType == 0)
	{// 进程内 
		Q_ASSERT(m_fnStopIoDriver);

		if ( m_fnStopIoDriver)
		{
			m_fnStopIoDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
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

bool  CIoChannel::LogString(const char *szLogTxt, int nLevel)
{

	return true;
}
void CIoChannel::SetMailBoxID(int nID)
{
	m_nMailBoxID = nID;
	if (m_pData)
	{
		m_pData->MailBoxID = nID;
	}
}
// std::string  CIoChannel::GetTagName()const
// {
// 	Q_ASSERT(false);
// 	return "";
// }

/** @}*/
