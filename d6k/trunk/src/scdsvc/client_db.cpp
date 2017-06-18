/*! @file client_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  client_db.cpp
文件实现功能 :  客户端节点的内存数据库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   客户端节点的内存数据库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "client_db.h"
#include "log/log.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QObject> 
#include <QString>  


typedef TRANSFORM_NONLINEAR::LINEAR_ITEM DataItem;

bool SortLFunction(DataItem& data1, DataItem& data2);
/*
{
	return data1.RawValue < data2.RawValue;
}*/


CClientDB::CClientDB( )
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
	m_nTagNameEstimateSize = 0;
}

CClientDB::~CClientDB(void)
{
	
}

/*! \fn bool CClientDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CClientDB::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CClientDB::Initialize(const char *pszDataPath, unsigned int nMode,int32u nOccNo)
{
	QString szLog;

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	m_nOccNo = nOccNo;

	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}

	LogMsg(szLog.toStdString().c_str(), 0);

	if (!ReadScdVarFile(pszDataPath))
	{
		Q_ASSERT(false);
		return false;
	}

	return true;
}

void CClientDB::Run()
{
 
}

void CClientDB::Shutdown()
{
 
}

size_t CClientDB::BuildDB(char * pAddr)
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
	//strncpy(m_pMagicMem->Config, m_szProjName.toStdString().c_str(), qMin(MEM_CONFIG_LEN, m_szProjName.length()));
	m_pMagicMem->AinAlarmCount = m_nAinAlarmCount;
	m_pMagicMem->DinAlarmCount = m_nDinAlarmCount;
	m_pMagicMem->AinAlarmLimitCount = m_nAinAlarmLimitCount;
	m_pMagicMem->DinAlarmLimitCount = m_nDinAlarmLimitCount;
	m_pMagicMem->LinearCount = m_nLinearCount;
	m_pMagicMem->NonLinearCount = m_nNonLinearCount;
	m_pMagicMem->SystemVariableCount = m_nSystemVariableCount;
	m_pMagicMem->UserVariableCount = m_nUserVariableCount;

	pAddr += sizeof HEAD_MEM;

	nLen = BuildAinAlarmDB(pAddr);
	pAddr += nLen;

	nLen = BuildDinAlarmDB(pAddr);
	pAddr += nLen;

	nLen = BuildLinearDB(pAddr);
	pAddr += nLen;

	nLen = BuildNonLinearDB(pAddr);
	pAddr += nLen;

	nLen = BuildSystemVariableDB(pAddr);
	pAddr += nLen;

	nLen = BuildUserVariableDB(pAddr);
	pAddr += nLen;

	return m_nEstimateSize;
}

size_t CClientDB::BuildTagnamedDB(char* pAddr)
{
	Q_ASSERT(pAddr);

	if (!pAddr)
	{
		return 0;
	}
	size_t nLen = 0;

	//Head
	m_pMagicTagNameMem = (HEAD_MEM*)(pAddr);
	m_pMagicTagNameMem->MagicHead1 = MAGIC_HEAD;
	m_pMagicTagNameMem->MagicHead2 = MAGIC_HEAD;
	m_pMagicTagNameMem->ShmLength = m_nTagNameEstimateSize;
	m_pMagicTagNameMem->MyNodeOccNo = m_nOccNo;
	//strncpy(m_pMagicMem->Config, m_szProjName.toStdString().c_str(), qMin(MEM_CONFIG_LEN, m_szProjName.length()));

	m_pMagicTagNameMem->ChannelCount = 0;
	m_pMagicTagNameMem->DeviceCount = 0;
	m_pMagicTagNameMem->AinCount = 0;
	m_pMagicTagNameMem->DinCount = 0;
	m_pMagicTagNameMem->AoutCount = 0;
	m_pMagicTagNameMem->DoutCount = 0;

	m_pMagicTagNameMem->AinAlarmCount = m_nAinAlarmCount;
	m_pMagicTagNameMem->AinAlarmLimitCount = m_nAinAlarmLimitCount;
	m_pMagicTagNameMem->DinAlarmCount = m_nDinAlarmCount;
	m_pMagicTagNameMem->DinAlarmLimitCount = m_nDinAlarmLimitCount;

	m_pMagicTagNameMem->LinearCount = m_nLinearCount;
	m_pMagicTagNameMem->NonLinearCount = m_nNonLinearCount;
	m_pMagicTagNameMem->SystemVariableCount = m_nSystemVariableCount;
	m_pMagicTagNameMem->UserVariableCount = m_nUserVariableCount;

	pAddr += sizeof HEAD_MEM;

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

	//liner
	nLen = BuildTagNameDB(pAddr, m_arrTransLinearOccNos);
	pAddr += nLen;

	//nonliner
	nLen = BuildTagNameDB(pAddr, m_arrTransNonLinearOccNos);
	pAddr += nLen;

	//system
	nLen = BuildTagNameDB(pAddr, m_arrSyetemVarOccNos);
	pAddr += nLen;

	//user
	nLen = BuildTagNameDB(pAddr, m_arrUserVarOccNos);
	pAddr += nLen;

	return  m_nTagNameEstimateSize;
}

bool CClientDB::ReadScdVarFile(const char* pszPath)
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
			if (m_XmlReader.name() == "scada")
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
						if (m_XmlReader.name() == "scale")
						{
							ReadScale();
						}
						else if (m_XmlReader.name() == "alarm")
						{
							ReadAlarm();
						}
						else if (m_XmlReader.name() == "variable")
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
	m_nLinearCount = m_arrTempTransFormLinears.size();

	m_nNonLinearCount = m_arrTempTransFormNonLinears.size();

	m_nAinAlarmCount = m_arrTempAinAlarms.size();

	m_nAinAlarmLimitCount = m_arrTempAinLimitAlarms.size();

	m_nDinAlarmCount = m_arrTempDinAlarms.size();

	m_nDinAlarmLimitCount = m_arrTempDinLimitAlarms.size();

	m_nUserVariableCount = m_arrTempSystemVariables.size();

	m_nSystemVariableCount = m_arrTempUserVariables.size();

	m_nEstimateSize += sizeof HEAD_MEM +
		sizeof TRANSFORM_LINEAR *   m_nLinearCount +
		sizeof TRANSFORM_NONLINEAR* m_nNonLinearCount +
		sizeof AIN_ALARM * m_nAinAlarmCount +
		sizeof AIN_ALARM_LIMIT * m_nAinAlarmLimitCount +
		sizeof DIN_ALARM * m_nDinAlarmCount +
		sizeof DIN_ALARM_LIMIT * m_nDinAlarmLimitCount +
		sizeof VARDATA * m_nUserVariableCount +
		sizeof VARDATA * m_nSystemVariableCount;
	m_nTagNameEstimateSize =  sizeof HEAD_MEM + sizeof TAG_OCCNO *(m_nLinearCount +
		m_nNonLinearCount +
		m_nAinAlarmCount +
		m_nAinAlarmLimitCount +
		m_nDinAlarmCount +
		m_nDinAlarmLimitCount +
		m_nUserVariableCount +
		m_nSystemVariableCount);

	return true;
}

void CClientDB::ReadScale()
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

void CClientDB::ReadLinearInfo()
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
			else if (m_XmlReader.name() == "l")
			{
				pTransLinear = std::make_shared<TRANSFORM_LINEAR>();

				pTransLinear->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pLinearOccNo = std::make_shared<TAG_OCCNO>();

				pLinearOccNo->OccNo = pTransLinear->OccNo;
				std::memset(pLinearOccNo->TagName, 0, sizeof(pLinearOccNo->TagName));

				strncpy(pLinearOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));


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

void CClientDB::ReadNonLinearInfo()
{
	TRANSFORM_NONLINEAR_DEF pTransFormNonLinear = Q_NULLPTR;

	TAG_OCCNO_DEF pNonLinearOccNo = Q_NULLPTR;

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
			else if (m_XmlReader.name() == "nl")
			{
				pTransFormNonLinear = std::make_shared<TRANSFORM_NONLINEAR>();

				pTransFormNonLinear->OccNo = m_XmlReader.attributes().value("OccNo").toInt();

				pNonLinearOccNo = std::make_shared<TAG_OCCNO>();

				pNonLinearOccNo->OccNo = pTransFormNonLinear->OccNo;
				std::memset(pNonLinearOccNo->TagName, 0, sizeof(pNonLinearOccNo->TagName));
				strncpy(pNonLinearOccNo->TagName, m_XmlReader.attributes().value("TagName").toLocal8Bit().data(),
					qMin(size_t(MAX_NAME_LENGTH), (size_t)m_XmlReader.attributes().value("TagName").length()));


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

void CClientDB::ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear)
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

void CClientDB::ReadAlarm()
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

void CClientDB::ReadAinAlarmInfo()
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

void CClientDB::ReadDinAlarmInfo()
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

void CClientDB::ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm)
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
			if (m_XmlReader.name() == "aa")
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

void CClientDB::ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm)
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

void CClientDB::SkipUnknownElement()
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

void CClientDB::ReadVariable()
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
			if (m_XmlReader.name() == "scadasystem")
			{
				ReadSystemVariable();
			}
			else if (m_XmlReader.name() == "scadauser")
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

void CClientDB::ReadSystemVariable()
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

void CClientDB::ReadUserVariable()
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

size_t CClientDB::BuildAlarmDB(char* pAddress)
{
	return 0;
}

size_t CClientDB::BuildAinAlarmDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//创建模拟量告警表
	int nIndex = 0;

	AIN_ALARM* pAinAlarm = Q_NULLPTR;

	m_pAinAlarm = (AIN_ALARM *)((char*)pAddress);

	for (auto i : m_arrTempAinAlarms)
	{
		pAinAlarm = (AIN_ALARM*)((char*)m_pAinAlarm + sizeof(AIN_ALARM)*nIndex);

		m_arrAinAlarms.push_back(pAinAlarm);

		std::memcpy(pAinAlarm, i.get(), sizeof(AIN_ALARM));

		nIndex++;
	}

	m_nAinAlarmCount = m_arrAinAlarms.size();

	m_pMagicMem->AinAlarmCount = m_arrAinAlarms.size();

	//	m_pMagicMem->AinAlarmTableOffset = m_pMagicMem->DeviceTableOffset + sizeof(DEVICE)*m_arrTempDevices.size();

	//创建模拟量限制表
	nIndex = 0;

	pAddress += sizeof AIN_ALARM * m_arrAinAlarms.size();

	AIN_ALARM_LIMIT* pAinLimitAlarm = Q_NULLPTR;

	m_pAinAlarmLimit = (AIN_ALARM_LIMIT *)((char*)pAddress);

	for (auto i : m_arrTempAinLimitAlarms)
	{
		pAinLimitAlarm = (AIN_ALARM_LIMIT*)((char*)m_pAinAlarmLimit + sizeof(AIN_ALARM_LIMIT)*nIndex);

		m_arrAinAlarmLimits.push_back(pAinLimitAlarm);

		std::memcpy(pAinLimitAlarm, i.get(), sizeof(AIN_ALARM_LIMIT));

		nIndex++;
	}

	m_nAinAlarmLimitCount = m_arrTempAinLimitAlarms.size();

	m_pMagicMem->AinAlarmLimitCount = m_arrTempAinLimitAlarms.size();

	m_pMagicMem->AinAlarmTableOffset = m_pMagicMem->AinAlarmTableOffset + sizeof(AIN_ALARM)*m_arrAinAlarms.size();

	//offset
	return sizeof AIN_ALARM * m_arrTempAinAlarms.size() + sizeof AIN_ALARM_LIMIT* m_arrTempAinLimitAlarms.size();
}

size_t CClientDB::BuildDinAlarmDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//创建开关量告警
	int nIndex = 0;

	DIN_ALARM* pDinAlarm = Q_NULLPTR;

	m_pDinAlarm = (DIN_ALARM *)((char*)pAddress);

	for (auto i : m_arrTempDinAlarms)
	{
		pDinAlarm = (DIN_ALARM*)((char*)m_pDinAlarm + sizeof(DIN_ALARM)*nIndex);

		m_arrDinAlarms.push_back(pDinAlarm);

		std::memcpy(pDinAlarm, i.get(), sizeof(DIN_ALARM));

		nIndex++;
	}

	m_nDinAlarmCount = m_arrTempDinAlarms.size();

	m_pMagicMem->DinAlarmCount = m_arrTempDinAlarms.size();

	m_pMagicMem->DinAlarmTableOffset = m_pMagicMem->AinAlarmLimitTableOffset + sizeof(AIN_ALARM)*m_arrAinAlarms.size();

	//创建开关量限值告警

	pAddress += sizeof DIN_ALARM* m_arrTempDinAlarms.size();

	nIndex = 0;

	DIN_ALARM_LIMIT* pDinAlarmLinit = Q_NULLPTR;

	m_pDinAlarmLimit = (DIN_ALARM_LIMIT*)(char*)(pAddress);

	for (auto i : m_arrTempDinLimitAlarms)
	{
		pDinAlarmLinit = (DIN_ALARM_LIMIT*)((char*)m_pDinAlarmLimit + sizeof(DIN_ALARM_LIMIT)*nIndex);

		m_arrDinAlarmLimits.push_back(pDinAlarmLinit);

		std::memcpy(pDinAlarmLinit, i.get(), sizeof(DIN_ALARM_LIMIT));

		nIndex++;
	}

	m_nDinAlarmLimitCount = m_arrDinAlarmLimits.size();

	m_pMagicMem->DinAlarmLimitCount = m_arrDinAlarmLimits.size();

	m_pMagicMem->DinAlarmLimtTableOffset = m_pMagicMem->DinAlarmTableOffset + sizeof(DIN_ALARM)*m_arrDinAlarms.size();

	//offset
	return sizeof DIN_ALARM * m_arrTempDinAlarms.size() + sizeof  DIN_ALARM_LIMIT * m_arrTempDinLimitAlarms.size();
}

size_t CClientDB::BuildLinearDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	int nIndex = 0;
	TRANSFORM_LINEAR* pTransLinear = Q_NULLPTR;

	m_pLinear = (TRANSFORM_LINEAR *)((char*)pAddress);

	for (auto i : m_arrTempTransFormLinears)
	{
		pTransLinear = (TRANSFORM_LINEAR*)((char*)m_pLinear + sizeof(TRANSFORM_LINEAR)*nIndex);

		m_arrLinears.push_back(pTransLinear);

		std::memcpy(pTransLinear, i.get(), sizeof(TRANSFORM_LINEAR));

		nIndex++;
	}

	m_nLinearCount = m_arrTempTransFormLinears.size();

	m_pMagicMem->LinearCount = m_arrTempTransFormLinears.size();

	//m_pMagicMem->LinearTableOffset = m_pMagicMem->DountTableOffset + sizeof(AOUT)*m_arrTempDouts.size();

	return sizeof TRANSFORM_LINEAR * m_arrTempTransFormLinears.size();
}

size_t CClientDB::BuildNonLinearDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	int nIndex = 0;

	TRANSFORM_NONLINEAR* pTransNonLinear = Q_NULLPTR;

	m_pNonLinear = (TRANSFORM_NONLINEAR *)((char*)pAddress);

	for (auto i : m_arrTempTransFormNonLinears)
	{
		pTransNonLinear = (TRANSFORM_NONLINEAR*)((char*)m_pNonLinear + sizeof(TRANSFORM_NONLINEAR)*nIndex);

		m_arrNonLinears.push_back(pTransNonLinear);

		std::memcpy(pTransNonLinear, i.get(), sizeof(TRANSFORM_NONLINEAR));

		nIndex++;
	}

	m_nNonLinearCount = m_arrTempTransFormNonLinears.size();

	m_pMagicMem->NonLinearCount = m_arrTempTransFormNonLinears.size();

	m_pMagicMem->NonLinearTableOffset = m_pMagicMem->LinearTableOffset + sizeof(TRANSFORM_LINEAR)*m_arrTempTransFormLinears.size();

	return sizeof TRANSFORM_NONLINEAR * m_arrTempTransFormNonLinears.size();
}

size_t CClientDB::BuildSystemVariableDB(char* pAddress)
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

	return sizeof VARDATA * m_arrTempSystemVariables.size();
}

size_t CClientDB::BuildUserVariableDB(char* pAddress)
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

	return sizeof VARDATA * m_arrTempUserVariables.size();
}



size_t CClientDB::BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec)
{
	Q_ASSERT(pAddress);

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
