/*! @file shape_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       : shape_widget.h
文件实现功能 : 基本形状图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基本形状图元
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#ifndef D_SHAPE_WIDGET_H
#define D_SHAPE_WIDGET_H
 
#include "base_widget.h"
  
class CShapeWidget : public CBaseWidget
{
public:
	CShapeWidget();
	
	virtual ~CShapeWidget();
public:
	enum SHAPE
	{
		RECTANGLE = 0,        //!< 矩形
		ELLIPSE,              //!< 椭圆
		ROUNDRECT,            //!< 圆角矩形
		TRAPEZOID,            //!< 梯形
		PARALLELOGRAM,        //!< 平行四边形
		DIAMOND,              //!< 菱形
		TRAPEZOID_MIRROR,
		PARALLELOGRAM_MIRROR,
		ROUND,                //!< 标准圆形
		TRIANGLE,             //!< 三角形
		ERR_SHAPE
	};
	CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape);
public:	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	virtual void DrawSingleSelectedTracker(QPainter *painter);
	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPaint, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);
	
protected:
 
private:
	//! 形状
	SHAPE  m_Shape;

};


#endif // D_SHAPE_WIDGET_H

/** @}*/

