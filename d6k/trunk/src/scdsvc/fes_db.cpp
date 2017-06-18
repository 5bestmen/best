/*! @file fes_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  fes_db.cpp
文件实现功能 :  前置节点的内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   前置节点的内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "fes_db.h"
#include "log/log.h"
#include "fesapi/fes_magicmem.h"
#include "fesapi/fescnst.h"
#include "fesapi/fesdatadef.h"
#include "stl_util-inl.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QObject> 
#include <QString>  

CFesDB::CFesDB( )
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
	m_nTagNameEstimateSize = 0;
	m_pChannelMgr = std::make_shared<CChannelMgr>();
}

CFesDB::~CFesDB()
{
	
}

/*! \fn bool CFesDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CFesDB::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CFesDB::Initialize(const char *pszDataPath, unsigned int nMode, int32u nOccNo)
{
	QString szLog;
	m_nOccNo = nOccNo;

	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <MAX_NODE_OCCNO );
	if (nOccNo==INVALID_OCCNO || nOccNo >MAX_NODE_OCCNO)
	{
		return false;
	}
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	if (!LoadFesXml(pszDataPath))
	{
		Q_ASSERT(false);
		return false;
	}

	return true;
}

void CFesDB::Run()
{
 
}

void CFesDB::Shutdown()
{ 

}

// 应用层 获取模拟量
bool CFesDB::GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAinCount)
		return false;

	val = m_pAins[nOccNo - 1].Value;
	nQuality = m_pAins[nOccNo - 1].Quality;

	return true;
}

bool CFesDB::GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAinCount)
		return false;

	val = m_pDins[nOccNo - 1].Value;
	nQuality = m_pDins[nOccNo - 1].Quality;
	 
	return true;
}
/*! \fn bool CFesDB::UpdateAinValue(int32u nOccNo, const CVariant & val, int8u nQuality)
********************************************************************************************************* 
** \brief CFesDB::UpdateAinValue 
** \details 更新测值，网络总线服务调用，
** \param nOccNo 
** \param val 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年12月19日 
** \note 
********************************************************************************************************/
bool CFesDB::UpdateAinValue(int32u nOccNo, const CVariant & val, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAinCount)
		return false;

	m_pAins[nOccNo - 1].Value = val.operator double();
	m_pAins[nOccNo - 1].Quality = nQuality;

	return true;
}

bool CFesDB::UpdateDinValue(int32u nOccNo, const CVariant & val, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO || nOccNo > m_nAinCount)
		return false;

	m_pDins[nOccNo - 1].Value = val.operator int8u();
	m_pDins[nOccNo - 1].Quality = nQuality;
	return true;
}

bool CFesDB::GetAinAlarmByOccNo(int32u nOccNo, AIN_ALARM** pAinAlarm)
{
	Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo <=m_nAinAlarmCount);
	if (nOccNo ==INVALID_OCCNO  || nOccNo >m_nAinAlarmCount)
	{
		return false;
	}
	*pAinAlarm = &m_pAinAlarm[nOccNo - 1];
	
	Q_ASSERT(pAinAlarm);
	if (!pAinAlarm)
	{
		return false;
	}
	
	return true;
}

bool CFesDB::GetAinAlarmLimitByOcNo(int32u nOccNo, AIN_ALARM_LIMIT** pAinAlarmLimit)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= m_nAinAlarmLimitCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > m_nAinAlarmLimitCount)
	{
		return false;
	}
	*pAinAlarmLimit = &m_pAinAlarmLimit[nOccNo - 1];

	Q_ASSERT(pAinAlarmLimit);
	if (!pAinAlarmLimit)
	{
		return false;
	}

	return true;
}

bool CFesDB::GetDinAlarmByOccNo(int32u nOccNo, DIN_ALARM** pDinAlarm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= m_nDinAlarmCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > m_nDinAlarmCount)
	{
		return false;
	}
	*pDinAlarm = &m_pDinAlarm[nOccNo - 1];

	Q_ASSERT(pDinAlarm);
	if (!pDinAlarm)
	{
		return false;
	}

	return true;
}

bool CFesDB::GetDinAlarmLimitByOccNo(int32u nOccNo, DIN_ALARM_LIMIT** pDinAlarmLimit)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= m_nDinAlarmLimitCount);
	if (nOccNo == INVALID_OCCNO || nOccNo > m_nDinAlarmLimitCount)
	{
		return false;
	}
	*pDinAlarmLimit = &m_pDinAlarmLimit[nOccNo - 1];

	Q_ASSERT(pDinAlarmLimit);
	if (!pDinAlarmLimit)
	{
		return false;
	}

	return true;
}

/*! \fn bool CFesDB::LoadFesXml(const char* pszDataPath)
********************************************************************************************************* 
** \brief CFesDB::LoadFesXml 
** \details 各自加载xml文件
** \param pszDataPath 
** \return bool 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
bool CFesDB::LoadFesXml(const char* pszDataPath)
{
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	m_szProjName = pszDataPath;

	LogMsg(szLog.toStdString().c_str(), 0);

	if (!QFileInfo(pszDataPath).exists())
	{
		LogMsg(QObject::tr("%1:file not exist").arg(pszDataPath).toStdString().c_str(), 0);
		return false;
	}
	QFile file(pszDataPath);

	if (!file.open(QIODevice::ReadOnly | QFile::Text))
	{
		return false;
	}
	m_XmlReader.setDevice(&file);
	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		if (m_XmlReader.isStartElement())
		{
			if (m_XmlReader.name() == "fes")
			{
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

	m_nChannelCount = m_arrTempChannels.size();

	m_nDeviceCount = m_arrTempDevices.size();

	m_nAinCount = m_arrTempAins.size();

	m_nDinCount = m_arrTempDins.size();

	m_nAoutCount = m_arrTempAouts.size();

	m_nDoutCount = m_arrTempDouts.size();

	m_nAinAlarmCount = m_arrTempAinAlarms.size();

	m_nAinAlarmLimitCount = m_arrTempAinLimitAlarms.size();

	m_nDinAlarmCount = m_arrTempDinAlarms.size();

	m_nDinAlarmLimitCount = m_arrTempDinLimitAlarms.size();

	m_nSystemVariableCount = m_arrTempSystemVariables.size();

	m_nUserVariableCount = m_arrTempUserVariables.size();

	m_nEstimateSize += sizeof  HEAD_MEM + 
		sizeof CHANNEL * m_nChannelCount +
		sizeof DEVICE  * m_nDeviceCount +
		sizeof AIN     * m_nAinCount +
		sizeof DIN     * m_nDinCount +
		sizeof AOUT    * m_nAoutCount +
		sizeof DOUT    * m_nDoutCount +
		sizeof AIN_ALARM * m_nAinAlarmCount +
		sizeof AIN_ALARM_LIMIT * m_nAinAlarmLimitCount +
		sizeof DIN_ALARM * m_nDinAlarmCount +
		sizeof DIN_ALARM_LIMIT * m_nDinAlarmLimitCount+
		sizeof VARDATA * m_nSystemVariableCount +
		sizeof VARDATA * m_nUserVariableCount;

	m_nTagNameEstimateSize = sizeof TAG_OCCNO * 
							 (m_nChannelCount + m_nDeviceCount + 
							  m_nAinCount + m_nDinCount + 
							  m_nAoutCount + 
							  m_nDoutCount+
							  m_nAinAlarmLimitCount + 
							  m_nAinAlarmCount+ 
							  m_nDinAlarmCount+ 
							  m_nDinAlarmLimitCount +
							  m_nSystemVariableCount+
							  m_nUserVariableCount) + sizeof HEAD_MEM;
	return true;
}

/*! \fn bool CFesDB::ReadFesInfo()
********************************************************************************************************* 
** \brief CFesDB::ReadFesInfo 
** \details 读取当前前置
** \return bool 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
bool CFesDB::ReadFesInfo()
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
	return true;
}

/*! \fn void CFesDB::ReadChannelInfo()
********************************************************************************************************* 
** \brief CFesDB::ReadChannelInfo 
** \details 读取通道信息
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadChannelInfo()
{
	CHANNEL_PTR pChannel = Q_NULLPTR;

	TAG_OCCNO_DEF pChannelOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

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
			else if (m_XmlReader.name() == "c")
			{
				pChannel = std::make_shared<CHANNEL>();

				pChannelOccNo = std::make_shared<TAG_OCCNO>();
				//读取文件
				CChannelPtr ppChannel = m_pChannelMgr->CreateChannel(pChannel);

				pChannel->OccNo = m_XmlReader.attributes().value("OccNo").toInt();
				pChannel->NodeOccNo = m_nOccNo;

				strncpy(pChannel->ChannelName,
					m_XmlReader.attributes().value("ChannelName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("ChannelName").length()));
				strncpy(pChannel->DriverName,
					m_XmlReader.attributes().value("DriverName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("DriverName").length()));

				pChannelOccNo->OccNo = pChannel->OccNo;
				std::memset(pChannelOccNo->TagName, 0, sizeof(pChannelOccNo->TagName));

				strncpy(pChannelOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

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

/*! \fn void CFesDB::ReadDev(CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadDev 
** \details 读取设备信息 框架
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadDev(CChannelPtr pChannel)
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
			qDebug() << m_XmlReader.name();

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

/*! \fn void CFesDB::ReadDevInfo(CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadDevInfo 
** \details 读取设备信息 细节
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadDevInfo(CChannelPtr pChannel)
{
	Q_ASSERT(pChannel);

	DEVICE_PTR pDevice = Q_NULLPTR;

	TAG_OCCNO_DEF pDevOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

	while (!m_XmlReader.atEnd())
	{
		qDebug() << m_XmlReader.name();

		if (m_XmlReader.isEndElement())
		{
			m_XmlReader.readNext();
			break;
		}
		if (m_XmlReader.isStartElement())
		{
			qDebug() << m_XmlReader.name();

			if (m_XmlReader.name() == "d")
			{
				pDevice = std::make_shared<DEVICE>();
				pDevOccNo = std::make_shared<TAG_OCCNO>();
				CDevicePtr ppDevice = m_pChannelMgr->CreateDevice(pChannel, pDevice);
				//设备属性
				pDevice->OccNo = m_XmlReader.attributes().value("OccNo").toInt();
				pDevice->NodeOccNo = m_nOccNo;
				pDevice->NameOccNo = m_XmlReader.attributes().value("ID").toInt();
				pDevice->ModelOccNo = m_XmlReader.attributes().value("DeviceModelID").toInt();
				pDevice->ChannelOccNo = pChannel->GetOccNo();// m_XmlReader.attributes().value("ID").toInt();

				pDevOccNo->OccNo = pDevice->OccNo;
				std::memset(pDevOccNo->TagName, 0, sizeof(pDevOccNo->TagName));

				strncpy(pDevOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

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

/*! \fn void CFesDB::ReadData(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadData 
** \details 读取设备点信息 AIN DIN AOUT DOUT 框架
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadData(CDevicePtr pDev, CChannelPtr pChannel)
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

/*! \fn void CFesDB::ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadDataInfo 
** \details 读取设备点信息 AIN DIN AOUT DOUT 细节
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel)
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

/*! \fn void CFesDB::ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadAinInfo 
** \details 读取AIN信息
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(pDev && pChannel);

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

				pAin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				//TODO
				pAin->NameOccNo = m_XmlReader.attributes().value("").toInt();
				pAin->ChannelOccNo = pChannel->GetOccNo();// m_XmlReader.attributes().value("ChannelOccNo").toInt();
				pAin->DeviceOccNo = pDev->GetOccNo();// m_XmlReader.attributes().value("DeviceOccNo").toInt();
				pAin->NodeOccNo = m_nOccNo;
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

				pAinOccNo->OccNo = pAin->OccNo;
				std::memset(pAinOccNo->TagName, 0, sizeof(pAinOccNo->TagName));

				strncpy(pAinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

				m_arrAinOccNos.push_back(pAinOccNo);

				m_arrTempAins.push_back(pAin);

				pChannel->AddAin(pAin);

				pDev->AddAin(pAin);

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

/*! \fn void CFesDB::ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadDinInfo 
** \details 读取DIN信息
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(pDev && pChannel);

	DIN_PTR pDin = Q_NULLPTR;

	TAG_OCCNO_DEF pDinOccNo = Q_NULLPTR;

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

				pDinOccNo = std::make_shared<TAG_OCCNO>();

				pDin->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pDin->ChannelOccNo = pChannel->GetOccNo();// m_XmlReader.attributes().value("ChannelOccNo").toInt();
				pDin->DeviceOccNo = pDev->GetOccNo();// m_XmlReader.attributes().value("DeviceOccNo").toInt();
				pDin->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				pDin->NodeOccNo = m_nOccNo;
				strncpy(pDin->Address,
					m_XmlReader.attributes().value("Address").toString().toStdString().c_str(),
					qMin((size_t)MAX_ADDRESS_LENGTH, (size_t)m_XmlReader.attributes().value("Address").length()));
				pDin->Init = m_XmlReader.attributes().value("InitValue").toFloat();
				pDin->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				pDin->AlarmOccNo = m_XmlReader.attributes().value("AlarmOccNo").toInt();

				pDinOccNo->OccNo = pDin->OccNo;
				std::memset(pDinOccNo->TagName, 0, sizeof(pDinOccNo->TagName));

				strncpy(pDinOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

				m_arrTempDins.push_back(pDin);

				m_arrDinOccNos.push_back(pDinOccNo);

				pChannel->AddDin(pDin);

				pDev->AddDin(pDin);

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

/*! \fn void CFesDB::ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadAoutInfo 
** \details 读取AOUT信息
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(pDev && pChannel);

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

				pAoutOccNo = std::make_shared<TAG_OCCNO>();

				pAout->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pAout->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				//TODO
				pAout->NameOccNo = m_XmlReader.attributes().value("").toInt();
				pAout->ChannelOccNo = pChannel->GetOccNo();// m_XmlReader.attributes().value("ChannelOccNo").toInt();
				pAout->DeviceOccNo = pDev->GetOccNo();// m_XmlReader.attributes().value("DeviceOccNo").toInt();
				pAout->NodeOccNo = m_nOccNo;
				pAout->DataSource = m_XmlReader.attributes().value("DataSource").toInt();
				pAout->TransformType = m_XmlReader.attributes().value("TransformType").toInt();

				pAoutOccNo->OccNo = pAout->OccNo;
				std::memset(pAoutOccNo->TagName, 0, sizeof(pAoutOccNo->TagName));

				strncpy(pAoutOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

				m_arrTempAouts.push_back(pAout);

				m_arrAoutOccNos.push_back(pAoutOccNo);

				pChannel->AddAout(pAout);

				pDev->AddAout(pAout);

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

/*! \fn void CFesDB::ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
********************************************************************************************************* 
** \brief CFesDB::ReadDoutInfo 
** \details 读取DOUT信息
** \param pDev 
** \param pChannel 
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel)
{
	Q_ASSERT(pDev && pChannel);
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

				pDout->ChannelOccNo = pChannel->GetOccNo();// m_XmlReader.attributes().value("ChannelOccNo").toInt();
				pDout->DeviceOccNo = pDev->GetOccNo();// m_XmlReader.attributes().value("DeviceOccNo").toInt();
				pDout->BlockNo = m_XmlReader.attributes().value("BlockOccNo").toInt();
				pDout->NodeOccNo = m_nOccNo;
				pDout->Init = m_XmlReader.attributes().value("InitValue").toFloat();
				pDout->DataSource = m_XmlReader.attributes().value("DataSource").toInt();

				pDoutOccNo->OccNo = pDout->OccNo;
				std::memset(pDoutOccNo->TagName, 0, sizeof(pDoutOccNo->TagName));
				strncpy(pDoutOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));


				m_arrTempDouts.push_back(pDout);

				m_arrDoutOccNos.push_back(pDoutOccNo);

				pChannel->AddDout(pDout);

				pDev->AddDout(pDout);

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

void CFesDB::ReadAlarm()
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

void CFesDB::ReadAinAlarmInfo()
{
	AIN_ALARM_DEF pAlarmDef = Q_NULLPTR;

	TAG_OCCNO_DEF pAinAlarmOccNo = Q_NULLPTR;

	m_XmlReader.readNext();

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
			else if (m_XmlReader.name() == "ain_alarm_node")
			{
				pAlarmDef = std::make_shared<AIN_ALARM>();

				pAlarmDef->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pAinAlarmOccNo = std::make_shared<TAG_OCCNO>();

				pAinAlarmOccNo->OccNo = pAlarmDef->OccNo;
				std::memset(pAinAlarmOccNo->TagName, 0, sizeof(pAinAlarmOccNo->TagName));
				strncpy(pAinAlarmOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

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

void CFesDB::ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm)
{
	AIN_ALARM_LIMIT_DEF pAlarmLimit = Q_NULLPTR;

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
			 if(m_XmlReader.name() == "aa")
			{
				pAlarmLimit = std::make_shared<AIN_ALARM_LIMIT>();
				pAlarmLimit->OccNo = m_XmlReader.attributes().value("OccNo").toInt();


				pAlarmLimitOccNo = std::make_shared<TAG_OCCNO>();

				pAlarmLimitOccNo->OccNo = pAlarmLimit->OccNo;
				pAlarmLimit->AlarmOccNo = pAinAlarm->OccNo;
				std::memset(pAlarmLimitOccNo->TagName, 0, sizeof(pAlarmLimitOccNo->TagName));

				strncpy(pAlarmLimitOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));
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

void CFesDB::ReadDinAlarmInfo()
{
	DIN_ALARM_DEF pAlarmDef = Q_NULLPTR;

	TAG_OCCNO_DEF pAlarmOccNo = Q_NULLPTR;
	m_XmlReader.readNext();

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
			else if (m_XmlReader.name() == "din_alarm_node")
			{
				pAlarmDef = std::make_shared<DIN_ALARM>();

				pAlarmOccNo = std::make_shared<TAG_OCCNO>();


				std::memset(pAlarmOccNo->TagName, 0, sizeof(pAlarmOccNo->TagName));

				strncpy(pAlarmOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));

				pAlarmDef->OccNo = m_XmlReader.attributes().value("OccNo").toInt();
				pAlarmOccNo->OccNo = pAlarmDef->OccNo;

				pAlarmDef->OnQualityGood = m_XmlReader.attributes().value("OnQualityGood").toInt();

				m_arrTempDinAlarms.push_back(pAlarmDef);

				m_arrDinAlarmOccNos.push_back(pAlarmOccNo);

				ReadDinAlarmLimitInfo(pAlarmDef);
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

void CFesDB::ReadDinAlarmLimitInfo(DIN_ALARM_DEF pDinAlarm)
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
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));
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

void CFesDB::ReadVariable()
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
			if (m_XmlReader.name() == "system")
			{
				ReadSysVariable();
			}
			else if (m_XmlReader.name() == "user")
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

void CFesDB::ReadSysVariable()
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
			if (m_XmlReader.name() == "s")
			{
				pVarData = std::make_shared<VARDATA>();
				pVarTag = std::make_shared<TAG_OCCNO>();
				pVarData->OccNo = m_XmlReader.attributes().value("OccNo").toUInt();
				pVarTag->OccNo = pVarData->OccNo;
				pVarData->DataType = m_XmlReader.attributes().value("DataType").toUInt();
				memset(pVarTag->TagName, 0, MAX_NAME_LENGTH + STRING_PAD);
				strncpy(pVarTag->TagName, m_XmlReader.attributes().value("TagName").toString().toStdString().c_str(),
					qMin((size_t)m_XmlReader.attributes().value("TagName").length(), MAX_NAME_LENGTH + STRING_PAD));
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

void CFesDB::ReadUserVariable()
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
			else if (m_XmlReader.name() == "u")
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

/*! \fn void CFesDB::SkipUnknownElement()
********************************************************************************************************* 
** \brief CFesDB::SkipUnknownElement 
** \details 跳过未知节点
** \return void 
** \author xingzhibing
** \date 2016年12月26日 
** \note 
********************************************************************************************************/
void CFesDB::SkipUnknownElement()
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

/*! \fn size_t CFesDB::EstimateMemSize()
********************************************************************************************************* 
** \brief CFesDB::GetEstimateSize 
** \details 获取预估计大小
** \return size_t 
** \author xingzhibing
** \date 2016年12月27日 
** \note 
********************************************************************************************************/
size_t CFesDB::EstimateMemSize()
{
	return m_nEstimateSize;
}



size_t CFesDB::BuildDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	size_t nLen = 0;

	//Head
	m_pMagicMem = (HEAD_MEM*)(pAddr);
	m_pMagicMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicMem->ShmLength = m_nEstimateSize;
	strncpy(m_pMagicMem->Config, m_szProjName.toStdString().c_str(), qMin(MEM_CONFIG_LEN,m_szProjName.length()));
	m_pMagicMem->ChannelCount = m_nChannelCount;
	m_pMagicMem->DeviceCount = m_nDeviceCount;
	m_pMagicMem->AinCount = m_nAinCount;
	m_pMagicMem->DinCount = m_nDinCount;
	m_pMagicMem->AoutCount = m_nAoutCount;
	m_pMagicMem->DoutCount = m_nDoutCount;

	m_pMagicMem->AinAlarmCount = m_nAinAlarmCount;
	m_pMagicMem->AinAlarmLimitCount = m_nAinAlarmLimitCount;
	m_pMagicMem->DinAlarmCount = m_nDinAlarmCount;
	m_pMagicMem->DinAlarmLimitCount = m_nDinAlarmLimitCount;
	m_pMagicMem->SystemVariableCount = m_nSystemVariableCount;
	m_pMagicMem->UserVariableCount = m_nUserVariableCount;

	pAddr += sizeof HEAD_MEM;

	//channel
	nLen = BuildChannelDB(pAddr);
	pAddr += nLen;

	//device
	nLen = BuildDeviceDB(pAddr);
	pAddr += nLen;

	//ain
	nLen = BuildAinDB(pAddr);
	pAddr += nLen;

	//din
	nLen = BuildDinDB(pAddr);
	pAddr += nLen;

	//aout
	nLen = BuildAoutDB(pAddr);
	pAddr += nLen;

	//dout
	nLen = BuildDoutDB(pAddr);
	pAddr += nLen;

	//ainalarm
	nLen = BuildAinAlarms(pAddr);
	pAddr += nLen;

	//ainalarmlimit
	nLen = BuildAinAlarmLimits(pAddr);
	pAddr += nLen;

	//dinalarm
	nLen = BuildDinAlarms(pAddr);
	pAddr += nLen;

	//dinalarmlimit
	nLen = BuildDinAlarmLimits(pAddr);
	pAddr += nLen;

	nLen = BuildSystemVariableDB(pAddr);
	pAddr += nLen;

	nLen = BuildUserVariableDB(pAddr);
	pAddr += nLen;

	return m_nEstimateSize;
}

size_t CFesDB::BuildChannelDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	int nIndex = 0;

	CHANNEL*  pChannel = Q_NULLPTR;

	m_pChannels = (CHANNEL *)((char*)pAddr);

	for (auto i : m_arrTempChannels)
	{
		pChannel = (CHANNEL*)((char*)m_pChannels + sizeof(CHANNEL)*nIndex);

		std::memcpy(pChannel, i.get(), sizeof(CHANNEL));

		m_arrChannels.push_back(pChannel);

		nIndex++;
	}

	return sizeof CHANNEL * m_arrChannels.size();
}


size_t CFesDB::BuildDeviceDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	int nIndex = 0;

	DEVICE* pDevNode = Q_NULLPTR;

	m_pDevices = (DEVICE*)((char*)pAddr);

	for (auto i : m_arrTempDevices)
	{
		pDevNode = (DEVICE *)((char*)m_pDevices + sizeof(DEVICE)*nIndex);

		m_arrDevices.push_back(pDevNode);

		std::memcpy(pDevNode, i.get(), sizeof(DEVICE));

		nIndex++;
	}
	return sizeof DEVICE * m_arrTempDevices.size();
}


size_t CFesDB::BuildAinDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	int nIndex = 0;

	AIN* pAin = Q_NULLPTR;

	m_pAins = (AIN*)((char*)pAddr);

	for (auto i : m_arrTempAins)
	{
		pAin = (AIN*)((char*)m_pAins + sizeof(AIN)*nIndex);

		i->ScanEnable = SCAN_IN;

		i->ManSet = NORMAL;

		i->IsDefined = FES_YES;

		m_arrAins.push_back(pAin);

		std::memcpy(pAin, i.get(), sizeof(AIN));

		nIndex++;
	}

	return sizeof AIN * m_arrTempAins.size();
}

size_t CFesDB::BuildDinDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;

	DIN* pDin = Q_NULLPTR;

	m_pDins = (DIN*)((char*)pAddr);

	for (auto i : m_arrTempDins)
	{
		pDin = (DIN*)((char*)m_pDins + sizeof(DIN)*nIndex);

		i->ScanEnable = SCAN_IN;

		i->ManSet = NORMAL;

		i->IsDefined = FES_YES;

		m_arrDins.push_back(pDin);

		std::memcpy(pDin, i.get(), sizeof(DIN));

		nIndex++;
	}


	return sizeof DIN * m_arrTempDins.size();
}

size_t CFesDB::BuildAoutDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;
	AOUT* pAout = Q_NULLPTR;
	m_pAouts = (AOUT *)((char*)pAddr);

	for (auto i : m_arrTempAouts)
	{
		pAout = (AOUT*)((char*)m_pAouts + sizeof(AOUT)*nIndex);

		m_arrAouts.push_back(pAout);

		std::memcpy(pAout, i.get(), sizeof(AOUT));

		nIndex++;
	}

	return sizeof AOUT * m_arrTempAouts.size();
}
size_t CFesDB::BuildDoutDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	int nIndex = 0;
	DOUT* pDout = Q_NULLPTR;

	m_pDouts = (DOUT *)((char*)pAddr );

	for (auto i : m_arrTempDouts)
	{
		pDout = (DOUT*)((char*)m_pDouts + sizeof(DOUT)*nIndex);

		m_arrDouts.push_back(pDout);

		std::memcpy(pDout, i.get(), sizeof(DOUT));

		nIndex++;
	}

	return sizeof DOUT * m_arrTempDouts.size();
}

size_t CFesDB::BuildAinAlarms(char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;

	AIN_ALARM* pAinAlarm = Q_NULLPTR;

	m_pAinAlarm = (AIN_ALARM *)((char*)pAddr);

	for (auto i : m_arrTempAinAlarms)
	{
		pAinAlarm = (AIN_ALARM*)((char*)m_pAinAlarm + sizeof(AIN_ALARM)*nIndex);

		m_arrAinAlarms.push_back(pAinAlarm);

		std::memcpy(pAinAlarm, i.get(), sizeof(AIN_ALARM));

		nIndex++;
	}

	return sizeof AIN_ALARM * m_arrAinAlarms.size();
}

size_t CFesDB::BuildAinAlarmLimits(char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;

	AIN_ALARM_LIMIT* pAinLimitAlarm = Q_NULLPTR;

	m_pAinAlarmLimit = (AIN_ALARM_LIMIT *)((char*)pAddr);

	for (auto i : m_arrTempAinLimitAlarms)
	{
		pAinLimitAlarm = (AIN_ALARM_LIMIT*)((char*)m_pAinAlarmLimit + sizeof(AIN_ALARM_LIMIT)*nIndex);

		m_arrAinAlarmLimits.push_back(pAinLimitAlarm);

		std::memcpy(pAinLimitAlarm, i.get(), sizeof(AIN_ALARM_LIMIT));

		nIndex++;
	}

	return  sizeof AIN_ALARM_LIMIT * m_nAinAlarmLimitCount;
		 
}

size_t CFesDB::BuildDinAlarms(char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;

	DIN_ALARM* pDinAlarm = Q_NULLPTR;

	m_pDinAlarm = (DIN_ALARM *)((char*)pAddr);

	for (auto i : m_arrTempDinAlarms)
	{
		pDinAlarm = (DIN_ALARM*)((char*)m_pDinAlarm + sizeof(DIN_ALARM)*nIndex);

		m_arrDinAlarms.push_back(pDinAlarm);

		std::memcpy(pDinAlarm, i.get(), sizeof(DIN_ALARM));

		nIndex++;
	}

	return m_nDinAlarmCount * sizeof DIN_ALARM;
}

size_t CFesDB::BuildDinAlarmLimits(char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	int nIndex = 0;

	DIN_ALARM_LIMIT* pDinAlarmLinit = Q_NULLPTR;

	m_pDinAlarmLimit = (DIN_ALARM_LIMIT*)(char*)(pAddr);

	for (auto i : m_arrTempDinLimitAlarms)
	{
		pDinAlarmLinit = (DIN_ALARM_LIMIT*)((char*)m_pDinAlarmLimit + sizeof(DIN_ALARM_LIMIT)*nIndex);

		m_arrDinAlarmLimits.push_back(pDinAlarmLinit);

		std::memcpy(pDinAlarmLinit, i.get(), sizeof(DIN_ALARM_LIMIT));

		nIndex++;
	}
	return m_nDinAlarmLimitCount * sizeof DIN_ALARM_LIMIT;
}

size_t CFesDB::BuildSystemVariableDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	int nIndex = 0;

	VARDATA* pSystemVar = Q_NULLPTR;

	m_pSystemVariable = (VARDATA *)((char*)pAddress);

	for (auto i : m_arrTempSystemVariables)
	{
		pSystemVar = (VARDATA*)((char*)m_pSystemVariable + sizeof(VARDATA)*nIndex);

		m_arrSystemVariables.push_back(pSystemVar);

		std::memcpy(pSystemVar, i.get(), sizeof(VARDATA));

		nIndex++;
	}

	m_nSystemVariableCount = m_arrSystemVariables.size();

	m_pMagicMem->SystemVariableCount = m_arrSystemVariables.size();

	m_pMagicMem->SystemVariableOffSet = m_pMagicMem->NonLinearTableOffset + sizeof(TRANSFORM_NONLINEAR)*m_arrTempTransFormNonLinears.size();

	return sizeof VARDATA * m_nSystemVariableCount;
}

size_t CFesDB::BuildUserVariableDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	int nIndex = 0;

	VARDATA* pUserVar = Q_NULLPTR;

	m_pUserVariable = (VARDATA *)((char*)pAddress);

	for (auto i : m_arrTempUserVariables)
	{
		pUserVar = (VARDATA*)((char*)m_pUserVariable + sizeof(VARDATA)*nIndex);

		m_arrUserVariables.push_back(pUserVar);

		std::memcpy(pUserVar, i.get(), sizeof(VARDATA));

		nIndex++;
	}

	m_nUserVariableCount = m_arrUserVariables.size();

	m_pMagicMem->UserVariableCount = m_arrUserVariables.size();

	m_pMagicMem->UserVariableOffset = m_pMagicMem->SystemVariableOffSet + sizeof(VARDATA)*m_arrTempSystemVariables.size();

	return sizeof VARDATA * m_nUserVariableCount;
}

/*! \fn void CFesDB::ClearTempArrays()
********************************************************************************************************* 
** \brief CFesDB::ClearTempArrays 
** \details 强制清除临时文件
** \return void 
** \author xingzhibing
** \date 2017年1月11日 
** \note 
********************************************************************************************************/
void CFesDB::ClearTempArrays()
{
	STLClearObject(&m_arrTempChannels);
	STLClearObject(&m_arrTempDevices);
	STLClearObject(&m_arrTempAins);
	STLClearObject(&m_arrTempDins);
	STLClearObject(&m_arrTempAouts);
	STLClearObject(&m_arrTempDouts);

	STLClearObject(&m_arrChannelOccNos);
	STLClearObject(&m_arrDeviceOccNos);
	STLClearObject(&m_arrAinOccNos);
	STLClearObject(&m_arrDinOccNos);
	STLClearObject(&m_arrAoutOccNos);
	STLClearObject(&m_arrDoutOccNos);

	STLClearObject(&m_arrSystemVariables);
	STLClearObject(&m_arrTempUserVariables);
	STLClearObject(&m_arrSyetemVarOccNos);
	STLClearObject(&m_arrUserVarOccNos);

	STLClearObject(&m_arrTempAinAlarms);
	STLClearObject(&m_arrTempAinAlarmLimits);
	STLClearObject(&m_arrTempDinAlarms);
	STLClearObject(&m_arrTempDinAlarmLimits);
}

size_t CFesDB::EstimateTagNameMemSize()
{
	return m_nTagNameEstimateSize;
}

size_t CFesDB::BuilTagNamedDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}

	size_t nLen = 0;

	//Head
	m_pMagicTagMem = (HEAD_MEM*)(pAddr);
	m_pMagicTagMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicTagMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicTagMem->ShmLength  = m_nTagNameEstimateSize;
	m_pMagicTagMem->MyNodeOccNo = m_nOccNo;
	strncpy(m_pMagicTagMem->Config, m_szProjName.toStdString().c_str(), qMin(MEM_CONFIG_LEN, m_szProjName.length()));
	m_pMagicTagMem->ChannelCount = m_nChannelCount;
	m_pMagicTagMem->DeviceCount = m_nDeviceCount;
	m_pMagicTagMem->AinCount = m_nAinCount;
	m_pMagicTagMem->DinCount = m_nDinCount;
	m_pMagicTagMem->AoutCount = m_nAoutCount;
	m_pMagicTagMem->DoutCount = m_nDoutCount;

	m_pMagicTagMem->LinearCount = 0;
	m_pMagicTagMem->NonLinearCount = 0;

	m_pMagicTagMem->AinAlarmCount = m_nAinAlarmCount;
	m_pMagicTagMem->AinAlarmLimitCount = m_nAinAlarmLimitCount;
	m_pMagicTagMem->DinAlarmCount = m_nDinAlarmCount;
	m_pMagicTagMem->DinAlarmLimitCount = m_nDinAlarmLimitCount;
	m_pMagicTagMem->SystemVariableCount = m_nSystemVariableCount;
	m_pMagicTagMem->UserVariableCount = m_nUserVariableCount;

	pAddr += sizeof HEAD_MEM;

	//channel
	nLen = BuildTagNameDB(pAddr, m_arrChannelOccNos);
	pAddr += nLen;

	//device
	nLen = BuildTagNameDB(pAddr, m_arrDeviceOccNos);
	pAddr += nLen;

	//ain
	nLen = BuildTagNameDB(pAddr, m_arrAinOccNos);
	pAddr += nLen;

	//din
	nLen = BuildTagNameDB(pAddr, m_arrDinOccNos);
	pAddr += nLen;

	//aout
	nLen = BuildTagNameDB(pAddr, m_arrAoutOccNos);
	pAddr += nLen;

	//dout
	nLen = BuildTagNameDB(pAddr, m_arrDoutOccNos);
	pAddr += nLen;

	//ain_alarm
	nLen = BuildTagNameDB(pAddr, m_arrAinAlarmOccNos);
	pAddr += nLen;

	//ainalarmlimit
	nLen = BuildTagNameDB(pAddr, m_arrAinAlarmLimitOccNos);
	pAddr += nLen;

	//dinalarm
	nLen = BuildTagNameDB(pAddr, m_arrDinAlarmOccNos);
	pAddr += nLen;

	//dinalarmlimit
	nLen = BuildTagNameDB(pAddr, m_arrDinAlarmLimitOccNos);
	pAddr += nLen;

	//system 
	nLen = BuildTagNameDB(pAddr, m_arrSyetemVarOccNos);
	pAddr += nLen;

	nLen = BuildTagNameDB(pAddr, m_arrUserVarOccNos);
	pAddr += nLen;

	ClearTempArrays();

	return EstimateTagNameMemSize();
}

size_t CFesDB::BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec)
{
	Q_ASSERT(pAddress && !vec.empty());

	if (pAddress == Q_NULLPTR || vec.empty())
	{
		return 0;
	}

	int  nIndex = 0;

	TAG_OCCNO* pTagOccNo = Q_NULLPTR;

	//基准地址
	TAG_OCCNO * pBaseTagOccNos = reinterpret_cast<TAG_OCCNO*>(pAddress);

	for (auto i : vec)
	{
		pTagOccNo = (TAG_OCCNO*)((char*)pBaseTagOccNos + sizeof TAG_OCCNO * nIndex);

		std::memcpy(pTagOccNo, i.get(), sizeof TAG_OCCNO);

		nIndex++;
	}

	return sizeof TAG_OCCNO * vec.size();
}

/** @}*/
