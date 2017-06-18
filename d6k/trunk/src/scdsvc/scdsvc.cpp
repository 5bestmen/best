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
	char * Name;  //! ��������
	int ID;       //! ����ID
	int MailNum; //! �������� 
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
** \details ����SCADA
** \param pszDataPath  proj�Ĺ����ļ�·����
** \param nMode ����ģʽ�����桢ʵ�����У�
** \return bool
** \author LiJin
** \date 2016��10��8��
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
	
 	// Ϊϵͳ������ģ�鴴������
 	CreateMailBoxs();

	return true;
}

void CScadaSvc::Run()
{
	// ��������ģ��
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
	// ֹͣ���е����񣨰����ⲿ��������ת����Ӧ�õȵȣ�
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
	// ��������
	DestroyMailBoxs();
}
/*! \fn void CScadaSvc::CreateMailBoxs()
*********************************************************************************************************
** \brief CScadaSvc::CreateMailBoxs
** \details Ϊϵͳ������ģ�鴴������
** \return void
** \author LiJin
** \date 2016��10��10��
** \note
********************************************************************************************************/
void CScadaSvc::CreateMailBoxs()
{
	std::vector<std::string> arrNames;
	// �����ʾ�
	size_t nMailBoxNum = 0;
//	nMailBoxNum += m_pDbSvc->GetIoChannelNames(arrNames);

	//std::copy(arrNames.cbegin(), arrNames.cend(), m_arrMailBoxNames.begin());
	//m_arrNames += arrNames;

	nMailBoxNum += s_arrMailBoxs.size();

	CreatePostOffice("SCADA", nMailBoxNum);

	// ����ϵͳ��������
	size_t i = 0;
	//int nID = 0;
	for (i = 0; i < s_arrMailBoxs.size(); i++)
	{
		CreateMailBox("SCADA", s_arrMailBoxs[i].Name, s_arrMailBoxs[i].MailNum, &s_arrMailBoxs[i].ID);
	}
	// ���� APP��ת��������
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->CreateMailBoxs();
 	}
}
/*! \fn void CScadaSvc::DestroyMailBoxs()
*********************************************************************************************************
** \brief CScadaSvc::DestroyMailBoxs
** \details �ر����䡢�����ʾ�
** \return void
** \author LiJin
** \date 2016��10��11��
** \note
********************************************************************************************************/
void CScadaSvc::DestroyMailBoxs()
{
	// ���� ��������
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
