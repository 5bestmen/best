
#ifndef MSG_LOG_H
#define MSG_LOG_H
 
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
 

class CMsgLog
{
public:
	CMsgLog()
	{
		m_nIndex = 0;

		m_nLevel = 0;
		m_nThreadId = 0;
		m_nProcessId = 0;

		m_nLine = 0;
	}

	CMsgLog(CMsgLog *pLog);
	CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex,const QDateTime & dt);
	CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex, const char * szFileName, int nLine, const char * szFunc, const QDateTime & dt);

	CMsgLog& operator= (const CMsgLog &);

	~CMsgLog()
	{

	}
public:
	// 序列化
	size_t SaveToBuffer(unsigned char *pInBuff,size_t nInBuffLen)const;
	bool  LoadFromBuffer(const char *pInBuff, size_t nInBuffLen);

	void SaveToLogData(LOG_DATA  *pData) const;
	bool LoadFromLogData(const LOG_DATA *pData);

	// 数据长度（包含同步头，长度等信息）
	size_t GetMsgSize() const
	{
		size_t nSyncHeadLen = sizeof(size_t);
		size_t nNextLen = sizeof(size_t);
		
		return GetDataSize() + nSyncHeadLen + nNextLen;
	}
private:
	// 有效数据长度（不包含同步头，长度等信息）
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

