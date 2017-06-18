
#ifndef LOG_CONFIG_H
#define LOG_CONFIG_H

//#include "singleton.h"

//#include "msglog.h"

#include <string>
#include <QDateTime>
#include <deque>
#include <vector>

#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  
#include <atomic>
#include <cassert>

#define MAX_BUFF_SIZE 1024
#define MAX_MSG_NUM 1024 

class CModuleLogConfig
{
public:
	CModuleLogConfig()
	{
		m_bWriteFile = true;
		m_nLogFormat = LOG_TXT;
		m_nFileNumber = 3;
		m_nFileLength = MAX_LOGFILE_LENGTH;
		m_nWriteLogLevel = 0;

		m_nCurrentFileNumber = 0;
		m_bIsStarted = false;
	}
	CModuleLogConfig(const QString & szName)
	{
		m_bWriteFile = true;
		m_nLogFormat = LOG_TXT;
		m_nFileNumber = 3;
		m_nFileLength = MAX_LOGFILE_LENGTH;
		m_nWriteLogLevel = 0;

		m_nCurrentFileNumber = 0;
		m_bIsStarted = false;

		m_szLogName = szName;
	}
	~CModuleLogConfig()
	{

	}
	enum LOG_FORMAT
	{
		LOG_TXT = 0,
		LOG_SQLITE = 1,
	};

	enum
	{
		MAX_LOGFILE_NUMBER = 10,
		MAX_LOGFILE_LENGTH = 10, //!< 50mb
	};
public:
	unsigned int GetFileNumber() const
	{
		return m_nFileNumber;
	}
	unsigned int GetFileLength() const
	{
		return m_nFileLength;
	}

	bool IsWriteFileEnabled() const
	{
		return m_bWriteFile;
	}

	unsigned int GetWriteLogLevel() const
	{
		return m_nWriteLogLevel;
	}

	void SetLogName(const QString & szLogName)
	{
		m_szLogName = szLogName;
	}

	const QString &GetLogName()const
	{
		return m_szLogName;
	}

	unsigned int GetCurrentFileNumber()  
	{
		if (m_nCurrentFileNumber > m_nFileNumber || m_nCurrentFileNumber == 0)
		{
			m_nCurrentFileNumber = 1;
		}

		return m_nCurrentFileNumber;
	}

	void SetCurrentFileNumber(unsigned int nIdx)  
	{
		m_nCurrentFileNumber = nIdx;
	}

	void IncreaseCurrentFileNumber()
	{
		m_nCurrentFileNumber++;
	}

	bool IsStarted() const
	{
		return m_bIsStarted;
	}

	void SetStarted()
	{
		m_bIsStarted = true;
	}

	void SetWriteFileFlag(bool bVal)
	{
		m_bWriteFile = bVal;
	}

	void SetLogFormat(LOG_FORMAT nVal)
	{
		m_nLogFormat = nVal;
	}
	
	void SetFileNumber(int nNum)
	{
		if (nNum > 0 && nNum <= MAX_LOGFILE_NUMBER)
		{
			m_nFileNumber = static_cast<unsigned int>(nNum);
		}
		else
		{
			m_nFileNumber = 3;
		}
	}

	void SetFileLength(int nLen)
	{
		if (nLen > 0 && nLen <= MAX_LOGFILE_LENGTH)
		{
			m_nFileLength = static_cast<unsigned int>(nLen);
		}
		else
		{
			m_nFileLength = MAX_LOGFILE_LENGTH;
		}
	}

	void SetWriteLogLevel(int nLevel)
	{
		m_nWriteLogLevel = static_cast<unsigned int>(nLevel);
	}

	unsigned int GetLogTaskPortNo()
	{
		return m_nPortNo;
	}

	void SetLogTaskPortNo(unsigned int nPortNo)
	{
		 m_nPortNo = nPortNo;
	}
private:
	bool         m_bWriteFile;   //! 是否记录文件 
	unsigned int m_nLogFormat;   //! 文件格式：txt，或者sqlite
	unsigned int m_nFileNumber;  //! 循环读写的文件个数，一般1-5
	unsigned int m_nFileLength;  //! 文件长度 MB为单位
	unsigned int m_nWriteLogLevel;//! 为0即表示所有的都存储到文件中

	unsigned int  m_nPortNo;      //! 端口号
	
	unsigned int m_nCurrentFileNumber; //! 当前正在写的顺序号，运行时用

	std::atomic<bool> m_bIsStarted;   //! 是否已经启动记录到文件中

	QString  m_szLogName;       //! 宿主模块名称
};


class CLogConfigMgr
{
public:
	CLogConfigMgr()
	{

	}
	~CLogConfigMgr()
	{
		m_arrLogConfigs.clear();
	}
public:
	// 加载配置文件
	void LoadXml();

	const std::vector<std::shared_ptr<CModuleLogConfig>> & GetConfigs()const
	{
		std::lock_guard<std::mutex> lock(m_Lock);
		return m_arrLogConfigs;
	}

	std::shared_ptr<CModuleLogConfig>  AddNewModule(const QString & szName)
	{
		assert(szName.isEmpty() == false);
		if (szName.isEmpty())
			return nullptr;

		std::lock_guard<std::mutex> lock(m_Lock);
		// 找到重名
		auto it = m_arrLogConfigs.cbegin();
		for (; it != m_arrLogConfigs.cend(); ++it)
		{
			if ((*it)->GetLogName().compare(szName, Qt::CaseInsensitive) == 0)
			{			
				assert(false);
				return nullptr;
			}
		}

		std::shared_ptr<CModuleLogConfig> pNew = std::make_shared<CModuleLogConfig>(szName);
		m_arrLogConfigs.push_back(pNew);
		return pNew;
	}

	void StartModuleWriteFile(const QString & szName)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		auto it = m_arrLogConfigs.cbegin();
		for (; it != m_arrLogConfigs.cend(); ++it)
		{
			if ((*it)->GetLogName().compare(szName, Qt::CaseInsensitive) == 0)
			{
				(*it)->SetStarted();
				return;
			}
		}
		assert(false);		 
	}

	std::shared_ptr<CModuleLogConfig> GetModule(const QString & szModuleName)
	{
		auto it = m_arrLogConfigs.cbegin();

		for (; it != m_arrLogConfigs.cend(); ++it)
		{
			if ((*it)->GetLogName().compare(szModuleName, Qt::CaseInsensitive) == 0)
			{
				return *it;
			}
		}
		return nullptr;
	}
private:
	std::vector<std::shared_ptr<CModuleLogConfig>> m_arrLogConfigs;

	mutable std::mutex m_Lock;
};
 
 
#endif // LOG_CONFIG_H

