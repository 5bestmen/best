/*! @file graphscene.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  graphscene.cpp
文件实现功能 :  场景管理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   场景管理
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
** \details 保存图形文件
** \param szFileName
** \return bool
** \author LiJin
** \date 2016年2月19日
** \note
********************************************************************************************************/
bool CGraphScene::SaveFile(const QString &szFileName)
{
	Q_ASSERT(m_pGraphFile!=nullptr);
	if (m_pGraphFile == nullptr)
		return false;
	return m_pGraphFile->SaveFile(szFileName);
}
//加载图形文件
bool CGraphScene::LoadFile(const QString &szFileName)
{
	Q_UNUSED(szFileName);
	return true;
}

/** @}*/

