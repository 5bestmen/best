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

#include "send_log_task.h"
#include "stl_util-inl.h"

#include <QDebug>
#include "libzmq/zmq.h"

#include "msglog.h"

CSendLogTask::CSendLogTask()
{
	m_pCtx = nullptr;
	m_pSender = nullptr;
	m_bIsStarted = false;
}

CSendLogTask::~CSendLogTask()
{
	StopModule();

	CloseMQ();
	Clear();
	m_bIsStarted = false;
}

bool CSendLogTask::StartLogTask(unsigned int nPortNo)
{
	if (m_bIsStarted == false)
	{
		m_bIsStarted = true;

		bool bRet = InitMQ(nPortNo);
		Q_ASSERT(bRet);
		if (bRet)
		{
			StartModule();
		}
		return bRet;	
	}
	return true;
}

bool CSendLogTask::InitMQ(unsigned int nPortNo)
{
	if (nPortNo == 0)
		return false;
	
	m_pCtx = zmq_ctx_new();
	Q_ASSERT(m_pCtx);
	if (m_pCtx == nullptr)
		return false;

	m_pSender = zmq_socket(m_pCtx, ZMQ_PUSH);

	Q_ASSERT(m_pSender);
	if (m_pSender == nullptr)
		return false;

	QString szAddr = QString("tcp://*:%1").arg(nPortNo);

//	int nRet = zmq_bind(m_pSender, "tcp://*:10002");
	int nRet = zmq_bind(m_pSender, szAddr.toStdString().c_str());
	Q_ASSERT(nRet == 0);
	if (nRet != 0)
		return false;


	return true;
}

void CSendLogTask::CloseMQ()
{
	if (m_pSender)
	{
		zmq_close(m_pSender);
		m_pSender = nullptr;
	}
	if (m_pCtx)
	{
		zmq_ctx_destroy(m_pCtx);
		m_pCtx = nullptr;
	}
}

void CSendLogTask::Clear()
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.clear();
}
/*! \fn void  CSendLogTask::AddLogMsg(CMsgLog *pMsg)
********************************************************************************************************* 
** \brief CSendLogTask::AddLogMsg 
** \details 
** \param pMsg 
** \return void 
** \author LiJin 
** \date 2016年12月29日 
** \note 
********************************************************************************************************/
void  CSendLogTask::AddLogMsg(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	std::shared_ptr<CMsgLog> pMsgLog = std::make_shared<CMsgLog>(pMsg);

	std::lock_guard<std::mutex> lock(m_LogLock);

	size_t nCnt = m_arrLogs.size();
	if (nCnt >= MAX_LOG_NUM)
	{
		while (nCnt >= MAX_LOG_NUM)
		{
			m_arrLogs.pop_front();
			nCnt = m_arrLogs.size();
		}
	}

	m_arrLogs.push_back(pMsgLog);
}

#if 0
static void FreeMsgLog(void *pData, void *hint)
{
	if (pData)
	{
		free(pData);
		pData = nullptr;
	}
}
#endif

bool CSendLogTask::SendScadaLog()
{
	if (m_pSender == nullptr)
	{
		return false;
	}

	bool bRet = false;
	std::shared_ptr<CMsgLog> pLog;
	
	bRet = TakeMsgLog(pLog);
	if (bRet == false )
	{
		return false;
	}

	Q_ASSERT(pLog);
	if (pLog == nullptr)
	{
		return false;
	}

	size_t nMsgSize = pLog->GetMsgSize();

	Q_ASSERT(nMsgSize);
	if (nMsgSize <= 0)
		return false;
	
	int nRet = 0;
	
// 	static unsigned char pData[16 * 1024] = { 0 };
// 	memset(pData, 0, 16 * 1024);

	zmq_msg_t msg;
	nRet = zmq_msg_init_size(&msg, (nMsgSize));
	size_t nTemp = pLog->SaveToBuffer(reinterpret_cast<unsigned char*> (zmq_msg_data(&msg)), nMsgSize);

//	memcpy(zmq_msg_data(&msg), pData,nMsgSize);
#if 0

// 	void *pData = malloc(nMsgSize);
// 	memset(pData, 0, nMsgSize);

//	size_t nTemp = pLog->SaveToBuffer(reinterpret_cast<unsigned char*> (pData), nMsgSize);
	LOG_DATA data_pool;

	pLog->SaveToLogData(&data_pool);

	nRet = zmq_msg_init_size(&msg, sizeof(LOG_DATA));
	memcpy(zmq_msg_data(&msg), &data_pool, sizeof(LOG_DATA));

//	nRet = zmq_msg_init_data(&msg, pData, nMsgSize, FreeMsgLog, nullptr);
//	nRet = zmq_msg_init_data(&msg, pData, nMsgSize, nullptr, nullptr);
//	nRet = zmq_msg_init_data(&msg, &data_pool, sizeof(LOG_DATA), nullptr, nullptr);
#endif

	Q_ASSERT(nRet != -1);
	if (nRet == -1)
	{
		zmq_msg_close(&msg);
		return false;
	}

	nRet = zmq_msg_send(&msg, m_pSender, ZMQ_DONTWAIT);
  
	if (nRet == -1)
	{
		zmq_msg_close(&msg);
		return false;
	}
	zmq_msg_close(&msg);

	return true;
}

void CSendLogTask::MainLoop()
{
	bool bRet = false;
	size_t nCount = 0;
	while (true)
	{
		bRet = SendScadaLog();
		if (bRet == true)
		{
			continue;
		}
		else
		{
			break;
		}
		if (nCount >= 100)
		{
			break;
		}
		nCount++;
	}
}

size_t CSendLogTask::GetLogCount()const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CSendLogTask::GetLogString(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	std::shared_ptr<CMsgLog> pTempLog = nullptr;

	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pTempLog = m_arrLogs.front();
	Q_ASSERT(pTempLog);
	if (pTempLog)
	{
		*pMsg = *pTempLog;
		m_arrLogs.pop_front();
	}
	return true;
}

bool  CSendLogTask::TakeMsgLog(std::shared_ptr<CMsgLog>&pLog)
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pLog = m_arrLogs.front();
	m_arrLogs.pop_front();
	return true;
}

/** @}*/
