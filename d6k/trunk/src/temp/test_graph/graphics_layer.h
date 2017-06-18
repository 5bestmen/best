/*! @file graphics_layer.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graphics_layer.h
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
*  \brief ͼ��.
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
	// ����ͼ��ĳߴ�
	void SetGraphSize(const QSize &sizeGraph);

	// ���һ������
	void AddWidget(CBaseWidget *pObj);
	// ɾ��һ������
	bool EraseWidget(CBaseWidget *pObj);
	// ��������Դ
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
	// ����ͼԪ��ѡ���
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
	// ��ѡ
	void SelectMultiWidgets(CBaseWidget *pWidget);

	bool DeleteSelectedWidgets();
	bool ReSelectWidget(CBaseWidget *pWidget);

	std::vector <CBaseWidget *> GetSelectedWidgets()
	{
		return m_arrSelection;
	}
	// �жϵ�ǰͼԪ�����ڶ�ѡ״̬�����ǵ�ѡ
	unsigned int IsSingleSelection();

	bool IsCurrentLayer();
	// �ǲ��ǵ�һ����ѡ�е�
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
	// ����ѡ����צ�ӵľ���λ��
	QRectF GetSelectedBoxHandleRect(const QRectF & rcInput,int nHandle)const;

	QRectF BuildRelativeLocation(const QRectF & rcUnion, const QRectF & rcWidget)const;

	void BuildTempGroupWidget();

	bool ChangeZStepValue(CBaseWidget *pWidget,int nStepLevel);
public:
	//! ���½���
	std::function< void(CBaseWidget* pWidget)> m_fnInvalidWidget;
	std::function< void(const QRectF &) > m_fnInvalidRect;

protected:
	//! ��ǰͼ���ѡ��ͼԪ
	std::vector <CBaseWidget *> m_arrSelection;
	//! ѡ�е�ͼԪ
	std::shared_ptr<CGroupWidget> m_pSelectedTempGrpWidget;
	//! ��ǰ�������ͼԪ
	CBaseWidget *m_pInsertObj;

	CGraphScene *m_pScene;
private:
	//! �༭ʱ�Ƿ�ɼ�
	bool     m_bIsVisible;
	QSizeF   m_Size;
	std::vector <CBaseWidget*> m_arrWidgets;
	//! ͼ����ţ���1��ʼ
	unsigned int   m_nIdx;
	//! Z��˳��
	double         m_dblMaxZValue;
};
 

 


#endif // D_GRAPH_LAYER_H

/** @}*/

