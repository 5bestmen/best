

#include "msglog.h"

#include "public.h"
#include <QDebug>
#include <QApplication>
#include <QProcess>
#include <QThread>

CMsgLog::CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex, const QDateTime & dt)
{
	m_szModuleName = szModuleName;
	m_szLogText = szLog;
	m_nLevel = nLevel;
	m_nIndex = nIndex;
	m_nLine = 0;

//	m_tmLog = QDateTime::currentDateTime();
	m_tmLog = dt;

	//QString szOutput = m_tmLog.toString();
	//qDebug() << szOutput;

	m_nProcessId = QCoreApplication::applicationPid();
	m_nThreadId = reinterpret_cast <size_t> (QThread::currentThreadId());
}

CMsgLog::CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex, const char * szFileName, int nLine, const char * szFunc, const QDateTime & dt)
{
	m_szModuleName = szModuleName;
	m_szLogText = szLog;
	m_nLevel = nLevel;
	m_nIndex = nIndex;
	m_nLine = 0;

//	m_tmLog = QDateTime::currentDateTime();
	m_tmLog = dt;
	//QString szOutput = m_tmLog.toString();
	//qDebug() << szOutput;

	m_nProcessId = QCoreApplication::applicationPid();
	m_nThreadId = reinterpret_cast <size_t> (QThread::currentThreadId());

	if (szFileName && strlen(szFileName))
	{
		m_szFileName = szFileName;
	}
	if (szFunc && strlen(szFunc))
	{
		m_szFuncName = szFunc;
	}
	m_nLine = nLine;
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

		m_szFileName = pLog->m_szFileName;
		m_nLine = pLog->m_nLine;
		m_szFuncName = pLog->m_szFuncName;
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

	m_szFileName = other.m_szFileName;
	m_nLine = other.m_nLine;
	m_szFuncName = other.m_szFuncName;

	return *this;
}
/*! \fn size_t CMsgLog::SaveToBuffer(unsigned char *pInBuff, size_t nInBuffLen) const
********************************************************************************************************* 
** \brief CMsgLog::SaveToBuffer 
** \details 序列化
** \param pInBuff 
** \param nInBuffLen 
** \return size_t 
** \author LiJin 
** \date 2016年12月30日 
** \note 
********************************************************************************************************/
size_t CMsgLog::SaveToBuffer(unsigned char *pInBuff, size_t nInBuffLen) const
{
	Q_ASSERT(pInBuff);
	if (pInBuff == nullptr)
		return 0;
	Q_ASSERT(nInBuffLen>sizeof(m_nIndex));
	if (nInBuffLen <sizeof(m_nIndex))
		return 0;

	size_t nOffset = 0;
	size_t nSize = 0;
//	size_t nRet = 0;

	nSize = GetDataSize();
	if (nSize > nInBuffLen)
	{
		Q_ASSERT(false);
		return 0;
	}

	size_t nSync = 0x494c494c;
	
	std::memcpy(&pInBuff[nOffset], &nSync, sizeof(size_t));
	nOffset += sizeof(size_t);

	std::memcpy(&pInBuff[nOffset], &nSize, sizeof(size_t));
	nOffset += sizeof(size_t);

	nSize = sizeof(m_nIndex);
	std::memcpy(&pInBuff[nOffset], &m_nIndex, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nLevel);
	std::memcpy(&pInBuff[nOffset], &m_nLevel, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nThreadId);
	std::memcpy(&pInBuff[nOffset], &m_nThreadId, nSize);
	nOffset += nSize;

	nSize = sizeof(m_nProcessId);
	std::memcpy(&pInBuff[nOffset], &m_nProcessId, nSize);
	nOffset += nSize;

	//m_tmLog.toTime_t()
	TIMEPAK tp;
	tp.Year = m_tmLog.date().year();
	tp.Month = m_tmLog.date().month();
	tp.Day = m_tmLog.date().day();
	tp.DayOfWeek = m_tmLog.date().dayOfWeek();

	tp.Hour = m_tmLog.time().hour();
	tp.Minute = m_tmLog.time().minute();
	tp.Second = m_tmLog.time().second();
	tp.Milliseconds = m_tmLog.time().msec();

	nSize = sizeof(tp);
	std::memcpy(&pInBuff[nOffset], &tp, nSize);
	nOffset += nSize;

	size_t nLen = 0;
	// 字符串存储方式：先是长度，然后是内容。
	nLen = m_szModuleName.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szModuleName.data(), nLen);
		nOffset += nLen;
	}

	nLen = m_szLogText.length();
	Q_ASSERT(nLen >= 0 && nLen < 2048);

	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szLogText.data(), nLen);
		nOffset += nLen;
	}

	nLen = m_szFileName.length();
	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szFileName.data(), nLen);
		nOffset += nLen;
	}

	nSize = sizeof(m_nLine);
	std::memcpy(&pInBuff[nOffset], &m_nLine, nSize);
	nOffset += nSize;

	nLen = m_szFuncName.length();
	nSize = sizeof(nLen);
	std::memcpy(&pInBuff[nOffset], &nLen, nSize);
	nOffset += nSize;
	if (nLen)
	{
		std::memcpy(&pInBuff[nOffset], m_szFuncName.data(), nLen);
		nOffset += nLen;
	}	

	return nOffset;
}

bool  CMsgLog::LoadFromBuffer(const char *pInBuff, size_t nInBuffLen)
{
	size_t nOffset = 0;
	size_t nSize = 0;
	 
	size_t nSync = 0;

	std::memcpy(&nSync, &pInBuff[nOffset], sizeof(size_t));
	nOffset += sizeof(size_t);

	//nSize = sizeof(nSize);
	std::memcpy(&nSize, &pInBuff[nOffset], sizeof(size_t));
	nOffset += sizeof(size_t);

	Q_ASSERT(nSize > 0);
	if (nSize <= 4)
		return false;

	Q_ASSERT(nInBuffLen >= nSize);
	if (nInBuffLen < nSize)
		return false;

	nSize = sizeof(m_nIndex);
	std::memcpy(&m_nIndex, &pInBuff[nOffset], nSize);
	nOffset += nSize;

	nSize = sizeof(m_nLevel);
	std::memcpy(&m_nLevel, &pInBuff[nOffset], nSize);
	nOffset += nSize;

	nSize = sizeof(m_nThreadId);
	std::memcpy(&m_nThreadId, &pInBuff[nOffset], nSize);
	nOffset += nSize;

	nSize = sizeof(m_nProcessId);
	std::memcpy(&m_nProcessId, &pInBuff[nOffset], nSize);
	nOffset += nSize;

	TIMEPAK tp;
	memset(&tp, 0, sizeof(tp));

	nSize = sizeof(tp);
	std::memcpy(&tp, &pInBuff[nOffset], nSize);
	nOffset += nSize;

//	QDate dt(tp.Year, tp.Month, tp.Day);
	QDate dt;
	dt.setDate(tp.Year, tp.Month, tp.Day);
	m_tmLog.setDate(dt);
//	QTime tm(tp.Hour, tp.Minute, tp.Second, tp.Milliseconds);
	QTime tm ;
	tm.setHMS(tp.Hour, tp.Minute, tp.Second, tp.Milliseconds);
	m_tmLog.setTime(tm);
	
//	qDebug() << "log time is:" << m_tmLog.date().year();

	size_t nLen = 0;
	//nLen = m_szModuleName.length();

	nSize = sizeof(nLen);
	std::memcpy(&nLen, &pInBuff[nOffset], nSize);
	nOffset += nSize;
	Q_ASSERT(nLen >= 0);
	if (nLen>0)
	{
		m_szModuleName.reserve(nLen);
		m_szModuleName.assign(reinterpret_cast<const char*>(&pInBuff[nOffset]), nLen);
	//  std::memcpy(m_szModuleName.data(), &pInBuff[nOffset], nLen);		
		nOffset += nLen;
	}

	nLen = m_szLogText.length();
	nSize = sizeof(nLen);
	std::memcpy(&nLen, &pInBuff[nOffset], nSize);
	nOffset += nSize;
	Q_ASSERT(nLen >= 0);
	if (nLen > 0)
	{
		m_szLogText.reserve(nLen);
		m_szLogText.assign(reinterpret_cast<const char*>(&pInBuff[nOffset]), nLen);

		nOffset += nLen;
	}
	else
	{
		m_szLogText.clear();
	}

	nLen = m_szFileName.length();

	nSize = sizeof(nLen);
	std::memcpy(&nLen, &pInBuff[nOffset], nSize);
	nOffset += nSize;
	Q_ASSERT(nLen >= 0);
	if (nLen>0)
	{
		m_szFileName.reserve(nLen);
		m_szFileName.assign(reinterpret_cast<const char*>(&pInBuff[nOffset]), nLen);
		nOffset += nLen;
	}
	else
	{
		m_szFileName.clear();
	}

	nSize = sizeof(m_nLine);
	std::memcpy(&m_nLine, &pInBuff[nOffset], nSize);
	nOffset += nSize;
	
	nLen = m_szFuncName.length();
	nSize = sizeof(nLen);
	std::memcpy(&nLen, &pInBuff[nOffset], nSize);
	nOffset += nSize;
	Q_ASSERT(nLen >= 0);
	if (nLen>0)
	{
		m_szFuncName.reserve(nLen);
		m_szFuncName.assign(reinterpret_cast<const char*>(&pInBuff[nOffset]), nLen);

		nOffset += nLen;
	}
	else
	{
		m_szFileName.clear();
	}

	return true;
}

void CMsgLog::SaveToLogData(LOG_DATA  *pData) const
{
	Q_ASSERT(pData);
	if (pData == nullptr)
		return;
	memset(pData, 0, sizeof(LOG_DATA));
	pData->m_nIndex = m_nIndex;

	pData->m_nLevel = m_nLevel;
	pData->m_nThreadId = m_nThreadId;
	pData->m_nProcessId = m_nProcessId;
	 
	pData->m_tp.Year = m_tmLog.date().year();
	pData->m_tp.Month = m_tmLog.date().month();
	pData->m_tp.Day = m_tmLog.date().day();
	pData->m_tp.DayOfWeek = m_tmLog.date().dayOfWeek();

	pData->m_tp.Hour = m_tmLog.time().hour();
	pData->m_tp.Minute = m_tmLog.time().minute();
	pData->m_tp.Second = m_tmLog.time().second();
	pData->m_tp.Milliseconds = m_tmLog.time().msec();
	pData->m_nLine = m_nLine;

	strncpy(pData->m_szModuleName, m_szModuleName.c_str(), std::min<size_t>(256, m_szModuleName.length()));
	strncpy(pData->m_szFileName, m_szFileName.c_str(), std::min<size_t>(64, m_szFileName.length()));
	strncpy(pData->m_szFuncName, m_szFuncName.c_str(), std::min<size_t>(64, m_szFuncName.length()));
	strncpy(pData->m_szLogText, m_szLogText.c_str(), std::min<size_t>(512, m_szLogText.length()));
}

bool CMsgLog::LoadFromLogData(const LOG_DATA *pData)
{
	m_nIndex = pData->m_nIndex;

	m_nLevel = pData->m_nLevel;
	m_nThreadId = pData->m_nThreadId;
	m_nProcessId = pData->m_nProcessId;
	m_nLine = pData->m_nLine;

	QDate dt;
	dt.setDate(pData->m_tp.Year, pData->m_tp.Month, pData->m_tp.Day);
	m_tmLog.setDate(dt);
	//	QTime tm(tp.Hour, tp.Minute, tp.Second, tp.Milliseconds);
	QTime tm;
	tm.setHMS(pData->m_tp.Hour, pData->m_tp.Minute, pData->m_tp.Second, pData->m_tp.Milliseconds);
	m_tmLog.setTime(tm);

	if (strlen(pData->m_szModuleName))
	{
		m_szModuleName = pData->m_szModuleName;
	}
	if (strlen(pData->m_szFileName))
	{
		m_szFileName = pData->m_szFileName;
	}
	if (strlen(pData->m_szFuncName))
	{
		m_szFuncName = pData->m_szFuncName;
	}
	if (strlen(pData->m_szLogText))
	{
		m_szLogText = pData->m_szLogText;
	} 
	return true;
}

/*! \fn size_t CMsgLog::GetDataSize() const
********************************************************************************************************* 
** \brief CMsgLog::GetDataSize 
** \details 有效内容长度，字符串的存储方式：长度+内容
** \return size_t 
** \author LiJin 
** \date 2016年12月30日 
** \note 
********************************************************************************************************/
size_t CMsgLog::GetDataSize() const
{
	size_t nOffset = 0;
	size_t nSize = 0;

	nSize = sizeof(m_nIndex);
	nOffset += nSize;

	nSize = sizeof(m_nLevel);
	nOffset += nSize;

	nSize = sizeof(m_nThreadId);
	nOffset += nSize;

	nSize = sizeof(m_nProcessId);
	nOffset += nSize;

	nSize = sizeof(TIMEPAK);
	nOffset += nSize;

	size_t nLen = 0;

	nLen = m_szModuleName.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szLogText.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}

	nLen = m_szFileName.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}
	
	nSize = sizeof(m_nLine);
	nOffset += nSize;


	nLen = m_szFuncName.length();
	nSize = sizeof(nLen);
	nOffset += nSize;
	if (nLen > 0)
	{
		nOffset += nLen;
	}
	
	return nOffset;
}


