#include "db_svc.h"
#include "io_channel.h"

/*! \fn void CDbSvc::BuildNodeDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildNodeDB
** \details 创建节点内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildNodeDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//NODE
	int nIndex = 0;

	NODE* pNode = Q_NULLPTR;

	m_pNodes = (NODE*)((char*)pAddress);

	for (auto i : m_arrTempNodes)
	{
		pNode = (NODE*)((char*)m_pNodes + sizeof(NODE)*nIndex);
		std::memcpy(pNode, i.get(), sizeof(NODE));
		nIndex++;
	}
	m_pMagicMem->NodeCount = m_arrTempNodes.size();

	m_nNodeCount = m_arrTempNodes.size();

	m_pMagicMem->NodeTableOffset = sizeof(HEAD_MEM);

	return sizeof NODE * m_arrTempNodes.size();
}
/*! \fn void CDbSvc::BuildChannelDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildChannelDB
** \details 创建通道内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildChannelDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//CHANNEL
	int nIndex = 0;

	CHANNEL*  pChannel = Q_NULLPTR;

	m_pChannels = (CHANNEL *)((char*)pAddress);

	for (auto i : m_arrTempChannels)
	{
		pChannel = (CHANNEL*)((char*)m_pChannels + sizeof(CHANNEL)*nIndex);

		std::memcpy(pChannel, i.get(), sizeof(CHANNEL));

		m_arrChannelPtrs.push_back(pChannel);

		m_arrIoChannels.push_back(std::make_shared<CIoChannel>(pChannel));

		nIndex++;
	}

	m_nChannelCount = m_arrTempChannels.size();

	m_pMagicMem->ChannelCount = m_arrTempChannels.size();

	m_pMagicMem->ChannelTableOffset = m_pMagicMem->NodeTableOffset + sizeof(NODE)*m_arrTempNodes.size();

	return sizeof CHANNEL * m_arrChannelPtrs.size();
}
/*! \fn void CDbSvc::BuildDeviceDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildDeviceDB
** \details 创建设备内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildDeviceDB(char* pAddress)
{
	Q_ASSERT(pAddress);
	//DEVICE
	int nIndex = 0;

	DEVICE* pDevNode = Q_NULLPTR;

	m_pDevices = (DEVICE*)((char*)pAddress);

	for (auto i : m_arrTempDevices)
	{
		pDevNode = (DEVICE *)((char*)m_pDevices + sizeof(DEVICE)*nIndex);

		m_arrDevices.push_back(pDevNode);

		std::memcpy(pDevNode, i.get(), sizeof(DEVICE));

		nIndex++;
	}
	m_nDeviceCount = m_arrTempDevices.size();

	m_pMagicMem->DeviceCount = m_arrTempDevices.size();

	m_pMagicMem->DeviceTableOffset = m_pMagicMem->ChannelTableOffset + sizeof(CHANNEL)*m_arrTempChannels.size();

	return sizeof DEVICE * m_arrTempDevices.size();
}
/*! \fn void CDbSvc::BuildAinDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildAinDB
** \details 创建AIN内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildAinDB(char* pAddress)
{
	Q_ASSERT(pAddress);
	//AIN
	int nIndex = 0;

	AIN* pAin = Q_NULLPTR;

	m_pAins = (AIN*)((char*)pAddress );

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
	m_nAinCount = m_arrTempAins.size();

	m_pMagicMem->AinCount = m_arrTempAins.size();

	m_pMagicMem->AinTableOffset = m_pMagicMem->DinAlarmLimtTableOffset + sizeof(DIN_ALARM_LIMIT)*m_arrTempDinLimitAlarms.size();

	return sizeof AIN * m_arrTempAins.size();
}
/*! \fn void CDbSvc::BuildDinDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildDinDB
** \details 创建DIN内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildDinDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//DIN
	int nIndex = 0;

	DIN* pDin = Q_NULLPTR;

	m_pDins = (DIN*)((char*)pAddress);

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

	m_nDinCount = m_arrTempDins.size();

	m_pMagicMem->DinCount = m_arrTempDins.size();

	m_pMagicMem->DinTableOffset = m_pMagicMem->AinTableOffset + sizeof(AIN)*m_arrTempAins.size();

	return sizeof DIN * m_arrTempDins.size();
}
/*! \fn void CDbSvc::BuildAoutDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildAoutDB
** \details 创建AOUT内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildAoutDB(char* pAddress)
{
	Q_ASSERT(pAddress);
	//AOUT

	int nIndex = 0;
	AOUT* pAout = Q_NULLPTR;
	m_pAouts = (AOUT *)((char*)pAddress /*+ sizeof(DIN)*m_arrTempDins.size()*/);

	for (auto i : m_arrTempAouts)
	{
		pAout = (AOUT*)((char*)m_pAouts + sizeof(AOUT)*nIndex);

		m_arrAouts.push_back(pAout);

		std::memcpy(pAout, i.get(), sizeof(AOUT));

		nIndex++;
	}
	m_nAoutCount = m_arrTempAouts.size();

	m_pMagicMem->AoutCount = m_arrTempAouts.size();

	m_pMagicMem->AoutTableOffset = m_pMagicMem->DinTableOffset + sizeof(DIN)*m_arrTempDins.size();

	return sizeof AOUT * m_arrTempAouts.size();
}

/*! \fn void CDbSvc::BuildDoutDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildDoutDB
** \details 创建DOUT内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildDoutDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	int nIndex = 0;
	DOUT* pDout = Q_NULLPTR;

	m_pDouts = (DOUT *)((char*)pAddress /*+ sizeof(AOUT)*m_arrTempAouts.size()*/);

	for (auto i : m_arrTempDouts)
	{
		pDout = (DOUT*)((char*)m_pDouts + sizeof(DOUT)*nIndex);

		m_arrDouts.push_back(pDout);

		std::memcpy(pDout, i.get(), sizeof(DOUT));

		nIndex++;
	}

	m_nDoutCount = m_arrTempDouts.size();

	m_pMagicMem->DoutCount = m_arrTempDouts.size();

	m_pMagicMem->DountTableOffset = m_pMagicMem->AoutTableOffset + sizeof(AOUT)*m_arrTempAouts.size();

	return sizeof DOUT * m_arrTempDouts.size();
}

/*! \fn void CDbSvc::BuildAlarmDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildAlarmDB
** \details 创建告警内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildAlarmDB(char* pAddress)
{
	return 0;
}
/*! \fn void CDbSvc::BuildExtraDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildExtraDB
** \details 创建变长内存
** \param pAddress
** \return void
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildExtraDB(char* pAddress)
{
	Q_ASSERT(pAddress);

	//CHANNEL
	unsigned char * pRefData = (unsigned char*)((char*)pAddress/* + sizeof(DOUT)*m_arrTempDouts.size()*/);
	for (int i = 0; i < m_pChannelMgr->GetChannelCount(); i++)
	{
		CChannelPtr pTr = m_pChannelMgr->GetChannel(i);
		if (pTr)
		{
			pTr->SaveToMem(pRefData);
		}
		pRefData = pRefData + pTr->GetChannelInfoLen();
	}

	//DEVICE
	for (int i = 0; i < m_pChannelMgr->GetDevCount(); ++i)
	{
		CDevicePtr pTr = m_pChannelMgr->GetDevice(i);
		if (pTr)
		{
			pTr->SaveToMem(pRefData);
		}
		pRefData = pRefData + pTr->GetDevLen();
	}
	return 0;
}
/*! \fn size_t CDbSvc::BuildAinAlarmDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildAinAlarmDB
** \details
** \param pAddress
** \return size_t
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildAinAlarmDB(char* pAddress)
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

	m_pMagicMem->AinAlarmTableOffset = m_pMagicMem->DeviceTableOffset + sizeof(DEVICE)*m_arrTempDevices.size();

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
/*! \fn size_t CDbSvc::BuildDinAlarmDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildDinAlarmDB
** \details
** \param pAddress
** \return size_t
** \author xingzhibing
** \date 2016年9月23日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildDinAlarmDB(char* pAddress)
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

	for (auto i:m_arrTempDinLimitAlarms)
	{
		pDinAlarmLinit = (DIN_ALARM_LIMIT*)((char*)m_pDinAlarmLimit + sizeof( DIN_ALARM_LIMIT )*nIndex);

		m_arrDinAlarmLimits.push_back(pDinAlarmLinit);

		std::memcpy(pDinAlarmLinit,i.get(),sizeof(DIN_ALARM_LIMIT));

		nIndex++;
	}

	m_nDinAlarmLimitCount = m_arrDinAlarmLimits.size();

	m_pMagicMem->DinAlarmLimitCount = m_arrDinAlarmLimits.size();

	m_pMagicMem->DinAlarmLimtTableOffset = m_pMagicMem->DinAlarmTableOffset + sizeof(DIN_ALARM)*m_arrDinAlarms.size();

	//offset
	return sizeof DIN_ALARM * m_arrTempDinAlarms.size() + sizeof  DIN_ALARM_LIMIT * m_arrTempDinLimitAlarms.size();
}
/*! \fn size_t CDbSvc::BuildLinearDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildLinearDB
** \details 线性变换内存
** \param pAddress
** \return size_t
** \author xingzhibing
** \date 2016年9月26日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildLinearDB(char* pAddress)
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

	m_pMagicMem->LinearTableOffset = m_pMagicMem->DountTableOffset + sizeof(AOUT)*m_arrTempDouts.size();

	return sizeof TRANSFORM_LINEAR * m_arrTempTransFormLinears.size();
}
/*! \fn size_t CDbSvc::BuildNonLinearDB(char* pAddress)
*********************************************************************************************************
** \brief CDbSvc::BuildNonLinearDB
** \details 非线性变换内存
** \param pAddress
** \return size_t
** \author xingzhibing
** \date 2016年9月26日
** \note
********************************************************************************************************/
size_t CDbSvc::BuildNonLinearDB(char* pAddress)
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


size_t CDbSvc::BuildSystemVariableDB(char* pAddress)
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

size_t CDbSvc::BuildUserVariableDB(char* pAddress)
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