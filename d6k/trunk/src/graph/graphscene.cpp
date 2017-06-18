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
#include "graphscene.h" 
#include "graph_file.h"

#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QDebug>


CGraphScene::CGraphScene(CGraphFile *pFile, QObject *parent) :
m_pGraphFile(pFile),
QGraphicsScene(parent)
{
	Q_ASSERT(pFile);
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
	Q_ASSERT(m_pGraphFile!=nullptr);
	if (m_pGraphFile == nullptr)
		return false;
	return m_pGraphFile->SaveFile(szFileName);
}
//����ͼ���ļ�
bool CGraphScene::LoadFile(const QString &szFileName)
{
	Q_UNUSED(szFileName);
	return true;
}

/** @}*/

