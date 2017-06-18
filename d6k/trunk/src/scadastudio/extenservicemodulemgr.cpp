/*! @file
<PRE>
********************************************************************************
模块名       :  管理模块
文件名       :  extenservicemodulemgr.cpp
文件实现功能 :  管理扩展服务模块的模块
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
*  @date	2016.12.1
*/
#include "extenservicemodulemgr.h"
#include "scadastudio/uimgr.h"
#include "scadastudio/icore.h"
#include "scadastudio/imodule.h"
#include "fesmodule.h"
#include "scadastudio/define.h"
#include "scadastudio/treewidget.h"
#include "closeprojectdialog.h"
#include "scadastudio/tabwidget.h"
#include "nodeparamodule.h"

#include "nodeparamodule.h"
#include "sysmodule.h"
#include "usermodule.h"

#include "scadastudio/quoteItem.h"
#include "extendservicemodule.h"
#include "scadastudiodefine.h"

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


CExtendServiceModuleMgr::CExtendServiceModuleMgr(IMainModuleInterface *pCore)
	:m_pCore(pCore), m_pDocument(nullptr), m_pRoot(nullptr), m_szPath(""), m_pTopItem(nullptr)
	, m_pExtendServiceModule(nullptr)
{
	Q_ASSERT(pCore);

	m_pExtendServiceModule = new CExtendServiceModule;
	m_pExtendServiceModule->Init(m_pCore);
}

CExtendServiceModuleMgr::~CExtendServiceModuleMgr()
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

	if (m_pExtendServiceModule)
	{
		delete m_pExtendServiceModule;
		m_pExtendServiceModule = nullptr;
	}
}

/*! \fn bool CExtendServiceModuleMgr::LoadProjFromFile(QDomDocument *pDocument
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::LoadProjFromFile 
** \details 加载工程文件
** \param pDocument 
** \return bool 
** \author gw
** \date 2016年4月13日 
** \note 
********************************************************************************************************/
bool CExtendServiceModuleMgr::LoadProjFromFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath)
{
	Q_ASSERT(pDocument);
	Q_ASSERT(szPath.isEmpty() == false);
	Q_ASSERT(pRoot);
	if (!pDocument)
	{
		return false;
	}
	if (!pRoot)
	{
		return false;
	}
	m_pDocument = pDocument;
	m_szPath = szPath;
	m_pRoot = pRoot;

	auto pTop = m_pCore->GetUIMgr()->GetTopItem();
	Q_ASSERT(pTop);
	//添加外部服务模块
	auto pExtendServiceItem = new CQuoteItem(QObject::tr("extend service"));
	pExtendServiceItem->setData(EXTEND_SERVICE_ITEM, Qt::UserRole);
	pExtendServiceItem->setIcon(QIcon(FES_PNG));
	pExtendServiceItem->setEditable(false);

	//获取外部服务节点项
	m_pCore->GetUIMgr()->SetExtendServiceTopItem(pExtendServiceItem);

	pTop->appendRow(pExtendServiceItem);
	m_pTopItem = pExtendServiceItem;

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

	m_pCore->GetUIMgr()->GetLeftTree()->expand(m_pCore->GetUIMgr()->GetLeftTreeModel()->indexFromItem((QStandardItem *)pTop));

	return true;
}
/*! \fn bool CExtendServiceModuleMgr::LoadInternalModule()
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::LoadInternalModule 
** \details 加载内部模块
** \return bool 
** \author gw
** \date 2016年6月3日 
** \note 
********************************************************************************************************/
bool CExtendServiceModuleMgr::LoadInternalModule()
{	


	return true;
}
/*! \fn bool CExtendServiceModuleMgr::LoadPlugInXml()
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::LoadPlugInXml 
** \details 从scadastuido.xml解析数据，并加载
** \return bool 
** \author LiJin 
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool CExtendServiceModuleMgr::LoadPlugInXml()
{
	QString szXmlPath = qApp->applicationDirPath();	
	szXmlPath += "/";

	szXmlPath += EXTENDSERVICEPLUGIN_XML;

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
			else if (xmlReader.name().toString() == "eitem")
			{
				QString szID = xmlReader.attributes().value("id").toString();
				QString szName = xmlReader.attributes().value("name").toString();
				QString szCommon = xmlReader.attributes().value("descript").toString();
				QString szDll = xmlReader.attributes().value("dllname").toString();
				QString szType = xmlReader.attributes().value("type").toString();
				QString szVersion = xmlReader.attributes().value("version").toString();

				std::shared_ptr <CExtendServiceModuleItem> pModule = std::make_shared<CExtendServiceModuleItem>(true);
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

void CExtendServiceModuleMgr::AddExternalModule(std::shared_ptr<CExtendServiceModuleItem>pModule)
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

void CExtendServiceModuleMgr::LoadModules()
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

void CExtendServiceModuleMgr::UnLoadModules()
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

bool  CExtendServiceModuleMgr::LogString(const char *szLogTxt, int nLevel)
{
	Q_ASSERT(m_pCore);
	if (m_pCore == nullptr)
		return false;

	m_pCore->LogMsg("scadastudio", szLogTxt, nLevel);

	return true;
}
/*! \fn bool CExtendServiceModuleMgr::SaveProjFromFile(QDomDocument *pDocument)
*********************************************************************************************************
** \brief CExtendServiceModuleMgr::SaveProjFromFile
** \details 保存工程文件 内部保存各个模块实现
** \param pDocument
** \return bool
** \author gw
** \date 2016年4月13日
** \note
********************************************************************************************************/
bool CExtendServiceModuleMgr::SaveToProjectFile(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath)
{
	Q_ASSERT(pDocument);
	Q_ASSERT(szPath.isEmpty() == false);
	if (!pDocument)
	{
		return false;
	}

	for each (auto it in m_setModules)
	{
	//	Q_ASSERT(it->m_pModuleInterface);
		if (it->m_pModuleInterface != nullptr)
		{
			it->m_pModuleInterface->SaveProject(pDocument, pRoot, szPath);
		}
	} 

	return true;
}

/*! \fn bool CExtendServiceModuleMgr::CreateNewProj(QDomDocument *pDocument, const QString& szPath)
*********************************************************************************************************
** \brief CExtendServiceModuleMgr::CreateNewProj
** \details 新建工程
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016年8月15日
** \note
********************************************************************************************************/
bool CExtendServiceModuleMgr::CreateNewProj(QDomDocument *pDocument, QDomElement *pRoot, const QString& szPath, CQuoteItem *pTopItem)
{
	Q_ASSERT(pDocument);
	if (!pDocument)
	{
		return false;
	}

	Q_ASSERT(pRoot);
	if (!pRoot)
	{
		return false;
	}

	Q_ASSERT(!szPath.isEmpty());
	if (szPath.isEmpty())
	{
		return false;
	}

	Q_ASSERT(pTopItem);
	if (!pTopItem)
	{
		return false;
	}

	m_pDocument = pDocument;
	m_pRoot = pRoot;
	m_szPath = szPath;
	m_pTopItem = pTopItem;

	auto pTop = m_pCore->GetUIMgr()->GetTopItem();
	Q_ASSERT(pTop);
	//添加外部服务模块
	auto pExtendServiceItem = new CQuoteItem(QObject::tr("extend service"));
	pExtendServiceItem->setData(EXTEND_SERVICE_ITEM, Qt::UserRole);
	pExtendServiceItem->setIcon(QIcon(FES_PNG));
	pExtendServiceItem->setEditable(false);

	//获取外部服务节点项
	m_pCore->GetUIMgr()->SetExtendServiceTopItem(pExtendServiceItem);

	pTop->appendRow(pExtendServiceItem);

	m_pTopItem = pExtendServiceItem;

	//外部服务新建不创建节点
	//for each (auto it in m_setModules)
	//{
	////	Q_ASSERT(it->m_pModuleInterface);
	//	if (it->m_pModuleInterface != nullptr)
	//	{
	//		it->m_pModuleInterface->CreateNewProject(pDocument, pRoot, szPath, pExtendServiceItem);
	//	}
	//}

	return true;
}

/*! \fn bool  CExtendServiceModuleMgr::CloseProj(QDomDocument *pDocument, const QString& szPath)
*********************************************************************************************************
** \brief CExtendServiceModuleMgr::CloseProj
** \details 关闭工程
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016年8月16日
** \note
********************************************************************************************************/
bool  CExtendServiceModuleMgr::CloseProj(QDomDocument *pDocument, const QString& szPath)
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

/*! \fn IModule *CExtendServiceModuleMgr::GetModule(const char *szModuleName)
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::GetModule 
** \details 获得模块指针
** \param szModuleName 
** \return IModule * 
** \author gw
** \date 2016年9月20日 
** \note 
********************************************************************************************************/
IModule *CExtendServiceModuleMgr::GetModule(const char *szModuleName)
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

/*! \fn CQuoteItem *CExtendServiceModuleMgr::GetTopItem()
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::GetTopItem 
** \details 获得扩展服务节点项
** \return CQuoteItem * 
** \author gw
** \date 2016年12月6日 
** \note 
********************************************************************************************************/
CQuoteItem *CExtendServiceModuleMgr::GetExtendServiceItem()
{
	Q_ASSERT(m_pTopItem);
	if (!m_pTopItem)
	{
		return nullptr;
	}

	return m_pTopItem;
}
