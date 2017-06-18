/*! @file shape_widget.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  shape_widget.h
�ļ�ʵ�ֹ��� :  ������״ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������״ͼԪ
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
*  \brief ��״������Ϣ.�����Ρ�Բ��...��
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
		CAMBER,               //����
		CHORD,                //����
		PIE,                  //����
		ERR_SHAPE
	};


	CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape);
public:	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// ��ת�����ĵ� 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// �ƶ��е�ͼԪ
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);


    //��ȡ����path
    void GetRectanglePath(QPainterPath& path);
    //��ȡ�����ƶ�path
    void GetRectangleMovingPath(QPainterPath& path);
    //��ȡ��Բpath
    void GetEllipsePath(QPainterPath& path);
    //��ȡ��Բ�ƶ�path
    void GetEllipseMovingPath(QPainterPath& path);
	//��ȡ���ε��ƶ�path
	void GetCamberMovingPath(QPainterPath& path);
	//��ȡ����path
	void GetCamberPath(QPainterPath &path);
	//��ȡ�����ƶ�path
	void GetChordMovingPath(QPainterPath &path);
	//��ȡ����path
	void GetChordPath(QPainterPath &path);

	//��ȡ�����ƶ�path
	void GetPieMovingPath(QPainterPath &path);
	//��ȡ����path
	void GetPiePath(QPainterPath &path);

    //��ȡԲ�Ǿ���path
    void GetRoundRectPath(QPainterPath& path);
    //��ȡ�ƶ��е�Բ�Ǿ��� path
    void GetMovingRoundRectPath(QPainterPath& path);

    //��ȡ����path
    void GetDiamondPath(QPainterPath& path);
    //��ȡ�ƶ��е�����path
    void GetMovingDiamondPath(QPainterPath& path);

    //��ȡ������path
    void GetTrianglePath(QPainterPath& path);
    //��ȡ�ƶ��е�������path
    void GetMovingTriglePath(QPainterPath& path);

    //��ȡ����path
    void GetTrapeZoidPath(QPainterPath& path);
    //��ȡ�ƶ��е�����path
    void GetMovingTrapezoidPath(QPainterPath& path);

    //��ȡƽ���ı���path
    void GetParallelogarm(QPainterPath& path);
    //��ȡ�ƶ��е�ƽ���ı���path
    void GetMovingParallelogarmPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);	
	//��������λ��
	void SetFontLayout(QTextOption &txtOpt);


protected:
 
private:
	//! ��״����
	SHAPE  m_Shape;

};


#endif // D_SHAPE_WIDGET_H

/** @}*/

