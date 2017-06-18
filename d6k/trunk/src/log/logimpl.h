
#ifndef _LOG_IMPL_H
#define _LOG_IMPL_H

#include "singleton.h"

#include "msglog.h"
#include "log_config.h"

#include <string>
#include <QDateTime>
#include <deque>

#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  
#include <atomic>

#define MAX_BUFF_SIZE 1024
#define MAX_MSG_NUM 1024


class CQueue; 
 
class QFile;

class CWriteLogTask;
class CSendLogTask;

class CLogImpl
{
public:
	static CLogImpl *GetInstance();
public:
	bool CreateLog(const char* szLogName);
	bool DestroyLog(const char* szLogName);

	bool OpenLog(const char* szLogName);
	bool CloseLog(const char* szLogName);

	bool LogMsg(const char* szMoudleName,const char* szLogTxt,int nLevel);
	bool LogMsg(const char* szMoudleName, const char* szLogTxt, int nLevel,const QDateTime & tm);

	bool LogDebugMsg(const char* szMoudleName, const char* szLogTxt, int nLevel,const char * szFile,int nLine,const char *szFunc);
	bool LogDebugMsg(const char* szMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, const QDateTime & tm);


protected:
	void AddLogMsg(CMsgLog *pLog);
 
// 	void WriteLogTxtFile(const char * pszPath,bool bChanged = false);
// 	
// 	void WriteLogTxtFileImpl(const QString & szFileName, bool bOverWrite = false);
// 
// 	void WriteLogToQue();
// 	void ReadLogFromQue();

private:
	friend DefaultSingletonTraits<CLogImpl>;

	CLogImpl();
	~CLogImpl();

	
	std::size_t  m_nMaxIndex;
	
// 	std::shared_ptr<QFile > m_pLogFile;
// 	unsigned int m_nCurrentFile;

//	CLogConfig m_LogConf;
//	QString  m_szLogName;       //! LOGÃû³Æ
	QString  m_szLogPath;       //! LOG´æ´¢Â·¾¶

	CLogConfigMgr m_LogConfigs;
	
//	std::shared_ptr<CQueue> m_pQue;


	std::atomic<bool> m_bTaskIsStarted;
	std::shared_ptr<CWriteLogTask> m_pWriteFileTask;

	std::shared_ptr<CSendLogTask> m_pSendLogTask;
};
 

#endif // _LOG_IMPL_H

