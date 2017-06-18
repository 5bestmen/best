/*! @file
<PRE>
********************************************************************************
模块名       :  管理模块
文件名       :  managemodule.cpp
文件实现功能 :  惯例各个模块的模块
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
*  @date	2016.04.05
*/
#include "modulemgr.h"
#include "scadastudio/uimgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"
//#include "uimodule/outputmessagerclient.h"
#include "fesmodule.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "closeprojectdialog.h"
#include "scadastudio/tabwidget.h"
#include "nodeparamodule.h"

#include "nodeparamodule.h"
#include "sysmodule.h"
#include "usermodule.h"
#include "extendservicemodule.h"
#include "scadastudio/iprojmgr.h"

#include "scadastudiodefine.h"
#include "scadavariablemodule.h"

#include <QLibrary>
#include <QApplication>
#include <QFile>
#include <QXmlStreamReader>
#include <QStandardItemModel>
#include <QDomDocument>
#include <QFileInfo>
#include <QString>
#include <QDialog>
#include <QDebug>




CModuleMgr::CModuleMgr(IMainModuleInterface *pCore)
	:m_pCore(pCore)
{
	Q_ASSERT(pCore); 
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;

	m_arrMap.clear();
	m_arrStringPoolVec.clear();
}

CModuleMgr::~CModuleMgr()
{
	UnLoadModules();
#if 0
	m_arrExtModules.clear();

	for each (auto var in m_arrInnerModules)
	{
		delete var;
	}
	m_arrInnerModules.clear();
#endif

	for (auto i : m_setModules)
	{
		if (i->IsExternalModule() == false)
		{
			delete i->m_pModuleInterface;
			i->m_pModuleInterface = nullptr;
		}
	}

	
	m_setModules.clear();
}
 
/*! \fn bool  CModuleMgr::AddInnerModule(IModule *pInner,unsigned int nID)
********************************************************************************************************* 
** \brief CModuleMgr::AddInnerModule 
** \details 添加内部模块
** \param pInner 
** \return bool 
** \author LiJin 
** \date 2016年8月17日 
** \note 
********************************************************************************************************/
bool  CModuleMgr::AddInternalModule(IModule *pInner, const QString &  strModuleName, const QString & szDescript)
{
	Q_ASSERT(pInner);
	if (pInner == nullptr)
		return false;

	std::shared_ptr<CModuleItem> pModule = std::make_shared<CModuleItem>(pInner, strModuleName,szDescript);

	auto ret = m_setModules.insert(pModule);

	return ret.second;

// 	auto it_find = std::find(m_arrInnerModules.begin(), m_arrInnerModules.end(), pInner);
// 	if (it_find == m_arrInnerModules.end())
// 	{
// 		m_arrInnerModules.push_back(pInner);
// 		return true;
// 	}
	return false;
}

/*! \fn bool CModuleMgr::LoadProjFromFile(QDomDocument *pDocument
********************************************************************************************************* 
** \brief CModuleMgr::LoadProjFromFile 
** \details 加载工程文件
** \param pDocument 
** \return bool 
** \author gw
** \date 2016年4月13日 
** \note 
********************************************************************************************************/
bool CModuleMgr::LoadProjFromFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath)
{
	Q_ASSERT(pDocument);
	Q_ASSERT(szPath.isEmpty() == false);
	if (!pDocument)
	{
		return false;
	}

	for ( auto i : m_setModules)
	{	 
	//	Q_ASSERT(i->m_pModuleInterface);
		if (i->m_pModuleInterface != nullptr)
		{
			i->m_pModuleInterface->LoadProject(pDocument, pRoot, szPath);
		} 
	}

#if 0
    //加载外部模块
	for each (auto it in m_arrExtModules)
	{
		if (it->m_pModuleInterface != nullptr)
		{
			it->m_pModuleInterface->LoadProject(pDocument, pRoot, szPath);
		}		
	}

	//加载内部模块
	for each (auto var in m_arrInnerModules)
	{
		var->LoadProject(pDocument, pRoot, szPath);
	}
#endif

	auto pTop = m_pCore->GetUIMgr()->GetTopItem();
	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem((QStandardItem *)pTop));

	return true;
}
/*! \fn bool CModuleMgr::LoadInternalModule()
********************************************************************************************************* 
** \brief CModuleMgr::LoadInternalModule 
** \details 加载内部模块
** \return bool 
** \author gw
** \date 2016年6月3日 
** \note 
********************************************************************************************************/
bool CModuleMgr::LoadInternalModule()
{	
	CFesModule *pFesModel = new CFesModule(m_pCore);
	pFesModel->Init(m_pCore);
	


	bool bRet = false;
	QString szLog;

	bRet = AddInternalModule(pFesModel, "fes", QStringLiteral("前置"));
	if (bRet)
	{
		// log 加载成功
		szLog = QObject::tr("start [  fes  ] successed.");
		LogString(szLog.toStdString().c_str(), 1);

	}
	else
	{
		// log 加载 失败
		szLog = QObject::tr("start [  fes  ]  failed.");
		LogString(szLog.toStdString().c_str(), 1);
	}

	CNodeParaModule *pNodeConfigModule = new CNodeParaModule;
	pNodeConfigModule->Init(m_pCore);
	bRet = AddInternalModule(pNodeConfigModule, "node_config", QStringLiteral("节点配置"));
	if (bRet)
	{
		// log 加载成功
		szLog = QObject::tr("start [  node_config  ] successed.");
		LogString(szLog.toStdString().c_str(), 1);

	}
	else
	{
		// log 加载 失败
		szLog = QObject::tr("start [  node_config  ]  failed.");
		LogString(szLog.toStdString().c_str(), 1);
	}


	CSysConfigurationModule *pSysConfigModule = new CSysConfigurationModule;
	pSysConfigModule->Init(m_pCore);

	bRet = AddInternalModule(pSysConfigModule, "sys_config", QStringLiteral("系统配置"));
	if (bRet)
	{
		// log 加载成功
		szLog = QObject::tr("start [  sys_config  ] successed.");
		LogString(szLog.toStdString().c_str(), 1);

	}
	else
	{
		// log 加载 失败
		szLog = QObject::tr("start [  sys_config  ]  failed.");
		LogString(szLog.toStdString().c_str(), 1);
	}

	CUserManageModule *pUserManageModule = new CUserManageModule;
	pUserManageModule->Init(m_pCore);

	bRet = AddInternalModule(pUserManageModule, "user_manager", QStringLiteral("用户管理"));
	if (bRet)
	{
		// log 加载成功
		szLog = QObject::tr("start [  user_manager  ] successed.");
		LogString(szLog.toStdString().c_str(), 1);

	}
	else
	{
		// log 加载 失败
		szLog = QObject::tr("start [  user_manager  ]  failed.");
		LogString(szLog.toStdString().c_str(), 1);
	}

	CScadaVariableModule *pScadaVariableModule = new CScadaVariableModule(m_pCore);
	pScadaVariableModule->Init(m_pCore);

	bRet = AddInternalModule(pScadaVariableModule, "scada_variable", QStringLiteral("用户变量"));
	if (bRet)
	{
		// log 加载成功
		szLog = QObject::tr("start [  scada_variable  ] successed.");
		LogString(szLog.toStdString().c_str(), 1);

	}
	else
	{
		// log 加载 失败
		szLog = QObject::tr("start [  scada_variable  ]  failed.");
		LogString(szLog.toStdString().c_str(), 1);
	}

	return true;
}
/*! \fn bool CModuleMgr::LoadPlugInXml()
********************************************************************************************************* 
** \brief CModuleMgr::LoadPlugInXml 
** \details 从scadastuido.xml解析数据，并加载
** \return bool 
** \author LiJin 
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CModuleMgr::LoadPlugInXml()
{
	QString szXmlPath = qApp->applicationDirPath();	
	szXmlPath += "/";

	szXmlPath += PLUGIN_XML;

	QFile PluginXml(szXmlPath);
	if (PluginXml.open(QFile::ReadOnly | QFile::Text) == false)
	{
		PluginXml.close(); 
		// log下
		QString szLog = "Open [ " + szXmlPath + " ] failed";

		LogString(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 解析数据
	QXmlStreamReader xmlReader(&PluginXml);
	QString strTmp;
	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//遍历所有配置
		xmlReader.readNext();
		strTmp = xmlReader.name().toString();
		if (xmlReader.isStartElement())
		{
			//qDebug() << "xmlReader:"<<xmlReader.name().toString();
			if (xmlReader.name().toString() == "config")
			{
				
			}
			else if (xmlReader.name().toString() == "module")
			{

			}
			else if (xmlReader.name().toString() == "item")
			{
				QString szID = xmlReader.attributes().value("id").toString();
				QString szName = xmlReader.attributes().value("name").toString();
				QString szCommon = xmlReader.attributes().value("descript").toString();
				QString szDll = xmlReader.attributes().value("dllname").toString();
				QString szType = xmlReader.attributes().value("type").toString();
				QString szVersion = xmlReader.attributes().value("version").toString();

				std::shared_ptr <CModuleItem> pModule = std::make_shared<CModuleItem>(true);
				pModule->m_nID = static_cast<unsigned int>(szID.toInt());

				pModule->m_szName = szName;
				pModule->m_szDescript = szCommon;
				pModule->m_szDllName = szDll;
				pModule->m_szVersion = szVersion;
				pModule->m_nType = static_cast<unsigned int>(szType.toInt());

				// 查找，名字不能重复
				AddExternalModule(pModule);
			}
		}
	}

	return true;
}

void CModuleMgr::AddExternalModule(std::shared_ptr<CModuleItem>pModule)
{
	if (pModule == nullptr)
	{
		Q_ASSERT(false);
		return;
	}

	auto it = m_setModules.begin();
	for (; it != m_setModules.end(); ++it)
	{
		if (pModule->m_szDllName == (*it)->m_szDllName)
		{// 重名
			Q_ASSERT(false);
			return;
		}
	}

	auto ret = m_setModules.insert(pModule);
	Q_ASSERT(ret.second);
}

void CModuleMgr::LoadModules()
{
	QString szExt ;
#ifdef Q_OS_MAC
	// mac
#endif

#ifdef Q_OS_LINUX
	// linux
	szExt =  ".so";
#endif

#ifdef Q_OS_WIN32
	// win
	szExt = ".dll";
#endif

	QString szDllName;
	QString szLog;

	for (auto i: m_setModules)
	{
		if (i->IsExternalModule() == true)
		{
			Q_ASSERT(i->m_szDllName.isEmpty() == false);
			if (i->m_szDllName.isEmpty() == false)
			{
				szDllName = i->m_szDllName + szExt;
				QLibrary dlllib(szDllName);
				if (dlllib.load())
				{
					CreateModule fnCreateModule = (CreateModule)dlllib.resolve("CreateModule");
					DestroyModule fnDestroyModule = (DestroyModule)dlllib.resolve("DestroyModule");
					Q_ASSERT(fnCreateModule);
					Q_ASSERT(fnDestroyModule);

					if (fnCreateModule && fnDestroyModule)
					{
						i->m_pModuleInterface = fnCreateModule();
						i->m_fnDestroyModule = fnDestroyModule;
						Q_ASSERT(i->m_pModuleInterface);
						if (i->m_pModuleInterface)
						{
							i->m_pModuleInterface->Init(m_pCore);
							// log 加载成功
							szLog = "Load [ " + szDllName + " ] successed.";
							LogString(szLog.toStdString().c_str(), 1);
						}
						else
						{// log
							szLog = "Load [ " + szDllName + " ] failed, the module's interface is wrong.";
							LogString(szLog.toStdString().c_str(), 1);
						}
					}
					else
					{// log
						szLog = "Load [ " + szDllName + " ] failed, the callback function is bad.";
						LogString(szLog.toStdString().c_str(), 1);
					}
				}
				else
				{// log 打印
					szLog = "Load [ " + szDllName + " ] failed";
					LogString(szLog.toStdString().c_str(), 1);
				}
			}
		}
	}

#if 0
	for each (auto it in m_arrExtModules)
	{
		Q_ASSERT(it->m_szDllName.isEmpty() == false);
		if (it->m_szDllName.isEmpty() == false)
		{
			szDllName = it->m_szDllName + szExt;
			QLibrary dlllib(szDllName);
			if (dlllib.load())
			{
				CreateModule fnCreateModule = (CreateModule)dlllib.resolve("CreateModule");
				DestroyModule fnDestroyModule = (DestroyModule)dlllib.resolve("DestroyModule");
				Q_ASSERT(fnCreateModule);
				Q_ASSERT(fnDestroyModule);

				if (fnCreateModule && fnDestroyModule)
				{
					it->m_pModuleInterface = fnCreateModule();
					it->m_fnDestroyModule = fnDestroyModule;
					Q_ASSERT(it->m_pModuleInterface);
					if (it->m_pModuleInterface)
					{
						it->m_pModuleInterface->Init(m_pCore);
						// log 加载成功
						szLog = "Load [ " + szDllName + " ] successed.";
						LogString(szLog.toStdString().c_str(), 1);
					}
					else
					{// log
						szLog = "Load [ " + szDllName + " ] failed, the module's interface is wrong.";
						LogString(szLog.toStdString().c_str(), 1);
					}
				}
				else
				{// log
					szLog = "Load [ " + szDllName + " ] failed, the callback function is bad.";
					LogString(szLog.toStdString().c_str(), 1);
				}
			}
			else
			{// log 打印
				szLog = "Load [ " + szDllName + " ] failed";
				LogString(szLog.toStdString().c_str(), 1);
			}
		}
	}
#endif

}

void CModuleMgr::UnLoadModules()
{
	for (auto it : m_setModules)
	{
		if (it->IsExternalModule() == true)
		{
			//Q_ASSERT(it->m_fnDestroyModule);
			if (it->m_fnDestroyModule)
			{
				it->m_fnDestroyModule();
			}	
		}
	}
#if 0
	for each (auto it in m_arrExtModules)
	{
		//Q_ASSERT(it->m_fnDestroyModule);
		if (it->m_fnDestroyModule)
		{
			it->m_fnDestroyModule();
		}	
	}
#endif
}

bool  CModuleMgr::LogString(const char *szLogTxt, int nLevel)
{
	Q_ASSERT(m_pCore);
	if (m_pCore == nullptr)
		return false;

	m_pCore->LogMsg("scadastudio", szLogTxt, nLevel);

	return true;
}
/*! \fn bool CModuleMgr::SaveProjFromFile(QDomDocument *pDocument)
*********************************************************************************************************
** \brief CModuleMgr::SaveProjFromFile
** \details 保存工程文件 内部保存各个模块实现
** \param pDocument
** \return bool
** \author gw
** \date 2016年4月13日
** \note
********************************************************************************************************/
bool CModuleMgr::SaveToProjectFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath)
{
	Q_ASSERT(pDocument);
	Q_ASSERT(szPath.isEmpty() == false);
	if (!pDocument)
	{
		return false;
	}
	
	m_arrMap.clear();
	m_arrStringPoolVec.clear();
	int32u nDescStringPoolOccNo = 0;

	for each (auto it in m_setModules)
	{
	//	Q_ASSERT(it->m_pModuleInterface);
		if (it->m_pModuleInterface != nullptr)
		{
			//
			it->m_pModuleInterface->GetDesPoolArr(&m_arrMap, &m_arrStringPoolVec, &nDescStringPoolOccNo);
			it->m_pModuleInterface->SaveProject(pDocument, pRoot, szPath);
		}
	}

	SaveDescStringPool();

	return true;
}

/*! \fn bool CModuleMgr::CreateNewProj(QDomDocument *pDocument, const QString& szPath)
*********************************************************************************************************
** \brief CModuleMgr::CreateNewProj
** \details 新建工程
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016年8月15日
** \note
********************************************************************************************************/
bool CModuleMgr::CreateNewProj(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath, CQuoteItem *pTopItem)
{
	Q_ASSERT(pDocument);

	if (!pDocument)
	{
		return false;
	}

	for each (auto it in m_setModules)
	{
	//	Q_ASSERT(it->m_pModuleInterface);
		if (it->m_pModuleInterface != nullptr)
		{
			it->m_pModuleInterface->CreateNewProject(pDocument, pRoot, szPath, pTopItem);
		}
	}

	return true;
}

/*! \fn bool  CModuleMgr::CloseProj(QDomDocument *pDocument, const QString& szPath)
*********************************************************************************************************
** \brief CModuleMgr::CloseProj
** \details 关闭工程
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016年8月16日
** \note
********************************************************************************************************/
bool  CModuleMgr::CloseProj(QDomDocument *pDocument, const QString& szPath)
{
	Q_ASSERT(pDocument);

	if (!pDocument)
	{
		return false;
	}

	//判断是否有修改
	auto bModify = false;
	for each (auto it in m_setModules)
	{
	//	Q_ASSERT(it->m_pModuleInterface);
		if (it->m_pModuleInterface != nullptr)
		{
			if (it->m_pModuleInterface->GetModifyFlag())
			{
				bModify = true;
			}
		}
	}
 
	if (bModify)
	{
		CCloseProjectDialog dialog;
		dialog.exec();
		if (dialog.GetType() == CCloseProjectDialog::SaveProject)
		{
			for each (auto it in m_setModules)
			{
				Q_ASSERT(it->m_pModuleInterface);
				if (it->m_pModuleInterface)
				{
					it->m_pModuleInterface->CloseProject(pDocument, &pDocument->documentElement(), szPath, true);
				}
			}


			if (m_pCore)
			{
				//delete all children of parent;
				//QStandardItem * loopItem = parent; //main loop item
				QList<QStandardItem *> carryItems; //Last In First Out stack of items
				QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
				auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
				while (pTop->rowCount())
				{
					itemsToBeDeleted << pTop->takeRow(0);
					//if the row removed has children:
					if (itemsToBeDeleted.at(0)->hasChildren())
					{
						carryItems << pTop;                 //put on the stack the current loopItem
						pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
					}
					//if current loopItem has no more rows but carryItems list is not empty:
					if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

				}

				qDeleteAll(itemsToBeDeleted);
			}
		}
		else if (dialog.GetType() == CCloseProjectDialog::NotsaveProject)
		{
			//关闭工程不保存
			//关闭tabwidget
			for (int i = 0; i < m_pCore->GetUIMgr()->GetTabWidget()->count(); i++)
			{
				m_pCore->GetUIMgr()->GetTabWidget()->widget(i)->deleteLater();
			}

			//关闭树
			if (m_pCore)
			{
				//delete all children of parent;
				//QStandardItem * loopItem = parent; //main loop item
				QList<QStandardItem *> carryItems; //Last In First Out stack of items
				QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
				auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
				while (pTop->rowCount())
				{
					itemsToBeDeleted << pTop->takeRow(0);
					//if the row removed has children:
					if (itemsToBeDeleted.at(0)->hasChildren())
					{
						carryItems << pTop;                 //put on the stack the current loopItem
						pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
					}
					//if current loopItem has no more rows but carryItems list is not empty:
					if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

				}

				qDeleteAll(itemsToBeDeleted);
			}

			for each (auto it in m_setModules)
			{
				Q_ASSERT(it->m_pModuleInterface);
				if (it->m_pModuleInterface)
				{
					it->m_pModuleInterface->CloseProject(pDocument, &pDocument->documentElement(), szPath, false);
				}
			}
		}
		else if (dialog.GetType() == CCloseProjectDialog::CancelProject)
		{
			//取消关闭工程
			return false;
		}
	}
	else
	{
		//关闭工程不保存
		//关闭tabwidget
		for (int i = 0; i < m_pCore->GetUIMgr()->GetTabWidget()->count(); i++)
		{
			m_pCore->GetUIMgr()->GetTabWidget()->widget(i)->deleteLater();
		}

		//关闭树
		if (m_pCore)
		{
			//delete all children of parent;
			//QStandardItem * loopItem = parent; //main loop item
			QList<QStandardItem *> carryItems; //Last In First Out stack of items
			QList<QStandardItem *> itemsToBeDeleted; //List of items to be deleted
			auto pTop = (QStandardItem *)m_pCore->GetUIMgr()->GetTopItem();
			while (pTop->rowCount())
			{
				itemsToBeDeleted << pTop->takeRow(0);
				//if the row removed has children:
				if (itemsToBeDeleted.at(0)->hasChildren())
				{
					carryItems << pTop;                 //put on the stack the current loopItem
					pTop = itemsToBeDeleted.at(0);      //set the row with children as the loopItem
				}
				//if current loopItem has no more rows but carryItems list is not empty:
				if (!pTop->rowCount() && !carryItems.isEmpty()) pTop = carryItems.takeFirst();

			}

			qDeleteAll(itemsToBeDeleted);
		}
	}


	return true;
}

/*! \fn IModule *CModuleMgr::GetModule(const char *szModuleName)
********************************************************************************************************* 
** \brief CModuleMgr::GetModule 
** \details 获得模块指针
** \param szModuleName 
** \return IModule * 
** \author gw
** \date 2016年9月20日 
** \note 
********************************************************************************************************/
IModule *CModuleMgr::GetModule(const char *szModuleName)
{
	auto it = m_setModules.begin();
	for (; it != m_setModules.end(); ++it)
	{
		//不区分大小写
		if ((*it)->m_szDllName.compare(szModuleName, Qt::CaseInsensitive) == 0)
		{
			return (*it)->m_pModuleInterface;
		}
	}

	return nullptr;
}

/*! \fn bool CModuleMgr::GetModulesName(std::vector<QString> & arrNames)
********************************************************************************************************* 
** \brief CModuleMgr::GetModulesName 
** \details 
** \param arrNames 
** \return bool 
** \author gw
** \date 2017年4月23日 
** \note 
********************************************************************************************************/
bool CModuleMgr::GetModulesName(std::vector<QString> & arrNames)
{
	for each (auto var in m_setModules)
	{
		arrNames.push_back(var->m_szDllName);
	}

	return true;
}

/*! \fn void CModuleMgr::SaveDescStringPool()
********************************************************************************************************* 
** \brief CModuleMgr::SaveDescStringPool 
** \details 
** \return void 
** \author gw
** \date 2017年3月4日 
** \note 
********************************************************************************************************/
void CModuleMgr::SaveDescStringPool()
{
	QString fileName = QString("%1%2").arg("desc_string_pool").arg(".xml");

	//前置文件修改

	QString strPath = m_pCore->GetProjMgr()->GetProjectPath() + "/scada/";

	strPath = strPath + fileName;

	QFile file(strPath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QString strTmp = QString(QObject::tr("open %1 fail!!!")).arg(fileName);

		m_pCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

		return;
	}

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	writer.writeStartElement("StringPool");
	writer.writeAttribute("Description", "test");

	writer.writeAttribute("IsTranslate", QString("%1").arg("false"));
	
	for (int32u i = 0; i < m_arrStringPoolVec.size(); i++)
	{
		//去掉空字符串
		if (m_arrStringPoolVec[i].empty())
		{
			continue;
		}
		
		writer.writeStartElement("Element");

		writer.writeAttribute("OccNo", QString("%1").arg(i + 1));
		writer.writeAttribute("Description", QString("%1").arg(m_arrStringPoolVec[i].c_str()));
		writer.writeAttribute("Language", QString("%1").arg(""));

		writer.writeEndElement();
	}

	writer.writeEndElement();
	writer.writeEndDocument();
}
