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

#include "scadalog_mq.h"
#include "stl_util-inl.h"

#include <QDebug>
#include "libzmq/zmq.h"

#include "scada_log.h"

#include "log_config.h"
#include "log_impl.h"

CScadaLogMQTask::CScadaLogMQTask()
{
	m_pConfigs = std::make_shared<CLogConfigMgr>();
	m_pCtx = nullptr;
	m_pReceiver = nullptr;
//	InitMQ();
}

CScadaLogMQTask::~CScadaLogMQTask()
{
	CloseMQ();
	Clear();

	m_arrTaskInfos.clear();
}

bool CScadaLogMQTask::InitMQ()
{
#if 0
	m_pCtx = zmq_ctx_new();
	Q_ASSERT(m_pCtx);
	if (m_pCtx == nullptr)
		return false;

	m_pReceiver = zmq_socket(m_pCtx, ZMQ_PULL);

	Q_ASSERT(m_pReceiver);
	if (m_pReceiver == nullptr)
		return false;

	//int nRet = zmq_bind(m_pRecver, "tcp://*:10002");
	int nRet = zmq_connect(m_pReceiver, "tcp://localhost:10002");
	Q_ASSERT(nRet == 0);
	if (nRet != 0)
		return false;
#endif
	QString szLog;

	auto it = m_arrTaskInfos.begin();
	for (; it != m_arrTaskInfos.end();++it)
	{
		(*it)->m_pCtx = zmq_ctx_new();
		Q_ASSERT((*it)->m_pCtx);
		if ((*it)->m_pCtx == nullptr)
		{
			continue;
		}

		(*it)->m_pReceiver = zmq_socket((*it)->m_pCtx, ZMQ_PULL);
		Q_ASSERT((*it)->m_pReceiver);
		if ((*it)->m_pReceiver == nullptr)
		{
			zmq_ctx_destroy((*it)->m_pCtx);
			(*it)->m_pCtx = nullptr;
			continue;
		}
		int nRet = zmq_connect((*it)->m_pReceiver, (*it)->GetAddr().toStdString().c_str());
		Q_ASSERT(nRet == 0);
		if (nRet != 0)
		{// 不成功
// 			if ((*it)->m_pReceiver)
// 			{
// 				zmq_close((*it)->m_pReceiver);
// 				(*it)->m_pReceiver = nullptr;
// 			}
// 
// 			if ((*it)->m_pCtx)
// 			{
// 				zmq_ctx_destroy((*it)->m_pCtx);
// 				(*it)->m_pCtx = nullptr;
// 			}
			szLog =   (QObject::tr("connect %1 failed")).arg((*it)->m_szLogName);
			LogString(szLog.toStdString().c_str(), 0);
			continue;
		}
		else
		{
			szLog = (QObject::tr("connect %1 success")).arg((*it)->m_szLogName);
			LogString(szLog.toStdString().c_str(), 0);
		} 
	}


	return true;
}

void CScadaLogMQTask::CloseMQ()
{
#if 0
	if (m_pReceiver)
	{
		zmq_close(m_pReceiver);
		m_pReceiver = nullptr;
	}
	if (m_pCtx)
	{
		zmq_ctx_destroy(m_pCtx);
		m_pCtx = nullptr;
	}
#endif
	auto it = m_arrTaskInfos.begin();
	for (; it != m_arrTaskInfos.end(); ++it)
	{
		if ((*it)->m_pReceiver)
		{
			zmq_close((*it)->m_pReceiver);
			(*it)->m_pReceiver = nullptr;
		}

		if ((*it)->m_pCtx)
		{
			zmq_ctx_destroy((*it)->m_pCtx);
			(*it)->m_pCtx = nullptr;
		}
	}
}

void CScadaLogMQTask::Clear()
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	//STLDeleteElements(&m_arrLogs);
	m_arrLogs.clear();
}

bool CScadaLogMQTask::RecvLogs()
{
	int nRet = 0;

	zmq_msg_t msg;
	nRet = zmq_msg_init(&msg);
	bool bRet = false;

	auto it = m_arrTaskInfos.begin();
	for (; it != m_arrTaskInfos.end(); ++it)
	{
		nRet = zmq_msg_init(&msg);
		nRet = zmq_msg_recv(&msg, (*it)->m_pReceiver, ZMQ_DONTWAIT);
		if (nRet == -1 && zmq_errno() == ETERM)
		{
			continue;
		}

		size_t nLen = zmq_msg_size(&msg);
		Q_ASSERT(nLen >= 0);
		if (nLen <= 0)
		{
			zmq_msg_close(&msg);
			continue;
		}
		std::shared_ptr<CScadaLog> pLog = std::make_shared<CScadaLog>();

		bRet = pLog->LoadFromBuffer(reinterpret_cast<const char *> (zmq_msg_data(&msg)), nLen);
		zmq_msg_close(&msg);

		Q_ASSERT(bRet);
		if (bRet == false)
		{
 			pLog.reset();
			continue;
		}

		if (pLog)
		{
			std::lock_guard<std::mutex> lock(m_LogLock);
			m_arrLogs.push_back(pLog);	
			bRet = true;
		}
	}

	return bRet;
}


bool CScadaLogMQTask::RecvScadaLog()
{
	if (m_pReceiver == nullptr)
	{
		return false;
	}

	int nRet = 0;

	zmq_msg_t msg;
	nRet = zmq_msg_init(&msg);

	nRet = zmq_msg_recv(&msg, m_pReceiver, ZMQ_DONTWAIT);

	if (nRet == -1 && zmq_errno() == ETERM)
	{
		return false;
	}

	bool bRet = false;

	size_t nLen = zmq_msg_size(&msg);

	Q_ASSERT(nLen >= 0);
	if (nLen <= 0)
	{
		zmq_msg_close(&msg);
		return false;
	}
//	CScadaLog *pLog = new CScadaLog;

	std::shared_ptr<CScadaLog> pLog = std::make_shared<CScadaLog>();
#if 0
	LOG_DATA data_pool;
	memset(&data_pool, 0, sizeof(data_pool));
	Q_ASSERT(nLen >= sizeof(LOG_DATA));
	
	unsigned char pTemp[4 * 1024] = { 0 };
//	memcpy(pTemp, zmq_msg_data(&msg), nLen);
//	memcpy(&data_pool, pTemp, sizeof(data_pool));
	memcpy(&data_pool, zmq_msg_data(&msg), std::min<size_t>(sizeof(data_pool),nLen));
//	bRet = pLog->LoadFromBuffer(reinterpret_cast<const char *> (zmq_msg_data(&msg)), nLen);
//	bRet = pLog->LoadFromBuffer(reinterpret_cast<const char *>(pTemp), nLen);
	bRet = pLog->LoadFromLogData(&data_pool);
#endif
	bRet = pLog->LoadFromBuffer(reinterpret_cast<const char *> (zmq_msg_data(&msg)), nLen);

	zmq_msg_close(&msg);

	Q_ASSERT(bRet);
	if (bRet == false)
	{
	//	delete pLog;
		pLog.reset();
	//	pLog = nullptr;

		return false;
	}

	if (pLog)
	{
		std::lock_guard<std::mutex> lock(m_LogLock);
		m_arrLogs.push_back(pLog);
		return true;
	}
	return false;
}

void CScadaLogMQTask::MainLoop(void *pIn)
{
	bool bRet = false;
	size_t nCount = 0;
	while (true)
	{
	//	bRet = RecvScadaLog();
		bRet = RecvLogs();
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

size_t CScadaLogMQTask::GetLogCount()const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CScadaLogMQTask::GetLogString(std::shared_ptr<CScadaLog>pMsg)
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pMsg = m_arrLogs.front();
	m_arrLogs.pop_front();
	return true;
}

#if 0
bool CScadaLogMQTask::GetLogString(CScadaLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	CScadaLog *pTempLog = nullptr;

	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pTempLog = m_arrLogs.front();
	Q_ASSERT(pTempLog);
	if (pTempLog)
	{
		*pMsg = *pTempLog;
		m_arrLogs.pop_front();
		delete pTempLog;
	}
	return true;
}
#endif

bool CScadaLogMQTask::GetLogs( std::deque<std::shared_ptr<CScadaLog>> & arrLogs)
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	arrLogs = m_arrLogs;
	
	m_arrLogs.clear();

	return true;
}
void  CScadaLogMQTask::LogString(const char *szLogTxt, int nLevel)
{
	CLogImpl * pImpl = CLogImpl::GetInstance();
	if (pImpl)
	{
		pImpl->LogString("MAIN", szLogTxt, nLevel);
	}
}

void CScadaLogMQTask::StartLogMQ()
{
	// 加载配置文件
	bool bRet = false;
	if (m_pConfigs)
	{
		m_pConfigs->LoadXml();
	}

	// 根据配置文件连接远程服务
	auto arrConfigs = m_pConfigs->GetConfigs();
	if (arrConfigs.empty() == true)
	{//需要log下
		LogString("warning: log.xml no item", 0);
		return;
	}
	unsigned int nPortNo = 0;
	auto it = arrConfigs.cbegin();
	for (; it != arrConfigs.cend();++it)
	{
		nPortNo = (*it)->GetLogTaskPortNo();
		if (nPortNo >0)
		{
			auto pTempTaskInfo = GetTaskInfo(nPortNo);
			if (pTempTaskInfo == nullptr)
			{
				std::shared_ptr<CMQTaskInfo> pTaskInfo = std::make_shared<CMQTaskInfo>(nPortNo, (*it)->GetLogName());
				m_arrTaskInfos.push_back(pTaskInfo);
			}
			else
			{
				Q_ASSERT(false);
			}
		}
	}

	if (arrConfigs.empty() == false)
	{
		InitMQ();
		StartModule();
	}
}

/** @}*/
