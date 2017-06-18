
#include "base_module.h"
 
#include "log/log.h"

#include <QDebug>

CBaseModule::CBaseModule( )  
{ 
	m_bStopFlag = false;
}


CBaseModule::~CBaseModule(void)
{

}

void CBaseModule::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("RT_GRAPH", szLogTxt, nLevel, nullptr);
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
		MainLoop(pImpl);
	}
}