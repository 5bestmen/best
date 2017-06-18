#ifndef SCDSVC_H
#define SCDSVC_H

#include "scdsvc_global.h"
#include <vector>
#include <memory>

//class SCADA_API CScadaSvc
class CScadaSvc;

class CDbgSvc;
class CDbSvc;
class CNetbusSvc;
class CAudioAlarmSvc;

class CScadaSvc
{
public:
    CScadaSvc();
	~CScadaSvc();
public:
	bool Initialize(const char *pszDataPath, unsigned int nMode);
	void Run();
	void Shutdown();
protected:
	void CreateMailBoxs();
	void DestroyMailBoxs();
public:
	std::shared_ptr<CDbSvc>& GetDBSvc()
	{
		return m_pDbSvc;
	}
	std::shared_ptr<CDbgSvc>& GetDBGSvc()
	{
		return m_pDbgSvc;
	}
	std::shared_ptr<CNetbusSvc>& GetNBSvc()
	{
		return m_pNetBusSvc;
	}
	std::shared_ptr<CAudioAlarmSvc>& GetAudioSvc()
	{
		return m_pAudioAlarmSvc;
	}
private:
	std::vector<std::string> m_arrMailBoxNames;

	std::shared_ptr<CDbgSvc> m_pDbgSvc;

	std::shared_ptr<CDbSvc> m_pDbSvc;

	std::shared_ptr<CNetbusSvc> m_pNetBusSvc;

	std::shared_ptr<CAudioAlarmSvc> m_pAudioAlarmSvc;
};

#endif // SCDSVC_H
