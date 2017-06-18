#pragma once

#include "base_widget.h"

class QMouseEvent;
class CCurvePlotWgt;

class CAddedWidget : public CBaseWidget
{
	Q_OBJECT

public:
	CAddedWidget();
	virtual ~CAddedWidget();
public:
	enum ADD_TYPE
	{
		//��������ͼ
		NORMAL_CURE_PLOT = 0,
	};

public:

	CAddedWidget(QRectF rcPos, CAddedWidget::ADD_TYPE shape);
public:
// 	void AddWidget(QWidget *pWgt);


public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// ��ת�����ĵ� 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
	// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);

public:
	//��ȡ���߶���
	CCurvePlotWgt *GetPlotWgt()
	{
		return m_pPlotWgt;
	}
	//��ȡ�߽���
	int GetInterval()
	{
		return m_nInterval;
	}

	//���ñ߽���
	void SetInterval(int nVal)
	{
		m_nInterval = nVal;
	}

private:
	ADD_TYPE m_Shape;
	//����
	//QGraphicsProxyWidget *m_pProxyWgt;
	//
	CCurvePlotWgt *m_pPlotWgt;
	//�߽���  ���
	int m_nInterval;

};
