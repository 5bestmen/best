/*! @file server_db.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  server_db.h
�ļ�ʵ�ֹ��� :  �������ڵ���ڴ����ݿ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �������ڵ���ڴ����ݿ�
*  @author  xingzhibing
*  @version 1.0
*  @date    2017.06.07
*******************************************************************************/
#ifndef SERVER_DB_MODULE_H
#define SERVER_DB_MODULE_H

#include "scada_db.h"
#include "fesapi/fes_magicmem.h"
#include <vector>
#include <memory>
#include <unordered_map>

typedef std::shared_ptr<VARDATA> VARDATA_DEF;

 
class CServerDB : public CScadaDB
{
public:
	CServerDB( );
	virtual ~CServerDB(void);

public:
	bool Initialize(const char *pszDataPath, unsigned int nMode, int32u nOccNo);
	virtual void Run();
	virtual void Shutdown();
public:
	size_t LoadMem(unsigned char * pAddr);	
	size_t GetEstimateSize()
	{
		return m_nEstimateSize;
	}
private:
	size_t m_nEstimateSize;
protected:
	size_t CreateAinAlarm(unsigned char* pAddr);
	size_t CreateAinLimitAlarm(unsigned char* pAddr);
	size_t CreateDinAlarm(unsigned char* pAddr);
	size_t CreateDinLimitAlarm(unsigned char* pAddr);
	size_t CreateTransFormLinear(unsigned char* pAddr);
	size_t CreateTransFormNonLinear(unsigned char* pAddr);
	size_t CreateSystemVariable(unsigned char* pAddr);
	size_t CreateUserVariable(unsigned char* pAddr);
private:
	HEAD_MEM* m_pMagicMem;
	//QString m_szProjName;
	int32u m_nOccNo;
private:
	std::vector<TRANSFORM_LINEAR_DEF>     m_arrTempTransFormLinears;
	std::vector<TRANSFORM_NONLINEAR_DEF>  m_arrTempTransFormNonLinears;
	std::vector<AIN_ALARM_DEF>            m_arrTempAinAlarms;
	std::vector<DIN_ALARM_DEF>            m_arrTempDinAlarms;
	std::vector<AIN_ALARM_LIMIT_DEF>      m_arrTempAinLimitAlarms;
	std::vector<DIN_ALARM_LIMIT_DEF>      m_arrTempDinLimitAlarms;
	std::vector<VARDATA_DEF> m_arrTempSystemVariables;
	std::vector<VARDATA_DEF> m_arrTempUserVariables;
private:
	TRANSFORM_LINEAR *m_pLinear;
	unsigned int m_nLinearCount;
	std::vector<TRANSFORM_LINEAR*> m_arrLinears;

	TRANSFORM_NONLINEAR *m_pNonLinear;
	unsigned int m_nNonLinearCount;
	std::vector<TRANSFORM_NONLINEAR*> m_arrNonLinears;

	AIN_ALARM  *m_pAinAlarm;
	unsigned int m_nAinAlarmCount;
	std::vector<AIN_ALARM*> m_arrAinAlarms;

	AIN_ALARM_LIMIT *m_pAinAlarmLimit;
	unsigned int m_nAinAlarmLimitCount;
	std::vector<AIN_ALARM_LIMIT*> m_arrAinAlarmLimits;

	DIN_ALARM *m_pDinAlarm;
	unsigned int m_nDinAlarmCount;
	std::vector<DIN_ALARM*> m_arrDinAlarms;

	DIN_ALARM_LIMIT *m_pDinAlarmLimit;
	unsigned int m_nDinAlarmLimitCount;
	std::vector<DIN_ALARM_LIMIT*> m_arrDinAlarmLimits;

	VARDATA * m_pSystemVariable;
	unsigned int m_nSystemVariableCount;
	std::vector<VARDATA* >m_arrSystemVariables;

	VARDATA*  m_pUserVariable;
	unsigned int m_nUserVariableCount;
	std::vector<VARDATA* >m_arrUserVariables;

};

#endif // SERVER_DB_MODULE_H


/** @}*/

