
#ifndef _BASE_MODULE_H
#define _BASE_MODULE_H
 

#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <chrono>  

 
class CScadaSvc;
class CBaseModule
{
public:
	CBaseModule(CScadaSvc* pServer,const std::string & szMailBoxName, int &nMailID);
	virtual ~CBaseModule(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode) = 0;
	virtual void Run()=0;
	virtual void Shutdown()=0;
	
	void LogMsg(const char *szLogTxt, int nLevel); 
protected:
	CScadaSvc* GetScadaSvc()
	{
		return m_pServer; 
	}

private:
	CScadaSvc* m_pServer;
	
public:
	void StartModule();
	void StopModule();
	std::string   GetMailBoxName() const
	{
		return m_szMailBoxName;
	}
public:
//	static void ModuleThreadProc(void * lpParameter);

	void MainTask(void *pImpl);

	void SetTaskStopFlag();
	void SetTaskStartFlag();

	bool IsStopped()
	{
		return m_bStopFlag;
	}

	//! Ö÷ÈÎÎñ
	std::shared_ptr< std::thread > m_pMainTask;
	mutable std::mutex m_mutThread;
	std::condition_variable m_cvThread;
	std::atomic<bool> m_bStopFlag;
	
	int GetMailBoxID() const
	{
		return m_nMailBoxID;
	}
protected:
	int &m_nMailBoxID;
	std::string m_szMailBoxName;
protected:
	virtual void MainLoop(){}

private:
	bool    m_bIsThreadRunning;
	bool    m_bIsThreadStopped;
};

#endif // _BASE_MODULE_H


