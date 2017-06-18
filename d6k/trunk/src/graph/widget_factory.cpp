/*! @file widget_factory.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  widget_factory.cpp
文件实现功能 :  图元工厂类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图元工厂类
*  @author  LiJin
*  @version 1.0
*  @date    2016.08.30
*******************************************************************************/

#include "base_widget.h"
#include "shape_widget.h"
#include "group_widget.h"
#include "line_widget.h"
#include "widget_factory.h"
#include "text_widget.h"
#include "pushbtn_widget.h"
#include "added_widget.h"
 
#include <QDebug>


CWidgetFactory::CWidgetFactory()
{

}

CWidgetFactory::~CWidgetFactory()
{

}

CBaseWidget * CWidgetFactory::CreateWidget(const QRectF & rcPos, unsigned int nWidgetType)
{	 
	CBaseWidget *pWidget = nullptr;
	switch (nWidgetType)
	{
	case DRAW_TOOLS_LINE:
	{
        //直线
		pWidget = new CLineWidget(rcPos, CLineWidget::BEE_LINE);
		break;
	}
	case DRAW_TOOLS_POLYLINE:
	{
		//折线
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::POLY_LINE);
		break;
	}
	case DRAW_TOOLS_PIPE:
	{
		//管道
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::PIPE);
		break;
	}
	case DRAW_TOOLS_POLYGON:
	{
		//多边形
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::POLYGONO);
		break;
	}
	case  DRAW_TOOLS_RECTANGLE:
	{
        //矩形
		pWidget = new CShapeWidget(rcPos, CShapeWidget::RECTANGLE);
	}
	break;
	case DRAW_TOOLS_ROUND:
	{
        //圆形
		pWidget = new CShapeWidget(rcPos, CShapeWidget::ROUND);
        break;
	}
    case DRAW_TOOLS_ELLIPSE:
    {
        //椭圆形状
        pWidget = new CShapeWidget(rcPos, CShapeWidget::ELLIPSE);
        break;
    }
    case DRAW_TOOLS_ROUNDRECT:
    {
        //圆角矩形
        pWidget = new CShapeWidget(rcPos, CShapeWidget::ROUNDRECT);
        break;
    }
    case DRAW_TOOLS_DIAMOND:
    {
        //菱形
        pWidget = new CShapeWidget(rcPos, CShapeWidget::DIAMOND);
        break;

    }
    case DRAW_TOOLS_TRAPEZOID:
    {
        //梯形
        pWidget = new CShapeWidget(rcPos, CShapeWidget::TRAPEZOID);
        break;

    }
    case DRAW_TOOLS_PARALLELOGRAM:
    {
        //平行四边形
        pWidget = new CShapeWidget(rcPos, CShapeWidget::PARALLELOGRAM);
        break;

    }
    case DRAW_TOOLS_TRIANGLE:
    {
        //三角形
        pWidget = new CShapeWidget(rcPos, CShapeWidget::TRIANGLE);
        break;

    }
	case DRAW_TOOLS_CAMBER:
	{
		//弧形
		pWidget = new CShapeWidget(rcPos,CShapeWidget::CAMBER);
		break;
	}
	case DRAW_TOOLS_CHORD:
	{
		//弓形
		pWidget = new CShapeWidget(rcPos, CShapeWidget::CHORD);
		break;
	}
	case DRAW_TOOLS_PIE:
	{
		//扇形
		pWidget = new CShapeWidget(rcPos, CShapeWidget::PIE);
		break;
	}
    case DRAW_TOOLS_GROUPS:
    {
        //组合图元
        pWidget = new CGroupWidget;
        break;
    }
    case DRAW_TOOLS_TEXT:
    {
        //文本
        pWidget = new CTextWidget(rcPos);;
        break;
    }
	case DRAW_TOOLS_BTN_NORMAL:
	{
		//基本按钮
		pWidget = new CPushBtnWidget(rcPos, CPushBtnWidget::NORMAL);
		break;
	}
	case DRAW_TOOLS_BTN_3D1:
	{
		//3d1
		pWidget = new CPushBtnWidget(rcPos, CPushBtnWidget::BTN_3D1);
		break;
	}
	case DRAW_TOOLS_BTN_3D2:
	{
		//3d2
		pWidget = new CPushBtnWidget(rcPos, CPushBtnWidget::BTN_3D2);
		break;
	}
	case DRAW_TOOLS_BTN_3D3:
	{
		//3d3
		pWidget = new CPushBtnWidget(rcPos, CPushBtnWidget::BTN_3D3);
		break;
	}
	case DRAW_TOOLS_BTN_3D4:
	{
		//3d3
		pWidget = new CPushBtnWidget(rcPos, CPushBtnWidget::BTN_3D4);
		break;
	}
	case DRAW_ADDR_CURE_PLOT:
	{
		//曲线图
		pWidget = new CAddedWidget(rcPos, CAddedWidget::NORMAL_CURE_PLOT);
		break;
	}
	default:
		break;
	}
	return pWidget;
}
 
/** @}*/

