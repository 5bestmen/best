#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDateTime>

#include "xmlreader.h"

CXmlReader::CXmlReader(QObject *parent)
	: QObject(parent)
{

}

CXmlReader::~CXmlReader()
{

}

/*! \fn void CXmlReader::SetFileInfo(std::shared_ptr<CDeviceMgr> pMgr)
********************************************************************************************************* 
** \brief CXmlReader::SetFileInfo 
** \details 获取指定管理器信息
** \param pMgr 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::SetFileInfo(std::shared_ptr<CDeviceMgr> pMgr)
{
	Q_ASSERT(pMgr);
	m_pDevMgr = pMgr;
}

/*! \fn void CXmlReader::ReadFileInfo(const QString& szName)
********************************************************************************************************* 
** \brief CXmlReader::ReadFileInfo 
** \details 开始读文件配置信息
** \param szName 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadFileInfo(const QString& szName)
{
	emit sig_ProcessInfo(tr("Begin Read File ~"));
	m_StartTime = GetProessInfo();
	if (!QFileInfo(szName).exists())
	{
		QMessageBox::information(Q_NULLPTR, "error", "file not exist");
		return;
	}
	QFile file(szName);
	if (!file.open(QIODevice::ReadOnly |QFile ::Text))
	{
		QMessageBox::information(Q_NULLPTR, "error", file.errorString());
	}
	m_XmlReader.setDevice(&file);
	m_XmlReader.readNext();
	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name()=="TestConfig")
			{
				emit sig_ProcessInfo(tr("ConfigTag"));
				ReadConfigElement();
			}
			else
			{
				m_XmlReader.raiseError("Not an invalid tagname");
			}
		}
		else
		{
			m_XmlReader.readNext();
		}
	}
	m_pDevMgr;
	file.close();

	emit sig_ProcessInfo(tr("File Read Over,Time Consuming %1").arg(GetProessInfo()-m_StartTime));
}


/*! \fn void CXmlReader::ReadConfigElement()
********************************************************************************************************* 
** \brief CXmlReader::ReadConfigElement 
** \details 第一步读到创建标签
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadConfigElement()
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
			if (m_XmlReader.name()=="Device")
			{
				ReadDevElement();
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

/*! \fn void CXmlReader::SkipUnknownElement()
********************************************************************************************************* 
** \brief CXmlReader::SkipUnknownElement 
** \details 遇到未知标签，则读取下一个标签
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::SkipUnknownElement()
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

/*! \fn void CXmlReader::ReadDevElement()
********************************************************************************************************* 
** \brief CXmlReader::ReadDevElement 
** \details 第二步，读取设备标签
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadDevElement()
{
	CDevice* pDev = new CDevice;
	pDev->m_OccNo = m_XmlReader.attributes().value("DevOccNo").toInt();
	pDev->m_szDevName = m_XmlReader.attributes().value("DevName").toString();

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
			if (m_XmlReader.name()=="AIN")
			{
				ReadAinElement(pDev);
			}
			else if (m_XmlReader.name()=="DIN")
			{
				ReadDinElement(pDev);
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
	m_pDevMgr->AddDevice(pDev);
	emit sig_ProcessInfo(tr("[%1] Device Info,Time Consuming %2").arg(pDev->m_OccNo).arg(GetProessInfo()-m_StartTime));
}

/*! \fn void CXmlReader::ReadAinElement(CDevice* pDev)
********************************************************************************************************* 
** \brief CXmlReader::ReadAinElement 
** \details 第三步，读取设备模拟量点
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadAinElement(CDevice* pDev)
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
			if (m_XmlReader.name() == "a")
			{
				ReadAinDataElement(pDev);
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

/*! \fn void CXmlReader::ReadDinElement(CDevice* pDev)
********************************************************************************************************* 
** \brief CXmlReader::ReadDinElement 
** \details 第四步，读取设备开关量点
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadDinElement(CDevice* pDev)
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
			if (m_XmlReader.name() == "d")
			{
				ReadDinDataElement(pDev);
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

/*! \fn void CXmlReader::ReadAinDataElement(CDevice* pDev)
********************************************************************************************************* 
** \brief CXmlReader::ReadAinDataElement 
** \details 读取单个模拟量点信息 
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadAinDataElement(CDevice* pDev)
{
	
	AIN* pAin = new AIN;
	pAin->m_OccNo = m_XmlReader.attributes().value("OccNo").toInt();
	pAin->m_Id = m_XmlReader.attributes().value("Id").toInt();
	pAin->m_ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
	pAin->m_DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
	pAin->m_BlockOccNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
	pAin->m_Address = m_XmlReader.attributes().value("Address").toInt();
	pAin->m_DataSource = m_XmlReader.attributes().value("DataSource").toInt();
	pAin->m_InitVal = m_XmlReader.attributes().value("InitVal").toInt();
	pAin->m_TransFromType = m_XmlReader.attributes().value("TransFromType").toInt();
	pAin->m_AlarmType = m_XmlReader.attributes().value("AlarmType").toInt();
	pAin->m_RangeL = m_XmlReader.attributes().value("RangeL").toFloat();
	pAin->m_RangeH = m_XmlReader.attributes().value("RangeH").toInt();
	pAin->m_HighQua = m_XmlReader.attributes().value("HighQua").toFloat();
	pAin->m_LowQua = m_XmlReader.attributes().value("LowQua").toFloat();
	pAin->m_PinLanel = m_XmlReader.attributes().value("PinLanel").toInt();
	pAin->m_DataType = m_XmlReader.attributes().value("DataType").toInt();
	pAin->m_SaveDisk = m_XmlReader.attributes().value("SaveDisk").toInt();
	pAin->m_SaveDiskPeriod = m_XmlReader.attributes().value("SaveDiskPeriod").toInt();
	pAin->m_SaveDb = m_XmlReader.attributes().value("SaveDb").toInt();
	pAin->m_SaveDbPeriod = m_XmlReader.attributes().value("SaveDbPeriod").toInt();
	pAin->m_Sensitivity = m_XmlReader.attributes().value("Sensitivity").toInt();
	pAin->m_ReferenceCount = m_XmlReader.attributes().value("ReferenceCount").toInt();
	pAin->m_szTagName = m_XmlReader.attributes().value("szTagName").toString();
	pAin->m_szDescription = m_XmlReader.attributes().value("szDescription").toString();
	pAin->m_szTransFormName = m_XmlReader.attributes().value("szTransFormName").toString();
	pAin->m_szTransFormDesc = m_XmlReader.attributes().value("szTransFormDesc").toString();
	pAin->m_szAlarmTagName = m_XmlReader.attributes().value("szAlarmTagName").toString();
	pAin->m_szAlarmDesc = m_XmlReader.attributes().value("szAlarmDesc").toString();
	pAin->m_szUnitName = m_XmlReader.attributes().value("szUnitName").toString();
	pAin->m_szExpress = m_XmlReader.attributes().value("szExpress").toString();

	pDev->AddAInInfo(pAin);

	m_XmlReader.readNext();

	if (m_XmlReader.isEndElement())
	{
		m_XmlReader.readNext();
	}
}

/*! \fn void CXmlReader::ReadDinDataElement(CDevice* pDev)
********************************************************************************************************* 
** \brief CXmlReader::ReadDinDataElement 
** \details 读取单个开关量点信息
** \param pDev 
** \return void 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
void CXmlReader::ReadDinDataElement(CDevice* pDev)
{
	m_XmlReader.readNext();

	DIN* pDin = new DIN;
	pDin->m_OccNo = m_XmlReader.attributes().value("OccNo").toInt();
	pDin->m_Id = m_XmlReader.attributes().value("Id").toInt();
	pDin->m_ChannelOccNo = m_XmlReader.attributes().value("ChannelOccNo").toInt();
	pDin->m_DeviceOccNo = m_XmlReader.attributes().value("DeviceOccNo").toInt();
	pDin->m_BlockOccNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
	pDin->m_Address = m_XmlReader.attributes().value("Address").toInt();
	pDin->m_DataSource = m_XmlReader.attributes().value("DataSource").toInt();
	pDin->m_InitVal = m_XmlReader.attributes().value("InitVal").toInt();
	pDin->m_TransFromType = m_XmlReader.attributes().value("TransFromType").toInt();
	pDin->m_AlarmType = m_XmlReader.attributes().value("AlarmType").toInt();

	pDin->m_PinLabel = m_XmlReader.attributes().value("PinLabel").toInt();
	pDin->m_bIsSBO = m_XmlReader.attributes().value("bIsSBO").toInt();
	pDin->m_TelCtrlWaitTime = m_XmlReader.attributes().value("TelCtrlWaitTime").toInt();
	pDin->m_TelCtrlSendTime = m_XmlReader.attributes().value("TelCtrlSendTime").toInt();
	pDin->m_ReferenceCount = m_XmlReader.attributes().value("ReferenceCount").toInt();

	pDin->m_szTagName = m_XmlReader.attributes().value("szTagName").toString();
	pDin->m_szDescription = m_XmlReader.attributes().value("szDescription").toString();
	pDin->m_szTransFormName = m_XmlReader.attributes().value("szTransFormName").toString();
	pDin->m_szTransFormDesc = m_XmlReader.attributes().value("szTransFormDesc").toString();
	pDin->m_szAlarmTagName = m_XmlReader.attributes().value("szAlarmTagName").toString();
	pDin->m_szAlarmDesc = m_XmlReader.attributes().value("szAlarmDesc").toString();
	pDin->m_szExpress = m_XmlReader.attributes().value("szExpress").toString();

	pDev->AddDINInfo(pDin);

	m_XmlReader.readNext();

	if (m_XmlReader.isEndElement())
	{
		m_XmlReader.readNext();
	}
}

/*! \fn unsigned int CXmlReader::GetProessInfo()
********************************************************************************************************* 
** \brief CXmlReader::GetProessInfo 
** \details 获取本地文件时间信息
** \return unsigned int 
** \author xingzhibing
** \date 2016年9月1日 
** \note 
********************************************************************************************************/
unsigned int CXmlReader::GetProessInfo()
{
	return QDateTime::currentDateTime().toTime_t();
}
