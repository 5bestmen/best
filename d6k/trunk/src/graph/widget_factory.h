/*! @file widget_factory.h
<PRE>
********************************************************************************
模块名       :
文件名       :  widget_factory.h
文件实现功能 :  图元工厂类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  抽象工厂设计模式，为今后的预留接口
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
#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H   
 
 
enum DRAW_TOOLS
{
	DRAW_TOOLS_SEL = 0,
	DRAW_TOOLS_LINE,   //!< 画线
	DRAW_TOOLS_ARC,    //!< 弧
	DRAW_TOOLS_POLYLINE,  //!< 折线
	DRAW_TOOLS_TRIANGLE, //!< 三角形
	DRAW_TOOLS_RECTANGLE, //!< 矩形
	DRAW_TOOLS_ELLIPSE,   //!< 椭圆
	DRAW_TOOLS_ROUND,   //!< 圆
	DRAW_TOOLS_ROUNDRECT, //!< 圆角矩形
	DRAW_TOOLS_TRAPEZOID,//!< 梯形
	DRAW_TOOLS_PARALLELOGRAM, //!< 平行四边形
	DRAW_TOOLS_DIAMOND,       //!< 菱形
	DRAW_TOOLS_POLYGON,       //!< 多边形
	DRAW_TOOLS_CHORD,         //!< 弓形
	DRAW_TOOLS_GROUPS,        //组合
	DRAW_TOOLS_CAMBER,         //弧形
	DRAW_TOOLS_PIE,           //!< 扇形
	DRAW_TOOLS_DONUT,         //! 环形 
	DRAW_TOOLS_TEXT,          //文本
	DRAW_TOOLS_BUTTON,
	DRAW_TOOLS_PIPE,           //管道
	DRAW_TOOLS_TABLE,       //!< 网格
	DRAW_TOOLS_PROGRESSBAR, //!< 棒图
	DRAW_TOOLS_METER,       //!< 仪表
	DRAW_TOOLS_RTCURVE,     //!< 实时曲线

	DRAW_TOOLS_DATETIME,     //!< 日期时间

	DRAW_TOOLS_HTREPORT,
	DRAW_TOOLS_RTREPORT,
	DRAW_TOOLS_HTCURVE,

	DRAW_TOOLS_HTALARM,
	DRAW_TOOLS_RTALARM,
	DRAW_TOOLS_SYMBOL,

	DRAW_TOOLS_TEMPLATE,  //图形库
	DRAW_TOOLS_TEMPLATE_FOLDER,  //图形库目录

	//按钮
	DRAW_TOOLS_BTN_NORMAL,        //!< 一般
	DRAW_TOOLS_BTN_3D1,           //3d按钮1
	DRAW_TOOLS_BTN_3D2,
	DRAW_TOOLS_BTN_3D3,
	DRAW_TOOLS_BTN_3D4,
	DRAW_TOOLS_BTN_EMGRCY_STOP_A,              //!< 急停A
	DRAW_TOOLS_BTN_EMGRCY_STOP_B,            //!< 急停B
	DRAW_TOOLS_BTN_EMGRCY_STOP_C,            //!< 急停C
	DRAW_TOOLS_BTN_YELLOW_LIGHT,        //!< 黄灯
	DRAW_TOOLS_BTN_BLUE_LIGHT,              //!< 蓝灯
	DRAW_TOOLS_BTN_GREEN_LIGHT,     //绿灯
	DRAW_TOOLS_BTN_RED_NIGHT, //红灯
	DRAW_TOOLS_BTN_YELLOW_LED,                //!< 黄LED
	DRAW_TOOLS_BTN_BLUE_LED,             //!< 蓝LED
	DRAW_TOOLS_BTN_GREEN_LED,               //绿LED
	DRAW_TOOLS_BTN_RED_LED,                //红LED

	//三方控件
	DRAW_ADDR_CURE_PLOT,   //曲线图
};

class CBaseWidget;

class CWidgetFactory
{
public:
	CWidgetFactory();
	~CWidgetFactory();
public: 
	static CBaseWidget * CreateWidget(const QRectF & rcPos,unsigned int nWidgetType);

//	static void DestroySerializer(CGraphSerializer *pSerializer);

};



#endif // WIDGET_FACTORY_H

/** @}*/

