/*! @file fes_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  fes_db.h
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
#ifndef FESDB_MODULE_H
#define FESDB_MODULE_H

#include "mem_db.h"
#include "fesconfig.h"
#include "fesapi/fes_magicmem.h"

#include <QXmlStreamReader>

struct HEAD_MEM;
struct TAG_OCCNO;
struct VARDATA;
typedef std::shared_ptr<TAG_OCCNO> TAG_OCCNO_DEF;
typedef std::shared_ptr<VARDATA> VARDATA_DEF;
class CFesDB : public CMemDB
{
public:
	CFesDB( );
	virtual ~CFesDB();

public:
	bool Initialize(const char *pszDataPath, unsigned int nMode,int32u nOccNo) ;
public:
	virtual void Run();
	virtual void Shutdown();
public:
	// 应用层 获取模拟量
	virtual bool GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;
	virtual bool GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;

	//网络总线服务调用，更新测值
	virtual bool UpdateAinValue(int32u nOccNo, const CVariant & val, int8u nQuality);
	virtual bool UpdateDinValue(int32u nOccNo, const CVariant & val, int8u nQuality);
	
	bool GetAinAlarmByOccNo(int32u nOccNo,AIN_ALARM** pAinAlarm);
	bool GetAinAlarmLimitByOcNo(int32u nOccNo,AIN_ALARM_LIMIT** pAinAlarmLimit);
	bool GetDinAlarmByOccNo(int32u nOccNo,DIN_ALARM** pDinAlarm);
	bool GetDinAlarmLimitByOccNo(int32u nOccNo,DIN_ALARM_LIMIT** pDinAlarmLimit);


	//预估内存大小
	size_t EstimateMemSize();
	size_t BuildDB(char* pAddr);
protected:
	size_t BuildChannelDB(char* pAddr);
	size_t BuildDeviceDB(char* pAddr);
	size_t BuildAinDB(char* pAddr);
	size_t BuildDinDB(char* pAddr);
	size_t BuildAoutDB(char* pAddr);
	size_t BuildDoutDB(char* pAddr);
	size_t BuildAinAlarms(char* pAddr);
	size_t BuildAinAlarmLimits(char* pAddr);
	size_t BuildDinAlarms(char* pAddr);
	size_t BuildDinAlarmLimits(char* pAddr);

	//系统变量
	size_t BuildSystemVariableDB(char* pAddress);
	//用户变量
	size_t BuildUserVariableDB(char* pAddress);

private:
	//概要信息
	bool LoadFesXml(const char* pszDataPath);
	bool ReadFesInfo();
	void ReadChannelInfo();
	void ReadDev(CChannelPtr pChannel);
	void ReadDevInfo(CChannelPtr pChannel);
	void ReadData(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDataInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadAinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDinInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadAoutInfo(CDevicePtr pDev, CChannelPtr pChannel);
	void ReadDoutInfo(CDevicePtr pDev, CChannelPtr pChannel);

	void ReadAlarm();

	void ReadAinAlarmInfo();
	void ReadAinOfflimitInfo(AIN_ALARM_DEF pAinAlarm);

	void ReadDinAlarmInfo();
	void ReadDinAlarmLimitInfo(DIN_ALARM_DEF pDinAlarm);

	void ReadVariable();
	void ReadSysVariable();
	void ReadUserVariable();

	void SkipUnknownElement();
	void ClearTempArrays();
private:
	QXmlStreamReader m_XmlReader;
	std::shared_ptr<CChannelMgr>          m_pChannelMgr;
	std::vector<std::shared_ptr<CHANNEL>> m_arrTempChannels;
	std::vector<std::shared_ptr<DEVICE>>  m_arrTempDevices;
	std::vector<std::shared_ptr<AIN>>     m_arrTempAins;
	std::vector<std::shared_ptr<DIN>>     m_arrTempDins;
	std::vector<std::shared_ptr<AOUT>>    m_arrTempAouts;
	std::vector<std::shared_ptr<DOUT>>    m_arrTempDouts;

	std::vector<std::shared_ptr<AIN_ALARM > > m_arrTempAinAlarms;
	std::vector<std::shared_ptr<AIN_ALARM_LIMIT> > m_arrTempAinAlarmLimits;
	std::vector<std::shared_ptr<DIN_ALARM> > m_arrTempDinAlarms;
	std::vector<std::shared_ptr<DIN_ALARM_LIMIT> > m_arrTempDinAlarmLimits;

	std::vector<VARDATA_DEF> m_arrTempSystemVariables;
	std::vector<VARDATA_DEF> m_arrTempUserVariables;

	size_t m_nEstimateSize;
	QString m_szProjName;
//以下为内存结构，便于遍历
protected:
	HEAD_MEM* m_pMagicMem;
	int32u m_nOccNo;
	CHANNEL *m_pChannels;
	unsigned int m_nChannelCount;
	std::vector<CHANNEL*> m_arrChannels;  //! 记住地址，方便遍历
	
	DEVICE *m_pDevices;
	unsigned int m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;

	AIN *m_pAins;
	unsigned int m_nAinCount;
	std::vector<AIN*> m_arrAins;

	DIN *m_pDins;
	unsigned int m_nDinCount;
	std::vector<DIN*> m_arrDins;

	DOUT *m_pDouts;
	unsigned int m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;

	AOUT *m_pAouts;
	unsigned int m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;

	//ADD ALARM/LIMIT 2017.03.22
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

public:
	size_t EstimateTagNameMemSize();
	size_t BuilTagNamedDB(char* pAddr);
	size_t BuildTagNameDB(char* pAddress, std::vector<TAG_OCCNO_DEF>& vec);

private:
	HEAD_MEM* m_pMagicTagMem;
	size_t m_nTagNameEstimateSize;
	std::vector<TAG_OCCNO_DEF> m_arrChannelOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDeviceOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAoutOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDoutOccNos;

	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrAinAlarmLimitOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrDinAlarmLimitOccNos;

	std::vector<TAG_OCCNO_DEF> m_arrSyetemVarOccNos;
	std::vector<TAG_OCCNO_DEF> m_arrUserVarOccNos;
};

#endif // FESDB_MODULE_H


/** @}*/

