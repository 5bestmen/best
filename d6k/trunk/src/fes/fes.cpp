

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
	const char * Name;  //! 邮箱名称
	int ID;       //! 邮箱ID
	int MailNum; //! 邮箱容量 
	///int MailSize;//! 邮件大小
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
** \details 启动前置
** \param pszDataPath  proj的工程文件路径名 
** \param nMode 启动模式，仿真、实际运行？
** \return bool 
** \author LiJin 
** \date 2016年10月8日 
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
	// 为系统各个子模块创建邮箱
	CreateMailBoxs();

	return true;
}

void CFesSvc::Run()
{
	// 启动各个模块
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
	// 销毁邮箱
	DestroyMailBoxs();	
}
/*! \fn void CFesSvc::CreateMailBoxs()
********************************************************************************************************* 
** \brief CFesSvc::CreateMailBoxs 
** \details 为系统各个子模块创建邮箱
** \return void 
** \author LiJin 
** \date 2016年10月10日 
** \note 
********************************************************************************************************/
void CFesSvc::CreateMailBoxs()
{
	std::vector<std::string> arrNames;
	// 创建邮局
	size_t nMailBoxNum = 0;
	nMailBoxNum += m_pDbSvc->GetIoChannelNames(arrNames);

	//std::copy(arrNames.cbegin(), arrNames.cend(), m_arrMailBoxNames.begin());
	//m_arrNames += arrNames;

	nMailBoxNum += s_arrMailBoxs.size();

	CreatePostOffice("FES", nMailBoxNum);

	// 创建系统内置邮箱
	size_t i = 0;
	//int nID = 0;
	for (i = 0; i < s_arrMailBoxs.size(); i++)
	{
		CreateMailBox("FES", s_arrMailBoxs[i].Name, s_arrMailBoxs[i].MailNum,  &s_arrMailBoxs[i].ID);
	} 
	// 创建 驱动及转发的邮箱
	Q_ASSERT(m_pDbSvc);
	if (m_pDbSvc)
	{
		m_pDbSvc->CreateMailBoxs();
	}	
}
/*! \fn void CFesSvc::DestroyMailBoxs()
********************************************************************************************************* 
** \brief CFesSvc::DestroyMailBoxs 
** \details 关闭邮箱、销毁邮局
** \return void 
** \author LiJin 
** \date 2016年10月11日 
** \note 
********************************************************************************************************/
void CFesSvc::DestroyMailBoxs()
{
	// 创建 驱动邮箱
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
** \details 实时数据显示
** \return void 
** \author xingzhibing
** \date 2016年9月14日 
** \note  此前已经解析过了，没有必要再重新去解析共享内存，直接使用解析过的数据。需要注意的是，内存数据尚未加载解析
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
