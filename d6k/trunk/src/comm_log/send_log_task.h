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
#ifndef SEND_LOG_TASK_H
#define SEND_LOG_TASK_H

#include "base_module.h"

#include <deque>
#include <string>
#include <mutex>

#include <QDateTime>
 
class CCommMsgLog;

class CSendLogTask:public CBaseModule
{
public:
	CSendLogTask();
	~CSendLogTask();
public:  
	bool SendScadaLog();
	void AddLogMsg(CCommMsgLog *pLog);
	bool StartLogTask(unsigned int nPortNo);
protected:
	bool TakeMsgLog(std::shared_ptr<CCommMsgLog>&pLog);
	virtual void MainLoop();
private: 
	void * m_pCtx;
	void * m_pSender;
	
	std::atomic<bool> m_bIsStarted;

	bool InitMQ(unsigned int nPortNo);

	void CloseMQ();

	void Clear();

	size_t GetLogCount()const;
	bool GetLogString(CCommMsgLog *pMsg);

	mutable std::mutex m_LogLock;

	std::deque <std::shared_ptr<CCommMsgLog>> m_arrLogs;
	enum  
	{
		MAX_LOG_NUM = 1024,
	};
};



#endif // SCADA_LOG_IMPL_H


/** @}*/

