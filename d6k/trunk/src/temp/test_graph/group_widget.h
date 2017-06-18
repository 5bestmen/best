/*! @file group_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : group_widget.h
�ļ�ʵ�ֹ��� : ���ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ���ͼԪ
*  @author LiJin
*  @version 1.0
*  @date    2016.02.3
*******************************************************************************/

#ifndef D_GROUP_WIDGET_H
#define D_GROUP_WIDGET_H
 
#include "base_widget.h"
#include <vector>
  
class CGroupWidget : public CBaseWidget
{
public:
	CGroupWidget();
	
	virtual ~CGroupWidget();
public:	 

public:
	struct GRP_MEMBER
	{
		CBaseWidget *WidgetPtr;     //! ͼԪ
		QRectF       RelativePos;   //! ���λ��
	};
public:	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	virtual void MoveHandleTo(int nHandle, const QPointF &ptInput);
	virtual void DrawSingleSelectedTracker(QPainter *painter);
	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPaint, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);	 

	// �ƶ����µ�λ��
	virtual void MoveTo(const QRectF& position);
	// �ƶ����µ�λ��
	virtual void MoveDelta(const QPointF &delta);
	virtual void MovingDelta(const QPointF &delta);

public:
	unsigned int GetWidgetCount()
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
	// ���һ��ͼԪ������
	void AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat);
	// ���������ͼԪ�����ǲ���������Դ
	void Clear()
	{
		m_arrWidgets.clear();
	}
protected:
 
private:
	std::vector <GRP_MEMBER> m_arrWidgets;   //! ��ͼԪ
};


#endif // D_SHAPE_WIDGET_H

/** @}*/

