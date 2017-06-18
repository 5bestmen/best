
#ifndef FES_H
#define FES_H

#include <vector>
#include <memory>
#include <QString>
//#include "fes_main.h"

class CDbgSvc;
class CDbSvc;
class CNetbusSvc;
class CDataView;

class CFesSvc
{
public:
	CFesSvc(void);
	~CFesSvc(void);
public:
	bool Initialize(const char *pszDataPath, unsigned int nMode);
	void Run();
	void Shutdown();
	void OnDataShow(const char *pszDataPath);
	void SetQuit()
	{
		m_bQuit = true;
	}
protected:
	void CreateMailBoxs();
	void DestroyMailBoxs();

private:
	bool m_bQuit;

private:
	//获取节点信息
	QString GetNodeInfo(const char* szName);
	//获取当前前置配置信息
	QString GetFesInfo(const char* szName);

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
private:
	std::vector<std::string> m_arrMailBoxNames;

	std::shared_ptr<CDbgSvc> m_pDbgSvc;

	std::shared_ptr<CDbSvc> m_pDbSvc;

	std::shared_ptr<CNetbusSvc> m_pNetBusSvc;

	std::shared_ptr<CDataView> m_pDataShow;
};


#endif // FES_H

