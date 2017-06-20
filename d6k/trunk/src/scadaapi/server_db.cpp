/*! @file server_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  server_db.cpp
文件实现功能 :  服务器节点的内存数据库，主要是系统变量以及用户变量的管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   服务器节点的内存数据库
*  @author  xingzhibing
*  @version 1.0
*  @date    2017.06.07
*******************************************************************************/
#include "server_db.h"
#include "log/log.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include <QObject> 
#include <QString>  


template<typename T>
bool SortFuction(const T& T1, const T& T2)
{
	return T1->OccNo < T2->OccNo;
}

typedef TRANSFORM_NONLINEAR::LINEAR_ITEM DataItem;

bool SortLFunction(DataItem& data1, DataItem& data2)
{
	return data1.RawValue < data2.RawValue;
}



CServerDB::CServerDB( )
{
	m_bStopFlag = false;
	m_nEstimateSize = 0;
}

CServerDB::~CServerDB(void)
{
	
}

bool CServerDB::Initialize(const char *pszDataPath, unsigned int nMode,int32u nOccNo)
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

	return true;
}

void CServerDB::Run()
{
 
}

void CServerDB::Shutdown()
{

}

size_t CServerDB::LoadMem(unsigned char * pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return false;
	}
	m_pMagicMem = (HEAD_MEM*)(pAddr);

	Q_ASSERT(m_pMagicMem->MagicHead1 == MAGIC_HEAD 
		  && m_pMagicMem->MagicHead2 == MAGIC_HEAD);
	if (m_pMagicMem->MagicHead1 != MAGIC_HEAD || m_pMagicMem->MagicHead2 != MAGIC_HEAD)
	{
		return false;
	}
	m_nAinAlarmCount = m_pMagicMem->AinAlarmCount;
	m_nAinAlarmLimitCount = m_pMagicMem->AinAlarmLimitCount;
	m_nDinAlarmCount = m_pMagicMem->DinAlarmCount;
	m_nDinAlarmLimitCount = m_pMagicMem->DinAlarmLimitCount;
	m_nLinearCount = m_pMagicMem->LinearCount;
	m_nNonLinearCount = m_pMagicMem->NonLinearCount;
	m_nSystemVariableCount = m_pMagicMem->SystemVariableCount;
	m_nUserVariableCount = m_pMagicMem->UserVariableCount;

	pAddr += sizeof HEAD_MEM;

	size_t nSize = 0;

	nSize = CreateAinAlarm((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateAinLimitAlarm((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateDinAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateDinLimitAlarm((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateTransFormLinear((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateTransFormNonLinear((unsigned char* )pAddr);
	pAddr += nSize;

	nSize = CreateSystemVariable((unsigned char*)pAddr);
	pAddr += nSize;

	nSize = CreateUserVariable((unsigned char*)pAddr);
	pAddr += nSize;

	m_nEstimateSize = sizeof HEAD_MEM+ sizeof AIN_ALARM * m_nAinAlarmCount +
		sizeof AIN_ALARM_LIMIT* m_nAinAlarmLimitCount +
		sizeof DIN_ALARM * m_nDinAlarmCount +
		sizeof DIN_ALARM_LIMIT* m_nDinAlarmLimitCount +
		sizeof TRANSFORM_LINEAR * m_nLinearCount +
		sizeof TRANSFORM_NONLINEAR* m_nNonLinearCount +
		sizeof VARDATA * m_nSystemVariableCount +
		sizeof VARDATA * m_nUserVariableCount;

	return m_nEstimateSize;
}

bool CServerDB::GetUserVarByOccNo(int32u nOccNo, VARDATA **pAout)
{
	return true;
}

bool CServerDB::GetSysVarByOccNo(int32u nOccNo, VARDATA** pDout)
{
	return true;
}

bool CServerDB::GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_SYSVAR:
		break;
	case IDD_USERVAR:
		break;
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}
	return true;
}

bool CServerDB::PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
		return false;

	bool bRet = false;
	switch (nIddType)
	{
	case IDD_SYSVAR:
		break;
	case IDD_USERVAR:
		break;
	default:
		Q_ASSERT(false);
		bRet = false;
		break;
	}
	return true;
}

void CServerDB::InitFuncArrary()
{
	m_arrGetUserVarRTDataFuncs[ATT_IN_OUT] = std::bind(&CServerDB::GetUserVarScanEnable, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_QUA] = std::bind(&CServerDB::GetUserVarQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_STATE] = std::bind(&CServerDB::GetUserVaState, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_VALUE] = std::bind(&CServerDB::GetUserVaValEx, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_SIGNAL_VALUE] = std::bind(&CServerDB::GetUserVaSignalValEx, this, std::placeholders::_1, std::placeholders::_2);

	m_arrGetUserVarRTDataFuncs[ATT_MANSET] = std::bind(&CServerDB::GetUserVaManSet, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MINOUTPUT] = std::bind(&CServerDB::GetUserVaLowOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_MAXOUTPUT] = std::bind(&CServerDB::GetUserVaHighOutPut, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_HIQUA] = std::bind(&CServerDB::GetUserVaHighQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_LOQUA] = std::bind(&CServerDB::GetUserVaLowQua, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_DESCRIPTION] = std::bind(&CServerDB::GetUserVaDesc, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_PINLABEL] = std::bind(&CServerDB::GetUserVaPin, this, std::placeholders::_1, std::placeholders::_2);
	m_arrGetUserVarRTDataFuncs[ATT_UNIT] = std::bind(&CServerDB::GetUserVaUint, this, std::placeholders::_1, std::placeholders::_2);

}

bool CServerDB::GetUserVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVarQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaState(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaDesc(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetUserVaUint(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVarQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaState(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaManSet(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaDesc(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaPin(int32u nOccNo, IO_VARIANT &RetData) const
{
	return true;
}

bool CServerDB::GetSysVaUint(int32u nOccNo, IO_VARIANT &RetData) const
{
	return  true;
}

size_t CServerDB::CreateAinAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateAinLimitAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateDinAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateDinLimitAlarm(unsigned char* pAddr)
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

size_t CServerDB::CreateTransFormLinear(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pLinear = reinterpret_cast<TRANSFORM_LINEAR*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nLinearCount; i++)
	{
		Q_ASSERT(m_pLinear[i].OccNo != INVALID_OCCNO && m_pLinear[i].OccNo <= MAX_OCCNO);
		if (m_pLinear[i].OccNo == INVALID_OCCNO || m_pLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrLinears.push_back(&m_pLinear[i]);
			continue;
		}
		m_arrLinears.push_back(&m_pLinear[i]);
	}
	return m_nLinearCount* sizeof TRANSFORM_LINEAR;
}

size_t CServerDB::CreateTransFormNonLinear(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pNonLinear = reinterpret_cast<TRANSFORM_NONLINEAR*>(pAddr);

	QString szLog;

	for (int32u i = 0; i < m_nNonLinearCount; i++)
	{
		Q_ASSERT(m_pNonLinear[i].OccNo != INVALID_OCCNO && m_pNonLinear[i].OccNo <= MAX_OCCNO);
		if (m_pNonLinear[i].OccNo == INVALID_OCCNO || m_pNonLinear[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] din_alarm_limit's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNonLinear[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrNonLinears.push_back(&m_pNonLinear[i]);
			continue;
		}
		m_arrNonLinears.push_back(&m_pNonLinear[i]);
	}
	return m_nNonLinearCount* sizeof TRANSFORM_NONLINEAR;
}

size_t CServerDB::CreateSystemVariable(unsigned char* pAddr)
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

size_t CServerDB::CreateUserVariable(unsigned char* pAddr)
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

/** @}*/
