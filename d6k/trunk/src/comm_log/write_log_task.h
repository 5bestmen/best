/*! @file scadalog_mq.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  scadalog_mq.cpp
文件实现功能 :  通过mq
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief    
*  @author  LiJin
*  @version 1.0
*  @date    2016.12.28
*******************************************************************************/
#ifndef WRITE_LOG_TASK_H
#define WRITE_LOG_TASK_H

#include "base_module.h"
#include "log_config.h"

#include <deque>
#include <string>
#include <mutex>

#include <QDateTime>
 
class CMsgLog;
class QFile;

class CWriteLogTask:public CBaseModule
{
public:
	CWriteLogTask(CLogConfigMgr & conf);
	~CWriteLogTask();
public:  
	void AddLogMsg(CCommMsgLog *pLog);

	void InitFilePath();

	void StartLogTask(const QString &szName);
protected:
	virtual void MainLoop();
	
protected:
	//  后台工作调用
	size_t GetLogCount()const;
	bool GetLogString(CCommMsgLog *pMsg);
	bool TakeMsgLog(std::shared_ptr<CCommMsgLog>&pLog);

//	void WriteLogTxtFile(const char * pszPath, bool bChanged = false);

	void WriteLogTxtFile(std::shared_ptr<CModuleLogConfig> pModuleConf,const char * pszPath, bool bChanged = false);

	void WriteLogTxtFileImpl(const std::shared_ptr<CModuleLogConfig> pModuleConf, const QString & szFileName, bool bOverWrite = false);
private: 
	void Clear();

	mutable std::mutex m_LogLock;

	std::deque <std::shared_ptr<CCommMsgLog>> m_arrLogs;

	CLogConfigMgr &m_LogConfig;

//	unsigned int m_nCurrentFile;

	QString  m_szLogPath;       //! LOG存储路径

	std::atomic<bool> m_bIsStarted;

private:
	enum
	{
		LOG_HANDLE_CYCLE = 5,    //每5秒处理一次日志
		LOG_HANDLE_COUNT = 1000, //每达到1000条处理一次日志
	};
};


#endif // WRITE_LOG_TASK_H


/** @}*/

