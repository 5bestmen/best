/*! @file mem_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.cpp
文件实现功能 :  内存库
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存库
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/


#include "mem_db.h"
#include "node.h"
#include "channel.h"
#include "device.h"
#include "tag.h"
#include "alarm.h"

#include "log/log.h"
#include "../fespub/sharemem.h"
#include "fesapi/fesdatadef.h"
#include "fesapi/fes_magicmem.h"
#include "mail/mail.h"
#include "../fespub/tag_att.h"
#include "../fespub/data_size.h"

#include <algorithm> 
#include <QObject>
#include <QDateTime>
#include <QFileInfo>

CMemDB::CMemDB( )  
{
	m_pMem = std::make_shared<CShareMem>();

//	m_ChannelMgr = std::make_shared<CChannelMgr>();
	m_pTagAttrMgr = std::make_shared<CTagAttMgr>();

	m_bInitialized = false;
 

	m_nNodeCount = 0;
	m_nChannelCount = 0;
	m_nDeviceCount = 0;
	m_nAinCount = 0;
	m_nDinCount = 0;
	m_nDoutCount = 0;
	m_nAoutCount = 0;

	m_nLinearCount = 0;
	m_nNonLinearCount = 0;

	m_pNodes = nullptr;
	m_pChannels = nullptr;
	m_pDevices = nullptr;
	m_pAins = nullptr;
	m_pDins = nullptr;
	m_pAouts = nullptr;
	m_pDouts = nullptr;
	m_pLinear = nullptr;
	m_pNonLinear = nullptr;
	m_pAinAlarm = nullptr;
	m_pAinAlarmLimit = nullptr;
	m_pDinAlarm = nullptr;
	m_pDinAlarmLimit = nullptr;
	m_nNBSvcMailBoxID = 0;

	InitFuncArrary();
}

CMemDB::~CMemDB(void)
{
	m_mapNodes.clear();
	m_mapChannels.clear();
	m_mapDevices.clear();
	
	m_mapAins.clear();
	m_mapDins.clear();
	m_mapAouts.clear();
	m_mapDouts.clear();

	m_arrChannels.clear();
	m_arrDevices.clear();
	m_arrAins.clear();
	m_arrDins.clear();
	m_arrAouts.clear();
	m_arrDouts.clear();	
}
/*! \fn bool CMemDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CMemDB::Initialize 
** \details 初始化
** \param pszDataPath 共享内存名字，通常就是proj的全路径名称
** \param nMode
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CMemDB::Initialize(const char *pszDataPath, unsigned int nMode)
{
	bool bRet = BuildMemDB(pszDataPath);
	if (bRet == false)
	{
		return false;
	}
	
	m_pDBAliveFlag = std::make_shared<ACE_Event>(1, 0, USYNC_PROCESS, pszDataPath);
	 
 

	
	return true;
}

void CMemDB::Run()
{
// 	if (Mail::RecvMail(SMKEY_MAIL_DBGSVR, m_mailbuf, MAIL_BUF_SIZE)==K_FALSE)
// 		return;
// 
// 	MSG_BASE* pMsgBase = (MSG_BASE*)m_mailbuf;

//	int nType = pMsgBase->Type;
}

void CMemDB::Shutdown()
{
//	StopModule();

//	Mail::DestroyMailBox(SMKEY_MAIL_DBGSVR);
}
void  CMemDB::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("FESAPI", pszText, nLevel, nullptr);
}
/*! \fn size_t CMemDB::CreateNodeTable(unsigned char *pHead)
********************************************************************************************************* 
** \brief CMemDB::CreateNodeTable 
** \details 从内存中加载数据，并创建节点表
** \param pHead 
** \return size_t 
** \author LiJin 
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
size_t CMemDB::CreateNodeTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	m_pNodes = reinterpret_cast<NODE*>(pHead);
	QString szLog;

	int32u i = 0;
	// 创建节点表
	for (i = 0; i < m_pMagicMem->NodeCount; i++)
	{
		Q_ASSERT(m_pNodes[i].OccNo != INVALID_OCCNO && m_pNodes[i].OccNo <= MAX_NODE_OCCNO);
		if (m_pNodes[i].OccNo == INVALID_OCCNO || m_pNodes[i].OccNo > MAX_NODE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodes[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		std::shared_ptr<CNode> pNode = std::make_shared<CNode >(&m_pNodes[i]);
		m_mapNodes.insert(std::make_pair(m_pNodes[i].OccNo, pNode));
	}
	
	return sizeof(NODE)* (m_pMagicMem->NodeCount);
}

size_t CMemDB::CreateChannelTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	m_pChannels = reinterpret_cast<CHANNEL*>(pHead);
	int32u i = 0;
	QString szLog;
	for (i = 0; i < m_pMagicMem->ChannelCount; i++)
	{
		Q_ASSERT(m_pChannels[i].OccNo != INVALID_OCCNO && m_pChannels[i].OccNo <= MAX_CHANNEL_OCCNO);
		if (m_pChannels[i].OccNo == INVALID_OCCNO || m_pChannels[i].OccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] channel's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pChannels[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrChannels.push_back(&m_pChannels[i]);
			continue;
		}
		m_arrChannels.push_back(&m_pChannels[i]);

		std::shared_ptr<CFesChannel> pChannel = std::make_shared<CFesChannel >(&m_pChannels[i]);
		pChannel->m_fnSetDeviceQua = std::bind(&CMemDB::IoSetDeviceQua, this, std::placeholders::_1, std::placeholders::_2);

		m_mapChannels.insert(std::make_pair(m_pChannels[i].OccNo, pChannel));
	}
	return sizeof(CHANNEL)* (m_pMagicMem->ChannelCount);
}

size_t  CMemDB::CreateDeviceTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	m_pDevices = reinterpret_cast<DEVICE*>(pHead);

	int32u i = 0;
	QString szLog;

	for (i = 0; i < m_pMagicMem->DeviceCount; i++)
	{
		Q_ASSERT(m_pDevices[i].OccNo != INVALID_OCCNO && m_pDevices[i].OccNo <= MAX_DEVICE_OCCNO);
		if (m_pDevices[i].OccNo == INVALID_OCCNO || m_pDevices[i].OccNo > MAX_DEVICE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] device's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDevices[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrDevices.push_back(&m_pDevices[i]);
			continue;
		}
		m_arrDevices.push_back(&m_pDevices[i]);

		std::shared_ptr<CFesDevice> pDevice = std::make_shared<CFesDevice >(&m_pDevices[i]);

		pDevice->m_fnSetAinQua = std::bind(&CMemDB::IoSetAinQua, this, std::placeholders::_1, std::placeholders::_2);
		pDevice->m_fnSetDinQua = std::bind(&CMemDB::IoSetDinQua, this, std::placeholders::_1, std::placeholders::_2);
		pDevice->m_fnSetAoutQua = std::bind(&CMemDB::IoSetAoutQua, this, std::placeholders::_1, std::placeholders::_2);
		pDevice->m_fnSetDoutQua = std::bind(&CMemDB::IoSetDoutQua, this, std::placeholders::_1, std::placeholders::_2);

		m_mapDevices.insert(std::make_pair(m_pDevices[i].OccNo, pDevice));
	}
	return sizeof(DEVICE)* (m_pMagicMem->DeviceCount);
}
/*! \fn size_t CMemDB::CreateAinTable(unsigned char *pHead)
********************************************************************************************************* 
** \brief CMemDB::CreateAinTable 
** \details 创建模拟量表
** \param pHead 
** \return size_t 
** \author LiJin 
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
size_t CMemDB::CreateAinTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	m_pAins = reinterpret_cast<AIN*>(pHead);

	int32u i = 0;
	QString szLog;

	m_mapAins.reserve(m_pMagicMem->AinCount *1.2);
	for (i = 0; i < m_pMagicMem->AinCount; i++)
	{
		Q_ASSERT(m_pAins[i].OccNo != INVALID_OCCNO && m_pAins[i].OccNo <= MAX_OCCNO);
		if (m_pAins[i].OccNo == INVALID_OCCNO || m_pAins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrAins.push_back(&m_pAins[i]);
			continue;
		}
		m_arrAins.push_back(&m_pAins[i]);

		std::shared_ptr<CFesAin> pAin = std::make_shared<CFesAin >(&m_pAins[i]);

		pAin->m_fnGetAlarm = std::bind(&CMemDB::GetAinAlarm, this, std::placeholders::_1);
		pAin->m_fnGetTransformLinear = std::bind(&CMemDB::GetTransformLinear, this, std::placeholders::_1);
		pAin->m_fnGetTransformNonLinear = std::bind(&CMemDB::GetTransformNonLinear, this, std::placeholders::_1);
		pAin->m_fnGetChannel = std::bind(&CMemDB::GetChannelPtr, this, std::placeholders::_1, std::placeholders::_2);
		pAin->m_fnGetDevice = std::bind(&CMemDB::GetDevicePtr, this, std::placeholders::_1, std::placeholders::_2);

		pAin->LoadConfig();
		
		m_mapAins.insert(std::make_pair(m_pAins[i].OccNo, pAin));
	}
	return sizeof(AIN)* (m_pMagicMem->AinCount);
}

size_t CMemDB::CreateDinTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;
	
	int32u i = 0;
	QString szLog;

	m_pDins = reinterpret_cast<DIN*>(pHead);
	for (i = 0; i < m_pMagicMem->DinCount; i++)
	{
		Q_ASSERT(m_pDins[i].OccNo != INVALID_OCCNO && m_pDins[i].OccNo <= MAX_OCCNO);
		if (m_pDins[i].OccNo == INVALID_OCCNO || m_pDins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrDins.push_back(&m_pDins[i]);
			continue;
		}
		m_arrDins.push_back(&m_pDins[i]);

		std::shared_ptr<CFesDin> pDin = std::make_shared<CFesDin >(&m_pDins[i]);

		pDin->m_fnGetAlarm = std::bind(&CMemDB::GetDinAlarm, this, std::placeholders::_1);
		pDin->m_fnGetChannel = std::bind(&CMemDB::GetChannelPtr, this, std::placeholders::_1, std::placeholders::_2);
		pDin->m_fnGetDevice = std::bind(&CMemDB::GetDevicePtr, this, std::placeholders::_1, std::placeholders::_2);

		pDin->LoadConfig();

		m_mapDins.insert(std::make_pair(m_pDins[i].OccNo, pDin));
	}
	return sizeof(DIN)* (m_pMagicMem->DinCount);
}

size_t CMemDB::CreateAoutTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;
	
	int32u i = 0;
	QString szLog;

	m_pAouts = reinterpret_cast<AOUT*>(pHead);
	for (i = 0; i < m_pMagicMem->AoutCount; i++)
	{
		Q_ASSERT(m_pAouts[i].OccNo != INVALID_OCCNO && m_pAouts[i].OccNo <= MAX_OCCNO);
		if (m_pAouts[i].OccNo == INVALID_OCCNO || m_pAouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrAouts.push_back(&m_pAouts[i]);
			continue;
		}
		m_arrAouts.push_back(&m_pAouts[i]);

		std::shared_ptr<CFesAout> pAout = std::make_shared<CFesAout >(&m_pAouts[i]);

		pAout->m_fnGetTransformLinear = std::bind(&CMemDB::GetTransformLinear, this, std::placeholders::_1);
		pAout->m_fnGetTransformNonLinear = std::bind(&CMemDB::GetTransformNonLinear, this, std::placeholders::_1);
	 	pAout->m_fnGetChannel = std::bind(&CMemDB::GetChannelPtr, this, std::placeholders::_1, std::placeholders::_2);
		pAout->m_fnGetDevice = std::bind(&CMemDB::GetDevicePtr, this, std::placeholders::_1, std::placeholders::_2);

		m_mapAouts.insert(std::make_pair(m_pAouts[i].OccNo, pAout));
	}

	return sizeof(AOUT)* (m_pMagicMem->AoutCount);
}

size_t CMemDB::CreateDoutTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pDouts = reinterpret_cast<DOUT*>(pHead);
	for (i = 0; i < m_pMagicMem->DoutCount; i++)
	{
		Q_ASSERT(m_pDouts[i].OccNo != INVALID_OCCNO && m_pDouts[i].OccNo <= MAX_OCCNO);
		if (m_pDouts[i].OccNo == INVALID_OCCNO || m_pDouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrDouts.push_back(&m_pDouts[i]);
			continue;
		}
		m_arrDouts.push_back(&m_pDouts[i]);

		std::shared_ptr<CFesDout> pDout = std::make_shared<CFesDout >(&m_pDouts[i]);
		
		pDout->m_fnGetChannel = std::bind(&CMemDB::GetChannelPtr, this, std::placeholders::_1, std::placeholders::_2);
		pDout->m_fnGetDevice = std::bind(&CMemDB::GetDevicePtr, this, std::placeholders::_1, std::placeholders::_2);

		m_mapDouts.insert(std::make_pair(m_pDouts[i].OccNo, pDout));
	}
	return sizeof(DOUT)* (m_pMagicMem->DoutCount);
}

size_t  CMemDB::CreateLinearTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pLinear = reinterpret_cast<TRANSFORM_LINEAR*>(pHead);
	for (i = 0; i < m_pMagicMem->LinearCount; i++)
	{
		Q_ASSERT(m_pLinear[i].OccNo != INVALID_OCCNO && m_pLinear[i].OccNo <= MAX_OCCNO);
		if (m_pLinear[i].OccNo == INVALID_OCCNO || m_pLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] Linear Transformer's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrLinears.push_back(&m_pLinear[i]);
			continue;
		}
		m_arrLinears.push_back(&m_pLinear[i]);
// 		std::shared_ptr<CFesDout> pDout = std::make_shared<CFesDout >(&m_pDouts[i]);
// 		m_mapDouts.insert(std::make_pair(m_pDouts[i].OccNo, pDout));
	}
	return sizeof(TRANSFORM_LINEAR)* (m_pMagicMem->LinearCount);
}

size_t  CMemDB::CreateNonLinearTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pNonLinear = reinterpret_cast<TRANSFORM_NONLINEAR*>(pHead);
	for (i = 0; i < m_pMagicMem->NonLinearCount; i++)
	{
		Q_ASSERT(m_pNonLinear[i].OccNo != INVALID_OCCNO && m_pNonLinear[i].OccNo <= MAX_OCCNO);
		if (m_pNonLinear[i].OccNo == INVALID_OCCNO || m_pNonLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] NonLinear Transformer's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNonLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrNonLinears.push_back(&m_pNonLinear[i]);
			continue;
		}
		m_arrNonLinears.push_back(&m_pNonLinear[i]);
	}
	return sizeof(TRANSFORM_NONLINEAR)* (m_pMagicMem->NonLinearCount);
}

size_t  CMemDB::CreateAinAlarmTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pAinAlarm = reinterpret_cast<AIN_ALARM*>(pHead);
	for (i = 0; i < m_pMagicMem->AinAlarmCount; i++)
	{
		Q_ASSERT(m_pAinAlarm[i].OccNo != INVALID_OCCNO && m_pAinAlarm[i].OccNo <= MAX_OCCNO);
		if (m_pAinAlarm[i].OccNo == INVALID_OCCNO || m_pAinAlarm[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain_alarm's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAinAlarm[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrAinAlarms.push_back(&m_pAinAlarm[i]);
			continue;
		}
		m_arrAinAlarms.push_back(&m_pAinAlarm[i]);

		std::shared_ptr<CAinAlarm> pAinAlarm = std::make_shared<CAinAlarm >(&m_pAinAlarm[i]);
		pAinAlarm->m_fnGetChannel = std::bind(&CMemDB::GetChannel, this, std::placeholders::_1, std::placeholders::_2);

		m_mapAinAlarms.insert(std::make_pair(m_pAinAlarm[i].OccNo, pAinAlarm));
	}
	// 创建 模拟量报警限值表
	m_pAinAlarmLimit = reinterpret_cast<AIN_ALARM_LIMIT*>(pHead+ sizeof(AIN_ALARM)* (m_pMagicMem->AinAlarmCount));
	for (i = 0; i < m_pMagicMem->AinAlarmLimitCount; i++)
	{
		Q_ASSERT(m_pAinAlarmLimit[i].OccNo != INVALID_OCCNO && m_pAinAlarmLimit[i].OccNo <= MAX_OCCNO);
		if (m_pAinAlarmLimit[i].OccNo == INVALID_OCCNO || m_pAinAlarmLimit[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAinAlarmLimit[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrAinAlarmLimits.push_back(&m_pAinAlarmLimit[i]);
			continue;
		}
		m_arrAinAlarmLimits.push_back(&m_pAinAlarmLimit[i]);

		Q_ASSERT(m_pAinAlarmLimit[i].AlarmOccNo != INVALID_OCCNO && m_pAinAlarmLimit[i].AlarmOccNo <= MAX_OCCNO);
		if (m_pAinAlarmLimit[i].AlarmOccNo != INVALID_OCCNO && m_pAinAlarmLimit[i].AlarmOccNo <= MAX_OCCNO)
		{
			// 找到所属告警，添加进去
			auto pAinAlarm = GetAinAlarm(m_pAinAlarmLimit[i].AlarmOccNo);
			Q_ASSERT(pAinAlarm);
			if (pAinAlarm != nullptr)
			{
				pAinAlarm->AddItem(&m_pAinAlarmLimit[i]);
			}
		}
	}

	return sizeof(AIN_ALARM)* m_pMagicMem->AinAlarmCount + sizeof(AIN_ALARM_LIMIT) *m_pMagicMem->AinAlarmLimitCount;
}

size_t CMemDB::CreateDinAlarmTable(unsigned char *pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pDinAlarm = reinterpret_cast<DIN_ALARM*>(pHead);
	for (i = 0; i < m_pMagicMem->DinAlarmCount; i++)
	{
		Q_ASSERT(m_pDinAlarm[i].OccNo != INVALID_OCCNO && m_pDinAlarm[i].OccNo <= MAX_OCCNO);
		if (m_pDinAlarm[i].OccNo == INVALID_OCCNO || m_pDinAlarm[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDinAlarm[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrDinAlarms.push_back(&m_pDinAlarm[i]);
			continue;
		}
		m_arrDinAlarms.push_back(&m_pDinAlarm[i]);

		std::shared_ptr<CDinAlarm> pDinAlarm = std::make_shared<CDinAlarm >(&m_pDinAlarm[i]);
		m_mapDinAlarms.insert(std::make_pair(m_pDinAlarm[i].OccNo, pDinAlarm));
	}

	// 创建 开关量报警限值表
	m_pDinAlarmLimit = reinterpret_cast<DIN_ALARM_LIMIT*>(pHead + sizeof(DIN_ALARM)* (m_pMagicMem->DinAlarmCount));

	for (i = 0; i < m_pMagicMem->DinAlarmLimitCount; i++)
	{
		Q_ASSERT(m_pDinAlarmLimit[i].OccNo != INVALID_OCCNO && m_pDinAlarmLimit[i].OccNo <= MAX_OCCNO);
		if (m_pDinAlarmLimit[i].OccNo == INVALID_OCCNO || m_pDinAlarmLimit[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDinAlarmLimit[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrDinAlarmLimits.push_back(&m_pDinAlarmLimit[i]);
			continue;
		}
		m_arrDinAlarmLimits.push_back(&m_pDinAlarmLimit[i]);

		Q_ASSERT(m_pDinAlarmLimit[i].AlarmOccNo != INVALID_OCCNO && m_pDinAlarmLimit[i].AlarmOccNo <= MAX_OCCNO);
		if (m_pDinAlarmLimit[i].AlarmOccNo != INVALID_OCCNO && m_pDinAlarmLimit[i].AlarmOccNo <= MAX_OCCNO)
		{
			// 找到所属告警，添加进去
			auto pDinAlarm = GetDinAlarm(m_pDinAlarmLimit[i].AlarmOccNo);
			Q_ASSERT(pDinAlarm);
			if (pDinAlarm != nullptr)
			{
				pDinAlarm->AddItem(&m_pDinAlarmLimit[i]);
			}
		}
	}

	return sizeof(DIN_ALARM)* m_pMagicMem->DinAlarmCount + sizeof(DIN_ALARM_LIMIT) *m_pMagicMem->DinAlarmLimitCount;
}

size_t CMemDB::CreateSystemVariableTable(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pSystemVariable = reinterpret_cast<VARDATA*>(pHead);
	for (i = 0; i < m_pMagicMem->SystemVariableCount; i++)
	{
		Q_ASSERT(m_pSystemVariable[i].OccNo != INVALID_OCCNO && m_pSystemVariable[i].OccNo <= MAX_OCCNO);
		if (m_pSystemVariable[i].OccNo == INVALID_OCCNO || m_pSystemVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] systemvariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrSystemVariables.push_back(&m_pSystemVariable[i]);
			continue;
		}
		m_arrSystemVariables.push_back(&m_pSystemVariable[i]);

	}
	return sizeof(VARDATA)* (m_pMagicMem->SystemVariableCount);
}

size_t CMemDB::CreateUserVariableTable(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (pHead == nullptr)
		return 0;

	int32u i = 0;
	QString szLog;

	m_pUserVariable = reinterpret_cast<VARDATA*>(pHead);
	for (i = 0; i < m_pMagicMem->UserVariableCount; i++)
	{
		Q_ASSERT(m_pUserVariable[i].OccNo != INVALID_OCCNO && m_pUserVariable[i].OccNo <= MAX_OCCNO);
		if (m_pUserVariable[i].OccNo == INVALID_OCCNO || m_pUserVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] uservariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			m_arrUserVariables.push_back(&m_pUserVariable[i]);
			continue;
		}
		m_arrUserVariables.push_back(&m_pUserVariable[i]);

	}
	return sizeof(VARDATA)* (m_pMagicMem->UserVariableCount);
}

/*! \fn bool CMemDB::BuildMemDB(const char *szDBName)
********************************************************************************************************* 
** \brief CMemDB::BuildMemDB 
** \details 建立内存数据库
** \param szDBName 
** \return bool 
** \author LiJin 
** \date 2016年9月16日 
** \note todo需要统计下性能时间
********************************************************************************************************/
bool CMemDB::BuildMemDB(const char *szDBName)
{
	Q_ASSERT(szDBName);
	if (szDBName == nullptr)
		return false;
	Q_ASSERT(strlen(szDBName));
	if (strlen(szDBName) <= 0)
		return false;

	if (m_bInitialized == true)
	{
		// todo log

		return true;
	}
	QString szLog;

	char * pData = (char*)m_pMem->GetShareMem(szDBName/*QFileInfo(szDBName).baseName().toStdString().c_str()*/);
	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		szLog = QString(QObject::tr("Open share memory [ %1 ] failed")).arg(szDBName);
		LogMsg(szLog.toStdString().c_str(), 0);
		return false;
	}

	m_pMem->Lock();

	m_pMagicMem = (HEAD_MEM*)(pData);

	Q_ASSERT(m_pMagicMem->MagicHead1 == MAGIC_HEAD);
	Q_ASSERT(m_pMagicMem->MagicHead2 == MAGIC_HEAD);
	 
// 	m_pMagicMem->ShmLength = nSize;
// 	strncpy(m_pMagicMem->Config, szDBName, qMin(0, 0));
	Q_ASSERT(m_pMagicMem->NodeCount >0 && m_pMagicMem->NodeCount <= MAX_NODE_COUNT);
	if (m_pMagicMem->NodeCount == 0 || m_pMagicMem->NodeCount > MAX_NODE_COUNT)
	{
		m_pMem->UnLock();

		szLog = QString(QObject::tr("The node's count [ %1 ] in memory db is wrong ")).arg(m_pMagicMem->NodeCount);
		LogMsg(szLog.toStdString().c_str(), 0);		
		return false;
	}

	m_nNodeCount = m_pMagicMem->NodeCount;
	m_nChannelCount = m_pMagicMem->ChannelCount;
	m_nDeviceCount = m_pMagicMem->DeviceCount;
	m_nAinCount = m_pMagicMem->AinCount;
	m_nDinCount = m_pMagicMem->DinCount;
	m_nDoutCount = m_pMagicMem->DoutCount;
	m_nAoutCount = m_pMagicMem->AoutCount;

	m_nLinearCount = m_pMagicMem->LinearCount;
	m_nNonLinearCount = m_pMagicMem->NonLinearCount;

	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_pAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;

	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;

	m_nSystemVariableCount = m_pMagicMem->SystemVariableCount;
	m_nUserVariableCount = m_pMagicMem->UserVariableCount;

	//NODE
	m_pNodes = reinterpret_cast<NODE*>(pData + sizeof(HEAD_MEM));

	unsigned char *pHead = reinterpret_cast<unsigned char*>(pData + sizeof(HEAD_MEM));
	size_t nRet = 0;

	int32u i = 0;
	// 创建节点表
	nRet = CreateNodeTable(pHead);
	pHead += nRet;
	// 创建通道表
	nRet = CreateChannelTable(pHead);
	pHead += nRet;
	// 创建装置表
	nRet = CreateDeviceTable(pHead);
	pHead += nRet;	

	// 创建 模拟量报警表，注意，告警表一定要放在模拟量表前面
	nRet = CreateAinAlarmTable(pHead);
	pHead += nRet;

	nRet = CreateDinAlarmTable(pHead);
	pHead += nRet;

	// 创建模拟量表
	nRet = CreateAinTable(pHead);
	pHead += nRet;
	// 创建开关量表
	nRet = CreateDinTable(pHead);
	pHead += nRet;
	// 创建模出量表	
	nRet = CreateAoutTable(pHead);
	pHead += nRet;
	// 创建开出量表	
	nRet = CreateDoutTable(pHead);
	pHead += nRet;
	// 创建 线性转换表
	nRet = CreateLinearTable(pHead);
	pHead += nRet;
	// 创建 非线性转换表
	nRet = CreateNonLinearTable(pHead);
	pHead += nRet;

	nRet = CreateSystemVariableTable(pHead);
	pHead += nRet;

	nRet = CreateUserVariableTable(pHead);
	pHead += nRet;

	// 处理变长区
	unsigned char * pRefData = (unsigned char*)(pHead);

	size_t nSize = 0;
	// 通道
	for (i = 0; i < m_pMagicMem->ChannelCount; i++)
	{
		CHANNEL_REF *pChannelRef = reinterpret_cast<CHANNEL_REF*>(pRefData);
		Q_ASSERT(pChannelRef->MagicHead == MAGIC_HEAD);
		Q_ASSERT(pChannelRef->IddType == IDD_CHANNEL);
		
		if (pChannelRef->MagicHead == MAGIC_HEAD && pChannelRef->IddType == IDD_CHANNEL)
		{
			Q_ASSERT(pChannelRef->ChannelOccNo != INVALID_OCCNO && pChannelRef->ChannelOccNo <= MAX_CHANNEL_OCCNO);

			auto it = m_mapChannels.find(pChannelRef->ChannelOccNo);
			Q_ASSERT(it != m_mapChannels.end());
			if (it != m_mapChannels.end())
			{
				Q_ASSERT(it->second);
				if (it->second)
				{
					nSize =it->second->LoadFromMem(pRefData);
					pRefData += nSize;
				}				
			}			
		}
	}
	// 装置
	for (i = 0; i < m_pMagicMem->DeviceCount; i++)
	{
		DEVICE_REF *pDeviceRef = reinterpret_cast<DEVICE_REF*>(pRefData);
		Q_ASSERT(pDeviceRef->MagicHead == MAGIC_HEAD);
		Q_ASSERT(pDeviceRef->IddType == IDD_DEVICE);

		if (pDeviceRef->MagicHead == MAGIC_HEAD && pDeviceRef->IddType == IDD_DEVICE)
		{
			Q_ASSERT(pDeviceRef->DeviceOccNo != INVALID_OCCNO && pDeviceRef->DeviceOccNo <= MAX_DEVICE_OCCNO);

			auto it = m_mapDevices.find(pDeviceRef->DeviceOccNo);
			Q_ASSERT(it != m_mapDevices.end());
			if (it != m_mapDevices.end())
			{
				Q_ASSERT(it->second);
				if (it->second)
				{
					nSize = it->second->LoadFromMem(pRefData);
					pRefData += nSize;
				}
			}
		}
	}

	m_pMem->UnLock();
	return true;
}

int8u  CMemDB::GetMyHostScanEnable()const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].ScanEnable;
}
/*! \fn int32u CMemDB::GetMyHostState()const
********************************************************************************************************* 
** \brief CMemDB::GetMyHostState 
** \details 
** \return int32u 
** \author LiJin 
** \date 2016年12月1日 
** \note 
********************************************************************************************************/
int32u CMemDB::GetMyHostState() const
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return STATE_UNKOWN;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	if (m_pMagicMem->MyNodeOccNo == INVALID_OCCNO || m_pMagicMem->MyNodeOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	return m_pNodes[m_pMagicMem->MyNodeOccNo - 1].State;
}
/*! \fn int32u CMemDB::GetMyNodeOccNo() const
********************************************************************************************************* 
** \brief CMemDB::GetMyNodeOccNo 
** \details 获取本节点的排行号
** \return int32u 
** \author LiJin 
** \date 2016年12月2日 
** \note 
********************************************************************************************************/
int32u CMemDB::GetMyNodeOccNo() const 
{
	Q_ASSERT(m_pMagicMem);
	if (m_pMagicMem == nullptr)
		return INVALID_OCCNO;

	Q_ASSERT(m_pMagicMem->MyNodeOccNo != INVALID_OCCNO && m_pMagicMem->MyNodeOccNo <= MAX_NODE_OCCNO);
	return m_pMagicMem->MyNodeOccNo;
}

int32u CMemDB::GetNodeState(int32u nOccNo)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
		return STATE_UNKOWN;

	if (nOccNo > m_nNodeCount)
	{
		return STATE_UNKOWN;
	}

	return m_pNodes[nOccNo - 1].State;
}
/*! \fn int32u CMemDB::GetNodeOccNo(const char *pszHostName)
********************************************************************************************************* 
** \brief CMemDB::GetNodeOccNo 
** \details  获取节点的OCCNO
** \param pszHostName 
** \return int32u 
** \author LiJin 
** \date 2016年9月21日 
** \note 
********************************************************************************************************/
int32u CMemDB::GetNodeOccNoByHostName(const char *pszHostName)
{
	Q_ASSERT(pszHostName);
	if (pszHostName == nullptr)
		return INVALID_OCCNO;
	Q_ASSERT(strlen(pszHostName) > 0);
	if (strlen(pszHostName) == 0)
		return INVALID_OCCNO;

	int32u nOccNo = INVALID_OCCNO;
	unsigned int i = 0;
	for (i = 0; i < m_nNodeCount; i++)
	{
		if (strcmp(pszHostName, m_pNodes[i].HostName) == 0  && m_pNodes[i].NodeType == NODE_FES)
		{
			nOccNo = m_pNodes[i].OccNo;
			break;
		}
	}

	return nOccNo;
}
/*! \fn bool CMemDB::OpenChannelMailBox(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief CMemDB::OpenChannelMailBox 
** \details 主要用于进程内外 打开驱动邮箱  若为进程内通信 则不需要打开 若进程外通信  则需要打开 否则驱动无法收到对应的下行邮件
** \param nChannelOccNo 
** \return bool 
** \author xingzhibing
** \date 2017年5月24日 
** \note 
********************************************************************************************************/
bool CMemDB::OpenChannelMailBox(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return  false;

	if (nChannelOccNo > m_nChannelCount)
	{
		return  false;
	}

	Q_ASSERT(m_pChannels[nChannelOccNo - 1].MailBoxID);

	int nMailBoxID;
	nMailBoxID = m_pChannels[nChannelOccNo - 1].MailBoxID;

	bool bRet = false;
	
	bRet = OpenPostOffice("FES");

	Q_ASSERT(bRet);

	if (!bRet)
	{
		return false;
	}
	
	bRet = OpenMailBoxByID("FES",nMailBoxID);

	Q_ASSERT(bRet);

	if (!bRet)
	{
		return false;
	}

	return bRet;
}
/*! \fn bool CMemDB::CloseChannelMailBox(int32u nChannelOcNo)
********************************************************************************************************* 
** \brief CMemDB::CloseChannelMailBox 
** \details 
** \param nChannelOcNo 
** \return bool 
** \author xingzhibing
** \date 2017年5月25日 
** \note 
********************************************************************************************************/
bool CMemDB::CloseChannelMailBox(int32u nChannelOcNo)
{
	Q_ASSERT(nChannelOcNo != INVALID_OCCNO && nChannelOcNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOcNo == INVALID_OCCNO || nChannelOcNo > MAX_CHANNEL_OCCNO)
		return  false;

	if (nChannelOcNo > m_nChannelCount)
	{
		return  false;
	}

	Q_ASSERT(m_pChannels[nChannelOcNo - 1].MailBoxID);

	int nMailBoxID = 0;
	nMailBoxID = m_pChannels[nChannelOcNo - 1].MailBoxID;

	return  CloseMailBoxByID("FES",nMailBoxID);
}
/*! \fn size_t CMemDB::GetDeviceCount(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief CMemDB::GetDeviceCount 
** \details 获取某个通道下的装置的数量
** \param nChannelOccNo 
** \return size_t 
** \author LiJin 
** \date 2016年9月18日 
** \note 
********************************************************************************************************/
size_t CMemDB::GetDeviceCount(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDeviceCount();
		}		
	}
	return 0;
}

size_t  CMemDB::GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDeviceOccNos(arrOccNos);
		}		
	}
	return 0;
}

size_t  CMemDB::GetAinCountInDevice( int32u nDeviceOccNo)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAinCount( );
		}
	}
	return 0;
}

size_t  CMemDB::GetDinCountInDevice( int32u nDeviceOccNo)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDinCount();
		}
	}
	return 0;
}

size_t  CMemDB::GetAoutCountInDevice( int32u nDeviceOccNo)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAoutCount();
		}
	}
	return 0;
}

size_t  CMemDB::GetDoutCountInDevice(int32u nDeviceOccNo)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDoutCount();
		}
	}
	return 0;
}

size_t CMemDB::GetAinCountInChannel(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAinCount();
		}
	}
	return 0;
}

size_t CMemDB::GetDinCountInChannel(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDinCount();
		}
	}
	return 0;
}

size_t CMemDB::GetAoutCountInChannel(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAoutCount();
		}
	}
	return 0;
}

size_t CMemDB::GetDoutCountInChannel(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDoutCount();
		}
	}
	return 0;
}

size_t CMemDB::GetAinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAinOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetDinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDinOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetAoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAoutOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetDoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	auto it = m_mapChannels.find(nChannelOccNo);
	Q_ASSERT(it != m_mapChannels.end());
	if (it != m_mapChannels.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDoutOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetAinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAinOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetDinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDinOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetAoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetAoutOccNos(arrOccNos);
		}
	}
	return 0;
}

size_t CMemDB::GetDoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return 0;

	auto it = m_mapDevices.find(nDeviceOccNo);
	Q_ASSERT(it != m_mapDevices.end());
	if (it != m_mapDevices.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->GetDoutOccNos(arrOccNos);
		}
	}
	return 0;
}

bool CMemDB::GetChannelPtr(int32u nChannelOccNo, CHANNEL **ppFB)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return false;

	if (nChannelOccNo > m_nChannelCount)
	{
		return false;
	}
	*ppFB = &m_pChannels[nChannelOccNo - 1];
	return true;
}

bool  CMemDB::GetDevicePtr(int32u nDeviceOccNo, DEVICE** ppFB)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return false;

	if (nDeviceOccNo > m_nDeviceCount)
	{
		return false;
	}

	*ppFB = &m_pDevices[nDeviceOccNo - 1];
	return true;
}

bool  CMemDB::GetAinPtr(int32u nOccNo, AIN** ppFB)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	*ppFB = &m_pAins[nOccNo - 1];
	return true;
}

bool  CMemDB::GetDinPtr(int32u nOccNo, DIN** ppFB)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}

	*ppFB = &m_pDins[nOccNo - 1];
	return true;
}

bool  CMemDB::GetAoutPtr(int32u nOccNo, AOUT** ppFB)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAoutCount)
	{
		return false;
	}

	*ppFB = &m_pAouts[nOccNo - 1];
	return true;
}

bool  CMemDB::GetDoutPtr(int32u nOccNo, DOUT** ppFB)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDoutCount)
	{
		return false;
	}

	*ppFB = &m_pDouts[nOccNo - 1];
	return true;
}

bool CMemDB::GetChannel(int32u nChannelOccNo, CHANNEL& ch)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return false;

	if (nChannelOccNo > m_nChannelCount)
	{
		return false;
	}
	std::memcpy(&ch,& m_pChannels[nChannelOccNo - 1], sizeof(CHANNEL));
	return true;
}

bool CMemDB::GetDevice(int32u nDeviceOccNo, DEVICE &device)
{
	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return false;

	if (nDeviceOccNo > m_nDeviceCount)
	{
		return false;
	}
	std::memcpy(&device, &m_pDevices[nDeviceOccNo - 1], sizeof(DEVICE));
	return true;
}

bool CMemDB::GetAin(int32u nOccNo, AIN &ai)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	std::memcpy(&ai, &m_pAins[nOccNo - 1], sizeof(AIN));
	return true;
}

bool CMemDB::GetDin(int32u nOccNo, DIN &di)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	std::memcpy(&di, &m_pDins[nOccNo - 1], sizeof(DIN));
	return true;
}

bool CMemDB::GetAout(int32u nOccNo, AOUT &aq)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	std::memcpy(&aq, &m_pAouts[nOccNo - 1], sizeof(AOUT));
	return true;
}

bool CMemDB::GetDout(int32u nOccNo, DOUT &dq)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	std::memcpy(&dq, &m_pDouts[nOccNo - 1], sizeof(DOUT));
	return true;
}

char* CMemDB::GetAinAddress(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;
	
	if (nOccNo > m_nAinCount)
	{
		return nullptr;
	}

	return m_pAins[nOccNo - 1].Address;
}

char* CMemDB::GetDinAddress(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	if (nOccNo > m_nDinCount)
	{
		return nullptr;
	}

	return m_pDins[nOccNo - 1].Address;
}

char* CMemDB::GetAoutAddress(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	if (nOccNo > m_nAoutCount)
	{
		return nullptr;
	}

	return m_pAouts[nOccNo - 1].Address;
}

char* CMemDB::GetDoutAddress(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	if (nOccNo > m_nDoutCount)
	{
		return nullptr;
	}

	return m_pDouts[nOccNo - 1].Address;
}


bool CMemDB::IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality, TIMEPAK * pTm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
	auto it = m_mapAins.find(nOccNo);
	Q_ASSERT(it != m_mapAins.end());
	if (it != m_mapAins.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->Update(fValue, nQuality,pTm);
		}
	}
	return true;
}
/*! \fn bool CMemDB::IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetAinValue 
** \details IO驱动更新测值，此处用到了map，性能如何尚需验证
** \param nOccNo 
** \param fValue 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年9月19日 
** \note 
********************************************************************************************************/
bool CMemDB::IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality)
{
	return IoSetAinValue(nOccNo, fValue, nQuality, nullptr);
}

bool CMemDB::IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	auto it = m_mapAouts.find(nOccNo);
	Q_ASSERT(it != m_mapAouts.end());
	if (it != m_mapAouts.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->Update(fValue, nQuality);
		}
	}
	return true;
}

bool CMemDB::IoSetDoutValue(int32u nOccNo, int8u nValue, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	auto it = m_mapDouts.find(nOccNo);
	Q_ASSERT(it != m_mapDouts.end());
	if (it != m_mapDouts.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->Update(nValue, nQuality);
		}
	}
	return true;
}
/*! \fn bool  CMemDB::IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality)const
********************************************************************************************************* 
** \brief CMemDB::IoGetAinValue 
** \details 获取AIN的测值以及品质
** \param nOccNo 
** \param fValue 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月9日 
** \note 
********************************************************************************************************/
bool  CMemDB::IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	auto it = m_mapAins.find(nOccNo);
	Q_ASSERT(it != m_mapAins.end());
	if (it != m_mapAins.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			it->second->GetValue(fValue, nQuality);
			return true;
		}
	}
	return false;
}

bool CMemDB::IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality, bool bCpuEnable, TIMEPAK * pTm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	auto it = m_mapDins.find(nOccNo);
	Q_ASSERT(it != m_mapDins.end());
	if (it != m_mapDins.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->Update(nValue, nQuality,bCpuEnable,pTm);
		}
	}
	return true;
}
/*! \fn bool CMemDB::IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetDinValue 
** \details IO驱动更新测值
** \param nOccNo 
** \param nValue 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年9月19日 
** \note 
********************************************************************************************************/
bool CMemDB::IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality)
{
	return IoSetDinValue(nOccNo, nValue, nQuality, true,nullptr);
}

bool CMemDB::IoDinAlarm(int32u nOccNo, int8u nValue, int8u nQuality, TIMEPAK * pTm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(pTm);
	if (pTm == nullptr)
		return false;

	auto it = m_mapDins.find(nOccNo);
	Q_ASSERT(it != m_mapDins.end());
	if (it != m_mapDins.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			return it->second->Alarm(nValue, nQuality, pTm);
		}
	}
	return true;
}
/*! \fn bool  CMemDB::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
********************************************************************************************************* 
** \brief CMemDB::IoGetDinValue 
** \details 获取DIN的测值以及品质
** \param nOccNo 
** \param nValue 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月9日 
** \note 
********************************************************************************************************/
bool  CMemDB::IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	auto it = m_mapDins.find(nOccNo);
	Q_ASSERT(it != m_mapDins.end());
	if (it != m_mapDins.end())
	{
		Q_ASSERT(it->second);
		if (it->second)
		{
			it->second->GetValue(nValue, nQuality);
			return true;
		}
	}
	return false;
}
/*! \fn std::shared_ptr<CAinAlarm> CMemDB::GetAinAlarm(int32u nOccNo)
********************************************************************************************************* 
** \brief CMemDB::GetAinAlarm 
** \details 根据occno获取报警
** \param nOccNo 
** \return std::shared_ptr<CAinAlarm> 
** \author LiJin 
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
std::shared_ptr<CAinAlarm> CMemDB::GetAinAlarm(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	auto it = m_mapAinAlarms.find(nOccNo);
	Q_ASSERT(it != m_mapAinAlarms.end());
	if (it != m_mapAinAlarms.end())
	{		
		return it->second;
	}
	return nullptr;
}

bool CMemDB::GetAinAlarmEx(int32u nOccNo, AIN_ALARM &ainAlarm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
		return false;

	if (nOccNo > m_nAinAlarmCount)
	{
		return false;
	}
	std::memcpy(&ainAlarm, &m_pAinAlarm[nOccNo - 1], sizeof(AIN_ALARM));

	return true;
}

bool CMemDB::GetDinAlarmEx(int32u nOccNo, DIN_ALARM &dinAlarm)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
		return false;

	if (nOccNo > m_nDinAlarmCount)
	{
		return false;
	}
	std::memcpy(&dinAlarm, &m_pDinAlarm[nOccNo - 1], sizeof(DIN_ALARM));

	return true;
}

bool CMemDB::GetAinAlarmLimit(int32u nOccNo, AIN_ALARM_LIMIT &ainAlarmLimit)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
		return false;

	if (nOccNo > m_pAinAlarmLimitCount)
	{
		return false;
	}
	std::memcpy(&ainAlarmLimit, &m_pAinAlarmLimit[nOccNo - 1], sizeof(AIN_ALARM_LIMIT));

	return true;
}

bool CMemDB::GetDinAlarmLimit(int32u nOccNo, DIN_ALARM_LIMIT &dinAlarmLimit)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_ALARM_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_ALARM_OCCNO)
		return false;

	if (nOccNo > m_nDinAlarmCount)
	{
		return false;
	}
	std::memcpy(&dinAlarmLimit, &m_pDinAlarmLimit[nOccNo - 1], sizeof(DIN_ALARM_LIMIT));

	return true;
}
/*! \fn std::shared_ptr<CDinAlarm>CMemDB::GetDinAlarm(int32u nOccNo)
********************************************************************************************************* 
** \brief CMemDB::GetDinAlarm 
** \details 根据occno获取开关量报警
** \param nOccNo 
** \return std::shared_ptr<CDinAlarm> 
** \author LiJin 
** \date 2016年9月22日 
** \note 
********************************************************************************************************/
std::shared_ptr<CDinAlarm>CMemDB::GetDinAlarm(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	auto it = m_mapDinAlarms.find(nOccNo);
	Q_ASSERT(it != m_mapDinAlarms.end());
	if (it != m_mapDinAlarms.end())
	{
		return it->second;
	}
	return nullptr;
}



TRANSFORM_LINEAR * CMemDB::GetTransformLinear(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	if (nOccNo > m_nLinearCount)
	{
		return nullptr;
	}

	return &m_pLinear[nOccNo - 1];
}

TRANSFORM_NONLINEAR * CMemDB::GetTransformNonLinear(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return nullptr;

	if (nOccNo > m_nNonLinearCount)
	{
		return nullptr;
	}

	return &m_pNonLinear[nOccNo - 1];
}
/*! \fn bool  CMemDB::GetDoutBlockState(DOUT & dout)
********************************************************************************************************* 
** \brief CMemDB::GetDoutBlockState 
** \details 检查开出闭锁状态
** \param dout 
** \return bool 
** \author LiJin 
** \date 2017年2月17日 
** \note 
********************************************************************************************************/
void  CMemDB::CheckDoutBlockState(DOUT & dout)
{
	if (dout.BlockingProgramOccNo != INVALID_OCCNO)
	{// 执行下开出闭锁程序，发信号给内核，由内核执行

	}
	else
	{
		if (dout.BlockingSignalOccNo != INVALID_OCCNO)
		{// 只有闭锁信号，没有闭锁程序
			int8u nValue = 0;
			int8u nQuality = 0;
			bool bRet = IoGetDinValue(dout.BlockingSignalOccNo, nValue, nQuality);
			Q_ASSERT(bRet);
			if (dout.BlockSignalType == NORMAL_CLOSE)
			{// 常闭信号，值为1
				if (nValue == 1)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
			else
			{
				if (nValue == 0)
				{
					dout.CheckOK = 1;
				}
				else
				{
					dout.CheckOK = 0;
				}
			}
		}
	}
	return  ;
}
/*! \fn bool CMemDB::AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource)
********************************************************************************************************* 
** \brief CMemDB::AppSetDoutValue 
** \details 应用层设置遥控
** \param nOccNo 
** \param Value 
** \param nSource 
** \return bool 
** \author LiJin 
** \date 2016年12月2日 
** \note 
********************************************************************************************************/
bool CMemDB::AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nDoutCount)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	DOUT *pFB = &m_pDouts[nOccNo - 1];

	if (pFB->IsSBO)
	{// SBO


//		return false;
	}

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		CheckDoutBlockState(*pFB);		 
		return false;
	}
	
	// 检查条件，如果控制闭锁没有通过，则告警
	if (pFB->CheckOK == 0)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Because the remotecontrol's bloking condition is't ready.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		// 告警-提示运行人员： 遥控闭锁条件检查失败。

		return false;
	}
	//! 如果是启动程序，则 转发消息到 顺控模块（支持异机）
	if (pFB->StartProgramOccNo != INVALID_OCCNO)
	{

	}
	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_DOUT;
	pSetValeEvt->Att = ATTW_DOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_BOOLEAN;

	S_BOOL( &pSetValeEvt->Value[0], &Value);
 
	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;
 
	dmsg.Size = sizeof(SETVAL_MSG);	
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//! 判断是否关联遥信，如果有关联遥信，则发送到db_svc，由db_svc去做处理，此处需要注意超时处理
	if (pFB->FeedbackOccNo != INVALID_OCCNO && pFB->FeedbackOccNo <= MAX_OCCNO)
	{
		Q_ASSERT(m_nDbSvcMailBoxID != 0);
		dmsg.RecverID = m_nDbSvcMailBoxID;
	}
	else
	{// 否则直接下发给通道驱动
		if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
		else
		{
			dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
			if (dmsg.RecverID == 0)
			{
				szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
				LogMsg(szLog.toStdString().c_str(), 1);

				return false;
			}
		}
	}
	bool bRet = SendMail("FES", &dmsg,0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}
/*! \fn bool CMemDB::AppSetAoutValue(int32u nOccNo, fp64 Value, int8u nSource)
********************************************************************************************************* 
** \brief CMemDB::AppSetAoutValue 
** \details 设模出调节
** \param nOccNo 
** \param Value 
** \param nSource 
** \return bool 
** \author LiJin 
** \date 2017年5月22日 
** \note 
********************************************************************************************************/
bool CMemDB::AppSetAoutValue(int32u nOccNo, fp64 Value, int8u nSource)
{
	//! 判断 OccNo是否有效
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	QString szLog;

	if (nOccNo > m_nAoutCount)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	// 如果本机状态是非主机状态，则不执行
	if (GetMyHostState() != STATE_MAIN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is't master state.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	// 如果本节点退出运行，则不执行
	if (GetMyHostScanEnable() != SCAN_IN)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed.My node state is scan out.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	AOUT *pFB = &m_pAouts[nOccNo - 1];

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
	
		return false;
	}
	//! 获取最大、最小范围，如果超过范围，则报警提示，并且不设值下去
	if (Value < pFB->RangeL || Value > pFB->RangeH)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. The value is wrong.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}

	//! 组织设值报文，并发送给 db_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_AOUT;
	pSetValeEvt->Att = ATTW_AOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Source1 = nSource;
	pSetValeEvt->Datatype = DT_DOUBLE;

	S_DOUBLE(&pSetValeEvt->Value[0], &Value);

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = 0;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	if (pFB->ChannelOccNo == INVALID_OCCNO || pFB->ChannelOccNo > MAX_CHANNEL_OCCNO)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel OccNo is incorrect.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);

		return false;
	}
	else
	{
		dmsg.RecverID = GetChannelMailBoxID(pFB->ChannelOccNo);
		if (dmsg.RecverID == 0)
		{
			szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
			LogMsg(szLog.toStdString().c_str(), 1);

			return false;
		}
	}

	bool bRet = SendMail("FES", &dmsg, 0);
	//! 如果报文发送成功，则启动告警记录
	if (bRet)
	{// 调用告警接口


	}
	else
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
	}

	return bRet;
}
/*! \fn bool  CMemDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
********************************************************************************************************* 
** \brief CMemDB::GetRTData 
** \details 获取 基础对象的 属性值 （VALUE\QUA等等）
** \param nIddType 
** \param nOccNo 
** \param nFiledID 
** \param RetData 
** \return bool 
** \author LiJin 
** \date 2016年10月20日 
** \note 
********************************************************************************************************/
bool  CMemDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
	
	bool bRet = false;
 
	switch (nIddType)
	{
	case IDD_NODE:
	{
		Q_ASSERT(m_arrGetNodeRTDataFuncs[nFiledID]);
		if (m_arrGetNodeRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetNodeRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_CHANNEL:
	{
		Q_ASSERT(m_arrGetChannelRTDataFuncs[nFiledID]);
		if (m_arrGetChannelRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetChannelRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_DEVICE:
	{
		Q_ASSERT(m_arrGetDeviceRTDataFuncs[nFiledID]);
		if (m_arrGetDeviceRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetDeviceRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_AIN:
	{
		Q_ASSERT(m_arrGetAinRTDataFuncs[nFiledID]);
		if (m_arrGetAinRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetAinRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_DIN:
	{
		Q_ASSERT(m_arrGetDinRTDataFuncs[nFiledID]);
		if (m_arrGetDinRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetDinRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_AOUT:
	{
		Q_ASSERT(m_arrGetAoutRTDataFuncs[nFiledID]);
		if (m_arrGetAoutRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetAoutRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	case IDD_DOUT:
	{
		Q_ASSERT(m_arrGetDoutRTDataFuncs[nFiledID]);
		if (m_arrGetDoutRTDataFuncs[nFiledID])
		{
			bRet = m_arrGetDoutRTDataFuncs[nFiledID](nOccNo, RetData);
		}
		break;
	}
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}	 

	return bRet;
}

bool  CMemDB::PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, const IO_VARIANT &RetData, void *pExt)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_NODE:
		break;
	default:
		break;
	}

	return true;
}
/*! \fn bool CMemDB::PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData,
	const char * szOperatorName, const char * szMonitorName, void *pExt, bool bOpLog)
********************************************************************************************************* 
** \brief CMemDB::PutRTData 
** \details 
** \param nIddType 
** \param nOccNo 
** \param nFiledID 
** \param nParam 
** \param pData 
** \param szOperatorName 
** \param szMonitorName 
** \param pExt 
** \param bOpLog 
** \return bool 
** \author LiJin 
** \date 2017年2月21日 
** \note 
********************************************************************************************************/
bool CMemDB::PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData,
	const char * szOperatorName, const char * szMonitorName, void *pExt, bool bOpLog)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_NODE:
		bRet = SetNodeAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_CHANNEL:
		bRet = SetChannelAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_DEVICE:
		bRet = SetDeviceAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_AIN:
		bRet = SetAinAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_DIN:
		bRet = SetDinAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_AOUT:
		bRet = SetAoutAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_DOUT:
		bRet = SetDoutAttrValue(nOccNo, nFiledID, *pData);
		break;
	case IDD_USERVAR:
		break;
	case IDD_SYSVAR:
		break;

	default:
		break;
	}

	return bRet;
}
/*! \fn bool CMemDB::SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
********************************************************************************************************* 
** \brief CMemDB::SetNodeAttrValue 
** \details 设置节点的相关属性值
** \param nOccNo 
** \param nFiledId 
** \param varVal 
** \return bool 
** \author LiJin 
** \date 2017年2月21日 
** \note 
********************************************************************************************************/
bool CMemDB::SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetNodeAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetNodeAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	if (nFiledId == ATT_REDSWITCH)
	{// 主从切换
		if (m_pNodes[nOccNo-1].IsDefined == FES_YES && m_pNodes[nOccNo-1].ScanEnable == SCAN_IN)
		{// 切换--调用切换函数

		}
	}
	else
	{
		unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pNodes[nOccNo - 1]);
		memcpy(&pMem[m_pTagAttrMgr->GetNodeAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));
		return true;
	}

	return false;
}

bool CMemDB::SetChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetChannelAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetChannelAttrs()[nFiledId].DataType == varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pChannels[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetChannelAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));
	return true;
}

bool CMemDB::SetDeviceAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_DEVICE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].DataType == varVal.Type)
		return false;
	
	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDevices[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDeviceAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CMemDB::SetAinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetAinAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetAinAttrs()[nFiledId].DataType != varVal.Type)
		return false;
	
	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pAins[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetAinAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CMemDB::SetDinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDinAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDinAttrs()[nFiledId].DataType != varVal.Type)
		return false;
	
	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDins[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDinAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CMemDB::SetAoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetAoutAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetAoutAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pAouts[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetAoutAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}

bool CMemDB::SetDoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nFiledId > ATT_MAX)
		return false;

	Q_ASSERT(m_pTagAttrMgr);
	if (m_pTagAttrMgr == nullptr)
		return false;

	if (m_pTagAttrMgr->GetDoutAttrs()[nFiledId].Att == CTagAttMgr::ATT_NO || m_pTagAttrMgr->GetDoutAttrs()[nFiledId].DataType != varVal.Type)
		return false;

	unsigned char *pMem = reinterpret_cast<unsigned char*> (&m_pDouts[nOccNo - 1]);
	memcpy(&pMem[m_pTagAttrMgr->GetDoutAttrs()[nFiledId].ByteOffset], &varVal.Value, CIoDataType::GetDataSize(varVal.Type));

	return true;
}
/*! \fn void CMemDB::InitFuncArrary()
********************************************************************************************************* 
** \brief CMemDB::InitFuncArrary 
** \details 初始化 通用属性读取函数表
** \return void 
** \author LiJin 
** \date 2016年10月20日 
** \note 
********************************************************************************************************/
void CMemDB::InitFuncArrary()
{ 
	//NODE
	m_arrGetNodeRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetNodeScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetNodeRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetNodeQuality, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetNodeRTDataFuncs[ATT_REDSWITCH] = std::bind(&CMemDB::GetNodeRedState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetNodeRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetNodeRedState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetNodeRTDataFuncs[ATT_NETA] = std::bind(&CMemDB::GetNodeNetA, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetNodeRTDataFuncs[ATT_NETB] = std::bind(&CMemDB::GetNodeNetB, this, std::placeholders::_1, std::placeholders::_2);
	//CHANNEL
	m_arrGetChannelRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetChannelScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetChannelRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetChannelQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetChannelRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetChannelState, this, std::placeholders::_1, std::placeholders::_2);
	//DEVICE
	m_arrGetDeviceRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetDeviceScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDeviceRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetDeviceQua, this, std::placeholders::_1, std::placeholders::_2);
	//AIN
	m_arrGetAinRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetAinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetAinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetAinState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_MANSET] = std::bind(&CMemDB::GetAinManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CMemDB::GetAinLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CMemDB::GetAinHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_HIQUA] = std::bind(&CMemDB::GetAinHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_LOQUA] = std::bind(&CMemDB::GetAinLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CMemDB::GetAinDesc, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_PINLABEL] = std::bind(&CMemDB::GetAinPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_UNIT] = std::bind(&CMemDB::GetAinUint, this, std::placeholders::_1, std::placeholders::_2);
	//DIN
	m_arrGetDinRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetDinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetDinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetDinState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_MANSET] = std::bind(&CMemDB::GetDinManSet, this, std::placeholders::_1, std::placeholders::_2);
	//din 的output 暂时用最大输出代替
	m_arrGetDinRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CMemDB::GetDinOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_START] = std::bind(&CMemDB::GetDinStart, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE0] = std::bind(&CMemDB::GetDinDesc0, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE1] = std::bind(&CMemDB::GetDinDesc1, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_PINLABEL] = std::bind(&CMemDB::GetDinPin, this, std::placeholders::_1, std::placeholders::_2);

	//AOUT
	m_arrGetAoutRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetAoutScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetAoutQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetAoutState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_AOUT] = std::bind(&CMemDB::GetAoutOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_PINLABEL] = std::bind(&CMemDB::GetAoutPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_UNIT] = std::bind(&CMemDB::GetAoutUint, this, std::placeholders::_1, std::placeholders::_2);

	//DOUT
	m_arrGetDoutRTDataFuncs[ATT_IN_OUT] = std::bind(&CMemDB::GetDinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_QUA] = std::bind(&CMemDB::GetDinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_STATE] = std::bind(&CMemDB::GetDinState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_DOUT] = std::bind(&CMemDB::GetDoutOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_PINLABEL] = std::bind(&CMemDB::GetDoutPin, this, std::placeholders::_1, std::placeholders::_2);
}
/*! \fn bool CMemDB::GetNodeScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
********************************************************************************************************* 
** \brief CMemDB::GetNodeScanEnable 
** \details 获取节点的投退状态
** \param nOccNo 
** \param RetData 
** \return bool 
** \author LiJin 
** \date 2016年10月20日 
** \note 
********************************************************************************************************/
bool CMemDB::GetNodeScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
	
	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];
	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;	
}
/*! \fn bool CMemDB::GetNodeQuality(int32u nOccNo, IO_VARIANT &RetData) const
********************************************************************************************************* 
** \brief CMemDB::GetNodeQuality 
** \details 获取节点的品质
** \param nOccNo 
** \param RetData 
** \return bool 
** \author LiJin 
** \date 2016年10月20日 
** \note 
********************************************************************************************************/
bool CMemDB::GetNodeQuality(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];
	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetNodeRedState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];
	S_BOOL(&RetData, &pFB->RedState);

	return true;
}

bool CMemDB::GetNodeState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetNodeNetA(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];

	S_WORD(&RetData, &pFB->Net1State);

	return true;
}

bool CMemDB::GetNodeNetB(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nNodeCount);
	if (nOccNo > m_nNodeCount)
	{
		return false;
	}
	NODE *pFB = &m_pNodes[nOccNo - 1];

	S_WORD(&RetData, &pFB->Net2State);

	return true;
}

bool CMemDB::GetChannelScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nChannelCount);
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}
	CHANNEL *pFB = &m_pChannels[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetChannelQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nChannelCount);
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}
	CHANNEL *pFB = &m_pChannels[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetChannelState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nChannelCount);
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}
	CHANNEL *pFB = &m_pChannels[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetDeviceScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDeviceCount);
	if (nOccNo > m_nDeviceCount)
	{
		return false;
	}
	DEVICE *pFB = &m_pDevices[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetDeviceQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDeviceCount);
	if (nOccNo > m_nDeviceCount)
	{
		return false;
	}
	DEVICE *pFB = &m_pDevices[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetAinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetAinQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetAinState(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_WORD(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetAinManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ManSet);

	return true;
}

bool CMemDB::GetAinLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->RangeL);

	return true;
}

bool CMemDB::GetAinHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->RangeH);

	return true;
}

bool CMemDB::GetAinHighQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->HighQua);

	return true;
}

bool CMemDB::GetAinLowQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->LowQua);

	return true;
}

bool CMemDB::GetAinDesc(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_INT(&RetData, &pFB->DescriptionOccNo);

	return true;
}

bool CMemDB::GetAinPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_INT(&RetData, &pFB->PinLabelOccNo);

	return true;
}

bool CMemDB::GetAinUint(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_INT(&RetData, &pFB->UnitNameOccNo);

	return true;
}

bool CMemDB::GetDinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetDinQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);
	return true;
}

bool CMemDB::GetDinState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetDinManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ManSet);

	return true;
}

bool CMemDB::GetDinOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Value);

	return true;
}

bool CMemDB::GetDinStart(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_BOOL(&RetData, &pFB->StartCtrl);

	return true;
}

bool CMemDB::GetDinDesc0(int32u nOccNo, IO_VARIANT &RetData) const
{

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_INT(&RetData, &pFB->Desc0_OccNo);

	return true;
}

bool CMemDB::GetDinDesc1(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_INT(&RetData, &pFB->Desc1_OccNo);

	return true;
}

bool CMemDB::GetDinPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_INT(&RetData, &pFB->PinLabelOccNo);


	return true;
}

bool CMemDB::GetAoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetAoutQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetAoutState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetAoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->Output);

	return true;
}

bool CMemDB::GetAoutPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_INT(&RetData, &pFB->PinLabelOccNo);

	return true;
}

bool CMemDB::GetAoutUint(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAoutCount);
	if (nOccNo > m_nAoutCount)
	{
		return false;
	}
	AOUT *pFB = &m_pAouts[nOccNo - 1];

	S_INT(&RetData, &pFB->UnitNameOccNo);

	return true;
}

bool CMemDB::GetDoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	DOUT *pFB = &m_pDouts[nOccNo - 1];

	S_BOOL(&RetData, &pFB->ScanEnable);

	return true;
}

bool CMemDB::GetDoutQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	DOUT *pFB = &m_pDouts[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Quality);

	return true;
}

bool CMemDB::GetDoutState(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	DOUT *pFB = &m_pDouts[nOccNo - 1];

	S_INT(&RetData, &pFB->State);

	return true;
}

bool CMemDB::GetDoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	DOUT *pFB = &m_pDouts[nOccNo - 1];

	S_BOOL(&RetData, &pFB->Output);

	return true;
}

bool CMemDB::GetDoutPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDoutCount);
	if (nOccNo > m_nDoutCount)
	{
		return false;
	}
	DOUT *pFB = &m_pDouts[nOccNo - 1];

	S_INT(&RetData, &pFB->PinLabelOccNo);

	return true;
}

/*! \fn bool CMemDB::IoSetAinQua(int32u nOccNo, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetAinQua 
** \details 修改测点品质
** \param nOccNo 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月27日 
** \note 
********************************************************************************************************/
bool CMemDB::IoSetAinQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapAins.find(nOccNo);
	Q_ASSERT(it_find != m_mapAins.end());
	if (it_find != m_mapAins.end())
	{
		it_find->second->SetQuality(nQuality);
	}
	return true;
}

bool CMemDB::IoSetDinQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapDins.find(nOccNo);
	Q_ASSERT(it_find != m_mapDins.end());
	if (it_find != m_mapDins.end())
	{
		it_find->second->SetQuality(nQuality);
	}
	return true;
}

bool CMemDB::IoSetAoutQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapAouts.find(nOccNo);
	Q_ASSERT(it_find != m_mapAouts.end());
	if (it_find != m_mapAouts.end())
	{
		it_find->second->SetQuality(nQuality);
	}
	return true;
}

bool CMemDB::IoSetDoutQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapDouts.find(nOccNo);
	Q_ASSERT(it_find != m_mapDouts.end());
	if (it_find != m_mapDouts.end())
	{
		it_find->second->SetQuality(nQuality);
	}
	return true;
}
/*! \fn bool  CMemDB::IoSetDeviceQua(int32u nDeviceOccNo, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetDeviceQua 
** \details 修改装置的品质位
** \param nDeviceOccNo 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月27日 
** \note  如果品质为差的话，则该装置所属的所有测点品质均为差，如果为好，则不修改测点的品质
********************************************************************************************************/
bool  CMemDB::IoSetDeviceQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_DEVICE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_DEVICE_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapDevices.find(nOccNo);
	Q_ASSERT(it_find != m_mapDevices.end());
	if (it_find != m_mapDevices.end())
	{
		return it_find->second->SetQuality(nQuality);
	}

	return false;
}
/*! \fn bool CMemDB::IoSetChannelQua(int32u nOccNo, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetChannelQua 
** \details 修改通道的品质位
** \param nOccNo 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年10月27日 
** \note 
********************************************************************************************************/
bool CMemDB::IoSetChannelQua(int32u nOccNo, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}

	auto it_find = m_mapChannels.find(nOccNo);
	Q_ASSERT(it_find != m_mapChannels.end());
	if (it_find != m_mapChannels.end())
	{
		return it_find->second->SetQuality(nQuality);
	}

	return false;
}
/*! \fn bool CMemDB::IsDBAlive(int nTimeout)
*********************************************************************************************************
** \brief CMemDB::IsDBAlive
** \details 数据库是否正在工作
** \param nTimeout
** \return bool false 数据库退出，true 数据库正常工作
** \author LiJin
** \date 2016年11月1日
** \note
********************************************************************************************************/
bool CMemDB::IsDBAlive(int nTimeout)
{
//	Q_ASSERT(m_pDBAliveFlag);
	if (m_pDBAliveFlag == nullptr)
		return false;

	ACE_Time_Value tm_val(0, nTimeout * 1000);

	int nRet = m_pDBAliveFlag->wait(&tm_val, 0);
	// 注意 返回值
	return (nRet == -1) ? false : true;
}

void CMemDB::IoSetDeviceHeartBeat(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return  ;
	}

	auto it_find = m_mapDevices.find(nOccNo);
	Q_ASSERT(it_find != m_mapDevices.end());
	if (it_find != m_mapDevices.end())
	{
		return it_find->second->SetHeartBeat();
	}
}

void CMemDB::IoSetChannelHeartBeat(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return;
	}

	auto it_find = m_mapChannels.find(nOccNo);
	Q_ASSERT(it_find != m_mapChannels.end());
	if (it_find != m_mapChannels.end())
	{
		return it_find->second->SetHeartBeat( );
	}
}
/*! \fn void CMemDB::IoDiagAlarm(int32u nChannleOccNo, int32u nDeviceOccNo, const char* pszAlarmTxt)
********************************************************************************************************* 
** \brief CMemDB::IoDiagAlarm 
** \details 自诊断告警
** \param nChannleOccNo 
** \param nDeviceOccNo 
** \param pszAlarmTxt 
** \return void 
** \author LiJin 
** \date 2016年12月2日 
** \note 
********************************************************************************************************/
void CMemDB::IoDiagAlarm(int32u nChannleOccNo, int32u nDeviceOccNo, const char* pszAlarmTxt, TIMEPAK * pTm)
{
	Q_ASSERT(pszAlarmTxt && strlen(pszAlarmTxt));
	if (pszAlarmTxt == nullptr || strlen(pszAlarmTxt) == 0)
		return;

	Q_ASSERT(nChannleOccNo != INVALID_OCCNO && nChannleOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannleOccNo == INVALID_OCCNO || nChannleOccNo > MAX_CHANNEL_OCCNO)
		return;
	
	Q_ASSERT(nChannleOccNo <= m_nChannelCount);
	if (nChannleOccNo > m_nChannelCount)
		return;

	Q_ASSERT(nDeviceOccNo != INVALID_OCCNO && nDeviceOccNo <= MAX_DEVICE_OCCNO);
	if (nDeviceOccNo == INVALID_OCCNO || nDeviceOccNo > MAX_DEVICE_OCCNO)
		return;	 

	if (m_nNBSvcMailBoxID == INVALID_MAILBOX_ID)
	{
		m_nNBSvcMailBoxID = QueryMailBoxID("FES", "NB_SVC");
	}
	Q_ASSERT(m_nNBSvcMailBoxID != INVALID_MAILBOX_ID);

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_DIAG;
	Q_ASSERT(m_pChannels[nChannleOccNo - 1].MailBoxID != INVALID_MAILBOX_ID);

	dmsg.SenderID = m_pChannels[nChannleOccNo-1].MailBoxID;

	dmsg.RecverID = m_nNBSvcMailBoxID;
	dmsg.Size = sizeof(DIAG_MSG);

	DIAG_MSG *pDiag = reinterpret_cast<DIAG_MSG*>(dmsg.Buff);

	pDiag->NodeOccNo = GetMyNodeOccNo();
	pDiag->ChannelOccNo = nChannleOccNo;
	pDiag->DeviceOccNo = nDeviceOccNo;

	if (pTm == nullptr)
	{
		// 时标
		QDateTime dtNow = QDateTime::currentDateTime();
		pDiag->Tm.Year = dtNow.date().year();
		pDiag->Tm.Month = dtNow.date().month();
		pDiag->Tm.Day = dtNow.date().day();
		pDiag->Tm.DayOfWeek = dtNow.date().dayOfWeek();

		pDiag->Tm.Hour = dtNow.time().hour();
		pDiag->Tm.Minute = dtNow.time().minute();
		pDiag->Tm.Second = dtNow.time().second();
		pDiag->Tm.Milliseconds = dtNow.time().msec();
	}
	else
	{
		std::memcpy(&pDiag->Tm, pTm, sizeof(TIMEPAK));
	}
	
	size_t nLen = std::min<size_t>(strlen(pszAlarmTxt), _countof(pDiag->ExtraData));
	strncpy(reinterpret_cast<char*> (pDiag->ExtraData), pszAlarmTxt, nLen);
	SendMail("FES", &dmsg, 50);
}

void  CMemDB::IoAlarmMsg(int32u nChannleOccNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm)
{
	Q_ASSERT(pszAlarmTxt && strlen(pszAlarmTxt));
	if (pszAlarmTxt == nullptr || strlen(pszAlarmTxt) == 0)
		return;

	Q_ASSERT(nChannleOccNo != INVALID_OCCNO && nChannleOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannleOccNo == INVALID_OCCNO || nChannleOccNo > MAX_CHANNEL_OCCNO)
		return;

	Q_ASSERT(nChannleOccNo <= m_nChannelCount);
	if (nChannleOccNo > m_nChannelCount)
		return; 

	if (m_nNBSvcMailBoxID == INVALID_MAILBOX_ID)
	{
		m_nNBSvcMailBoxID = QueryMailBoxID("FES", "NB_SVC");
	}
	Q_ASSERT(m_nNBSvcMailBoxID != INVALID_MAILBOX_ID);

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = nAlarmType;
	Q_ASSERT(m_pChannels[nChannleOccNo - 1].MailBoxID != INVALID_MAILBOX_ID);

	dmsg.SenderID = m_pChannels[nChannleOccNo - 1].MailBoxID;

	dmsg.RecverID = m_nNBSvcMailBoxID;
	dmsg.Size = sizeof(ALARM_MSG);

	ALARM_MSG *pMsg = reinterpret_cast<ALARM_MSG*>(dmsg.Buff);

	pMsg->NodeOccNo = GetMyNodeOccNo();
// 	pMsg->ChannelOccNo = nChannleOccNo;
// 	pMsg->DeviceOccNo = nDeviceOccNo;

	if (pTm == nullptr)
	{
		// 时标
		QDateTime dtNow = QDateTime::currentDateTime();
		pMsg->Tm.Year = dtNow.date().year();
		pMsg->Tm.Month = dtNow.date().month();
		pMsg->Tm.Day = dtNow.date().day();
		pMsg->Tm.DayOfWeek = dtNow.date().dayOfWeek();

		pMsg->Tm.Hour = dtNow.time().hour();
		pMsg->Tm.Minute = dtNow.time().minute();
		pMsg->Tm.Second = dtNow.time().second();
		pMsg->Tm.Milliseconds = dtNow.time().msec();
	}
	else
	{
		std::memcpy(&pMsg->Tm, pTm, sizeof(TIMEPAK));
	}

	size_t nLen = std::min<size_t>(strlen(pszAlarmTxt), _countof(pMsg->ExtraData));
	strncpy(reinterpret_cast<char*> (pMsg->ExtraData), pszAlarmTxt, nLen);
	SendMail("FES", &dmsg, 50);
}
/*! \fn int32s CMemDB::GetChannelMailBoxID(int32u nChannelOccNo)
********************************************************************************************************* 
** \brief CMemDB::GetChannelMailBoxID 
** \details 通过通道排行号，获取该通道的邮箱ID
** \param nChannelOccNo 
** \return int 
** \author LiJin 
** \date 2016年12月12日 
** \note 
********************************************************************************************************/
int32s CMemDB::GetChannelMailBoxID(int32u nChannelOccNo)
{
	Q_ASSERT(nChannelOccNo != INVALID_OCCNO && nChannelOccNo <= MAX_CHANNEL_OCCNO);
	if (nChannelOccNo == INVALID_OCCNO || nChannelOccNo > MAX_CHANNEL_OCCNO)
		return 0;

	if (nChannelOccNo > m_nChannelCount)
	{
		return 0;
	}

	Q_ASSERT(m_pChannels[nChannelOccNo - 1].MailBoxID);
	return m_pChannels[nChannelOccNo - 1].MailBoxID;
}
/*! \fn bool  CMemDB::ReadHostCmd(int32u nChannleNo, FES_CMD *pCmd, int32u nTimeout)
********************************************************************************************************* 
** \brief CMemDB::ReadHostCmd 
** \details 读取引擎下发的遥控、遥调命令，其他的命令忽略
** \param nChannleNo 
** \param pCmd 
** \param nTimeout 
** \return bool 
** \author LiJin 
** \date 2017年2月15日 
** \note 
********************************************************************************************************/
bool  CMemDB::ReadHostCmd(int32u nChannleNo, SETVAL_MSG *pCmd, int32u nTimeout)
{
	Q_ASSERT(nChannleNo != INVALID_OCCNO && nChannleNo <= MAX_CHANNEL_OCCNO);
	if (nChannleNo == INVALID_OCCNO || nChannleNo > MAX_CHANNEL_OCCNO)
		return false;

	Q_ASSERT(pCmd);
	if (pCmd == nullptr)
		return false;

	int32s nMailBoxID = GetChannelMailBoxID(nChannleNo);
	Q_ASSERT(nMailBoxID);
	if (nMailBoxID == 0)
		return false;

	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));
	dmsg.RecverID = nMailBoxID;

	QString szLog;

	bool bRet = RecvMail("FES", &dmsg, nTimeout);
	if (bRet)
	{
		if (dmsg.Type != MSG_EVT_SETVAL)
		{
			szLog = QObject::tr("Discard the msg.[SenderID=%1,ReciverID=%2,Type=%3,Size=%4].").arg(dmsg.SenderID).arg(dmsg.RecverID).arg(dmsg.Type).arg(dmsg.Size);
			LogMsg(szLog.toStdString().c_str(), 1);
			return false;
		}
		std::memcpy(pCmd, dmsg.Buff,std::min<size_t>(sizeof(SETVAL_MSG),MAIL_MAX_SIZE));

		szLog = QObject::tr("receive the setvalue msg.[SenderID=%1,ReciverID=%2,Type=%3,Size=%4].").arg(dmsg.SenderID).arg(dmsg.RecverID).arg(dmsg.Type).arg(dmsg.Size);
		LogMsg(szLog.toStdString().c_str(), 1);

		return true;
	}
	return false;
}
/*! \fn bool CMemDB::AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua)
********************************************************************************************************* 
** \brief CMemDB::AppGetAinValue 
** \details 应用层获取模拟量
** \param nOccNo 
** \param pValue 
** \param pQua 如果为空，则不返回
** \return bool 
** \author LiJin 
** \date 2017年4月13日 
** \note 
********************************************************************************************************/
bool CMemDB::AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
 
	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	AIN *pFB = &m_pAins[nOccNo - 1];

	*pValue = pFB->Value;

	if (pQua)
	{
		*pQua = pFB->Quality;
	}

	return true;
}
/*! \fn bool CMemDB::AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua)
********************************************************************************************************* 
** \brief CMemDB::AppGetDinValue 
** \details 应用层获取开关量
** \param nOccNo 
** \param pValue 
** \param pQua 
** \return bool 
** \author LiJin 
** \date 2017年4月13日 
** \note 
********************************************************************************************************/
bool CMemDB::AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}

	DIN *pFB = &m_pDins[nOccNo - 1];

	*pValue = pFB->Value;

	if (pQua)
	{
		*pQua = pFB->Quality;
	}

	return true;
}
/*! \fn bool CMemDB::AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
********************************************************************************************************* 
** \brief CMemDB::AppGetUserVarValue 
** \details 获取用户变量的测值
** \param nOccNo 
** \param pVariant 
** \param pQua 
** \return bool 
** \author LiJin 
** \date 2017年5月17日 
** \note 
********************************************************************************************************/
bool CMemDB::AppGetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA *pFB = & m_pUserVariable[nOccNo - 1];

 	*pVariant = pFB->Value;

	if (pQua)
	{
		*pQua = pFB->Quality;
	}

	return true;
}
/*! \fn bool CMemDB::AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
********************************************************************************************************* 
** \brief CMemDB::AppGetSysVarValue 
** \details 获取系统变量的测值
** \param nOccNo 
** \param pVariant 
** \param pQua 
** \return bool 
** \author LiJin 
** \date 2017年5月17日 
** \note 
********************************************************************************************************/
bool CMemDB::AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nSystemVariableCount);
	if (nOccNo > m_nSystemVariableCount)
	{
		return false;
	}

	VARDATA *pFB = &m_pSystemVariable[nOccNo - 1];

	*pVariant = pFB->Value;

	if (pQua)
	{
		*pQua = pFB->Quality;
	}
	 
	return true;
}
/*! \fn bool CMemDB::AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u  nQua)
********************************************************************************************************* 
** \brief CMemDB::AppSetUserVarValue 
** \details 应用层修改用户变量 值
** \param nOccNo 
** \param pVariant 
** \param nQua 
** \return bool 
** \author LiJin 
** \date 2017年5月17日 
** \note 应用层只是简单修改测值？？ 如果关联的是遥控、遥调，则需要直接设值下去
********************************************************************************************************/
bool CMemDB::AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u  nQua)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}

	VARDATA *pFB = &m_pUserVariable[nOccNo - 1];
	if (pFB->ScanEnable != SCAN_OUT)
	{
		pFB->Value = *pVariant;
		if (pFB->IsRefTag == FES_YES)
		{// 不是引用变量，那么会有一些触发
			if (pFB->SrcIddType == IDD_DOUT)
			{
				AppSetDoutValue(pFB->SrcOccNo, V_BOOL(*pVariant),0);
			}
			else if(pFB->SrcIddType == IDD_AOUT)
			{
				AppSetAoutValue(pFB->SrcOccNo, V_DOUBLE(*pVariant), 0);
			}
			else
			{
				if (pFB->DataType == DT_BOOLEAN)
				{// 开关量

				}
				else if (pFB->DataType == DT_FLOAT || pFB->DataType == DT_DOUBLE)
				{// 模拟量

				}
			}
		}
		return true;
	}
	return false;
} 
/*! \fn bool CMemDB::IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
********************************************************************************************************* 
** \brief CMemDB::IoSetUserVarValue 
** \details 驱动层修改用户变量的测值
** \param nOccNo 
** \param pVariant 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2017年5月18日 
** \note  今后最好是由IO驱动更新测值的同时，直接更新关联的用户变量
********************************************************************************************************/
bool CMemDB::IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nUserVariableCount);
	if (nOccNo > m_nUserVariableCount)
	{
		return false;
	}
	VARDATA *pFB = &m_pUserVariable[nOccNo - 1];
	if (pFB->ScanEnable != SCAN_OUT)
	{
		pFB->Value = *pVariant;
		if (pFB->IsRefTag == FES_YES)
		{// 不是引用变量，那么会有一些触发
			if (pFB->DataType == DT_BOOLEAN)
			{// 开关量

			}
			else 
			{// 模拟量 - 该报警就报警

			}


		}
		return true;
	}
	return false;
}
// 驱动层修改系统变量的测值
bool CMemDB::IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality)
{
	return true;
}

/** @}*/
