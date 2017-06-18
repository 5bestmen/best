

#include "db_svc.h"
#include <QMessageBox>
#include <QHostAddress>
#include <QDomDocument>
#include <QHostInfo>
#include <QFile>

/*! \fn QString CDbSvc::GetNodeInfo(const char* szName)
********************************************************************************************************* 
** \brief CFesSvc::GetNodeInfo 
** \details 
** \param szName            工程路径名称
** \return QString          获取节点属性名称
** \author xingzhibing
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetNodeInfo(const char* szName)
{
	bool bRet = false;

	Q_ASSERT(szName);

	if (!szName)
	{
		bRet = false;
	}

	Q_ASSERT(strlen(szName) > 0);
	if (strlen(szName)<=0)
	{
		LogMsg("node name err!", 1);
		return  false;
	}

	QDomDocument doc;

	QFile file(szName);

	if (!file.open(QIODevice::ReadOnly))
	{
		LogMsg("node：file open err !", 0);
		return  false;
	}
	

	if (!doc.setContent(&file)) 
	{
		file.close();
		LogMsg("node：file format  err !", 0);
		Q_ASSERT(false);
		return false;
	}

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName()!="Project")
	{
		LogMsg("node：file can not get project tag !", 0);
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
			for (QDomNode nGrandChild = n.firstChild(); !nGrandChild.isNull();nGrandChild=nGrandChild.nextSibling())
			{
				QDomElement nGC = nGrandChild.toElement();

				m_szNodeName = nGC.attribute("file");

				bRet = true;
			}			
		}
		//获取前置配置信息
		if (n.nodeName()=="fes")
		{
			for(QDomNode nGrandChild = n.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				QDomElement nGC = nGrandChild.toElement();

				QString szFileName = nGC.attribute("name");

				if (!szFileName.isEmpty())
				{
					m_VecFesFileName.push_back(szFileName);
				}
				else
				{
					bRet = false;
					Q_ASSERT(bRet);
					LogMsg("node:parase fes File name error!", 0);
				}
				bRet = true;
			}
		}
	}

	file.close();

	return bRet;
}

/*! \fn bool CDbSvc::GetFesInfo(const char* szName)
********************************************************************************************************* 
** \brief CFesSvc::GetFesInfo 
** \details 
** \param szName 
** \return QString 
** \author xingzhibing
** \date 2016年10月8日 
** \note 
********************************************************************************************************/
bool CDbSvc::GetFesInfo(const char* szName)
{
	Q_ASSERT(szName);

	if (!szName)
	{
		Q_ASSERT(false);
		LogMsg("fesinfo:name error !", 0);
		return false;
	}

	Q_ASSERT(strlen(szName) > 0);
	if (strlen(szName) <= 0)
	{
		Q_ASSERT(false);
		LogMsg("fesinfo:name error !", 0);
		return false;
	}

	QDomDocument doc;

	QFile file(szName);

	if (!file.open(QIODevice::ReadOnly))
	{
		LogMsg("fesinfo:file open error !", 0);
		return false;
	}
		

	if (!doc.setContent(&file))
	{
		file.close();
		LogMsg("fesinfo:file format error !", 0);
		Q_ASSERT(false);
		return false;
	}
	//读取本机名称和下面信息对应
	QString szRealHostName = QHostInfo::localHostName();

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName() != "node")
	{
		file.close();
		LogMsg("fesinfo: can not get node info!", 0);
		Q_ASSERT(false);
		return false;
	}

	bool bRet = false;

	QDomNode nChild = docElem.firstChild();

	for (; !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement n = nChild.toElement();

		//读取到前置节点组信息
		if (n.nodeName() == "nfes")
		{
			for (QDomNode nGrandChild = n.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				//读取到前置组信息
				QDomElement nGC = nGrandChild.toElement();

				if (nGC.tagName()=="group")
				{
					for (QDomNode nThirdChild = nGC.firstChild(); !nThirdChild.isNull(); nThirdChild = nThirdChild.nextSibling())
					{
						bRet = IsNodeAdded(szRealHostName, nThirdChild.toElement());
						if (bRet)
						{
							file.close();
							return bRet;
						}
					}
				}
				else if (nGC.tagName()=="n")
				{
					bRet = IsNodeAdded(szRealHostName, nGC);
					if (bRet)
					{
						file.close();
						return bRet;
					}
				}				
			}
		}
	}
	QMessageBox::information(nullptr, QObject::tr("node_info"), QObject::tr("can not find this node "));
	LogMsg(("Error In node.xml ,can't find fes tagname info"), 0);
	return bRet;
}

bool CDbSvc::IsNodeAdded(const QString& szRealHostName, QDomElement nEle)
{
	QDomElement nTC = nEle.toElement();

	QString szHostInfo = nTC.attribute("HostName");

	int32u nMyOccNo = nTC.attribute("OccNo").toUInt();

	if (szHostInfo == szRealHostName)
	{
		m_nMyNodeOccNo = nMyOccNo;
		QString szCurFesInfoName = nTC.attribute("Config");
		if (!szCurFesInfoName.isEmpty())
		{
			//读取到为配置的fes tagname 信息
			if (m_VecFesFileName.contains(szCurFesInfoName))
			{
				m_szCurFesInfoName = szCurFesInfoName + ".xml";
				return true;
			}
			else
			{
			//	Q_ASSERT(false);
				//节点尚未配置
				QMessageBox::information(nullptr, QObject::tr("fes_info"), QObject::tr("this node isn't in ordered_list"));
				LogMsg(("can't find fes tagname info in proj's fes list"), 0);
				return false;
			}
		}
		else
		{
			//Q_ASSERT(false);
			QMessageBox::information(nullptr, QObject::tr("node_info"), QObject::tr("can not find this node "));
			LogMsg(("Error In node.xml ,can't find fes tagname info"), 1);
			return false;
		}
	}
	return false;
}

bool CDbSvc::GetTotalNodeInfo(const char* szName)
{
	Q_ASSERT(szName);

	if (!szName)
	{
		return false;
	}

	Q_ASSERT(strlen(szName) > 0);
	if (strlen(szName) <= 0)
	{
		return false;
	}

	QDomDocument doc;

	QFile file(szName);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	if (!doc.setContent(&file))
	{
		file.close();
		Q_ASSERT(false);
		return false;
	}
	//读取本机名称和下面信息对应
	QString szRealHostName = QHostInfo::localHostName();

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName() != "node")
	{
		file.close();
		Q_ASSERT(false);
		return false;
	}

	QDomNode nChild = docElem.firstChild();

	std::shared_ptr<NODE> pNode = Q_NULLPTR;
	TAG_OCCNO_DEF pNodeTag = Q_NULLPTR;
	//读取server节点信息
	for (; !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement firstDom = nChild.toElement();


		if (firstDom.nodeName() == "nserver")
		{
			QDomElement secondDom = firstDom.firstChild().toElement();

			if (secondDom.tagName() == "group")
			{
				for (QDomNode nGrandChild = secondDom.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
				{
					QDomElement pEle = nGrandChild.toElement();

					pNode = std::make_shared<NODE>();
					pNodeTag = std::make_shared<TAG_OCCNO>();

					pNode->OccNo = pEle.attribute("OccNo").toULong();
					pNode->GrpNo = pEle.attribute("GroupNo").toULong();
					pNode->NameOccNo = pEle.attribute("NameOccNo").toULong();
					pNode->NodeType = pEle.attribute("NodeType").toInt();
					pNode->NetAIPAddr = QHostAddress(pEle.attribute("Network_IP_A").toStdString().c_str()).toIPv4Address();
					pNode->NetBIPAddr = QHostAddress(pEle.attribute("Network_IP_B").toStdString().c_str()).toIPv4Address();
					strncpy(pNode->TagName, pEle.attribute("TagName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t( pEle.attribute("TagName").size())));
					strncpy(pNode->HostName, pEle.attribute("HostName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("HostName").size())));
					m_arrTempNodes.push_back(pNode);

					pNodeTag->OccNo = pNode->OccNo;
					strncpy(pNodeTag->TagName, pNode->TagName,
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD),strlen(pNode->TagName)));
					
					m_arrNodeOccNos.push_back(pNodeTag);
				}
			}		
		}

		//读取到前置节点组信息
		if (firstDom.nodeName() == "nfes")
		{
			for (QDomNode nGrandChild = firstDom.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				//读取到前置组信息
				QDomElement nGC = nGrandChild.toElement();

				if (nGC.tagName()=="group")
				{
					for (QDomNode nThirdChild = nGC.firstChild(); !nThirdChild.isNull(); nThirdChild = nThirdChild.nextSibling())
					{
						QDomElement pEle = nThirdChild.toElement();

						pNode = std::make_shared<NODE>();
						pNodeTag = std::make_shared<TAG_OCCNO>();

						pNode->OccNo = pEle.attribute("OccNo").toULong();
						pNode->GrpNo = pEle.attribute("GroupNo").toULong();
						pNode->NameOccNo = pEle.attribute("NameOccNo").toULong();
						pNode->NodeType = pEle.attribute("NodeType").toInt();
						pNode->NetAIPAddr = QHostAddress(pEle.attribute("Network_IP_A").toStdString().c_str()).toIPv4Address();
						pNode->NetBIPAddr = QHostAddress(pEle.attribute("Network_IP_B").toStdString().c_str()).toIPv4Address();
						strncpy(pNode->TagName, pEle.attribute("TagName").toStdString().c_str(),
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("TagName").size())));
						strncpy(pNode->HostName, pEle.attribute("HostName").toStdString().c_str(),
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("HostName").size())));
						pNode->State = 1;
						pNode->ScanEnable = SCAN_IN;
						m_arrTempNodes.push_back(pNode);

						pNodeTag->OccNo = pNode->OccNo;
						strncpy(pNodeTag->TagName, pNode->TagName,
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), strlen(pNode->TagName)));

						m_arrNodeOccNos.push_back(pNodeTag);

					}
				}	
				else if (nGC.tagName() == "n")
				{
					QDomElement pEle = nGC;

					pNode = std::make_shared<NODE>();
					pNodeTag = std::make_shared<TAG_OCCNO>();

					pNode->OccNo = pEle.attribute("OccNo").toULong();
					pNode->GrpNo = pEle.attribute("GroupNo").toULong();
					pNode->NameOccNo = pEle.attribute("NameOccNo").toULong();
					pNode->NodeType = pEle.attribute("NodeType").toInt();
					pNode->NetAIPAddr = QHostAddress(pEle.attribute("Network_IP_A").toStdString().c_str()).toIPv4Address();
					pNode->NetBIPAddr = QHostAddress(pEle.attribute("Network_IP_B").toStdString().c_str()).toIPv4Address();
					strncpy(pNode->TagName, pEle.attribute("TagName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("TagName").size())));
					strncpy(pNode->HostName, pEle.attribute("HostName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("HostName").size())));
					pNode->State = 1;
					pNode->ScanEnable = SCAN_IN;
					m_arrTempNodes.push_back(pNode);

					pNodeTag->OccNo = pNode->OccNo;
					strncpy(pNodeTag->TagName, pNode->TagName,
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), strlen(pNode->TagName)));

					m_arrNodeOccNos.push_back(pNodeTag);
				}
			}
		}
		
		if (firstDom.nodeName() == "nworksstation")
		{
			for (QDomNode nGrandChild = firstDom.firstChild(); !nGrandChild.isNull(); nGrandChild = nGrandChild.nextSibling())
			{
				//读取到前置组信息
				QDomElement nGC = nGrandChild.toElement();

				if (nGC.tagName() == "group")
				{
					for (QDomNode nThirdChild = nGC.firstChild(); !nThirdChild.isNull(); nThirdChild = nThirdChild.nextSibling())
					{
						QDomElement pEle = nThirdChild.toElement();

						pNode = std::make_shared<NODE>();
						pNodeTag = std::make_shared<TAG_OCCNO>();

						pNode->OccNo = pEle.attribute("OccNo").toULong();
						pNode->GrpNo = pEle.attribute("GroupNo").toULong();
						pNode->NameOccNo = pEle.attribute("NameOccNo").toULong();
						pNode->NodeType = pEle.attribute("NodeType").toInt();
						pNode->NetAIPAddr = QHostAddress(pEle.attribute("Network_IP_A").toStdString().c_str()).toIPv4Address();
						pNode->NetBIPAddr = QHostAddress(pEle.attribute("Network_IP_B").toStdString().c_str()).toIPv4Address();
						strncpy(pNode->TagName, pEle.attribute("TagName").toStdString().c_str(),
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("TagName").size())));
						strncpy(pNode->HostName, pEle.attribute("HostName").toStdString().c_str(),
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("HostName").size())));

						m_arrTempNodes.push_back(pNode);

						pNodeTag->OccNo = pNode->OccNo;
						strncpy(pNodeTag->TagName, pNode->TagName,
							qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), strlen(pNode->TagName)));

						m_arrNodeOccNos.push_back(pNodeTag);
					}
				}
				else if (nGC.tagName() == "n")
				{
					QDomElement pEle = nGC;

					pNode = std::make_shared<NODE>();
					pNodeTag = std::make_shared<TAG_OCCNO>();

					pNode->OccNo = pEle.attribute("OccNo").toULong();
					pNode->GrpNo = pEle.attribute("GroupNo").toULong();
					pNode->NameOccNo = pEle.attribute("NameOccNo").toULong();
					pNode->NodeType = pEle.attribute("NodeType").toInt();
					pNode->NetAIPAddr = QHostAddress(pEle.attribute("Network_IP_A").toStdString().c_str()).toIPv4Address();
					pNode->NetBIPAddr = QHostAddress(pEle.attribute("Network_IP_B").toStdString().c_str()).toIPv4Address();
					strncpy(pNode->TagName, pEle.attribute("TagName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("TagName").size())));
					strncpy(pNode->HostName, pEle.attribute("HostName").toStdString().c_str(),
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), size_t(pEle.attribute("HostName").size())));

					m_arrTempNodes.push_back(pNode);

					pNodeTag->OccNo = pNode->OccNo;
					strncpy(pNodeTag->TagName, pNode->TagName,
						qMin(size_t(MAX_NAME_LENGTH + STRING_PAD), strlen(pNode->TagName)));

					m_arrNodeOccNos.push_back(pNodeTag);
				}
			}
		}
	}
	file.close();
	return true;
}