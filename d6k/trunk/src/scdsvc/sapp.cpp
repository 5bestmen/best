/*! @file sapp.cpp
<PRE>
********************************************************************************
ģ����       :  SCADA Ӧ��ģ�����
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author LiJin
*  @version 1.0
*  @date    2017.06.01
*/
#include "sapp.h"
//#include "fesapi/fesdatadef.h"
#include "scadaapi/scdapp_def.h"

#include <QDebug>
#include <QLibrary> 

CScadaApp::CScadaApp()
{
	m_nOccNo = 0;
	m_pData = nullptr;
	m_fnStartApp = nullptr;
	m_fnStopApp = nullptr;
}

CScadaApp::~CScadaApp()
{
	
}

CScadaApp::CScadaApp(SAPP *pApp) :m_pData(pApp)
{
	Q_ASSERT(pApp);
	if (m_pData)
	{
		m_nOccNo = m_pData->OccNo;
	}

	m_szProgrammeName = m_pData->ProgramName;

	m_fnStartApp = nullptr;
	m_fnStopApp = nullptr;
}

bool CScadaApp::Load()
{
	QString szExt;
#ifdef Q_OS_MAC
	// mac
#endif

#ifdef Q_OS_LINUX
	// linux
	szExt = ".so";
#endif

#ifdef Q_OS_WIN32
	// win
	szExt = ".dll";
#endif

	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	if (m_pData->nLoadType == Load_Dll)
	{// ������
		Q_ASSERT(strlen(m_pData->ProgramName));
		if (strlen(m_pData->ProgramName) <= 0)
		{
			return false;
		}
		QString szDllName;
		QString szLog;
		szDllName = m_pData->ProgramName + szExt;
		QLibrary dlllib(szDllName);
		if (dlllib.load())
		{
			m_fnStartApp = (StartApp)dlllib.resolve("StartIoDriver");
			m_fnStopApp = (StopApp)dlllib.resolve("StopIoDriver");
			Q_ASSERT(m_fnStartApp);
			Q_ASSERT(m_fnStopApp);

			if (m_fnStartApp && m_fnStopApp)
			{
				szLog = "Load [ " + szDllName + " ] success.";
				LogString(szLog.toStdString().c_str(), 0);
			}
			else
			{
				szLog = "Load [ " + szDllName + " ] failed, the callback function is bad.";
				LogString(szLog.toStdString().c_str(), 1);
			}
		}
		else
		{// log ��ӡ
			szLog = "Load [ " + szDllName + " ] failed";
			LogString(szLog.toStdString().c_str(), 1);
		}
	}
	else
	{// ������
		// modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}

	return true;
}

bool CScadaApp::Start(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog; 
	
	if (m_pData->nLoadType == Load_Dll)
	{// ������ 
		Q_ASSERT(m_fnStartApp);

		if (m_fnStartApp)
		{
			//m_fnStopIoDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
		}
		else
		{
			if (strlen(m_pData->ProgramName))
			{
				szLog = "Start [ " + QString(m_pData->ProgramName) + " ] failed.";
			}
			else
			{
				szLog = "Start [ " + QString("Programme:%1").arg(m_pData->ProgramName) + " ] failed.";
			}

			LogString(szLog.toStdString().c_str(), 1);
		}
	}
	else if (m_pData->nLoadType == Load_Exe)
	{// ������
	 // modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}


	return true;
}

bool CScadaApp::Stop(const char * pszName)
{
	Q_ASSERT(m_pData);
	if (m_pData == nullptr)
		return false;

	QString szLog;

	if (m_pData->nLoadType == Load_Dll)
	{// ������ 
		Q_ASSERT(m_fnStopApp);

		if (m_fnStopApp)
		{
			//m_fnStopIoDriver(pszName, m_pData->NodeOccNo, m_pData->OccNo);
		}
		else
		{
			if (strlen(m_pData->ProgramName))
			{
				szLog = "Stop [ " + QString(m_pData->ProgramName) + " ] failed.";
			}
			else
			{
				szLog = "Stop [ " + QString("Programme:%1").arg(m_pData->ProgramName) + " ] failed.";
			}

			LogString(szLog.toStdString().c_str(), 1);
		}

	}
	else if (m_pData->nLoadType == Load_Exe)
	{// ������
		// modbus_tcp.exe --szDataBaseName --NodeOccNo --OccNo

	}

	return true;
}

bool  CScadaApp::LogString(const char *szLogTxt, int nLevel)
{

	return true;
}

std::string CScadaApp::GetProgrammeName() const
{
	return m_szProgrammeName;
}

void CScadaApp::SetMailBoxID(int nID)
{
	m_nMailBoxID = nID;
	if (m_pData)
	{
		m_pData->MailBoxID = nID;
	}
}

bool CScadaApp::InnerProgarmmeEnable()
{
	if (m_pData->nAppType==Inn_APP)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** @}*/
