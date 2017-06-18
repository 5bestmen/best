/*! @file
<PRE>
********************************************************************************
ģ����       :  ����ģ��
�ļ���       :  extenservicemodulemgr.cpp
�ļ�ʵ�ֹ��� :  ������չ����ģ���ģ��
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

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
** \details ���ع����ļ�
** \param pDocument 
** \return bool 
** \author gw
** \date 2016��4��13�� 
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
	//����ⲿ����ģ��
	auto pExtendServiceItem = new CQuoteItem(QObject::tr("extend service"));
	pExtendServiceItem->setData(EXTEND_SERVICE_ITEM, Qt::UserRole);
	pExtendServiceItem->setIcon(QIcon(FES_PNG));
	pExtendServiceItem->setEditable(false);

	//��ȡ�ⲿ����ڵ���
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
    //�����ⲿģ��
	for each (auto it in m_arrExtModules)
	{
		if (it->m_pModuleInterface != nullptr)
		{
			it->m_pModuleInterface->LoadProject(pDocument, pRoot, szPath);
		}		
	}

	//�����ڲ�ģ��
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
** \details �����ڲ�ģ��
** \return bool 
** \author gw
** \date 2016��6��3�� 
** \note 
********************************************************************************************************/
bool CExtendServiceModuleMgr::LoadInternalModule()
{	


	return true;
}
/*! \fn bool CExtendServiceModuleMgr::LoadPlugInXml()
********************************************************************************************************* 
** \brief CExtendServiceModuleMgr::LoadPlugInXml 
** \details ��scadastuido.xml�������ݣ�������
** \return bool 
** \author LiJin 
** \date 2016��8��31�� 
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
		// log��
		QString szLog = "Open [ " + szXmlPath + " ] failed";

		LogString(szLog.toStdString().c_str(), 1);
		return false;
	}
	// ��������
	QXmlStreamReader xmlReader(&PluginXml);
	QString strTmp;
	while (!xmlReader.atEnd() && (!xmlReader.hasError()))
	{
		//������������
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

				// ���ң����ֲ����ظ�
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
		{// ����
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
							// log ���سɹ�
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
				{// log ��ӡ
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
						// log ���سɹ�
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
			{// log ��ӡ
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
** \details ���湤���ļ� �ڲ��������ģ��ʵ��
** \param pDocument
** \return bool
** \author gw
** \date 2016��4��13��
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
** \details �½�����
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016��8��15��
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
	//����ⲿ����ģ��
	auto pExtendServiceItem = new CQuoteItem(QObject::tr("extend service"));
	pExtendServiceItem->setData(EXTEND_SERVICE_ITEM, Qt::UserRole);
	pExtendServiceItem->setIcon(QIcon(FES_PNG));
	pExtendServiceItem->setEditable(false);

	//��ȡ�ⲿ����ڵ���
	m_pCore->GetUIMgr()->SetExtendServiceTopItem(pExtendServiceItem);

	pTop->appendRow(pExtendServiceItem);

	m_pTopItem = pExtendServiceItem;

	//�ⲿ�����½��������ڵ�
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
** \details �رչ���
** \param pDocument
** \param szPath
** \return bool
** \author gw
** \date 2016��8��16��
** \note
********************************************************************************************************/
bool  CExtendServiceModuleMgr::CloseProj(QDomDocument *pDocument, const QString& szPath)
{
	Q_ASSERT(pDocument);

	if (!pDocument)
	{
		return false;
	}

	//�ж��Ƿ����޸�
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
			//�رչ��̲�����
			//�ر�tabwidget
			for (int i = 0; i < m_pCore->GetUIMgr()->GetTabWidget()->count(); i++)
			{
				m_pCore->GetUIMgr()->GetTabWidget()->widget(i)->deleteLater();
			}

			//�ر���
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
			//ȡ���رչ���
			return false;
		}
	}
	else
	{
		//�رչ��̲�����
		//�ر�tabwidget
		for (int i = 0; i < m_pCore->GetUIMgr()->GetTabWidget()->count(); i++)
		{
			m_pCore->GetUIMgr()->GetTabWidget()->widget(i)->deleteLater();
		}

		//�ر���
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
** \details ���ģ��ָ��
** \param szModuleName 
** \return IModule * 
** \author gw
** \date 2016��9��20�� 
** \note 
********************************************************************************************************/
IModule *CExtendServiceModuleMgr::GetModule(const char *szModuleName)
{
	auto it = m_setModules.begin();
	for (; it != m_setModules.end(); ++it)
	{
		//�����ִ�Сд
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
** \details �����չ����ڵ���
** \return CQuoteItem * 
** \author gw
** \date 2016��12��6�� 
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
