
#ifndef COMM_LOG_IMPL_H
#define COMM_LOG_IMPL_H


#include "log_config.h"

#include "singleton.h"

#include <string>
#include <QDateTime>
#include <deque>

#include <memory>
#include <thread>
#include <atomic>

#include <mutex>
#include <condition_variable>
#include <chrono>  


class CWriteLogTask;
class CSendLogTask;
class CCommMsgLog;

class CCommLogImpl
{
public:
	static CCommLogImpl *GetInstance();
public:
	bool CreateLog(const char* szLogName);
	bool DestroyLog(const char* szLogName);

	bool OpenLog(const char* szLogName);
	bool CloseLog(const char* szLogName);

	bool CommMsgLog(const char *pszModuleName, const char *pszProtocalName, const unsigned char *pMsg, unsigned int nMsgLen, unsigned int nMsgType,
		const char * pMsgSubType, const char *pSource, const char *pDest, const char *pDescription);

	bool CommMsgLog(struct COMM_MSG *pMsgLog);
protected:
	void AddCommMsg(CCommMsgLog *pCommLog);

private:
	friend DefaultSingletonTraits<CCommLogImpl>;

	CCommLogImpl();
	~CCommLogImpl();


	std::size_t  m_nMaxIndex;

	// 	std::shared_ptr<QFile > m_pLogFile;
	// 	unsigned int m_nCurrentFile;

	//	CLogConfig m_LogConf;
	//	QString  m_szLogName;       //! LOGÃû³Æ
	QString  m_szLogPath;       //! LOG´æ´¢Â·¾¶

 	CLogConfigMgr m_LogConfigs;
	 


	std::atomic<bool> m_bTaskIsStarted;
	std::shared_ptr<CWriteLogTask> m_pWriteFileTask;
	std::shared_ptr<CSendLogTask> m_pSendLogTask;
};



#endif // _LOG_IMPL_H

