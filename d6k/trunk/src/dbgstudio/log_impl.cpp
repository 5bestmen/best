/*! @file
<PRE>
********************************************************************************
模块名       :  系统输出dock窗口
文件名       :  sysoutdock.cpp
文件实现功能 :  系统输出dock窗口
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.15
*/
#include "stl_util-inl.h"
#include "log_impl.h"

#include <QProcess>
#include <QThread>
#include <QDebug>

#if defined (WIN32) || defined (WIN64) 
#include <process.h>
#endif

#if defined(__linux__) || defined(__linux)
#include<unistd.h>
#endif // LINUX


size_t GetCurPID()
{
	size_t nPid = 0;
#if defined (WIN32) || defined (WIN64) 
#include <process.h>

	nPid = _getpid();
#endif
#if defined(__linux__) || defined(__linux)
#include<unistd.h>
	nPid = getpid();
#endif // LINUX
	return nPid;
}

CLogImpl::CLogImpl()
{
	m_nMaxIndex = 0;
}

CLogImpl::~CLogImpl()
{	 
	Clear();
}
 

CLogImpl * CLogImpl::GetInstance()
{
	return Singleton<CLogImpl, DefaultSingletonTraits<CLogImpl>>::instance();
}
void CLogImpl::Clear()
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	STLDeleteElements(&m_arrLogs);
}

// 报警用
void CLogImpl::LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel)
{
	Q_ASSERT(szLogTxt != nullptr);
	if (szLogTxt == nullptr)
		return;
	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return;
	std::string szModuleName;
	if (pszMoudleName == nullptr)
	{
		szModuleName = "Unknown";
	}
	else
	{
		if (strlen(pszMoudleName) == 0)
		{
			szModuleName = "Unknown";
		}
		else
		{
			szModuleName = pszMoudleName;
		}
	}
	m_nMaxIndex++;
	CMsgLog *pMsg = new CMsgLog(szModuleName,szLogTxt,nLevel,m_nMaxIndex);
	
	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.push_back(pMsg);
}
//  UI部分调用
size_t CLogImpl::GetLogCount() const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CLogImpl::GetLogString(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;
	
	CMsgLog *pTempLog = nullptr;

	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pTempLog = m_arrLogs.front();
	Q_ASSERT(pTempLog);
	if (pTempLog)
	{
		*pMsg = *pTempLog;
		m_arrLogs.pop_front();
		delete pTempLog;
	}
	return true;
}

CMsgLog::CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex)
{
	m_szModuleName = szModuleName;
	m_szLogText = szLog;
	m_nLevel = nLevel;
	m_nIndex = nIndex;

	m_tmLog = QDateTime::currentDateTime();
	 
	//QString szOutput = m_tmLog.toString();
	//qDebug() << szOutput;
 
	m_nProcessId = GetCurPID();
//	m_nProcessId = QProcess::processId();
	m_nThreadId = reinterpret_cast <size_t> (QThread::currentThreadId());
}
CMsgLog::CMsgLog(CMsgLog *pLog)
{
	Q_ASSERT(pLog);
	Q_ASSERT(pLog != this);
	if (pLog && pLog != this)
	{
		m_nIndex = pLog->m_nIndex;
		m_szModuleName = pLog->m_szModuleName;
		m_szLogText = pLog->m_szLogText;
		m_nLevel = pLog->m_nLevel;

		m_tmLog = pLog->m_tmLog;

//		QString szOutput = m_tmLog.toString();
//		qDebug() << szOutput;

		//	m_nProcessId = QProcess::processId();
		m_nThreadId = pLog->m_nThreadId;
		m_nProcessId = pLog->m_nProcessId;
	}
}


CMsgLog& CMsgLog::operator= (const CMsgLog & other)
{
	if (this == &other)
	{
		return *this;
	}
	m_nIndex = other.m_nIndex;
	m_nProcessId = other.m_nProcessId;
	m_nThreadId = other.m_nThreadId;
	m_nLevel = other.m_nLevel;
	m_szModuleName = other.m_szModuleName;
	m_szLogText = other.m_szLogText;

	m_tmLog = other.m_tmLog;

	return *this;
}
/** @}*/