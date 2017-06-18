/*! @file graphics_layer.h
<PRE>
********************************************************************************
模块名       :
文件名       : graphics_layer.h
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
#ifndef D_GRAPH_LAYER_H
#define D_GRAPH_LAYER_H
 
#include "base_widget.h"

#include <QSize>
#include <QPainter>

#include <vector>
#include <string>
#include <memory>
#include <functional>


class CGraphScene;

/*! \class CGraphicsLayer  graphics_layer.h  "graphics_layer.h"
*  \brief 图层.
*/
class CGraphicsLayer 
{
public:
	CGraphicsLayer();
	CGraphicsLayer(unsigned int nLayerIndex, CGraphScene *pScene);

	virtual ~CGraphicsLayer();
public:
	enum SELECTION_STATE
	{
		NONE_SELECTED,
		SINGLE_SELECTED,
		MULTI_SELECTED,
	};
public:
	// 设置图层的尺寸
	void SetGraphSize(const QSize &sizeGraph);

	// 添加一个对象
	void AddWidget(CBaseWidget *pObj);
	// 删除一个对象
	bool EraseWidget(CBaseWidget *pObj);
	// 不回收资源
	bool RemoveWidget(CBaseWidget *pObj);

	std::vector <CBaseWidget*> &GetWidgets();

	void SetVisible(bool bDsp);

	bool IsVisible()const
	{
		return m_bIsVisible;
	}

	bool IsSelectionEmpty()
	{
		return m_arrSelection.empty();
	}

	void SetLayerIdx(unsigned int nIdx);

	unsigned int GetLayerIdx()
	{
		return m_nIdx;
	}
	// 画多图元的选择框
	void DrawSelectionFrame(QPainter *pPainter);

	void DrawMovingWidgets(QPainter *pPainter);

	bool SelectAll( );
	void Select(CBaseWidget* pObj);
	void Deselect(CBaseWidget* pObj);
	void SelectWidgetsInRect(const QRectF &rcSel);

	bool ResizeSelectionWidgets(int nDragHanle,  QPointF & ptInput);

	bool IsSelectedWidgetsCanMove(const QPointF & delta,bool bMoving = true) const;
	bool CanMoveWidget(CBaseWidget *pWidget,const QPointF & newPos) const;
	bool CanMoveWidgetDelta(CBaseWidget *pWidget, const QPointF & delta)const;

	bool MoveWidgets(const QPointF & delta);
	bool MovingWidgets(const QPointF & delta);

	bool GroupWidgets();
	bool BreakGroupedWidgets();

	void SetCurrentWidget(CBaseWidget *pWidget);
	CBaseWidget* GetCurrentWidget()const
	{
		return m_pInsertObj;
	}
	// 多选
	void SelectMultiWidgets(CBaseWidget *pWidget);

	bool DeleteSelectedWidgets();
	bool ReSelectWidget(CBaseWidget *pWidget);

	std::vector <CBaseWidget *> GetSelectedWidgets()
	{
		return m_arrSelection;
	}
	// 判断当前图元是属于多选状态？还是单选
	unsigned int IsSingleSelection();

	bool IsCurrentLayer();
	// 是不是第一个被选中的
	bool IsFirstSelectedWidget(CBaseWidget *pWidget);

	std::pair <int, CBaseWidget *> HitTestSingleSelectedWidgetHandle(const QPointF & point) const;

	int HitTestMultiSelectedWidgetHandle(const QPointF &ptTemp)const;

	bool OnAutoAlign();
	bool OnLeftAlign();
	bool OnRightAlign();
	bool OnCenterHorzAlign();
	bool OnTopAlign();
	bool OnCenterVertAlign();
	bool OnButtomAlign();
	
	bool MoveUpSelectedWidgets();
	bool MoveDownSelectedWidgets();
	bool MoveSelectedWidgetsToTopLevel();
	bool MoveSelectedWidgetsToBottomLevel();

	bool MoveWidgetToTopLevel( );
	bool MoveWidgetToBottomLevel( );

protected:
	bool MoveSelectedWidgetToTopLevel(CBaseWidget *pWidget);
	bool MoveSelectedWidgetToBottomLevel(CBaseWidget *pWidget);
	void InvalidateWidget(CBaseWidget* pBlock);
	void InvalidateRect(const QRectF & rcClient);
	void InvalidateLayer();
	// 计算选择框的爪子的矩形位置
	QRectF GetSelectedBoxHandleRect(const QRectF & rcInput,int nHandle)const;

	QRectF BuildRelativeLocation(const QRectF & rcUnion, const QRectF & rcWidget)const;

	void BuildTempGroupWidget();

	bool ChangeZStepValue(CBaseWidget *pWidget,int nStepLevel);
public:
	//! 更新界面
	std::function< void(CBaseWidget* pWidget)> m_fnInvalidWidget;
	std::function< void(const QRectF &) > m_fnInvalidRect;

protected:
	//! 当前图层的选中图元
	std::vector <CBaseWidget *> m_arrSelection;
	//! 选中的图元
	std::shared_ptr<CGroupWidget> m_pSelectedTempGrpWidget;
	//! 当前插入的新图元
	CBaseWidget *m_pInsertObj;

	CGraphScene *m_pScene;
private:
	//! 编辑时是否可见
	bool     m_bIsVisible;
	QSizeF   m_Size;
	std::vector <CBaseWidget*> m_arrWidgets;
	//! 图层序号，从1开始
	unsigned int   m_nIdx;
	//! Z轴顺序
	double         m_dblMaxZValue;
};
 

 


#endif // D_GRAPH_LAYER_H

/** @}*/

