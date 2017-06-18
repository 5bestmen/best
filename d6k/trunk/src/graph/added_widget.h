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
		//基本曲线图
		NORMAL_CURE_PLOT = 0,
	};

public:

	CAddedWidget(QRectF rcPos, CAddedWidget::ADD_TYPE shape);
public:
// 	void AddWidget(QWidget *pWgt);


public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// 旋转的中心点 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
	// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);

public:
	//获取曲线对象
	CCurvePlotWgt *GetPlotWgt()
	{
		return m_pPlotWgt;
	}
	//获取边界宽度
	int GetInterval()
	{
		return m_nInterval;
	}

	//设置边界宽度
	void SetInterval(int nVal)
	{
		m_nInterval = nVal;
	}

private:
	ADD_TYPE m_Shape;
	//代理
	//QGraphicsProxyWidget *m_pProxyWgt;
	//
	CCurvePlotWgt *m_pPlotWgt;
	//边界宽度  间隔
	int m_nInterval;

};
