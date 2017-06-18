#include "scdsvc.h"
#include "dbg_svc.h"
#include "db_svc.h"
#include "nb_svc.h"

#include "mail/mail.h"
#include "audio_alarm_svc.h"
#include <QDebug>
#include <array>

struct MailBoxConf
{
	char * Name;  //! 邮箱名称
	int ID;       //! 邮箱ID
	int MailNum; //! 邮箱容量 
};

static std::array<MailBoxConf, 9> s_arrMailBoxs =
{
	"DBG_SVC", 0, 100, 
	"DB_SVC", 0, 100, 
	"NB_SVC", 0, 100, 
	"AUDIO_ALARM_SVC", 0, 100,
	"ALARM_WIN", 0, 1000, 
	"ALARM_INGRAPH", 0, 1000, 
	"ALARM_SCD_WIN",0,1000,
	"ALARM_SCD_INGRAPH",0,1000,
	"LOGIC_SVC", 0, 100, 
};

CScadaSvc::CScadaSvc()
{
	m_pDbgSvc = std::make_shared<CDbgSvc>(this, s_arrMailBoxs[0].Name, s_arrMailBoxs[0].ID);

	m_pDbSvc = std::make_shared<CDbSvc>(this, s_arrMailBoxs[1].Name, s_arrMailBoxs[1].ID);

	m_pNetBusSvc = std::make_shared<CNetbusSvc>(this, s_arrMailBoxs[2].Name, s_arrMailBoxs[2].ID);

	m_pAudioAlarmSvc = std::make_shared<CAudioAlarmSvc>(this, s_arrMailBoxs[3].Name, s_arrMailBoxs[3].ID);

}

CScadaSvc::~CScadaSvc()
{

}

/*! \fn bool CScadaSvc::Initialize(const char *pszDataPath, unsigned int nMode)
*********************************************************************************************************
** \brief CScadaSvc::Initialize
** \details 启动SCADA
** \param pszDataPath  proj的工程文件路径名
** \param nMode 启动模式，仿真、实际运行？
** \return bool
** \author LiJin
** \date 2016年10月8日
** \note
********************************************************************************************************/
bool CScadaSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
	if (m_pDbgSvc)
	{
		m_pDbgSvc->Initialize(pszDataPath, nMode);
	}
	if (m_pDbSvc)
	{
		m_pDbSvc->Initialize(pszDataPath, nMode);
	}
	if (m_pAudioAlarmSvc)
	{
		m_pAudioAlarmSvc->Initialize(pszDataPath, nMode);
	}
	if (m_pNetBusSvc)
	{
		m_pNetBusSvc->Initialize(pszDataPath, nMode);
	}
	
 	// 为系统各个子模块创建邮箱
 	CreateMailBoxs();

	return true;
}

void CScadaSvc::Run()
{
	// 启动各个模块
	if (m_pDbgSvc)
	{
		m_pDbgSvc->Run();
	}
	if (m_pDbSvc)
	{
		m_pDbSvc->Run();
	}
	if (m_pAudioAlarmSvc)
	{
		m_pAudioAlarmSvc->Run();
	}
	if (m_pNetBusSvc)
	{
		m_pNetBusSvc->Run();
	}
	
}

void CScadaSvc::Shutdown()
{
	// 停止所有的任务（包括外部的驱动、转发、应用等等）
	if (m_pNetBusSvc)
	{
		m_pNetBusSvc->Shutdown();
	}
	if (m_pDbSvc)
	{
		m_pDbSvc->Shutdown();
	}
	if (m_pDbgSvc)
	{
		m_pDbgSvc->Shutdown();
	}
	if (m_pAudioAlarmSvc)
	{
		m_pAudioAlarmSvc->Shutdown();
	}
	// 销毁邮箱
	DestroyMailBoxs();
}
/*! \fn void CScadaSvc::CreateMailBoxs()
*********************************************************************************************************
** \brief CScadaSvc::CreateMailBoxs
** \details 为系统各个子模块创建邮箱
** \return void
** \author LiJin
** \date 2016年10月10日
** \note
********************************************************************************************************/
void CScadaSvc::CreateMailBoxs()
{
	std::vector<std::string> arrNames;
	// 创建邮局
	size_t nMailBoxNum = 0;
//	nMailBoxNum += m_pDbSvc->GetIoChannelNames(arrNames);

	//std::copy(arrNames.cbegin(), arrNames.cend(), m_arrMailBoxNames.begin());
	//m_arrNames += arrNames;

	nMailBoxNum += s_arrMailBoxs.size();

	CreatePostOffice("SCADA", nMailBoxNum);

	// 创建系统内置邮箱
	size_t i = 0;
	//int nID = 0;
	for (i = 0; i < s_arrMailBoxs.size(); i++)
	{
		CreateMailBox("SCADA", s_arrMailBoxs[i].Name, s_arrMailBoxs[i].MailNum, &s_arrMailBoxs[i].ID);
	}
	// 创建 APP及转发的邮箱
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->CreateMailBoxs();
 	}
}
/*! \fn void CScadaSvc::DestroyMailBoxs()
*********************************************************************************************************
** \brief CScadaSvc::DestroyMailBoxs
** \details 关闭邮箱、销毁邮局
** \return void
** \author LiJin
** \date 2016年10月11日
** \note
********************************************************************************************************/
void CScadaSvc::DestroyMailBoxs()
{
	// 创建 驱动邮箱
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->DestroyMailBoxs();
	}

	size_t i = 0;

 	for (i = 0; i < s_arrMailBoxs.size(); i++)
 	{
 		DestroyMailBox("SCADA", s_arrMailBoxs[i].Name);
 	}

	DestroyPostOffice("SCADA");
}
