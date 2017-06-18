/*! @file graphscene.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphscene.cpp
�ļ�ʵ�ֹ��� :  ��������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��������
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

//#include "graphscene.h" 
#include "graph_file.h"
#include "realtime_scene.h"
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTimerEvent>

CGraphScene::CGraphScene(CGraphFile *pFile, QObject *parent) :m_pGraphFile(pFile),
QGraphicsScene(parent)
{
	Q_ASSERT(pFile);
	//m_nTimerID = startTimer(1000);
}

CGraphScene::~CGraphScene()
{ 

}

#if 0
void  CGraphScene::NewFile()
{
	m_pGraphFile = std::make_shared<CGraphFile>(this);

	this->setSceneRect(m_pGraphFile->GetBackgroundRect());
}
#endif

/*! \fn bool CGraphScene::SaveFile(const QString &szFileName)
********************************************************************************************************* 
** \brief CGraphScene::SaveFile 
** \details ����ͼ���ļ�
** \param szFileName
** \return bool
** \author LiJin
** \date 2016��2��19��
** \note
********************************************************************************************************/
bool CGraphScene::SaveFile(const QString &szFileName)
{
	Q_ASSERT(!szFileName.isEmpty() && szFileName.length() > 0);
	if (szFileName.isEmpty() || szFileName.length() == 0)
	{
		return false;
	}
	Q_ASSERT(m_pGraphFile!=nullptr);
	if (m_pGraphFile == nullptr)
		return false;
	return m_pGraphFile->SaveFile(szFileName);
}

bool CGraphScene::LoadFile(const QString &szFileName)
{
	Q_ASSERT(!szFileName.isEmpty() && szFileName.length()>0);
	if (szFileName.isEmpty() || szFileName.length()==0)
	{
		return false;
	}
	Q_ASSERT(m_pGraphFile);
	if (m_pGraphFile==nullptr)
	{
		return false;
	}
	return m_pGraphFile->LoadFile(szFileName);
}

void CGraphScene::timerEvent(QTimerEvent *event)
{
	if (event)
	{
		if (event->timerId()==m_nTimerID)
		{
			update();
		}
	}
}

/** @}*/

