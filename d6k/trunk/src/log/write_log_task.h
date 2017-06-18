/*! @file scadalog_mq.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  scadalog_mq.cpp
�ļ�ʵ�ֹ��� :  ͨ��mq
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
	void AddLogMsg(CMsgLog *pLog);

	void InitFilePath();

	void StartLogTask(const QString &szName);
protected:
	virtual void MainLoop();
	
protected:
	//  ��̨��������
	size_t GetLogCount()const;
	bool GetLogString(CMsgLog *pMsg);
	bool TakeMsgLog(std::shared_ptr<CMsgLog>&pLog);

//	void WriteLogTxtFile(const char * pszPath, bool bChanged = false);

	void WriteLogTxtFile(std::shared_ptr<CModuleLogConfig> pModuleConf,const char * pszPath, bool bChanged = false);

	void WriteLogTxtFileImpl(const std::shared_ptr<CModuleLogConfig> pModuleConf, const QString & szFileName, bool bOverWrite = false);
private: 
	void Clear();

	mutable std::mutex m_LogLock;

	std::deque <std::shared_ptr<CMsgLog>> m_arrLogs;

//	CLogConfig &m_LogConf;

	CLogConfigMgr &m_LogConfig;

//	unsigned int m_nCurrentFile;

	QString  m_szLogPath;       //! LOG�洢·��

	std::atomic<bool> m_bIsStarted;

//	CWriteTaskInfo m_WriteTaskInfo;
private:
	enum
	{
		LOG_HANDLE_CYCLE = 5,    //ÿ5�봦��һ����־
		LOG_HANDLE_COUNT = 1000, //ÿ�ﵽ1000������һ����־
	};
};


#endif // WRITE_LOG_TASK_H


/** @}*/

