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
	//�����澯�ڴ�
	size_t BuildAlarmDB(char* pAddress);
	//ģ�����澯�Լ���ֵ
	size_t BuildAinAlarmDB(char* pAddress);
	//�������澯
	size_t BuildDinAlarmDB(char* pAddress);
	//���Ա仯
	size_t BuildLinearDB(char* pAddress);
	//�����Ա任
	size_t BuildNonLinearDB(char* pAddress);
	//ϵͳ����
	size_t BuildSystemVariableDB(char* pAddress);
	//�û�����
	size_t BuildUserVariableDB(char* pAddress);
	//tagname
	size_t BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec);
private:
	size_t m_nEstimateSize;
	size_t m_nTagNameEstimateSize;
protected:
	bool ReadScdVarFile(const char* pszFilePath);
private:
	//ת����Ϣ
	void ReadScale();
	void ReadLinearInfo();
	void ReadNonLinearInfo();
	void ReadNonLinearVal(TRANSFORM_NONLINEAR_DEF pTransFormNonLinear);
	//�澯��Ϣ
	void ReadAlarm();
	void ReadAinAlarmInfo();
	void ReadDinAlarmInfo();
	//Խ�޸澯
	void ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm);
	void ReadDinOfflimitInfo(DIN_ALARM_DEF pDinAlarm);
	void SkipUnknownElement();
	//����
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

