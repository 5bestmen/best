/*! @file dbg_svc.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dbg_svc.cpp
�ļ�ʵ�ֹ��� :  ���Դ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���Դ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "dbg_svc.h"
#include "log/log.h"
#include <QObject> 
#include <QString> 

// #include "fesapi\fesapi.h"
// #include "mail\mail.h"

CDbgSvc::CDbgSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName,nMailBoxID)
{
	m_bStopFlag = false;
}

CDbgSvc::~CDbgSvc(void)
{
	
}

/*! \fn bool CDbgSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details ��ʼ��
** \return bool 
** \author LiJin 
** \date 2016��9��1�� 
** \note  
********************************************************************************************************/
bool CDbgSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
// 	if (Mail::CreateMailBox(SMKEY_MAIL_DBGSVR)==K_FALSE)
// 		return false;

//	StartModule();
	// ��һ��������logģ��
	CreateLog("FES");

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

void CDbgSvc::Run()
{
// 	if (Mail::RecvMail(SMKEY_MAIL_DBGSVR, m_mailbuf, MAIL_BUF_SIZE)==K_FALSE)
// 		return;
// 
// 	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

//	int nType = pMsgBase->Type;
}

void CDbgSvc::Shutdown()
{
//	StopModule();

//	Mail::DestroyMailBox(SMKEY_MAIL_DBGSVR);
	DestroyLog("FES");
}



/** @}*/
