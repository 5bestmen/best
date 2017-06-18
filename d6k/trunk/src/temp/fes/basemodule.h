
#ifndef _BASE_MODULE_H
#define _BASE_MODULE_H

class CServer;
class CBaseModule
{
public:
	CBaseModule(CServer* pServer);
	virtual ~CBaseModule(void);

public:
	virtual bool Initialize()=0;
	virtual void Run()=0;
	virtual void Shutdown()=0;
	
protected:
	enum
	{
		MAIL_BUF_SIZE=1024,
	};

	char m_mailbuf[MAIL_BUF_SIZE];
protected:
	CServer* GetServer()
	{
		return m_pServer; 
	}

private:
	CServer* m_pServer;
	
public:
	void StartModule();
	void StopModule();
public:
	static void ModuleThreadProc(void * lpParameter);
private:
	bool    m_bIsThreadRunning;
	bool    m_bIsThreadStopped;
};

#endif // _BASE_MODULE_H


