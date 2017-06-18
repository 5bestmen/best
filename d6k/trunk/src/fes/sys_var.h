/*! @file sys_var.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  sys_var.h
�ļ�ʵ�ֹ��� :  ϵͳ��������ģ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ϵͳ��������ģ��
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
		const char * TagName;     //! ��������
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
	// ϵͳ�����Ĵ�����
	void DateTimeHandler();

protected:
	virtual void MainLoop();
private:
	CDbSvc * m_pDBSvc;

 	static std::array<SysConfig, $SV_MAX> s_arrSysVars;

private:
	//! ������
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	std::atomic<bool> m_bStopFlag;
};

#endif // _DBGSVR_MODULE_H


/** @}*/

