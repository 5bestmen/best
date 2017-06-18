#include "predictdatainfo.h"

extern IMainModuleInterface *s_pGlobleCore;



CItemDataInfo::CItemDataInfo()
{


}


CItemDataInfo::~CItemDataInfo()
{


}

/*! \fn CPredictGroup::CPredictGroup()
********************************************************
** \brief CPredictGroup::CPredictGroup()
** \details 厂站组构造初始化
** \return
** \author GuoHaijun
** \date 2017年3月15日
** \note
********************************************************/
CPredictGroup::CPredictGroup()
{

	

}

/*! \fn CPredictGroup::CPredictGroup()
********************************************************
** \brief CPredictGroup::~CPredictGroup()
** \details 厂站组析构函数
** \return
** \author GuoHaijun
** \date 2017年3月15日
** \note
********************************************************/
CPredictGroup::~CPredictGroup()
{
	auto iter = m_mapItemInfo.begin();
	for (; iter != m_mapItemInfo.end(); ++iter)
	{
		delete iter.value();
		iter.value() = nullptr;
	}
	m_mapItemInfo.clear();

	auto  it = m_mapGroups.begin();
	for (; it != m_mapGroups.end(); ++it)
	{
		delete it.value();
		it.value() = nullptr;
	}
	m_mapGroups.clear();

}


/*! \fn QString CPredictGroup::GetGroupName(const QString& strGroupName)
************************************************************************
** \brief CPredictGroup::GetGroupName
** \details 获取组名
** \param strGroupName
** \return QString
** \author ghj
** \date 2017年2月14日
** \note
***********************************************************************/
QString CPredictGroup::GetGroupName(const QString& strGroupName)
{
	if (strGroupName.isEmpty())
	{
		return "";
	}

	int nIndex = 0;

	QString strName = strGroupName;
	
	while (1)
	{
		if (nIndex == 0)
		{
			strName = strGroupName;
		} 
		else
		{
			strName = QString("%1%2%3%4").arg(strGroupName).arg(LEFT_BRACKETS).arg(nIndex).arg(RIGHT_BRACKETS);
		}

		auto iter = m_mapGroups.find(strName);
		if(iter == m_mapGroups.end())
		{
			return std::move(strName);
		}
		nIndex++;
	}
}


bool CPredictGroup::IsPredictGroup(const QString& strGroupNameKey, CPredictGroup* pPredictItem)
{
	if (strGroupNameKey.isEmpty() || pPredictItem == nullptr)
	{
		return false;
	}

	auto iter = m_mapGroups.find(strGroupNameKey);
	if (iter == m_mapGroups.end())
	{
		m_mapGroups[strGroupNameKey] = pPredictItem;
	}

	return true;
}

bool CPredictGroup::IsPredictItem(const QString& strGroupNameKey, const CPredictItem* pItem)
{
	if (strGroupNameKey.isEmpty() || pItem == nullptr)
	{
		return false;
	}

	auto iter = m_mapItemInfo.find(strGroupNameKey);
	if (iter == m_mapItemInfo.end())
	{
		m_mapItemInfo[strGroupNameKey] = const_cast<CPredictItem*>(pItem);
		
		return true;
	}

	if (m_mapItemInfo[strGroupNameKey] == nullptr)
	{
		m_mapItemInfo[strGroupNameKey] = const_cast<CPredictItem*>(pItem);

		return true;
	}

	return false;
}


/*! \fn QString CPredictGroup::GetItemName(const QString& strItemName)
************************************************************************
** \brief CPredictGroup::GetItemName
** \details 获取item名
** \param
** \return QString
** \author GuoHaijun
** \date 2017年3月15日
** \note
***********************************************************************/
QString CPredictGroup::GetItemName(const QString& strItemName)
{
	if (strItemName.isEmpty())
	{
		return"";
	}

	int nIndex = 0;

	QString strName = strItemName;
	while (true)
	{
		if (nIndex == 0)
		{
			strName = strItemName;
		}
		else
		{
			strName = QString("%1%2%3%4").arg(strItemName).arg(LEFT_BRACKETS).arg(nIndex).arg(RIGHT_BRACKETS);
		}

		auto iter = m_mapItemInfo.find(strName);
		if (iter == m_mapItemInfo.end())
		{
			return std::move(strName);
		}

		nIndex++;
	}
}

/*! \fn CPredictGroup* CPredictGroup::CreateGroup()
************************************************************************
** \brief CPredictGroup::CreateGroup
** \details 创建组,分配内存
** \param 
** \return CPredictGroup*
** \author GuoHaijun
** \date 2017年3月14日
** \note
***********************************************************************/
CPredictGroup* CPredictGroup::CreateGroup()
{
	int nIndex = (int)m_mapGroups.size();
	QString strName = "";

	while (true)
	{
		strName = QString("%1%2").arg(PREDICT_GROUP_NAME).arg(nIndex);

		auto it = m_mapGroups.find(strName);
		if (it == m_mapGroups.end())
		{
			auto pGroup = new CPredictGroup();
			pGroup->m_strGroupName = strName;

			m_mapGroups[strName] = pGroup;

			return pGroup;
		}
		else
		{
			nIndex++;
		}
	}

	return nullptr;
}

/*! \fn CPredictItem* CPredictGroup::CreateItem()
************************************************************************
** \brief CPredictGroup::CreateItem
** \details 创建项,分配内存
** \param
** \return CPredictItem*
** \author GuoHaijun
** \date 2017年3月15日
** \note
***********************************************************************/
CPredictItem* CPredictGroup::CreateItem()
{
	int nIndex = (int)m_mapItemInfo.size();
	QString strName = "";

	while (true)
	{
		strName = QString("%1%2").arg(PREDICT_ITEM_NAME).arg(nIndex);

		auto it = m_mapItemInfo.find(strName);
		if (it == m_mapItemInfo.end())
		{
			auto pItem = new CPredictItem();
			pItem->m_strItemName = strName;

			m_mapItemInfo[strName] = pItem;

			return pItem;
		}
		else
		{
			nIndex++;
		}
	}

	return nullptr;

}


/*! \fn bool CPredictGroup::DeleteItem(const QString& strTagGroup)
************************************************************************
** \brief CPredictGroup::DeleteItem
** \details 删除项,释放内存
** \param
** \return bool
** \author GuoHaijun
** \date 2017年3月15日
** \note
***********************************************************************/
bool CPredictGroup::DeleteItem(const QString& strTagGroup)
{
	auto iter = m_mapItemInfo.find(strTagGroup);
	if (iter == m_mapItemInfo.end())
	{
		return false;
	}
	delete iter.value();
	m_mapItemInfo.erase(iter);

	return true;
}


/*! \fn bool CPredictGroup::DeleteGroup(const &strLastTagName)
************************************************************************
** \brief CPredictGroup::DeleteGroup
** \details 删除组,释放内存
** \param
** \return bool
** \author GuoHaijun
** \date 2017年3月15日
** \note
***********************************************************************/
bool CPredictGroup::DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName)
{
	Q_UNUSED(pPrdtGroup);

	auto iter = m_mapGroups.find(strLastTagName);
	if (iter == m_mapGroups.end())
	{
		return false;
	}

	delete iter.value();

	m_mapGroups.erase(iter);

	return true;
}


/*! \fn bool CPredictGroup::ModifyItem(const QString& strTagName, const QString& strLastItemName)
*************************************************************************************************
** \brief CPredictGroup::ModifyItem
** \details 修改item
** \param strTagName
** \param strLastItemName
** \return bool
** \author GuoHaijun
** \date 2017年3月15日
** \note
*************************************************************************************************/
bool CPredictGroup::ModifyItem(const QString& strTagName, const QString& strLastItemName)
{
	auto iter = m_mapItemInfo.find(strLastItemName);
	if (iter != m_mapItemInfo.end())
	{
		memset(iter.value()->m_strTagName, 0, sizeof(iter.value()->m_strTagName));
		int nSize = strTagName.size();
		strncpy(iter.value()->m_strTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));
		std::swap(m_mapItemInfo[strTagName], iter.value());
		m_mapItemInfo.erase(iter);

		return true;
	}

	auto it = m_mapGroups.begin();
	for (; it != m_mapGroups.end(); ++it)
	{
		if (it.value()->ModifyItem(strTagName, strLastItemName))
		{
			return true;
		}
	}

	return false;
}

/*! \fn bool CPredictGroup::ModifyGroup(const QString& strTagName, const QString& strLastName)
**********************************************************************************************
** \brief CPredictGroup::ModifyGroup
** \details 修改组
** \param
** \return bool
** \author GuoHaijun
** \date 2017年3月15日
** \note
**********************************************************************************************/
bool CPredictGroup::ModifyGroup(const QString& strTagName, const QString& strLastName)
{
	auto iter = m_mapGroups.find(strLastName);
	if (iter != m_mapGroups.end())
	{
		iter.value()->m_strGroupName = strTagName;

		std::swap(m_mapGroups[strTagName], iter.value());

		m_mapGroups.erase(iter);

		return true;
	}

	return false;

}










CPredictItem::CPredictItem()
{
	m_nID = 0;
	m_strDesc = "test";
	memset(m_strTagName, 0, MAX_NAME_LENGTH);

}


CPredictItem::~CPredictItem()
{
	auto iter = m_mapDevice.begin();

	for (; iter != m_mapDevice.end(); ++iter)
	{
		delete iter.value();
		iter.value() = nullptr;
	}
	m_mapDevice.clear();

	for (size_t i = 0; i < m_pPredictDevices.size(); i++)
	{
		if (m_pPredictDevices[i] != nullptr)
		{
			delete m_pPredictDevices[i];
			m_pPredictDevices[i] = nullptr;
		}
	}
	m_pPredictDevices.clear();

}

/*! \fn QString CPredictItem::GetDeviceName(const QString& strDeviceName)
************************************************************************
** \brief CPredictItem::GetDeviceName
** \details 获取装置名
** \param strDeviceName
** \return QString
** \author GuoHaijun
** \date 2017年3月14日
** \note
***********************************************************************/
QString CPredictItem::GetDeviceName(const QString& strDeviceName, CPredictDevice* pPrdtDevice)
{
	if (strDeviceName.isEmpty())
	{
		return "";
	}

	Q_ASSERT(pPrdtDevice);

	if (!pPrdtDevice)
	{
		return "";
	}
	int nIndex = 0;

	QString strName = strDeviceName;
	while (true)
	{
		if (nIndex == 0)
		{
			strName = strDeviceName;
		}
		else
		{
			strName = QString("%1%2%3%4").arg(strDeviceName).arg(LEFT_BRACKETS).arg(nIndex).arg(RIGHT_BRACKETS);
		}

		if (m_mapDevice.isEmpty())
		{	
			m_mapDevice[strName] = pPrdtDevice;
			return std::move(strName);
		}
		else
		{
			auto iter = m_mapDevice.find(strName);
			if (iter==m_mapDevice.end())
			{
				m_mapDevice[strName] = pPrdtDevice;
				return std::move(strName);
			}
		}

		nIndex++;
	}
}

/*! \fn QString CPredictItem::GetItemName(const QString& strItemName)
************************************************************************
** \brief CPredictItem::GetItemName
** \details 获取项名
** \param strItemName
** \return QString
** \author GuoHaijun
** \date 2017年3月14日
** \note
***********************************************************************/
QString CPredictItem::GetItemName(const QString& strItemName)
{
	if (strItemName.isEmpty())
	{
		return "";
	}

	int nIndex = 0;

	QString strName = strItemName;
	while (true)
	{
		if (nIndex == 0)
		{
			strName = strItemName;
		}
		else
		{
			strName = QString("%1%2%3%4").arg(strItemName).arg(LEFT_BRACKETS).arg(nIndex).arg(RIGHT_BRACKETS);
		}

		auto iter = m_mapItem.find(strName);
		if (iter == m_mapItem.end())
		{
			return std::move(strName);
		}

		nIndex++;
	}
}

bool CPredictItem::IsPredictItem(const QString& strItemNameKey, const CPredictItem* pItem)
{
	if (strItemNameKey.isEmpty() || pItem == nullptr)
	{
		return false;
	}

	auto iter = m_mapItem.find(strItemNameKey);
	if (iter == m_mapItem.end())
	{
		m_mapItem[strItemNameKey] = const_cast<CPredictItem*>(pItem);

		return true;
	}

	if (m_mapItem[strItemNameKey] == nullptr)
	{
		m_mapItem[strItemNameKey] = const_cast<CPredictItem*>(pItem);

		return true;
	}

	return false;


}

bool CPredictItem::IsPredictDevice(const QString& strDeviceNameKey, const CPredictDevice* pItem)
{
	if (strDeviceNameKey.isEmpty() || pItem == nullptr)
	{
		return false;
	}

	auto iter = m_mapDevice.find(strDeviceNameKey);
	if (iter == m_mapDevice.end())
	{
		m_mapDevice[strDeviceNameKey] = const_cast<CPredictDevice*>(pItem);

		return true;
	}

	if (m_mapDevice[strDeviceNameKey] == nullptr)
	{
		m_mapDevice[strDeviceNameKey] = const_cast<CPredictDevice*>(pItem);

		return true;
	}

	return false;
}


bool CPredictItem::SavePredictItemData(const QString& szRoot, int nOccNo)
{
	QString fileName = QString("%1%2").arg(m_strTagName).arg(".xml");

	QString strPath = szRoot + "powerprediction";

	strPath = strPath + fileName;

	QFile file(strPath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QString strTmp = QString(QObject::tr("open %1 failed!")).arg(fileName);

		return false;
	}

	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();

	writer.writeStartElement("prediction");
	writer.writeAttribute("Description", m_strDesc);
	writer.writeAttribute("TagName", m_strTagName);
	writer.writeAttribute("version", "V1.0");
	writer.writeAttribute("ID", QString("%1").arg(m_nID));
	m_nOccNo = nOccNo;
	writer.writeAttribute("OccNo", QString("%1").arg(m_nOccNo));
	
	SaveChildItemData(writer, m_pPredictDevices);

	writer.writeEndElement();
	writer.writeEndDocument();
	
	return true;
}

bool CPredictItem::SaveChildItemData(QXmlStreamWriter& writer, QVector<CPredictDevice* > pPredictDevice)
{
	writer.writeStartElement("device");
	
	writer.writeAttribute("Count", QString("%1").arg(m_pPredictDevices.size()));

	int nOccNoAI = 0;
	int nOccNoDI = 0;
	int nOccNoParam = 0;
	int nOccNoDevice = 0;
	for (auto const & it : m_pPredictDevices)
	{
		it->SavePredictDevice(writer, nOccNoDevice, nOccNoAI, nOccNoDI, nOccNoParam);
	}

	writer.writeEndElement();

	return true;
}

bool CPredictItem::LoadPredictItemData(QXmlStreamReader &reader)
{




	return true;
}

void CPredictItem::AddPredictDevice(CPredictDevice* pDevice)
{
	m_pPredictDevices.push_back(pDevice);
}

bool CPredictItem::DeletePredictDevice(CPredictItem* pPrdtItem, const QString& strLastTagName)
{
	auto iter = pPrdtItem->m_mapDevice.find(strLastTagName);
	if (iter == pPrdtItem->m_mapDevice.end())
	{
		delete iter.value();
		iter.value() = nullptr;
		m_mapDevice.erase(iter);
	}
	
	return true;
}


bool CPredictItem::LoadDeviceData(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "device")
			{

			}
			else if (strTmp == "d")
			{
				int32u nOccNo = reader.attributes().value("OccNo").toInt();
				Q_ASSERT(nOccNo >= MIN_OCCNO_PREDICTION && nOccNo <= MAX_OCCNO_PREDICTION);
				if (nOccNo <= MIN_OCCNO_PREDICTION || nOccNo > MAX_OCCNO_PREDICTION)
				{
					reader.readNext();
					continue;
				}

				QString strTagName = reader.attributes().value("TagName").toString();
				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_PREDICTION);

				if (strTagName.isEmpty() == true)
				{
					reader.readNext();

					continue;
				}

				CPredictDevice* pDevice = new CPredictDevice(s_pGlobleCore);
				pDevice->SetOccNo(nOccNo);
				memset(pDevice->m_szTagName, 0, sizeof(pDevice->m_szTagName));
				int nSize = strTagName.size();
				strncpy(pDevice->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, nSize));

				int32u nID = reader.attributes().value("ID").toUInt();
				pDevice->m_nID = nID;

				QString strDescription = reader.attributes().value("Desc").toString();
				pDevice->m_strDesc = strDescription;

				int8u nType = reader.attributes().value("Type").toInt();
				pDevice->m_nDeviceType = nType;

				QString strDeviceAddr = reader.attributes().value("DeviceAddr").toString();
				pDevice->m_strDeviceAddr = strDeviceAddr;

				pDevice->LoadData(reader, pPrdtItem);

				m_pPredictDevices.push_back(pDevice);

			}
		}
		else if (reader.isEndElement() && strTmp == "device")
		{
			break;
		}

		reader.readNext();
	}

	return true;
}

/*! \fn CPredictRootItem::CPredictRootItem()
************************************************************************
** \brief CPredictRootItem::CPredictRootItem
** \details  预测管理构造
** \param 
** \return 
** \author GuoHaijun
** \date 2017年3月16日
** \note
***********************************************************************/
CPredictRootItem::CPredictRootItem()
{
	//预测项vector
	m_vecPredictItemDatas.clear();

	//预测项map
	m_mapPredictItem.clear();

	//预测组,分配内存
	m_pPredictGroup = new CPredictGroup();
}


/*! \fn CPredictRootItem::~CPredictRootItem()
************************************************************************
** \brief CPredictRootItem::~CPredictRootItem
** \details  预测管理析构
** \param
** \return
** \author GuoHaijun
** \date 2017年3月16日
** \note
***********************************************************************/
CPredictRootItem::~CPredictRootItem()
{
	auto iter = m_mapPredictItem.begin();
	for (; iter != m_mapPredictItem.end(); ++iter)
	{
		delete iter.value();
		iter.value() = nullptr;

	}
	m_mapPredictItem.clear();

	//释放预测组内存
	if (m_pPredictGroup != nullptr)
	{
		delete m_pPredictGroup;
		m_pPredictGroup = nullptr;
	}
}

bool CPredictRootItem::SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot)
{
	Q_ASSERT(m_pPredictGroup);
	if (!m_pPredictGroup)
	{
		return false;
	}

	SaveChildPredictNode(pXml, pElement, m_pPredictGroup);

	int nOccNo = 0;

	SaveChildItem(m_pPredictGroup, szRoot, nOccNo);

	return true;

}

bool CPredictRootItem::SaveChildPredictNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPredictGroup)
{
	Q_ASSERT(pXml);
	Q_ASSERT(pPredictGroup);
	if (pXml == nullptr || pXml == nullptr)
	{
		return false;
	}

	auto iterGroup = pPredictGroup->m_mapGroups.begin();
	for (; iterGroup != pPredictGroup->m_mapGroups.end(); ++iterGroup)
	{
		QDomElement dElement = pXml->createElement("group");
		dElement.setAttribute("name", iterGroup.key());

		pElement.appendChild(dElement);

		SaveChildPredictNode(pXml, dElement, iterGroup.value());
	}

	auto iterItem = pPredictGroup->m_mapItemInfo.begin();
	for (; iterItem != pPredictGroup->m_mapItemInfo.end(); ++iterItem)
	{
		QDomElement dElement = pXml->createElement("file");
		auto strFile = QString("%1.xml").arg(iterItem.key());
		dElement.setAttribute("name", iterItem.key());

		pElement.appendChild(dElement);
	}

	return true;
}


bool CPredictRootItem::SaveChildItem(CPredictGroup* pPredictGroup, const QString& szRoot, int nOccNo)
{
	auto it = pPredictGroup->m_mapItemInfo.begin();
	for (; it != pPredictGroup->m_mapItemInfo.end(); ++it)
	{
		nOccNo++;
		it.value()->SavePredictItemData(szRoot, nOccNo);
	}

	auto iter = pPredictGroup->m_mapGroups.begin();
	for (; iter != pPredictGroup->m_mapGroups.end(); ++iter)
	{
		SaveChildItem(iter.value(), szRoot, nOccNo);
	}

	return true;
}


bool CPredictRootItem::LoadProjectPredictNode(QDomElement& pElement, const QString& szRoot)
{
	QDomNode node = pElement.firstChild();

	m_mapPredictItem.clear();

	while (!node.isNull())
	{
		QDomElement dElement = node.toElement();

		auto strTmp = dElement.tagName();

		if (strTmp.compare("group",Qt::CaseInsensitive) == 0)
		{
			QString strGroupName = dElement.attribute("name");
			if (strGroupName.isEmpty() == false)
			{
				CPredictGroup* pPredictGrp = new CPredictGroup();
				pPredictGrp->m_strGroupName = strGroupName;
				LoadPredictChildGroup(pPredictGrp, dElement, szRoot);

				m_pPredictGroup->m_mapGroups[strGroupName] = pPredictGrp;
				
			}
		}
		else if (strTmp.compare("file", Qt::CaseInsensitive) == 0)
		{
			QString strTagName = pElement.attribute("name");
			CPredictItem* pPredictItem = new CPredictItem();
			memset(pPredictItem->m_strTagName, 0, sizeof(pPredictItem->m_strTagName));

			int nSize = strTagName.size();
			strncpy(pPredictItem->m_strTagName, strTagName.toStdString().c_str(), qMin(512, nSize));

			m_pPredictGroup->m_mapItemInfo[strTagName] = pPredictItem;


		}

		node = node.nextSibling();

	}

	return true;

}


bool CPredictRootItem::LoadPredictChildGroup(CPredictGroup* pPredictGrp, QDomElement& pElement, const QString& szRoot)
{
	QDomNode node = pElement.firstChild();

	while(!node.isNull())
	{
		QDomElement dElement = node.toElement();
		auto strTmp = dElement.tagName();

		if (strTmp.compare("group", Qt::CaseInsensitive) == 0)
		{
			QString strGroupName = dElement.attribute("name");
			CPredictGroup* pPrdtGrp = new CPredictGroup();
			pPrdtGrp->m_strGroupName = strGroupName;

			m_pPredictGroup->m_mapGroups[strGroupName] = pPrdtGrp;

			LoadPredictChildGroup(pPrdtGrp, dElement, szRoot);
		}
		else if (strTmp.compare("file", Qt::CaseInsensitive) == 0)
		{
			QString strTagName = dElement.attribute("name");

			CPredictItem* pPredictItem = new CPredictItem();
			memset(pPredictItem->m_strTagName, 0, sizeof(pPredictItem->m_strTagName));
			int nSize = strTagName.size();
			strncpy(pPredictItem->m_strTagName, strTagName.toStdString().c_str(), qMin(512, nSize));
			pPredictGrp->m_mapItemInfo[strTagName] = pPredictItem;

			LoadPredictChildItem(pPredictItem, pPredictItem->m_strTagName);

		}

		node = node.nextSibling();
	}

	return true;
}


bool CPredictRootItem::LoadPredictChildItem(CPredictItem* pPredictItem, const QString& szRoot)
{
	LoadPredictFile(pPredictItem, szRoot);

	return true;

}


bool CPredictRootItem::LoadPredictFile(CPredictItem* pPredictItem, const QString& filePath)
{
	QXmlStreamReader xml;
	//auto strPath = s_pGlobleCore->GetProjMgr()->GetFile()->fileName();
	//auto nIndex = strPath.lastIndexOf("/");
	//strPath = strPath.mid(0, nIndex);
	QString strPath = "/powerprediction/";

	QString strTmp = strPath + "/" + filePath + ".xml";
	QFile file(strTmp);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		auto strError = QString(QObject::tr("load powerprediction file failed!!!"));

		return false;
	}
	xml.setDevice(&file);

	if (xml.readNextStartElement())
	{
		QString strTmp = xml.name().toString();

		QString strTmp1 = xml.attributes().value("version").toString();

		if (xml.name() == "prediction" && xml.attributes().value("version") == "1.0")
		{
			QString strTagName = xml.attributes().value("TagName").toString();

			int nSize = strTagName.size();
			strncpy(pPredictItem->m_strTagName, strTagName.toStdString().c_str(), qMin(512, nSize));

			QString strDesc = xml.attributes().value("Description").toString();
			pPredictItem->m_strDesc = strDesc;

			int nID = xml.attributes().value("ID").toInt();

			bool bFlag = true;
			if (nID >= 0)
			{
				bFlag = true;
			}
			else
			{
				bFlag = false;
			}

			if (!bFlag)
			{
				auto strPredictError = QString("ID %1 of powerprediction %! is invalid.").arg(nID).arg(strTagName);

				delete pPredictItem;
				pPredictItem = nullptr;
				return false;
			}

			if (strTagName.isEmpty())
			{
				auto strPredictError = QString("powerprediction %1 is empty").arg(strTagName);
				
				delete pPredictItem;
				pPredictItem = nullptr;

				return false;
			}

			auto iter = m_mapPredictItem.find(pPredictItem->m_strTagName);
			if (iter != m_mapPredictItem.end())
			{
				file.close();

				return false;
			}

			pPredictItem->LoadPredictItemData(xml);

			m_vecPredictItemDatas.push_back(pPredictItem);

			
			
		}

		file.close();
	}

	return true;
}

void CPredictRootItem::Clear()
{
	m_vecPredictItemDatas.clear();

	//QVector<CPredictItem*>().swap(m_vecPredictItemDatas);

	m_mapPredictItem.clear();

	if (m_pPredictGroup)
	{
		delete m_pPredictGroup;
		m_pPredictGroup = nullptr;
	}
}


void CPredictRootItem::Create()
{
	if (m_pPredictGroup == nullptr)
	{
		m_pPredictGroup = new CPredictGroup();
	}
}

CPredictItem* CPredictRootItem::AddNewPredictItem(CPredictGroup* pPredictGroup)
{
	CPredictItem* pData = new CPredictItem();
	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		return nullptr;
	}

	auto index = (int)m_mapPredictItem.size();

	while (true)
	{
		QString strStation = QString("station%1").arg(index);
		QString strDesc = QString(QObject::tr("station%1")).arg(index);

		auto iter = m_mapPredictItem.find(strStation);
		if (iter == m_mapPredictItem.end())
		{
			m_mapPredictItem[strStation] = pData;
		}
		else
		{
			index++;

			continue;
		}

		//厂站ID，从0开始？？？？
		pData->m_nID = index;

		pData->m_strDesc = strDesc;

		memset(pData->m_strTagName, 0, sizeof(pData->m_strTagName));

		int nSize = strStation.size();
		strncpy(pData->m_strTagName, strStation.toStdString().c_str(), qMin(512, nSize));

		pPredictGroup->m_mapItemInfo.insert(strStation, pData);

		break;
	}

	m_vecPredictItemDatas.push_back(pData);
	
	return pData;
}


bool CPredictRootItem::DeleteHashPredictItem(QString& strPredictTag)
{
	auto iter = m_mapPredictItem.find(strPredictTag);
	if (iter != m_mapPredictItem.end())
	{
		m_mapPredictItem.erase(iter);
		return true;
	}

	return false;
}

bool CPredictRootItem::DeleteItem()
{
	QVector<CPredictItem*>().swap(m_vecPredictItemDatas);
	Q_ASSERT(m_vecPredictItemDatas.capacity() == 0);

	return true;

}


QString CPredictRootItem::GetRootItemName(const QString& strRootItemName)
{
	if (strRootItemName.isEmpty())
	{
		return"";
	}

	int nIndex = 0;

	QString strName = strRootItemName;
	while (true)
	{
		if (nIndex == 0)
		{
			strName = strRootItemName;
		}
		else
		{
			strName = QString("%1%2%3%4").arg(strRootItemName).arg(LEFT_BRACKETS).arg(nIndex).arg(RIGHT_BRACKETS);
		}

		auto iter = m_mapRootItemInfo.find(strName);
		if (iter == m_mapRootItemInfo.end())
		{
			return std::move(strName);
		}

		nIndex++;
	}
}

bool CPredictRootItem::IsPredictItem(const QString& strItemName, const CPredictItem* pItem)
{
	if (strItemName.isEmpty() || pItem == nullptr)
	{
		return false;
	}

	auto iter = m_mapPredictItem.find(strItemName);
	if (iter == m_mapPredictItem.end())
	{
		m_mapPredictItem[strItemName] = const_cast<CPredictItem*>(pItem);

		return true;
	}

	if (m_mapPredictItem[strItemName] == nullptr)
	{
		m_mapPredictItem[strItemName] = const_cast<CPredictItem*>(pItem);

		return true;
	}

	return false;
}

/*! \fn CDiInput::CDiInput()
********************************************************
** \brief CDiInput::CDiInput
** \details 遥信构造初始化
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CDiInput::CDiInput()
{
	//遥信ID从1开始
	m_nID = 1;
	m_strDesc = "";
	//m_szTagName = "";
	m_nOccNo = 0;
	m_nBlockOccNo = 0;
	memset(m_szTagName, 0, MAX_NAME_LENGTH);
}


/*! \fn CDiInput::CDiInput()
********************************************************
** \brief CDiInput::~CDiInput
** \details 遥信析构
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CDiInput::~CDiInput()
{



}


/*! \fn CAiInput::CAiInput()
********************************************************
** \brief CAiInput::CAiInput
** \details 遥测构造初始化
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CAiInput::CAiInput()
{
	//遥测ID从1开始
	m_nID = 1;
	m_strDesc = "";
	//m_szTagName = "";
	m_nOccNo = 0;
	m_nBlockOccNo = 0;
	memset(m_szTagName, 0, MAX_NAME_LENGTH);
}


/*! \fn CAiInput::CAiInput()
********************************************************
** \brief CAiInput::~CAiInput
** \details 遥测析构
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CAiInput::~CAiInput()
{
	
}

/*! \fn CParamInput::CParamInput()
********************************************************
** \brief CParamInput::CParamInput
** \details 参数构造初始化
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CParamInput::CParamInput()
{
	m_nID = 1;
	m_strDesc = "";
	//m_szTagName = "";
	m_nOccNo = 0;
	m_dValue = 0.0;
	m_nBlockOccNo = 0;
	memset(m_szTagName, 0, MAX_NAME_LENGTH);

}


/*! \fn CParamInput::CParamInput()
********************************************************
** \brief CParamInput::~CParamInput
** \details 参数析构
** \return
** \author GuoHaijun
** \date 2016年3月13日
** \note
********************************************************/
CParamInput::~CParamInput()
{


}



CPredictDevice::CPredictDevice(IMainModuleInterface *pCore)
{
	m_nID = 0; 
	m_nOccNo = 0;
	m_strDesc = "";	
	m_nDeviceType = 0;
	m_strDeviceAddr = "";
	//m_nDeviceModelId = 0;
	memset(m_szTagName, 0, MAX_NAME_LENGTH);

	Q_ASSERT(pCore);
	if (!pCore)
	{
		return;
	}
	m_pCore = pCore;
}


CPredictDevice::~CPredictDevice()
{
	for (size_t i = 0; i < m_vecAis.size(); i++)
	{
		delete m_vecAis[i];
		m_vecAis[i] = nullptr;
	}
	m_vecAis.clear();

	for (size_t i = 0; i < m_vecDis.size(); i++)
	{
		delete m_vecDis[i];
		m_vecDis[i] = nullptr;
	}
	m_vecDis.clear();

	for (size_t i = 0; i < m_vecParams.size(); i++)
	{
		delete m_vecParams[i];
		m_vecParams[i] = nullptr;
	}
	m_vecParams.clear();
	
}

void CPredictDevice::AddAiInput(CAiInput* pAnalogInput)
{
	Q_ASSERT(pAnalogInput);
	if (pAnalogInput == nullptr)
	{
		return;
	}

	m_vecAis.push_back(pAnalogInput);

}

void CPredictDevice::AddDiInput(CDiInput* pDigitalInput)
{
	Q_ASSERT(pDigitalInput);
	if (pDigitalInput == nullptr)
	{
		return;
	}

	m_vecDis.push_back(pDigitalInput);
}

void CPredictDevice::AddParamInput(CParamInput* pParamInput)
{
	Q_ASSERT(pParamInput);
	if (pParamInput == nullptr)
	{
		return;
	}

	m_vecParams.push_back(pParamInput);
}

bool CPredictDevice::ReadAINode(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "ai")
			{
				ReadAiValue(reader, pPrdtItem);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}


bool CPredictDevice::ReadDINode(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "di")
			{
				ReadDiValue(reader, pPrdtItem);
			}
		}
		else if(reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}


bool CPredictDevice::ReadParamNode(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "param")
			{
				ReadParamValue(reader, pPrdtItem);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;

}


bool CPredictDevice::ReadAiValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "ai")
			{
				int32u nOccNo = reader.attributes().value("OccNo").toInt();
				Q_ASSERT(nOccNo >= MIN_OCCNO_PREDICTION && nOccNo <= MAX_OCCNO_PREDICTION);

				if (nOccNo < MIN_OCCNO_PREDICTION || nOccNo > MAX_OCCNO_PREDICTION)
				{
					auto strError = QObject::tr("ai OccNo is invalid!!!");

					reader.readNext();

					continue;
				}

				QString strTagName = reader.attributes().value("TagName").toString();
				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_PREDICTION);
				if (strTagName.isEmpty() == true)
				{
					auto strError = QObject::tr("ai tag name is empty!!!!!!");

					reader.readNext();

					continue;
				}

				CAiInput* pAnalog = new CAiInput(nOccNo);
				memset(pAnalog->m_szTagName, 0, MAX_NAME_LENGTH);
				strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strTagName.size()));
				pAnalog->SetOccNo(nOccNo);

				int32u nID = reader.attributes().value("ID").toUInt();
				pAnalog->m_nID = nID;

				int32u nBlockOccNo = reader.attributes().value("BlockNo").toUInt();
				pAnalog->m_nBlockOccNo = nBlockOccNo;

				QString strDescription = reader.attributes().value("Desc").toString();
				pAnalog->m_strDesc = strDescription;

				m_vecAis.push_back(pAnalog);




			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();

	}

	return true;
}

bool CPredictDevice::ReadDiValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "di")
			{
				int32u nOccNo = reader.attributes().value("OccNo").toInt();
				Q_ASSERT(nOccNo >= MIN_OCCNO_PREDICTION && nOccNo <= MAX_OCCNO_PREDICTION);

				if (nOccNo < MIN_OCCNO_PREDICTION || nOccNo > MAX_OCCNO_PREDICTION)
				{
					auto strError = QObject::tr("di OccNo is invalid!!!");

					reader.readNext();

					continue;
				}

				QString strTagName = reader.attributes().value("TagName").toString();
				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_PREDICTION);
				if (strTagName.isEmpty() == true)
				{
					auto strError = QObject::tr("ai tag name is empty!!!!!!");

					reader.readNext();

					continue;
				}

				CDiInput* pDigital = new CDiInput();
				pDigital->SetOccNo(nOccNo);
				memset(pDigital->m_szTagName, 0, MAX_NAME_LENGTH);
				strncpy(pDigital->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strTagName.size()));

				int32u nID = reader.attributes().value("ID").toUInt();
				pDigital->m_nID = nID;

				int32u nBlockOccNo = reader.attributes().value("BlockNo").toUInt();
				pDigital->m_nBlockOccNo = nBlockOccNo;

				QString strDescription = reader.attributes().value("Desc").toString();
				pDigital->m_strDesc = strDescription;

				m_vecDis.push_back(pDigital);




			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();

	}

	return true;

}

bool CPredictDevice::ReadParamValue(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "param")
			{
				int32u nOccNo = reader.attributes().value("OccNo").toInt();
				Q_ASSERT(nOccNo >= MIN_OCCNO_PREDICTION && nOccNo <= MAX_OCCNO_PREDICTION);

				if (nOccNo < MIN_OCCNO_PREDICTION || nOccNo > MAX_OCCNO_PREDICTION)
				{
					auto strError = QObject::tr("di OccNo is invalid!!!");

					reader.readNext();

					continue;
				}

				QString strTagName = reader.attributes().value("TagName").toString();
				Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_PREDICTION);
				if (strTagName.isEmpty() == true)
				{
					auto strError = QObject::tr("ai tag name is empty!!!!!!");

					reader.readNext();

					continue;
				}

				CParamInput* pParam = new CParamInput();
				pParam->SetOccNo(nOccNo);
				memset(pParam->m_szTagName, 0, MAX_NAME_LENGTH);
				strncpy(pParam->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_NAME_LENGTH, strTagName.size()));

				int32u nID = reader.attributes().value("ID").toUInt();
				pParam->m_nID = nID;

				int32u nBlockOccNo = reader.attributes().value("BlockNo").toUInt();
				pParam->m_nBlockOccNo = nBlockOccNo;

				QString strDescription = reader.attributes().value("Desc").toString();
				pParam->m_strDesc = strDescription;

				double dDouble = reader.attributes().value("DValue").toDouble();
				pParam->m_dValue = dDouble;

				m_vecParams.push_back(pParam);


			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();

	}

	return true;

}

bool CPredictDevice::LoadData(QXmlStreamReader& reader, CPredictItem* pPrdtItem)
{
	Q_ASSERT(pPrdtItem);
	if (!pPrdtItem)
	{
		return false;
	}

	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();

		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "AI")
			{ 
				ReadAINode(reader, pPrdtItem);
			}
			else if (strTmp == "DI")
			{
				ReadDINode(reader, pPrdtItem);
			}
			else if (strTmp == "Param")
			{
				ReadParamNode(reader, pPrdtItem);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}
	
	return true;
}


bool CPredictDevice::SavePredictAI(QXmlStreamWriter& writer, int &nOccNoDevice, int &nAIOccNo)
{

	writer.writeStartElement("AI");
	writer.writeAttribute("Count", QString("%1").arg(m_vecAis.size()));

	Q_ASSERT(nAIOccNo + 1 > 0);
	int nBlockOccNo = 0;

	for (auto const& ai : m_vecAis)
	{
		writer.writeStartElement("ai");
		nAIOccNo++;
		ai->SetOccNo(nAIOccNo);
		writer.writeAttribute("ID", QString("%1").arg(ai->m_nID));
		writer.writeAttribute("TagName", QString("%1").arg(ai->m_szTagName));
		writer.writeAttribute("Desc", QString("%1").arg(ai->m_strDesc));
		writer.writeAttribute("OccNo", QString("%1").arg(nAIOccNo));
		nBlockOccNo++;
		ai->m_nBlockOccNo = nBlockOccNo;
		writer.writeAttribute("BlockNo", QString("%1").arg(ai->m_nBlockOccNo));

		writer.writeEndElement();
	}

	writer.writeEndElement();

	return true;
}


bool CPredictDevice::SavePredictDI(QXmlStreamWriter& writer, int &nOccNoDevice, int &nDIOccNo)
{
	writer.writeStartElement("DI");
	writer.writeAttribute("Count", QString("%1").arg(m_vecDis.size()));

	Q_ASSERT(nDIOccNo + 1 > 0);
	int nBlockOccNo = 0;

	for (auto const& di : m_vecDis)
	{
		writer.writeStartElement("di");
		nDIOccNo++;
		di->SetOccNo(nDIOccNo);
		writer.writeAttribute("ID", QString("%1").arg(di->m_nID));
		writer.writeAttribute("TagName", QString("%1").arg(di->m_szTagName));
		writer.writeAttribute("Desc", QString("%1").arg(di->m_strDesc));
		writer.writeAttribute("OccNo", QString("%1").arg(nDIOccNo));
		nBlockOccNo++;
		di->m_nBlockOccNo = nBlockOccNo;
		writer.writeAttribute("BlockNo", QString("%1").arg(di->m_nBlockOccNo));

		writer.writeEndElement();
	}

	writer.writeEndElement();

	return true;
}


bool CPredictDevice::SavePredictParam(QXmlStreamWriter& writer, int &nOccNoDevice, int &nParamOccNo)
{
	writer.writeStartElement("Param");
	writer.writeAttribute("Count", QString("%1").arg(m_vecDis.size()));

	Q_ASSERT(nParamOccNo + 1 > 0);
	int nBlockOccNo = 0;

	for (auto const& param : m_vecParams)
	{
		writer.writeStartElement("param");
		nParamOccNo++;
		param->SetOccNo(nParamOccNo);
		writer.writeAttribute("ID", QString("%1").arg(param->m_nID));
		writer.writeAttribute("TagName", QString("%1").arg(param->m_szTagName));
		writer.writeAttribute("Desc", QString("%1").arg(param->m_strDesc));
		writer.writeAttribute("OccNo", QString("%1").arg(nParamOccNo));
		writer.writeAttribute("DValue", QString("%1").arg(param->m_dValue));
		nBlockOccNo++;
		param->m_nBlockOccNo = nBlockOccNo;
		writer.writeAttribute("BlockNo", QString("%1").arg(param->m_nBlockOccNo));

		writer.writeEndElement();
	}

	writer.writeEndElement();

	return true;
}

bool CPredictDevice::SavePredictDevice(QXmlStreamWriter& writer, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nParamOccNo)
{
	writer.writeStartElement("d");
	nOccNoDevice++;
	Q_ASSERT(nOccNoDevice > 0);
	m_nOccNo = nOccNoDevice;
	writer.writeAttribute("OccNo", QString("%1").arg(nOccNoDevice));
	writer.writeAttribute("TagName", QString("%1").arg(m_szTagName));
	writer.writeAttribute("ID", QString("%1").arg(m_nID));
	writer.writeAttribute("Description", QString("%1").arg(m_strDesc));
	writer.writeStartElement("data");

	SavePredictAI(writer, nOccNoDevice, nAIOccNo);
	SavePredictDI(writer, nOccNoDevice, nDIOccNo);
	SavePredictParam(writer, nOccNoDevice, nParamOccNo);

	writer.writeEndElement();

	writer.writeEndElement();

	return true;

}




