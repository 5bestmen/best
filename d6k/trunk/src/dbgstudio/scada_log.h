
#ifndef SCADA_MSG_LOG_H
#define SCADA_MSG_LOG_H
 
#include <string>
#include <QDateTime>

#include "public.h"

struct  LOG_DATA
{
	size_t m_nIndex;

	unsigned int m_nLevel;
	size_t   m_nThreadId;
	size_t   m_nProcessId;

	TIMEPAK  m_tp;

	char m_szModuleName[256];

	char  m_szFileName[64];
	unsigned int m_nLine;
	char  m_szFuncName[64];

	char m_szLogText[512];
};

class CScadaLog
{
public:
	CScadaLog()
	{
		m_nIndex = 0;
		m_nLevel = 0;
		m_nThreadId = 0;
		m_nProcessId = 0;
		m_nLine = 0;
	}
	CScadaLog(CScadaLog *pLog);
	CScadaLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex);
	CScadaLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex,const char * szFileName,int nLine,const char * szFunc);

	CScadaLog& operator= (const CScadaLog &);

	~CScadaLog()
	{

	}
public:
	size_t SaveToBuffer(unsigned char *pInBuff,size_t nInBuffLen) const;
	bool  LoadFromBuffer(const char *pInBuff, size_t nInBuffLen);

	void SaveToLogData(LOG_DATA  *pData) const;
	bool LoadFromLogData(const LOG_DATA *pData);

	size_t GetMsgSize() const
	{
		size_t nSyncHeadLen = sizeof(size_t);
		size_t nNextLen = sizeof(size_t);

		return GetDataSize() + nSyncHeadLen + nNextLen;
	}
private:
	size_t GetDataSize() const;

public:
	size_t m_nIndex;

	unsigned int m_nLevel;
	size_t   m_nThreadId;
	size_t   m_nProcessId;	

	QDateTime m_tmLog;
	std::string m_szModuleName;

	std::string  m_szFileName;
	unsigned int m_nLine;
	std::string  m_szFuncName;

	std::string m_szLogText;
};
 
 

#endif // MSG_LOG_H

