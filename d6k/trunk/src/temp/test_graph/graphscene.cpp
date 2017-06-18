/*! @file graphscene.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  graphscene.cpp
文件实现功能 :  场景
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  场景
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QDebug>
#include "graphscene.h" 
#include "graph_file.h"


CGraphScene::CGraphScene(QObject *parent) :
QGraphicsScene(parent)
{
	
}

CGraphScene::~CGraphScene()
{ 

}

std::shared_ptr<CGraphFile> CGraphScene::GetGraphFile()
{
	return m_pGraphFile;
}

void  CGraphScene::NewFile()
{
	m_pGraphFile = std::make_shared<CGraphFile>(this);

	this->setSceneRect(m_pGraphFile->GetBackgroundRect());
} 
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
	if (szFileName.isEmpty())
		return false;
	
	QString szNewFileName = szFileName;
	QFileInfo nInfo(szNewFileName);
	if (nInfo.suffix() != "pic")
	{
		szNewFileName += ".pic";
	}
	
	QFile file(szNewFileName);

	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		qDebug() << "Error: Cannot write file: " << qPrintable(file.errorString());
		return false;
	}
	
	std::shared_ptr<QXmlStreamWriter> pXmlWriter = std::make_shared<QXmlStreamWriter>(&file);
// 	QList<QGraphicsItem *> items = this->items();
// 	writeGraphics(xmlWriter, items);
	file.close();
	if (file.error())
	{
		qDebug() << "Error: Cannot write file: "<< qPrintable(file.errorString());
		return false;
	}
// 	views().first()->setWindowTitle(fileName);
// 	m_fileName = fileName;
// 	m_sceneDirty = FALSE;
	return true;
}
//加载图形文件
bool CGraphScene::LoadFile(const QString &szFileName)
{
	return true;
}

/** @}*/

