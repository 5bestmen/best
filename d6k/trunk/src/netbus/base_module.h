
#ifndef _BASE_MODULE_H
#define _BASE_MODULE_H 

#include <memory>
#include <thread>
#include <atomic>

#include <mutex>
#include <condition_variable>
#include <chrono>  

 
class CNetbusSvc;
class CBaseModule
{
public:
	CBaseModule(CNetbusSvc* pServer );
	virtual ~CBaseModule(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode) = 0;
	virtual void Run()=0;
	virtual void Shutdown()=0;
	
	void LogMsg(const char *szLogTxt, int nLevel); 
protected:
	CNetbusSvc* GetFesSvc()
	{
		return m_pServer; 
	}

private:
	CNetbusSvc* m_pServer;
	
public:
	void StartModule();
	void StopModule();
 
public:
 
	void MainTask(void *pImpl);

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}

	//! ������
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;

	std::atomic<bool> m_bStopFlag; 
protected:
	virtual void MainLoop();

protected:
 

private:
	bool    m_bIsThreadRunning;
	bool    m_bIsThreadStopped;
};

#endif // _BASE_MODULE_H


