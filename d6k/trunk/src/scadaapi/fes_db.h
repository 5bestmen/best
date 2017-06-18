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
#include "fesapi/fescnst.h"
#include <QString>
#include < array >
#include < functional >
struct HEAD_MEM;
struct VARDATA;
class CFesDB : public CMemDB
{
public:
	CFesDB( );
	virtual ~CFesDB(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode,int32u nOccNo);
	virtual void Run();
	virtual void Shutdown();
public:
	bool LoadMem(unsigned char* pAddr);
	// 应用层 获取模拟量
	virtual bool GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;
	virtual bool GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;
public:
	bool  GetChannelByOccNo(int32u  nOccNo,CHANNEL** pChannel);
	bool  GetDeviceByOccNo(int32u nOccNo, DEVICE** pDev);
	bool  GetAinByOccNo(int32u nOccNo, AIN** pAin);
	bool  GetDinByOccNo(int32u nOccNo, DIN** pDin);
	bool  GetAoutByOccNo(int32u nOccNo, AOUT **pAout);
	bool  GetDoutByOccNo(int32u nOccNo, DOUT** pDout);

	bool  GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData);
	
	bool  PutRtData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc);
protected:
	size_t CreateChannel(unsigned char* pAddr);
	size_t CreateDevice(unsigned char* pAddr);
	size_t CreateAin(unsigned char* pAddr);
	size_t CreateDin(unsigned char* pAddr);
	size_t CreateAout(unsigned char* pAddr);
	size_t CreateDout(unsigned char* pAddr);

	size_t CreateAinAlarm(unsigned char* pAddr);
	size_t CreateAinLimitAlarm(unsigned char* pAddr);
	size_t CreateDinAlarm(unsigned char* pAddr);
	size_t CreateDinLimitAlarm(unsigned char* pAddr);

	size_t CreateSystemVariable(unsigned char* pAddr);
	size_t CreateUserVariable(unsigned char* pAddr);

	void InitFuncArrary();
private:
 
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

	//ADD 2017.06.06
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
private:
	//CHANNEL
	bool GetChannelScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetChannelQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetChannelState(int32u nOccNo, IO_VARIANT &RetData) const;

	//DEVICE
	bool GetDeviceScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDeviceQua(int32u nOccNo, IO_VARIANT &RetData) const;

	//AIN
	bool GetAinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinState(int32u nOccNo, IO_VARIANT &RetData) const;

	bool GetAinValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinSignalValEx(int32u nOccNo, IO_VARIANT &RetData) const;

	bool GetAinManSet(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinLowOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinHighOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinHighQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinLowQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinDesc(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinPin(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAinUint(int32u nOccNo, IO_VARIANT &RetData) const;

	//DIN
	bool GetDinScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinManSet(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinStart(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinDesc0(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinDesc1(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinPin(int32u nOccNo, IO_VARIANT &RetData) const;

	bool GetDinValEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinNegValueEx(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDinRawValueEx(int32u nOccNo, IO_VARIANT &RetData) const;

	//AOUT
	bool GetAoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutPin(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutUint(int32u nOccNo, IO_VARIANT &RetData) const;

	//bool SetAoutValue(int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, void *pExt, void *pSrc);
	//DOUT
	bool GetDoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutPin(int32u nOccNo, IO_VARIANT &RetData) const;

	bool SetDoutValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc);
	bool SetAoutValue(int32u nOccNo, IO_VARIANT *pData, void *pExt, void *pSrc);

private:

	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetChannelRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDeviceRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetAinRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDinRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetAoutRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDoutRTDataFuncs;

	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetSystemVariablesFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetUserVariablesFuncs;

	//设置属性
	std::array< std::function<bool(int32u,  IO_VARIANT*, void*, void* )   >, ATT_MAX> m_arrAoutSetFunctions;
	std::array< std::function<bool(int32u,  IO_VARIANT*, void*, void* )   >, ATT_MAX> m_arrDoutSetFunctions;
}; 

#endif // FESDB_MODULE_H


/** @}*/

