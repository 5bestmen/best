/*! @file shape_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       :  shape_widget.h
文件实现功能 :  基本形状图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本形状图元
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#ifndef D_SHAPE_WIDGET_H
#define D_SHAPE_WIDGET_H
 
#include <QTextOption>
#include "base_widget.h"

#include "widget_factory.h"

  
/*! \class CShapeWidget  shape_widget.h  "shape_widget.h"
*  \brief 形状基类信息.（矩形、圆形...）
*/
class CShapeWidget : public CBaseWidget
{
	Q_OBJECT
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
		CAMBER,               //弧形
		CHORD,                //弓形
		PIE,                  //扇形
		ERR_SHAPE
	};


	CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape);
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


    //获取矩形path
    void GetRectanglePath(QPainterPath& path);
    //获取矩形移动path
    void GetRectangleMovingPath(QPainterPath& path);
    //获取椭圆path
    void GetEllipsePath(QPainterPath& path);
    //获取椭圆移动path
    void GetEllipseMovingPath(QPainterPath& path);
	//获取弧形的移动path
	void GetCamberMovingPath(QPainterPath& path);
	//获取弧形path
	void GetCamberPath(QPainterPath &path);
	//获取弓形移动path
	void GetChordMovingPath(QPainterPath &path);
	//获取弓形path
	void GetChordPath(QPainterPath &path);

	//获取扇形移动path
	void GetPieMovingPath(QPainterPath &path);
	//获取扇形path
	void GetPiePath(QPainterPath &path);

    //获取圆角矩形path
    void GetRoundRectPath(QPainterPath& path);
    //获取移动中的圆角矩形 path
    void GetMovingRoundRectPath(QPainterPath& path);

    //获取菱形path
    void GetDiamondPath(QPainterPath& path);
    //获取移动中的菱形path
    void GetMovingDiamondPath(QPainterPath& path);

    //获取三角形path
    void GetTrianglePath(QPainterPath& path);
    //获取移动中的三角形path
    void GetMovingTriglePath(QPainterPath& path);

    //获取梯形path
    void GetTrapeZoidPath(QPainterPath& path);
    //获取移动中的梯形path
    void GetMovingTrapezoidPath(QPainterPath& path);

    //获取平行四边形path
    void GetParallelogarm(QPainterPath& path);
    //获取移动中的平行四边形path
    void GetMovingParallelogarmPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);	
	//设置字体位置
	void SetFontLayout(QTextOption &txtOpt);


protected:
 
private:
	//! 形状类型
	SHAPE  m_Shape;

};


#endif // D_SHAPE_WIDGET_H

/** @}*/

