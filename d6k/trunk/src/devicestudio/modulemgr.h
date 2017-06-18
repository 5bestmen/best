#ifndef MANAGEMODULE_H
#define MANAGEMODULE_H

#include <vector>
#include <QString>

class CMainModule;
class IMainModule;
class IPluginModule;
class CModuleMgr  
{
public:
	CModuleMgr(IMainModule *pMainModule);
	~CModuleMgr();
	//加载动态库
	bool LoadModule(const QString &module, const QString &strDeviceName);

private:
	IMainModule *m_pCore;

	std::vector<IPluginModule*> m_arrModules;
};

#endif // MANAGEMODULE_H
