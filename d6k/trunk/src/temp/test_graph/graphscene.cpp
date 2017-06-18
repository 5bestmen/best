/*! @file graphscene.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphscene.cpp
�ļ�ʵ�ֹ��� :  ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ����
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
** \details ����ͼ���ļ�
** \param szFileName 
** \return bool 
** \author LiJin 
** \date 2016��2��19�� 
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
//����ͼ���ļ�
bool CGraphScene::LoadFile(const QString &szFileName)
{
	return true;
}

/** @}*/

