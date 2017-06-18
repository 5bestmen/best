/*! @file group_widget.h
<PRE>
********************************************************************************
模块名       :
文件名       :  group_widget.h
文件实现功能 :  组合图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   组合图元
*  @author  LiJin
*  @version 1.0
*  @date    2016.02.3
*******************************************************************************/

#ifndef D_GROUP_WIDGET_H
#define D_GROUP_WIDGET_H
 
#include "base_widget.h"
#include <vector>
  

/*! \class CGroupWidget  group_widget.h  "group_widget.h"
*  \brief 组合图元. 
*/
class CGroupWidget : public CBaseWidget
{
	Q_OBJECT
public:
	CGroupWidget();	
	virtual ~CGroupWidget();
public:
	struct GRP_MEMBER
	{
		CBaseWidget *WidgetPtr;     //! 图元
		QRectF       RelativePos;   //! 相对位置
	};
public:	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	virtual void MoveHandleTo(int nHandle, const QPointF &ptInput);
// 	virtual void DrawSingleSelectedTracker(QPainter *painter);
// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPaint, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);	 

	// 移动到新的位置
	virtual void MoveTo(const QRectF& position);
	// 移动到新的位置
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);
    //
    // 选中状态时，画外部的选择框
//     virtual void DrawSingleSelectedTracker(QPainter *pPainter);


public:
	size_t GetWidgetCount()
	{
		return m_arrWidgets.size();
	}
	CBaseWidget* GetWidget(unsigned int nIdx)
	{
		if (nIdx < m_arrWidgets.size())
		{
			return m_arrWidgets[nIdx].WidgetPtr;
		}
		return nullptr;
	}
	bool IsEmpty()
	{
		return m_arrWidgets.empty();
	}
	// 添加一个图元到组中
	void AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat);
	// 清除掉所有图元，但是并不回收资源
	void Clear()
	{
		m_arrWidgets.clear();
	}
protected:
 
private:
	std::vector <GRP_MEMBER> m_arrWidgets;   //! 子图元
};


#endif // D_SHAPE_WIDGET_H

/** @}*/

