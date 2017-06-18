
#include "commlog_impl.h"
#include "log/comm_log.h"

#include "write_log_task.h"
#include "send_log_task.h"
#include "comm_msglog.h"

#include "stl_util-inl.h"
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile>

CCommLogImpl * CCommLogImpl::GetInstance()
{
	return Singleton<CCommLogImpl, DefaultSingletonTraits<CCommLogImpl>>::instance();
}

CCommLogImpl::CCommLogImpl()
{
	m_bTaskIsStarted = false;
	m_nMaxIndex = 0;
	m_pWriteFileTask = std::make_shared<CWriteLogTask>(m_LogConfigs);
	m_pSendLogTask = std::make_shared<CSendLogTask>();
}

CCommLogImpl::~CCommLogImpl()
{

}

bool CCommLogImpl::CommMsgLog(struct COMM_MSG *pMsgLog)
{
	Q_ASSERT(pMsgLog);
	if (pMsgLog == nullptr)
		return false;	
	
	m_nMaxIndex++;
	CCommMsgLog *pMsg = new CCommMsgLog(pMsgLog);

	AddCommMsg(pMsg);

	return true;
}
/*! \fn bool CCommLogImpl::CreateLog(const char* szLogName)
********************************************************************************************************* 
** \brief CCommLogImpl::CreateLog 
** \details 
** \param szLogName 
** \return bool 
** \author LiJin 
** \date 2017年1月28日 
** \note 
********************************************************************************************************/
bool CCommLogImpl::CreateLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;

	m_LogConfigs.LoadXml();

	// 启动写文件任务以及发送给监视器的任务
	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->InitFilePath();
		m_pWriteFileTask->StartLogTask(szLogName);
	}
	if (m_pSendLogTask)
	{
		auto pModule = m_LogConfigs.GetModule(szLogName);
		if (pModule == nullptr)
		{
			m_pSendLogTask->StartLogTask(0);
		}
		else
		{
			m_pSendLogTask->StartLogTask(pModule->GetLogTaskPortNo());
		}
	}

	return true;
}

bool CCommLogImpl::DestroyLog(const char* szLogName)
{
	Q_UNUSED(szLogName);

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->StopModule();
	}
	if (m_pSendLogTask)
	{
		m_pSendLogTask->StopModule();
	}

	m_pWriteFileTask.reset();
	m_pSendLogTask.reset();

	return true;
}

bool CCommLogImpl::OpenLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;


	return true;
}

bool CCommLogImpl::CloseLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;


	return true;
}

void  CCommLogImpl::AddCommMsg(CCommMsgLog *pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->AddLogMsg(pLog);
	}
	if (m_pSendLogTask)
	{
		m_pSendLogTask->AddLogMsg(pLog);
	}

	delete pLog;
	pLog = nullptr;
}
