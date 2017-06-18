
#include "base_module.h"

//#include "fesapi/fesapi.h"
// #include "mail/mail.h"
#include "log/log.h"

#include <QDebug>
#include <chrono>  

CBaseModule::CBaseModule(CFesSvc* pServer, std::string szMailBoxName, int &nMailID) :m_nMailBoxID(nMailID)
{
	m_pServer = pServer;
	m_szMailBoxName = szMailBoxName;
	m_nMailBoxID = 0;

	m_bStopFlag = false;
}


CBaseModule::~CBaseModule(void)
{

}

void CBaseModule::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("FES", szLogTxt, nLevel, nullptr);
}   

void CBaseModule::LogDebugMsg(const char *szLogTxt, int nLevel, const char * szFile, int nLine, const char *szFunc)
{
	::LogDebugMsg("FES", szLogTxt, nLevel, szFile, nLine, szFunc, nullptr);
}

void CBaseModule::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}

void CBaseModule::StartModule()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CBaseModule::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CBaseModule::MainTask, this, nullptr);
	}
}

void CBaseModule::StopModule()
{
	SetTaskStopFlag();
	// ÍË³öÏß³Ì
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
}

void CBaseModule::MainTask(void *pImpl)
{
	//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CBaseModule::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
			//			LogString("exit main task1....", 2);
			return;
		}

		// 		if (bRet != std::cv_status::timeout)
		// 		{
		// 			LogString("rly103", "exit main task....", 2);
		// 			return;
		// 		}
		//		LogString("rly103", "in main task....", 2);

		//DoSth();
		MainLoop();
	}
}

void CBaseModule::MainLoop()
{

}