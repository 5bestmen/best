
#ifndef _SERVER_H
#define _SERVER_H

class CRtdbModule;
class CCoreModule;
class CDbgSvrModule;
class CNetBusModule;

class CServer
{
public:
	CServer(void);
	~CServer(void);

public:
	bool Initialize();
	void Run();
	void Shutdown();

public:
	void SetQuit()
	{
		m_bQuit = true;
	}
private:
	bool m_bQuit;

public:
	CRtdbModule*	GetRtDbModule() { return m_pRtDbModule; }
	CCoreModule*	GetCoreModule() { return m_pCoreModule; }
	CDbgSvrModule*	GetDbgSvrModule() { return m_pDbgSvrModule; }
	CNetBusModule*	GetNetBusModule() { return m_pNetBusModule; }
private:
	CRtdbModule*	m_pRtDbModule;
	CCoreModule*	m_pCoreModule;
	CDbgSvrModule*	m_pDbgSvrModule;
	CNetBusModule*	m_pNetBusModule;
};

#endif // _SERVER_H
