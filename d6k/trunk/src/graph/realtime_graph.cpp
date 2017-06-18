
#include "realtime_graph.h"
#include "graph_file.h"

#include <QDebug>


CRealTimeTask::CRealTimeTask()
{
	m_pGraphFile = nullptr;
}

CRealTimeTask::~CRealTimeTask()
{

}
CRealTimeTask::CRealTimeTask(std::shared_ptr<CGraphFile>& pFile):m_pGraphFile(pFile)
{
	Q_ASSERT(pFile);

}

void CRealTimeTask::MainLoop(void *pImpl) 
{
/*
	Q_ASSERT(pImpl);
	if (pImpl == nullptr)
		return;
*/

//	CGraphFile *pFile = reinterpret_cast<CGraphFile*>(pImpl);

	if (m_pGraphFile == nullptr)
		return;

	std::vector < CBaseWidget* >::iterator it_widget;
	unsigned int i = 0;

	for (i = 0; i < CGraphFile::MAX_LAYER_CNT; i++)
	{
		it_widget = m_pGraphFile->m_arrLayers[i]->GetWidgets().begin();
		for (; it_widget != m_pGraphFile->m_arrLayers[i]->GetWidgets().end(); ++it_widget)
		{
			(*it_widget)->ProcessDynamic();
		}
	}
}

bool CRealTimeTask::Initialize(std::shared_ptr<CGraphFile>& pFile,  unsigned int nMode)
{
	Q_ASSERT(pFile);
	if (pFile == nullptr)
		return false;

	m_pGraphFile = pFile;

	std::vector < CBaseWidget* >::iterator it_widget;
	unsigned int i = 0;
	 
	for (i = 0; i < CGraphFile::MAX_LAYER_CNT; i++)
	{
		it_widget = pFile->m_arrLayers[i]->GetWidgets().begin();
		for (; it_widget != pFile->m_arrLayers[i]->GetWidgets().end(); ++it_widget)
		{
			(*it_widget)->PrepareDynamic();
		}
	}

	return true;
}

void CRealTimeTask::Run()
{
	StartModule();
}

void CRealTimeTask::Shutdown()
{
	StopModule();

	/*if (m_pGraphFile)
	{
		std::vector < CBaseWidget* >::iterator it_widget;
		unsigned int i = 0;
	
		for (i = 0; i < CGraphFile::MAX_LAYER_CNT; i++)
		{
			it_widget = m_pGraphFile->m_arrLayers[i]->GetWidgets().begin();
			for (; it_widget != m_pGraphFile->m_arrLayers[i]->GetWidgets().end(); ++it_widget)
			{
				(*it_widget)->ExitDynamic();
			}
		}
	}
	*/
}