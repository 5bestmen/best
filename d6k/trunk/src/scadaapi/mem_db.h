/*! @file mem_db.h
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.h
文件实现功能 :  内存数据库基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.2
*******************************************************************************/
#ifndef MEMDB_MODULE_H
#define MEMDB_MODULE_H

#include "datatypes.h"
#include "../scdpub/variant_data.h"

#include "base_mem_db.h"

#include <vector>
#include <memory>
#include <thread>

#include <mutex>
#include <condition_variable>
#include <chrono>  
#include <atomic>


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
struct AIN_ALARM_LIMIT;
struct DIN_ALARM;
struct DIN_ALARM_LIMIT; 

using TRANSFORM_LINEAR_DEF = std::shared_ptr<TRANSFORM_LINEAR>;
using TRANSFORM_NONLINEAR_DEF = std::shared_ptr<TRANSFORM_NONLINEAR>;
using AIN_ALARM_DEF = std::shared_ptr<AIN_ALARM>;
using DIN_ALARM_DEF = std::shared_ptr<DIN_ALARM>;
using AIN_ALARM_LIMIT_DEF = std::shared_ptr<AIN_ALARM_LIMIT>;
using DIN_ALARM_LIMIT_DEF = std::shared_ptr<DIN_ALARM_LIMIT> ; 

class CMemDB : public CBaseMemDB
{
public:
	CMemDB( );
	virtual ~CMemDB( );

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
 	 
public: 
	// 应用层 获取模拟量  (其实，AIN\DIN\AO\DO等定位变量只有在前置有，SCADA上暂没有)
	// 本接口暂时采用这种通用设计，为今后SCADA上支持定位变量的可能做准备。
	virtual bool GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;
	virtual bool GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const;


#if 0
	// 预估所需共享内存的尺寸
	virtual size_t EstimateMemSize();

	//网络总线服务调用，更新测值
	virtual bool UpdateAinValue(int32u nOccNo, const CVariant & val, int8u nQuality);
	virtual bool UpdateDinValue(int32u nOccNo, const CVariant & val, int8u nQuality);
#endif


protected:
	void LogMsg(const char *szLogTxt, int nLevel);
	void MainTask(void *pImpl);

	void StartModule();
	void StopModule();

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}

	//! 主任务
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	std::atomic<bool> m_bStopFlag; 
protected:
	// 注意：节点信息保存到db_svc里面，每个子节点不再维护节点信息
//	std::vector<std::shared_ptr<NODE>>    m_arrTempNodes;
	std::vector<std::shared_ptr<CHANNEL>> m_arrTempChannels;
	std::vector<std::shared_ptr<DEVICE>>  m_arrTempDevices;
	std::vector<std::shared_ptr<AIN>>     m_arrTempAins;
	std::vector<std::shared_ptr<DIN>>     m_arrTempDins;
	std::vector<std::shared_ptr<AOUT>>    m_arrTempAouts;
	std::vector<std::shared_ptr<DOUT>>    m_arrTempDouts;

	std::vector<TRANSFORM_LINEAR_DEF>     m_arrTempTransFormLinears;
	std::vector<TRANSFORM_NONLINEAR_DEF>  m_arrTempTransFormNonLinears;

	std::vector<AIN_ALARM_DEF>            m_arrTempAinAlarms;
	std::vector<DIN_ALARM_DEF>            m_arrTempDinAlarms;
	std::vector<AIN_ALARM_LIMIT_DEF>      m_arrTempAinLimitAlarms;
	std::vector<DIN_ALARM_LIMIT_DEF>      m_arrTempDinLimitAlarms;


private:
	bool    m_bIsThreadRunning;
	bool    m_bIsThreadStopped;

};

#endif // _DBGSVR_MODULE_H


/** @}*/

