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

#include "ccore.h"
#include "scadastudio/uimgr.h"
#include "projmgr.h"
#include "modulemgr.h"
//#include "uimodule/outputmessagerclient.h"
#include "logdefine.h"
#include "scadastudio/define.h"
#include "timepack.h"
#include "selectpointdialog.h"
#include "scadastudio/imodule.h"
#include "totalselectdialog.h"
#include "extenservicemodulemgr.h"

#include <QtGlobal>
#include <QThread>
#include <QApplication>
#include <QMessageBox>


#if 0
#if defined (WIN32) || defined (WIN64) 
#include <process.h>
#endif

#if defined(__linux__) || defined(__linux)
#include<unistd.h>
#endif // LINUX


size_t GetCurPID()
{
	size_t nPid = 0;
#if defined (WIN32) || defined (WIN64) 
#include <process.h>

	nPid = _getpid();
#endif
#if defined(__linux__) || defined(__linux)
#include<unistd.h>
	nPid = getpid();
#endif // LINUX
	return nPid;
}
#endif


/*! \fn CCore::CCore(QObject *parent)
********************************************************************************************************* 
** \brief CCore::CCore 
** \details 
** \param parent 
** \return  
** \author gw
** \date 2016年6月2日 
** \note 
********************************************************************************************************/
CCore::CCore(QObject *parent)
	:m_pUi(NULL), m_pProjMgr(NULL), m_pModuleMgr(NULL), /*m_pOutClient(NULL),*/ m_pExtendServiceModuleMgr(nullptr)
{
	Q_UNUSED(parent);
	
	m_pUi = new CUIMgr();

	m_pProjMgr = new CProjMgr(this);

	m_pModuleMgr = new CModuleMgr(this);

//	m_pOutClient = new COutputMessagerClient(this);

	m_pExtendServiceModuleMgr = new CExtendServiceModuleMgr(this);

	connect(m_pUi, SIGNAL(DeleteCore()), this, SLOT(DeleteCoreSlot()));
	//m_pUi->setParent(this);
}

/*! \fn CCore::~CCore()
********************************************************************************************************* 
** \brief CCore::~CCore 
** \details 
** \return  
** \author gw
** \date 2016年6月2日 
** \note 
********************************************************************************************************/
CCore::~CCore()
{
	if (m_pProjMgr)
	{
		delete m_pProjMgr;
		m_pProjMgr = NULL;
	}

	if (m_pModuleMgr)
	{
		delete m_pModuleMgr;
		m_pModuleMgr = NULL;
	}

	if (m_pUi)
	{
		delete m_pUi;
		m_pUi = nullptr;
	}

	if (m_pExtendServiceModuleMgr)
	{
		delete m_pExtendServiceModuleMgr;
		m_pExtendServiceModuleMgr = nullptr;
	}
}

/*! \fn IMainWindow *CCore::GetUIMgr()
********************************************************************************************************* 
** \brief CCore::GetUIMgr 
** \details 获得main ui指针
** \return IMainWindow * 
** \author gw
** \date 2016年4月6日 
** \note 
********************************************************************************************************/
IMainWindow *CCore::GetUIMgr()
{
	if (m_pUi)
	{
		return m_pUi;
	}
	
	return NULL;
}

/*! \fn IProjMgr *CCore::GetProjMgr()
********************************************************************************************************* 
** \brief CCore::GetProjMgr 
** \details 工程管理模块
** \return IProjMgr * 
** \author gw
** \date 2016年4月6日 
** \note 
********************************************************************************************************/
IProjMgr *CCore::GetProjMgr()
{
	if(m_pProjMgr)
	{
		return m_pProjMgr;
	}

	return NULL;
}

/*! \fn IModuleMgr *CCore::GetModuleMgr()
********************************************************************************************************* 
** \brief CCore::GetModuleMgr 
** \details 模块管理模块
** \return IModuleMgr * 
** \author gw
** \date 2016年4月6日 
** \note 
********************************************************************************************************/
CModuleMgr *CCore::GetModuleMgr()
{
	if(m_pModuleMgr)
	{
		return m_pModuleMgr;
	}
	
	return NULL;
}

CExtendServiceModuleMgr *CCore::GetExtendServiceModuleMgr()
{
	if (m_pExtendServiceModuleMgr)
	{
		return m_pExtendServiceModuleMgr;
	}

	return nullptr;
}

void CCore::DeleteCoreSlot()
{
	this->deleteLater();
}

/*! \fn void CCore::LoadModules()
********************************************************************************************************* 
** \brief CCore::LoadModules 
** \details 加载配置文件，并加载插件
** \return void 
** \author LiJin 
** \date 2016年8月16日 
** \note 
********************************************************************************************************/
void CCore::LoadModules()
{
	Q_ASSERT(m_pModuleMgr);
	if (m_pModuleMgr)
	{
 		m_pModuleMgr->LoadPlugInXml();
		m_pModuleMgr->LoadModules();
	}

	Q_ASSERT(m_pExtendServiceModuleMgr);
	if (m_pExtendServiceModuleMgr)
	{
		m_pExtendServiceModuleMgr->LoadPlugInXml();
		m_pExtendServiceModuleMgr->LoadModules();
	}
}


/*! \fn bool CCore::SelectPoint(std::string & szTagName)
********************************************************************************************************* 
** \brief CCore::SelectPoint 
** \details 在该接口内实现相关的选点界面 前置 对象
** \param szTagName 即是输入参数，也是返回结果
** \return bool 
** \author gw
** \date 2016年9月20日 
** \note 
********************************************************************************************************/
//bool CCore::SelectPoint(std::string & szTagName)
//{
//	IModule *pFesModule = GetModule("FES");
//
//
//	QDialog *pDlg = (QDialog *)pFesModule->SelectTagDialog();
//
//	/*pDlg->setWindowModality(Qt::ApplicationModal);
//	pDlg->exec();*/
//
//	//QTabWidget *pTab;
//	//pTab->addTab(pDlg);
//
//	//CTotalSelectWindow *pTotalSelect = new CTotalSelectWindow(nullptr);
//	//pTotalSelect->setWindowModality(Qt::ApplicationModal);
//
//	//pTotalSelect->show();
//
//	CTotalSelectDialog *pDialog = new CTotalSelectDialog(nullptr);
//
//	pDialog->setWindowModality(Qt::ApplicationModal);
//	pDialog->AddWidget(pDlg, "FES");
//	pDialog->exec();
//
//
//
//
//	//pWidget = pTab->currentWidget();
//	//
//	//pWidget 
//
//	//pDialog->exec();
//
////	CSelectPointDialog dlg;
////	dlg.doModal();
//// 	if(dlg.exec() == IDOK)
//// 	{
//// 		szInputPointName = dlg.m_szTagName;
//// 	}
//
// //  IModule *pObjectModule =	GetModule("OBJECT");
//
////	CSelectObjectPointDialog  *pDlg =  pObjectModule->SelectPointDlg();
////	dlg.add(pDlg)
//
//	return true;
//}

/*! \fn bool CCore::SelectTag(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName)
********************************************************************************************************* 
** \brief CCore::SelectTag 
** \details 
** \param arrAllowDataTypes 
** \param bFilterEnWrite 
** \param szNodeTagName 
** \param szTagName 
** \param szFiledName 
** \return bool 
** \author gw
** \date 2017年2月14日 
** \note 
********************************************************************************************************/
bool CCore::SelectTag(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szFesTagName, std::string & szTagName, std::string &szFiledName)
{
	IModule *pFesModule = GetModule("FES");
	Q_ASSERT(pFesModule);
	if (!pFesModule)
	{
		return false;
	}

	std::vector<int> vec;
	vec.clear();
	//if (arrAllowDataTypes.empty())
	{
			/*DT_BOOLEAN = 1,	  //!< 布尔，8位，缺省值0
			DT_CHAR = 2,      //!< 有符号字节（8位）
			DT_BYTE = 3,      //!< 无符号字节（8位）
			DT_SHORT = 4,     //!< 无符号短整数 ，16位， 
			DT_WORD = 5,      //!< 无符短整数 ，16位
			DT_INT = 6,       //!< 有符号长整数 32位
			DT_DWORD = 7,      //!< 无符号长整数 32位
			DT_LLONG = 8,      //!< 有符号长整数  64位
			DT_LWORD = 9,      //!< 无符号长整数  64位
			DT_FLOAT = 10,
			DT_DOUBLE = 11,
			DT_STRING = 12,
			DT_BLOCK = 13*/
		vec.push_back(DT_BOOLEAN);
		vec.push_back(DT_CHAR);
		vec.push_back(DT_BYTE);
		vec.push_back(DT_SHORT);
		vec.push_back(DT_WORD);
		vec.push_back(DT_DWORD);
		vec.push_back(DT_LLONG);
		vec.push_back(DT_FLOAT);
		vec.push_back(DT_DOUBLE);
		vec.push_back(DT_STRING);
		vec.push_back(DT_BLOCK);
	}

	//待讨论
	QDialog *pDlg = static_cast<QDialog *>(pFesModule->SelectTagDialog(vec, bFilterEnWrite, szFesTagName, szTagName, szFiledName));
	Q_ASSERT(pDlg);
	if (!pDlg)
	{
		return false;
	}

	CTotalSelectDialog *pDialog = new CTotalSelectDialog(nullptr);
	
	pDialog->setWindowModality(Qt::ApplicationModal);
	pDialog->AddWidget(pDlg, "FES");
	if (pDialog->exec() == QDialog::Accepted)
	{
		std::vector<QString> vecName;
		GetModulesName(vecName);
		
		auto pNodeModule = GetModule("node_config");
		Q_ASSERT(pNodeModule);
		if (!pNodeModule)
		{
			return false;
		}
		
		std::string strTmp = szFesTagName;
		szFesTagName = pNodeModule->GetNodeNameByFesConfigName(szFesTagName);
		if (szFesTagName.empty())
		{
			QMessageBox box;
			box.setText(QObject::tr("Fes tagname %1 is related to fes node").arg(strTmp.c_str()));
			box.exec();

			return false;
		}

		return true;
	}
	else
	{
		return false;
	}


	//std::vector<std::string> arrAttrNames;

	//CTagAttMgr att;

	//for each (auto type in arrAllowDataTypes)
	//{
	//	att.GetTotalWriteableAttrssWithDataType(IDD_AIN, type, arrAttrNames);
	//}

	return true;
}

bool CCore::GetModulesName(std::vector<QString> & arrNames)
{
	arrNames.clear();
	//arrNames.push_back("FES");
	//arrNames.push_back("GRAPH");

	m_pModuleMgr->GetModulesName(arrNames);

	return true;
}
/*! \fn bool CCore::LogCheckMsg(const char *pszMoudleName, const char *szLogTxt, int nLevel,int nErrNo)
********************************************************************************************************* 
** \brief CCore::LogCheckMsg 
** \details 供子模块调用，显示编译检查的结果到输出窗口
** \param pszMoudleName 
** \param szLogTxt 
** \param nLevel 
** \param nErrNo 
** \return bool 
** \author gw
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
bool CCore::LogCheckMsg(const char *pszMoudleName, const char *szLogTxt, int nLevel,int nErrNo)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;
	if (strlen(szLogTxt) == 0)
		return false;

	QString szLogCheck = QString("%1 err no = [ %2 ],level = [ %3 ]").arg(szLogTxt).arg(nErrNo).arg(nLevel);

	return	LogMsg(pszMoudleName, szLogCheck.toStdString().c_str(), 0);
}
/*! \fn bool CCore::LogMsg(char *pszMoudleName, char *szLogTxt, int nLevel)
********************************************************************************************************* 
** \brief CCore::LogMsg 
** \details Log
** \param pszMoudleName 
** \param szLogTxt 
** \param nLevel 
** \return bool 
** \author gw
** \date 2016年6月2日 
** \note 
********************************************************************************************************/
bool CCore::LogMsg(const char *pszMoudleName,const char *szLogTxt, int nLevel)
{
	Q_ASSERT(szLogTxt);
	if (szLogTxt == nullptr)
		return false;
	if (strlen(szLogTxt) == 0)
		return false;

	std::string szModuleName;

	if (pszMoudleName == nullptr || strlen(pszMoudleName) == 0)
	{
		szModuleName = "Unknown";
	}
	else
	{
		szModuleName = pszMoudleName;
	}

	MSG_LOG log;
	memset(&log, 0, sizeof(log));

	// 时间 
	CTimePack::GetCurrentDateTime(log.Time);

	// 进程、线程信息
	log.ProcessID = QCoreApplication::applicationPid();
	log.ThreadID = reinterpret_cast <size_t> (QThread::currentThreadId());

	log.Level = nLevel;
	memset(log.ModuleName, 0, sizeof(log.ModuleName));
	strncpy(log.ModuleName, szModuleName.c_str(), qMin(szModuleName.length(),(size_t)MAX_LOG_MODULE_NAME_LEN ) ) ;
	memset(log.LogText, 0, sizeof(log.LogText));
	strncpy(log.LogText, szLogTxt, qMin( strlen(szLogTxt) , (size_t)MAX_LOG_MODULE_NAME_LEN));

	int nCount = sizeof(MSG_LOG);

	//m_pOutClient->sendData(reinterpret_cast<char*>(&log), nCount);

	m_pUi->AddSysOutItem(log);

	//delete pSendData;
	//pSendData = nullptr;

	return true;
}

/*! \fn void CCore::SetModifyFlag(bool bFlag, char *pModule)
********************************************************************************************************* 
** \brief CCore::SetModifyFlag 
** \details 模块修改
** \param bFlag 
** \param pModule 
** \return void 
** \author gw
** \date 2016年4月12日 
** \note 
********************************************************************************************************/
//void CCore::SetModifyFlag(int nModuleID, bool bIsModified)
//{
//	GetModuleMgr()->SetModifyFlag(nModuleID, bIsModified);
//}
//
//bool CCore::GetModifyFlag(int nModuleID)
//{
//	return GetModuleMgr()->GetModifyFlag(nModuleID);
//}

/*! \fn bool CCore::LoadInternalModule()
********************************************************************************************************* 
** \brief CCore::LoadInternalModule 
** \details 加载内部模块
** \return bool 
** \author gw
** \date 2016年6月2日 
** \note 
********************************************************************************************************/
bool CCore::LoadInternalModule()
{
	return m_pModuleMgr->LoadInternalModule();
}

/*! \fn IModule *CCore::GetModule(const char *szTxt)
********************************************************************************************************* 
** \brief CCore::GetModule 
** \details 获得module
** \param szTxt 
** \return IModule * 
** \author gw
** \date 2017年2月14日 
** \note 
********************************************************************************************************/
IModule *CCore::GetModule(const char *szTxt)
{
	if (m_pModuleMgr == nullptr)
	{
		return nullptr;
	}

	auto pModule = m_pModuleMgr->GetModule(szTxt);
	Q_ASSERT(pModule);
	if (!pModule)
	{
		return nullptr;
	}

	return pModule;
}