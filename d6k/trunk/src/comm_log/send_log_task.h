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

