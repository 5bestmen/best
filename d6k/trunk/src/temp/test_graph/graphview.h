/*! @file graphview.h
<PRE>
********************************************************************************
模块名       :
文件名       : graphview.h
文件实现功能 : 图层
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  图层
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#ifndef GRAPH_VIEW_H
#define GRAPH_VIEW_H
 
#include <QGraphicsView>
#include <vector> 

enum WIDGET_MOUSE_MODES
{
	WIDGET_MOUSE_NOTHING,
	WIDGET_MOUSE_SELECT,
	WIDGET_MOUSE_MOVE,
//	WIDGET_MOUSE_MOVEING,
	WIDGET_MOUSE_SIZE,
	WIDGET_MOUSE_SIZING,
	WIDGET_MOUSE_ROTATE,
	WIDGET_MOUSE_ROTATING
};

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
	DRAW_TOOLS_PIE,           //!< 扇形
	DRAW_TOOLS_DONUT,         //! 环形 
	DRAW_TOOLS_TEXT,
	DRAW_TOOLS_BUTTON,
	DRAW_TOOLS_PIPE,
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
	DRAW_TOOLS_SYMBOL
};

class CGraphFile;
class CBaseWidget;

class CGraphScene;
class CGraphView : public QGraphicsView
{
    Q_OBJECT
public:
	CGraphView(QWidget *parent = 0);
	CGraphView(QGraphicsScene *scene, QWidget * parent = 0);

	~CGraphView();
public:
	unsigned int  m_nDrawTool;
	unsigned int  m_nMouseMode;
public:
	bool AddNewWidget(const QRectF & rcPos, unsigned int nWidgetType);
//	void SelectWidget(CBaseWidget* pObj);

 	void MoveWidgets(const QPointF & delta);

	void MovingWidgets(const QPointF & delta);
	
	void SelectAll(unsigned int nLayerIdx);
	void Select(CBaseWidget* pObj);
	void Deselect(CBaseWidget* pObj);
	void SelectWidgetsInRect( const QRectF &rcSel);

	void InvalidateWidget(CBaseWidget* pObj);

	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnCut();

	bool MoveWidgets(unsigned int nPixel, int nKey);

	void OnWidgetsGroup();
	void OnWidgetsBreak();
	
	void OnAutoAlign();
	void OnLeftAlign();
	void OnRightAlign();
	void OnCenterHorzAlign();
	void OnTopAlign();
	void OnCenterVertAlign();
	void OnButtomAlign();
	
	void MoveToBottomLevel();
	void MoveToTopLevel();
	void MoveUp();
	void MoveDown();

	unsigned int GetCurEditingLayerIdx()
	{
		return m_nCurEditingLayerIndex;
	}
	unsigned int  SetLayerVisible(unsigned int nLayIdx, bool bDisp);
	void SetCurEditingLayer(unsigned int nLayerIdx);
protected:
	virtual void drawBackground(QPainter *painter, const QRectF &rect);
	virtual void drawForeground(QPainter *painter, const QRectF &rect);

	void DrawRuler(QPainter *pPainter,const QRectF & rcViewPort,const QRectF & rcScene);

	void MoveSelectionWidgets(const QPoint & ptMove);
	void ResizeSelectionWidgets( QPointF & ptInput);

	void BuildSelectBox(const QRectF & rcSel);
	void BuildSelectBox(const QPointF & ptCur);
	void DrawSelectBox(QPainter *painter);

	void OnLButtonDown(QMouseEvent * e);
	void OnLButtonUp(QMouseEvent * e);

	CBaseWidget *HitTest(QPoint & pos,unsigned int nLayerIdx);
	// 获取鼠标在图元上的爪子的位置
	int GetMouseOnHandleIdx(const QPointF & pointf);
protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent(QMouseEvent * e);
	virtual void contextMenuEvent(QContextMenuEvent *event);

private:
	//返回场景
	CGraphScene *GetGraphicsScene() const;

	QPointF m_ptFisrst, m_ptLast;

	int				m_nDragHandle;

	unsigned int m_nCurEditingLayerIndex; //! 当前正在编辑的图层
	//! 画选择框
	bool m_bDrawSelRect;

	//! 选择框
	QRectF m_rcSelectBox;
	QRectF m_rcOldSelBox;
};

#endif // GRAPH_VIEW_H

/** @}*/

