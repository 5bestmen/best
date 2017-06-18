#ifndef VMCS_MAIN_H
#define VMCS_MAIN_H
#include "mainfrm.h"
#include <QObject>
#include <memory>
#include <array>
#include <vector>
#include <QMainWindow>
#include <QWidget>
#include <QDomElement>
class CGraphContainer;

class CVmcs
{
public:
	CVmcs();
	~CVmcs();
public:
	std::vector < SIM_GRAPH_DEF >& GetFileInfo() 
	{
		return m_arrFiles;
	}
	bool Initalize(const char * pszProjectName);
	bool ShutDown(const char * pszProjectName);
private:
	bool LoadProj(const char* pszProjectName);
	void AnalyseGroupFile(QDomElement nElememt);
	void LogMsg(const char* logMsg,int nLevel);
private:
	std::vector < SIM_GRAPH_DEF > m_arrFiles;
	std::shared_ptr<CGraphContainer> m_pGraphContainer;
	std::shared_ptr<CMainFrame> m_pMainFrame;
};



#endif // VMCS_MAIN_H
