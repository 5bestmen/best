

#include "fes.h"
#include "dbg_svc.h"
#include "db_svc.h"
#include "nb_svc.h"
#include "dataview.h"

#include "mail/mail.h"

#include <QFileInfo>
#include <QTranslator>
#include <array>
#include <QFile>
#include <QTextStream>
struct MailBoxConf
{
	const char * Name;  //! ��������
	int ID;       //! ����ID
	int MailNum; //! �������� 
	///int MailSize;//! �ʼ���С
};

static std::array<MailBoxConf, 10> s_arrMailBoxs =
{
	"DBG_SVC", 0, 100,
	"DB_SVC", 0, 100,
	"NB_SVC", 0, 100,
	"AUDIO_ALARM_SVC", 0, 100,
	"ALARM_WIN", 0, 1000,
	"ALARM_INGRAPH", 0, 1000,
	"ALARM_SCD_WIN", 0, 1000,
	"ALARM_SCD_INGRAPH",0,1000,
	"LOGIC_SVC", 0, 100,
	"fes_tester",0, 100,
};

CFesSvc::CFesSvc(void)
{
	m_pDbgSvc = std::make_shared<CDbgSvc>(this, s_arrMailBoxs[0].Name, s_arrMailBoxs[0].ID);

	m_pDbSvc = std::make_shared<CDbSvc>(this, s_arrMailBoxs[1].Name, s_arrMailBoxs[1].ID);

	m_pNetBusSvc = std::make_shared<CNetbusSvc>(this, s_arrMailBoxs[2].Name, s_arrMailBoxs[2].ID);
}


CFesSvc::~CFesSvc(void)
{
	
}
/*! \fn bool CFesSvc::Initialize(const char *pszDataPath, unsigned int nMode)
********************************************************************************************************* 
** \brief CFesSvc::Initialize 
** \details ����ǰ��
** \param pszDataPath  proj�Ĺ����ļ�·���� 
** \param nMode ����ģʽ�����桢ʵ�����У�
** \return bool 
** \author LiJin 
** \date 2016��10��8�� 
** \note 
********************************************************************************************************/
bool CFesSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
	if (m_pDbgSvc)
	{
		m_pDbgSvc->Initialize(pszDataPath,nMode);
	}
	if (m_pDbSvc)
	{
		m_pDbSvc->Initialize(pszDataPath,nMode);
	}
	if (m_pNetBusSvc)
	{
		m_pNetBusSvc->Initialize(pszDataPath, nMode);
	}
	// Ϊϵͳ������ģ�鴴������
	CreateMailBoxs();

	return true;
}

void CFesSvc::Run()
{
	// ��������ģ��
	if (m_pDbgSvc)
	{
		m_pDbgSvc->Run( );
	}
	if (m_pDbSvc)
	{
		m_pDbSvc->Run(  );
	}
	if (m_pNetBusSvc)
	{
		m_pNetBusSvc->Run( );
	}
}

void CFesSvc::Shutdown()
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
	// ��������
	DestroyMailBoxs();	
}
/*! \fn void CFesSvc::CreateMailBoxs()
********************************************************************************************************* 
** \brief CFesSvc::CreateMailBoxs 
** \details Ϊϵͳ������ģ�鴴������
** \return void 
** \author LiJin 
** \date 2016��10��10�� 
** \note 
********************************************************************************************************/
void CFesSvc::CreateMailBoxs()
{
	std::vector<std::string> arrNames;
	// �����ʾ�
	size_t nMailBoxNum = 0;
	nMailBoxNum += m_pDbSvc->GetIoChannelNames(arrNames);

	//std::copy(arrNames.cbegin(), arrNames.cend(), m_arrMailBoxNames.begin());
	//m_arrNames += arrNames;

	nMailBoxNum += s_arrMailBoxs.size();

	CreatePostOffice("FES", nMailBoxNum);

	// ����ϵͳ��������
	size_t i = 0;
	//int nID = 0;
	for (i = 0; i < s_arrMailBoxs.size(); i++)
	{
		CreateMailBox("FES", s_arrMailBoxs[i].Name, s_arrMailBoxs[i].MailNum,  &s_arrMailBoxs[i].ID);
	} 
	// ���� ������ת��������
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->CreateMailBoxs();
	}	
}
/*! \fn void CFesSvc::DestroyMailBoxs()
********************************************************************************************************* 
** \brief CFesSvc::DestroyMailBoxs 
** \details �ر����䡢�����ʾ�
** \return void 
** \author LiJin 
** \date 2016��10��11�� 
** \note 
********************************************************************************************************/
void CFesSvc::DestroyMailBoxs()
{
	// ���� ��������
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->DestroyMailBoxs();
	}

	size_t i = 0;
	//int nID = 0;
	for (i = 0; i < s_arrMailBoxs.size(); i++)
	{
		DestroyMailBox("FES", s_arrMailBoxs[i].Name);
	}

	DestroyPostOffice("FES");
}
/*! \fn void CFesSvc::OnDataShow()
********************************************************************************************************* 
** \brief CFesSvc::OnDataShow 
** \details ʵʱ������ʾ
** \return void 
** \author xingzhibing
** \date 2016��9��14�� 
** \note  ��ǰ�Ѿ��������ˣ�û�б�Ҫ������ȥ���������ڴ棬ֱ��ʹ�ý����������ݡ���Ҫע����ǣ��ڴ�������δ���ؽ���
********************************************************************************************************/
void CFesSvc::OnDataShow(const char *pszDataPath)
{
	QTranslator *qtPlugini = new QTranslator();

	QString strRunPath = qApp->applicationDirPath();

	if (qtPlugini->load(strRunPath + "/locales/fes_zh.qm"))
	{
		qApp->installTranslator(qtPlugini);
	}

	QFile data(":/image/black.css");

	QString qssFile;

	if (data.open(QFile::ReadOnly))
	{
		QTextStream styleIn(&data);

		qssFile = styleIn.readAll();

		data.close();

		qApp->setStyleSheet(qssFile);

	}


	if (m_pDataShow == nullptr)
	{
		m_pDataShow = std::make_shared<CDataView>(nullptr, m_pDbSvc, pszDataPath);
	}
	m_pDataShow->setWindowModality(Qt::WindowModal);

	m_pDataShow->exec(); 
}
