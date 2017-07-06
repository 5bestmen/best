#include "feschannel_forwardingchannel_data.h"
#include "scadastudio/icore.h"
#include "variant.h"
#include "global.h"
#include "fes_conf_data.h"
#include "log/log2file.h"

#include "stl_util-inl.h"

#include <QObject>
#include "qxmlstream.h"
#include <QModelIndex>
#include "qxmlstream.h"

namespace Config
{
	CForwardingAnalogOutputData::CForwardingAnalogOutputData()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_nOccNo = 0;
		m_nBlockNo = 0;


		m_nGroupNo = 0;
		m_nEntryNo = 0;

		m_strAddress = "";

		m_strSourceTagName = "";

		m_strDescription = "";
		m_strFUN = "";
		m_strInfo = "";
		m_nDescriptionOccNo = 0;

		m_dbMaxRaw = 10000;
		m_dbMinRaw = 0;
		m_dbMaxConvert = 10000;
		m_dbMinConvert = 0;
	}

	CForwardingControlGearData::CForwardingControlGearData()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_nOccNo = 0;
		m_nBlockNo = 0;

		m_nGroupNo = 0;
		m_nEntryNo = 0;

		m_strSourceTagName = "";

		m_strAddress = "";

		m_strDescription = "";
		m_strFUN = "";
		m_strInfo = "";
		m_nDescriptionOccNo = 0;
	}

	CForwardingControlGearData::~CForwardingControlGearData()
	{

	}

	CForwardingAnalogOutputData::~CForwardingAnalogOutputData()
	{
	}

	CForwardingKWHData::CForwardingKWHData()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_nOccNo = 0;
		m_nBlockNo = 0;
		m_strDescription = "";
		m_strFUN = "";
		m_strInfo = "";
		m_nDescriptionOccNo = 0;

		m_nGroupNo = 0;		//组号
		m_nEntryNo = 0;		//条目号

		m_strAddress = "";	//地址

		m_strSourceTagName = "";

		m_nChannelOccNo = 0;
	}

	CForwardingKWHData::~CForwardingKWHData()
	{

	}

	CForwardingChannelGroup::CForwardingChannelGroup()
	{
		m_arrGroup.clear();
		m_arrItem.clear();
	}

	CForwardingChannelGroup::~CForwardingChannelGroup()
	{
		for each (auto var in m_arrGroup)
		{
			delete var.second;
		}

		for each (auto var in m_arrItem)
		{
			delete var.second;
		}
	}

	CForwardingChannelGroup * CForwardingChannelGroup::CreateGroup()
	{
		int nIndex = (int)m_arrGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = m_arrGroup.find(strName.toStdString());
			if (it == m_arrGroup.end())
			{
				CForwardingChannelGroup *pGroup = new CForwardingChannelGroup;
				pGroup->SetGroupName(strName);

				m_arrGroup.insert(std::make_pair(strName.toStdString(), pGroup));

				return pGroup;
			}
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}

	/*! \fn bool CForwardingChannelGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelGroup::ModifyItem
	** \details 修改组内项arr hash
	** \param strTagName
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2017年3月23日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	{
		auto bFlag = strTagName.isEmpty() || strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strLastTagName.isEmpty() || strLastTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			auto strError = QObject::tr("Forwarding channel tagname is invalid!!!");
			LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

			return false;
		}

		auto ite = m_arrItem.find(strLastTagName.toStdString());
		if (ite != m_arrItem.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			memset(ite->second->m_szTagName, 0, sizeof(ite->second->m_szTagName));
			int nSize = strTagName.size();
			strncpy(ite->second->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			std::swap(m_arrItem[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrItem.erase(ite);

			return true;
		}

		return false;
	}

	bool CForwardingChannelGroup::AddItem(std::string &strTagName, CForwardingChannelData *pItem)
	{
		auto bFlag = strTagName.size() > MAX_TAGNAME_LEN_SCADASTUDIO || strTagName.empty();
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		Q_ASSERT(pItem);
		if (!pItem)
		{
			return false;
		}

		m_arrItem.insert(make_pair(strTagName, pItem));

		return true;
	}

	bool CForwardingChannelGroup::DeletGroup(const QString &strTagName)
	{
		auto it = m_arrGroup.find(strTagName.toStdString());
		if (it == m_arrGroup.end())
		{
			return false;
		}

		delete it->second;

		m_arrGroup.erase(it);

		return true;
	}

	/*! \fn bool CForwardingChannelGroup::DeleteItem(const QString &strTagName)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelGroup::DeleteItem
	** \details 删除组内的项
	** \param strTagName
	** \return bool
	** \author gw
	** \date 2017年4月9日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelGroup::DeleteItem(const QString &strTagName)
	{
		auto nSize = strTagName.size();
		auto bFlag = nSize > MAX_TAGNAME_LEN_SCADASTUDIO || nSize == 0;
		Q_ASSERT(!bFlag);
		if (bFlag)
		{
			return false;
		}

		auto it = m_arrItem.find(strTagName.toStdString());
		if (it == m_arrItem.end())
		{
			return false;
		}
		else
		{
			delete it->second;
			m_arrItem.erase(it);
		}

		return true;
	}

	CForwardingAnalogInputData::CForwardingAnalogInputData()
	{
		m_nID = 0;
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);

		m_strSourceTagName = "";

		m_strDescription = "";
		m_bModifyFlag = false;
		m_nOccNo = 0;
		m_nBlockNo = 0;

		m_nGroupNo = 0;
		m_nEntryNo = 0;

		m_nDescriptionOccNo;

		m_strFUN = "";
		m_strInfo = "";

		m_dblMaxRaw = 10000;
		m_dblMinRaw = 0;
		m_dblMaxConvert = 10000;
		m_dblMinConvert = 0;

		m_strAddress = "";

		m_strScaleTagName = "";
		m_nScaleType = ScaleType::LINEAR;
		m_dblHLimit = 0;
		m_dblLLimit = 0;

		m_dblThreshold = 0;

		m_nChannelOccNo = 0;
	}

	CForwardingAnalogInputData::~CForwardingAnalogInputData()
	{

	}

	CForwardingDigitalInputData::CForwardingDigitalInputData()
	{
		m_nID = 0;
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);

		m_strSourceTagName = "";

		m_strDescription = "";
		m_bModifyFlag = false;
		m_nOccNo = 0;
		m_nBlockNo = 0;
		m_nDescriptionOccNo;

		m_nGroupNo = 0;
		m_nEntryNo = 0;

		m_strAddress = "";

		m_strFUN = "";
		m_strInfo = "";
		m_bInverse = false;

		m_nChannelOccNo = 0;
	}

	CForwardingDigitalInputData::~CForwardingDigitalInputData()
	{

	}

	CForwardingDigitalOutputData::CForwardingDigitalOutputData()
	{
		memset(m_szTagName, 0, sizeof(m_szTagName));
		m_nID = 0;
		m_nOccNo = 0;
		m_nBlockNo = 0;
		m_strDescription = "";
		m_strFUN = "";
		m_strInfo = "";
		m_nDescriptionOccNo = 0;

		m_nGroupNo = 0;
		m_nEntryNo = 0;

		m_strAddress = "";

		memset(m_szAssociatedDI, 0, sizeof(m_szAssociatedDI));
		memset(m_szBlockingSignalTag, 0, sizeof(m_szAssociatedDI));
		m_bBlockingSignalCondition = false;

		m_strSourceTagName = "";
	}

	CForwardingDigitalOutputData::~CForwardingDigitalOutputData()
	{

	}

	/*! \fn CAnalogInput::CAnalogInput()
	*********************************************************************************************************
	** \brief CAnalogInput::CAnalogInput
	** \details 构造函数初始化
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CAnalogInput::CAnalogInput()
	{
		m_nID = 1;
		memset(m_szTagName, MAX_NAME_LENGTH_SCADASTUDIO + STRING_PAD, 0);
		m_strDescription = " ";
		m_nChannelOccNo = 0;
		m_nDeviceOccNo = 0;
		m_nBlockOccNo = 0;
		m_nOccNo = 0;
		m_strAddress = "";
		//m_nDataType = VariableDataType::DoubleType;
		m_dbInitValue = 0;
		m_nInitialQua = NormalQua;
		m_strPinLabel = "";
		m_dblSensitivity = 0.5;
		m_nSensitivityType = 0;
		m_nReferenceCount = 0;
		m_nDataSource = 0;
		m_bSaveDB = false;
		m_nSaveDBPeriod = 0;
		m_bSaveDisk = false;
		m_nSaveDiskPeriod = 0;

		m_strScaleTagName = "";
		m_nScaleType = LINEAR;
		m_strScaleDesc = "";
		m_nScaleOccNo = 0;
		m_nAlarmType = AIALARM;
		m_nAlarmOccNo = 0;
		m_strAlarmDesc = "";
		m_dblRangeL = 0;
		m_dblRangeH = 0;
		m_dblHighQua = 0;
		m_dblLowQua = 0;
		m_strUnit = "";
		m_strExpress = "";

		m_bNewPointScale = true;
		m_bNewPointAlarm = true;

		m_dbMaxRaw = 10000;
		m_dbMinRaw = 0;
		m_dbMaxConvert = 10000;
		m_dbMinConvert = 0;

		m_dblRangeL = 0;
		m_dblRangeH = 10000;
		m_dblLowQua = -1.2;
		m_dblHighQua = 1.2;				//*LowQua  通常为-1.2  HighQua 通常为1.2
										/*OutputL = RangeH + (RangeH - RangeL) * LowQua; （有效值范围低值）
										OutputH = RangeL + (RangeH - RangeL) * HighQua; （有效值范围高值）
										如果在此有效值范围外，则认为测值品质不可信。
										通常来说：
										OutputH > RangeH > HH > H*/


		m_bEnable = true;

	}
	/*! \fn CAnalogInput::~CAnalogInput()
	*********************************************************************************************************
	** \brief CAnalogInput::~CAnalogInput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CAnalogInput::~CAnalogInput()
	{

	}
	/*! \fn void CAnalogInput::Save()
	*********************************************************************************************************
	** \brief CAnalogInput::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CAnalogInput::Save()
	{

	}
	/*! \fn CDigitalInput::CDigitalInput()
	*********************************************************************************************************
	** \brief CDigitalInput::CDigitalInput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDigitalInput::CDigitalInput()
	{
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		memset(m_szDoubleDI, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_nID = 0;
		m_nBlockOccNo = 0;
		//m_nDataType = 0;					//bit
		m_strDescription = "";



		m_bInitValue = false;
		m_nInitialQua = 0;
		m_strAddress = "";
		m_strPinLabel = "";
		m_nSignalType = 0;		//常开 常闭
		m_bSOE = false;
		m_strBitCloseString = "";
		m_strBitOpenString = "";

		m_nAlarmType = DIALARM;
		m_strAlarmTag = "";
		m_strAlarmDesc = "";
		m_nAlarmOccNo = 0;

		m_bSaveDisk = false;
		m_nSaveDiskPeriod = 0;
		m_bSaveDB = false;
		m_nSaveDBPeriod = 0;

		m_nReferenceCount = 0;

		m_nChannelOccNo = 0;
		m_nDeviceOccNo = 0;


		m_nDataSource = 0;
		m_strExpress = "";

		m_bNewPointAlarm = false;
		m_nOccNo = 0;
	}
	/*! \fn CDigitalInput::~CDigitalInput()
	*********************************************************************************************************
	** \brief CDigitalInput::~CDigitalInput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDigitalInput::~CDigitalInput()
	{

	}
	/*! \fn void CDigitalInput::Save()
	*********************************************************************************************************
	** \brief CDigitalInput::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDigitalInput::Save()
	{

	}
	/*! \fn CAnalogOutput::CAnalogOutput()
	*********************************************************************************************************
	** \brief CAnalogOutput::CAnalogOutput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CAnalogOutput::CAnalogOutput()
	{
		m_nID = 0;
		//m_szTagName = "";
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_strDescription = "";
		m_nBlockOccNo = 0;
		m_nDataType = DATA_TYPE::DT_DOUBLE;
		m_dbInitValue = 100;
		m_nInitialQua = 0;
		m_strAddress = "";
		m_strPinLabel = "";
		m_nReferenceCount = 0;
		m_nChannelOccNo = 0;
		m_nDeviceOccNo = 0;
		m_nScaleType = 1;
		m_dblRangeL = 0;
		m_dblRangeH = 10000;
		m_dblHighQua = 1.2;
		m_dblLowQua = -1.2;
		m_bNewPointAlarm = true;
		m_bNewPointAlarm = true;

		m_bEnable = true;
		m_dbMinRaw = 0;
		m_dbMaxRaw = 10000;
		m_dbMaxConvert = 0;
		m_dbMinConvert = 10000;

		m_strScaleTagName = "";
		m_strScaleDesc = "";
		m_nScaleType = ScaleType::LINEAR;

		m_nReferenceCount = 0;
	}
	/*! \fn CAnalogOutput::~CAnalogOutput()
	*********************************************************************************************************
	** \brief CAnalogOutput::~CAnalogOutput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CAnalogOutput::~CAnalogOutput()
	{

	}
	/*! \fn void CAnalogOutput::Save()
	*********************************************************************************************************
	** \brief CAnalogOutput::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CAnalogOutput::Save()
	{

	}
	/*! \fn CDigtalOutput::CDigtalOutput()
	*********************************************************************************************************
	** \brief CDigtalOutput::CDigtalOutput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDigitalOutput::CDigitalOutput()
	{
		m_nID = 0;
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		memset(m_szAssociatedDI, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		memset(m_szBlockingSignalTag, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_strDescription = "";
		m_bModifyFlag = false;
		m_bNewPointAlarm = false;
		m_bInitValue = 0;
		m_nInitialQua = 0;
		m_nOccNo = 0;
		m_nReferenceCount = 0;
		m_strAddress = "";
		m_strPinLabel = "";

		m_dblTelCtlExecTime = 0;
		m_dblTelCtlSendTime = 0;
		m_dblTelCtlWaitTime = 0;

		m_nChannelOccNo = 0;
		m_nDeviceOccNo = 0;
		m_nBlockOccNo = 0;

		m_nDataType = DT_BOOLEAN;
		m_nAssociatedDIValType = 0;
	}
	/*! \fn CDigtalOutput::~CDigtalOutput()
	*********************************************************************************************************
	** \brief CDigtalOutput::~CDigtalOutput
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDigitalOutput::~CDigitalOutput()
	{

	}
	/*! \fn void CDigtalOutput::Save()
	*********************************************************************************************************
	** \brief CDigtalOutput::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDigitalOutput::Save()
	{

	}
	/*! \fn CChannelData::CChannelData()
	*********************************************************************************************************
	** \brief CChannelData::CChannelData
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CChannelData::CChannelData()
		:CBaseData()
	{
		m_nID = 0;
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_strDescription = "";
		m_bModifyFlag = false;
		m_strExtention = "";
		m_nOccNo = 0;
	}

	/*! \fn CChannelData::~CChannelData()
	*********************************************************************************************************
	** \brief CChannelData::~CChannelData
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CChannelData::~CChannelData()
	{
		Clear();
	}
	/*! \fn void CChannelData::Save()
	*********************************************************************************************************
	** \brief CChannelData::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CChannelData::Save()
	{

	}

	void CChannelData::Clear()
	{
		for (size_t i = 0; i < m_arrDevices.size(); i++)
		{
			delete m_arrDevices[i];
		}
		m_arrDevices.clear();
	}
	/*! \fn void CChannelData::GetDevice(CDevice *pDevice)
	*********************************************************************************************************
	** \brief CChannelData::GetDevice
	** \details
	** \param pDevice
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CChannelData::AddDevice(CDevice *pDevice)
	{
		Q_ASSERT(pDevice);
		if (pDevice == nullptr)
			return;

		m_arrDevices.push_back(pDevice);
	}


	bool CChannelData::DeleteDevice(CDevice *pDevice)
	{
		Q_ASSERT(pDevice);
		if (pDevice == nullptr)
			return false;

		auto it = std::find(m_arrDevices.begin(), m_arrDevices.end(), pDevice);
		if (it != m_arrDevices.end())
		{
			delete pDevice;
			pDevice = nullptr;

			m_arrDevices.erase(it);

			return true;
		}

		return false;
	}

	/*! \fn bool CChannelData::LoadDevicessData(QXmlStreamReader &reader, std::unordered_map<std::string, CBaseData *> &arrTotalFesTag,
	std::unordered_map<std::string, CBaseData *> &arrDeviceTagName)
	*********************************************************************************************************
	** \brief Config::CChannelData::LoadDevicessData
	** \details 加载装置数据
	** \param reader
	** \param  fes all hash 主要检查tagname重复
	** \param arrTotalFesTag
	** \param  前置device hash
	** \param arrDeviceTagName
	** \return bool
	** \author gw
	** \date 2016年10月25日
	** \note
	********************************************************************************************************/
	bool CChannelData::LoadDevicessData(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "device")
				{
					//ReadD(reader);
				}
				else if (strTmp == "d")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("device OccNo is invalid!!!");
						s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					//Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					//if (strTagName.isEmpty() == true)
					//{
					//	auto strError = QObject::tr("device tag name is empty!!!!!!");

					//	s_pGlobleCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

					//	reader.readNext();

					//	continue;
					//}
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						reader.readNext();

						continue;
					}

					CDevice *pDevice = new CDevice(s_pGlobleCore);
					pDevice->SetOccNo(nOccNo);
					//pDevice->m_szTagName = strTagName;
					memset(pDevice->m_szTagName, 0, sizeof(pDevice->m_szTagName));
					int nSize = strTagName.size();
					strncpy(pDevice->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDevice->m_nID = nID;

					QString strDescription = reader.attributes().value("Description").toString();
					pDevice->m_strDescription = strDescription;

					int32u DeviceModelID = reader.attributes().value("DeviceModelID").toUInt();
					pDevice->m_nDeviceModelID = DeviceModelID;

					int8u nType = reader.attributes().value("Type").toInt();
					pDevice->m_nType = nType;

					QString strDeviceAddr = reader.attributes().value("DeviceAddr").toString();
					pDevice->m_strDeviceAddr = strDeviceAddr;

					int32u nPriority = reader.attributes().value("Priority").toInt();
					pDevice->m_nPriority = nPriority;

					QString strExtent = reader.attributes().value("ExtentionParam").toString();
					pDevice->m_strExtentionParam = strExtent;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						//auto strFesError = QString(QObject::tr("ID %1 of device %2 is invalid < 0")).arg(nID).arg(strTagName);
						//s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						//auto strFesError = QString(QObject::tr("device %1 is empty")).arg(strTagName);
						//s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_NAME_LENGTH_SCADASTUDIO)
					{
						//auto strFesError = QString(QObject::tr("device %1 size is more than %2 ")).arg(strTagName).arg(MAX_NAME_LENGTH_SCADASTUDIO);
						//s_pGlobleCore->LogMsg(DEVICE_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (pFesData->CheckTagNameIsExist(pDevice->m_szTagName))
					{
						reader.readNext();

						return false;
					}


					pDevice->LoadData(reader, pFesData);

					//通道挂装置
					m_arrDevices.push_back(pDevice);

					pFesData->PushFesTagNameHashMap(DEVICEHASHID, pDevice->m_szTagName, pDevice);
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
	bool CChannelData::SaveChannelData(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nDOOccNo
		, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		nOccNoChannel++;
		Q_ASSERT(nOccNoChannel);
		writer.writeStartElement("c");
		writer.writeAttribute("OccNo", QString("%1").arg(nOccNoChannel));
		writer.writeAttribute("TagName", QString("%1").arg(m_szTagName));
		if (!CheckTagNameIsValid(m_szTagName, FES_DESC))
		{
			auto strTmp = QString("-->Fes TagName %1  Channel TagName %2 count is invalid").arg(pFes->m_szTagName).arg(m_szTagName);
			MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), true);
		}
		writer.writeAttribute("ID", QString("%1").arg(m_nID));
		writer.writeAttribute("Description", QString("%1").arg(m_strDescription));
		//字符串内存池
		const auto it = pHash->find(m_strDescription.toStdString());
		if (it == pHash->end())
		{
			*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			pHash->insert(std::make_pair(m_strDescription.toStdString(), *pDescStringPoolOccNo));
			pStringPoolVec->push_back(m_strDescription.toStdString());

			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
		}
		else
		{
			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
		}

		//pHash->insert(m_strDescription.toStdString());


		writer.writeAttribute("DriverName", QString("%1").arg(m_CommPara.DriverName));
		writer.writeAttribute("DriverPara", QString("%1").arg(m_CommPara.DriverName));
		writer.writeAttribute("Medium", QString("%1").arg(m_CommPara.Medium));
		/*writer.writeAttribute("IP", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.IP));
		writer.writeAttribute("Port", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Port));
		writer.writeAttribute("COM", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Com));
		writer.writeAttribute("Baud", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Baud));*/
		writer.writeAttribute("ChannelPara", QString("%1").arg(m_strChannelPara));
		writer.writeAttribute("Extention", QString("%1").arg(m_strExtention));
		writer.writeAttribute("GroupName", QString("%1").arg(m_strGroupName));

		//SaveDevice(writer, pFes);
		///////////////////////device///////////////////////////////////////////
		writer.writeStartElement("device");
		
		writer.writeAttribute("Count", QString("%1").arg(m_arrDevices.size()));
		auto strTmp = QString("-->Fes TagName %1  Channel TagName %2 device count is %3").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrDevices.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);

		for (auto const &var : m_arrDevices)
		{
			var->SaveDeviceData(writer, nOccNoChannel, nOccNoDevice, nAIOccNo, nDIOccNo, nAOOccNo, nDOOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		}
		writer.writeEndElement();
		//////////////////////////////////////////////////////////////////

		writer.writeEndElement();

		return true;
	}

	bool CChannelData::LogToFile(const char* pFilename, const char* pLog)
	{
		MYLIB::Log2File(pFilename, pLog, true);
		std::sort(m_arrDevices.begin(), m_arrDevices.end());

		//装置
		for each (auto var in m_arrDevices)
		{
			auto strTmp = QString("[Start Device %1 Log Success!!!]").arg(var->m_szTagName);
		}

		return true;
	}

	/*! \fn CChannelGroup::CChannelGroup()
	*********************************************************************************************************
	** \brief Config::CChannelGroup::CChannelGroup
	** \details 通道组
	** \return
	** \author gw
	** \date 2016年10月20日
	** \note
	********************************************************************************************************/
	CChannelGroup::CChannelGroup()
	{

	}

	CChannelGroup::~CChannelGroup()
	{
		auto it = m_arrChannelGroup.begin();
		for (; it != m_arrChannelGroup.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
		m_arrChannelGroup.clear();

		auto ite = m_arrChannelItem.begin();
		for (; ite != m_arrChannelItem.end(); ++ite)
		{
			delete ite->second;
			ite->second = nullptr;
		}
		m_arrChannelItem.clear();
	}
	/*! \fn bool CChannelGroup::CreateGroup(CChannelGroup *pChanneGroup)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::CreateGroup
	** \details
	** \param pChanneGroup
	** \return bool
	** \author gw
	** \date 2016年10月20日
	** \note
	********************************************************************************************************/
	CChannelGroup * CChannelGroup::CreateGroup()
	{
		int nIndex = (int)m_arrChannelGroup.size();
		QString strName = "";
		while (true)
		{
			strName = QString("%1%2").arg(GROUP_NAME).arg(nIndex);

			auto it = m_arrChannelGroup.find(strName.toStdString());
			if (it == m_arrChannelGroup.end())
			{
				CChannelGroup *pGroup = new CChannelGroup;
				pGroup->m_strGroup = strName;

				m_arrChannelGroup.insert(std::make_pair(strName.toStdString(), pGroup));

				return pGroup;
			}
			else
			{
				nIndex++;
			}
		}

		return nullptr;
	}


	/*! \fn bool CChannelGroup::ModifyGroup(const QString &strTagName, const QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::ModifyGroup
	** \details
	** \param strTagName
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2016年10月20日
	** \note
	********************************************************************************************************/
	bool CChannelGroup::ModifyItem(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrChannelItem.find(strLastTagName.toStdString());
		if (ite != m_arrChannelItem.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			memset(ite->second->m_szTagName, 0, sizeof(ite->second->m_szTagName));
			int nSize = strTagName.size();
			strncpy(ite->second->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));
			std::swap(m_arrChannelItem[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrChannelItem.erase(ite);

			return true;
		}

		//auto it = m_arrChannelGroup.begin();
		//for (; it != m_arrChannelGroup.end(); ++it)
		//{
		//	if (it->second->ModifyItem(strTagName, strLastTagName))
		//	{
		//		return true;
		//	}
		//}

		return false;
	}
	/*! \fn bool CChannelGroup::DeleteItem(QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::DeleteItem
	** \details
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2016年10月24日
	** \note
	********************************************************************************************************/
	bool CChannelGroup::DeleteItem(QString &strTagName)
	{
		auto it = m_arrChannelItem.find(strTagName.toStdString());
		if (it == m_arrChannelItem.end())
		{
			return false;
		}
		else
		{
			delete it->second;
			m_arrChannelItem.erase(it);
		}



		return true;
	}


	/*! \fn bool CChannelGroup::DeleteGroup(QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::DeleteGroup
	** \details
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2016年10月24日
	** \note
	********************************************************************************************************/
	bool CChannelGroup::DeleteGroup(QString &strLastTagName)
	{
		auto it = m_arrChannelGroup.find(strLastTagName.toStdString());
		if (it == m_arrChannelGroup.end())
		{
			return false;
		}

		delete it->second;

		m_arrChannelGroup.erase(it);

		return true;
	}


	/*! \fn bool CChannelGroup::CheckModifyGroupNameExist(CFesGroup *pFesGroup, QString strGroupName)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::CheckModifyGroupNameExist
	** \details 检查tagname在同级通道组是否存在
	** \param pFesGroup
	** \param strGroupName
	** \return bool
	** \author gw
	** \date 2016年10月28日
	** \note
	********************************************************************************************************/
	bool CChannelGroup::CheckModifyGroupNameExist(CChannelGroup *pChannelGroup, const QString &strGroupName)
	{
		for (auto it = m_arrChannelGroup.begin(); it != m_arrChannelGroup.end(); ++it)
		{
			if (it->second != pChannelGroup)
			{
				if (it->second->m_strGroup == strGroupName)
				{
					return true;
				}
			}
		}

		return false;
	}


	/*! \fn bool CChannelGroup::ModifyGroup(const QString &strTagName, const QString &strLastTagName)
	*********************************************************************************************************
	** \brief Config::CChannelGroup::ModifyGroup
	** \details 修改组名
	** \param strTagName
	** \param strLastTagName
	** \return bool
	** \author gw
	** \date 2016年10月28日
	** \note
	********************************************************************************************************/
	bool CChannelGroup::ModifyGroup(const QString &strTagName, const QString &strLastTagName)
	{
		auto ite = m_arrChannelGroup.find(strLastTagName.toStdString());
		if (ite != m_arrChannelGroup.end())
		{
			// Swap value from oldKey to newKey, note that a default constructed value 
			// is created by operator[] if 'm' does not contain newKey.

			ite->second->m_strGroup = strTagName;
			std::swap(m_arrChannelGroup[strTagName.toStdString()], ite->second);
			// Erase old key-value from map
			m_arrChannelGroup.erase(ite);

			return true;
		}

		//auto it = m_arrFesGroup.begin();
		//for (; it != m_arrFesGroup.end(); ++it)
		//{
		//	if (it->second->ModifyGroup(strTagName, strLastTagName))
		//	{
		//		return true;
		//	}
		//}

		return false;
	}

	CForwardingChannelData::CForwardingChannelData()
	{
		m_nID = 0;
		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_strDescription = "";
		m_bModifyFlag = false;
		m_strExtention = "";
		m_nOccNo = 0;

		m_arrAIs.clear();
		m_arrDIs.clear();
		m_arrDOs.clear();
		m_arrAOs.clear();
		m_arrCGs.clear();
		m_arrKWHs.clear();
	}


	CForwardingChannelData::~CForwardingChannelData()
	{
		STLDeleteElements(&m_arrAIs);
		STLDeleteElements(&m_arrDIs);
		STLDeleteElements(&m_arrAOs);
		STLDeleteElements(&m_arrCGs);
		STLDeleteElements(&m_arrDOs);
		STLDeleteElements(&m_arrKWHs);
	}

	void CForwardingChannelData::Save()
	{

	}

	/*! \fn bool CForwardingChannelData::SaveForwardingChannelData(QXmlStreamWriter & writer, int & nOccNoChannel, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveForwardingChannelData
	** \details 保存转发通道
	** \param writer
	** \param nOccNoChannel
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月13日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveForwardingChannelData(QXmlStreamWriter & writer, int & nOccNoChannel, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nCGOccNo, int &nDOOccNo, int &nKwhOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		nOccNoChannel++;
		Q_ASSERT(nOccNoChannel);
		writer.writeStartElement("forward_c");
		writer.writeAttribute("OccNo", QString("%1").arg(nOccNoChannel));
		writer.writeAttribute("TagName", QString("%1").arg(m_szTagName));
		if (!CheckTagNameIsValid(m_szTagName, FES_DESC))
		{
			auto strTmp = QString("-->Fes %1  Forwarding Channel TagName %2 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName);
			MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
		}

		writer.writeAttribute("ID", QString("%1").arg(m_nID));
		writer.writeAttribute("Description", QString("%1").arg(m_strDescription));
		//字符串内存池
		const auto it = pHash->find(m_strDescription.toStdString());
		if (it == pHash->end())
		{
			*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			pHash->insert(std::make_pair(m_strDescription.toStdString(), *pDescStringPoolOccNo));
			pStringPoolVec->push_back(m_strDescription.toStdString());

			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
		}
		else
		{
			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
		}

		//pHash->insert(m_strDescription.toStdString());


		writer.writeAttribute("DriverName", QString("%1").arg(m_CommPara.DriverName));
		writer.writeAttribute("DriverPara", QString("%1").arg(m_CommPara.DriverName));
		writer.writeAttribute("Medium", QString("%1").arg(m_CommPara.Medium));
		/*writer.writeAttribute("IP", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.IP));
		writer.writeAttribute("Port", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Port));
		writer.writeAttribute("COM", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Com));
		writer.writeAttribute("Baud", QString("%1").arg(pFes->m_arrChannels[i]->m_CommPara.Baud));*/
		writer.writeAttribute("ChannelPara", QString("%1").arg(m_strChannelPara));
		writer.writeAttribute("Extention", QString("%1").arg(m_strExtention));
		writer.writeAttribute("GroupName", QString("%1").arg(m_strGroupName));

		SaveAI(writer, nOccNoChannel, nAIOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveDI(writer, nOccNoChannel, nDIOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveAO(writer, nOccNoChannel, nAOOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveCG(writer, nOccNoChannel, nCGOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveDO(writer, nOccNoChannel, nDOOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);
		SaveKwh(writer, nOccNoChannel, nKwhOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();

		return true;
	}


	/*! \fn bool CForwardingChannelData::SaveAI(QXmlStreamWriter & writer, int & nOccNoChannel, int & nAIOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveAI
	** \details 保存转发AIs
	** \param writer
	** \param nOccNoChannel
	** \param nAIOccNo
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月15日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveAI(QXmlStreamWriter & writer, int & nOccNoChannel, int & nAIOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		writer.writeStartElement("Forwarding_AIN");
		writer.writeAttribute("Count", QString("%1").arg(m_arrAIs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 AIs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrAIs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);


		Q_ASSERT(nAIOccNo + 1 > 0);
		int nBlockNo = 0;
		for (auto const &ai : m_arrAIs)
		{
			writer.writeStartElement("Forwarding_ai");

			nAIOccNo++;
			//ai OccNo
			ai->SetOccNo(nAIOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAIOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(ai->m_szTagName));
			if (!CheckTagNameIsValid(ai->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  AI TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(ai->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(ai->m_nID));
			nBlockNo++;
			ai->m_nBlockNo = nBlockNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockNo));

			writer.writeAttribute("GroupNo", QString("%1").arg(ai->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(ai->m_nEntryNo));

			writer.writeAttribute("Address", QString("%1").arg(ai->m_strAddress));

			writer.writeAttribute("SourceTagName", QString("%1").arg(ai->m_strSourceTagName));

			writer.writeAttribute("Description", QString("%1").arg(ai->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(ai->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(ai->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(ai->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			//功能码
			writer.writeAttribute("FUN", QString("%1").arg(ai->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(ai->m_strInfo));
			writer.writeAttribute("MaxRaw", QString("%1").arg(ai->m_dblMaxRaw));
			writer.writeAttribute("MinRaw", QString("%1").arg(ai->m_dblMinRaw));
			writer.writeAttribute("MaxConvert", QString("%1").arg(ai->m_dblMaxConvert));
			writer.writeAttribute("MinConvert", QString("%1").arg(ai->m_dblMinConvert));

			//关联转换
			writer.writeAttribute("ScaleTagName", QString("%1").arg(ai->m_strScaleTagName));
			//ai 关联的转换大排行号
			//判断转换tagname是否为空
			if (ai->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(""));
			}
			else
			{
				auto pTransform = (Config::CTransform *)pFes->GetData(ai->m_strScaleTagName, HASHID::TRANSFORMHASHID);
				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(ai->m_strScaleTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(ai->m_nScaleType));
			}


			writer.writeAttribute("LLimit", QString("%1").arg(ai->m_dblHLimit));
			writer.writeAttribute("HLimit", QString("%1").arg(ai->m_dblHLimit));
			writer.writeAttribute("Threshold", QString("%1").arg(ai->m_dblThreshold));

			ai->m_nChannelOccNo = nOccNoChannel;
			//转发通道大排行号
			writer.writeAttribute("ForwardingChannelOccNo", QString("%1").arg(nOccNoChannel));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CForwardingChannelData::SaveDI(QXmlStreamWriter & writer, int & nOccNoChannel, int & nAIOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveDI
	** \details
	** \param writer
	** \param nOccNoChannel
	** \param nAIOccNo
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月16日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveDI(QXmlStreamWriter & writer, int & nOccNoChannel, int & nDIOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		writer.writeStartElement("Forwarding_DIN");
		writer.writeAttribute("Count", QString("%1").arg(m_arrDIs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 DIs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrDIs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		Q_ASSERT(nDIOccNo + 1);

		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		int nBlockNo = 0;
		for (auto const &di : m_arrDIs)
		{
			writer.writeStartElement("Forwarding_di");

			nDIOccNo++;
			di->SetOccNo(nDIOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nDIOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(di->m_szTagName));
			if (!CheckTagNameIsValid(di->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  DI TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(di->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(di->m_nID));

			nBlockNo++;
			di->m_nBlockNo = nBlockNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockNo));

			writer.writeAttribute("SourceTagName", QString("%1").arg(di->m_strSourceTagName));

			writer.writeAttribute("Description", QString("%1").arg(di->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(di->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(di->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(di->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("GroupNo", QString("%1").arg(di->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(di->m_nEntryNo));
			writer.writeAttribute("Address", QString("%1").arg(di->m_strAddress));

			writer.writeAttribute("FUN", QString("%1").arg(di->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(di->m_strInfo));
			writer.writeAttribute("Inverse", QString("%1").arg(di->m_bInverse));

			di->m_nChannelOccNo = nOccNoChannel;
			//转发通道大排行号
			writer.writeAttribute("ForwardingChannelOccNo", QString("%1").arg(nOccNoChannel));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CForwardingChannelData::SaveAO(QXmlStreamWriter & writer, int & nOccNoChannel, int & nAOOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveAO
	** \details 保存模拟量输出
	** \param writer
	** \param nOccNoChannel
	** \param nAOOccNo
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月16日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveAO(QXmlStreamWriter & writer, int & nOccNoChannel, int & nAOOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		writer.writeStartElement("Forwarding_AOUT");
		writer.writeAttribute("Count", QString("%1").arg(m_arrAOs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 AOs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrAOs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		Q_ASSERT(nAOOccNo + 1);

		int nBlockOccNo = 0;
		for (auto const &ao : m_arrAOs)
		{
			writer.writeStartElement("Forwarding_ao");

			nAOOccNo++;
			ao->SetOccNo(nAOOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAOOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(ao->m_szTagName));
			if (!CheckTagNameIsValid(ao->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  AO TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(ao->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(ao->m_nID));
			nBlockOccNo++;
			ao->m_nBlockNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));

			writer.writeAttribute("GroupNo", QString("%1").arg(ao->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(ao->m_nEntryNo));

			writer.writeAttribute("Address", QString("%1").arg(ao->m_strAddress));

			writer.writeAttribute("SourceTagName", QString("%1").arg(ao->m_strSourceTagName));

			writer.writeAttribute("Description", QString("%1").arg(ao->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(ao->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(ao->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(ao->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("FUN", QString("%1").arg(ao->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(ao->m_strInfo));

			writer.writeAttribute("MaxRaw", QString("%1").arg(ao->m_dbMaxRaw));
			writer.writeAttribute("MinRaw", QString("%1").arg(ao->m_dbMinRaw));
			writer.writeAttribute("MaxConvert", QString("%1").arg(ao->m_dbMaxConvert));
			writer.writeAttribute("MinConvert", QString("%1").arg(ao->m_dbMinConvert));

			//关联转换
			writer.writeAttribute("ScaleTagName", QString("%1").arg(ao->m_strScaleTagName));
			//ai 关联的转换大排行号
			//判断转换tagname是否为空
			if (ao->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(""));
			}
			else
			{
				auto pTransform = (Config::CTransform *)pFes->GetData(ao->m_strScaleTagName, HASHID::TRANSFORMHASHID);
				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(ao->m_strScaleTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(ao->m_nScaleType));
			}


			ao->m_nChannelOccNo = nOccNoChannel;
			//转发通道大排行号
			writer.writeAttribute("ForwardingChannelOccNo", QString("%1").arg(nOccNoChannel));


			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveCG
	** \details
	** \param writer
	** \param nOccNoChannel
	** \param nCGOccNo
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author xingzhibing
	** \date 2017年4月19日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveCG(QXmlStreamWriter &writer, int &nOccNoChannel, int &nCGOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		writer.writeStartElement("Forwarding_CGOUT");
		writer.writeAttribute("Count", QString("%1").arg(m_arrCGs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 CGs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrCGs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		Q_ASSERT(nCGOccNo >= 0);

		int nBlockOccNo = 0;
		for (auto const &cg : m_arrCGs)
		{
			writer.writeStartElement("Forwarding_cgo");

			nCGOccNo++;
			cg->SetOccNo(nCGOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nCGOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(cg->m_szTagName));
			if (!CheckTagNameIsValid(cg->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  CG TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(cg->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(cg->m_nID));
			nBlockOccNo++;
			cg->m_nBlockNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));

			writer.writeAttribute("GroupNo", QString("%1").arg(cg->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(cg->m_nEntryNo));

			writer.writeAttribute("Description", QString("%1").arg(cg->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(cg->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(cg->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(cg->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("FUN", QString("%1").arg(cg->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(cg->m_strInfo));

			writer.writeAttribute("Address", QString("%1").arg(cg->m_strAddress));

			cg->m_nChannelOccNo = nOccNoChannel;
			//转发通道大排行号
			writer.writeAttribute("ForwardingChannelOccNo", QString("%1").arg(nOccNoChannel));


			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CForwardingChannelData::SaveDO(QXmlStreamWriter & writer, int & nOccNoChannel, int & nDOOccNo, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveDO
	** \details 开关量输出
	** \param writer
	** \param nOccNoChannel
	** \param nDOOccNo
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月16日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveDO(QXmlStreamWriter & writer, int & nOccNoChannel, int & nDOOccNo, Config::CFesData *pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		writer.writeStartElement("Forwarding_DOUT");
		writer.writeAttribute("Count", QString("%1").arg(m_arrDOs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 DOs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrDOs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		Q_ASSERT(nDOOccNo + 1);

		int nBlockOccNo = 0;
		for (auto const &dout : m_arrDOs)
		{
			writer.writeStartElement("Forwarding_do");

			nDOOccNo++;

			writer.writeAttribute("OccNo", QString("%1").arg(nDOOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(dout->m_szTagName));
			if (!CheckTagNameIsValid(dout->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  DOUT TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(dout->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(dout->m_nID));
			nBlockOccNo++;
			dout->m_nBlockNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));

			writer.writeAttribute("GroupNo", QString("%1").arg(dout->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(dout->m_nEntryNo));

			writer.writeAttribute("SourceTagName", QString("%1").arg(dout->m_strSourceTagName));

			writer.writeAttribute("Description", QString("%1").arg(dout->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(dout->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(dout->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(dout->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("FUN", QString("%1").arg(dout->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(dout->m_strInfo));

			writer.writeAttribute("Address", QString("%1").arg(dout->m_strAddress));

			//关联遥控分
			writer.writeAttribute("AssociatedDI", dout->m_szAssociatedDI);
			//闭锁遥信
			writer.writeAttribute("BlockingSignalTag", dout->m_szBlockingSignalTag);
			//闭锁条件
			writer.writeAttribute("BlockingSignalCondition", QString("%1").arg(dout->m_bBlockingSignalCondition));

			dout->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(dout->m_nChannelOccNo));



			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CForwardingChannelData::SaveKwh(QXmlStreamWriter & writer, int & nOccNoChannel, int & nKwhOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::SaveKwh
	** \details 保存电度量
	** \param writer
	** \param nOccNoChannel
	** \param nKwhOccNo
	** \param pFes
	** \param
	** \param pHash
	** \param pStringPoolVec
	** \param pDescStringPoolOccNo
	** \return bool
	** \author gw
	** \date 2017年4月17日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::SaveKwh(QXmlStreamWriter & writer, int & nOccNoChannel, int & nKwhOccNo, CFesData * pFes, std::unordered_map<std::string, int32u>* pHash, std::vector<std::string>* pStringPoolVec, int32u * pDescStringPoolOccNo)
	{
		writer.writeStartElement("Forwarding_KWH");
		writer.writeAttribute("Count", QString("%1").arg(m_arrKWHs.size()));
		auto strTmp = QString("-->Fes %1  Forwarding Channel %2 KWHs count is %3!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(m_arrKWHs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);

		Q_ASSERT(nKwhOccNo + 1);

		int nBlockOccNo = 0;
		for (auto const &kwh : m_arrKWHs)
		{
			writer.writeStartElement("Forwarding_kwh");

			nKwhOccNo++;

			writer.writeAttribute("OccNo", QString("%1").arg(nKwhOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(kwh->m_szTagName));
			if (!CheckTagNameIsValid(kwh->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("Fes %1  Forwarding Channel TagName %2  KWH TagName %3 is invalid!!!").arg(pFes->m_szTagName).arg(m_szTagName).arg(kwh->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), true);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(kwh->m_nID));
			nBlockOccNo++;
			kwh->m_nBlockNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));

			writer.writeAttribute("GroupNo", QString("%1").arg(kwh->m_nGroupNo));
			writer.writeAttribute("EntryNo", QString("%1").arg(kwh->m_nEntryNo));

			writer.writeAttribute("Description", QString("%1").arg(kwh->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(kwh->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(kwh->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(kwh->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("SourceTagName", QString("%1").arg(kwh->m_strSourceTagName));

			writer.writeAttribute("FUN", QString("%1").arg(kwh->m_strFUN));
			writer.writeAttribute("INF", QString("%1").arg(kwh->m_strInfo));

			writer.writeAttribute("Address", QString("%1").arg(kwh->m_strAddress));

			kwh->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(kwh->m_nChannelOccNo));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	/*! \fn bool CForwardingChannelData::LoadData(QXmlStreamReader & reader, CFesData * pFes)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::LoadData
	** \details 加载转发通道下的AI DI AO DO KWH
	** \param reader
	** \param pFes
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::LoadData(QXmlStreamReader & reader, CFesData * pFes)
	{
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				//if (strTmp == "d")
				{
					//QString strTmp = reader.name().toString();

					//转发遥测
					if (strTmp == "Forwarding_AIN")
					{
						ReadAin(reader, pFes);
					}
					else if (strTmp == "Forwarding_DIN")
					{
						ReadDin(reader, pFes);
					}
					else if (strTmp == "Forwarding_AOUT")
					{
						ReadAout(reader, pFes);
					}
					else if (strTmp == "Forwarding_CGOUT")
					{
						ReadCgout(reader, pFes);
					}
					else if (strTmp == "Forwarding_DOUT")
					{
						ReadDout(reader, pFes);
					}
					else if (strTmp == "Forwarding_KWH")
					{
						ReadKwh(reader, pFes);
					}

					//	//ReadData(reader);
				}
			}
			else if (reader.isEndElement() && strTmp == "forward_c")
			{
				break;
			}

			reader.readNext();

		}

		return true;
	}

	/*! \fn bool CForwardingChannelData::ReadAin(QXmlStreamReader & reader, CFesData * pFesData)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::ReadAin
	** \details 转发遥测
	** \param reader
	** \param pFesData
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::ReadAin(QXmlStreamReader & reader, CFesData * pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_ai")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding ai OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding ai tag name %1 is invalid!!!!!!").arg(strTagName);
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingAnalogInputData *pAnalog = new CForwardingAnalogInputData();
					memset(pAnalog->m_szTagName, 0, sizeof(pAnalog->m_szTagName));
					strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pAnalog->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pAnalog->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pAnalog->m_nBlockNo = nBlockOccNo;

					pAnalog->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pAnalog->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					pAnalog->m_strAddress = reader.attributes().value("Address").toString();

					pAnalog->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();

					QString strDescription = reader.attributes().value("Description").toString();
					pAnalog->m_strDescription = strDescription;

					pAnalog->m_strFUN = reader.attributes().value("FUN").toString();
					pAnalog->m_strInfo = reader.attributes().value("INF").toString();

					pAnalog->m_dblMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pAnalog->m_dblMinRaw = reader.attributes().value("MinRaw").toDouble();
					pAnalog->m_dblMaxConvert = reader.attributes().value("MaxConvert").toDouble();
					pAnalog->m_dblMinConvert = reader.attributes().value("MinConvert").toDouble();

					pAnalog->m_strScaleTagName = reader.attributes().value("ScaleTagName").toString();
					pAnalog->m_nScaleType = reader.attributes().value("ScaleType").toInt();


					pAnalog->m_dblHLimit = reader.attributes().value("HLimit").toDouble();
					pAnalog->m_dblLLimit = reader.attributes().value("LLimit").toDouble();

					pAnalog->m_dblThreshold = reader.attributes().value("Threshold").toDouble();
					pAnalog->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					if (pFesData->CheckTagNameIsExist(pAnalog->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrAIs.push_back(pAnalog);
					//fes ai 检查重复tagname		//效率 是否保留
					pFesData->PushFesTagNameHashMap(FORWARDINGAIHASHID, pAnalog->m_szTagName, pAnalog);

					//添加转换对转发模拟量关系
					pFesData->AddForwardingAIRelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog);

					//note
					//转发AI source tagname关联前置AI tagname
					//前置AI对转换AI AI tagname是key,转换AI vec是value
					if (!pAnalog->m_strSourceTagName.isEmpty())
					{
						auto strKey = pAnalog->m_strSourceTagName;
						bool bFlag = pFesData->AddForwardingAIRelationSourceTagNameArr(strKey.toStdString(), pAnalog);
						Q_ASSERT(bFlag);
						if (!bFlag)
						{
							auto strError = QObject::tr("Source tagname of Forwarding ai tag name %1 is invalid!!!!!!").arg(strTagName);
							CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}
					}
				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_AIN")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CForwardingChannelData::ReadDin(QXmlStreamReader & reader, CFesData * pFesData)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::ReadDin
	** \details 读取转发遥信
	** \param reader
	** \param pFesData
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::ReadDin(QXmlStreamReader & reader, CFesData * pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_di")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding di OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding di tag name is invalid!!!!!!").arg(strTagName);
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingDigitalInputData *pDigital = new CForwardingDigitalInputData();
					memset(pDigital->m_szTagName, 0, sizeof(pDigital->m_szTagName));
					strncpy(pDigital->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pDigital->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigital->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pDigital->m_nBlockNo = nBlockOccNo;

					pDigital->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pDigital->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					pDigital->m_strAddress = reader.attributes().value("Address").toString();

					pDigital->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();

					QString strDescription = reader.attributes().value("Description").toString();
					pDigital->m_strDescription = strDescription;

					pDigital->m_strFUN = reader.attributes().value("FUN").toString();
					pDigital->m_strInfo = reader.attributes().value("INF").toString();
					pDigital->m_bInverse = reader.attributes().value("Inverse").toInt();

					pDigital->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					//fes di 检查重复tagname		//效率 是否保留
					if (pFesData->CheckTagNameIsExist(pDigital->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrDIs.push_back(pDigital);

					//管理hash
					pFesData->PushFesTagNameHashMap(FORWARDINGDIHASHID, pDigital->m_szTagName, pDigital);

					//note
					//转发DI source tagname关联前置DI tagname
					//前置DI对转换DI DI tagname是key,转换DI vec是value
					if (!pDigital->m_strSourceTagName.isEmpty())
					{
						auto bFlag = pFesData->AddForwardingDIRelationSourceTagNameArr(pDigital->m_strSourceTagName.toStdString(), pDigital);
						Q_ASSERT(bFlag);
						if (!bFlag)
						{
							auto strError = QObject::tr("Source tagname of Forwarding di tag name %1 is invalid!!!!!!").arg(strTagName);
							CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}
					}

				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_DIN")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CForwardingChannelData::ReadAout(QXmlStreamReader & reader, CFesData * pFesData)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::ReadAout
	** \details 读取转发遥调
	** \param reader
	** \param pFesData
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::ReadAout(QXmlStreamReader & reader, CFesData * pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_ao")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding ao OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding ao tag name is invalid!!!!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingAnalogOutputData *pAnalogOutput = new CForwardingAnalogOutputData();
					memset(pAnalogOutput->m_szTagName, 0, sizeof(pAnalogOutput->m_szTagName));
					strncpy(pAnalogOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pAnalogOutput->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pAnalogOutput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pAnalogOutput->m_nBlockNo = nBlockOccNo;

					pAnalogOutput->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pAnalogOutput->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					pAnalogOutput->m_strAddress = reader.attributes().value("Address").toString();;

					pAnalogOutput->m_dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pAnalogOutput->m_dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pAnalogOutput->m_dbMaxConvert = reader.attributes().value("MaxConvert").toDouble();
					pAnalogOutput->m_dbMinConvert = reader.attributes().value("MinConvert").toDouble();
					pAnalogOutput->m_strScaleTagName = reader.attributes().value("ScaleTagName").toString();
					pAnalogOutput->m_nScaleType = reader.attributes().value("ScaleType").toInt();

					pAnalogOutput->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();

					QString strDescription = reader.attributes().value("Description").toString();
					pAnalogOutput->m_strDescription = strDescription;

					pAnalogOutput->m_strFUN = reader.attributes().value("FUN").toString();
					pAnalogOutput->m_strInfo = reader.attributes().value("INF").toString();

					pAnalogOutput->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					//fes ao 检查重复tagname		//效率 是否保留
					if (pFesData->CheckTagNameIsExist(pAnalogOutput->m_szTagName))
					{
						auto strError = QObject::tr("Forwarding ao tag name %1 is exist!!!!!!").arg(pAnalogOutput->m_szTagName);
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						return false;
					}

					pFesData->PushFesTagNameHashMap(FORWARDINGAOHASHID, pAnalogOutput->m_szTagName, pAnalogOutput);

					//AOUT添加关联的转换 
					if (!pAnalogOutput->m_strScaleTagName.isEmpty())
					{
						pFesData->AddForwardingAORelationScaleArr(pAnalogOutput->m_strScaleTagName.toStdString(), pAnalogOutput);
					}

					//关联前置AO tagname
					if (!pAnalogOutput->m_strSourceTagName.isEmpty())
					{
						if (!pFesData->AddForwardingAORelationSourceTagNameArr(pAnalogOutput->m_strSourceTagName.toStdString(), pAnalogOutput))
						{
							auto strError = QObject::tr("SourceTagName %1 hash of Forwarding ao tag name %2 add failed!!!!!!").arg(pAnalogOutput->m_strSourceTagName).arg(pAnalogOutput->m_szTagName);
							CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}


					}

					m_arrAOs.push_back(pAnalogOutput);


				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_AOUT")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CForwardingChannelData::ReadCgout(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_cgo")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding cg OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding cg tag name is invalid!!!!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingControlGearData *pAnalogOutput = new CForwardingControlGearData();
					memset(pAnalogOutput->m_szTagName, 0, sizeof(pAnalogOutput->m_szTagName));
					strncpy(pAnalogOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pAnalogOutput->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pAnalogOutput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pAnalogOutput->m_nBlockNo = nBlockOccNo;

					pAnalogOutput->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pAnalogOutput->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					QString strDescription = reader.attributes().value("Description").toString();
					pAnalogOutput->m_strDescription = strDescription;

					pAnalogOutput->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();

					pAnalogOutput->m_strFUN = reader.attributes().value("FUN").toString();
					pAnalogOutput->m_strInfo = reader.attributes().value("INF").toString();

					pAnalogOutput->m_strAddress = reader.attributes().value("Address").toString();

					pAnalogOutput->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					//fes cg 检查重复tagname		//效率 是否保留
					if (pFesData->CheckTagNameIsExist(pAnalogOutput->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrCGs.push_back(pAnalogOutput);

					pFesData->PushFesTagNameHashMap(FORWARDINGCGHASHID, pAnalogOutput->m_szTagName, pAnalogOutput);


					//

				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_CGOUT")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CForwardingChannelData::ReadDout(QXmlStreamReader & reader, CFesData * pFesData)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::ReadDout
	** \details
	** \param reader
	** \param pFesData
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::ReadDout(QXmlStreamReader & reader, CFesData * pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_do")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding do OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding do tag name %1 is invalid!!!!!!").arg(strTagName);
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingDigitalOutputData *pDigitalOutput = new CForwardingDigitalOutputData();
					memset(pDigitalOutput->m_szTagName, 0, sizeof(pDigitalOutput->m_szTagName));
					strncpy(pDigitalOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pDigitalOutput->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalOutput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pDigitalOutput->m_nBlockNo = nBlockOccNo;

					pDigitalOutput->m_strSourceTagName = reader.attributes().value("SourceTagName").toString();

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalOutput->m_strDescription = strDescription;

					pDigitalOutput->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pDigitalOutput->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					pDigitalOutput->m_strFUN = reader.attributes().value("FUN").toString();
					pDigitalOutput->m_strInfo = reader.attributes().value("INF").toString();

					pDigitalOutput->m_strAddress = reader.attributes().value("Address").toString();

					//关联遥控分
					auto strAssociatedDI = reader.attributes().value("AssociatedDI").toString();
					bFlag = /*strAssociatedDI.length() == 0 || */strAssociatedDI.length() > MAX_TAGNAME_LEN_SCADASTUDIO;
					Q_ASSERT(!bFlag);
					if (bFlag)
					{
						auto strError = QObject::tr("Forwarding do tag name associatedDI is invalid!!!!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}
					memset(pDigitalOutput->m_szAssociatedDI, 0, strlen(pDigitalOutput->m_szAssociatedDI));
					strncpy(pDigitalOutput->m_szAssociatedDI, strAssociatedDI.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strAssociatedDI.size()));

					//闭锁遥信
					auto strBlockingSignalTag = reader.attributes().value("BlockingSignalTag").toString();
					bFlag = /*strBlockingSignalTag.length() == 0 ||*/ strBlockingSignalTag.length() > MAX_TAGNAME_LEN_SCADASTUDIO;
					Q_ASSERT(!bFlag);
					if (bFlag)
					{
						auto strError = QObject::tr("Forwarding do tag name BlockingSignalTag is invalid!!!!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}
					memset(pDigitalOutput->m_szBlockingSignalTag, 0, strlen(pDigitalOutput->m_szAssociatedDI));
					strncpy(pDigitalOutput->m_szBlockingSignalTag, strBlockingSignalTag.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strBlockingSignalTag.size()));

					//闭锁条件
					pDigitalOutput->m_bBlockingSignalCondition = reader.attributes().value("BlockingSignalCondition").toInt();

					pDigitalOutput->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					//fes ao 检查重复tagname		//效率 是否保留
					if (pFesData->CheckTagNameIsExist(pDigitalOutput->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					//关联前置AO tagname
					if (!pDigitalOutput->m_strSourceTagName.isEmpty())
					{
						if (!pFesData->AddForwardingDORelationSourceTagNameArr(pDigitalOutput->m_strSourceTagName.toStdString(), pDigitalOutput))
						{
							auto strError = QObject::tr("SourceTagName %1 hash of Forwarding ao tag name %2 add failed!!!!!!").arg(pDigitalOutput->m_strSourceTagName).arg(pDigitalOutput->m_szTagName);
							CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}


					}

					m_arrDOs.push_back(pDigitalOutput);

					pFesData->PushFesTagNameHashMap(FORWARDINGDOHASHID, pDigitalOutput->m_szTagName, pDigitalOutput);
				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_DOUT")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CForwardingChannelData::ReadKwh(QXmlStreamReader & reader, CFesData * pFesData)
	*********************************************************************************************************
	** \brief Config::CForwardingChannelData::ReadKwh
	** \details 读转发电度
	** \param reader
	** \param pFesData
	** \return bool
	** \author gw
	** \date 2017年4月18日
	** \note
	********************************************************************************************************/
	bool CForwardingChannelData::ReadKwh(QXmlStreamReader & reader, CFesData * pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "Forwarding_kwh")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("Forwarding kwh OccNo is invalid!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						auto strError = QObject::tr("Forwarding kwh tag name is invalid!!!!!!");
						CBaseData::LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CForwardingKWHData *pKwh = new CForwardingKWHData();
					memset(pKwh->m_szTagName, 0, sizeof(pKwh->m_szTagName));
					strncpy(pKwh->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pKwh->SetOccNo(nOccNo);

					int32u nID = reader.attributes().value("ID").toUInt();
					pKwh->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pKwh->m_nBlockNo = nBlockOccNo;

					pKwh->m_nGroupNo = reader.attributes().value("GroupNo").toUInt();
					pKwh->m_nEntryNo = reader.attributes().value("EntryNo").toUInt();

					QString strDescription = reader.attributes().value("Description").toString();
					pKwh->m_strDescription = strDescription;

					pKwh->m_strFUN = reader.attributes().value("SourceTagName").toString();

					pKwh->m_strFUN = reader.attributes().value("FUN").toString();
					pKwh->m_strInfo = reader.attributes().value("INF").toString();

					pKwh->m_strAddress = reader.attributes().value("Address").toString();

					pKwh->m_nChannelOccNo = reader.attributes().value("ForwardingChannelOccNo").toUInt();

					//fes kwh 检查重复tagname		//效率 是否保留
					if (pFesData->CheckTagNameIsExist(pKwh->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrKWHs.push_back(pKwh);

					pFesData->PushFesTagNameHashMap(FORWARDINGKWHHASHID, pKwh->m_szTagName, pKwh);
				}
			}
			else if (reader.isEndElement() && strTmp == "Forwarding_KWH")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	//CChannelGroup *CChannelGroup::CreateItem(CChannelGroup *pChannelGroup)
	//{
	//	
	//}

	/*! \fn CDevice::CDevice()
	*********************************************************************************************************
	** \brief CDevice::CDevice
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDevice::CDevice(IMainModuleInterface *pCore)
		:m_pCore(nullptr)
	{
		m_nID = 0;

		memset(m_szTagName, 0, MAX_TAGNAME_LEN_SCADASTUDIO + STRING_PAD);
		m_strDescription = "";
		m_nOccNo = 0;
		m_bModifyFlag = false;
		m_nPriority = 0;				//优先级别

		m_strDeviceAddr = "";		//装置地址
		m_strExtentionParam = "";	//扩展参数

		m_nDeviceModelID = 0;

		m_nType = 0;

		Q_ASSERT(pCore);
		if (!pCore)
		{
			return;
		}

		m_pCore = pCore;
	}

	/*! \fn CDevice::~CDevice()
	*********************************************************************************************************
	** \brief CDevice::~CDevice
	** \details
	** \return
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	CDevice::~CDevice()
	{
		Clear();
	}

	/*! \fn void CDevice::Save()
	*********************************************************************************************************
	** \brief CDevice::Save
	** \details
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDevice::Save()
	{

	}

	void CDevice::Clear()
	{
		for (size_t i = 0; i < m_arrAIs.size(); i++)
		{
			delete m_arrAIs[i];
		}
		m_arrAIs.clear();

		for (size_t i = 0; i < m_arrDIs.size(); i++)
		{
			delete m_arrDIs[i];
		}
		m_arrDIs.clear();

		for (size_t i = 0; i < m_arrAOs.size(); i++)
		{
			delete m_arrAOs[i];
		}
		m_arrAOs.clear();

		for (size_t i = 0; i < m_arrDOs.size(); i++)
		{
			delete m_arrDOs[i];
		}
		m_arrDOs.clear();
	}

	/*! \fn void CDevice::GetAnalogInput(CAnalogInput *pAnalogInput)
	*********************************************************************************************************
	** \brief CDevice::GetAnalogInput
	** \details 装置挂模拟量输入
	** \param pAnalogInput
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDevice::AddAnalogInput(CAnalogInput *pAnalogInput)
	{
		Q_ASSERT(pAnalogInput);
		if (pAnalogInput == nullptr)
			return;

		// 在调试态下检查是否重复添加
#ifdef DEBUG
		auto it = std::find(m_arrAIs.begin(), m_arrAIs.end(), pAnalogInput);
		if (it != m_arrAIs.end())
		{
			Q_ASSERT(false);
			return;
		}
#endif // DEBUG
		m_arrAIs.push_back(pAnalogInput);
	}

	/*! \fn void CDevice::GetDigitalInput(CDigitalInput *pDigitalInput)
	*********************************************************************************************************
	** \brief CDevice::GetDigitalInput
	** \details 装置挂数字量输入
	** \param pDigitalInput
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDevice::AddDigitalInput(CDigitalInput *pDigitalInput)
	{
		m_arrDIs.push_back(pDigitalInput);
	}

	/*! \fn void CDevice::GetAnalogOutput(CAnalogOutput *pAnalogOutput)
	*********************************************************************************************************
	** \brief CDevice::GetAnalogOutput
	** \details 装置挂模拟量输出
	** \param pAnalogOutput
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDevice::AddAnalogOutput(CAnalogOutput *pAnalogOutput)
	{
		m_arrAOs.push_back(pAnalogOutput);
	}

	/*! \fn void CDevice::GetDigitalOutput(CDigtalOutput *pDigitalOutput)
	*********************************************************************************************************
	** \brief CDevice::GetDigitalOutput
	** \details	装置挂数字量输出
	** \param pDigitalOutput
	** \return void
	** \author gw
	** \date 2016年1月4日
	** \note
	********************************************************************************************************/
	void CDevice::AddDigitalOutput(CDigitalOutput *pDigitalOutput)
	{
		m_arrDOs.push_back(pDigitalOutput);
	}

	/*! \fn bool CDevice::LoadNodeData(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::LoadNodeData
	** \details 加载点
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::LoadData(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();
			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				//if (strTmp == "d")
				{
					//QString strTmp = reader.name().toString();

					if (strTmp == "AIN")
					{
						ReadAIN(reader, pFesData);
					}
					else if (strTmp == "DIN")
					{
						ReadDIN(reader, pFesData);
					}
					else if (strTmp == "AOUT")
					{
						ReadAOUT(reader, pFesData);
					}
					else if (strTmp == "DOUT")
					{
						ReadDOUT(reader, pFesData);
					}

					//ReadData(reader);
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
	/*! \fn bool CDevice::ReadAIN(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadAIN
	** \details 读取ain数据
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadAIN(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
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
					ReadAi(reader, pFesData);
				}
			}
			else if (reader.isEndElement() && strTmp == "AIN")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CDevice::ReadAi(QXmlStreamReader &reader, std::unordered_map<std::string, CBaseData *> &arrTotalFesTag)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadAi
	** \details
	** \param reader
	** \param
	** \param arrTotalFesTag
	** \return bool
	** \author gw
	** \date 2016年10月25日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadAi(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;
		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ai")
				{

					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ai OccNo is invalid!!!");
						m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("ai tag name is invalid!!!!!!");
						m_pCore->LogMsg(DEVICE_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					CAnalogInput *pAnalog = new CAnalogInput(nOccNo);
					memset(pAnalog->m_szTagName, 0, sizeof(pAnalog->m_szTagName));
					strncpy(pAnalog->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));
					pAnalog->SetOccNo(nOccNo);
					//		pAnalog->m_nOccNo = nOccNo;



					int32u nID = reader.attributes().value("ID").toUInt();
					pAnalog->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pAnalog->m_nBlockOccNo = nBlockOccNo;

					//int8u nDataType = reader.attributes().value("DataType").toInt();
					//pAnalog->m_nDataType = nDataType;

					QString strDescription = reader.attributes().value("Description").toString();
					pAnalog->m_strDescription = strDescription;

					int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
					pAnalog->m_nInitialQua = nInitialQua;

					pAnalog->m_strAddress = reader.attributes().value("Address").toString();

					QString strPinLabel = reader.attributes().value("PinLabel").toString();
					pAnalog->m_strPinLabel = strPinLabel;

					//转换
					pAnalog->m_dbInitValue = reader.attributes().value("InitValue").toDouble();
					//Format
					pAnalog->m_strUnit = reader.attributes().value("Unit").toString();
					pAnalog->m_bEnable = reader.attributes().value("Enable").toInt();
					pAnalog->m_dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pAnalog->m_dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pAnalog->m_dbMaxConvert = reader.attributes().value("MaxConvert").toDouble();
					pAnalog->m_dbMinConvert = reader.attributes().value("MinConvert").toDouble();
					pAnalog->m_strScaleTagName = reader.attributes().value("ScaleTagName").toString();
					pAnalog->m_nScaleOccNo = reader.attributes().value("ScaleOccNo").toInt();
					pAnalog->m_nScaleType = reader.attributes().value("ScaleType").toInt();
					pAnalog->m_strScaleDesc = reader.attributes().value("ScaleDesc").toString();


					pAnalog->m_dblRangeL = reader.attributes().value("RangeL").toDouble();
					pAnalog->m_dblRangeH = reader.attributes().value("RangeH").toDouble();
					pAnalog->m_dblHighQua = reader.attributes().value("HighQua").toDouble();
					pAnalog->m_dblLowQua = reader.attributes().value("LowQua").toDouble();


					int8u nSaveDisk = reader.attributes().value("SaveDisk").toUInt();
					pAnalog->m_bSaveDisk = nSaveDisk;

					int32u nSaveDiskPeriod = reader.attributes().value("SaveDiskPeriod").toUInt();
					pAnalog->m_nSaveDiskPeriod = nSaveDiskPeriod;

					int8u nSaveDB = reader.attributes().value("SaveDB").toUInt();
					pAnalog->m_bSaveDB = nSaveDB;

					int32u nSaveDBPeriod = reader.attributes().value("SaveDBPeriod").toUInt();
					pAnalog->m_nSaveDBPeriod = nSaveDBPeriod;


					pAnalog->m_dblSensitivity = reader.attributes().value("Sensitivity").toDouble();
					pAnalog->m_nSensitivityType = reader.attributes().value("SensitivityType").toInt();


					pAnalog->m_strAlarmTagName = reader.attributes().value("AlarmTagName").toString();
					pAnalog->m_nAlarmOccNo = reader.attributes().value("AlarmOccNO").toInt();
					pAnalog->m_nAlarmType = reader.attributes().value("AlarmType").toInt();
					pAnalog->m_strAlarmDesc = reader.attributes().value("AlarmDesc").toString();

					pAnalog->m_nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
					pAnalog->m_nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
					pAnalog->m_nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
					pAnalog->m_nDataSource = reader.attributes().value("DataSource").toInt();
					pAnalog->m_strExpress = reader.attributes().value("Express").toString();
					pAnalog->m_strUnit = reader.attributes().value("Unit").toString();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ai %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					if (strTagName.isEmpty())
					{
						auto strFesError = QString(QObject::tr("ai %1 is empty")).arg(strTagName);
						m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}
					else if (strTagName.count() > MAX_TAGNAME_LEN_SCADASTUDIO)
					{
						auto strFesError = QString(QObject::tr("ai %1 size is more than %2 ")).arg(strTagName).arg(MAX_TAGNAME_LEN_SCADASTUDIO);
						m_pCore->LogMsg(AI_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (pFesData->CheckTagNameIsExist(pAnalog->m_szTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrAIs.push_back(pAnalog);
					//fes ai 检查重复tagname
					pFesData->PushFesTagNameHashMap(AIHASHID, pAnalog->m_szTagName, pAnalog);
					//添加
					pFesData->AddAIRelationScaleArr(pAnalog->m_strScaleTagName.toStdString(), pAnalog);
					pFesData->AddAIRelationAlarmArr(pAnalog->m_strAlarmTagName.toStdString(), pAnalog);
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

	/*! \fn void CDevice::ReadDIN(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadDIN
	** \details 读取数字量
	** \param reader
	** \return void
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadDIN(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
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
					ReadDi(reader, pFesData);
				}
			}
			else if (reader.isEndElement() && strTmp == "DIN")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CDevice::ReadDi(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadDi
	** \details
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadDi(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "di")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("di OccNo is invalid!!!");

						m_pCore->LogMsg(DI_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					Q_ASSERT(strTagName.length() > 0 && strlen(strTagName.toStdString().c_str()) < MAX_TAGNAME_LEN_SCADASTUDIO);
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						auto strError = QObject::tr("di Tag name %1 length is invalid!!!").arg(strTagName);
						m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}



					CDigitalInput *pDigtalInput = new CDigitalInput();
					pDigtalInput->SetOccNo(nOccNo);
					memset(pDigtalInput->m_szTagName, 0, sizeof(pDigtalInput->m_szTagName));
					strncpy(pDigtalInput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					memset(pDigtalInput->m_szDoubleDI, 0, sizeof(pDigtalInput->m_szDoubleDI));
					auto strDoubleDI = reader.attributes().value("DoubleDI").toString();
					strncpy(pDigtalInput->m_szDoubleDI, strDoubleDI.toStdString().c_str(), qMin(MAX_NAME_LENGTH_SCADASTUDIO, strDoubleDI.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigtalInput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pDigtalInput->m_nBlockOccNo = nBlockOccNo;

					//int8u nDataType = reader.attributes().value("DataType").toInt();
					//pDigtalInput->m_nDataType = nDataType;

					QString strDIName = reader.attributes().value("Description").toString();
					pDigtalInput->m_strDescription = strDIName;

					bool bInitValue = reader.attributes().value("InitValue").toInt();
					pDigtalInput->m_bInitValue = bInitValue;

					int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
					pDigtalInput->m_nInitialQua = nInitialQua;

					QString strAddress = reader.attributes().value("Address").toString();
					pDigtalInput->m_strAddress = strAddress;

					QString strPinLabel = reader.attributes().value("PinLabel").toString();
					pDigtalInput->m_strPinLabel = strPinLabel;

					pDigtalInput->m_nSignalType = reader.attributes().value("SignalType").toInt();

					pDigtalInput->m_bSOE = reader.attributes().value("SOE").toInt();

					pDigtalInput->m_strBitCloseString = reader.attributes().value("BitCloseString").toString();
					pDigtalInput->m_strBitOpenString = reader.attributes().value("BitOpenString").toString();

					pDigtalInput->m_strAlarmTag = reader.attributes().value("AlarmTagName").toString();
					pDigtalInput->m_strAlarmDesc = reader.attributes().value("AlarmDesc").toString();

					int8u nSaveDisk = reader.attributes().value("SaveDisk").toUInt();
					pDigtalInput->m_bSaveDisk = nSaveDisk;

					int32u nSaveDiskPeriod = reader.attributes().value("SaveDiskPeriod").toUInt();
					pDigtalInput->m_nSaveDiskPeriod = nSaveDiskPeriod;

					int8u nSaveDB = reader.attributes().value("SaveDB").toUInt();
					pDigtalInput->m_bSaveDB = nSaveDB;

					int32u nSaveDBPeriod = reader.attributes().value("SaveDBPeriod").toUInt();
					pDigtalInput->m_nSaveDBPeriod = nSaveDBPeriod;

					int nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
					pDigtalInput->m_nReferenceCount = nReferenceCount;

					pDigtalInput->m_nDataSource = reader.attributes().value("DataSource").toUInt();

					int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
					pDigtalInput->m_nChannelOccNo = nChannelOccNo;

					int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
					pDigtalInput->m_nDeviceOccNo = nDeviceOccNo;

					pDigtalInput->m_strExpress = reader.attributes().value("Express").toString();

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strError = QString(QObject::tr("ID %1 of di %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
					}

					if (pFesData->CheckTagNameIsExist(strTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrDIs.push_back(pDigtalInput);
					//fes di hash
					pFesData->PushFesTagNameHashMap(DIHASHID, pDigtalInput->m_szTagName, pDigtalInput);


					//开关量添加双位遥信的次遥信TagName关联
					
					if (strlen(pDigtalInput->m_szDoubleDI) > 0)
					{
						if (!pFesData->AddDIRelationDoubleDIArr(pDigtalInput->m_szDoubleDI, pDigtalInput))
						{
							auto strError = QString(QObject::tr("DI tagname %1 of Fes %2  Add DoubleDI %3 relation failed!!!")).arg(pFesData->m_szTagName).arg(strTagName).arg(pDigtalInput->m_szDoubleDI);
							m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}
					}

					//开关量添加告警关联
					if (!pDigtalInput->m_strAlarmTag.isEmpty())
					{
						if (!pFesData->AddDIRelationAlarmArr(pDigtalInput->m_strAlarmTag.toStdString(), pDigtalInput))
						{
							auto strError = QString(QObject::tr("DI tagname %1 of Fes %2  Add DI Relation Alarm %3 relation failed!!!")).arg(pFesData->m_szTagName).arg(strTagName).arg(pDigtalInput->m_strAlarmTag);
							m_pCore->LogMsg(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);
						}
					}

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

	/*! \fn bool CDevice::ReadDOUT(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadDOUT
	** \details 读取dout数据
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadDOUT(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "do")
				{
					ReadDo(reader, pFesData);
				}
			}
			else if (reader.isEndElement() && strTmp == "DOUT")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	/*! \fn bool CDevice::ReadDo(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadDo
	** \details
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadDo(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "do")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ao OccNo is invalid!!!");
						m_pCore->LogMsg(AO_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					bool bFlag = CheckTagNameIsValid(strTagName, FES_DESC);
					Q_ASSERT(bFlag);
					if (!bFlag)
					{
						reader.readNext();

						continue;
					}

					CDigitalOutput *pDigitalOutput = new CDigitalOutput();

					pDigitalOutput->SetOccNo(nOccNo);
					//pDigitalOutput->m_szTagName = strTagName;
					memset(pDigitalOutput->m_szTagName, 0, sizeof(pDigitalOutput->m_szTagName));
					strncpy(pDigitalOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pDigitalOutput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pDigitalOutput->m_nBlockOccNo = nBlockOccNo;

					pDigitalOutput->m_nDataType = reader.attributes().value("DataType").toUInt();

					QString strDescription = reader.attributes().value("Description").toString();
					pDigitalOutput->m_strDescription = strDescription;

					pDigitalOutput->m_bInitValue = reader.attributes().value("InitValue").toInt();

					int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
					pDigitalOutput->m_nInitialQua = nInitialQua;

					pDigitalOutput->m_strAddress = reader.attributes().value("Address").toString();
					pDigitalOutput->m_strPinLabel = reader.attributes().value("PinLabel").toString();

					int8u nIsSBO = reader.attributes().value("IsSBO").toInt();
					pDigitalOutput->m_bIsSBO = nIsSBO;

					int32u nTelCtlWaitTime = reader.attributes().value("TelCtlWaitTime").toUInt();
					pDigitalOutput->m_dblTelCtlWaitTime = nTelCtlWaitTime;

					int32u nTelCtlSendTime = reader.attributes().value("TelCtlSendTime").toUInt();
					pDigitalOutput->m_dblTelCtlSendTime = nTelCtlSendTime;

					int32u nTelCtlExecTime = reader.attributes().value("TelCtlExecTime").toUInt();
					pDigitalOutput->m_dblTelCtlExecTime = nTelCtlExecTime;

					//关联遥信点名
					auto strAssociatedDI = reader.attributes().value("AssociatedDI").toString();
					memset(pDigitalOutput->m_szAssociatedDI, 0, sizeof(pDigitalOutput->m_szAssociatedDI));
					auto nSize = strAssociatedDI.size();
					strncpy(pDigitalOutput->m_szAssociatedDI, strAssociatedDI.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					//关联开入点的测值
					pDigitalOutput->m_nAssociatedDIValType = reader.attributes().value("AssociatedDIValType").toUInt();

					//闭锁开入
					auto strBlockingSignalTag = reader.attributes().value("BlockingSignalTag").toString();
					memset(pDigitalOutput->m_szBlockingSignalTag, 0, sizeof(pDigitalOutput->m_szAssociatedDI));
					nSize = strBlockingSignalTag.size();
					strncpy(pDigitalOutput->m_szBlockingSignalTag, strBlockingSignalTag.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, nSize));

					pDigitalOutput->m_strBlockingProgram = reader.attributes().value("BlockingProgram").toString();

					pDigitalOutput->m_strStartProgram = reader.attributes().value("StartProgram").toString();

					int32u nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
					pDigitalOutput->m_nReferenceCount = nReferenceCount;

					int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
					pDigitalOutput->m_nChannelOccNo = nChannelOccNo;

					int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
					pDigitalOutput->m_nDeviceOccNo = nDeviceOccNo;

					bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of do %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(DO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (pFesData->CheckTagNameIsExist(strTagName))
					{
						reader.readNext();

						return false;
					}

					m_arrDOs.push_back(pDigitalOutput);


					//note 关联关系
					//fes do hash
					pFesData->PushFesTagNameHashMap(DOHASHID, pDigitalOutput->m_szTagName, pDigitalOutput);

					//关联遥信点 AssociatedDI
					if (!strAssociatedDI.isEmpty())
					{
						if (!pFesData->AddDORelationAssociatedDIArr(strAssociatedDI.toStdString(), pDigitalOutput))
						{
							auto strError = QString(QObject::tr("Add DOs %1 Relation AssociatedDI %2 failed!!!")).arg(strTagName).arg(strAssociatedDI);
							m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						}
					}

					//关联遥信 BlockingSignalTag
					if (!strBlockingSignalTag.isEmpty())
					{
						if (!pFesData->AddDORelationBlockingSignalTagArr(strBlockingSignalTag.toStdString(), pDigitalOutput))
						{
							auto strError = QString(QObject::tr("Add DOs %1 Relation BlockingSignalTag %2 failed!!!")).arg(strTagName).arg(strBlockingSignalTag);
							m_pCore->LogMsg(FES_DESC, strError.toStdString().c_str(), LEVEL_1);
						}
					}
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

	/*! \fn bool CDevice::ReadAOUT(QXmlStreamReader &reader)
	*********************************************************************************************************
	** \brief Config::CDevice::ReadAOUT
	** \detail
	** \param reader
	** \return bool
	** \author gw
	** \date 2016年9月1日
	** \note
	********************************************************************************************************/
	bool CDevice::ReadAOUT(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ao")
				{
					ReadAo(reader, pFesData);
				}
			}
			else if (reader.isEndElement() && strTmp == "AOUT")
			{
				break;
			}

			reader.readNext();
		}

		return true;
	}

	bool CDevice::ReadAo(QXmlStreamReader &reader, CFesData *pFesData)
	{
		Q_ASSERT(pFesData);
		if (!pFesData)
		{
			return false;
		}

		using namespace Config;

		while (!reader.atEnd())
		{
			QString strTmp = reader.name().toString();

			if (reader.isStartElement())
			{
				QString strTmp = reader.name().toString();

				if (strTmp == "ao")
				{
					int32u nOccNo = reader.attributes().value("OccNo").toInt();
					Q_ASSERT(nOccNo >= MIN_OCCNO_SCADASTUDIO && nOccNo <= MAX_OCCNO_SCADASTUDIO);
					if (nOccNo <= MIN_OCCNO_SCADASTUDIO || nOccNo > MAX_OCCNO_SCADASTUDIO)
					{// LOG
						auto strError = QObject::tr("ao OccNo is invalid!!!");
						m_pCore->LogMsg(AO_DESC, strError.toStdString().c_str(), LEVEL_1);

						reader.readNext();

						continue;
					}

					QString strTagName = reader.attributes().value("TagName").toString();
					if (!CheckTagNameIsValid(strTagName, FES_DESC))
					{
						reader.readNext();

						continue;
					}

					CAnalogOutput *pAnalogOutput = new CAnalogOutput();

					pAnalogOutput->SetOccNo(nOccNo);
					//pAnalogOutput->m_szTagName = strTagName;

					memset(pAnalogOutput->m_szTagName, 0, sizeof(pAnalogOutput->m_szTagName));
					strncpy(pAnalogOutput->m_szTagName, strTagName.toStdString().c_str(), qMin(MAX_TAGNAME_LEN_SCADASTUDIO, strTagName.size()));

					int32u nID = reader.attributes().value("ID").toUInt();
					pAnalogOutput->m_nID = nID;

					int32u nBlockOccNo = reader.attributes().value("BlockOccNo").toUInt();
					pAnalogOutput->m_nBlockOccNo = nBlockOccNo;

					int8u nDataType = reader.attributes().value("DataType").toInt();
					pAnalogOutput->m_nDataType = nDataType;


					QString strDescription = reader.attributes().value("Description").toString();
					pAnalogOutput->m_strDescription = strDescription;

					int8u nInitialQua = reader.attributes().value("InitialQua").toInt();
					pAnalogOutput->m_nInitialQua = nInitialQua;

					pAnalogOutput->m_strAddress = reader.attributes().value("Address").toString();
					pAnalogOutput->m_strPinLabel = reader.attributes().value("PinLabel").toString();

					fp64 dbInitValue = reader.attributes().value("InitValue").toDouble();
					pAnalogOutput->m_dbInitValue = dbInitValue;
					pAnalogOutput->m_strFormat = reader.attributes().value("Format").toString();
					pAnalogOutput->m_strUnit = reader.attributes().value("Unit").toString();
					pAnalogOutput->m_bEnable = reader.attributes().value("Enable").toInt();
					pAnalogOutput->m_dbMaxRaw = reader.attributes().value("MaxRaw").toDouble();
					pAnalogOutput->m_dbMinRaw = reader.attributes().value("MinRaw").toDouble();
					pAnalogOutput->m_dbMaxConvert = reader.attributes().value("MaxConvert").toDouble();
					pAnalogOutput->m_dbMinConvert = reader.attributes().value("MinConvert").toDouble();
					pAnalogOutput->m_strScaleTagName = reader.attributes().value("ScaleTagName").toString();
					pAnalogOutput->m_nScaleType = reader.attributes().value("ScaleType").toInt();
					pAnalogOutput->m_strScaleDesc = reader.attributes().value("ScaleDesc").toString();

					fp64 dbRangeL = reader.attributes().value("RangeL").toDouble();
					pAnalogOutput->m_dblRangeL = dbRangeL;

					fp64 dbRangeH = reader.attributes().value("RangeH").toDouble();
					pAnalogOutput->m_dblRangeH = dbRangeH;

					fp64 dbHighQua = reader.attributes().value("HighQua").toDouble();
					pAnalogOutput->m_dblHighQua = dbHighQua;

					fp64 dbLowQua = reader.attributes().value("LowQua").toDouble();
					pAnalogOutput->m_dblLowQua = dbLowQua;

					int nReferenceCount = reader.attributes().value("ReferenceCount").toUInt();
					pAnalogOutput->m_nReferenceCount = nReferenceCount;

					int32u nChannelOccNo = reader.attributes().value("ChannelOccNo").toUInt();
					pAnalogOutput->m_nChannelOccNo = nChannelOccNo;

					int32u nDeviceOccNo = reader.attributes().value("DeviceOccNo").toUInt();
					pAnalogOutput->m_nDeviceOccNo = nDeviceOccNo;

					auto bFlag = CHECK_LESS_ZERO_INT(nID);
					if (!bFlag)
					{
						auto strFesError = QString(QObject::tr("ID %1 of ao %2 is invalid < 0")).arg(nID).arg(strTagName);
						m_pCore->LogMsg(AO_DESC, (char *)strFesError.toStdString().c_str(), LEVEL_1);
					}

					if (pFesData->CheckTagNameIsExist(strTagName))
					{
						return false;
					}

					m_arrAOs.push_back(pAnalogOutput);
					//fes ao hash
					pFesData->PushFesTagNameHashMap(AOHASHID, pAnalogOutput->m_szTagName, pAnalogOutput);
					
					//note
					//模拟量输出关联转换
					if (!pAnalogOutput->m_strScaleTagName.isEmpty())
					{
						pFesData->AddAORelationScaleArr(pAnalogOutput->m_strScaleTagName.toStdString(), pAnalogOutput);
					}
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

	bool CDevice::SaveDeviceData(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, int &nDIOccNo, int &nAOOccNo, int &nDOOccNo
		, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("d");
		nOccNoDevice++;
		Q_ASSERT(nOccNoDevice > 0);
		m_nOccNo = nOccNoDevice;
		writer.writeAttribute("OccNo", QString("%1").arg(nOccNoDevice));
		writer.writeAttribute("TagName", QString("%1").arg(m_szTagName));
		if (!CheckTagNameIsValid(m_szTagName, FES_DESC))
		{
			auto strTmp = QString("-->Fes TagName %1  device %2 is invalid").arg(pFes->m_szTagName).arg(m_szTagName);
			MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
			s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), true);
		}

		writer.writeAttribute("ID", QString("%1").arg(m_nID));
		writer.writeAttribute("Description", QString("%1").arg(m_strDescription));
		//字符串内存池
		const auto it = pHash->find(m_strDescription.toStdString());
		if (it == pHash->end())
		{
			*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
			pHash->insert(make_pair(m_strDescription.toStdString(), *pDescStringPoolOccNo));
			pStringPoolVec->push_back(m_strDescription.toStdString());

			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
		}
		else
		{
			//desc occno
			writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
		}
		//pHash->insert(std::make_pair(m_strDescription.toStdString());

		writer.writeAttribute("DeviceModelID", QString("%1").arg(m_nDeviceModelID));
		writer.writeAttribute("Type", QString("%1").arg(m_nType));
		writer.writeAttribute("DeviceAddr", QString("%1").arg(m_strDeviceAddr));
		writer.writeAttribute("Priority", QString("%1").arg(m_nID));
		writer.writeAttribute("ExtentionParam", QString("%1").arg(m_strExtentionParam));

		//////////////////////////data////////////////////////////////////////
		writer.writeStartElement("data");

		SaveAI(writer, nOccNoChannel, nOccNoDevice, nAIOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveDI(writer, nOccNoChannel, nOccNoDevice, nDIOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveAO(writer, nOccNoChannel, nOccNoDevice, nAOOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);

		SaveDO(writer, nOccNoChannel, nOccNoDevice, nDOOccNo, pFes, pHash, pStringPoolVec, pDescStringPoolOccNo);

		writer.writeEndElement();
		//////////////////////////////////////////////////////////////////

		writer.writeEndElement();

		return true;
	}

	bool CDevice::SaveAI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAIOccNo, CFesData *pFes
		, std::unordered_map<std::string, int32u> *pHash, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		auto strTmp = QString("-->Fes TagName %1  device %2 AIN log start!!!").arg(pFes->m_szTagName).arg(m_szTagName);
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
		
		writer.writeStartElement("AIN");
		writer.writeAttribute("Count", QString("%1").arg(m_arrAIs.size()));
		strTmp = QString("-->Fes TagName %1  device %2 AIs count is %3").arg(pFes->m_szTagName).arg(m_szTagName).arg((int)m_arrAIs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);

		Q_ASSERT(nAIOccNo + 1 > 0);
		int nBlockOccNo = 0;
		for (auto const &ai : m_arrAIs)
		{
			Q_ASSERT(pFes);
			if (!pFes)
			{
				break;
			}

			writer.writeStartElement("ai");

			nAIOccNo++;
			//ai OccNo
			ai->SetOccNo(nAIOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAIOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(ai->m_szTagName));
			if (!CheckTagNameIsValid(ai->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("-->Fes TagName %1  device %2 ai %3 is invalid").arg(pFes->m_szTagName).arg(m_szTagName).arg(ai->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(ai->m_nID));
			nBlockOccNo++;
			ai->m_nBlockOccNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));
			//writer.writeAttribute("DataType", QString("%1").arg(ai->m_nDataType));
			writer.writeAttribute("Description", QString("%1").arg(ai->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(ai->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(ai->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(ai->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}


			writer.writeAttribute("InitialQua", QString("%1").arg(ai->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(ai->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(ai->m_strPinLabel));
			//
			writer.writeAttribute("InitValue", QString("%1").arg(ai->m_dbInitValue));
			writer.writeAttribute("Format", QString("%1").arg(ai->m_strFormat));
			writer.writeAttribute("Unit", QString("%1").arg(ai->m_strUnit));
			writer.writeAttribute("Enable", QString("%1").arg(ai->m_bEnable));
			writer.writeAttribute("MaxRaw", QString("%1").arg(ai->m_dbMaxRaw));
			writer.writeAttribute("MinRaw", QString("%1").arg(ai->m_dbMinRaw));
			writer.writeAttribute("MaxConvert", QString("%1").arg(ai->m_dbMaxConvert));
			writer.writeAttribute("MinConvert", QString("%1").arg(ai->m_dbMinConvert));
			writer.writeAttribute("ScaleTagName", QString("%1").arg(ai->m_strScaleTagName));
			//ai 关联的转换大排行号
			//判断转换tagname是否为空
			if (ai->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(""));
			}
			else
			{
				auto pTransform = (Config::CTransform *)pFes->GetData(ai->m_strScaleTagName, HASHID::TRANSFORMHASHID);
				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(ai->m_strScaleTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(ai->m_nScaleType));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(ai->m_strScaleDesc));
			}


			writer.writeAttribute("RangeL", QString("%1").arg(ai->m_dblRangeL));
			writer.writeAttribute("RangeH", QString("%1").arg(ai->m_dblRangeH));
			writer.writeAttribute("HighQua", QString("%1").arg(ai->m_dblHighQua));
			writer.writeAttribute("LowQua", QString("%1").arg(ai->m_dblLowQua));

			writer.writeAttribute("SaveDisk", QString("%1").arg(ai->m_bSaveDisk));
			writer.writeAttribute("SaveDiskPeriod", QString("%1").arg(ai->m_nSaveDiskPeriod));
			writer.writeAttribute("SaveDB", QString("%1").arg(ai->m_bSaveDB));
			writer.writeAttribute("SaveDBPeriod", QString("%1").arg(ai->m_nSaveDBPeriod));

			writer.writeAttribute("Sensitivity", QString("%1").arg(ai->m_dblSensitivity));
			writer.writeAttribute("SensitivityType", QString("%1").arg(ai->m_nSensitivityType));

			//ai 关联的告警大排行号
			//判断告警tagname是否为空
			if (ai->m_strAlarmTagName.isEmpty())
			{
				writer.writeAttribute("AlarmTagName", QString("%1").arg(""));
				writer.writeAttribute("AlarmType", QString("%1").arg(0));
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(0));
				writer.writeAttribute("AlarmDesc", QString("%1").arg(ai->m_strAlarmDesc));
			}
			else
			{
				writer.writeAttribute("AlarmTagName", QString("%1").arg(ai->m_strAlarmTagName));


				if (ai->m_nAlarmType == AIALARM)
				{
					auto pAIAlarm = (Config::CAnalogAlarm *)pFes->GetData(ai->m_strAlarmTagName, HASHID::ANALOGALARMHASHID);
					Q_ASSERT(pAIAlarm);
					if (!pAIAlarm)
					{
						auto strError = QObject::tr("ai table save error, ai join alarm tagname %1 can not find!!!").arg(ai->m_strAlarmTagName);
						LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					writer.writeAttribute("AlarmType", QString("%1").arg(ai->m_nAlarmType));
					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pAIAlarm->GetOccNo()));
					writer.writeAttribute("AlarmDesc", QString("%1").arg(ai->m_strAlarmDesc));
				}
				else
				{
					auto pDIAlarm = (Config::CDigitalAlarm *)pFes->GetData(ai->m_strAlarmTagName, HASHID::DIGITALALARMHASHID);
					Q_ASSERT(pDIAlarm);
					if (!pDIAlarm)
					{
						auto strError = QObject::tr("di table save error, di join alarm tagname %1 can not find!!!").arg(ai->m_strAlarmTagName);
						LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

						return false;
					}
					writer.writeAttribute("AlarmType", QString("%1").arg(ai->m_nAlarmType));
					writer.writeAttribute("AlarmOccNo", QString("%1").arg(pDIAlarm->GetOccNo()));
					writer.writeAttribute("AlarmDesc", QString("%1").arg(ai->m_strAlarmDesc));
				}

			}

			ai->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(nOccNoChannel));

			ai->m_nDeviceOccNo = nOccNoDevice;
			writer.writeAttribute("DeviceOccNo", QString("%1").arg(nOccNoDevice));
			writer.writeAttribute("ReferenceCount", QString("%1").arg(ai->m_nReferenceCount));
			writer.writeAttribute("DataSource", QString("%1").arg(ai->m_nDataSource));
			writer.writeAttribute("Express", QString("%1").arg(ai->m_strExpress));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	bool CDevice::SaveDI(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nDIOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("DIN");
		writer.writeAttribute("Count", QString("%1").arg(m_arrDIs.size()));
		auto strTmp = QString("-->Fes TagName %1  device %2 DIs count is %3").arg(pFes->m_szTagName).arg(m_szTagName).arg((int)m_arrDIs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);


		Q_ASSERT(nDIOccNo + 1);

		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		int nBlockOccNo = 0;
		for (auto const &di : m_arrDIs)
		{
			writer.writeStartElement("di");

			nDIOccNo++;
			di->SetOccNo(nDIOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nDIOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(di->m_szTagName));
			if (!CheckTagNameIsValid(di->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("-->Fes TagName %1  device %2 di %3 is invalid").arg(pFes->m_szTagName).arg(m_szTagName).arg(di->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("DoubleDI", QString("%1").arg(di->m_szDoubleDI));
			writer.writeAttribute("ID", QString("%1").arg(di->m_nID));

			nBlockOccNo++;
			di->m_nBlockOccNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));
			//writer.writeAttribute("DataType", QString("%1").arg(di->m_nDataType));
			writer.writeAttribute("Description", QString("%1").arg(di->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(di->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(di->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(di->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("InitValue", QString("%1").arg(di->m_bInitValue));
			writer.writeAttribute("InitialQua", QString("%1").arg(di->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(di->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(di->m_strPinLabel));
			writer.writeAttribute("SignalType", QString("%1").arg(di->m_nSignalType));
			writer.writeAttribute("SOE", QString("%1").arg(di->m_bSOE));
			writer.writeAttribute("BitOpenString", QString("%1").arg(di->m_strBitOpenString));
			writer.writeAttribute("BitCloseString", QString("%1").arg(di->m_strBitCloseString));

			if (di->m_strAlarmTag.isEmpty())
			{
				writer.writeAttribute("AlarmTagName", QString("%1").arg(""));
				writer.writeAttribute("AlarmType", QString("%1").arg(0));
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(0));
				writer.writeAttribute("AlarmDesc", QString("%1").arg(di->m_strAlarmDesc));
			}
			else
			{
				writer.writeAttribute("AlarmTagName", QString("%1").arg(di->m_strAlarmTag));

				auto pDIAlarm = (Config::CDigitalAlarm *)pFes->GetData(di->m_strAlarmTag, HASHID::DIGITALALARMHASHID);
				Q_ASSERT(pDIAlarm);
				if (!pDIAlarm)
				{
					auto strError = QObject::tr("di table save error, di join alarm tagname %1 can not find!!!").arg(di->m_strAlarmTag);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					return false;
				}
				writer.writeAttribute("AlarmType", QString("%1").arg(di->m_nAlarmType));
				writer.writeAttribute("AlarmOccNo", QString("%1").arg(pDIAlarm->GetOccNo()));
				writer.writeAttribute("AlarmDesc", QString("%1").arg(di->m_strAlarmDesc));
			}

			writer.writeAttribute("SaveDisk", QString("%1").arg(di->m_bSaveDisk));
			writer.writeAttribute("SaveDiskPeriod", QString("%1").arg(di->m_nSaveDiskPeriod));
			writer.writeAttribute("SaveDB", QString("%1").arg(di->m_bSaveDB));
			writer.writeAttribute("SaveDBPeriod", QString("%1").arg(di->m_nSaveDBPeriod));
			writer.writeAttribute("ReferenceCount", QString("%1").arg(di->m_nReferenceCount));
			writer.writeAttribute("DataSource", QString("%1").arg(di->m_nDataSource));

			di->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(nOccNoChannel));

			di->m_nDeviceOccNo = nOccNoDevice;
			writer.writeAttribute("DeviceOccNo", QString("%1").arg(nOccNoDevice));
			writer.writeAttribute("Express", QString("%1").arg(di->m_strExpress));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	bool CDevice::SaveAO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nAOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		Q_ASSERT(pFes);
		if (!pFes)
		{
			return false;
		}

		writer.writeStartElement("AOUT");
		writer.writeAttribute("Count", QString("%1").arg(m_arrAOs.size()));
		auto strTmp = QString("-->Fes TagName %1  device %2 AOs count is %3").arg(pFes->m_szTagName).arg(m_szTagName).arg((int)m_arrAOs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);

		Q_ASSERT(nAOOccNo + 1);

		int nBlockOccNo = 0;
		for (auto const &ao : m_arrAOs)
		{
			writer.writeStartElement("ao");

			nAOOccNo++;
			ao->SetOccNo(nAOOccNo);
			writer.writeAttribute("OccNo", QString("%1").arg(nAOOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(ao->m_szTagName));
			if (!CheckTagNameIsValid(ao->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("-->Fes TagName %1  device %2 ao %3 is invalid").arg(pFes->m_szTagName).arg(m_szTagName).arg(ao->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(ao->m_nID));
			nBlockOccNo++;
			ao->m_nBlockOccNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));
			writer.writeAttribute("DataType", QString("%1").arg(ao->m_nDataType));
			writer.writeAttribute("Description", QString("%1").arg(ao->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(ao->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(ao->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(ao->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("InitialQua", QString("%1").arg(ao->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(ao->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(ao->m_strPinLabel));
			writer.writeAttribute("InitValue", QString("%1").arg(ao->m_dbInitValue));
			writer.writeAttribute("Format", QString("%1").arg(ao->m_strFormat));
			writer.writeAttribute("Unit", QString("%1").arg(ao->m_strUnit));
			writer.writeAttribute("Enable", QString("%1").arg(ao->m_bEnable));
			writer.writeAttribute("MaxRaw", QString("%1").arg(ao->m_dbMaxRaw));
			writer.writeAttribute("MinRaw", QString("%1").arg(ao->m_dbMinRaw));
			writer.writeAttribute("MaxConvert", QString("%1").arg(ao->m_dbMaxConvert));
			writer.writeAttribute("MinConvert", QString("%1").arg(ao->m_dbMinConvert));

			//////////////////////////////////////////////////////////////////////////
			//ao 关联的转换大排行号
			//判断转换tagname是否为空
			if (ao->m_strScaleTagName.isEmpty())
			{
				writer.writeAttribute("ScaleOccNo", QString("%1").arg(0));
				writer.writeAttribute("ScaleType", QString("%1").arg(LINEAR));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(""));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(ao->m_strScaleTagName));
			}
			else
			{
				auto pTransform = (Config::CTransform *)pFes->GetData(ao->m_strScaleTagName, HASHID::TRANSFORMHASHID);
				Q_ASSERT(pTransform);
				if (!pTransform)
				{
					auto strError = QObject::tr("scale tagname %1 is isvalid!!!").arg(ao->m_strScaleTagName);
					LogString(FES_DESC, (char *)strError.toStdString().c_str(), LEVEL_1);

					break;
				}

				writer.writeAttribute("ScaleOccNo", QString("%1").arg(pTransform->GetOccNo()));
				writer.writeAttribute("ScaleType", QString("%1").arg(ao->m_nScaleType));
				writer.writeAttribute("ScaleDesc", QString("%1").arg(ao->m_strScaleDesc));
				writer.writeAttribute("ScaleTagName", QString("%1").arg(ao->m_strScaleTagName));
			}

			//////////////////////////////////////////////////////////////////////////

			writer.writeAttribute("RangeL", QString("%1").arg(ao->m_dblRangeL));
			writer.writeAttribute("RangeH", QString("%1").arg(ao->m_dblRangeH));
			writer.writeAttribute("HighQua", QString("%1").arg(ao->m_dblHighQua));
			writer.writeAttribute("LowQua", QString("%1").arg(ao->m_dblLowQua));
			writer.writeAttribute("ReferenceCount", QString("%1").arg(ao->m_nReferenceCount));

			ao->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(ao->m_nChannelOccNo));

			ao->m_nDeviceOccNo = nOccNoDevice;
			writer.writeAttribute("DeviceOccNo", QString("%1").arg(1));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}

	bool CDevice::SaveDO(QXmlStreamWriter &writer, int &nOccNoChannel, int &nOccNoDevice, int &nDOOccNo, CFesData *pFes, std::unordered_map<std::string, int32u> *pHash
		, std::vector<std::string> *pStringPoolVec, int32u *pDescStringPoolOccNo)
	{
		writer.writeStartElement("DOUT");
		writer.writeAttribute("Count", QString("%1").arg(m_arrDOs.size()));
		auto strTmp = QString("-->Fes TagName %1  device %2 DOs count is %3").arg(pFes->m_szTagName).arg(m_szTagName).arg((int)m_arrDOs.size());
		MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);

		Q_ASSERT(nDOOccNo + 1);

		int nBlockOccNo = 0;
		for (auto const &dout : m_arrDOs)
		{
			writer.writeStartElement("do");

			nDOOccNo++;

			writer.writeAttribute("OccNo", QString("%1").arg(nDOOccNo));
			writer.writeAttribute("TagName", QString("%1").arg(dout->m_szTagName));
			if (!CheckTagNameIsValid(dout->m_szTagName, FES_DESC))
			{
				auto strTmp = QString("-->Fes TagName %1  device %2 ao %3 is invalid").arg(pFes->m_szTagName).arg(m_szTagName).arg(dout->m_szTagName);
				MYLIB::Log2File(LOG_FES_LOG, strTmp.toStdString().c_str(), LEVEL_1);
				s_pGlobleCore->LogMsg(FES_DESC, strTmp.toStdString().c_str(), LEVEL_1);
			}

			writer.writeAttribute("ID", QString("%1").arg(dout->m_nID));
			nBlockOccNo++;
			dout->m_nBlockOccNo = nBlockOccNo;
			writer.writeAttribute("BlockOccNo", QString("%1").arg(nBlockOccNo));
			writer.writeAttribute("DataType", QString("%1").arg(dout->m_nDataType));
			writer.writeAttribute("Description", QString("%1").arg(dout->m_strDescription));
			//字符串内存池
			const auto it = pHash->find(dout->m_strDescription.toStdString());
			if (it == pHash->end())
			{
				*pDescStringPoolOccNo = *pDescStringPoolOccNo + 1;
				pHash->insert(make_pair(dout->m_strDescription.toStdString(), *pDescStringPoolOccNo));
				pStringPoolVec->push_back(dout->m_strDescription.toStdString());

				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(*pDescStringPoolOccNo));
			}
			else
			{
				//desc occno
				writer.writeAttribute("DescriptionOccNo", QString("%1").arg(it->second));
			}

			writer.writeAttribute("InitValue", QString("%1").arg(dout->m_bInitValue));
			writer.writeAttribute("InitialQua", QString("%1").arg(dout->m_nInitialQua));
			writer.writeAttribute("Address", QString("%1").arg(dout->m_strAddress));
			writer.writeAttribute("PinLabel", QString("%1").arg(dout->m_strPinLabel));
			writer.writeAttribute("IsSBO", QString("%1").arg(dout->m_bIsSBO));
			writer.writeAttribute("TelCtlWaitTime", QString("%1").arg(dout->m_dblTelCtlWaitTime));
			writer.writeAttribute("TelCtlSendTime", QString("%1").arg(dout->m_dblTelCtlSendTime));
			writer.writeAttribute("TelCtlExecTime", QString("%1").arg(dout->m_dblTelCtlExecTime));
			writer.writeAttribute("AssociatedDI", QString("%1").arg(dout->m_szAssociatedDI));
			writer.writeAttribute("AssociatedDIValType", QString("%1").arg(dout->m_nAssociatedDIValType));
			writer.writeAttribute("BlockingSignalTag", QString("%1").arg(dout->m_szBlockingSignalTag));
			writer.writeAttribute("BlockingProgram", QString("%1").arg(dout->m_strBlockingProgram));
			writer.writeAttribute("StartProgram", QString("%1").arg(dout->m_strStartProgram));
			writer.writeAttribute("ReferenceCount", QString("%1").arg(dout->m_nReferenceCount));

			dout->m_nChannelOccNo = nOccNoChannel;
			writer.writeAttribute("ChannelOccNo", QString("%1").arg(dout->m_nChannelOccNo));

			dout->m_nDeviceOccNo = nOccNoDevice;
			writer.writeAttribute("DeviceOccNo", QString("%1").arg(dout->m_nDeviceOccNo));

			writer.writeEndElement();
		}

		writer.writeEndElement();

		return true;
	}


	bool CDevice::LogToFile(const char* pFilename, const char* pLog)
	{
		MYLIB::Log2File(pFilename, pLog, true);

		for each (auto var in m_arrAIs)
		{

		}

		return true;
	}

	void CTransform::Save()
	{

	}

}
