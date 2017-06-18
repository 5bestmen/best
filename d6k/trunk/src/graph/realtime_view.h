/*! @file graphview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graphview.h
�ļ�ʵ�ֹ��� :  ͼ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼ��
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#ifndef REALTIME_GRAPH_VIEW_H
#define REALTIME_GRAPH_VIEW_H
 
#include <QGraphicsView>
#include <vector> 
#include <QMap>

class CGraphFile;
class CBaseWidget;
class CPenInfo;
class CBrushInfo;
class CFontInfo;
class CGraphScene;

class CRealTimeView : public QGraphicsView
{
    Q_OBJECT
public:
	CRealTimeView(QWidget *parent = 0);
	CRealTimeView(QGraphicsScene *scene, QWidget * parent = 0);

	~CRealTimeView();
public:
	void timerEvent(QTimerEvent *event);

protected:
 
protected:
	
	virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void paintEvent(QPaintEvent *event);
   	 
	//���س���
	CGraphScene *GetGraphicsScene() const; 

private:
	int m_nTimerID;
};

#endif // REALTIME_GRAPH_VIEW_H

/** @}*/

