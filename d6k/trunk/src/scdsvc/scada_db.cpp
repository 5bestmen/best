/*! @file scada_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  scada_db.cpp
文件实现功能 :  SCADA节点的内存数据库，主要是系统变量以及用户变量的管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   服务器节点的内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "scada_db.h"
#include "log/log.h"
#include "mail/mail.h"
#include "sapp.h"
#include <QObject> 
#include <QString>  

CScadaDB::CScadaDB( )
{
	m_bStopFlag = false;
	m_pScadaApps = nullptr;
	m_nAppCount = 0;
}

CScadaDB::~CScadaDB(void)
{
	m_arrApps.clear();
}
/*! \fn bool CScadaDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CScadaDB::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CScadaDB::Initialize(const char *pszDataPath, unsigned int nMode)
{
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	return true;
}


bool CScadaDB::InitMailBoxInfo(std::vector<std::shared_ptr<SAPP>>& arrApps)
{
	Q_ASSERT(arrApps.empty()==false);
	if (arrApps.empty())
	{
		return false;
	}
	m_arrAppInfos = arrApps;

	std::shared_ptr<CScadaApp > pScdApp = nullptr;

	for each (std::shared_ptr<SAPP> var in m_arrAppInfos)
	{
		pScdApp = std::make_shared<CScadaApp>(var.get());
		m_arrApps.push_back(pScdApp);
	}

	m_nAppCount = m_arrApps.size();


	return  true;
}
void CScadaDB::Run()
{
 
}

void CScadaDB::Shutdown()
{

}

/*! \fn void  CScadaDB::CreateMailBoxs()
********************************************************************************************************* 
** \brief CScadaDB::CreateMailBoxs 
** \details 创建邮箱
** \return void 
** \author LiJin 
** \date 2017年6月1日 
** \note 
********************************************************************************************************/
void  CScadaDB::CreateMailBoxs()
{
	std::string szTagName;
	int nID = 0;

	for (auto i : m_arrApps)
	{
		//内置应用则不开辟邮箱，已经默认在启动之初就开辟好了
		if (!i->InnerProgarmmeEnable())
		{
			szTagName = i->GetProgrammeName();
			Q_ASSERT(szTagName.empty() == false);
			if (szTagName.empty() == false)
			{
				CreateMailBox("SCADA", szTagName.c_str(), 100, &nID);
				Q_ASSERT(nID != 0);
				i->SetMailBoxID(nID);
			}
		}		
	}
}
/*! \fn void  CScadaDB::DestroyMailBoxs()
********************************************************************************************************* 
** \brief CScadaDB::DestroyMailBoxs 
** \details 销毁邮箱
** \return void 
** \author LiJin 
** \date 2017年6月1日 
** \note 
********************************************************************************************************/
void  CScadaDB::DestroyMailBoxs()
{

}


/** @}*/
