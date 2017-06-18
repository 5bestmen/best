#if _MSC_VER >= 1900
//#ifdef DEBUG
//#include <vld.h>
//#endif // DEBUG
#endif

#include "vmcs_main.h"
#include "vmc/hmiapi.h"
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#include "graph_container.h"
#include "mainfrm.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
CVmcs::CVmcs( )
{
	//m_pGraphContainer = std::make_shared<CGraphContainer>();
	m_pMainFrame = std::make_shared<CMainFrame>();
}

CVmcs::~CVmcs()
{
	m_pMainFrame.reset();
}


bool CVmcs::Initalize(const char * pszProjectName)
{
	Q_ASSERT(pszProjectName &&strlen(pszProjectName) > 0);
	if (!pszProjectName || strlen(pszProjectName) == 0)
	{
		return false;
	}
	if (!LoadProj(pszProjectName))
	{
		return false;
	}

	m_pMainFrame->SetProjPath(pszProjectName);

	if (!m_pMainFrame->Initalize(GetFileInfo()))
	{
		return false;
	}


	m_pMainFrame->show();
	
	return true;
}

bool CVmcs::ShutDown(const char * pszProjectName)
{
	Q_ASSERT(pszProjectName &&strlen(pszProjectName) > 0);
	if (!pszProjectName || strlen(pszProjectName) == 0)
	{
		return false;
	}

	m_pMainFrame.reset();
	m_arrFiles.clear();

	return true;
}


bool CVmcs::LoadProj(const char* pszProjectName)
{
	Q_ASSERT(pszProjectName &&strlen(pszProjectName) > 0);
	if (!pszProjectName || strlen(pszProjectName) == 0)
	{
		return false;
	}
	QFile file(pszProjectName);

	QDomDocument doc;

	if (!file.open(QIODevice::ReadOnly))
		return  false;

	if (!doc.setContent(&file))
	{
		file.close();
		Q_ASSERT(false);
		return false;
	}

	QDomElement docElem = doc.documentElement();

	if (docElem.tagName() != "Project")
	{
		file.close();
		return false;
	}

	QDomNode nChild = docElem.firstChild();

	bool bRet;
	
	for (; !nChild.isNull(); nChild = nChild.nextSibling())
	{
		QDomElement n = nChild.toElement();

		//获取节点配置信息
		if (n.nodeName() == "graph")
		{
			QDomElement nElement = n.toElement();
			if (nElement.hasChildNodes())
			{			
				QDomNodeList nList = nElement.childNodes();

				for (int i=0;i<nList.size();++i)
				{
					QDomNode nNode = nList.at(i);

					QDomElement tElement = nNode.toElement();
					
					if (tElement.tagName() == "file")
					{
						auto pGraph = std::make_shared<GraphFileDataUnit>();
						pGraph->szFileName = nNode.toElement().attribute("name").toStdString();
						m_arrFiles.push_back(pGraph);
					}
					else if(tElement.tagName()=="group")
					{
						AnalyseGroupFile(tElement);
					}
				}
			}
		}
	}
	std::string szPathBase = QFileInfo(pszProjectName).path().toStdString();
	
	szPathBase += "/graph/";
	
	for (auto iter:m_arrFiles)
	{
		iter->szFilePath = szPathBase + iter->szFileName +".pic";
	}
	

	return true;
}

void CVmcs::LogMsg(const char* logMsg, int nLevel)
{
	::LogMsg("VMCS",logMsg,nLevel,nullptr);
}

void CVmcs::AnalyseGroupFile(QDomElement pGroupNode)
{
	Q_ASSERT(!pGroupNode.isNull());
	if (pGroupNode.isNull())
	{
		return;
	}
	if (pGroupNode.hasChildNodes())
	{
		QDomNodeList nList = pGroupNode.childNodes();
		for (int i = 0; i < nList.count(); i++)
		{
			QDomNode nNode = nList.at(i);

			QDomElement tElement = nNode.toElement();

			if (tElement.tagName() == "file")
			{
				auto pGraph = std::make_shared<GraphFileDataUnit>();
				pGraph->szFileName = nNode.toElement().attribute("name").toStdString();
				m_arrFiles.push_back(pGraph);
			}
			else if (tElement.tagName()=="group")
			{
				AnalyseGroupFile(tElement);
			}
		}
	}
}


std::shared_ptr<CVmcs> s_Vmcs;

extern "C"
{
	// 启动
	bool StartHMI(const char * pszProjectName)
	{
		Q_ASSERT(pszProjectName &&strlen(pszProjectName) > 0);
		if (!pszProjectName || strlen(pszProjectName) == 0)
		{
			return false;
		}
		if (s_Vmcs)
		{
			s_Vmcs.reset();
		}
		s_Vmcs = std::make_shared<CVmcs>();

		if (!s_Vmcs->Initalize(pszProjectName))
		{
			return false;
		}
		return true;
	}
	// 关闭
	bool StopHMI(const char * pszProjectName)
	{
		Q_ASSERT(pszProjectName &&strlen(pszProjectName) > 0);
		if (!pszProjectName || strlen(pszProjectName) == 0)
		{
			return false;
		}

		return true;
	}
}
