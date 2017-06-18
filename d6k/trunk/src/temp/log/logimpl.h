
#ifndef _LOG_IMPL_H
#define _LOG_IMPL_H

#include "utils/queue.h"

#include "logcache.h"

#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"

class CLogImpl
{
public:
	CLogImpl(void);
	~CLogImpl(void);

	//Part1
	//////////////////////////////////////////////////////////////////////////
	//开放接口
public:
	bool CreateLog(char* szLogName);
	bool DestroyLog();
	bool OpenLog(char* szLogName);
	bool CloseLog();

	bool LogMsg(char* szMoudleName,char* szLogTxt,int nLevel);
private:
	bool PrepareLog(char* szLogName);
	std::string			m_strLogName; //log名称
	unsigned int	m_nLogID;     //logID(标识共享内存)

	//Part2
	//////////////////////////////////////////////////////////////////////////
	//接收并缓存日志
private:
	bool StartCache();//启动接收并缓存线程
	void StopCache(); //停止接收并缓存线程
private:
	bool CreateCache(); //创建日志共享内存缓存队列
	bool OpenCache();   //打开日志共享内存缓存队列
	void CloseCache();  //关闭日志共享内存缓存队列
private:
	bool DoLogCache();  //日志缓存核心逻辑
	static void LogCacheThreadProc(void * lpParameter);//日志缓存线程
private:
	bool    m_bIsCacheing;    //线程运行标志
	bool    m_bIsCacheStopped;//线程退出标志
private:
	enum{LOG_QUEUE_SIZE = 1000,};//共享内存日志队列大小(最大日志条数)
	CQueue<LOGDATA> m_logQueue;  //共享内存日志队列

	//Part3
	//////////////////////////////////////////////////////////////////////////
	//定时或定量处理缓存的日志
private:
	void StartProcess();//启动缓存处理线程
	void StopProcess(); //停止缓存处理线程
private:
	bool DoLogProcess();//日志处理核心逻辑
	static void LogProcessThreadProc(void * lpParameter);//日志处理线程

	bool Log2Sqlite();
	bool Log2File();
	bool m_bIsLog2File;
private:
	void InitSqlite();
	void UnInitSqlite();
	Kompex::SQLiteDatabase* m_pDatabase;
	Kompex::SQLiteStatement* m_pStmt;
private:
	bool    m_bIsProcessing;    //线程运行标志
	bool    m_bIsProcessStopped;//线程退出标志
private:
	CLogCache m_logCache;//本地日志缓存队列
	int m_nLastTime;	 //上次处理日志时间
	int m_nCurrTime;     //当前时间
	enum
	{
		LOG_HANDLE_CYCLE = 5,    //每5秒处理一次日志
		LOG_HANDLE_COUNT = 1000, //每达到1000条处理一次日志
	};

};

extern CLogImpl g_LogImpl;


#endif // _LOG_IMPL_H

