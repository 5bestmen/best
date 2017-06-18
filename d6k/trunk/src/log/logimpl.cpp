#include "logimpl.h"

#include "queue_impl.h"

#include "public.h"

#include "stl_util-inl.h"

#include "write_log_task.h"
#include "send_log_task.h"

#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile> 
 

CLogImpl * CLogImpl::GetInstance()
{
	return Singleton<CLogImpl, DefaultSingletonTraits<CLogImpl>>::instance();
}

CLogImpl::CLogImpl()
{
	m_bTaskIsStarted = false;
	m_nMaxIndex = 0;
	m_pWriteFileTask = std::make_shared<CWriteLogTask>(m_LogConfigs);
	m_pSendLogTask = std::make_shared<CSendLogTask>();
}

CLogImpl::~CLogImpl()
{

} 
/*! \fn bool  CLogImpl::CreateLog(const char* szLogName)
********************************************************************************************************* 
** \brief CLogImpl::CreateLog 
** \details  创建log服务器，
** \param szLogName 
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
bool  CLogImpl::CreateLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;
	
	m_LogConfigs.LoadXml( );

	QString szLog = QString(QObject::tr("Start [%1] log ......")).arg(szLogName);

	LogMsg("LOG", szLog.toStdString().c_str(), 0);

	QThread::sleep(2);

	// 防止启动多遍，尤其是在SCADA和FES运行同一进程空间内的情况时
// 	if (m_bTaskIsStarted == false)
// 	{
// 		m_bTaskIsStarted = true;
// 	}
// 	else
// 	{
// 		return true;
// 	}

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

bool CLogImpl::DestroyLog(const char* szLogName)
{
	Q_UNUSED(szLogName);

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->StopModule();
	}
	m_pWriteFileTask.reset();
	if (m_pSendLogTask)
	{
		m_pSendLogTask->StopModule();
	}
	m_pSendLogTask.reset();
	return true;
}

bool  CLogImpl::OpenLog(const char* szLogName)
{
	Q_ASSERT(szLogName);
	if (szLogName == nullptr)
		return false;
	Q_ASSERT(strlen(szLogName) > 0);
	if (strlen(szLogName) == 0)
		return false;

	QString szQueName = QString(szLogName) + QString("_LOG");

// 	if (m_pQue == nullptr)
// 	{
// 		m_pQue = std::make_shared<CQueue>();
// 	}
// 
// 	// 如果队列没有打开过
// 	if (m_pQue->IsOpened() == false)
// 	{
// 		bool bRet = m_pQue->Open(szQueName.toStdString().c_str());
// 		Q_ASSERT(bRet);
// 		if (bRet == false)
// 		{
// 			return bRet;
// 		}
// 	}
	// 启动后台IPC转发
//	StartClientTask();
	return true;
}

bool  CLogImpl::CloseLog(const char* szLogName)
{
	Q_UNUSED(szLogName);
// 	StopClientTask();
// 	
// 	if (m_pQue)
// 	{
// 		if (m_pQue->IsOpened())
// 		{
// 			m_pQue->Close();
// 		}		
// 	}

	return true;
}

void  CLogImpl::AddLogMsg(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	if (m_pWriteFileTask)
	{
		m_pWriteFileTask->AddLogMsg(pMsg);
	}
	if (m_pSendLogTask)
	{
		m_pSendLogTask->AddLogMsg(pMsg);
	}
	delete pMsg;
	pMsg = nullptr;
}

bool CLogImpl::LogMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const QDateTime & tm)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;

	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	Q_ASSERT(szLogTxt != nullptr);
	if (szLogTxt == nullptr)
		return false;
	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	std::string szModuleName;
	if (pszMoudleName == nullptr)
	{
		szModuleName = "Unknown";
	}
	else
	{
		if (strlen(pszMoudleName) == 0)
		{
			szModuleName = "Unknown";
		}
		else
		{
			szModuleName = pszMoudleName;
		}
	}
	m_nMaxIndex++;
	CMsgLog *pMsg = new CMsgLog(szModuleName, szLogTxt, nLevel, m_nMaxIndex,tm);

	AddLogMsg(pMsg);

	return true;
}

bool CLogImpl::LogMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel)
{
	QDateTime  tm = QDateTime::currentDateTime();
	return LogMsg(pszMoudleName, szLogTxt, nLevel, tm);
}

bool  CLogImpl::LogDebugMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc, const QDateTime & tm)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;

	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	Q_ASSERT(szLogTxt != nullptr);
	if (szLogTxt == nullptr)
		return false;
	Q_ASSERT(strlen(szLogTxt) > 0);
	if (strlen(szLogTxt) == 0)
		return false;

	std::string szModuleName;
	if (pszMoudleName == nullptr)
	{
		szModuleName = "Unknown";
	}
	else
	{
		if (strlen(pszMoudleName) == 0)
		{
			szModuleName = "Unknown";
		}
		else
		{
			szModuleName = pszMoudleName;
		}
	}
	m_nMaxIndex++;
	CMsgLog *pMsg = new CMsgLog(szModuleName, szLogTxt, nLevel, m_nMaxIndex, szFile, nLine, szFunc,tm);

	AddLogMsg(pMsg);

	return true;
}

bool  CLogImpl::LogDebugMsg(const char* pszMoudleName, const char* szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc)
{
	QDateTime  tm = QDateTime::currentDateTime();
	return LogDebugMsg(pszMoudleName, szLogTxt, nLevel, szFile, nLine, szFunc, tm);
}
#if 0
/*! \fn void CLogImpl::MainTask(void *pImpl)
********************************************************************************************************* 
** \brief CLogImpl::MainTask 
** \details 
** \param pImpl 
** \return void 
** \author LiJin 
** \date 2016年9月28日 
** \note 注意：主进程模式与子模块模式不一样，主进程要写文件，子模块不写文件，只做IPC转发
********************************************************************************************************/
void CLogImpl::MainTask(void *pImpl)
{
//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	if (m_szLogName.isEmpty() == true)
	{
		m_szLogName = QCoreApplication::applicationName();
	}
	m_bStopFlag = false;

	LogMsg("LOG", QObject::tr("Start log task.").toStdString().c_str(), 0);

	std::chrono::milliseconds dura(1000);
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CLogImpl::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
//			LogString("exit main task1....", 2);
			return;
		}
	
		// 收IPC
		ReadLogFromQue();
	}
} 

void CLogImpl::ClientTask(void *pImpl)
{
	qDebug() << "start log client task....";
	if (m_szLogName.isEmpty() == true)
	{
		m_szLogName = QCoreApplication::applicationName();
	}
	m_bStopFlag = false;

	LogMsg("LOG", QObject::tr("Start log client task.").toStdString().c_str(), 0);

	std::chrono::milliseconds dura(100);
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CLogImpl::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit client task....";
//			LogString("exit main task1....", 2);
			return;
		}
		LogMsg("LOG", QObject::tr("Start log client task. 111").toStdString().c_str(), 0);

		std::this_thread::sleep_for(dura);
		WriteLogToQue(); 
	}
}


void  CLogImpl::ReadLogFromQue()
{ 
	char buff[MAX_BUFF_SIZE * 2] = { 0 };
	
	if (m_pQue == nullptr)
	{
		Q_ASSERT(false);
		return;
	}
	size_t   nRecvdSize = 0;
	unsigned int   nPriority = 0;
	bool bRet = false;
	CMsgLog *pLog;

	while (true)
	{
		bRet = m_pQue->Receive(reinterpret_cast<unsigned char*>(buff), MAX_BUFF_SIZE, nRecvdSize, nPriority, 50);
		if (bRet == false)
		{
			break;
		}
		Q_ASSERT(nRecvdSize > 0);
		pLog = new CMsgLog;

		bRet = pLog->LoadData(buff, MAX_BUFF_SIZE);
		if (bRet)
		{// 插入到队列中
			AddLogMsg(pLog);
		}
		else
		{
			delete pLog;
		}	 
	}
}

void  CLogImpl::WriteLogToQue()
{
	size_t nCnt = GetLogCount();
	unsigned char buff[MAX_BUFF_SIZE * 2] = { 0 };
	size_t nRet = 0;
	bool bRet = false;
	CMsgLog log;

	if (nCnt)
	{
		while (nCnt)
		{
			bRet = GetLogString(&log);
			if (bRet)
			{
				nRet = log.SaveData(buff, MAX_BUFF_SIZE);
				Q_ASSERT(nRet >= 0 && nRet <= MAX_BUFF_SIZE);
				if (m_pQue->IsOpened())
				{
					if (nRet > 0 && nRet < MAX_BUFF_SIZE)
					{
						bRet = m_pQue->Send(reinterpret_cast<const unsigned char*> (buff), nRet, 0, 100);
						if (bRet == false)
						{
							qDebug() << QObject::tr("write log err...");
						}
					}					
				}
				else
				{
					qDebug() << QObject::tr("Queue open failed,write log err...");
				}
			}
			nCnt = GetLogCount();
		}
	}
}
#endif

/** @}*/
