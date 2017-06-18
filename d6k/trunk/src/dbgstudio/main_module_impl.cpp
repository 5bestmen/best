/*! @file
<PRE>
********************************************************************************
模块名       :  core模块
文件名       :  ccore.cpp
文件实现功能 :  UI管理模块 工程管理模块 模块管理模块
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.26
*/

#include "main_frame.h"

#include "main_module_impl.h" 
#include "log_impl.h"
#include "capturemsgwgt.h"

#include <QtGlobal>
#include <QMessageBox>


CMainModule::CMainModule(QObject *parent)
	:m_pMainWindow(nullptr),
	m_pModuleMgr(new CModuleMgr(this))
{
	Q_UNUSED(parent);
	m_pMainWindow = new CMainFrm();

	connect(m_pMainWindow, SIGNAL(DeleteMainModule()), this, SLOT(DeleteMainModuleSlot()));

	connect(m_pMainWindow, SIGNAL(Signal_NeedLoadInfo(QString,QString)), this, SLOT(Slot_RelodePlugin(QString,QString)));

	//m_pUi->setParent(this);
}

void CMainModule::Slot_RelodePlugin(const QString &strPluginName, const QString &strDeviceName)
{
	QString strPlugin;

#ifdef Q_OS_MAC
	// mac
#endif

#ifdef Q_OS_LINUX
	// linux
	strPlugin = strPluginName + ".so";
#endif

#ifdef Q_OS_WIN32
	// win
	strPlugin = strPluginName + ".dll";
#endif

	
	if (LoadModule(strPlugin, strDeviceName))
	{
	//	QMessageBox::information(0, tr("成功"), tr("装置加载成功!"));
	}
	else
	{
		QMessageBox::warning(0, tr("Warning"), tr("Device Load Failed,Please Check Config:% file").arg(strPlugin));
	}

}

CMainModule::~CMainModule()
{
	m_pMainWindow->deleteLater();
	
}
 
IMainWindow *CMainModule::GetMainWindow()
{
	return m_pMainWindow;
}

bool CMainModule::LoadModule(const QString &strName, const QString &strDeviceName)
{
	bool bFlag = m_pModuleMgr->LoadModule(strName, strDeviceName);
	return bFlag;
}
 
void CMainModule::DeleteMainModuleSlot()
{
	this->deleteLater();
}
/*! \fn bool CMainModule::LogString(char *pszMoudleName, const char *szLogTxt, int nLevel)
********************************************************************************************************* 
** \brief CMainModule::LogString 
** \details 登录报警接口，由主程序实现，供dll调用
** \param pszMoudleName 
** \param szLogTxt 
** \param nLevel 
** \return bool 
** \author LiJin 
** \date 2016年5月12日 
** \note 
********************************************************************************************************/
bool CMainModule::LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel)
{
	CLogImpl::GetInstance()->LogString(pszMoudleName, szLogTxt, nLevel);

	return true;
}


/*********************************************************************************************************
** \brief CMainModule::LogString
** \details 网络数据报文
** \param pszMoudleName
** \param szLogTxt
** \param nLevel
** \return bool
** \author LiJin
** \date 2016年5月12日
* * \note
********************************************************************************************************/
bool CMainModule::CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pszSrc, const char *pszDest)
{
	std::string szModuleName;
	if (pszModuleName == nullptr || strlen(pszModuleName) == 0)
	{
		szModuleName = "Unknown";
	}
	else
	{
		szModuleName = pszModuleName;
	}

	std::string szSource;
	if (pszSrc == nullptr || strlen(pszSrc) == 0)
	{
		szSource = "Unknown";
	}
	else
	{
		szSource = pszSrc;

	}
	std::string szDest;
	if (pszDest == nullptr || strlen(pszDest) == 0)
	{
		szDest = "Unknown";
	}
	else
	{
		szDest = pszDest;
	}

	CCaptureMsgWgt::GetInstance()->UpdateCapturingData(szModuleName.c_str(), pMsg, nMsgLen, nMsgType, pMsgSubType, szSource.c_str(), szDest.c_str());
	return true;
}
