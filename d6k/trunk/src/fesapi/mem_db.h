/*! @file mem_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.h
文件实现功能 :  实时数据库模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  需要注意是否在一个进程空间内
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   实时数据库模块
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.16
*******************************************************************************/
#ifndef MEMDB_MODULE_H
#define MEMDB_MODULE_H

#include "fesapi/fes_magicmem.h"
#include "fesapi/fescnst.h"

#include <atomic>    
//#include <thread>
#include <vector>
#include <map>
#include <memory>
#include <QString>
#include <unordered_map>
#include <array>
#include <functional>
#include <ace/Event.h>

struct NODE;
struct CHANNEL;
struct DEVICE;
struct DIN;
struct AIN;
struct DOUT;
struct AOUT;
struct TRANSFORM_LINEAR;
struct TRANSFORM_NONLINEAR;
struct TRANSFORM;
struct ALARM;
struct AIN_ALARM;
struct DIN_ALARM;


typedef std::shared_ptr<TRANSFORM_LINEAR>    TRANSFORM_LINEAR_DEF;
typedef std::shared_ptr<TRANSFORM_NONLINEAR> TRANSFORM_NONLINEAR_DEF;
typedef std::shared_ptr<AIN_ALARM> AIN_ALARM_DEF;
typedef std::shared_ptr<DIN_ALARM> DIN_ALARM_DEF;

class CShareMem;

class CNode;
class CFesChannel;
class CFesDevice;
class CFesAin;
class CFesDin;
class CFesAout;
class CFesDout;
class CAinAlarm;
class CDinAlarm;
class CTagAttMgr;
 
class CMemDB 
{
public:
	CMemDB(  );
	virtual ~CMemDB(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

public:
	int32u GetNodeOccNoByHostName(const char *pszHostName);
	int32u GetMyNodeOccNo() const;
	
	int32u GetMyHostState()const;
	int8u GetMyHostScanEnable()const;

	int32u GetNodeState(int32u nOccNo)const;

	size_t GetChannelCount()
	{
		return m_arrChannels.size();
	}
	
	//统一封装打开关闭邮箱系统（仅适用于驱动侧）
	bool OpenChannelMailBox(int32u nChannelOccNo);
	bool CloseChannelMailBox(int32u nChannelOcNo);

	size_t GetDeviceCount(int32u nChannelOccNo);
	// 获取通道中的装置列表
	size_t GetDeviceOccNos(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	size_t GetAinCountInChannel(int32u nChannelOccNo);
	size_t GetDinCountInChannel(int32u nChannelOccNo);
	size_t GetAoutCountInChannel(int32u nChannelOccNo);
	size_t GetDoutCountInChannel(int32u nChannelOccNo);

	size_t GetAinCountInDevice( int32u nDeviceOccNo);
	size_t GetDinCountInDevice( int32u nDeviceOccNo);
	size_t GetAoutCountInDevice( int32u nDeviceOccNo);
	size_t GetDoutCountInDevice( int32u nDeviceOccNo);

	// 获取通道中测点的列表
	size_t GetAinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInChannel(int32u nChannelOccNo, std::vector<int32u> & arrOccNos);

	// 获取装置中测点的列表
	size_t GetAinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDinOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetAoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	size_t GetDoutOccNosInDevice(int32u nDeviceOccNo, std::vector<int32u> & arrOccNos);
	 
	bool GetChannel(int32u nChannelOccNo, CHANNEL& ch);
	bool GetDevice(int32u nDeviceOccNo, DEVICE &device);
	bool GetAin(int32u nOccNo, AIN &ai);
	bool GetDin(int32u nOccNo, DIN &di);
	bool GetAout(int32u nOccNo, AOUT &aq);
	bool GetDout(int32u nOccNo, DOUT &dq);

	bool GetAinAlarmEx(int32u nOccNo, AIN_ALARM &ainAlarm);
	bool GetDinAlarmEx(int32u nOccNo, DIN_ALARM &dinAlarm);
	bool GetAinAlarmLimit(int32u nOccNo, AIN_ALARM_LIMIT &ainAlarmLimit);
	bool GetDinAlarmLimit(int32u nOccNo, DIN_ALARM_LIMIT &dinAlarmLimit);


	bool GetChannelPtr(int32u nChannelOccNo, CHANNEL **ppFB);
	bool GetDevicePtr(int32u nDeviceOccNo, DEVICE** ppFB);
	bool GetAinPtr(int32u nOccNo, AIN** ppFB);
	bool GetDinPtr(int32u nOccNo, DIN** ppFB);
	bool GetAoutPtr(int32u nOccNo, AOUT** ppFB);
	bool GetDoutPtr(int32u nOccNo, DOUT** ppFB);

	char* GetAinAddress(int32u nOccNo);
	char* GetDinAddress(int32u nOccNo);

	char* GetAoutAddress(int32u nOccNo);
	char* GetDoutAddress(int32u nOccNo);


	
	std::shared_ptr<CAinAlarm>GetAinAlarm(int32u nOccNo);
	std::shared_ptr<CDinAlarm>GetDinAlarm(int32u nOccNo);

	TRANSFORM_LINEAR * GetTransformLinear(int32u nOccNo);
	TRANSFORM_NONLINEAR * GetTransformNonLinear(int32u nOccNo);

	// 获取occno
	int32u GetOccNoByTagName(const char * szTagName, int32u &nDataType);

	bool IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality, TIMEPAK * pTm);
	bool IoSetAinValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	bool IoGetAinValue(int32u nOccNo, fp64 &fValue, int8u &nQuality) const;

	bool IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality, bool bCpuEnable, TIMEPAK * pTm);
	bool IoSetDinValue(int32u nOccNo, int8u nValue, int8u nQuality);
	bool IoDinAlarm(int32u nOccNo, int8u nValue, int8u nQuality, TIMEPAK * pTm);

	bool IoGetDinValue(int32u nOccNo, int8u &nValue, int8u &nQuality)const;

	bool IoSetAoutValue(int32u nOccNo, fp64 fValue, int8u nQuality);
	bool IoSetDoutValue(int32u nOccNo, int8u nValue, int8u nQuality);

	bool IoSetAinQua(int32u nOccNo, int8u nQuality);
	bool IoSetDinQua(int32u nOccNo, int8u nQuality);
	bool IoSetAoutQua(int32u nOccNo, int8u nQuality);
	bool IoSetDoutQua(int32u nOccNo, int8u nQuality);

	bool IoSetDeviceQua(int32u nOccNo, int8u nQuality);
	bool IoSetChannelQua(int32u nOccNo, int8u nQuality);

	void IoSetDeviceHeartBeat(int32u nOccNo);
	void IoSetChannelHeartBeat(int32u nOccNo);
	
	bool AppGetAinValue(int32u nOccNo, fp64* pValue, int8u *pQua);
	bool AppGetDinValue(int32u nOccNo, int8u* pValue, int8u *pQua);
	
	// 获取用户变量的测值
	bool AppGetUserVarValue(int32u nOccNo,IO_VARIANT *pVariant,int8u * pQua);
	// 获取系统变量的测值
	bool AppGetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u * pQua);
	// 应用层修改用户变量的测值
	bool AppSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u  nQua);
	// 驱动层修改用户变量的测值
	bool IoSetUserVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);
	// 驱动层修改系统变量的测值
	bool IoSetSysVarValue(int32u nOccNo, IO_VARIANT *pVariant, int8u nQuality);

	// 设开出
	bool AppSetDoutValue(int32u nOccNo, int8u Value, int8u nSource);
	bool AppSetAoutValue(int32u nOccNo, fp64 Value, int8u nSource);

	bool GetRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, IO_VARIANT &RetData);
	bool PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, const IO_VARIANT &RetData,void *pExt);
	bool PutRTData(int32u nIddType, int32u nOccNo, int32u nFiledID, int32u nParam, IO_VARIANT *pData, 
		const char * szOperatorName, const char * szMonitorName, void *pExt, bool bOpLog);

	bool IsDBAlive(int nTimeout);

	void IoDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 操作报警
	void IoOperAlarm(int32u nChannleNo, int32u nDeviceNo, const char * pszAlarmTxt, TIMEPAK * pTm);
	// 保护的自诊断
	void IoRelayDiagAlarm(int32u nChannleNo, int32u nDeviceNo, const char* pszAlarmTxt, TIMEPAK * pTm);
	// 通用的告警
	void IoAlarmMsg(int32u nChannleNo, int32u nAlarmType, const char* pszAlarmTxt, TIMEPAK * pTm);

	int32s GetChannelMailBoxID(int32u nChannelNo);
	bool ReadHostCmd(int32u nChannleNo, SETVAL_MSG *pCmd, int32u nTimeout);

private:
	//  内存库
	bool BuildMemDB(const char *szDBName);

	size_t CreateNodeTable(unsigned char *pHead);
	size_t CreateChannelTable(unsigned char *pHead);
	size_t CreateDeviceTable(unsigned char *pHead);

	size_t CreateAinTable(unsigned char *pHead);
	size_t CreateDinTable(unsigned char *pHead);
	size_t CreateAoutTable(unsigned char *pHead);
	size_t CreateDoutTable(unsigned char *pHead);

	size_t CreateLinearTable(unsigned char *pHead);
	size_t CreateNonLinearTable(unsigned char *pHead);

	size_t CreateAinAlarmTable(unsigned char *pHead);
	size_t CreateDinAlarmTable(unsigned char *pHead);
	
	size_t CreateSystemVariableTable(unsigned char* pHead);
	size_t CreateUserVariableTable(unsigned char* pHead);
	void LogMsg(const char * pszText, int nLevel);

	void InitFuncArrary();
	// 检查开出的闭锁状态
	void CheckDoutBlockState(DOUT & dout);
		
	// 获取各 基础对象 的属性值 函数
	//NODE
	bool GetNodeScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetNodeQuality(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetNodeRedState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetNodeState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetNodeNetA(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetNodeNetB(int32u nOccNo, IO_VARIANT &RetData) const;

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

	//AOUT
	bool GetAoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutPin(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetAoutUint(int32u nOccNo, IO_VARIANT &RetData) const;

	//DOUT
	bool GetDoutScanEnable(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutQua(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutState(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutOutPut(int32u nOccNo, IO_VARIANT &RetData) const;
	bool GetDoutPin(int32u nOccNo, IO_VARIANT &RetData) const;

	bool SetChannelAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetNodeAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

	bool SetDeviceAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetAinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetDinAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

	bool SetAoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);
	bool SetDoutAttrValue(int32u nOccNo, int32u nFiledId, const IO_VARIANT & varVal);

private:
	QString  m_szRdbName;

	HEAD_MEM* m_pMagicMem;

	std::shared_ptr<ACE_Event> m_pDBAliveFlag;

	int  m_nDbSvcMailBoxID;
	int  m_nNBSvcMailBoxID;
private:
	//bool m_bInitialized; //! 进程内初始化过的标志，后续需要修改成原子操作
	std::atomic<bool> m_bInitialized;
	//! 所谓的内存数据库
	std::shared_ptr<CShareMem> m_pMem;

	NODE *m_pNodes;
	unsigned int m_nNodeCount;
	std::unordered_map<int32u, std::shared_ptr<CNode>> m_mapNodes;

	// 共享内存中的各数据的排布
	CHANNEL *m_pChannels;
	unsigned int m_nChannelCount;
	//! 记住地址，方便遍历
	std::vector<CHANNEL*> m_arrChannels;
	std::unordered_map<int32u, std::shared_ptr<CFesChannel>> m_mapChannels;

	DEVICE *m_pDevices;
	unsigned int m_nDeviceCount;
	std::vector<DEVICE*> m_arrDevices;
	std::unordered_map<int32u, std::shared_ptr<CFesDevice>> m_mapDevices;

	AIN *m_pAins;
	unsigned int m_nAinCount;
	std::vector<AIN*> m_arrAins;
	std::unordered_map<int32u, std::shared_ptr<CFesAin>> m_mapAins;

	DIN *m_pDins;
	unsigned int m_nDinCount;
	std::vector<DIN*> m_arrDins;
	std::unordered_map<int32u, std::shared_ptr<CFesDin>> m_mapDins;

	AOUT *m_pAouts;
	unsigned int m_nAoutCount;
	std::vector<AOUT*> m_arrAouts;
	std::unordered_map<int32u, std::shared_ptr<CFesAout>> m_mapAouts;

	DOUT *m_pDouts;
	unsigned int m_nDoutCount;
	std::vector<DOUT*> m_arrDouts;
	std::unordered_map<int32u, std::shared_ptr<CFesDout>> m_mapDouts;

	TRANSFORM_LINEAR *m_pLinear;
	unsigned int m_nLinearCount;
	std::vector<TRANSFORM_LINEAR*> m_arrLinears;

	TRANSFORM_NONLINEAR *m_pNonLinear;
	unsigned int m_nNonLinearCount;
	std::vector<TRANSFORM_NONLINEAR*> m_arrNonLinears;

	AIN_ALARM  *m_pAinAlarm;
	unsigned int m_nAinAlarmCount;
	std::vector<AIN_ALARM*> m_arrAinAlarms;
	std::unordered_map<int32u, std::shared_ptr<CAinAlarm>> m_mapAinAlarms;

	AIN_ALARM_LIMIT *m_pAinAlarmLimit;
	unsigned int m_pAinAlarmLimitCount;
	std::vector<AIN_ALARM_LIMIT*> m_arrAinAlarmLimits;
	std::unordered_map<int32u, std::shared_ptr<AIN_ALARM_LIMIT>> m_mapAinAlarmLimits;

	DIN_ALARM *m_pDinAlarm;
	unsigned int m_nDinAlarmLimitCount;
	std::vector<DIN_ALARM*> m_arrDinAlarms;
	std::unordered_map<int32u, std::shared_ptr<CDinAlarm>> m_mapDinAlarms;

	DIN_ALARM_LIMIT *m_pDinAlarmLimit;
	unsigned int m_nDinAlarmCount;
	std::vector<DIN_ALARM_LIMIT*> m_arrDinAlarmLimits;
	std::unordered_map<int32u, std::shared_ptr<DIN_ALARM_LIMIT>> m_mapDinAlarmLimits;

	VARDATA * m_pSystemVariable;
	unsigned int m_nSystemVariableCount;
	std::vector<VARDATA* >m_arrSystemVariables;

	VARDATA*  m_pUserVariable;
	unsigned int m_nUserVariableCount;
	std::vector<VARDATA* >m_arrUserVariables;

	 //using BaseObjectAttrFuncs = std::array<std::function<bool(int32u, IO_VARIANT&) >, ATT_MAX>;
	// 获取各个基础对象的属性
	//std::array<std::function<bool(int32u, int32u, IO_VARIANT&) >  ,ATT_MAX> m_arrGetRtDataFuncs;
	//std::array< BaseObjectAttrFuncs, MAX_IDD> m_arrGetRTDataFuncs;

	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetNodeRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetChannelRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDeviceRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetAinRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDinRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetAoutRTDataFuncs;
	std::array< std::function<bool(int32u, IO_VARIANT&)  >, ATT_MAX> m_arrGetDoutRTDataFuncs;

	//std::array< BaseObjectAttrFuncs, MAX_IDD> m_arrGetRTDataFuncs;

	std::shared_ptr<CTagAttMgr> m_pTagAttrMgr;
};

#endif // MEMDB_MODULE_H


/** @}*/

