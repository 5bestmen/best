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
#include "mail/mail.h"
#include "stl_util-inl.h"

#include <QDebug>
#include <QObject> 
#include <QString>  

CFesDB::CFesDB( )
{
	m_bStopFlag = false;
	m_nEstimateSize = 0; 
	InitFuncArrary();
}

CFesDB::~CFesDB(void)
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

bool CFesDB::GetChannelByOccNo(int32u nOccNo, CHANNEL** pChannel)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_CHANNEL_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_CHANNEL_OCCNO)
	{
		return false;
	}
	if (nOccNo > m_nChannelCount)
	{
		return false;
	}
	*pChannel = &m_pChannels[nOccNo - 1];

	return true;
}

bool CFesDB::GetDeviceByOccNo(int32u nOccNo, DEVICE** pDev)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo < MAX_DEVICE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_DEVICE_OCCNO)
	{
		return false;
	}
	if (nOccNo >m_nDeviceCount)
	{
		return false;
	}
	*pDev = &m_pDevices[nOccNo-1];

	return true;
}

bool  CFesDB::GetAinByOccNo(int32u nOccNo,AIN** pAin)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	*pAin = &m_pAins[nOccNo-1];

	return true;
}

bool CFesDB::GetDinByOccNo(int32u nOccNo, DIN** pDin)
{

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDinCount)
	{
		return false;
	}

	*pDin = &m_pDins[nOccNo - 1];
	return true;
}

bool CFesDB::GetAoutByOccNo(int32u nOccNo, AOUT **pAout)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nAoutCount)
	{
		return false;
	}

	*pAout = &m_pAouts[nOccNo - 1];

	return true;
}

bool CFesDB::GetDoutByOccNo(int32u nOccNo, DOUT** pDout)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	if (nOccNo > m_nDoutCount)
	{
		return false;
	}

	*pDout = &m_pDouts[nOccNo - 1];

	return true;
}

bool CFesDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;

	switch (nIddType)
	{
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

/*! \fn bool CFesDB::PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
********************************************************************************************************* 
** \brief CFesDB::PutRtData 
** \details  关于写的属性（遥控 遥调 设置 设置相关状态等……）
** \param nIddType 
** \param nOccNo 
** \param nFiledID 
** \param pData 
** \param pExt 
** \param pSrc 
** \return bool 
** \author xingzhibing
** \date 2017年5月17日 
** \note 
********************************************************************************************************/

bool CFesDB::PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;

	switch (nIddType)
	{
	case IDD_CHANNEL:
	{
		break;
	}
	case IDD_DEVICE:
	{
		break;
	}
	case IDD_AIN:
	{
		break;
	}
	case IDD_DIN:
	{
		break;
	}
	case IDD_AOUT:
	{
		Q_ASSERT(m_arrAoutSetFunctions[nFiledID]);
		if (m_arrAoutSetFunctions[nFiledID])
		{
			m_arrAoutSetFunctions[nFiledID](nOccNo, pData, pExt, pSrc);
		}
		break;
	}
	case IDD_DOUT:
	{
		Q_ASSERT(m_arrDoutSetFunctions[nFiledID]);
		if (m_arrDoutSetFunctions[nFiledID])
		{
			m_arrDoutSetFunctions[nFiledID](nOccNo,pData,pExt,pSrc);
		}
		break;
	}
	case IDD_USERVAR:
		break;
	case IDD_SYSVAR:
		break;
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}

	return bRet;
}

/*! \fn bool CFesDB::LoadMem(unsigned char* pAddr)
********************************************************************************************************* 
** \brief CFesDB::LoadMem 
** \details 
** \param pAddr 
** \return bool 
** \author xingzhibing
** \date 2017年2月12日 
** \note 
********************************************************************************************************/
bool CFesDB::LoadMem(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return false;
	}
	m_pMagicMem = (HEAD_MEM*)(pAddr);

	Q_ASSERT(m_pMagicMem->MagicHead1 == MAGIC_HEAD && m_pMagicMem->MagicHead2 == MAGIC_HEAD);
	if (m_pMagicMem->MagicHead1 != MAGIC_HEAD || m_pMagicMem->MagicHead2 != MAGIC_HEAD)
	{
		return false;
	}

	m_nChannelCount = m_pMagicMem->ChannelCount;
	m_nDeviceCount = m_pMagicMem->DeviceCount;
	m_nAinCount = m_pMagicMem->AinCount;
	m_nDinCount = m_pMagicMem->DinCount;
	m_nAoutCount = m_pMagicMem->AoutCount;
	m_nDoutCount = m_pMagicMem->DoutCount;
	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;
	m_nSystemVariableCount = m_pMagicMem->SystemVariableCount;
	m_nUserVariableCount = m_pMagicMem->UserVariableCount;

	pAddr += sizeof HEAD_MEM;

	size_t nSize = 0;
	nSize = CreateChannel((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDevice((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAin((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDin((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAout((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDout((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateAinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinAlarm((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateDinLimitAlarm((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateSystemVariable((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateUserVariable((unsigned char*)pAddr);
	pAddr += nSize;

	return true;

}

size_t CFesDB::CreateChannel(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pChannels = reinterpret_cast<CHANNEL*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nChannelCount; ++i)
	{
		Q_ASSERT(m_pChannels[i].OccNo != INVALID_OCCNO && m_pChannels[i].OccNo < MAX_CHANNEL_OCCNO);
		if (m_pChannels[i].OccNo == INVALID_OCCNO || m_pChannels[i].OccNo > MAX_CHANNEL_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] channel's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pChannels[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrChannels.push_back(&m_pChannels[i]);
	}

	return m_nChannelCount * sizeof CHANNEL;
}

size_t CFesDB::CreateDevice(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDevices = reinterpret_cast<DEVICE*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDeviceCount; ++i)
	{
		Q_ASSERT(m_pDevices[i].OccNo != INVALID_OCCNO && m_pDevices[i].OccNo < MAX_DEVICE_OCCNO);
		if (m_pDevices[i].OccNo == INVALID_OCCNO || m_pDevices[i].OccNo > MAX_DEVICE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] device's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDevices[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDevices.push_back(&m_pDevices[i]);
	}
	return m_nDeviceCount * sizeof DEVICE;
}

size_t CFesDB::CreateAin(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAins = reinterpret_cast<AIN*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nAinCount; ++i)
	{
		Q_ASSERT(m_pAins[i].OccNo != INVALID_OCCNO && m_pAins[i].OccNo < MAX_OCCNO);
		if (m_pAins[i].OccNo == INVALID_OCCNO || m_pAins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] ain's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrAins.push_back(&m_pAins[i]);
	}
	return m_nAinCount * sizeof AIN;
}

size_t CFesDB::CreateDin(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDins = reinterpret_cast<DIN*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDinCount; ++i)
	{
		Q_ASSERT(m_pDins[i].OccNo != INVALID_OCCNO && m_pDins[i].OccNo < MAX_OCCNO);
		if (m_pDins[i].OccNo == INVALID_OCCNO || m_pDins[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDins[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDins.push_back(&m_pDins[i]);
	}
	return m_nDinCount * sizeof DIN;
}

size_t CFesDB::CreateAout(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAouts = reinterpret_cast<AOUT*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nAoutCount; ++i)
	{
		Q_ASSERT(m_pAouts[i].OccNo != INVALID_OCCNO && m_pAouts[i].OccNo < MAX_OCCNO);
		if (m_pAouts[i].OccNo == INVALID_OCCNO || m_pAouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] aout's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pAouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrAouts.push_back(&m_pAouts[i]);
	}
	return m_nAoutCount * sizeof AOUT;
}

size_t CFesDB::CreateDout(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDouts = reinterpret_cast<DOUT*>(pAddr);

	QString szLog;

	for (int i = 0; i < m_nDoutCount; ++i)
	{
		Q_ASSERT(m_pDouts[i].OccNo != INVALID_OCCNO && m_pDouts[i].OccNo < MAX_OCCNO);
		if (m_pDouts[i].OccNo == INVALID_OCCNO || m_pDouts[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] aout's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pDouts[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrDouts.push_back(&m_pDouts[i]);
	}
	return m_nDoutCount * sizeof DOUT;
}

size_t CFesDB::CreateAinAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAinAlarm = reinterpret_cast<AIN_ALARM*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nAinAlarmCount; i++)
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
	}
	return m_nAinAlarmCount * sizeof AIN_ALARM;
}

size_t CFesDB::CreateAinLimitAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pAinAlarmLimit = reinterpret_cast<AIN_ALARM_LIMIT*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nAinAlarmLimitCount; i++)
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
	}
	return m_nAinAlarmLimitCount * sizeof AIN_ALARM_LIMIT;
}

size_t CFesDB::CreateDinAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDinAlarm = reinterpret_cast<DIN_ALARM*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nDinAlarmCount; i++)
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
	}
	return m_nDinAlarmCount * sizeof DIN_ALARM;
}

size_t CFesDB::CreateDinLimitAlarm(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pDinAlarmLimit = reinterpret_cast<DIN_ALARM_LIMIT*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nDinAlarmLimitCount; i++)
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
	}
	return m_nDinAlarmLimitCount* sizeof DIN_ALARM_LIMIT;
}

size_t CFesDB::CreateSystemVariable(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pSystemVariable = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nSystemVariableCount; i++)
	{
		Q_ASSERT(m_pSystemVariable[i].OccNo != INVALID_OCCNO && m_pSystemVariable[i].OccNo <= MAX_OCCNO);
		if (m_pSystemVariable[i].OccNo == INVALID_OCCNO || m_pSystemVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] systemvariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pSystemVariable[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrSystemVariables.push_back(&m_pSystemVariable[i]);
			continue;
		}
		m_arrSystemVariables.push_back(&m_pSystemVariable[i]);
	}
	return m_nSystemVariableCount* sizeof VARDATA;
}

size_t CFesDB::CreateUserVariable(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pUserVariable = reinterpret_cast<VARDATA*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nUserVariableCount; i++)
	{
		Q_ASSERT(m_pUserVariable[i].OccNo != INVALID_OCCNO && m_pUserVariable[i].OccNo <= MAX_OCCNO);
		if (m_pUserVariable[i].OccNo == INVALID_OCCNO || m_pUserVariable[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] uservariable's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pSystemVariable[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrUserVariables.push_back(&m_pUserVariable[i]);
			continue;
		}
		m_arrUserVariables.push_back(&m_pUserVariable[i]);
	}
	return m_nUserVariableCount* sizeof VARDATA;
}

void CFesDB::InitFuncArrary()
{
	//CHANNEL
	m_arrGetChannelRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetChannelScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetChannelRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetChannelQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetChannelRTDataFuncs[ATT_STATE] = std::bind(&CFesDB::GetChannelState, this, std::placeholders::_1, std::placeholders::_2);
	//DEVICE
	m_arrGetDeviceRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetDeviceScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDeviceRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetDeviceQua, this, std::placeholders::_1, std::placeholders::_2);
	//AIN
	m_arrGetAinRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetAinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetAinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_STATE] = std::bind(&CFesDB::GetAinState, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetAinRTDataFuncs[ATT_VALUE] = std::bind(&CFesDB::GetAinValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_SIGNAL_VALUE] = std::bind(&CFesDB::GetAinSignalValEx, this, std::placeholders::_1, std::placeholders::_2);
	
	m_arrGetAinRTDataFuncs[ATT_MANSET] = std::bind(&CFesDB::GetAinManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CFesDB::GetAinLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CFesDB::GetAinHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_HIQUA] = std::bind(&CFesDB::GetAinHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_LOQUA] = std::bind(&CFesDB::GetAinLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CFesDB::GetAinDesc, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_PINLABEL] = std::bind(&CFesDB::GetAinPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAinRTDataFuncs[ATT_UNIT] = std::bind(&CFesDB::GetAinUint, this, std::placeholders::_1, std::placeholders::_2);
	//DIN
	m_arrGetDinRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetDinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetDinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE] = std::bind(&CFesDB::GetDinState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_MANSET] = std::bind(&CFesDB::GetDinManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CFesDB::GetDinOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_START] = std::bind(&CFesDB::GetDinStart, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE0] = std::bind(&CFesDB::GetDinDesc0, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_STATE1] = std::bind(&CFesDB::GetDinDesc1, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_PINLABEL] = std::bind(&CFesDB::GetDinPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDinRTDataFuncs[ATT_VALUE] = std::bind(&CFesDB::GetDinValEx, this, std::placeholders::_1, std::placeholders::_2);

	//AOUT
	m_arrGetAoutRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetAoutScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetAoutQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_STATE] = std::bind(&CFesDB::GetAoutState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_AOUT] = std::bind(&CFesDB::GetAoutOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_PINLABEL] = std::bind(&CFesDB::GetAoutPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetAoutRTDataFuncs[ATT_UNIT] = std::bind(&CFesDB::GetAoutUint, this, std::placeholders::_1, std::placeholders::_2);

	//DOUT
	m_arrGetDoutRTDataFuncs[ATT_IN_OUT] = std::bind(&CFesDB::GetDinScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_QUA] = std::bind(&CFesDB::GetDinQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_STATE] = std::bind(&CFesDB::GetDinState, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_DOUT] = std::bind(&CFesDB::GetDoutOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetDoutRTDataFuncs[ATT_PINLABEL] = std::bind(&CFesDB::GetDoutPin, this, std::placeholders::_1, std::placeholders::_2);

	m_arrDoutSetFunctions[ATT_DOUT] = std::bind(&CFesDB::SetDoutValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	m_arrAoutSetFunctions[ATT_AOUT] = std::bind(&CFesDB::SetAoutValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

/*! \fn bool CFesDB::GetChannelScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
********************************************************************************************************* 
** \brief CFesDB::GetChannelScanEnable 
** \details 
** \param nOccNo 
** \param RetData 
** \return bool 
** \author xingzhibing
** \date 2017年2月12日 
** \note 
********************************************************************************************************/
bool CFesDB::GetChannelScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetChannelQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetChannelState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDeviceScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDeviceQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->Value);

	return true;
}

bool CFesDB::GetAinSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nAinCount);
	if (nOccNo > m_nAinCount)
	{
		return false;
	}
	AIN *pFB = &m_pAins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->SignalValue);

	return true;
}

bool CFesDB::GetAinManSet(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinHighQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinLowQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinDesc(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinPin(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAinUint(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinManSet(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinStart(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinDesc0(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinDesc1(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinPin(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDinValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->Value);

	return true;
}

bool CFesDB::GetDinNegValueEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->NegValue);

	return true;
}

bool CFesDB::GetDinRawValueEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	Q_ASSERT(nOccNo <= m_nDinCount);
	if (nOccNo > m_nDinCount)
	{
		return false;
	}
	DIN *pFB = &m_pDins[nOccNo - 1];

	S_DOUBLE(&RetData, &pFB->RawValue);

	return true;
}

bool CFesDB::GetAoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAoutQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAoutState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAoutPin(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetAoutUint(int32u nOccNo, IO_VARIANT &RetData) const
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

/*
bool CFesDB::SetAoutValue(int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
	if (nOccNo>m_nAoutCount)
	{
		return false;
	}
	return true;
}*/

bool CFesDB::SetAoutValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc)
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

	AOUT *pFB = &m_pAouts[nOccNo - 1];

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;

		return false;
	}

	fp64 Value = CVariant(*pData).operator double();

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
	pSetValeEvt->Source1 = *((int8u*)(pSrc)); ;
	pSetValeEvt->Datatype = DT_DOUBLE;

	S_DOUBLE(&pSetValeEvt->Value[0], &Value);

	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = pFB->NodeOccNo;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//直接发送给总线，然后通过总线转发到相应的前置
	int nMailID = QueryMailBoxID("SCADA", "NB_SVC");
	if (nMailID == INVALID_OCCNO)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	dmsg.RecverID = nMailID;

	bool bRet = SendMail("SCADA", &dmsg, 0);
	if (!bRet)
	{
		szLog = QObject::tr("Set aout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	return true;
}

bool CFesDB::SetDoutValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;
	if (nOccNo>m_nDoutCount)
	{
		return false;
	}
	QString szLog;
	DOUT * pFB;
	bool bRet = GetDoutByOccNo(nOccNo, &pFB);
	if (!bRet)
	{
		return false;
	}
	if (pFB->IsSBO)
	{// SBO

	}

	if (pFB->Init != INITED)
	{
		pFB->Init = INITED;
		//CheckDoutBlockState(*pFB);
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

	//! 组织设值报文，并发送给 nb_svc
	std::shared_ptr<SETVAL_MSG>  pSetValeEvt = std::make_shared<SETVAL_MSG>();
	std::memset(pSetValeEvt.get(), 0, sizeof(SETVAL_MSG));

	pSetValeEvt->MsgCode = MSG_SETVAL;
	pSetValeEvt->Len = sizeof(SETVAL_MSG);
	pSetValeEvt->IddType = IDD_DOUT;
	pSetValeEvt->Att = ATTW_DOUT;
	pSetValeEvt->NodeOccNo = pFB->NodeOccNo;
	pSetValeEvt->Occno = nOccNo;
	pSetValeEvt->Source1=*((int8u*)(pSrc));   //Todo :please notice!!!! 
	pSetValeEvt->Datatype = DT_BOOLEAN;

	S_BOOL(&pSetValeEvt->Value[0], &pData);

	//! 再次封装成邮件发送MSG_EVT_SETVAL
	DMSG dmsg;
	std::memset(&dmsg, 0, sizeof(DMSG));

	dmsg.Type = MSG_EVT_SETVAL;
	dmsg.SenderID = pFB->NodeOccNo;

	dmsg.Size = sizeof(SETVAL_MSG);
	memcpy(dmsg.Buff, pSetValeEvt.get(), std::min<size_t>(static_cast <size_t> (dmsg.Size), static_cast <size_t>(MAIL_MAX_SIZE)));

	//直接发送给总线，然后通过总线转发到相应的前置
	int nMailID = QueryMailBoxID("SCADA","NB_SVC");
	if (nMailID==INVALID_OCCNO)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Channel's MailBoxID is incorrect.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}
	dmsg.RecverID = nMailID;

	bRet = SendMail("SCADA", &dmsg, 0);
	if (!bRet)
	{
		szLog = QObject::tr("Set dout [OccNo=%1] failed. Send setvalmsg failed.").arg(nOccNo);
		LogMsg(szLog.toStdString().c_str(), 1);
		return false;
	}

	return true;
}

bool CFesDB::GetDoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDoutQua(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDoutState(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const
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

bool CFesDB::GetDoutPin(int32u nOccNo, IO_VARIANT &RetData) const
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


/** @}*/
