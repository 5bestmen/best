/*! @file graphview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graphview.h
�ļ�ʵ�ֹ��� : ͼ��
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ͼ��
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
	DRAW_TOOLS_LINE,   //!< ����
	DRAW_TOOLS_ARC,    //!< ��
	DRAW_TOOLS_POLYLINE,  //!< ����
	DRAW_TOOLS_TRIANGLE, //!< ������
	DRAW_TOOLS_RECTANGLE, //!< ����
	DRAW_TOOLS_ELLIPSE,   //!< ��Բ
	DRAW_TOOLS_ROUND,   //!< Բ
	DRAW_TOOLS_ROUNDRECT, //!< Բ�Ǿ���
	DRAW_TOOLS_TRAPEZOID,//!< ����
	DRAW_TOOLS_PARALLELOGRAM, //!< ƽ���ı���
	DRAW_TOOLS_DIAMOND,       //!< ����
	DRAW_TOOLS_POLYGON,       //!< �����
	DRAW_TOOLS_CHORD,         //!< ����
	DRAW_TOOLS_PIE,           //!< ����
	DRAW_TOOLS_DONUT,         //! ���� 
	DRAW_TOOLS_TEXT,
	DRAW_TOOLS_BUTTON,
	DRAW_TOOLS_PIPE,
	DRAW_TOOLS_TABLE,       //!< ����
	DRAW_TOOLS_PROGRESSBAR, //!< ��ͼ
	DRAW_TOOLS_METER,       //!< �Ǳ�
	DRAW_TOOLS_RTCURVE,     //!< ʵʱ����

	DRAW_TOOLS_DATETIME,     //!< ����ʱ��

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
	// ��ȡ�����ͼԪ�ϵ�צ�ӵ�λ��
	int GetMouseOnHandleIdx(const QPointF & pointf);
protected:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void mouseMoveEvent(QMouseEvent * e);
	virtual void mousePressEvent(QMouseEvent * e);
	virtual void mouseReleaseEvent(QMouseEvent * e);
	virtual void mouseDoubleClickEvent(QMouseEvent * e);
	virtual void contextMenuEvent(QContextMenuEvent *event);

private:
	//���س���
	CGraphScene *GetGraphicsScene() const;

	QPointF m_ptFisrst, m_ptLast;

	int				m_nDragHandle;

	unsigned int m_nCurEditingLayerIndex; //! ��ǰ���ڱ༭��ͼ��
	//! ��ѡ���
	bool m_bDrawSelRect;

	//! ѡ���
	QRectF m_rcSelectBox;
	QRectF m_rcOldSelBox;
};

#endif // GRAPH_VIEW_H

/** @}*/

