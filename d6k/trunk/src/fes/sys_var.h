/*! @file sys_var.h
<PRE>
********************************************************************************
模块名       :
文件名       :  sys_var.h
文件实现功能 :  系统变量处理模块
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   系统变量处理模块
*  @author  LiJin
*  @version 1.0
*  @date    2016.10.31
*******************************************************************************/
#ifndef SYSVAR_MODULE_H
#define SYSVAR_MODULE_H
 
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
//#include <chrono>  
#include <array>
#include "datatypes.h"
 
class CDbSvc;
class CSysVarSvc 
{
public:
	CSysVarSvc(CDbSvc* pServer);
	virtual ~CSysVarSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();
public:
	enum SYS_TABLE
	{
		$SV_NULL = 0,
		$CPU = 1,
		$YEAR, 
		$DAY,
		$MONTH,
		$HOUR, 
		$MINUTE,

		$SV_MAX = $MINUTE
	};
	struct SysConfig
	{
		const char * TagName;     //! 邮箱名称
		int32u OccNo;       //! OccNo
		int8u   DataType;
		int8u   IddType;
	};
protected:
	void StartModule();
	void StopModule();

	void LogMsg(const char *szLogTxt, int nLevel);
	void MainTask(void *pImpl);

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}
	// 系统变量的处理函数
	void DateTimeHandler();

protected:
	virtual void MainLoop();
private:
	CDbSvc * m_pDBSvc;

 	static std::array<SysConfig, $SV_MAX> s_arrSysVars;

private:
	//! 主任务
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	std::atomic<bool> m_bStopFlag;
};

#endif // _DBGSVR_MODULE_H


/** @}*/

