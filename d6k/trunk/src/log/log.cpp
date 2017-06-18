
#include "log/log.h"

#include "logimpl.h"

#include <vector>

extern "C"
{
	LOGAPI bool CreateLog(const char* szLogName)
	{
		return CLogImpl::GetInstance()->CreateLog(szLogName);
	}

	LOGAPI bool DestroyLog(const char* szLogName)
	{
		return CLogImpl::GetInstance()->DestroyLog(szLogName);
	}

	LOGAPI bool LogMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, struct TIMEPAK *pTm)
	{
		QDateTime  now = QDateTime::currentDateTime();
		if (pTm != nullptr)
		{
			Q_ASSERT(pTm->Year > 1978 && pTm->Year < 2500);
			Q_ASSERT(pTm->Month > 0 && pTm->Month < 13);
			Q_ASSERT(pTm->Day > 0 && pTm->Day < 32);
			Q_ASSERT(pTm->Hour >= 0 && pTm->Hour < 25);
			Q_ASSERT(pTm->Minute >= 0 && pTm->Minute < 61);
			Q_ASSERT(pTm->Second >= 0 && pTm->Second < 61);
			Q_ASSERT(pTm->Milliseconds > 0 && pTm->Milliseconds < 1000);

			QDate dt;
			dt.setDate(pTm->Year, pTm->Month, pTm->Day);
			now.setDate(dt);

			QTime tm;
			tm.setHMS(pTm->Hour, pTm->Minute, pTm->Second, pTm->Milliseconds);
			now.setTime(tm);
		}
		return CLogImpl::GetInstance()->LogMsg(pszMoudleName, szLogTxt, nLevel, now);
	}

	LOGAPI bool LogDebugMsg(const char * pszMoudleName, const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, struct TIMEPAK *pTm)
	{
		QDateTime  now = QDateTime::currentDateTime();
		if (pTm != nullptr)
		{
			Q_ASSERT(pTm->Year > 1978 && pTm->Year < 2500);
			Q_ASSERT(pTm->Month > 0 && pTm->Month < 13);
			Q_ASSERT(pTm->Day > 0 && pTm->Day < 32);
			Q_ASSERT(pTm->Hour >= 0 && pTm->Hour < 25);
			Q_ASSERT(pTm->Minute >= 0 && pTm->Minute < 61);
			Q_ASSERT(pTm->Second >= 0 && pTm->Second < 61);
			Q_ASSERT(pTm->Milliseconds > 0 && pTm->Milliseconds < 1000);

			QDate dt;
			dt.setDate(pTm->Year, pTm->Month, pTm->Day);
			now.setDate(dt);

			QTime tm;
			tm.setHMS(pTm->Hour, pTm->Minute, pTm->Second, pTm->Milliseconds);
			now.setTime(tm);
		}
		return CLogImpl::GetInstance()->LogDebugMsg(pszMoudleName, szLogTxt, nLevel, szFile, nLine, szFunc, now);
	}

	LOGAPI bool OpenLog(const char* szLogName)
	{
		return CLogImpl::GetInstance()->OpenLog(szLogName);
	}

	LOGAPI bool CloseLog(const char* szLogName)
	{
		return CLogImpl::GetInstance()->CloseLog(szLogName);
	}
}