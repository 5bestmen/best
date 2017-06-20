/*! @file server_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  server_db.h
文件实现功能 :  服务器节点的内存数据库
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
#ifndef SERVER_DB_MODULE_H
#define SERVER_DB_MODULE_H

#include "scada_db.h"
#include "variant.h"
#include "fesapi/fes_magicmem.h"
#include <array>
#include <vector>
#include <memory>
#include <functional>
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
	bool  GetUserVarByOccNo(int32u nOccNo, VARDATA **pUserVar);
	bool  GetSysVarByOccNo(int32u nOccNo,  VARDATA** pSysVar);

	bool  GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData);
	bool  PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc);

private:
	void  InitFuncArrary();
	//获取
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetUserVarRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetSysVarRTDataFuncs;
	//设置属性
	std::array< std::function<bool(int32u, IO_VARIANT*, void*, void*)   >, ATT_MAX> m_arrUserVarSetFunctions;
	std::array< std::function<bool(int32u, IO_VARIANT*, void*, void*)   >, ATT_MAX> m_arrSysVarSetFunctions;

private:
	bool GetUserVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVarQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaManSet(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaDesc(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaPin(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetUserVaUint(int32u nOccNo, IO_VARIANT &RetData) const;

	bool GetSysVarScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVarQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaManSet(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaHighQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaLowQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaDesc(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaPin(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetSysVaUint(int32u nOccNo, IO_VARIANT &RetData) const;
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

