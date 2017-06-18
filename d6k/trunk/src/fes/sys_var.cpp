/*! @file sys_var.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sys_var.cpp
�ļ�ʵ�ֹ��� :  ϵͳ��������ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ��������ģ��
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.31
*******************************************************************************/
#include "sys_var.h"
#include "log/log.h"
#include "fesapi/fesdatadef.h"

#include <QString> 
#include <QDebug> 
#include <array>


std::array<CSysVarSvc::SysConfig, CSysVarSvc::$SV_MAX>  CSysVarSvc::s_arrSysVars =
{
	"$CPU", 0, DT_DWORD, IDD_AIN,
	"$YEAR", 0, DT_DWORD, IDD_AIN,
	"$DAY", 0, DT_DWORD, IDD_AIN,
	"$MONTH", 0, DT_DWORD, IDD_AIN,
	"$HOUR", 0, DT_DWORD, IDD_AIN,
	"$MINUTE", 0, DT_DWORD, IDD_AIN,
};


CSysVarSvc::CSysVarSvc(CDbSvc* pServer) :m_pDBSvc(pServer)
{
	Q_ASSERT(pServer); 
	m_bStopFlag = false;
}

CSysVarSvc::~CSysVarSvc(void)
{
	
}

/*! \fn bool CSysVarSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CSysVarSvc::Initialize 
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
** \note  
********************************************************************************************************/
bool CSysVarSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
	// ���ҵ�����ϵͳ������OccNo
 

	return true;
}

void CSysVarSvc::Run()
{
	StartModule(); 
}

void CSysVarSvc::Shutdown()
{
	StopModule();
}

void CSysVarSvc::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("FES", szLogTxt, nLevel, nullptr);
}

void CSysVarSvc::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}

void CSysVarSvc::StartModule()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CSysVarSvc::MainTask, this, nullptr));
 	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CSysVarSvc::MainTask, this, nullptr);
	}
}

void CSysVarSvc::StopModule()
{
	SetTaskStopFlag();
	// �˳��߳�
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
}

void CSysVarSvc::MainTask(void *pImpl)
{
	//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CSysVarSvc::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
			//			LogString("exit main task1....", 2);
			return;
		}

 

		//DoSth();
		MainLoop();
	}
}

void CSysVarSvc::MainLoop()
{

}

void CSysVarSvc::DateTimeHandler()
{

}

/** @}*/
