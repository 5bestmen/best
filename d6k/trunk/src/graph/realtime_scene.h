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
	//����ͼ���ļ�
	bool SaveFile(const QString &szFileName);
	//����ͼ���ļ�
	bool LoadFile(const QString &szFileName);

public:
	void timerEvent(QTimerEvent *event);
	int m_nTimerID;
private:
	CGraphFile* m_pGraphFile;
};

#endif // GRAPH_SCENE_H

/** @}*/
