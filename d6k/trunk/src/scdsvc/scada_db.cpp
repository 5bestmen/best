/*! @file scada_db.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  scada_db.cpp
�ļ�ʵ�ֹ��� :  SCADA�ڵ���ڴ����ݿ⣬��Ҫ��ϵͳ�����Լ��û������Ĺ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �������ڵ���ڴ����ݿ�
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
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
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
** \details ��������
** \return void 
** \author LiJin 
** \date 2017��6��1�� 
** \note 
********************************************************************************************************/
void  CScadaDB::CreateMailBoxs()
{
	std::string szTagName;
	int nID = 0;

	for (auto i : m_arrApps)
	{
		//����Ӧ���򲻿������䣬�Ѿ�Ĭ��������֮���Ϳ��ٺ���
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
** \details ��������
** \return void 
** \author LiJin 
** \date 2017��6��1�� 
** \note 
********************************************************************************************************/
void  CScadaDB::DestroyMailBoxs()
{

}


/** @}*/
