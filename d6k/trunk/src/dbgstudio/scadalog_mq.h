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
#ifndef SCADA_LOG_IMPL_H
#define SCADA_LOG_IMPL_H

#include "base_module.h"

#include <deque>
#include <string>
#include <mutex>
#include <vector>

#include <QDateTime>
#include <QString>
 
class CScadaLog;
class CLogConfigMgr;

class CMQTaskInfo
{
public:
	CMQTaskInfo()
	{
		m_pCtx = nullptr;
		m_pReceiver = nullptr;
		m_nPortNo = 0;
	}
	explicit CMQTaskInfo(unsigned int nNo,const QString & szLogName)
	{
		m_pCtx = nullptr;
		m_pReceiver = nullptr;
		m_nPortNo = nNo;
		m_szLogName = szLogName;
	}
	~CMQTaskInfo()
	{

	}

	QString GetAddr()
	{
		QString szAddr = QString("tcp://localhost:%1").arg(m_nPortNo);
		return szAddr;
	}
public:
	void * m_pCtx;
	void * m_pReceiver;
	unsigned int m_nPortNo;

	QString  m_szLogName;
};

class CScadaLogMQTask:public CBaseModule
{
public:
	CScadaLogMQTask();
	~CScadaLogMQTask();
public:  
	void StartLogMQ();

	bool RecvLogs();

	bool RecvScadaLog();

	size_t GetLogCount()const;

//	bool GetLogString(CScadaLog *pMsg);
	bool GetLogString(std::shared_ptr<CScadaLog>pMsg);

	bool GetLogs(std::deque<std::shared_ptr<CScadaLog>> & logs);
protected:
	void LogString( const char *szLogTxt, int nLevel);
	virtual void MainLoop(void *pIn);
private: 
	void * m_pCtx;
	void * m_pReceiver;

	bool InitMQ();

	void CloseMQ();

	void Clear();

	mutable std::mutex m_LogLock;

	std::shared_ptr<CLogConfigMgr> m_pConfigs;
	std::vector<std::shared_ptr<CMQTaskInfo>> m_arrTaskInfos;

	std::shared_ptr<CMQTaskInfo> GetTaskInfo(unsigned int nPortNo)
	{
		auto it = m_arrTaskInfos.cbegin();
		for (; it != m_arrTaskInfos.cend();++it)
		{
			if ((*it)->m_nPortNo == nPortNo)
			{
				return *it;
			}
		}
		return nullptr;
	}

 //	std::deque <CScadaLog*> m_arrLogs;
	std::deque<std::shared_ptr<CScadaLog>> m_arrLogs;
	std::size_t  m_nMaxIndex; 
};



#endif // SCADA_LOG_IMPL_H


/** @}*/

