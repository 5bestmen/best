/*! @file widget_factory.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  widget_factory.cpp
�ļ�ʵ�ֹ��� :  ͼԪ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ͼԪ������
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
        //ֱ��
		pWidget = new CLineWidget(rcPos, CLineWidget::BEE_LINE);
		break;
	}
	case DRAW_TOOLS_POLYLINE:
	{
		//����
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::POLY_LINE);
		break;
	}
	case DRAW_TOOLS_PIPE:
	{
		//�ܵ�
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::PIPE);
		break;
	}
	case DRAW_TOOLS_POLYGON:
	{
		//�����
		pWidget = new CLineWidget(QRectF(rcPos.topLeft().x(), rcPos.topLeft().y(), 2, 2), CLineWidget::POLYGONO);
		break;
	}
	case  DRAW_TOOLS_RECTANGLE:
	{
        //����
		pWidget = new CShapeWidget(rcPos, CShapeWidget::RECTANGLE);
	}
	break;
	case DRAW_TOOLS_ROUND:
	{
        //Բ��
		pWidget = new CShapeWidget(rcPos, CShapeWidget::ROUND);
        break;
	}
    case DRAW_TOOLS_ELLIPSE:
    {
        //��Բ��״
        pWidget = new CShapeWidget(rcPos, CShapeWidget::ELLIPSE);
        break;
    }
    case DRAW_TOOLS_ROUNDRECT:
    {
        //Բ�Ǿ���
        pWidget = new CShapeWidget(rcPos, CShapeWidget::ROUNDRECT);
        break;
    }
    case DRAW_TOOLS_DIAMOND:
    {
        //����
        pWidget = new CShapeWidget(rcPos, CShapeWidget::DIAMOND);
        break;

    }
    case DRAW_TOOLS_TRAPEZOID:
    {
        //����
        pWidget = new CShapeWidget(rcPos, CShapeWidget::TRAPEZOID);
        break;

    }
    case DRAW_TOOLS_PARALLELOGRAM:
    {
        //ƽ���ı���
        pWidget = new CShapeWidget(rcPos, CShapeWidget::PARALLELOGRAM);
        break;

    }
    case DRAW_TOOLS_TRIANGLE:
    {
        //������
        pWidget = new CShapeWidget(rcPos, CShapeWidget::TRIANGLE);
        break;

    }
	case DRAW_TOOLS_CAMBER:
	{
		//����
		pWidget = new CShapeWidget(rcPos,CShapeWidget::CAMBER);
		break;
	}
	case DRAW_TOOLS_CHORD:
	{
		//����
		pWidget = new CShapeWidget(rcPos, CShapeWidget::CHORD);
		break;
	}
	case DRAW_TOOLS_PIE:
	{
		//����
		pWidget = new CShapeWidget(rcPos, CShapeWidget::PIE);
		break;
	}
    case DRAW_TOOLS_GROUPS:
    {
        //���ͼԪ
        pWidget = new CGroupWidget;
        break;
    }
    case DRAW_TOOLS_TEXT:
    {
        //�ı�
        pWidget = new CTextWidget(rcPos);;
        break;
    }
	case DRAW_TOOLS_BTN_NORMAL:
	{
		//������ť
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
		//����ͼ
		pWidget = new CAddedWidget(rcPos, CAddedWidget::NORMAL_CURE_PLOT);
		break;
	}
	default:
		break;
	}
	return pWidget;
}
 
/** @}*/

