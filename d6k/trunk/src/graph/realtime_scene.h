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
#ifndef GRAPH_SCENE_H
#define GRAPH_SCENE_H
 
#include <QGraphicsScene>
#include <memory>

class CGraphFile;
class CGraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
	CGraphScene(CGraphFile *pFile,QObject *parent = 0);
	~CGraphScene();
public:
	CGraphFile* GetGraphFile()
	{
		return m_pGraphFile;
	} 
	//保存图形文件
	bool SaveFile(const QString &szFileName);
	//加载图形文件
	bool LoadFile(const QString &szFileName);

public:
	void timerEvent(QTimerEvent *event);
	int m_nTimerID;
private:
	CGraphFile* m_pGraphFile;
};

#endif // GRAPH_SCENE_H

/** @}*/
