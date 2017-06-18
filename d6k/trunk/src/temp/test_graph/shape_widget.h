/*! @file shape_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : shape_widget.h
�ļ�ʵ�ֹ��� : ������״ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ������״ͼԪ
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
		RECTANGLE = 0,        //!< ����
		ELLIPSE,              //!< ��Բ
		ROUNDRECT,            //!< Բ�Ǿ���
		TRAPEZOID,            //!< ����
		PARALLELOGRAM,        //!< ƽ���ı���
		DIAMOND,              //!< ����
		TRAPEZOID_MIRROR,
		PARALLELOGRAM_MIRROR,
		ROUND,                //!< ��׼Բ��
		TRIANGLE,             //!< ������
		ERR_SHAPE
	};
	CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape);
public:	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	virtual void DrawSingleSelectedTracker(QPainter *painter);
	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPaint, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);
	virtual void GetMovingPath(QPainterPath& path);
	
protected:
 
private:
	//! ��״
	SHAPE  m_Shape;

};


#endif // D_SHAPE_WIDGET_H

/** @}*/

