/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ���dock����
�ļ���       :  sysoutdock.cpp
�ļ�ʵ�ֹ��� :  ϵͳ���dock����
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.15
*/
#ifndef LOG_IMPL_H
#define LOG_IMPL_H

#include <deque>
#include <string>
#include <mutex>

#include <QDateTime>

#include "singleton.h"


class CMsgLog
{
public:
	CMsgLog()
	{
		m_nIndex = 0;
		m_nProcessId = 0;
		m_nThreadId = 0;
		m_nLevel = 0;

		m_szModuleName = "";
		m_szLogText = "";
	}
	CMsgLog(CMsgLog *pLog);
	CMsgLog(const std::string & szModuleName, const char * szLog, int nLevel, size_t nIndex);
	CMsgLog& operator= (const CMsgLog &);

	~CMsgLog()
	{

	}
public:
	size_t m_nIndex;

	size_t m_nProcessId;
	size_t m_nThreadId;

	unsigned int m_nLevel;

	QDateTime m_tmLog;

	std::string m_szModuleName;
	std::string m_szLogText;
};


class CLogImpl
{
public:
public:
	static CLogImpl *GetInstance();	 
public:
	// ������
	void LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel);
	//  UI���ֵ���
	size_t GetLogCount()const;
	bool GetLogString(CMsgLog *pMsg);
private:
	friend DefaultSingletonTraits<CLogImpl>;

	CLogImpl();
	~CLogImpl();

	void Clear();

	mutable std::mutex m_LogLock;

	std::deque <CMsgLog*> m_arrLogs;
	std::size_t  m_nMaxIndex;
 
};



#endif // LOG_IMPL_H


/** @}*/

