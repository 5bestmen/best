/*! @file write_log_task.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  write_log_task.cpp
文件实现功能 :  记录到文件中
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

#include "comm_msglog.h"
#include "write_log_task.h"
#include "stl_util-inl.h"
 
#include <QProcess>
#include <QThread>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QFile> 


CWriteLogTask::CWriteLogTask(CLogConfigMgr & conf) :m_LogConfig(conf)
{
	m_bIsStarted = false;
}

CWriteLogTask::~CWriteLogTask()
{
	StopModule();
	Clear();
	m_bIsStarted = false;
}

void CWriteLogTask::Clear()
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.clear();
}

void  CWriteLogTask::AddLogMsg(CCommMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	std::shared_ptr<CCommMsgLog> pMsgLog = std::make_shared<CCommMsgLog>(pMsg);

	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.push_back(pMsgLog);
}

size_t CWriteLogTask::GetLogCount() const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CWriteLogTask::GetLogString(CCommMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return false;

	std::shared_ptr<CCommMsgLog> pTempLog = nullptr;

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

bool  CWriteLogTask::TakeMsgLog(std::shared_ptr<CCommMsgLog>&pLog)
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	if (m_arrLogs.empty() == true)
		return false;

	pLog = m_arrLogs.front();
	m_arrLogs.pop_front();
	return true;
}

void CWriteLogTask::WriteLogTxtFile(std::shared_ptr<CModuleLogConfig> pModuleConf, const char * pszPath, bool bChanged)
{
	Q_ASSERT(pModuleConf);
	if (pModuleConf == nullptr)
		return;
	
	Q_ASSERT(pszPath);
	if (pszPath == nullptr)
		return;

	Q_ASSERT(strlen(pszPath) > 0);
	if (strlen(pszPath) == 0)
	{
		return;
	}
	// 检查文件是否超过长度 如果超过长度，则切换
// 	unsigned int nMaxFileCount = pModuleConf->GetFileNumber();
// 	if (pModuleConf->GetCurrentFileNumber() > nMaxFileCount || pModuleConf->GetCurrentFileNumber() == 0)
// 	{
// 		pModuleConf->SetCurrentFileNumber(1);
// 	}

	QString szFileName = pModuleConf->GetLogName() + QString("_%1.log").arg(pModuleConf->GetCurrentFileNumber());
	QFileInfo logFile(m_szLogPath + szFileName);

	if (logFile.exists() == true)
	{// 文件存在		 
		size_t nSize = logFile.size();
		if (nSize > std::size_t(pModuleConf->GetFileLength()) * 1024 * 1024)
		{
			if (bChanged == false)
			{
				QString szLog = "log [ " + m_szLogPath + szFileName + " ]  too large.";
				qDebug() << szLog;
				// 文件过大，切换     
				pModuleConf->IncreaseCurrentFileNumber();
				WriteLogTxtFile(pModuleConf,pszPath, true);
			}
			else
			{ // 覆盖写入
				WriteLogTxtFileImpl(pModuleConf,szFileName, true);
			}
		}
		else
		{
			// 取出所有内容，一次性写入
			WriteLogTxtFileImpl(pModuleConf,szFileName, false);
		}
	}
	else
	{// 文件不存在，直接覆盖式写入
		// 取出所有内容，一次性写入
		WriteLogTxtFileImpl(pModuleConf,szFileName, true);
	}
}
/*! \fn void  CLogImpl::WriteLogTxtFileImpl(const QString & szFileName,bool bOverWrite)
*********************************************************************************************************
** \brief CLogImpl::WriteLogTxtFileImpl
** \details 批量写入
** \param szFileName
** \param bOverWrite
** \return void
** \author LiJin
** \date 2016年9月10日
** \note
********************************************************************************************************/
void  CWriteLogTask::WriteLogTxtFileImpl(const std::shared_ptr<CModuleLogConfig> pModuleConf, const QString & szFileName, bool bOverWrite)
{
	QFile data(m_szLogPath + szFileName);

	QFile::OpenMode om;
	if (bOverWrite == true)
	{
		om = QFile::WriteOnly | QIODevice::Text | QIODevice::Truncate;
	}
	else
	{
		om = QFile::ReadWrite | QIODevice::Text | QIODevice::Append;
	}

	if (data.open(om))
	{
		QTextStream out(&data);
		size_t nCnt = GetLogCount();

		bool bRet = false;
		//CMsgLog log;
		std::shared_ptr<CCommMsgLog> pLog;
		if (nCnt)
		{
			while (nCnt)
			{
				bRet = TakeMsgLog(pLog);
				if (bRet && pLog)
				{
					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(8);
					out << pLog->m_nIndex;

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(20);
					out << QString(pLog->m_szModuleName.c_str());

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(28);
					out << pLog->m_tmLog.toString("yyyy-MM-dd  HH:mm:ss.zzz");
#if 0
					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(100);
					out << QString(pLog->m_szLogText.c_str());

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(4);
					out << pLog->m_nLevel;

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(6);
					out << pLog->m_nProcessId;

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(6);
					out << pLog->m_nThreadId;

					if (pLog->m_szFileName.length())
					{
						out.setFieldAlignment(QTextStream::AlignLeft);
						out.setFieldWidth(20);
						out << QString(pLog->m_szFileName.c_str());

						out.setFieldAlignment(QTextStream::AlignLeft);
						out.setFieldWidth(18);
						out << QString("Line:%1").arg(pLog->m_nLine);

						out.setFieldAlignment(QTextStream::AlignLeft);
						out.setFieldWidth(40);
						out << QString(pLog->m_szFuncName.c_str());
					}
#endif
					out << qSetFieldWidth(0) << endl;
				}
				nCnt = GetLogCount();
			}
		}

		//out.flush();
		data.flush();
		data.close();
	}
}

void CWriteLogTask::InitFilePath()
{
	// 确定log目录，如果在根目录下，则log目录在当前路径的子目录，如果不在根，则在上一级
	QString szRunPath = QCoreApplication::applicationDirPath();
	//	QString szCurrentPath = QCoreApplication::applicationFilePath();
	QString szPath = QFileInfo(szRunPath).absoluteFilePath();

	if (QFileInfo(szPath).isRoot())
	{
		m_szLogPath = QFileInfo(szPath + "/log").absolutePath();
	}
	else
	{
		m_szLogPath = QFileInfo(szPath).absolutePath();
		m_szLogPath += "/log/";
	}

	QDir drLog(m_szLogPath);
	if (drLog.exists() == false)
	{
		drLog.mkdir("log");
	}
}

void CWriteLogTask::MainLoop()
{
	// 重新加载配置文件，看看配置文件内容是否有变化
	m_LogConfig.LoadXml();

	auto arrConfigs = m_LogConfig.GetConfigs();
	if (arrConfigs.empty() == true)
	{
		QString szMyProcessName = QCoreApplication::applicationName();
		if (szMyProcessName.isEmpty())
		{
			szMyProcessName = "nonamed_log";
		}
	 
		std::shared_ptr<CModuleLogConfig> pTemp = std::make_shared<CModuleLogConfig>(szMyProcessName);
		WriteLogTxtFile(pTemp, m_szLogPath.toStdString().c_str());
	}
	else
	{
		// TODO: 今后优化下：定时1秒检查，如果缓冲区多或者超时5秒 写库一次。
		for (auto it = arrConfigs.cbegin(); it != arrConfigs.cend(); ++it)
		{
			if ((*it)->IsWriteFileEnabled() == true && (*it)->IsStarted() == true)
			{
				WriteLogTxtFile(*it, m_szLogPath.toStdString().c_str());
			}
		}
	}
}

/*! \fn void  CWriteLogTask::StartLogTask(const QString &szName)
********************************************************************************************************* 
** \brief CWriteLogTask::StartLogTask 
** \details 启动写日志任务
** \param szName LOG的名称，比如FES、SCADA等等
** \return void 
** \author LiJin 
** \date 2017年1月7日 
** \note 
********************************************************************************************************/
void  CWriteLogTask::StartLogTask(const QString &szName)
{
	m_LogConfig.StartModuleWriteFile(szName);
	// 记录文件任务是否已经启动过，如果启动过，则不再启动
	if (m_bIsStarted == false)
	{
		m_bIsStarted = true;

		StartModule();
	}
}
/** @}*/
