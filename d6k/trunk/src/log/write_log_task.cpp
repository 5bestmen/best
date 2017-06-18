/*! @file write_log_task.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  write_log_task.cpp
�ļ�ʵ�ֹ��� :  ��¼���ļ���
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

#include "msglog.h"
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

void  CWriteLogTask::AddLogMsg(CMsgLog *pMsg)
{
	Q_ASSERT(pMsg);
	if (pMsg == nullptr)
		return;

	std::shared_ptr<CMsgLog> pMsgLog = std::make_shared<CMsgLog>(pMsg);

	std::lock_guard<std::mutex> lock(m_LogLock);
	m_arrLogs.push_back(pMsgLog);
}

size_t CWriteLogTask::GetLogCount() const
{
	std::lock_guard<std::mutex> lock(m_LogLock);
	return	m_arrLogs.size();
}

bool CWriteLogTask::GetLogString(CMsgLog *pMsg)
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

bool  CWriteLogTask::TakeMsgLog(std::shared_ptr<CMsgLog>&pLog)
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
	// ����ļ��Ƿ񳬹����� ����������ȣ����л�
// 	unsigned int nMaxFileCount = pModuleConf->GetFileNumber();
// 	if (pModuleConf->GetCurrentFileNumber() > nMaxFileCount || pModuleConf->GetCurrentFileNumber() == 0)
// 	{
// 		pModuleConf->SetCurrentFileNumber(1);
// 	}

	QString szFileName = pModuleConf->GetLogName() + QString("_%1.log").arg(pModuleConf->GetCurrentFileNumber());
	QFileInfo logFile(m_szLogPath + szFileName);

	if (logFile.exists() == true)
	{// �ļ�����		 
		size_t nSize = logFile.size();
		if (nSize > std::size_t(pModuleConf->GetFileLength()) * 1024 * 1024)
		{
			if (bChanged == false)
			{
				QString szLog = "log [ " + m_szLogPath + szFileName + " ]  too large.";
				qDebug() << szLog;
				// �ļ������л�     
				pModuleConf->IncreaseCurrentFileNumber();
				WriteLogTxtFile(pModuleConf,pszPath, true);
			}
			else
			{ // ����д��
				WriteLogTxtFileImpl(pModuleConf,szFileName, true);
			}
		}
		else
		{
			// ȡ���������ݣ�һ����д��
			WriteLogTxtFileImpl(pModuleConf,szFileName, false);
		}
	}
	else
	{// �ļ������ڣ�ֱ�Ӹ���ʽд��
		// ȡ���������ݣ�һ����д��
		WriteLogTxtFileImpl(pModuleConf,szFileName, true);
	}
}
/*! \fn void  CLogImpl::WriteLogTxtFileImpl(const QString & szFileName,bool bOverWrite)
*********************************************************************************************************
** \brief CLogImpl::WriteLogTxtFileImpl
** \details ����д��
** \param szFileName
** \param bOverWrite
** \return void
** \author LiJin
** \date 2016��9��10��
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
		std::shared_ptr<CMsgLog> pLog;
		if (nCnt)
		{
			while (nCnt)
			{
				bRet = TakeMsgLog(pLog);
				if (bRet && pLog)
				{
					// �����־�ȼ�����Ҫд�ļ��ĵȼ��ߣ������������洢
					if (pLog->m_nLevel < pModuleConf->GetWriteLogLevel())
					{
						nCnt = GetLogCount();
						continue;
					}

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(8);
					out << pLog->m_nIndex;

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(20);
					out << QString(pLog->m_szModuleName.c_str());

					out.setFieldAlignment(QTextStream::AlignLeft);
					out.setFieldWidth(28);
					out << pLog->m_tmLog.toString("yyyy-MM-dd  HH:mm:ss.zzz");

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
	// ȷ��logĿ¼������ڸ�Ŀ¼�£���logĿ¼�ڵ�ǰ·������Ŀ¼��������ڸ���������һ��
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
		drLog.mkdir(m_szLogPath);
	}
}

void CWriteLogTask::MainLoop()
{
	// ���¼��������ļ������������ļ������Ƿ��б仯
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
		// TODO: ����Ż��£���ʱ1���飬�������������߳�ʱ5�� д��һ�Ρ�
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
** \details ����д��־����
** \param szName LOG�����ƣ�����FES��SCADA�ȵ�
** \return void 
** \author LiJin 
** \date 2017��1��7�� 
** \note 
********************************************************************************************************/
void  CWriteLogTask::StartLogTask(const QString &szName)
{
	m_LogConfig.StartModuleWriteFile(szName);
	// ��¼�ļ������Ƿ��Ѿ����������������������������
	if (m_bIsStarted == false)
	{
		m_bIsStarted = true;
	
		StartModule();
	}
}
/** @}*/
