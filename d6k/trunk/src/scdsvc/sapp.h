/*! @file
<PRE>
********************************************************************************
ģ����       :  SCADA Ӧ��ģ�����
�ļ���       :  sapp.h
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

#ifndef SCADA_APP_H
#define SCADA_APP_H
#pragma once

#include "datatypes.h"

#include <memory>
#include <string>

struct SAPP;

typedef int(*StartApp)(const char * pszName, int32u nAppOccNo, int nExtraArgc, char *pszExtraArgv[]);
typedef int(*StopApp)(const char * pszName, int32u nAppOccNo);
 
class CScadaApp
{
public:
	CScadaApp();
	explicit CScadaApp(SAPP *pApp);
	~CScadaApp();
public:
	bool Load();
	bool Start(const char * pszName);
	bool Stop(const char * pszName);
// 	virtual bool LoadData(int32u nOccNo) = 0;

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}
	std::string GetProgrammeName()const;

	void SetMailBoxID(int nID);

	int GetMailBoxID() const
	{
		return m_nMailBoxID;
	}
	bool InnerProgarmmeEnable();
protected:
	bool LogString(const char *szLogTxt, int nLevel);

	StartApp  m_fnStartApp;
	StopApp   m_fnStopApp;

protected:
	int32u m_nOccNo;                //! ��������к�
	std::string m_szProgrammeName;  //! ��������
	SAPP * m_pData;
	int m_nMailBoxID;
};
 
 


/** @}*/

#endif // SCADA_APP_H
