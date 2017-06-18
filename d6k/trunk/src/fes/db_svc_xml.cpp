
#include "db_svc.h"
#include <QtMath>
#include <QColor>
#include <QFileInfo>
#include <QDebug>

//内部排序
template<typename T>
bool SortFuction(const T& T1, const T& T2)
{
	return T1->OccNo < T2->OccNo;
}
/*! \fn bool  CDbSvc::ReadXmlInfo(const char* pszPath, XmlReaderType nType)
********************************************************************************************************* 
** \brief CDbSvc::ReadXmlInfo 
** \details 
** \param pszPath  文件路径
** \param nType    读取方式类型
** \return bool 
** \author xingzhibing
** \date 2016年9月18日 
** \note 
********************************************************************************************************/
bool  CDbSvc::ReadXmlInfo(const char* pszPath, XmlReaderType nType)
{
	Q_ASSERT(pszPath);
	if (pszPath == Q_NULLPTR)
	{
		return false;
	}
	if (!QFileInfo(pszPath).exists())
	{
		//文件不对
		return false;
	}
	QFile file(pszPath);

	if (!file.open(QIODevice::ReadOnly | QFile::Text ))
	{
		return false;
	}

	switch (nType)
	{
	case _DOM_XML_TYPE:
	{
		QDomDocument document;
		if (!document.setContent(&file))
		{
			file.close();
			return false;
		}

		//ROOT_NODE
		QDomElement nRoot = document.documentElement();
		if (nRoot.tagName() != "fes")
		{
			return false;
		}

		//COMMON_NODE_CHANNEL
		QDomNode nNode = nRoot.firstChild();

		for (; !nNode.isNull(); nNode = nNode.nextSibling())
		{
			QString szName = nNode.nodeName();

			if (szName == "channel")
			{
				m_DomChannel = nNode.toElement();
				if (m_DomChannel.isNull())
				{
					return false;
				}
			}
			if (szName == "scale")
			{
				m_DomDev = nNode.toElement();
				if (m_DomDev.isNull())
				{
					return false;
				}
			}
			if (szName == "alarm")
			{
				m_DomData = nNode.toElement();
				if (m_DomData.isNull())
				{
					return false;
				}
			}
		}

		if (!ReadChannelInfo(m_DomChannel))
		{
			return false;
		}
		if (!ReadTransFormInfo(m_DomDev))
		{
			return false;
		}
		if (!ReadAlarmInfo(m_DomData))
		{
			return false;
		}
		break;
	}		
	case _STREAM_XML_TYPE:
	{
		m_XmlReader.setDevice(&file);
		m_XmlReader.readNext();
		while (!m_XmlReader.atEnd())
		{
			if (m_XmlReader.isStartElement())
			{
				if (m_XmlReader.name()=="fes")
				{
					m_pFesInfo->FesId = m_XmlReader.attributes().value("ID").toInt();
					
					strncpy(m_pFesInfo->FesDesc, m_XmlReader.attributes().value("Description").toLocal8Bit().data(), qMin((size_t)MAX_NAME_LENGTH, (size_t)m_XmlReader.attributes().value("Description").toLocal8Bit().length()));
					strncpy(m_pFesInfo->FesTagName, m_XmlReader.attributes().value("TagName").toString().toStdString().c_str(), qMin((size_t)MAX_NAME_LENGTH, (size_t)m_XmlReader.attributes().value("TagName").toString().length()));
					strncpy(m_pFesInfo->FesVersion, m_XmlReader.attributes().value("version").toString().toStdString().c_str(), qMin((size_t)MAX_NAME_LENGTH, (size_t)m_XmlReader.attributes().value("version").toString().length()));
					strncpy(m_pFesInfo->FesExt, m_XmlReader.attributes().value("Extention").toString().toStdString().c_str(), qMin((size_t)MAX_NAME_LENGTH, (size_t)m_XmlReader.attributes().value("Extention").toString().length()));
					strncpy(m_pFesInfo->FesGrpName, m_XmlReader.attributes().value("GroupName").toLocal8Bit().data(), qMin((size_t)MAX_NAME_LENGTH, (size_t)m_XmlReader.attributes().value("GroupName").toLocal8Bit().length()));

					ReadFesInfo();
				}
				else
				{
					m_XmlReader.raiseError("not valid tagname");
				}
			}
			else
			{
				m_XmlReader.readNext();
			}
		}
		break;
	}		
	default:
		break;
	}
	
	file.close();

	//排序
	std::sort(m_arrTempNodes.begin(), m_arrTempNodes.end(), SortFuction<NODE_PTR>);
	std::sort(m_arrTempChannels.begin(), m_arrTempChannels.end(), SortFuction<CHANNEL_PTR>);
	std::sort(m_arrTempDevices.begin(), m_arrTempDevices.end(), SortFuction<DEVICE_PTR>);
	std::sort(m_arrTempAins.begin(), m_arrTempAins.end(), SortFuction<AIN_PTR>);
	std::sort(m_arrTempDins.begin(), m_arrTempDins.end(), SortFuction<DIN_PTR>);
	std::sort(m_arrTempAouts.begin(), m_arrTempAouts.end(), SortFuction<AOUT_PTR>);
	std::sort(m_arrTempDouts.begin(), m_arrTempDouts.end(), SortFuction<DOUT_PTR>);

	
	m_pChannelMgr->SortMgrFunction();

	return true;
}

bool CDbSvc::ReadChannelInfo(QDomElement eleCHannel)
{
	Q_ASSERT(eleCHannel.isNull() == false);
	if (eleCHannel.isNull())
	{
		return false;
	}

	QDomNode domNode = eleCHannel.firstChild();
	CHANNEL_PTR pChannel = Q_NULLPTR;

	for (; !domNode.isNull(); domNode = domNode.nextSibling())
	{
		QDomElement pData = domNode.toElement();

		if (pData.tagName() == "c")
		{
			pChannel = std::make_shared<CHANNEL>();

			//读取文件
			CChannelPtr ppChannel = m_pChannelMgr->CreateChannel(pChannel);

			pChannel->OccNo = pData.attribute("OccNo").toInt();
			strncpy(pChannel->ChannelName,
				pData.attribute("ChannelName").toStdString().c_str(),
				qMin((size_t)(MAX_NAME_LENGTH), pData.attribute("ChannelName").toStdString().length()));
			strncpy(pChannel->DriverName,
				pData.attribute("DriverName").toStdString().c_str(),
				qMin((size_t)(MAX_NAME_LENGTH), pData.attribute("DriverName").toStdString().length()));

			QDomNode ChildNode = pData.firstChild();

			DEVICE_PTR pDevice;
			for (; !ChildNode.isNull(); ChildNode = ChildNode.nextSibling())
			{
				QDomElement pChildDom = ChildNode.toElement();

				if (pChildDom.tagName() == "device")
				{
					QDomNode pNodeTempDev = pChildDom.firstChild();

					for (; !pNodeTempDev.isNull(); pNodeTempDev = pNodeTempDev.nextSibling())
					{
						QDomElement pEleDev = pNodeTempDev.toElement();

						if (pEleDev.tagName() == "d")
						{
							pDevice = std::make_shared<DEVICE>();

							CDevicePtr ppDevice = m_pChannelMgr->CreateDevice(ppChannel, pDevice);

							pDevice->OccNo = pData.attribute("OccNo").toInt();
							pDevice->NameOccNo = pData.attribute("OccNo").toInt();
							pDevice->ModelOccNo = pData.attribute("ModelOccNo").toInt();
							pDevice->ChannelOccNo = pData.attribute("OccNo").toInt();

							QDomElement DataELe = pEleDev.firstChild().toElement();

							if (DataELe.tagName() == "data")
							{
								QDomNode pGrangChild = DataELe.firstChild();

								for (; !pGrangChild.isNull(); pGrangChild = pGrangChild.nextSibling())
								{
									QDomElement pGrangDom = pGrangChild.toElement();
									if (pGrangDom.tagName() == "AIN")
									{
										ReadAinInfo(pGrangDom, ppDevice,ppChannel);
									}
									if (pGrangDom.tagName() == "DIN")
									{
										ReadDinInfo(pGrangDom, ppDevice, ppChannel);
									}
									if (pGrangDom.tagName() == "AOUT")
									{
										ReadAoutInfo(pGrangDom, ppDevice, ppChannel);
									}
									if (pGrangDom.tagName() == "DOUT")
									{
										ReadDoutInfo(pGrangDom, ppDevice, ppChannel);
									}
								}
							}
							m_arrTempDevices.push_back(pDevice);
						}
					}
				}
			}
		}
		m_arrTempChannels.push_back(pChannel);
	}
	return true;
}

bool CDbSvc::ReadTransFormInfo(QDomElement ellTransForm)
{
	Q_ASSERT(ellTransForm.isNull() == false);
	if (ellTransForm.isNull())
	{
		return false;
	}

	QDomNode domNode = ellTransForm.firstChild();

	for (; !domNode.isNull(); domNode = domNode.nextSibling())
	{
		QDomElement pData = domNode.toElement();

		if (pData.tagName() == "linear")
		{
			ReadLinearInfo(pData);
		}
		if (pData.tagName() == "nonlinear")
		{
			ReadNonLinearInfo(pData);
		}
	}
	return true;
}

bool CDbSvc::ReadAlarmInfo(QDomElement eleData)
{
	Q_ASSERT(eleData.isNull() == false);

	if (eleData.isNull())
	{
		return false;
	}

	QDomNode doNode = eleData.firstChild();

	for (; !doNode.isNull(); doNode = doNode.nextSibling())
	{
		QDomElement pData = doNode.toElement();
		if (pData.tagName() == "ain_alarm")
		{
			ReadAinAlarmInfo(pData);
		}
		if (pData.tagName() == "din_alarm")
		{
			ReadDinAlarmInfo(pData);
		}
	}
	return true;
}

bool CDbSvc::ReadAinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(eleData.isNull() == false);
	if (eleData.isNull())
	{
		return false;
	}

	AIN_PTR pAin;

	QDomNode pNode = eleData.firstChild();

	for (; !pNode.isNull(); pNode = pNode.nextSibling())
	{
		QDomElement ainDomEle = pNode.toElement();
		if (ainDomEle.tagName() == "ai")
		{
			pAin = std::make_shared<AIN>();
			pAin->OccNo = ainDomEle.attribute("OccNo").toInt();
			pAin->BlockNo = ainDomEle.attribute("BlockOccNo").toInt();
			//TODO
			pAin->NameOccNo = ainDomEle.attribute("").toInt();
			pAin->ChannelOccNo = ainDomEle.attribute("ChannelOccNo").toInt();
			pAin->DeviceOccNo = ainDomEle.attribute("DeviceOccNo").toInt();
			strncpy(pAin->Address,
				ainDomEle.attribute("Address").toStdString().c_str(),
				qMin((size_t)MAX_ADDRESS_LENGTH, ainDomEle.attribute("Address").toStdString().length()));
			pAin->InitValue = ainDomEle.attribute("InitValue").toFloat();
			pAin->DataSource = ainDomEle.attribute("DataSource").toInt();
			pAin->TransformType = ainDomEle.attribute("TransformType").toInt();
			pAin->RangeL = ainDomEle.attribute("RangeL").toDouble();
			pAin->RangeH = ainDomEle.attribute("RangeH").toDouble();

			pAin->HighQua = ainDomEle.attribute("HighQua").toDouble();
			pAin->LowQua = ainDomEle.attribute("LowQua").toDouble();

			m_arrTempAins.push_back(pAin);

			pChannel->AddAin(pAin);

			pDev->AddAin(pAin);
		}
	}

	return true;
}

bool CDbSvc::ReadDinInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(eleData.isNull() == false);
	if (eleData.isNull())
	{
		return false;
	}

	DIN_PTR pDin = Q_NULLPTR;

	QDomNode pDinNode = eleData.firstChild();

	for (; !pDinNode.isNull(); pDinNode = pDinNode.nextSibling())
	{
		QDomElement pDinEle = pDinNode.toElement();
		if (pDinEle.tagName() == "di")
		{
			pDin = std::make_shared<DIN>();
			pDin->OccNo = pDinEle.attribute("OccNo").toInt();
			pDin->ChannelOccNo = pDinEle.attribute("ChannelOccNo").toInt();
			pDin->DeviceOccNo = pDinEle.attribute("DeviceOccNo").toInt();
			pDin->BlockNo = pDinEle.attribute("BlockOccNo").toInt();
			strncpy(pDin->Address,
				pDinEle.attribute("Address").toStdString().c_str(),
				qMin((size_t)MAX_ADDRESS_LENGTH, pDinEle.attribute("Address").toStdString().length()));
			pDin->Init = pDinEle.attribute("InitValue").toFloat();
			pDin->DataSource = pDinEle.attribute("DataSource").toInt();

			m_arrTempDins.push_back(pDin);

			pChannel->AddDin(pDin);

			pDev->AddDin(pDin);
		}
	}

	return true;
}

bool CDbSvc::ReadAoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(eleData.isNull() == false);
	if (eleData.isNull())
	{
		return false;
	}
	AOUT_PTR pAout;

	QDomNode pNode = eleData.firstChild();

	for (; !pNode.isNull(); pNode = pNode.nextSibling())
	{
		QDomElement ainDomEle = pNode.toElement();
		if (ainDomEle.tagName() == "ao")
		{
			pAout = std::make_shared<AOUT>();
			pAout->OccNo = ainDomEle.attribute("OccNo").toInt();
			pAout->BlockNo = ainDomEle.attribute("BlockOccNo").toInt();
			//TODO
			pAout->NameOccNo = ainDomEle.attribute("").toInt();
			pAout->ChannelOccNo = ainDomEle.attribute("ChannelOccNo").toInt();
			pAout->DeviceOccNo = ainDomEle.attribute("DeviceOccNo").toInt();

			pAout->DataSource = ainDomEle.attribute("DataSource").toInt();
			pAout->TransformType = ainDomEle.attribute("TransformType").toInt();

			m_arrTempAouts.push_back(pAout);

			pChannel->AddAout(pAout);

			pDev->AddAout(pAout);
		}
	}

	return true;
}

bool CDbSvc::ReadDoutInfo(QDomElement eleData, CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(eleData.isNull() == false);
	if (eleData.isNull())
	{
		return false;
	}
	DOUT_PTR pDout = Q_NULLPTR;
	QDomNode pDinNode = eleData.firstChild();

	for (; !pDinNode.isNull(); pDinNode = pDinNode.nextSibling())
	{
		QDomElement pDinEle = pDinNode.toElement();
		if (pDinEle.tagName() == "do")
		{
			pDout = std::make_shared<DOUT>();
			pDout->OccNo = pDinEle.attribute("OccNo").toInt();
			pDout->ChannelOccNo = pDinEle.attribute("ChannelOccNo").toInt();
			pDout->DeviceOccNo = pDinEle.attribute("DeviceOccNo").toInt();
			pDout->BlockNo = pDinEle.attribute("BlockOccNo").toInt();
			pDout->Init = pDinEle.attribute("InitValue").toFloat();
			pDout->DataSource = pDinEle.attribute("DataSource").toInt();

			m_arrTempDouts.push_back(pDout);

			pChannel->AddDout(pDout);

			pDev->AddDout(pDout);
		}
	}
	return true;
}

bool CDbSvc::ReadLinearInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}
	QDomNode nodeChild = eleData.firstChild();

	TRANSFORM_LINEAR_DEF pTransLinear = Q_NULLPTR;

	for (; !nodeChild.isNull(); nodeChild = nodeChild.nextSibling())
	{
		QDomElement eleLinear = nodeChild.toElement();
		if (eleLinear.tagName() == "l")
		{
			pTransLinear = std::make_shared<TRANSFORM_LINEAR>();
			pTransLinear->OccNo = eleLinear.attribute("OccNo").toInt();
			pTransLinear->MaxRaw = eleLinear.attribute("MaxRaw").toDouble();
			pTransLinear->MinRaw = eleLinear.attribute("MinRaw").toDouble();
			pTransLinear->MaxScale = eleLinear.attribute("MaxScale").toDouble();
			pTransLinear->MinScale = eleLinear.attribute("MinScale").toDouble();
			pTransLinear->InverseFactor = eleLinear.attribute("Inverse").toInt();
			pTransLinear->EnableFactor = eleLinear.attribute("EnaleFactor").toInt();
			pTransLinear->Gain = eleLinear.attribute("Gain").toDouble();
			pTransLinear->Offset = eleLinear.attribute("Offset").toDouble();

			m_arrTempTransFormLinears.push_back(pTransLinear);
		}
	}
	return true;
}

bool CDbSvc::ReadNonLinearInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}
	QDomNode nodeChild = eleData.firstChild();

	TRANSFORM_NONLINEAR_DEF pTransFormNonLinear = Q_NULLPTR;

	for (; !nodeChild.isNull(); nodeChild = nodeChild.nextSibling())
	{
		QDomElement eleTransNonLinear = nodeChild.toElement();

		if (eleTransNonLinear.tagName() == "nl")
		{
			pTransFormNonLinear = std::make_shared<TRANSFORM_NONLINEAR>();
			pTransFormNonLinear->OccNo = eleTransNonLinear.attribute("OccNo").toInt();
			pTransFormNonLinear->DataCount = eleTransNonLinear.attribute("DataCount").toInt();

			QDomNode grandChildNode = eleTransNonLinear.firstChild();
			int nIndex = 0;
			for (; !grandChildNode.isNull(); grandChildNode = grandChildNode.nextSibling())
			{
				QDomElement pVal = grandChildNode.toElement();
				if (pVal.tagName() == "p")
				{
					pTransFormNonLinear->NonLinearData[nIndex].RawValue = pVal.attribute("RawValue").toDouble();
					pTransFormNonLinear->NonLinearData[nIndex].ScaleValue = pVal.attribute("ScaleValue").toDouble();
					nIndex++;
				}
			}
			m_arrTempTransFormNonLinears.push_back(pTransFormNonLinear);
		}
	}
	return true;
}

bool CDbSvc::ReadAinAlarmInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}

	QDomNode nodeChild = eleData.firstChild();

	for (; !nodeChild.isNull(); nodeChild = nodeChild.nextSibling())
	{
		QDomElement eleAinAlarm = nodeChild.toElement();
		if (eleAinAlarm.tagName() == "ain_alarm_offlimit")
		{
			ReadAinOfflimitInfo(eleAinAlarm);
		}
	}
	return true;
}

bool CDbSvc::ReadDinAlarmInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}
	QDomNode nodeChild = eleData.firstChild();

	for (; !nodeChild.isNull(); nodeChild = nodeChild.nextSibling())
	{
		QDomElement eleDinAlarm = nodeChild.toElement();
		if (eleDinAlarm.tagName() == "din_alarm_offlimit")
		{
			ReadDinOfflimitInfo(eleDinAlarm);
		}
	}
	return true;
}

bool CDbSvc::ReadAinOfflimitInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}
	QDomNode childNode = eleData.firstChild();

	AIN_ALARM_DEF pAlarmLimit = Q_NULLPTR;

	for (; !childNode.isNull(); childNode = childNode.nextSibling())
	{
		QDomElement eleAinOfflimit = childNode.toElement();
		if (eleAinOfflimit.tagName() == "aa")
		{
			pAlarmLimit = std::make_shared<AIN_ALARM>();
			pAlarmLimit->OccNo = eleAinOfflimit.attribute("OccNo").toInt();

			m_arrTempAinAlarms.push_back(pAlarmLimit);
		}
	}
	return true;
}

bool CDbSvc::ReadDinOfflimitInfo(QDomElement eleData)
{
	if (eleData.isNull())
	{
		return false;
	}
	QDomNode childNode = eleData.firstChild();

	DIN_ALARM_DEF pDinAlarmLimit = Q_NULLPTR;

	for (; !childNode.isNull(); childNode = childNode.nextSibling())
	{
		QDomElement eleAinOfflimit = childNode.toElement();
		if (eleAinOfflimit.tagName() == "da")
		{
			pDinAlarmLimit = std::make_shared<DIN_ALARM>();
			pDinAlarmLimit->OccNo = eleAinOfflimit.attribute("OccNo").toInt();

			m_arrTempDinAlarms.push_back(pDinAlarmLimit);
		}
	}
	return true;
}
/*! \fn bool CDbSvc::ReadChannelInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadChannelInfo
** \details 读取通道信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadChannelInfo()
{
	CHANNEL_PTR pChannel = Q_NULLPTR;

	TAG_OCCNO_DEF pChannelOccNo = Q_NULLPTR;
	int nIndex = 0;
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			if (nIndex<=0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}		
		}
		if (m_XmlReader.isStartElement())
		{
			qDebug() << m_XmlReader.name();
			
			if (m_XmlReader.name()=="group")
			{
				nIndex++;
				m_XmlReader.readNext();
			}
			else if (m_XmlReader.name() == "c")
			{
				pChannel = std::make_shared<CHANNEL>();

				pChannelOccNo = std::make_shared<TAG_OCCNO>();
				//读取文件
				CChannelPtr ppChannel = m_pChannelMgr->CreateChannel(pChannel);

				pChannel->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pChannelOccNo->OccNo = pChannel->OccNo;
				std::memset(pChannelOccNo->TagName, 0, sizeof(pChannelOccNo->TagName));

				strncpy(pChannelOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				strncpy(pChannel->ChannelName,
					m_XmlReader.attributes().value("ChannelName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("ChannelName").length()));
				strncpy(pChannel->DriverName,
					m_XmlReader.attributes().value("DriverName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("DriverName").length()));
				
				pChannel->NodeOccNo = m_nMyNodeOccNo;

				pChannel->ScanEnable = SCAN_IN;

				pChannel->Init = INITED;

				m_arrTempChannels.push_back(pChannel);

				m_arrChannelOccNos.push_back(pChannelOccNo);

				ReadDev(ppChannel);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadTransFormInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadTransFormInfo
** \details 读取转换信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadTransFormInfo()
{

}
/*! \fn bool CDbSvc::ReadAlarmInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadAlarmInfo
** \details 读取告警信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadAlarmInfo()
{

}
/*! \fn bool CDbSvc::ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel)
*********************************************************************************************************
** \brief CDbSvc::ReadAinInfo
** \details 读取AIN信息
** \param pDev
** \param pChannel
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	AIN_PTR pAin = Q_NULLPTR;

	TAG_OCCNO_DEF pAinOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "ai")
			{
				pAin = std::make_shared<AIN>();

				pAinOccNo = std::make_shared<TAG_OCCNO>();

				pAin->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pAinOccNo->OccNo = pAin->OccNo;
				std::memset(pAinOccNo->TagName, 0, sizeof(pAinOccNo->TagName));

				strncpy(pAinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				pAin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				//TODO
				pAin->NameOccNo = m_XmlReader.attributes().value("NameOccNo").toInt();
				pAin->ChannelOccNo = pChannel->GetOccNo();
				pAin->DeviceOccNo = pDev->GetOccNo();
				//pAin->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
				strncpy(pAin->Address,
					m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
					qMin((size_t)MAX_ADDRESS_LENGTH, (size_t)m_XmlReader.attributes().value("Address").length()));
				pAin->InitValue = m_XmlReader.attributes().value("InitValue").toFloat();
				pAin->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				pAin->TransformType = m_XmlReader.attributes().value("TransformType").toInt();
				pAin->RangeL = m_XmlReader.attributes().value("RangeL").toDouble();
				pAin->RangeH = m_XmlReader.attributes().value("RangeH").toDouble();
				pAin->AlarmOccNo = m_XmlReader.attributes().value("AlarmOccNo").toInt();
				pAin->HighQua = m_XmlReader.attributes().value("HighQua").toDouble();
				pAin->LowQua = m_XmlReader.attributes().value("LowQua").toDouble();
				pAin->MaxRaw = m_XmlReader.attributes().value("MaxRaw").toDouble();
				pAin->MinRaw = m_XmlReader.attributes().value("MinRaw").toDouble();
				pAin->MaxScale = m_XmlReader.attributes().value("MaxConvert").toDouble();
				pAin->MinScale = m_XmlReader.attributes().value("MinConvert").toDouble();
				pAin->NodeOccNo = m_nMyNodeOccNo;
				pAin->ScanEnable = SCAN_IN;
				pAin->Init = INITED;
				m_arrTempAins.push_back(pAin);

				pChannel->AddAin(pAin);

				pDev->AddAin(pAin);

				m_arrAinOccNos.push_back(pAinOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel)
*********************************************************************************************************
** \brief CDbSvc::ReadDinInfo
** \details 读取DIN信息
** \param pDev
** \param pChannel
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	DIN_PTR pDin = Q_NULLPTR;
	TAG_OCCNO_DEF pDinOccNo=Q_NULLPTR;
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "di")
			{
				pDin = std::make_shared<DIN>();

				pDin->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pDinOccNo = std::make_shared<TAG_OCCNO>();

				pDinOccNo->OccNo = pDin->OccNo;
				std::memset(pDinOccNo->TagName, 0, sizeof(pDinOccNo->TagName));

				strncpy(pDinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				
				//pDin->ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
				//pDin->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
				pDin->ChannelOccNo = pChannel->GetOccNo();
				pDin->DeviceOccNo = pDev->GetOccNo();

				pDin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				strncpy(pDin->Address,
					m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
					qMin((size_t)MAX_ADDRESS_LENGTH, (size_t)m_XmlReader.attributes().value("Address").length()));
				pDin->Init = m_XmlReader.attributes().value("InitValue").toFloat();
				pDin->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				pDin->AlarmOccNo = m_XmlReader.attributes().value("AlarmOccNo").toInt();
				pDin->NodeOccNo = m_nMyNodeOccNo;
				pDin->ScanEnable = SCAN_IN;
				pDin->Init = INITED;
				m_arrTempDins.push_back(pDin);

				pChannel->AddDin(pDin);

				pDev->AddDin(pDin);

				m_arrDinOccNos.push_back(pDinOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
*********************************************************************************************************
** \brief CDbSvc::ReadAoutInfo
** \details 读取AOUT信息
** \param pDev
** \param pChannel
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	AOUT_PTR pAout = Q_NULLPTR;
	TAG_OCCNO_DEF pAoutOccNo = Q_NULLPTR;
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "ao")
			{
				pAout = std::make_shared<AOUT>();

				pAout->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pAoutOccNo = std::make_shared<TAG_OCCNO>();

				pAoutOccNo->OccNo = pAout->OccNo;
				std::memset(pAoutOccNo->TagName, 0, sizeof(pAoutOccNo->TagName));

				strncpy(pAoutOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));
				
				pAout->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				//TODO
				pAout->NameOccNo = m_XmlReader.attributes().value("").toInt();
			//	pAout->ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
			//	pAout->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();

				pAout->ChannelOccNo = pChannel->GetOccNo();
				pAout->DeviceOccNo = pDev->GetOccNo();

				pAout->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				pAout->TransformType = m_XmlReader.attributes().value("TransformType").toInt();


				pAout->RangeL = m_XmlReader.attributes().value("RangeL").toDouble();
				pAout->RangeH = m_XmlReader.attributes().value("RangeH").toDouble();
				pAout->AlarmOccNo = m_XmlReader.attributes().value("AlarmOccNo").toInt();
				pAout->HighQua = m_XmlReader.attributes().value("HighQua").toDouble();
				pAout->LowQua = m_XmlReader.attributes().value("LowQua").toDouble();
				pAout->MaxRaw = m_XmlReader.attributes().value("MaxRaw").toDouble();
				pAout->MinRaw = m_XmlReader.attributes().value("MinRaw").toDouble();
				pAout->MaxScale = m_XmlReader.attributes().value("MaxConvert").toDouble();
				pAout->MinScale = m_XmlReader.attributes().value("MinConvert").toDouble();
				pAout->NodeOccNo = m_nMyNodeOccNo;
				//pAout->ScanEnable = SCAN_IN;
				//pAout->Init = INITED;

				pAout->NodeOccNo = m_nMyNodeOccNo;
				pAout->ScanEnable = SCAN_IN;
				memset(pAout->Address, 0, MAX_ADDRESS_LENGTH + STRING_PAD);
				strncpy(pAout->Address,
					m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
					qMin(size_t(MAX_ADDRESS_LENGTH + STRING_PAD), size_t(m_XmlReader.attributes().value("Address").length())));
				pAout->Init = INITED;
				m_arrTempAouts.push_back(pAout);

				pChannel->AddAout(pAout);

				pDev->AddAout(pAout);

				m_arrAoutOccNos.push_back(pAoutOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
*********************************************************************************************************
** \brief CDbSvc::ReadDoutInfo
** \details 读取DOUT信息
** \param pDev
** \param pChannel
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	DOUT_PTR pDout = Q_NULLPTR;
	TAG_OCCNO_DEF pDoutOccNo = Q_NULLPTR;

	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "do")
			{
				pDout = std::make_shared<DOUT>();

				pDout->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pDoutOccNo = std::make_shared<TAG_OCCNO>();

				pDoutOccNo->OccNo = pDout->OccNo;
				std::memset(pDoutOccNo->TagName, 0, sizeof(pDoutOccNo->TagName));
				strncpy(pDoutOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				
				//pDout->ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
				//pDout->DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();

				pDout->ChannelOccNo = pChannel->GetOccNo();
				pDout->DeviceOccNo = pDev->GetOccNo();

				pDout->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				pDout->Init = m_XmlReader.attributes().value("InitValue").toFloat();
				pDout->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				memset(pDout->Address, 0, MAX_ADDRESS_LENGTH + STRING_PAD);
				strncpy(pDout->Address, 
					m_XmlReader.attributes().value("Address").toString().toStdString().c_str(), 
					qMin( size_t (MAX_ADDRESS_LENGTH + STRING_PAD),size_t( m_XmlReader.attributes().value("Address").length())));
				pDout->NodeOccNo = m_nMyNodeOccNo;
				pDout->ScanEnable = SCAN_IN;
				pDout->Init = INITED;
				pDout->CheckOK = true;
				pDout->StartProgramOccNo = 1;
				m_arrTempDouts.push_back(pDout);

				pChannel->AddDout(pDout);

				pDev->AddDout(pDout);

				m_arrDoutOccNos.push_back(pDoutOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadLinearInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadLinearInfo
** \details 读取线性转换
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadLinearInfo()
{
	TRANSFORM_LINEAR_DEF pTransLinear = Q_NULLPTR;

	TAG_OCCNO_DEF pLinearOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	int nIndex = 0;

	while (!m_XmlReader.atEnd())
	{
		qDebug() << m_XmlReader.name();

		if (m_XmlReader.isEndElement())
		{
			if (nIndex<=0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}			
		}
		if (m_XmlReader.isStartElement())
		{			
			if (m_XmlReader.name()=="group")
			{		
				nIndex++;
				m_XmlReader.readNext();
			}
			else if (m_XmlReader.name() == "l")
			{
				pTransLinear = std::make_shared<TRANSFORM_LINEAR>();

				pTransLinear->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pLinearOccNo = std::make_shared<TAG_OCCNO>();

				pLinearOccNo->OccNo = pTransLinear->OccNo;
				std::memset(pLinearOccNo->TagName, 0, sizeof(pLinearOccNo->TagName));

				strncpy(pLinearOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				
				pTransLinear->MaxRaw = m_XmlReader.attributes().value("MaxRaw").toDouble();
				pTransLinear->MinRaw = m_XmlReader.attributes().value("MinRaw").toDouble();
				pTransLinear->MaxScale = m_XmlReader.attributes().value("MaxScale").toDouble();
				pTransLinear->MinScale = m_XmlReader.attributes().value("MinScale").toDouble();
				pTransLinear->InverseFactor = m_XmlReader.attributes().value("Inverse").toInt();
				pTransLinear->EnableFactor = m_XmlReader.attributes().value("EnaleFactor").toInt();
				pTransLinear->Gain = m_XmlReader.attributes().value("Gain").toDouble();
				pTransLinear->Offset = m_XmlReader.attributes().value("Offset").toDouble();

				m_arrTempTransFormLinears.push_back(pTransLinear);

				m_arrTransLinearOccNos.push_back(pLinearOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadNonLinearInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadNonLinearInfo
** \details 读取非线性转换
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadNonLinearInfo()
{
	TRANSFORM_NONLINEAR_DEF pTransFormNonLinear = Q_NULLPTR;

	TAG_OCCNO_DEF pNonLinearOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	int nIndex = 0;

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			if (nIndex<=0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}			
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "group")
			{
				nIndex++;
				m_XmlReader.readNext();
			}
			else if (m_XmlReader.name() == "nl")
			{
				pTransFormNonLinear = std::make_shared<TRANSFORM_NONLINEAR>();

				pTransFormNonLinear->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pNonLinearOccNo = std::make_shared<TAG_OCCNO>();

				pNonLinearOccNo->OccNo = pTransFormNonLinear->OccNo;
				std::memset(pNonLinearOccNo->TagName, 0, sizeof(pNonLinearOccNo->TagName));
				strncpy(pNonLinearOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				
				pTransFormNonLinear->DataCount = m_XmlReader.attributes().value("DataCount").toInt();

				ReadNonLinearVal(pTransFormNonLinear);

				m_arrTempTransFormNonLinears.push_back(pTransFormNonLinear);

				m_arrTransNonLinearOccNos.push_back(pNonLinearOccNo);

			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadAinAlarmInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadAinAlarmInfo
** \details 读取AIN告警信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadAinAlarmInfo()
{
	AIN_ALARM_DEF pAlarmDef = Q_NULLPTR;

	TAG_OCCNO_DEF pAinAlarmOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	int nIndex = 0;

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			if (nIndex<=0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}			
		}
		if (m_XmlReader.isStartElement())
		{			
			if (m_XmlReader.name()=="group")
			{
				nIndex++;
				m_XmlReader.readNext();
			}
			else if (m_XmlReader.name() == "ain_alarm_node")
			{
				pAlarmDef = std::make_shared<AIN_ALARM>();

				pAlarmDef->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pAinAlarmOccNo = std::make_shared<TAG_OCCNO>();

				pAinAlarmOccNo->OccNo = pAlarmDef->OccNo;
				std::memset(pAinAlarmOccNo->TagName, 0, sizeof(pAinAlarmOccNo->TagName));
				strncpy(pAinAlarmOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				pAlarmDef->OnQualityGood = m_XmlReader.attributes().value("OnQualityGood").toInt();
				pAlarmDef->DeadBandType = m_XmlReader.attributes().value("DeadType").toInt();
				pAlarmDef->DeadBand = m_XmlReader.attributes().value("DeadArea").toDouble();
				
				m_arrTempAinAlarms.push_back(pAlarmDef);

				m_arrAinAlarmOccNos.push_back(pAinAlarmOccNo);

				ReadAinOfflimitInfo(pAlarmDef);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDinAlarmInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadDinAlarmInfo
** \details 读取DIN告警信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDinAlarmInfo()
{
	DIN_ALARM_DEF pAlarmDef = Q_NULLPTR;

	TAG_OCCNO_DEF pAlarmOccNo = Q_NULLPTR;
	m_XmlReader.readNext();

	int nIndex = 0;

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			if (nIndex<=0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}			
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "group")
			{
				nIndex++;
				m_XmlReader.readNext();
			}
			else if (m_XmlReader.name() == "din_alarm_node")
			{
				pAlarmDef = std::make_shared<DIN_ALARM>();

				pAlarmOccNo = std::make_shared<TAG_OCCNO>();

				
				std::memset(pAlarmOccNo->TagName, 0, sizeof(pAlarmOccNo->TagName));

				strncpy(pAlarmOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				pAlarmDef->OccNo = m_XmlReader.attributes().value("OccNo").toInt();
				pAlarmOccNo->OccNo = pAlarmDef->OccNo;
				
				pAlarmDef->OnQualityGood = m_XmlReader.attributes().value("OnQualityGood").toInt();
				
				m_arrTempDinAlarms.push_back(pAlarmDef);

				m_arrDinAlarmOccNos.push_back(pAlarmOccNo);

				ReadDinOfflimitInfo(pAlarmDef);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadAinOfflimitInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadAinOfflimitInfo
** \details 读取AIN越线信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm)
{
	AIN_ALARM_LIMIT_DEF pAlarmLimit = Q_NULLPTR;

	TAG_OCCNO_DEF pAlarmLimitOccNo=Q_NULLPTR;

	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "aa")
			{
				pAlarmLimit = std::make_shared<AIN_ALARM_LIMIT>();
				pAlarmLimit->OccNo = m_XmlReader.attributes().value("OccNo").toInt();


				pAlarmLimitOccNo = std::make_shared<TAG_OCCNO>();

				pAlarmLimitOccNo->OccNo = pAlarmLimit->OccNo;
				pAlarmLimit->AlarmOccNo = pAinAlarm->OccNo;
				std::memset(pAlarmLimitOccNo->TagName, 0, sizeof(pAlarmLimitOccNo->TagName));

				strncpy(pAlarmLimitOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));
				pAlarmLimit->BlockNo = m_XmlReader.attributes().value("BlockNo").toInt();
				pAlarmLimit->Condition = m_XmlReader.attributes().value("Condition").toInt();
				pAlarmLimit->Category = m_XmlReader.attributes().value("Category").toInt();
				pAlarmLimit->Priority = m_XmlReader.attributes().value("Priority").toInt();
				pAlarmLimit->DelayAlarm = m_XmlReader.attributes().value("DelayAlarm").toInt();
				pAlarmLimit->EnableAck = m_XmlReader.attributes().value("SupportAck").toUInt();
				pAlarmLimit->EnableDelete = m_XmlReader.attributes().value("SupportDelete").toInt();
				pAlarmLimit->AckType = m_XmlReader.attributes().value("AckType").toInt();

				strncpy(pAlarmLimit->SoundFile, 
						m_XmlReader.attributes().value("SoundFile").toString().toStdString().c_str(), 
						qMin(512, m_XmlReader.attributes().value("SoundFile").length()));
				pAlarmLimit->PlaySoundTimes = m_XmlReader.attributes().value("PlaySoundTimes").toInt();

				pAlarmLimit->SpeechAlarmText = m_XmlReader.attributes().value("SpeechAlarmText").toInt();

				pAlarmLimit->Beep = m_XmlReader.attributes().value("Beep").toInt();

				strncpy(pAlarmLimit->PushGraphName,
					m_XmlReader.attributes().value("PushGraph").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("PushGraph").length()));

				pAlarmLimit->BlinkGraphObject = m_XmlReader.attributes().value("BlinkGraph").toInt();

				pAlarmLimit->LogToAlarmWIndow = m_XmlReader.attributes().value("Log").toInt();

				pAlarmLimit->Color.BackColor = m_XmlReader.attributes().value("BackColor").toLong();

				pAlarmLimit->Color.BlinkBackColor = m_XmlReader.attributes().value("BlinkBackColor").toLong();

				pAlarmLimit->Color.BlinkTextColor = m_XmlReader.attributes().value("BlinkTextColor").toLong();

				pAlarmLimit->Color.TextColor = m_XmlReader.attributes().value("TxtColor").toLong();

				pAlarmLimit->LightBoard = m_XmlReader.attributes().value("LightBoard").toInt();

				strncpy(pAlarmLimit->DispGuide,
					m_XmlReader.attributes().value("DispGuide").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("DispGuide").length()));

				strncpy(pAlarmLimit->Commands,
					m_XmlReader.attributes().value("Commands").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("Commands").length()));

				pAlarmLimit->LowValue = m_XmlReader.attributes().value("LowValue").toDouble();

				pAlarmLimit->LimitValue = m_XmlReader.attributes().value("Value").toDouble();

				pAlarmLimit->ScanEnable = SCAN_IN;

				m_arrTempAinLimitAlarms.push_back(pAlarmLimit);

				m_arrAinAlarmLimitOccNos.push_back(pAlarmLimitOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDinOfflimitInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadDinOfflimitInfo
** \details 读取DIN越线信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm)
{
	DIN_ALARM_LIMIT_DEF pAlarmLimit = Q_NULLPTR;
	TAG_OCCNO_DEF pAlarmLimitOccNo = Q_NULLPTR;
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "da")
			{
				pAlarmLimit = std::make_shared<DIN_ALARM_LIMIT>();
				pAlarmLimit->OccNo = m_XmlReader.attributes().value("OccNo").toInt();
				pAlarmLimit->AlarmOccNo = pDinAlarm->OccNo;
				pAlarmLimitOccNo = std::make_shared<TAG_OCCNO>();

				pAlarmLimitOccNo->OccNo = pAlarmLimit->OccNo;
				std::memset(pAlarmLimitOccNo->TagName, 0, sizeof(pAlarmLimitOccNo->TagName));

				strncpy(pAlarmLimitOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));
				pAlarmLimit->BlockNo= m_XmlReader.attributes().value("BlockNo").toInt();
				pAlarmLimit->Condition = m_XmlReader.attributes().value("Condition").toInt();
				pAlarmLimit->Category = m_XmlReader.attributes().value("Category").toInt();
				pAlarmLimit->Priority = m_XmlReader.attributes().value("Priority").toInt();
				pAlarmLimit->DelayAlarm = m_XmlReader.attributes().value("DelayAlarm").toInt();
				pAlarmLimit->EnableAck = m_XmlReader.attributes().value("SupportAck").toUInt();
				pAlarmLimit->EnableDelete = m_XmlReader.attributes().value("SupportDelete").toInt();
				pAlarmLimit->AckType = m_XmlReader.attributes().value("AckType").toInt();

				strncpy(pAlarmLimit->SoundFile,
					m_XmlReader.attributes().value("SoundFile").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("SoundFile").length()));
				pAlarmLimit->PlaySoundTimes = m_XmlReader.attributes().value("PlaySoundTimes").toInt();

				pAlarmLimit->SpeechAlarmText = m_XmlReader.attributes().value("SpeechAlarmText").toInt();

				pAlarmLimit->Beep = m_XmlReader.attributes().value("Beep").toInt();

				strncpy(pAlarmLimit->PushGraphName,
					m_XmlReader.attributes().value("PushGraph").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("PushGraph").length()));

				pAlarmLimit->BlinkGraphObject = m_XmlReader.attributes().value("BlinkGraph").toInt();

				pAlarmLimit->LogToAlarmWIndow = m_XmlReader.attributes().value("Log").toInt();

				pAlarmLimit->Color.BackColor = m_XmlReader.attributes().value("BackColor").toLong();

				pAlarmLimit->Color.BlinkBackColor = m_XmlReader.attributes().value("BlinkBackColor").toLong();

				pAlarmLimit->Color.BlinkTextColor = m_XmlReader.attributes().value("BlinkTextColor").toLong();

				pAlarmLimit->Color.TextColor = m_XmlReader.attributes().value("TxtColor").toLong();

				pAlarmLimit->LightBoard = m_XmlReader.attributes().value("LightBoard").toInt();

				strncpy(pAlarmLimit->DispGuide,
					m_XmlReader.attributes().value("DispGuide").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("DispGuide").length()));

				strncpy(pAlarmLimit->Commands,
					m_XmlReader.attributes().value("Commands").toString().toStdString().c_str(),
					qMin(512, m_XmlReader.attributes().value("Commands").length()));
				
				pAlarmLimit->ScanEnable = SCAN_IN;

				m_arrTempDinLimitAlarms.push_back(pAlarmLimit);

				m_arrDinAlarmLimitOccNos.push_back(pAlarmLimitOccNo);

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadFesInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadFesInfo
** \details 开始读取信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadFesInfo()
{
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "channel")
			{
				ReadChannelInfo();
			}
			else if (m_XmlReader.name()=="scale")
			{
				ReadScale();
			}
			else if (m_XmlReader.name()=="alarm")
			{
				ReadAlarm();
			}
			else if (m_XmlReader.name()=="variable")
			{
				ReadVariable();
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::SkipUnknownElement()
*********************************************************************************************************
** \brief CDbSvc::SkipUnknownElement
** \details 跳出未知图元
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::SkipUnknownElement()
{
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			SkipUnknownElement();
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDevInfo()
*********************************************************************************************************
** \brief CDbSvc::ReadDevInfo
** \details 读取设备信息
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDev(CChannelPtr pChannel)
{
	Q_ASSERT(pChannel);

	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "device")
			{
				ReadDevInfo(pChannel);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn bool CDbSvc::ReadDevInfo(CChannelPtr pChannel)
*********************************************************************************************************
** \brief CDbSvc::ReadDevInfo
** \details
** \param pChannel
** \return bool
** \author xingzhibing
** \date 2016年9月18日
** \note
********************************************************************************************************/
void CDbSvc::ReadDevInfo(CChannelPtr pChannel)
{
	Q_ASSERT(pChannel);

	DEVICE_PTR pDevice=Q_NULLPTR;

	TAG_OCCNO_DEF pDevOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "d")
			{
				pDevice = std::make_shared<DEVICE>();

				pDevOccNo = std::make_shared<TAG_OCCNO>();

				CDevicePtr ppDevice = m_pChannelMgr->CreateDevice(pChannel, pDevice);
				//设备属性
				pDevice->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pDevOccNo->OccNo = pDevice->OccNo;
				std::memset(pDevOccNo->TagName, 0, sizeof(pDevOccNo->TagName));

				strncpy(pDevOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH ), (size_t)m_XmlReader.attributes().value("TagName").length()));

				pDevice->NameOccNo = m_XmlReader.attributes().value("ID").toInt();
				pDevice->ModelOccNo = m_XmlReader.attributes().value("DeviceModelID").toInt();
				pDevice->ChannelOccNo = pChannel->GetOccNo();
				pDevice->NodeOccNo = m_nMyNodeOccNo;
				pDevice->ScanEnable = SCAN_IN;

				pDevice->Init = INITED;

				m_arrTempDevices.push_back(pDevice);

				m_arrDeviceOccNos.push_back(pDevOccNo);

				ReadData(ppDevice, pChannel);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

void CDbSvc::ReadData(CDevicePtr pDev, CChannelPtr pChannel)
{
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "data")
			{
				ReadDataInfo(pDev, pChannel);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

void CDbSvc::ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "AIN")
			{
				ReadAinInfo(pDev, pChannel);
			}
			else if (m_XmlReader.name() == "DIN")
			{
				ReadDinInfo(pDev, pChannel);
			}
			else if (m_XmlReader.name() == "AOUT")
			{
				ReadAoutInfo(pDev, pChannel);
			}
			else if (m_XmlReader.name() == "DOUT")
			{
				ReadDoutInfo(pDev, pChannel);
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

/*! \fn void CDbSvc::ReadScale()
*********************************************************************************************************
** \brief CDbSvc::ReadScale
** \details
** \return void
** \author xingzhibing
** \date 2016年9月19日
** \note
********************************************************************************************************/
void CDbSvc::ReadScale()
{
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "linear")
			{
				ReadLinearInfo();
			}
			else if (m_XmlReader.name() == "nonlinear")
			{
				ReadNonLinearInfo();
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn void CDbSvc::ReadAlarm()
*********************************************************************************************************
** \brief CDbSvc::ReadAlarm
** \details
** \return void
** \author xingzhibing
** \date 2016年9月19日
** \note
********************************************************************************************************/
void CDbSvc::ReadAlarm()
{
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "ain_alarm")
			{
				//TODO   先读取概要信息，暂未知意义，待稍后添加
				ReadAinAlarmInfo();
			}
			else if (m_XmlReader.name() == "din_alarm")
			{
				//TODO   先读取概要信息，暂未知意义，待稍后添加
				ReadDinAlarmInfo();
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}
/*! \fn void CDbSvc::ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear)
*********************************************************************************************************
** \brief CDbSvc::ReadNonLinearVal
** \details 读取非线性点信息
** \param pTransFormNonLinear
** \return void
** \author xingzhibing
** \date 2016年9月19日
** \note
********************************************************************************************************/

typedef TRANSFORM_NONLINEAR::LINEAR_ITEM DataItem;

bool SortLFunction(DataItem& data1, DataItem& data2)
{
	return data1.RawValue < data2.RawValue;
}

void CDbSvc::ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear)
{
	Q_ASSERT(pTransFormNonLinear);

	m_XmlReader.readNext();
	int nIndex = 0;
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "p")
			{
				pTransFormNonLinear->NonLinearData[nIndex].RawValue = m_XmlReader.attributes().value("RawValue").toDouble();
				pTransFormNonLinear->NonLinearData[nIndex].ScaleValue = m_XmlReader.attributes().value("ScaleValue").toDouble();
				nIndex++;

				m_XmlReader.readNext();

				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
	pTransFormNonLinear->DataCount = nIndex;
	std::sort(pTransFormNonLinear->NonLinearData, pTransFormNonLinear->NonLinearData + nIndex, SortLFunction);
}

void CDbSvc::ReadVariable()
{
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name()=="system")
			{
				ReadSystemVariable();
			}
			else if (m_XmlReader.name()=="user")
			{
				ReadUserVariable();
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

void CDbSvc::ReadSystemVariable()
{
	m_XmlReader.readNext();
	VARDATA_DEF pVarData = nullptr;
	TAG_OCCNO_DEF pVarTag = nullptr;
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name()=="s")
			{
				pVarData = std::make_shared<VARDATA>();
				pVarTag = std::make_shared<TAG_OCCNO>();
				pVarData->OccNo = m_XmlReader.attributes().value("OccNo").toUInt();
				pVarTag->OccNo = pVarData->OccNo;
				pVarData->DataType = m_XmlReader.attributes().value("DataType").toUInt();
				memset(pVarTag->TagName,0, MAX_NAME_LENGTH + STRING_PAD);
				strncpy(pVarTag->TagName,m_XmlReader.attributes().value("TagName").toString().toStdString().c_str(),
					qMin((size_t )m_XmlReader.attributes().value("TagName").length(), MAX_NAME_LENGTH + STRING_PAD));
				m_arrSyetemVarOccNos.push_back(pVarTag);
				m_arrTempSystemVariables.push_back(pVarData);
				m_XmlReader.readNext();
				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}

void CDbSvc::ReadUserVariable()
{
	m_XmlReader.readNext();

	VARDATA_DEF pVarData = nullptr;

	TAG_OCCNO_DEF pVarTag = nullptr;
	
	int nIndex = 0;

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isEndElement())
		{
			if (nIndex <= 0)
			{
				m_XmlReader.readNext();
				break;
			}
			else
			{
				nIndex--;
				m_XmlReader.readNext();
			}
		}
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "group")
			{
				nIndex++;
				m_XmlReader.readNext();
			}
			else if(m_XmlReader.name() == "u")
			{
				pVarData = std::make_shared<VARDATA>();
				pVarTag = std::make_shared<TAG_OCCNO>();

				pVarData->OccNo = m_XmlReader.attributes().value("OccNo").toUInt();
				pVarTag->OccNo = pVarData->OccNo;
				pVarData->DataType = m_XmlReader.attributes().value("DataType").toUInt();
				memset(pVarTag->TagName, 0, MAX_NAME_LENGTH + STRING_PAD);
				strncpy(pVarTag->TagName, m_XmlReader.attributes().value("TagName").toString().toStdString().c_str(),
					qMin((size_t)m_XmlReader.attributes().value("TagName").length(), MAX_NAME_LENGTH + STRING_PAD));
				pVarData->Init = DEFAULT_INIT;
				pVarData;
				m_arrUserVarOccNos.push_back(pVarTag);
				m_arrTempUserVariables.push_back(pVarData);
				m_XmlReader.readNext();
				if (m_XmlReader.isEndElement())
				{
					m_XmlReader.readNext();
				}
			}
			else
			{
				SkipUnknownElement();
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
}