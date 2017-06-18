/*! @file loadfile.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  loadfile.cpp
文件实现功能 :  加载配置文件
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   加载配置文件
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.12.26
*******************************************************************************/

#include "db_svc.h"
#include "log/log.h"
#include "fesapi/fes_magicmem.h"
#include "scadaapi/scdapp_def.h"
#include <QFile>
#include <QFileInfo>
#include <QObject> 
#include <QString> 
#include <QDomDocument>
#include <QHostAddress>
#include <QHostInfo>
#include <QString>
#include <QByteArray>

typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;

/*! \fn bool CDbSvc::LoadProject(const char *pszFilePath)
*********************************************************************************************************
** \brief CDbSvc::LoadProject
** \details 加载工程配置文件proj，解析
** \param pszFilePath
** \return bool
** \author LiJin
** \date 2016年11月3日
** \note
********************************************************************************************************/
bool CDbSvc::LoadProject(const char *pszFilePath)
{
	Q_ASSERT(pszFilePath != nullptr);
	if (pszFilePath == nullptr)
		return false;
	Q_ASSERT(strlen(pszFilePath));
	if (strlen(pszFilePath) < 2)
		return false;

	bool bRet = false;

	//node name
	QString szNodeName;

	QDomDocument doc;

	QFile file(pszFilePath);

	if (!file.open(QIODevice::ReadOnly))
		return  false;

	if (!doc.setContent(&file))
	{
		file.close();
		Q_ASSERT(false);
		return false;
	}

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName() != "Project")
	{
		file.close();
		return false;
	}

	QDomNode nChild = docElem.firstChild();

	for (; !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement n = nChild.toElement();

		//获取节点配置信息
		if (n.nodeName() == "node")
		{
			for (QDomNode nGrandChild = n.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				QDomElement nGC = nGrandChild.toElement();

				szNodeName = nGC.attribute("file");

				Q_ASSERT(!szNodeName.isEmpty());

				if (szNodeName.isEmpty())
				{
					LogMsg(QObject::tr("project: can not find node file name ").toStdString().c_str(),0);
					bRet = false;
					return bRet;
				}

				bRet = true;
			}
		}
		//获取前置配置信息
		if (n.nodeName() == "fes")
		{
			for (QDomNode nGrandChild = n.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				QDomElement nGC = nGrandChild.toElement();

				QString szFileName = nGC.attribute("name");

				if (!szFileName.isEmpty())
				{
					m_arrTempFesTagNames.push_back(szFileName.toStdString());
				}
				else
				{
					bRet = false;
					Q_ASSERT(bRet);
					LogMsg(QObject::tr("Parse Fes File name error!").toStdString().c_str(), 0);
				}
				bRet = true;
			}
		}
	}

	file.close();

	QString szPath = QFileInfo(pszFilePath).path();

	QString szPathName = szPath + "/node/" + szNodeName;

	bRet = LoadNodeFile(szPathName.toStdString().c_str());

	Q_ASSERT(bRet);
	//fes
	std::map<int32u, QString>::iterator it = m_mapFesConfig.begin();
	for (;it!=m_mapFesConfig.end();++it)
	{
		QString  str = it->second;
		it->second = szPath + "/scada/" + str+".xml";
	}
	//server
	std::map<int32u, QString >::iterator it2 = m_mapScdConfig.begin();
	for (;it2!=m_mapScdConfig.end();++it2)
	{
		QString  str = it2->second;
		it2->second = szPath + "/scadavariable/" + str + ".xml";
	}
	//workstation
	std::map<int32u, QString >::iterator it3 = m_mapWorkStationConfig.begin();
	for (; it3 != m_mapWorkStationConfig.end(); ++it3)
	{
		QString  str = it3->second;
		it3->second = szPath + "/scadavariable/" + str + ".xml";
	}
	return bRet;

}

/*! \fn bool CDbSvc::LoadNodeFile(const char *pszFilePath)
********************************************************************************************************* 
** \brief CDbSvc::LoadNodeFile 
** \details 加载节点配置文件
** \param pszFilePath 
** \return bool 
** \author xingzhibing
** \date 2016年12月21日 
** \note 
********************************************************************************************************/
bool CDbSvc::LoadNodeFile(const char *pszFilePath)
{
	Q_ASSERT(pszFilePath != nullptr);
	if (pszFilePath == nullptr)

		return false;
	Q_ASSERT(strlen(pszFilePath));
	if (strlen(pszFilePath) < 2)
		return false;

	QFile file(pszFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		Q_ASSERT(false);
		LogMsg(QObject::tr("node.xml:open file error ").toStdString().c_str(), 0);
		return false;
	}
	QDomDocument doc;
	if (!doc.setContent(&file))
	{
		Q_ASSERT(false);
		LogMsg(QObject::tr("node.xml:set file content error,file format error").toStdString().c_str(), 0);
		return false;
	}

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName() != "node")
	{
		file.close();
		return false;
	}

	QDomNode nNode = docElem.firstChild();

	NODE_GROUP_DEF pGrp;
	for (;!nNode.isNull();nNode=nNode.nextSibling())
	{
		QString tagName = nNode.nodeName();
		if (tagName=="nserver")
		{
			m_DomServer = nNode.toElement();

			Q_ASSERT(!m_DomServer.isNull());

			if (m_DomServer.isNull())
			{
				LogMsg(QObject::tr("node.xml:server tag  error").toStdString().c_str(), 0);
				return false;
			}

			pGrp = std::make_shared<CNodeGroup>(tagName.toStdString());

			if (!LoadServer(m_DomServer,pGrp))
			{
				return false;
			}
			m_pNodeGrpMgr->AddGroup(NODE_SVR,pGrp);
		}
		else if (tagName=="nfes")
		{
			m_DomFes = nNode.toElement();

			Q_ASSERT(!m_DomFes.isNull());

			if (m_DomFes.isNull())
			{
				LogMsg(QObject::tr("node.xml:fes tag  error").toStdString().c_str(), 0);
				return false;
			}

			pGrp = std::make_shared<CNodeGroup>(tagName.toStdString());

			if (!LoadClient(m_DomFes, pGrp))
			{
				return false;
			}
			m_pNodeGrpMgr->AddGroup(NODE_FES, pGrp);
		}
		else if (tagName=="nworksstation")
		{
			m_DomClient = nNode.toElement();

			Q_ASSERT(!m_DomClient.isNull());

			if (m_DomClient.isNull())
			{
				LogMsg(QObject::tr("node.xml:client  tag  error").toStdString().c_str(), 0);
				return false;
			}

			pGrp = std::make_shared<CNodeGroup>(tagName.toStdString());

			if (!LoadFes(m_DomClient, pGrp))
			{
				return false;
			}
			m_pNodeGrpMgr->AddGroup(NODE_CLIENT, pGrp);
		}		
	}
	return true;
}

bool CDbSvc::LoadServer(QDomElement nEle, NODE_GROUP_DEF pGrp)
{
	Q_ASSERT(!nEle.isNull()  && pGrp );

	if (nEle.isNull() || !pGrp)
	{
		return false;
	}
	QDomNode nNode = nEle.firstChild();

	NODE_GROUP_DEF ppGrp = std::make_shared<CNodeGroup>(nEle.tagName().toStdString());

	for (;!nNode.isNull();nNode=nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();
		if (pData.tagName()=="group")
		{
			LoadGroup(pData, ppGrp);
		}
	}

	pGrp->AddGrp(ppGrp);

	return true;
}

bool CDbSvc::LoadFes(QDomElement nEle, NODE_GROUP_DEF pGrp)
{
	Q_ASSERT(!nEle.isNull() && pGrp);

	if (nEle.isNull() || !pGrp)
	{
		return false;
	}
	QDomNode nNode = nEle.firstChild();

	NODE_GROUP_DEF ppGrp;

	for (; !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();

		if (pData.tagName() == "group")
		{
			ppGrp = std::make_shared<CNodeGroup>(pData.attribute("name").toStdString());

			LoadGroup(pData, ppGrp);

			pGrp->AddGrp(ppGrp);
		}
		else if (pData.tagName()=="n")
		{
			LoadNode(pData, pGrp);
		}
	}
	return true;
}

bool CDbSvc::LoadClient(QDomElement nEle, NODE_GROUP_DEF pGrp)
{
	Q_ASSERT(!nEle.isNull() && pGrp);

	if (nEle.isNull() || !pGrp)
	{
		return false;
	}
	QDomNode nNode = nEle.firstChild();

	NODE_GROUP_DEF ppGrp;

	for (; !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();

		if (pData.tagName() == "group")
		{
			ppGrp = std::make_shared<CNodeGroup>(pData.attribute("name").toStdString());

			LoadGroup(pData, ppGrp);

			pGrp->AddGrp(ppGrp);

		}
		if (pData.tagName() == "n")
		{
			LoadNode(pData, pGrp);
		}
	}
	return true;
}

bool CDbSvc::LoadGroup(QDomElement nEle, NODE_GROUP_DEF pGrp)
{
	Q_ASSERT(!nEle.isNull() && pGrp);

	if (nEle.isNull() || !pGrp)
	{
		return false;
	}
	QDomNode nNode = nEle.firstChild();

	for (;!nNode.isNull();nNode=nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();

		if (pData.tagName()=="n")
		{
			LoadNode(pData, pGrp);
		}
	}

	return true;
}

bool CDbSvc::CheckOccNoExist(int32u nOccNo)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <MAX_NODE_OCCNO);

	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_NODE_OCCNO)
	{
		return false;
	}

	auto it = m_MapID2String.find(nOccNo);

	if (it== m_MapID2String.end())
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool CDbSvc::LoadNode(QDomElement nEle, NODE_GROUP_DEF pGrp)
{
	Q_ASSERT(!nEle.isNull() && pGrp);

	if (nEle.isNull() || !pGrp)
	{
		return false;
	}
	char buf[MAX_NAME_LENGTH];
	memset(buf, 0, MAX_NAME_LENGTH);

	NODE_DEF pNode = std::make_shared<NODE>();

	TAG_OCCNO_DEF pTag = nullptr;
	
	pNode->OccNo = nEle.attribute("OccNo").toULong();
	pNode->GrpNo = nEle.attribute("GrpNo").toULong();
	pNode->NetAIPAddr = QHostAddress(nEle.attribute("Network_IP_A")).toIPv4Address();
	pNode->NetBIPAddr = QHostAddress(nEle.attribute("Network_IP_B")).toIPv4Address();
	pNode->NodeType = nEle.attribute("NodeType").toULong();
	pNode->SlaveOccNo = nEle.attribute("SlaveOccNo").toULong();
	strncpy(pNode->TagName, nEle.attribute("TagName").toLocal8Bit().data(), qMin((size_t)(MAX_NAME_LENGTH + STRING_PAD), (size_t)nEle.attribute("TagName").length()));
	strncpy(pNode->HostName, nEle.attribute("HostName").toLocal8Bit().data(), qMin((size_t)(MAX_NAME_LENGTH + STRING_PAD), (size_t)nEle.attribute("HostName").length()));
	strncpy(pNode->GroupName, nEle.attribute("GrpName").toLocal8Bit().data(), qMin((size_t)nEle.attribute("GrpName").length(), (size_t)(MAX_NAME_LENGTH + STRING_PAD)));

	if (pNode->NodeType==NODE_FES)
	{
		pTag = std::make_shared<TAG_OCCNO>();
		//判断主次节点,只需要加载一次
		if (pNode->SlaveOccNo)
		{
			strncpy(buf, nEle.attribute("Config").toLocal8Bit().data(),
				qMin((size_t)nEle.attribute("Config").length(), (size_t)(MAX_NAME_LENGTH + STRING_PAD)));

			bool bRet = CheckOccNoExist(pNode->OccNo);
			if (!bRet)
			{
				pTag->OccNo = pNode->OccNo;
				strncpy(pTag->TagName, pNode->TagName, qMin(MAX_NAME_LENGTH + STRING_PAD, strlen(pNode->TagName)));

				m_arrNodeTagName.push_back(pTag);

				m_mapFesConfig.insert(std::make_pair(pNode->OccNo, buf));
				m_MapID2String.insert(std::make_pair(pNode->OccNo, pNode->TagName));
				m_MapString2ID.insert(std::make_pair(pNode->TagName, pNode->OccNo));
			}
			else
			{
				LogMsg(QObject::tr("scdsvc:nodefes occno dumplicate").toStdString().c_str(), 0);
			}			
		}		
	}else if (pNode->NodeType==NODE_SVR)
	{
		pTag = std::make_shared<TAG_OCCNO>();

		QString localHostName = QHostInfo::localHostName();

		if (localHostName == pNode->HostName)
		{
			m_nMyNodeOccNo = pNode->OccNo;
		}

		//判断主次节点,只需要加载一次
		if (pNode->SlaveOccNo)
		{
			strncpy(buf, nEle.attribute("Config").toLocal8Bit().data(),
				qMin((size_t)nEle.attribute("Config").length(), (size_t)(MAX_NAME_LENGTH + STRING_PAD)));

			bool bRet = CheckOccNoExist(pNode->OccNo);
			if (!bRet)
			{
				pTag->OccNo = pNode->OccNo;
				strncpy(pTag->TagName, pNode->TagName, qMin(MAX_NAME_LENGTH + STRING_PAD, strlen(pNode->TagName)));

				m_arrNodeTagName.push_back(pTag);

				LoadApps(nEle,pNode->OccNo);

				m_mapScdConfig.insert(std::make_pair(pNode->OccNo, buf));
				m_MapID2String.insert(std::make_pair(pNode->OccNo, pNode->TagName));
				m_MapString2ID.insert(std::make_pair(pNode->TagName, pNode->OccNo));
			}
			else
			{
				LogMsg(QObject::tr("scdsvc:nodesvr occno dumplicate").toStdString().c_str(), 0);
			}
		}
	}else if (pNode->NodeType==NODE_CLIENT)
	{
		pTag = std::make_shared<TAG_OCCNO>();
		
		//判断主次节点,只需要加载一次
		if (pNode->SlaveOccNo)
		{
			strncpy(buf, nEle.attribute("Config").toLocal8Bit().data(),
				qMin((size_t)nEle.attribute("Config").length(), (size_t)(MAX_NAME_LENGTH + STRING_PAD)));

			bool bRet = CheckOccNoExist(pNode->OccNo);
			if (!bRet)
			{
				pTag->OccNo = pNode->OccNo;
				strncpy(pTag->TagName, pNode->TagName, qMin(MAX_NAME_LENGTH + STRING_PAD, strlen(pNode->TagName)));

				m_arrNodeTagName.push_back(pTag);

				m_mapWorkStationConfig.insert(std::make_pair(pNode->OccNo, buf));
				m_MapID2String.insert(std::make_pair(pNode->OccNo, pNode->TagName));
				m_MapString2ID.insert(std::make_pair(pNode->TagName, pNode->OccNo));
			}
			else
			{
				LogMsg(QObject::tr("scdsvc:nodeworkstation occno dumplicate").toStdString().c_str(), 0);
			}
		}
	}
	pGrp->AddNode(pNode);
	m_arrTempNodes.push_back(pNode);
	return true;
}

bool CDbSvc::LoadApps(QDomElement nEle, int32u nOccNo)
{
	Q_ASSERT(nOccNo !=INVALID_OCCNO && nOccNo <=MAX_OCCNO);
	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_OCCNO)
	{
		return false;
	}
	QDomNode nNode = nEle.firstChild();
	APP_INFO_DEF pAppInfo = nullptr;

	std::vector<APP_INFO_DEF> arrAppInfos;

	for (; !nNode.isNull(); nNode = nNode.nextSibling())
	{
		QDomElement pData = nNode.toElement();

		if (pData.tagName() == "m")
		{
			pAppInfo = std::make_shared<SAPP>();
			memset(pAppInfo.get(), 0, sizeof SAPP);

			pAppInfo->OccNo = pData.attribute("OccNo").toInt();
			pAppInfo->IsDefined = pData.attribute("IsDefined").toInt();
			pAppInfo->ScanEnable = true;
			pAppInfo->Init = INITED;

			pAppInfo->nLoadType = (LoadType)pData.attribute("LoadType").toInt();
			pAppInfo->nAppType = (AppType)pData.attribute("AppType").toInt();


			strncpy(pAppInfo->ProgramName,pData.attribute("name").toLocal8Bit(),
				qMin((size_t )MAX_NAME_LEN,(size_t) pData.attribute("name").length()));

			//strncpy(pAppInfo->loadMode, pData.attribute("load").toLocal8Bit(),
			//	qMin((size_t)MAX_NAME_LEN, (size_t)pData.attribute("load").length()));
			//strncpy(pAppInfo->agru, pData.attribute("argument").toLocal8Bit(),
			//	qMin((size_t)MAX_NAME_LEN, (size_t)pData.attribute("argument").length()));
			//strncpy(pAppInfo->desc, pData.attribute("description").toLocal8Bit(),
			//	qMin((size_t)MAX_NAME_LEN, (size_t)pData.attribute("description").length()));
			//pAppInfo->nDescOccNo = pData.attribute("DescriptionOccNo").toInt();		
			arrAppInfos.push_back(pAppInfo);
		}
	}
	m_mapApps.insert(std::make_pair(nOccNo, arrAppInfos));
	return true;
}