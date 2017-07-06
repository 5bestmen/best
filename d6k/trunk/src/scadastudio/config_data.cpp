/*! @file
<PRE>
********************************************************************************
模块名       :  配置结构
文件名       :  config_data.cpp
文件实现功能 :  配置结构
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
*  @date	2016.1.4
*/
#include "config_data.h"


#include "scadastudiodefine.h"
#include "scadastudio/icore.h"
#include "scadastudio/iprojmgr.h"

#include "stl_util-inl.h"
#include "scadastudio/define.h"
#include "scadastudiodefine.h"
#include "variant.h"
#include "fesapi/fescnst.h"
#include "../fespub/tag_att.h"

#include "global.h"
#include "scadastudio/imodule.h"
#include "log/log2file.h"
#include "qhostaddress.h"

#include <algorithm>
#include <QFile>
#include <QXmlStreamReader>
#include <QModelIndex>
#include <QXmlStreamWriter>
#include <QDomElement>
#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>

namespace Config
{  
	//bool CChannelData::ReadD(QXmlStreamReader &reader)
	//{
	//	using namespace Config;
	//	//while (!reader.atEnd())
	//	//{
	//	//	QString strTmp = reader.name().toString();

	//	//	if (reader.isStartElement())
	//	//	{
	//	//		if (strTmp == "d")
	//	//		{
	//	//			int32u nOccNo = reader.attributes().value("OccNo").toInt();
	//	//			Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
	//	//			if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
	//	//			{// LOG
	//	//				auto strError = QObject::tr("device OccNo is invalid!!!");
	//	//				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	//				reader.readNext();

	//	//				continue;
	//	//			}

	//	//			QString strTagName = reader.attributes().value("TagName").toString();
	//	//			Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
	//	//			if (strTagName.isEmpty() == true)
	//	//			{
	//	//				auto strError = QObject::tr("device tag name is empty!!!!!!");

	//	//				s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

	//	//				reader.readNext();

	//	//				continue;
	//	//			}

	//	//			CDevice *pDevice = new CDevice(s_pGlobleCore);
	//	//			pDevice->SetOccNo(nOccNo);
	//	//			//pDevice->m_szTagName = strTagName;
	//	//			strncpy(pDevice->m_szTagName, strTagName.toStdString().c_str(), MAX_NAME_LENGTH);
	//	//			pDevice->m_szTagName[MAX_NAME_LENGTH] = 0;

	//	//			int32u nID = reader.attributes().value("ID").toUInt();
	//	//			pDevice->m_nID = nID;

	//	//			QString strDescription = reader.attributes().value("Description").toString();
	//	//			pDevice->m_strDescription = strDescription;

	//	//			int32u DeviceModelID = reader.attributes().value("DeviceModelID").toUInt();
	//	//			pDevice->m_nDeviceModelID = DeviceModelID;

	//	//			int8u nType = reader.attributes().value("Type").toInt();
	//	//			pDevice->m_nType = nType;

	//	//			QString strDeviceAddr = reader.attributes().value("DeviceAddr").toString();
	//	//			pDevice->m_strDeviceAddr = strDeviceAddr;

	//	//			int32u nPriority = reader.attributes().value("Priority").toInt();
	//	//			pDevice->m_nPriority = nPriority;

	//	//			QString strExtent = reader.attributes().value("ExtentionParam").toString();
	//	//			pDevice->m_strExtentionParam = strExtent;

	//	//			auto bFlag = CHECK_LESS_ZERO_INT(nID);
	//	//			if (!bFlag)
	//	//			{
	//	//				auto strFesError = QString(QObject::tr("ID %1 of device %2 is invalid < 0")).arg(nID).arg(strTagName);
	//	//				s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
	//	//			}
	//	//			if (strTagName.isEmpty())
	//	//			{
	//	//				auto strFesError = QString(QObject::tr("device %1 is empty")).arg(strTagName);
	//	//				s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
	//	//			}
	//	//			else if (strTagName.count() > MAX_NAME_LENGTH)
	//	//			{
	//	//				auto strFesError = QString(QObject::tr("device %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH);
	//	//				s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
	//	//			}

	//	//			pDevice->LoadData(reader, );

	//	//			//通道挂装置
	//	//			m_arrDevices.push_back(pDevice);
	//	//		}
	//	//	}
	//	//	else if (reader.isEndElement())
	//	//	{
	//	//		break;
	//	//	}
	//	//	
	//	//	reader.readNext();
	//	//}

	//	return true;
	//} 

	///*! \fn 
	//********************************************************************************************************* 
	//** \brief Config::CDevice::ReadData 
	//** \details 
	//** \param reader 
	//** \return bool 
	//** \author gw
	//** \date 2016年9月1日 
	//** \note 
	//********************************************************************************************************/
	//bool CDevice::ReadData(QXmlStreamReader &reader)
	//{
	//	while (!reader.atEnd())
	//	{
	//		QString strTmp = reader.name().toString();

	//	//	if (reader.isStartElement())
	//	//	{
	//	//		
	//	//		QString strTmp = reader.name().toString();

	//	//		if (strTmp == "AIN")
	//	//		{
	//	//			ReadAIN(reader);
	//	//		}
	//	//		else if (strTmp == "DIN")
	//	//		{
	//	//			ReadDIN(reader);
	//	//		}
	//	//		else if (strTmp == "AOUT")
	//	//		{
	//	//			ReadAOUT(reader);
	//	//		}
	//	//		else if (strTmp == "DOUT")
	//	//		{
	//	//			ReadDOUT(reader);
	//	//		}
	//	//	}
	//	//	else if (reader.isEndElement())
	//	//	{
	//	//		break;
	//	//	}

	//	//	reader.readNext();
	//	}

	//	return true;
	//} 

	CNodeStandardTemplateServiceRole::CNodeStandardTemplateServiceRole()
	{
		m_strName = "";
		m_strDesc = "";

		m_arrServiceRole.clear();
	}

	CNodeStandardTemplateServiceRole::~CNodeStandardTemplateServiceRole()
	{
		STLDeleteElements(&m_arrServiceRole);
		m_arrServiceRole.clear();
	}


	//////////////////////////////////////////node//////////////////////////////////////////////
	CNode::CNode(IMainModuleInterface *pCore)
		:m_pCore(nullptr)
	{
		Q_ASSERT(pCore);
		if (!pCore)
		{
			return;
		}

		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_strHostName = "";
		m_nOccNo = 0;
		m_nBlockNo = 0;
		m_nNodeType = -1;
		m_strNetwork_IP_A = "";
		m_strNetwork_IP_B = "";
		m_bIsTimeSource = false;
		m_strProgram = "";
		m_strConfig = "";
		m_strGroup = "";
		m_strService = "";

		m_nSlaveOccNo = 0;

		m_arrNodeServiceRole.clear();

		m_pCore = pCore;
	}

	CNode::~CNode()
	{
		//节点用户服务都是从基本节点服务和scada可配服务选取的(指向同一块内存),删除的时候只需要node config删除一次就可以
		
		//STLDeleteElements(&m_arrNodeServiceRole);
		//m_arrNodeServiceRole.clear();
	}

	CNodeServer::CNodeServer(IMainModuleInterface *pCore)
	{
		Q_ASSERT(pCore);
		if (!pCore)
		{
			return;
		}

		m_pCore = pCore;

		m_arrNodes.clear();

		memset(m_szTagName, 0, sizeof(m_szTagName));
		//server节点的组名
		int nSize = (int)strlen("server");
		strncpy(m_szTagName, "server", qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
	}

	CNodeServer::~CNodeServer()
	{
		STLDeleteElements(&m_arrNodes);
	}

	bool CNodeServer::AddNode(CNode *pNode)
	{
		Q_ASSERT(pNode);
		if (pNode == nullptr)
			return false;
		
		auto it = std::find(m_arrNodes.begin(), m_arrNodes.end(), pNode);
		if (it == m_arrNodes.end())
		{
			m_arrNodes.push_back(pNode);
			return true;
		}
		else
		{
			return false;
		}
	}

	/*! \fn bool CNodeServer::SaveData(QXmlStreamWriter &writer)
	********************************************************************************************************* 
	** \brief Config::CNodeServer::SaveData 
	** \details 节点模块 服务器节点保存数据
	** \param writer 
	** \return bool 
	** \author gw
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	bool CNodeServer::SaveData(QXmlStreamWriter &writer, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("group");
		writer.writeAttribute("name", m_szTagName);
		writer.writeAttribute("count", QString("%1").arg(m_arrNodes.size()));

		int nBlockNo = 0;

		if (m_arrNodes.size() == 2)
		{
			for each (auto var in m_arrNodes)
			{
				nOccNo++;

				var->m_nOccNo = nOccNo;
			}

			m_arrNodes[0]->m_nSlaveOccNo = m_arrNodes[1]->m_nOccNo;

			//check
			//判断主备主机名是否相同
			if (m_arrNodes[0]->m_strHostName == m_arrNodes[1]->m_strHostName)
			{
				auto strTmp = QObject::tr("Error-->Node Server group HostNames are same!!!").arg(m_arrNodes[0]->m_strHostName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			//check IP
			if (m_arrNodes[0]->m_strNetwork_IP_A == m_arrNodes[1]->m_strNetwork_IP_B)
			{
				auto strTmp = QObject::tr("Error-->Node Server group IPs %2 are same!!!").arg(m_arrNodes[0]->m_strNetwork_IP_A);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
		}
		else if (m_arrNodes.size() == 1)
		{
			nOccNo++;

			m_arrNodes[0]->m_nOccNo = nOccNo;

			m_arrNodes[0]->m_nSlaveOccNo = m_arrNodes[0]->m_nOccNo;
		}
		else if (m_arrNodes.size() == 0)
		{
			//check
			auto strTmp = QObject::tr("Error-->Node Server node count is 0!!!");
			MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
		}

		//check
		auto strTmp = QObject::tr("-->Node Server node count is %1!!!").arg(m_arrNodes.size());
		MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);

		for each (auto var in m_arrNodes)
		{
			writer.writeStartElement("n");

			//nOccNo++;
			nBlockNo++;

			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			//check
			if (!CheckTagNameIsValid(var->m_szTagName, NODE_CONFIG_DESC))
			{
				auto strTmp = QObject::tr("Error-->Node Server TagName %1 is invalid!!!").arg(var->m_szTagName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("HostName", QString("%1").arg(var->m_strHostName));
			//check主机名
			if (!CheckDescIsValid(var->m_strHostName, NODE_CONFIG_DESC) || var->m_strHostName.isEmpty())
			{
				auto strTmp = QObject::tr("Error-->Node Server HostName %1 is invalid!!!").arg(var->m_strHostName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			const auto it = pHash->find(var->m_strHostName.toStdString());
			if (it == pHash->end())
			{
				//字符串池
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strHostName.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strHostName.toStdString());
				//DescriptionOccNo

				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			//var->m_nOccNo = nOccNo;

			writer.writeAttribute("OccNo", QString("%1").arg(var->m_nOccNo));
			
			var->m_nBlockNo = nBlockNo;
			writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));
			writer.writeAttribute("NodeType", QString("%1").arg(var->m_nNodeType));
			writer.writeAttribute("Network_IP_A", QString("%1").arg(var->m_strNetwork_IP_A));
			QRegExp rx2(MYIPREG);
			if (!rx2.exactMatch(var->m_strNetwork_IP_A))
			{
				auto strTmp = QObject::tr("Error-->Node Server TagName %1  Network_IP_A %2 is invalid!!!").arg(var->m_szTagName).arg(var->m_strNetwork_IP_A);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("Network_IP_B", QString("%1").arg(var->m_strNetwork_IP_B));
			if (!var->m_strNetwork_IP_B.isEmpty())
			{
				if (!rx2.exactMatch(var->m_strNetwork_IP_B))
				{
					auto strTmp = QObject::tr("Error-->Node Server TagName %1  Network_IP_B %2 is invalid!!!").arg(var->m_szTagName).arg(var->m_strNetwork_IP_B);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}
			}

			//A网段和B网段不一样
			if (var->m_strNetwork_IP_A == var->m_strNetwork_IP_B)
			{
				auto strTmp = QObject::tr("Error-->Node Server TagName %1  Network_IP_A %2 and Network_IP_B %3 are same!!!").arg(var->m_szTagName).arg(var->m_strNetwork_IP_B).arg(var->m_strNetwork_IP_B);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}


			writer.writeAttribute("IsTimeSource", QString("%1").arg(var->m_bIsTimeSource));
			writer.writeAttribute("Program", QString("%1").arg(var->m_strProgram)); 
			
			writer.writeAttribute("Config", QString("%1").arg(var->m_strConfig));
			if (var->m_strConfig.isEmpty())
			{
				auto strTmp = QObject::tr("Error-->Node Server TagName %1  Config is empty!!!").arg(var->m_szTagName).arg(var->m_strConfig);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("GrpName", QString("%1").arg(var->m_strGroup));
			writer.writeAttribute("Service", QString("%1").arg(var->m_strService));
			writer.writeAttribute("SlaveOccNo", QString("%1").arg(var->m_nSlaveOccNo));

			//server节点服务
			int nOccNo = 0;
			//check
			if (var->m_arrNodeServiceRole.empty())
			{
				auto strTmp = QObject::tr("Error-->Node Server TagName %1 Node Service Role is empty!!!").arg(var->m_szTagName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			for each (auto var in var->m_arrNodeServiceRole)
			{
				//[0]
				writer.writeStartElement("m");

				nOccNo++;
				writer.writeAttribute("index", QString("%1").arg(var->Index));
				writer.writeAttribute("OccNo", QString("%1").arg(nOccNo));
				writer.writeAttribute("Name", QString("%1").arg(var->Name));
				writer.writeAttribute("AppType", QString("%1").arg(var->AppType));
				writer.writeAttribute("LoadType", QString("%1").arg(var->LoadType));
				writer.writeAttribute("Argument", QString("%1").arg(var->Argument));
				writer.writeAttribute("Prority", QString("%1").arg(var->Prority));
				writer.writeAttribute("Description", QString("%1").arg(var->Description));
				const auto it = pHash->find(var->Description.toStdString());
				if (it == pHash->end())
				{
					//字符串池
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
					pHash->insert(std::make_pair(var->Description.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(var->Description.toStdString());
					//DescriptionOccNo

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}

				//[0]
				writer.writeEndElement();
			}


			writer.writeEndElement();
		}


		writer.writeEndElement();

		return true;
	}
	 

	////////////////////////////////////////////////////////////////////////////////////////


	//bool CFesConfigData::SaveFesFiles(QDomDocument *pXml, QDomElement &e, const QString &szRoot, IMainModuleInterface *pCore)
	//{
	//	Q_ASSERT(pCore);
	//	if (!pCore)
	//	{
	//		return false;
	//	}
	//	
	//	m_pCore = pCore;

	//	for (size_t i = 0; i < m_arrFesDatas.size(); i++)
	//	{
	//		m_arrFesDatas[i]->SaveFesData(szRoot);
	//	}

	//	return true;
	//}



	///*! \fn CFesGroup *CFesConfigData::AddFesGroup(QString &strGroupName)
	//********************************************************************************************************* 
	//** \brief Config::CFesConfigData::AddFesGroup 
	//** \details 添加前置组
	//** \param strGroupName 
	//** \return CFesGroup * 
	//** \author gw
	//** \date 2016年10月14日 
	//** \note 
	//********************************************************************************************************/
	//CFesGroup *CFesConfigData::AddFesGroup()
	//{
	//	CFesGroup *pGroup = new CFesGroup;

	//	int nindex = m_arrFesGroupMap.size();
	//	while (true)
	//	{
	//		QString strGroupName = QString("%1%2").arg(FES_GROUP_NAME).arg(nindex);
	//		auto &szTmp = strGroupName.toStdString();

	//		auto it = m_arrFesGroupMap.find(szTmp);
	//		if (it == m_arrFesGroupMap.end())
	//		{
	//			pGroup->m_strGroup = strGroupName;
	//			m_arrFesGroupMap.insert(std::make_pair(szTmp, pGroup));

	//			return pGroup;
	//		}
	//		else
	//		{
	//			nindex++;
	//			continue;
	//		}
	//	}

	//	return nullptr;
	//}

	//CFesGroup *CFesConfigData::AddFesGroup(CFesGroup *pFesGroup)
	//{	
	//	CFesGroup *pGroup = pFesGroup->CreateGroup();

	//	Q_ASSERT(pGroup);
	//	if (!pGroup)
	//	{
	//		return nullptr;
	//	}

	//	return pGroup;
	//}
	 
	//void CFesConfigData::LoadFesGroupMap(std::unordered_map<std::string, CFesGroup *> &arrFesGroup, const QAbstractItemModel *pModel, const QModelIndex &top, int nType)
	//{
	//	QModelIndex Item;

	//	if (!top.isValid())
	//	{

	//	}
	//	CFesGroup *pFesGroup = nullptr;
	//	for (int r = 0; r < pModel->rowCount(top); r++)
	//	{
	//		for (int c = 0; c < pModel->columnCount(top); c++)
	//		{
	//			Item = pModel->index(r, c, top);

	//			const QString strTmp = Item.data(Qt::EditRole).toString();

	//			if (FES_ITEM_GROUP == Item.data(Qt::UserRole).toInt())
	//			{
	//				pFesGroup = new CFesGroup;
	//				pFesGroup->m_strGroup = Item.data(Qt::EditRole).toString();
	//				
	//				auto &szGroup = pFesGroup->m_strGroup.toStdString();

	//				Item.data(reinterpret_cast<long long>(pFesGroup));

	//				arrFesGroup.insert(std::make_pair(szGroup, pFesGroup));
	//			}

	//			//qDebug() << Item.data(Qt::EditRole).toString();
	//		}

	//		for (int c = 0; c < pModel->columnCount(top); c++)
	//		{
	//			Item = pModel->index(r, c, top);
	//			LoadFesGroupMap(pFesGroup->m_arrFesGroup, pModel, Item, nType);
	//		}
	//	}
	//}

	/////////////////////////////////////////node module////////////////////////////////////////////////////////////////////////////////////
	
	/*! \fn CNodeGroup::CNodeGroup()
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::CNodeGroup 
	** \details 
	** \return  
	** \author gw
	** \date 2016年12月7日 
	** \note 
	********************************************************************************************************/
	CNodeGroup::CNodeGroup()
	{
		m_arrGroup.clear();
		m_arrItem.clear();

		m_strGroup = "";
	}

	/*! \fn CNodeGroup::~CNodeGroup()
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::~CNodeGroup 
	** \details 
	** \return  
	** \author gw
	** \date 2016年12月7日 
	** \note 
	********************************************************************************************************/
	CNodeGroup::~CNodeGroup()
	{
		STLDeleteElements(&m_arrGroup);
		m_arrGroup.clear();
		STLDeleteElements(&m_arrItem);
		m_arrItem.clear();
	}

	/*! \fn QString &CNodeGroup::GetNodeTagNameByFesConfigTagName(const QString &strFesTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::GetNodeTagNameByFesConfigTagName 
	** \details 根据前置tagname查找node tagname
	** \param strFesTagName 
	** \return QT_NAMESPACE::QString & 
	** \author gw
	** \date 2017年4月27日 
	** \note 
	********************************************************************************************************/
	void CNodeGroup::GetNodeTagNameByFesConfigTagName(const QString &strFesTagName, QString &strNodeTagName)
	{
		for each (auto var in m_arrGroup)
		{
			var->GetNodeTagNameByFesConfigTagName(strFesTagName, strNodeTagName);
		}

		for each (auto var in m_arrItem)
		{
			//关联前置tagname
			if (var->m_strConfig == strFesTagName)
			{
				strNodeTagName = var->m_szTagName;
			}
		}
	}

	void CNodeGroup::GetFesConfigTagNameByNodeTagName(const QString &strNodeTagName, QString &strFesTagName)
	{
		for each (auto var in m_arrGroup)
		{
			var->GetFesConfigTagNameByNodeTagName(strNodeTagName, strFesTagName);
		}

		for each (auto var in m_arrItem)
		{
			//关联前置tagname
			if (var->m_szTagName == strNodeTagName)
			{	
				strFesTagName = var->m_strConfig;
			}
		}
	}

	bool CNodeGroup::DeleteItem(CNode *pItem)
	{
		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		auto it = std::find(m_arrItem.begin(), m_arrItem.end(), pItem);
		if (it != m_arrItem.end())
		{
			m_arrItem.erase(it);

			delete pItem;

			return true;
		}

		return false;
	}

	/*! \fn bool CNodeGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::ModifyItem 
	** \details 
	** \param strTagName 
	** \param strLastTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeGroup::ModifyItem(const QString &strTagName, CNode *pNode)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		Q_ASSERT(pNode);
		if (!pNode)
		{
			return false;
		}

		auto it = std::find(m_arrItem.begin(), m_arrItem.end(), pNode);
		if (it != m_arrItem.end())
		{
			memset((*it)->m_szTagName, 0, sizeof((*it)->m_szTagName));
			int nSize = strTagName.size();
			strncpy((*it)->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			return true;
		}

		return false;
	}

	/*! \fn bool CNodeGroup::ModifyGroup(const QString &strTagName, CNodeGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::ModifyGroup 
	** \details 
	** \param strTagName 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeGroup::ModifyGroup(const QString &strTagName, CNodeGroup *pGroup)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto it = std::find(m_arrGroup.begin(), m_arrGroup.end(), pGroup);
		if (it == m_arrGroup.end())
		{
			return false;
		}

		(*it)->m_strGroup = strTagName;
		//组内项的GrpName
		for each (auto var in (*it)->m_arrItem)
		{
			var->m_strGroup = strTagName;
		}

		return true;
	}

	/*! \fn bool CNodeGroup::DeletGroup(CNodeGroup *pGroup, const QString &strTagGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeGroup::DeletGroup 
	** \details 删除组
	** \param pGroup 
	** \param strTagGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月14日 
	** \note 
	********************************************************************************************************/
	bool CNodeGroup::DeletGroup(CNodeGroup *pGroup)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto it = std::find(m_arrGroup.begin(), m_arrGroup.end(), pGroup);
		if (it == m_arrGroup.end())
		{
			return false;
		}

		m_arrGroup.erase(it);

		delete pGroup;
		pGroup = nullptr;

		return true;
	}


	CNodeConfigData::CNodeConfigData()
		:m_pFesGroup(nullptr), m_pWorkstationGroup(nullptr)
	{
		m_pNodesServer = nullptr;

		m_pFesGroup = new CNodeGroup;
		m_pWorkstationGroup = new CNodeGroup;
	}

	CNodeConfigData::~CNodeConfigData()
	{
		Clear();
	}

	void CNodeConfigData::Clear()
	{
		if (m_pNodesServer)
		{
			delete m_pNodesServer;
			m_pNodesServer = nullptr;
		}

		if (m_pFesGroup)
		{
			delete m_pFesGroup;
			m_pFesGroup = nullptr;
		}

		if (m_pWorkstationGroup)
		{
			delete m_pWorkstationGroup;
			m_pWorkstationGroup = nullptr;
		}

		m_arrHashGroup.clear();
		m_arrHashItem.clear();

		//节点服务回收
		STLDeleteElements(&m_arrServerService);
		m_arrServerService.clear();

		STLDeleteElements(&m_arrStandTemplateService);
		m_arrStandTemplateService.clear();
	}

	void CNodeConfigData::Create()
	{
		if (!m_pNodesServer)
		{
			m_pNodesServer = new CNodeServer(s_pGlobleCore);
		}
		
		if (!m_pFesGroup)
		{
			m_pFesGroup = new CNodeGroup;
		}

		if (!m_pWorkstationGroup)
		{
			m_pWorkstationGroup = new CNodeGroup;
		}
	}

	/*! \fn CNode *CNodeConfigData::AddNodeItem(CNodeGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::AddNodeItem 
	** \details 添加节点项
	** \param pGroup 
	** \return CNode * 
	** \author gw
	** \date 2016年12月7日 
	** \note 
	********************************************************************************************************/
	CNode *CNodeConfigData::AddNodeItem(CNodeGroup *pGroup)
	{
		auto nCount = m_arrHashItem.size();
		CNode *pData = new CNode(s_pGlobleCore);
		while (true)
		{
			QString strNodeTagName = QString("node%1").arg(nCount);
			//QString strDesc = QString(QObject::tr("node%1")).arg(nCount);

			std::string str = strNodeTagName.toStdString();

			//tagname是否存在
			auto it = m_arrHashItem.find(str);
			if (it != m_arrHashItem.end())
			{
				nCount++;

				continue;
			}

			if (strNodeTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strNodeTagName.size() == 0)
			{
				return nullptr;
			}
			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strNodeTagName.size();
			strncpy(pData->m_szTagName, strNodeTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));


			//组
			pData->m_strGroup = pGroup->m_strGroup;

			//item添加到组
			pGroup->m_arrItem.push_back(pData);
			//push node hash
			m_arrHashItem.insert(std::make_pair(str, pData));
			break;
		}

		return pData;
	}

	CNode *CNodeConfigData::AddServerNodeItem()
	{
		auto nCount = m_arrHashItem.size();
		CNode *pData = new CNode(s_pGlobleCore);
		while (true)
		{
			QString strNode = QString("node%1").arg(nCount);
			QString strDesc = QString(QObject::tr("node%1")).arg(nCount);

			std::string str = strNode.toStdString();

			//tagname是否存在
			auto it = m_arrHashItem.find(str);
			if (it != m_arrHashItem.end())
			{
				nCount++;

				continue;
			}

			//判断tagname长度
			if (strNode.size() > MAX_TAGNAME_LEN_SCADASTUDIO)
			{
				return nullptr;
			}

			memset(pData->m_szTagName, 0, sizeof(pData->m_szTagName));
			int nSize = strNode.size();
			strncpy(pData->m_szTagName, strNode.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

			//server节点组名
			pData->m_strGroup = "server";

			pData->m_strService = QObject::tr("ServiceConfig");

			for each (auto var in m_arrStandTemplateService)
			{
				if (var->m_strName.compare(NORMAL_SERVER, Qt::CaseInsensitive) == 0)
				{
					//
					pData->m_arrNodeServiceRole = var->m_arrServiceRole;
				}
			}

			if (m_arrServerService.empty())
			{
				QMessageBox box;
				box.setText(QObject::tr("load server node service fail!!!"));

				box.exec();
			}

			//push node hash
			m_arrHashItem.insert(std::make_pair(str, pData));
			break;
		}

		return pData;
	}

	bool CNodeConfigData::DeleteHashItem(QString strTagName)
	{
		auto it = m_arrHashItem.find(strTagName.toStdString());
		if (it == m_arrHashItem.end())
		{
			return false;
		}

		m_arrHashItem.erase(it);

		return true;
	}

	bool CNodeConfigData::PushTagNameToHash(QString strTagName, CNode *pData)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		Q_ASSERT(pData);
		if (!pData)
		{
			return false;
		}

		m_arrHashItem.insert(make_pair(strTagName.toStdString(), pData));

		return true;
	}

	/*! \fn bool CNodeConfigData::PushGroupTagNameToHash(QString strTagName, CNodeGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::PushGroupTagNameToHash 
	** \details 
	** \param strTagName 
	** \param pGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::PushGroupTagNameToHash(QString strTagName)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		//Q_ASSERT(pGroup);
		//if (!pGroup)
		//{
		//	return false;
		//}

		m_arrHashGroup.insert(make_pair(strTagName.toStdString(), strTagName.toStdString()));

		return true;
	}

	/*! \fn bool CNodeConfigData::CheckTagNameIsExist(QString strTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::CheckTagNameIsExist 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::CheckGroupTagNameIsExist(QString strTagName)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}
		
		auto it = m_arrHashGroup.find(strTagName.toStdString());
		if (it != m_arrHashGroup.end())
		{
			return true;
		}

		return false;
	}

	/*! \fn bool CNodeConfigData::ModifyGroupHashMap(QString strTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::ModifyGroupHashMap 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::ModifyGroupHashMap(const QString &strTagName, const QString &strLastTagName)
	{
		//hash tagname key修改
		auto &arr = m_arrHashGroup;

		auto it = arr.find(strLastTagName.toStdString());
		if (it != arr.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(arr[strTagName.toStdString()], strTagName.toStdString());
			// Erase old key-value from map
			arr.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CNodeConfigData::DeleteHashGroup(QString strTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::DeleteHashGroup 
	** \details 
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月14日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::DeleteHashGroup(QString strTagName)
	{
		auto it = m_arrHashGroup.find(strTagName.toStdString());
		if (it == m_arrHashGroup.end())
		{
			return false;
		}

		m_arrHashGroup.erase(it);

		return true;
	}


	/*! \fn bool CNodeConfigData::CheckTagNameIsExist(QString strTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::CheckTagNameIsExist 
	** \details 检查节点项tagname是否存在
	** \param strTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::CheckTagNameIsExist(QString strTagName)
	{
		Q_ASSERT(!strTagName.isEmpty());
		if (strTagName.isEmpty())
		{
			return false;
		}

		auto it = m_arrHashItem.find(strTagName.toStdString());
		if (it != m_arrHashItem.end())
		{
			return true;
		}

		return false;
	}

	/*! \fn bool CNodeConfigData::ModifyGroupHashMap(const QString &strTagName, const QString &strLastTagName)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::ModifyGroupHashMap 
	** \details 
	** \param strTagName 
	** \param strLastTagName 
	** \return bool 
	** \author gw
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::ModifyItemHashMap(const QString &strTagName, const QString &strLastTagName)
	{
		//hash tagname key修改
		auto &arr = m_arrHashItem;

		auto it = arr.find(strLastTagName.toStdString());
		if (it != arr.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.
			std::swap(arr[strTagName.toStdString()], it->second);
			// Erase old key-value from map
			arr.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn 
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::ChangeTagNameNotifyFes 
	** \details node fes的tagname修改，前置关联该节点的点也跟着修改
	** \param strTagName 
	** \param strLastTagName 
	** \return bool 
	** \author gw
	** \date 2017年6月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::ChangeNodeFesTagNameNotifyFes(QString &strTagName, QString &strLastTagName)
	{
		//网络节点tagname修改，
		//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
		auto pFesModule = s_pGlobleCore->GetModule("fes");
		Q_ASSERT(pFesModule);
		if (!pFesModule)
		{
			return false;
		}
		pFesModule->ChangeTagNameNodify(strTagName, strLastTagName, Module_ID::NODE_MODULE_FES);

		return true;
	}

	bool CNodeConfigData::ChangeNodeServerTagNameNotifyScada(QString &strTagName, QString &strLastTagName)
	{	
		//网络节点tagname修改，
		//如果前置用户变量的sourcetagname和scada变量的sourcetagname关联网络节点，前置用户变量的sourcetagname也跟着修改，scada变量的sourcetagname也跟着修改
		auto pScadaVaraibleModule = s_pGlobleCore->GetModule("scada_variable");
		Q_ASSERT(pScadaVaraibleModule);
		if (!pScadaVaraibleModule)
		{
			return false;
		}
		pScadaVaraibleModule->ChangeTagNameNodify(strTagName, strLastTagName, Module_ID::NODE_MODULE_SERVER);

		return true;
	}

	/*! \fn CNodeGroup *CNodeConfigData::AddFesNodeGroup(CNodeGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::AddFesNodeGroup 
	** \details 添加前置节点组
	** \param pGroup 
	** \return CNodeGroup * 
	** \author gw
	** \date 2016年12月7日 
	** \note 
	********************************************************************************************************/
	CNodeGroup *CNodeConfigData::AddNodeGroup(CNodeGroup *pGroup)
	{
		auto nCount = m_arrHashGroup.size();
		auto *pGroupChild = new CNodeGroup();
		while (true)
		{
			QString strFes = QString("nodegroup%1").arg(nCount);
			QString strDesc = QString(QObject::tr("nodegroup%1")).arg(nCount);

			std::string str = strFes.toStdString();

			//tagname是否存在 规则 fes channel device ai di ao do所有的hash都要检查
			auto it = m_arrHashGroup.find(str);
			if (it != m_arrHashGroup.end())
			{
				nCount++;

				continue;
			}

			pGroupChild->m_strGroup = strFes;

			//item添加到组
			pGroup->m_arrGroup.push_back(pGroupChild);
			//push node hash
			m_arrHashGroup.insert(std::make_pair(str, str));
			break;
		}

		return pGroupChild;
	}

	/*! \fn bool CNodeConfigData::AddNodeData()
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::AddNodeData 
	** \details 添加节点数据
	** \param pNode 节点 三种类型
	** \return bool 
	** \author gw
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::AddNodeData(CNodeServer *pServer)
	{
		Q_ASSERT(pServer);
		if (pServer == nullptr)
		{
			return false;
		}
			
		m_pNodesServer = pServer;

		return true;
	}

	/*! \fn 	bool CNodeConfigData::LoadNodeService(QString &strFile)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadNodeService 
	** \details 加载bin下的service.xml 节点运行服务
	** \param strFile 
	** \return bool 
	** \author gw
	** \date 2017年2月22日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadNodeService()
	{
		QXmlStreamReader xml;
		auto strPath = qApp->applicationDirPath();
		QString strFile = strPath + "/service.xml";
		
		QFile file(strFile);
		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			auto strError = QString(QObject::tr("load node file failed!!!"));
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), 1);

			return false;
		}

		xml.setDevice(&file);
		while (!xml.atEnd())
		{
			if (xml.hasError())
			{
				auto strError = QObject::tr("xml error!!!");
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}
			else if (xml.isStartElement()) //遇到开始标签，如<city>  
			{
				if (xml.name().toString() == "scada_module")
				{
					if (!m_pNodesServer)
					{
						m_pNodesServer = new Config::CNodeServer(s_pGlobleCore);
					}

					LoadScadaModule(xml);

					xml.readNext();
				}
				else if (xml.name().toString() == "scheme")
				{
					Q_ASSERT(m_pFesGroup);
					if (!m_pFesGroup)
					{
						xml.readNext();

						continue;
					}

					LoadSchemeModule(xml);

					xml.readNext();
				}
				else //遇到不需要的标签，如<fengli>和<shidu>  
					xml.readNext();
			}
			else //遇到结束标签，<span style="font-family: Arial, Helvetica, sans-serif;">如</city></span>  
				xml.readNext();
		}

		file.close();

		return true;

	}

	/*! \fn bool CNodeConfigData::LoadScadaModule(QXmlStreamReader &xml)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadScadaModule 
	** \details 
	** \param xml 
	** \return bool 
	** \author gw
	** \date 2017年2月22日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadScadaModule(QXmlStreamReader &xml)
	{
		Q_ASSERT(m_pNodesServer);
		if (!m_pNodesServer)
		{
			return false;
		}

		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "m")
				{
					Config::NODE_SERVICE_ROLE *pNodeServiceRole = new Config::NODE_SERVICE_ROLE;

					pNodeServiceRole->Index = xml.attributes().value("index").toInt();
					pNodeServiceRole->Name = xml.attributes().value("Name").toString();
					auto bFlag = CheckTagNameIsValid(pNodeServiceRole->Name, NODE_CONFIG_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						xml.readNext();
						continue;
					}

					pNodeServiceRole->AppType = xml.attributes().value("AppType").toInt();
					pNodeServiceRole->LoadType = xml.attributes().value("LoadType").toInt();
					pNodeServiceRole->Argument = xml.attributes().value("Argument").toString();
					pNodeServiceRole->Description = xml.attributes().value("Description").toString();
					pNodeServiceRole->Prority = xml.attributes().value("Prority").toInt();

					m_arrServerService.push_back(pNodeServiceRole);
				}
			}
			else if (xml.isEndElement() && xml.name() == "scada_module")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::LoadSchemeModule(QXmlStreamReader &xml)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadSchemeModule 
	** \details 加载节点不同节点的服务
	** \param xml 
	** \return bool 
	** \author gw
	** \date 2017年2月22日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadSchemeModule(QXmlStreamReader &xml)
	{
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "s")
				{
					Config::CNodeStandardTemplateServiceRole *pNodeService = new Config::CNodeStandardTemplateServiceRole();

					pNodeService->m_strName = xml.attributes().value("name").toString();
					pNodeService->m_strDesc = xml.attributes().value("description").toString();

					LoadSchemeModuleMember(xml, pNodeService);

					m_arrStandTemplateService.push_back(pNodeService);
				}
			}
			else if (xml.isEndElement() && xml.name() == "scheme")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::LoadSchemeModuleMember(QXmlStreamReader &xml)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadSchemeModuleMember 
	** \details 加载节点服务成员s->m
	** \param xml 
	** \return bool 
	** \author gw
	** \date 2017年2月22日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadSchemeModuleMember(QXmlStreamReader &xml, Config::CNodeStandardTemplateServiceRole *pService)
	{
		Q_ASSERT(pService);
		if (!pService)
		{
			return false;
		}

		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "m")
				{
					Config::NODE_SERVICE_ROLE *pNodeServiceRole = new Config::NODE_SERVICE_ROLE;

					pNodeServiceRole->Index = xml.attributes().value("index").toInt();
					pNodeServiceRole->Name = xml.attributes().value("Name").toString();
					auto bFlag = CheckTagNameIsValid(pNodeServiceRole->Name, NODE_CONFIG_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						xml.readNext();
						continue;
					}

					pNodeServiceRole->AppType = xml.attributes().value("AppType").toInt();
					pNodeServiceRole->LoadType = xml.attributes().value("LoadType").toInt();
					pNodeServiceRole->Argument = xml.attributes().value("Argument").toString();
					pNodeServiceRole->Description = xml.attributes().value("Description").toString();
					pNodeServiceRole->Prority = xml.attributes().value("Prority").toInt();

					pService->m_arrServiceRole.push_back(pNodeServiceRole);
				}
			}
			else if (xml.isEndElement() && xml.name() == "s")
			{
				break;
			}

			xml.readNext();
		}

		return true;


	}

	/*! \fn bool CNodeConfigData::LoadNodeFile(QString &strFile)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadNodeFile 
	** \details 加载节点工程文件
	** \param strFile 
	** \return bool 
	** \author gw
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadNodeFile(QString &strFile)
	{
		if (!m_pFesGroup)
		{
			m_pFesGroup = new CNodeGroup;
		}

		if (!m_pWorkstationGroup)
		{
			m_pWorkstationGroup = new CNodeGroup;
		}
		
		QXmlStreamReader xml;
		auto strPath = s_pGlobleCore->GetProjMgr()->GetFile()->fileName();
		auto nIndex = strPath.lastIndexOf("/");
		strPath = strPath.mid(0, nIndex);
		strPath += "/node/";

		auto strTmp = strPath + "/" + strFile;
		QFile file(strTmp);

		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			auto strError = QString(QObject::tr("load node file failed!!!"));
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), 1);

			return false;
		}

		xml.setDevice(&file);

		while (!xml.atEnd())
		{
			if (xml.hasError())
			{
				auto strError = QObject::tr("xml error!!!");
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
				return false;
			}
			else if (xml.isStartElement()) //遇到开始标签，如<city>  
			{
				if (xml.name().toString() == "nserver")
				{
					if (!m_pNodesServer)
					{
						m_pNodesServer = new Config::CNodeServer(s_pGlobleCore);
					}
					
					LoadNodeServerNode(xml);

					xml.readNext();
				}
				else if (xml.name().toString() == "nfes")
				{
					Q_ASSERT(m_pFesGroup);
					if (!m_pFesGroup)
					{
						xml.readNext();

						continue;
					}

					LoadNodeFesNode(xml, m_pFesGroup);

					xml.readNext();
				}
				else if (xml.name().toString() == "nworksstation")
				{
					Q_ASSERT(m_pWorkstationGroup);
					if (!m_pWorkstationGroup)
					{
						xml.readNext();

						continue;
					}

					LoadNodeWorkstationNode(xml, m_pWorkstationGroup);

					xml.readNext();
				}
				else //遇到不需要的标签，如<fengli>和<shidu>  
					xml.readNext();
			}
			else //遇到结束标签，<span style="font-family: Arial, Helvetica, sans-serif;">如</city></span>  
				xml.readNext();
		}

		file.close();

		return true;
	}

	bool CNodeConfigData::LoadNodeFesNode(QXmlStreamReader &xml, Config::CNodeGroup *pFesGroup)
	{
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "n")
				{
					Config::CNode *pNode = new Config::CNode(s_pGlobleCore);
					
					memset(pNode->m_szTagName, 0, sizeof(pNode->m_szTagName));
					auto strTagName = xml.attributes().value("TagName").toString();
					int nSize = strTagName.size();
					strncpy(pNode->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			
					pNode->m_strHostName = xml.attributes().value("HostName").toString();
					pNode->m_nOccNo = xml.attributes().value("OccNo").toInt();
					pNode->m_nBlockNo = xml.attributes().value("BlockNo").toInt();
					pNode->m_nNodeType = xml.attributes().value("NodeType").toInt();
					pNode->m_strNetwork_IP_A = xml.attributes().value("Network_IP_A").toString();
					pNode->m_strNetwork_IP_B = xml.attributes().value("Network_IP_B").toString();
					pNode->m_bIsTimeSource = xml.attributes().value("IsTimeSource").toInt();
					pNode->m_strProgram = xml.attributes().value("Program").toString();
					pNode->m_strConfig = xml.attributes().value("Config").toString();
					pNode->m_strGroup = xml.attributes().value("GrpName").toString();
					pNode->m_nSlaveOccNo = xml.attributes().value("SlaveOccNo").toInt();

					//pFesGroup->m_strGroup = pNode->m_strGroup;
					pFesGroup->m_arrItem.push_back(pNode);
					//push hash
					m_arrHashItem.insert(make_pair(strTagName.toStdString(), pNode));
				}
				else if (strTmp == "group")
				{
					Config::CNodeGroup *pChildGroup = new Config::CNodeGroup;
					pChildGroup->m_strGroup = xml.attributes().value("name").toString();

					pFesGroup->m_arrGroup.push_back(pChildGroup);

					//添加到hash表里
					PushGroupTagNameToHash(pChildGroup->m_strGroup);

					//组group的子组group
					xml.readNext();

					LoadNodeFesGroupNode(xml, pChildGroup);

					continue;
				}
			}
			else if (xml.isEndElement() && xml.name() == "nfes")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::LoadNodeFesGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pFesGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadNodeFesGroupNode 
	** \details 加载前置节点组
	** \param xml 
	** \param pFesGroup 
	** \return bool 
	** \author gw
	** \date 2016年12月10日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadNodeFesGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pFesGroup)
	{
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "n")
				{
					Config::CNode *pNode = new Config::CNode(s_pGlobleCore);

					memset(pNode->m_szTagName, 0, sizeof(pNode->m_szTagName));
					auto strTagName = xml.attributes().value("TagName").toString();
					int nSize = strTagName.size();
					strncpy(pNode->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pNode->m_strHostName = xml.attributes().value("HostName").toString();
					pNode->m_nOccNo = xml.attributes().value("OccNo").toInt();
					pNode->m_nBlockNo = xml.attributes().value("BlockNo").toInt();
					pNode->m_nNodeType = xml.attributes().value("NodeType").toInt();
					pNode->m_strNetwork_IP_A = xml.attributes().value("Network_IP_A").toString();
					pNode->m_strNetwork_IP_B = xml.attributes().value("Network_IP_B").toString();
					pNode->m_bIsTimeSource = xml.attributes().value("IsTimeSource").toInt();
					pNode->m_strProgram = xml.attributes().value("Program").toString();
					pNode->m_strGroup = xml.attributes().value("Group").toString();
					pNode->m_strConfig = xml.attributes().value("Config").toString();

					//pFesGroup->m_strGroup = pNode->m_strGroup;
					pFesGroup->m_arrItem.push_back(pNode);
					//push hash
					m_arrHashItem.insert(make_pair(strTagName.toStdString(), pNode));
				}
				else if (strTmp == "group")
				{
					Config::CNodeGroup *pChildGroup = new Config::CNodeGroup;
					pChildGroup->m_strGroup = xml.attributes().value("name").toString();

					pFesGroup->m_arrGroup.push_back(pChildGroup);

					//组group的子组group
					xml.readNext();

					LoadNodeFesGroupNode(xml, pChildGroup);

					continue;
				}
			}
			else if (xml.isEndElement() && xml.name() == "group")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	bool CNodeConfigData::LoadNodeServerNode(QXmlStreamReader &xml)
	{
		Q_ASSERT(m_pNodesServer);
		if (!m_pNodesServer)
		{
			return false;
		}
		
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "group")
				{
					auto strTmp = xml.attributes().value("name").toString();
					memset(m_pNodesServer->m_szTagName, 0, sizeof(m_pNodesServer->m_szTagName));
					int nSize = strTmp.size();
					strncpy(m_pNodesServer->m_szTagName, strTmp.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
				
					PushGroupTagNameToHash(strTmp);
				}
				else if (strTmp == "n")
				{
					Config::CNode *pNode = new Config::CNode(s_pGlobleCore);

					memset(pNode->m_szTagName, 0, sizeof(pNode->m_szTagName));
					auto strTagName = xml.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, NODE_CONFIG_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						xml.readNext();
						
						return false;
					}
					int nSize = strTagName.size();
					strncpy(pNode->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pNode->m_strHostName = xml.attributes().value("HostName").toString();
					pNode->m_nOccNo = xml.attributes().value("OccNo").toInt();
					pNode->m_nBlockNo = xml.attributes().value("BlockNo").toInt();
					pNode->m_nNodeType = xml.attributes().value("NodeType").toInt();
					pNode->m_strNetwork_IP_A = xml.attributes().value("Network_IP_A").toString();
					pNode->m_strNetwork_IP_B = xml.attributes().value("Network_IP_B").toString();
					pNode->m_bIsTimeSource = xml.attributes().value("IsTimeSource").toInt();
					pNode->m_strProgram = xml.attributes().value("Program").toString();
					pNode->m_strConfig = xml.attributes().value("Config").toString();
					pNode->m_strGroup = xml.attributes().value("GrpName").toString();
					pNode->m_nSlaveOccNo = xml.attributes().value("SlaveOccNo").toInt();
					pNode->m_strService = xml.attributes().value("Service").toString();

					//加载服务
					LoadNodeServerAndWorkstatonServiceMember(xml, pNode);

					m_pNodesServer->m_arrNodes.push_back(pNode);
					//push hash
					m_arrHashItem.insert(make_pair(strTagName.toStdString(), pNode));
				}
			}
			else if (xml.isEndElement() && xml.name() == "group")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::LoadNodeServerNodeServiceMember(QXmlStreamReader &xml, Config::CNode *pNode)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadNodeServerNodeServiceMember 
	** \details server节点加载服务和workstation节点加载服务
	** \param xml 
	** \param pNode 
	** \return bool 
	** \author gw
	** \date 2017年2月24日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadNodeServerAndWorkstatonServiceMember(QXmlStreamReader &xml, Config::CNode *pNode)
	{
		Q_ASSERT(pNode);
		if (!pNode)
		{
			return false;
		}

		while (!xml.atEnd())
		{
			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();
				if (strTmp == "m")
				{
					pNode->m_arrNodeServiceRole;
					auto pService = new Config::NODE_SERVICE_ROLE;

					auto strError = QObject::tr("Load server node service fail!!!");

					pService->Name = xml.attributes().value("Name").toString();
					auto bFlag = CheckTagNameIsValid(pService->Name, NODE_CONFIG_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						//auto strError = QObject::tr("Load server node service fail!!!");
						
						s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);
							
						xml.readNext();

						continue;
					}

					pService->Index = xml.attributes().value("index").toInt();
					pService->AppType = xml.attributes().value("AppType").toInt();
					pService->LoadType = xml.attributes().value("LoadType").toInt();
					pService->Argument = xml.attributes().value("Argument").toString();
					pService->Description = xml.attributes().value("Description").toString();
					pService->Prority = xml.attributes().value("Prority").toInt();

					bFlag = CheckDescIsValid(pService->Description, NODE_CONFIG_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Load server node service fail, desc %1 is invalid!!!").arg(pService->Description);

						s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

						xml.readNext();

						continue;
					}

					pNode->m_arrNodeServiceRole.push_back(pService);
				}

			}
			else if (xml.isEndElement() && xml.name() == "n")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	bool CNodeConfigData::LoadNodeWorkstationNode(QXmlStreamReader &xml, Config::CNodeGroup *pWorkstation)
	{
		Q_ASSERT(pWorkstation);
		if (!pWorkstation)
		{
			return false;
		}
		
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "nw")
				{



				}
				else if (strTmp == "n")
				{
					Config::CNode *pNode = new Config::CNode(s_pGlobleCore);

					memset(pNode->m_szTagName, 0, sizeof(pNode->m_szTagName));
					auto strTagName = xml.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, NODE_CONFIG_DESC))
					{
						return false;
					}
					int nSize = strTagName.size();
					strncpy(pNode->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pNode->m_strHostName = xml.attributes().value("HostName").toString();
					pNode->m_nOccNo = xml.attributes().value("OccNo").toInt();
					pNode->m_nBlockNo = xml.attributes().value("BlockNo").toInt();
					pNode->m_nNodeType = xml.attributes().value("NodeType").toInt();
					pNode->m_strNetwork_IP_A = xml.attributes().value("Network_IP_A").toString();
					pNode->m_strNetwork_IP_B = xml.attributes().value("Network_IP_B").toString();
					pNode->m_bIsTimeSource = xml.attributes().value("IsTimeSource").toInt();
					pNode->m_strProgram = xml.attributes().value("Program").toString();
					pNode->m_strConfig = xml.attributes().value("Config").toString();
					pNode->m_strGroup = xml.attributes().value("GrpName").toString();
					pNode->m_nSlaveOccNo = xml.attributes().value("SlaveOccNo").toInt();
					pNode->m_strService = xml.attributes().value("Service").toString();

					//加载服务
					LoadNodeServerAndWorkstatonServiceMember(xml, pNode);

					pWorkstation->m_arrItem.push_back(pNode);
					//push hash
					m_arrHashItem.insert(make_pair(strTagName.toStdString(), pNode));
				}
				else if (strTmp == "group")
				{
					Config::CNodeGroup *pChildGroup = new Config::CNodeGroup;
					pChildGroup->m_strGroup = xml.attributes().value("name").toString();

					pWorkstation->m_arrGroup.push_back(pChildGroup);

					//添加到hash表里
					PushGroupTagNameToHash(pChildGroup->m_strGroup);

					//组group的子组group
					xml.readNext();

					LoadNodeWorkstationGroupNode(xml, pChildGroup);

					continue;
				}
			}
			else if (xml.isEndElement() && xml.name() == "nw")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::LoadNodeWorkstationGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pWorkstation)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::LoadNodeWorkstationGroupNode 
	** \details 加载工作站组
	** \param xml 
	** \param pWorkstation 
	** \return bool 
	** \author gw
	** \date 2016年12月10日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::LoadNodeWorkstationGroupNode(QXmlStreamReader &xml, Config::CNodeGroup *pWorkstation)
	{
		Q_ASSERT(pWorkstation);
		if (!pWorkstation)
		{
			return false;
		}
		
		while (!xml.atEnd())
		{
			QString strTmp = xml.name().toString();

			if (xml.isStartElement())
			{
				QString strTmp = xml.name().toString();

				if (strTmp == "n")
				{
					Config::CNode *pNode = new Config::CNode(s_pGlobleCore);

					memset(pNode->m_szTagName, 0, sizeof(pNode->m_szTagName));
					auto strTagName = xml.attributes().value("TagName").toString();
					int nSize = strTagName.size();
					strncpy(pNode->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pNode->m_strHostName = xml.attributes().value("HostName").toString();
					pNode->m_nOccNo = xml.attributes().value("OccNo").toInt();
					pNode->m_nBlockNo = xml.attributes().value("BlockNo").toInt();
					pNode->m_nNodeType = xml.attributes().value("NodeType").toInt();
					pNode->m_strNetwork_IP_A = xml.attributes().value("Network_IP_A").toString();
					pNode->m_strNetwork_IP_B = xml.attributes().value("Network_IP_B").toString();
					pNode->m_bIsTimeSource = xml.attributes().value("IsTimeSource").toInt();
					pNode->m_strProgram = xml.attributes().value("Program").toString();
					pNode->m_strConfig = xml.attributes().value("Config").toString();
					pNode->m_strGroup = xml.attributes().value("GrpName").toString();
					pNode->m_strService = xml.attributes().value("Service").toString();

					LoadNodeServerAndWorkstatonServiceMember(xml, pNode);

					pWorkstation->m_arrItem.push_back(pNode);
					//push hash
					m_arrHashItem.insert(make_pair(strTagName.toStdString(), pNode));
				}
				else if (strTmp == "group")
				{
					Config::CNodeGroup *pChildGroup = new Config::CNodeGroup;
					pChildGroup->m_strGroup = xml.attributes().value("name").toString();

					pWorkstation->m_arrGroup.push_back(pChildGroup);

					//组group的子组group
					xml.readNext();

					LoadNodeWorkstationGroupNode(xml, pChildGroup);

					continue;
				}
			}
			else if (xml.isEndElement() && xml.name() == "group")
			{
				break;
			}

			xml.readNext();
		}

		return true;
	}

	/*! \fn bool CNodeConfigData::SaveNodeFiles(QDomDocument *pXml, QDomElement &e, const QString &szRoot)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::SaveNodeFiles 
	** \details 保存节点数据
	** \param pXml 
	** \param e 
	** \param szRoot 
	** \return bool 
	** \author gw
	** \date 2016年9月27日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::SaveNodeFiles(QDomDocument *pXml, QDomElement &e, const QString &szRoot, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto strError = QObject::tr("Save node file fail!!!");
		
		Q_ASSERT(pHash);
		if (!pHash)
		{
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		Q_ASSERT(pStringPoolVec);
		if (!pStringPoolVec)
		{
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		Q_ASSERT(pDescStringPoolOccNo);
		if (!pDescStringPoolOccNo)
		{
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strError.toStdString().c_str(), LEVEL_1);

			return false;
		}
		
		auto strTmp = QObject::tr("Node log start!!!");
		MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), false);

		QDomElement nElet = pXml->createElement("n");
		QString fileName = "node.xml";
		nElet.setAttribute("file", fileName);
		
		e.appendChild(nElet);

		QString strPath = szRoot + "/node/";

		strPath = strPath + fileName;

		QFile file(strPath);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QString strTmp = QString(QObject::tr("open %1 fail!!!")).arg(fileName);

			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);

			return false;
		}

		QXmlStreamWriter writer(&file);
		writer.setAutoFormatting(true);
		writer.writeStartDocument();


		writer.writeStartElement("node");
		//writer.writeAttribute("Description", m_strDescription);
		//writer.writeAttribute("TagName", m_szTagName);
		//writer.writeAttribute("version", VERSION);
		//writer.writeAttribute("ID", QString("%1").arg(m_nID));
		//writer.writeAttribute("Extention", m_strExtention);
		//writer.writeAttribute("GroupName", m_strGroupName);

		//节点大排行号
		int nOccNo = 0;

		writer.writeStartElement("nserver");
		Q_ASSERT(m_pNodesServer);
		if (!m_pNodesServer)
		{
			auto strError = QObject::tr("m_pNodesServer is null!!!");
			s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

			writer.writeEndElement();

			writer.writeEndDocument();

			return false;
		}
		m_pNodesServer->SaveData(writer, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		writer.writeEndElement();
		

		writer.writeStartElement("nfes");
		int nTmp = nOccNo;
		SaveChildNode(writer, m_pFesGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		if (nOccNo == nTmp)
		{
			auto strTmp = QObject::tr("Error-->Node Fes count is 0!!!");
			MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
		}
		else
		{
			auto strTmp = QObject::tr("-->Node Fes count is %1!!!").arg(nOccNo - nTmp);
			MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
		}
		writer.writeEndElement();


		writer.writeStartElement("nworksstation");
		nTmp = nOccNo;
		SaveWorkstationChildNode(writer, m_pWorkstationGroup, nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);
		if (nOccNo == nTmp)
		{
			auto strTmp = QObject::tr("Error-->Node Workstation count is 0!!!");
			MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
			s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
		}
		else
		{
			auto strTmp = QObject::tr("-->Node worksstation count is %1!!!").arg(nOccNo - nTmp);
			MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
		}
		writer.writeEndElement();


		writer.writeEndElement();

		writer.writeEndDocument();
		
		return true;
	}

	/*! \fn bool CNodeConfigData::SaveFesChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::SaveFesChildNode 
	** \details 
	** \param writer 
	** \param pGroup 
	** \return bool 
	** \author guowei
	** \date 2016年12月13日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::SaveChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto ite = pGroup->m_arrGroup.begin();
		for (; ite != pGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", (*ite)->m_strGroup);
			writer.writeAttribute("count", QString("%1").arg(pGroup->m_arrItem.size()));

			SaveChildNode(writer, (*ite), nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto it = pGroup->m_arrItem.begin();
		int nBlockNo = 0;

		if (!pGroup->m_strGroup.isEmpty())
		{
			if (pGroup->m_arrItem.size() == 2)
			{
				for each (auto var in pGroup->m_arrItem)
				{
					nOccNo++;

					var->m_nOccNo = nOccNo;
				}

				pGroup->m_arrItem[0]->m_nSlaveOccNo = pGroup->m_arrItem[1]->m_nOccNo;

				//check
				//判断主备主机名是否相同
				if (pGroup->m_arrItem[0]->m_strHostName == pGroup->m_arrItem[1]->m_strHostName)
				{
					auto strTmp = QObject::tr("Error-->Node Fes group %1  HostNames %2 are same!!!").arg(pGroup->m_strGroup).arg(pGroup->m_arrItem[0]->m_strHostName);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

				//check IP
				if (pGroup->m_arrItem[0]->m_strNetwork_IP_A == pGroup->m_arrItem[1]->m_strNetwork_IP_B)
				{
					auto strTmp = QObject::tr("Error-->Node Fes group %1 IPs %2 are same!!!").arg(pGroup->m_strGroup).arg(pGroup->m_arrItem[0]->m_strNetwork_IP_A);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}
			}
			else if (pGroup->m_arrItem.size() == 1)
			{
				nOccNo++;

				pGroup->m_arrItem[0]->m_nOccNo = nOccNo;

				pGroup->m_arrItem[0]->m_nSlaveOccNo = pGroup->m_arrItem[0]->m_nOccNo;
			}
			else if (pGroup->m_arrItem.size() == 0)
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1  node count is 0!!!").arg(pGroup->m_strGroup);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
		}
		else
		{
			for each (auto var in pGroup->m_arrItem)
			{
				nOccNo++;

				var->m_nOccNo = nOccNo;

				var->m_nSlaveOccNo = var->m_nOccNo;
			}
		}


		for each (auto var in pGroup->m_arrItem)
		{
			writer.writeStartElement("n");

			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			//check tagname
			if (!CheckTagNameIsValid(var->m_szTagName, NODE_CONFIG_DESC))
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1  Node TagName %2 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			
			writer.writeAttribute("HostName", QString("%1").arg(var->m_strHostName));
			//check主机名
			if (!CheckDescIsValid(var->m_strHostName, NODE_CONFIG_DESC) || var->m_strHostName.isEmpty())
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1  Node HostName %2 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_strHostName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			
			const auto it = pHash->find(var->m_strHostName.toStdString());
			if (it == pHash->end())
			{
				//字符串池
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(std::make_pair(var->m_strHostName.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(var->m_strHostName.toStdString());
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			//nOccNo++;
			//var->m_nOccNo = nOccNo;

			writer.writeAttribute("OccNo", QString("%1").arg(var->m_nOccNo));

			nBlockNo++;
			var->m_nBlockNo = nBlockNo;
			writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));

			writer.writeAttribute("NodeType", QString("%1").arg(var->m_nNodeType));
			writer.writeAttribute("Network_IP_A", QString("%1").arg(var->m_strNetwork_IP_A));
			//check Network_IP_A是否合法
			QRegExp rx2(MYIPREG);
			if (!rx2.exactMatch(var->m_strNetwork_IP_A))
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1 TagName %2  Network_IP_A %3 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_A);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("Network_IP_B", QString("%1").arg(var->m_strNetwork_IP_B));
			if (!var->m_strNetwork_IP_B.isEmpty())
			{
				if (!rx2.exactMatch(var->m_strNetwork_IP_B))
				{
					auto strTmp = QObject::tr("Error-->Node Fes Group %1 TagName %2  Network_IP_B %3 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_B);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

			}

			//A网段和B网段不一样
			if (var->m_strNetwork_IP_A == var->m_strNetwork_IP_B)
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1  Node TagName %2  Network_IP_A %3 and Network_IP_B %4 are same!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_B).arg(var->m_strNetwork_IP_B);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("IsTimeSource", QString("%1").arg(var->m_bIsTimeSource));
			writer.writeAttribute("Program", QString("%1").arg(var->m_strProgram));


			writer.writeAttribute("Config", QString("%1").arg(var->m_strConfig));
			if (var->m_strConfig.isEmpty())
			{
				auto strTmp = QObject::tr("Error-->Node Fes Group %1  Node TagName %2  Config %3 is empty!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strConfig);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("GrpName", QString("%1").arg(var->m_strGroup));
			//新加SlaveOccNo
			writer.writeAttribute("SlaveOccNo", QString("%1").arg(var->m_nSlaveOccNo));

			writer.writeEndElement();
		}
		return true;
	}

	/*! \fn 	bool CNodeConfigData::SaveWorkstationChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup, int &nOccNo)
	********************************************************************************************************* 
	** \brief Config::CNodeConfigData::SaveWorkstationChildNode 
	** \details 保存工作站组
	** \param writer 
	** \param pGroup 
	** \param nOccNo 
	** \return bool 
	** \author gw
	** \date 2017年2月25日 
	** \note 
	********************************************************************************************************/
	bool CNodeConfigData::SaveWorkstationChildNode(QXmlStreamWriter &writer, Config::CNodeGroup *pGroup, int &nOccNo, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pGroup);
		if (!pGroup)
		{
			return false;
		}

		auto ite = pGroup->m_arrGroup.begin();
		for (; ite != pGroup->m_arrGroup.end(); ++ite)
		{
			writer.writeStartElement("group");
			writer.writeAttribute("name", (*ite)->m_strGroup);
			writer.writeAttribute("count", QString("%1").arg(pGroup->m_arrItem.size()));

			SaveWorkstationChildNode(writer, (*ite), nOccNo, pHash, pStringPoolVec, pDescStringPoolOccNo);

			writer.writeEndElement();
		}

		auto it = pGroup->m_arrItem.begin();
		int nBlockNo = 0;

		if (!pGroup->m_strGroup.isEmpty())
		{
			if (pGroup->m_arrItem.size() == 2)
			{
				for each (auto var in pGroup->m_arrItem)
				{
					nOccNo++;

					var->m_nOccNo = nOccNo;
				}

				pGroup->m_arrItem[0]->m_nSlaveOccNo = pGroup->m_arrItem[1]->m_nOccNo;

				//check
				//判断主备主机名是否相同
				if (pGroup->m_arrItem[0]->m_strHostName == pGroup->m_arrItem[1]->m_strHostName)
				{
					auto strTmp = QObject::tr("Error-->Node Workstation group %1  HostNames %2 are same!!!").arg(pGroup->m_strGroup).arg(pGroup->m_arrItem[0]->m_strHostName);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

				//check IP
				if (pGroup->m_arrItem[0]->m_strNetwork_IP_A == pGroup->m_arrItem[1]->m_strNetwork_IP_B)
				{
					auto strTmp = QObject::tr("Error-->Node Workstation group %1 IPs %2 are same!!!").arg(pGroup->m_strGroup).arg(pGroup->m_arrItem[0]->m_strNetwork_IP_A);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}

			}
			else if (pGroup->m_arrItem.size() == 1)
			{
				nOccNo++;

				pGroup->m_arrItem[0]->m_nOccNo = nOccNo;

				pGroup->m_arrItem[0]->m_nSlaveOccNo = pGroup->m_arrItem[0]->m_nOccNo;
			}
			else if (pGroup->m_arrItem.size() == 0)
			{
				auto strTmp = QObject::tr("Error-->Node Workstation Group %1  member node count is 0!!!");
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
		}
		else
		{
			for each (auto var in pGroup->m_arrItem)
			{
				nOccNo++;

				var->m_nOccNo = nOccNo;

				var->m_nSlaveOccNo = var->m_nOccNo;
			}
		}


		for each (auto var in pGroup->m_arrItem)
		{
			writer.writeStartElement("n");

			writer.writeAttribute("TagName", QString("%1").arg(var->m_szTagName));
			if (!CheckTagNameIsValid(var->m_szTagName, NODE_CONFIG_DESC))
			{
				auto strTmp = QObject::tr("Error-->Node Workstation Group %1 Node TagName %2 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("HostName", QString("%1").arg(var->m_strHostName));
			//const auto it = pHash->find(var->m_strHostName.toStdString());
			//if (it == pHash->end())
			//{
			//	//字符串池
			//	*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			//	pHash->insert(std::make_pair(var->m_strHostName.toStdString(), *pDescStringPoolOccNo));
			//	pStringPoolVec->push_back(var->m_strHostName.toStdString());
			//	//desc occno
			//	writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			//}
			//else
			//{
			//	//desc occno
			//	writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			//}


			//nOccNo++;
			//var->m_nOccNo = nOccNo;

			writer.writeAttribute("OccNo", QString("%1").arg(var->m_nOccNo));

			nBlockNo++;
			var->m_nBlockNo = nBlockNo;
			writer.writeAttribute("BlockNo", QString("%1").arg(nBlockNo));

			writer.writeAttribute("NodeType", QString("%1").arg(var->m_nNodeType));
			writer.writeAttribute("Network_IP_A", QString("%1").arg(var->m_strNetwork_IP_A));
			QRegExp rx2(MYIPREG);
			if (!rx2.exactMatch(var->m_strNetwork_IP_A))
			{
				auto strTmp = QObject::tr("Error-->Node Workstation Group %1 TagName %2  Network_IP_A %3 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_A);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("Network_IP_B", QString("%1").arg(var->m_strNetwork_IP_B));
			if (!var->m_strNetwork_IP_B.isEmpty())
			{
				if (!rx2.exactMatch(var->m_strNetwork_IP_B))
				{
					auto strTmp = QObject::tr("Error-->Node Workstation Group %1 TagName %2  Network_IP_B %3 is invalid!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_B);
					MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
					s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
				}
			}

			//A网段和B网段不一样
			if (var->m_strNetwork_IP_A == var->m_strNetwork_IP_B)
			{
				auto strTmp = QObject::tr("Error-->Node Workstation Group %1  Node TagName %2  Network_IP_A %3 and Network_IP_B %4 are same!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strNetwork_IP_B).arg(var->m_strNetwork_IP_B);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("IsTimeSource", QString("%1").arg(var->m_bIsTimeSource));
			writer.writeAttribute("Program", QString("%1").arg(var->m_strProgram));
			
			writer.writeAttribute("Config", QString("%1").arg(var->m_strConfig));
			if (var->m_strConfig.isEmpty())
			{
				auto strTmp = QObject::tr("Error-->Node Workstation Group %1  Node TagName %2  Config %3 is empty!!!").arg(pGroup->m_strGroup).arg(var->m_szTagName).arg(var->m_strConfig);
				MYLIB::Log2File(LOG_NODE_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(NODE_CONFIG_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}
			
			writer.writeAttribute("GrpName", QString("%1").arg(var->m_strGroup));

			writer.writeAttribute("SlaveOccNo", QString("%1").arg(var->m_nSlaveOccNo));

			writer.writeAttribute("Service", QString("%1").arg(var->m_strService));

			//server节点服务
			for each (auto var in var->m_arrNodeServiceRole)
			{
				//[0]
				writer.writeStartElement("m");

				writer.writeAttribute("index", QString("%1").arg(var->Index));
				writer.writeAttribute("OccNo", QString("%1").arg(var->Index));
				writer.writeAttribute("Name", QString("%1").arg(var->Name));
				writer.writeAttribute("AppType", QString("%1").arg(var->AppType));
				writer.writeAttribute("LoadType", QString("%1").arg(var->LoadType));
				writer.writeAttribute("Argument", QString("%1").arg(var->Argument));
				writer.writeAttribute("Prority", QString("%1").arg(var->Prority));
				writer.writeAttribute("Description", QString("%1").arg(var->Description));
				const auto it = pHash->find(var->Description.toStdString());
				if (it == pHash->end())
				{
					//字符串池
					*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
					pHash->insert(std::make_pair(var->Description.toStdString(), *pDescStringPoolOccNo));
					pStringPoolVec->push_back(var->Description.toStdString());
					//DescriptionOccNo

					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
				}
				else
				{
					writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
				}

				//[0]
				writer.writeEndElement();
			}

			writer.writeEndElement();
		}
		return true;
	}
	 
}