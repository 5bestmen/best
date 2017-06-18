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
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef SERVER_DB_MODULE_H
#define SERVER_DB_MODULE_H

#include "scada_db.h"
#include "fesapi/fes_magicmem.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <QXmlStreamReader>

typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;
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
	size_t EstimateMemSize()
	{
		return m_nEstimateSize;
	}
	size_t EstimateTagNameMemSize()
	{
		return m_nTagNameEstimateSize;
	}
	int32u GetOccNo()
	{
		return m_nOccNo;
	}
	size_t BuildDB(char * pAddr);	
	size_t BuildTagnamedDB(char* pAddr);
protected:
	//创建告警内存
	size_t BuildAlarmDB(char* pAddress);
	//模拟量告警以及限值
	size_t BuildAinAlarmDB(char* pAddress);
	//开关量告警
	size_t BuildDinAlarmDB(char* pAddress);
	//线性变化
	size_t BuildLinearDB(char* pAddress);
	//非线性变换
	size_t BuildNonLinearDB(char* pAddress);
	//系统变量
	size_t BuildSystemVariableDB(char* pAddress);
	//用户变量
	size_t BuildUserVariableDB(char* pAddress);
	//tagname
	size_t BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec);
private:
	size_t m_nEstimateSize;
	size_t m_nTagNameEstimateSize;
protected:
	bool ReadScdVarFile(const char* pszFilePath);
private:
	//转换信息
	void ReadScale();
	void ReadLinearInfo();
	void ReadNonLinearInfo();
	void ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear);
	//告警信息
	void ReadAlarm();
	void ReadAinAlarmInfo();
	void ReadDinAlarmInfo();
	//越限告警
	void ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm);
	void ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm);
	void SkipUnknownElement();
	//变量
	void ReadVariable();
	void ReadSystemVariable();
	void ReadUserVariable();
private:
	HEAD_MEM* m_pMagicMem;
	HEAD_MEM* m_pMagicTagNameMem;
	int32u m_nOccNo;

	QXmlStreamReader m_XmlReader;
private:
	std::vector<TRANSFORM_LINEAR_DEF>     m_arrTempTransFormLinears;
	std::vector<TRANSFORM_NONLINEAR_DEF>  m_arrTempTransFormNonLinears;

	std::vector<AIN_ALARM_DEF>            m_arrTempAinAlarms;
	std::vector<DIN_ALARM_DEF>            m_arrTempDinAlarms;
	std::vector<AIN_ALARM_LIMIT_DEF>      m_arrTempAinLimitAlarms;
	std::vector<DIN_ALARM_LIMIT_DEF>      m_arrTempDinLimitAlarms;
	std::vector<VARDATA_DEF> m_arrTempSystemVariables;
	std::vector<VARDATA_DEF> m_arrTempUserVariables;

	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmLimitOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmLimitOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrTransLinearOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrTransNonLinearOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrSyetemVarOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrUserVarOccNos;

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

