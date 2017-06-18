/*! @file shape_widget.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  shape_widget.cpp
�ļ�ʵ�ֹ��� :  ������״ͼԪ
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
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

#include "shape_widget.h" 
#include "colour_define.h"
#include "graphics_layer.h"
#include "graph_module.h"

#include <QPainter>
#include <QDebug>
#include <QApplication>

CShapeWidget::CShapeWidget() :CBaseWidget()
{
	m_nObjType = WIDGET_RECT;
}

CShapeWidget::~CShapeWidget()
{

}

CShapeWidget::CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape) :CBaseWidget(rcPos)
{
	m_nObjType = WIDGET_RECT;
	m_Shape = shape;

	m_eRotateCenter = ROTATE_MIDCENTER;

	switch (shape)
	{
    case CShapeWidget::RECTANGLE:
    {
        //����
        m_szName = "RECT";
        m_nObjType = DRAW_TOOLS_RECTANGLE;
        //	m_szTxt = QStringLiteral("���Ǿ���");
        break;
    }
    case CShapeWidget::ELLIPSE:
    {
        //��Բ
		m_szName = "Ellipse";
        m_nObjType = DRAW_TOOLS_ELLIPSE;
        break;
    }
    case CShapeWidget::ROUNDRECT:
    {
        //Բ�Ǿ���
		m_szName = "RoundRect";
        m_nObjType = DRAW_TOOLS_ROUNDRECT;
        break;
    }
    case CShapeWidget::TRAPEZOID:
    {
        //����
		m_szName = "Trapezoid";
        m_nObjType = DRAW_TOOLS_TRAPEZOID;
        break;
    }
    case CShapeWidget::PARALLELOGRAM:
    {
        //ƽ���ı���
		m_szName = "Parallelogram";
        m_nObjType = DRAW_TOOLS_PARALLELOGRAM;
        break;
    }
    case CShapeWidget::DIAMOND:
    {
        //����
		m_szName = "Diamond";
        m_nObjType = DRAW_TOOLS_DIAMOND;
        break;
    }
    case CShapeWidget::TRIANGLE:
    {
        //������
		m_szName = "Triangle";
        m_nObjType = DRAW_TOOLS_TRIANGLE;
        break;
    }
	case CShapeWidget::CAMBER:
	{
		//����
		m_szName = "Camber";
		m_nObjType = DRAW_TOOLS_CAMBER;
		break;
	}
	case CShapeWidget::CHORD:
	{
		//����
		m_szName = "Chord";
		m_nObjType = DRAW_TOOLS_CHORD;
		break;
	}
	case CShapeWidget::PIE:
	{
		//����
		m_szName = "Pie";
		m_nObjType = DRAW_TOOLS_PIE;
		break;
	}
    case CShapeWidget::TRAPEZOID_MIRROR:
    {

        break;
    }
    case CShapeWidget::PARALLELOGRAM_MIRROR:
    {
        break;
    }
    case CShapeWidget::ROUND:
    {
        m_szName = "ROUND";
        //m_szTxt = QStringLiteral("����Բ��");
        m_nObjType = DRAW_TOOLS_ROUND;
        break;
    }
    case CShapeWidget::ERR_SHAPE:
    {
        break;
    }
	default:
		break;
	}
//	setToolTip(QStringLiteral("���Ǿ��Σ�����..."));
}

QRectF CShapeWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();
// 	double dPenWidth = m_PenInfo.GetWidth() / 2;
// 
// 	if (dPenWidth > 0.0f)
// 	{ // �������߿�
// 		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
// 	}
	return rcBounding;
}

void CShapeWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	pPainter->save();
	
// 	if (m_Shape == RECTANGLE)
// 	{
// 		if (static_cast<int>(m_dblRotateAngle) % 90 != 0)
// 		{
// 			pPainter->setRenderHint(QPainter::Antialiasing);
// 		}
// 	}
// 	else
// 	{
// 		pPainter->setRenderHint(QPainter::Antialiasing);
// 	}

	if (static_cast<int>(m_dblRotateAngle) % 90 != 0 || CShapeWidget::RECTANGLE != m_Shape)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}
    

	QPen pen;
	m_PenInfo.ToQPen(pen);
	pPainter->setPen(pen);

	QPointF ptCenter = GetRotationCenterPoint();

	if (m_dblRotateAngle != 0.0f)
	{
// 		pPainter->translate(ptCenter);
// 		pPainter->rotate(m_dblRotateAngle);
// 		pPainter->translate(-ptCenter);
	}
	
	QColor clBrush(XGraphics::LightBlue);
	clBrush.setAlpha(255);
	QBrush brBack;
	brBack.setColor(clBrush);
	brBack.setStyle(Qt::SolidPattern);


	QColor clBrush2(XGraphics::LightCoral);
	clBrush2.setAlpha(255);
	
// 	CBrushInfo info;
// 	info.SetStyle(Qt::SolidPattern);
// 	info.SetColor(clBrush2);

	

	if (m_Shape == RECTANGLE)
	{
	}
	else
	{
		clBrush.setRgb(XGraphics::YellowGreen);
		clBrush.setAlpha(255);
		brBack.setColor(clBrush);
	}
//	pPainter->fillRect(m_rcPos, brBack);
//	pPainter->drawRect(m_rcPos);
	QPainterPath path;
	GetPath(path);

//	QPainterPath p;
//	p.addRect(m_rcPos);
    //��䱳����ɫ
	CBrushInfo* pBrush = GetBrushInfo();
	FillPath(pPainter, path,*pBrush);
//	pPainter->fillRect(m_rcPos, brBack);
	pPainter->drawPath(path);

	//m_szTxt = QString::fromLocal8Bit(m_szWidgetName.c_str());
	if (m_szTxt.length())
	{
		//pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		QTextOption txtOpt;
		SetFontLayout(txtOpt);
		
		//txtOpt.
		pen.setColor(m_FontInfo.GetColor());
		pPainter->setPen(pen);
		pPainter->setFont(m_FontInfo.GetFont());
		pPainter->setRenderHint(QPainter::TextAntialiasing);

		if (m_dblRotateAngle != 0.0f)
		{
			QPointF ptCenter = GetRotationCenterPoint();

			pPainter->translate(ptCenter);
			pPainter->rotate(m_dblRotateAngle);
			pPainter->translate(-ptCenter.x(), -ptCenter.y());
	    }

		pPainter->drawText(m_rcPos, m_szTxt,txtOpt);
#if 0
		QString szVal = QString("Z=%1").arg(this->zValue());
		QRectF rcTxt = m_rcPos;
		rcTxt.setY(m_rcPos.y() + 16);
		pPainter->drawText(rcTxt, szVal, txtOpt);
#endif
	}
	pPainter->restore();


	/////////////////////////
	pPainter->save();

	QPixmap tImage;
#ifdef GRAPH_LIB
	//����״̬
	tImage = GetModuleApi()->GetGraphicPath() + GetImageInfo()->GetBackgroundImage();
#else
	tImage = GetImageInfo()->GetBackgroundImage();
#endif // GRAPH_LIB
	//GetImageInfo()->SetImageSize(tImage.size());

	if (!tImage.isNull())
	{
		if (GetImageInfo()->GetImageSize().width() == 0 || GetImageInfo()->GetImageSize().height() == 0)
		{
			//ͼƬ��ʵ�ʳߴ�
			GetImageInfo()->SetImageSize(tImage.size());
		}
		else
		{
			tImage = tImage.scaled(GetImageInfo()->GetImageSize());
		}

		if (GetImageInfo()->GetStretch())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			//����
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage.scaled(m_rcPos.size().toSize()));
			//m_szImage = rect.size().toSize();

		}
		else if (GetImageInfo()->GetbTiling())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			//ƽ��
			pPainter->drawTiledPixmap(m_rcPos, tImage);
			//m_szImage = rect.size().toSize();
		}
		else if (tImage.size().width() > m_rcPos.size().width() || tImage.size().height() > m_rcPos.size().height())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage.scaled(m_rcPos.size().toSize()));
			//m_szImage = rect.size().toSize();
		}
		else
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage);
			//m_szImage = tImage.size();

		}
	}

	pPainter->restore();
	/////////////////////////
	/*if (!GetPixMap().isNull())
	{
		pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
		pPainter->rotate(GetRotateAngle());
		pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
		pPainter->drawPixmap(boundingRect().toRect(), GetPixMap());	
	}*/
	// ��ѡ���
	DrawSelectedTracker(pPainter);
}

void CShapeWidget::SetFontLayout(QTextOption &txtOpt)
{
	if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_CENTER)
	{
		//����
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_TOP)
	{
		//��
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_BOTTOM)
	{
		//��
		txtOpt.setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_LEFT)
	{
		//��
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_RIGHT)
	{
		//��
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignRight);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_LEFT)
	{
		//��  - ��
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_RIGHT)
	{
		//��  - ��
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	}
}

/*! \fn void CShapeWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
********************************************************************************************************* 
** \brief CShapeWidget::DrawMovingWidget 
** \details ���ƶ��е�ͼԪ
** \param pPainter 
** \param bSingleSelected 
** \return void 
** \author LiJin
** \date 2016��2��11�� 
** \note ��ѡ״̬���ⲿѡ���Ϊ����ɫ���������Ƕ�ѡ״̬ԭ����ɶ��������ɶ��
********************************************************************************************************/
void CShapeWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
{ 
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return; 

	pPainter->save();
	
	if (bSingleSelected)
	{
		QPen pen(XGraphics::LightBlue);
		pen.setWidthF(m_PenInfo.GetWidth());
		pen.setStyle(Qt::DashLine);

		pPainter->setPen(pen);
	}
	else
	{// ��ѡ���ƶ�(�������ͼԪ����ͼԪ)��ԭ����ɶ��������ɶ��
		QPen pen(Qt::darkGray);
		pen.setWidth(1);
		pen.setStyle(Qt::SolidLine);
		pPainter->setPen(pen);
	}	

	QColor clBrush(XGraphics::LightBlue);
    clBrush.setAlpha(240);
	QBrush brBack;
	brBack.setColor(clBrush);
	brBack.setStyle(Qt::SolidPattern);

    //��ת������
    if (GetRotateAngle() == 0.0f)
    {

    }
    else
    {
        pPainter->setRenderHint(QPainter::Antialiasing);

        QPointF tCenter = GetRotationCenterPoint();
        
        pPainter->translate(GetMovingRotationCenterPoint());
        pPainter->rotate(GetRotateAngle());
        pPainter->translate(-GetMovingRotationCenterPoint());


    }

    
	pPainter->fillRect(m_rcMovingPos, brBack);
    

    pPainter->restore();

    pPainter->save();
	QPainterPath path;
	GetMovingPath(path);

//     pPainter->fillPath(path, brBack);

	if (GetRotateAngle() != 0.0f && m_Shape != RECTANGLE)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}
	
	pPainter->drawPath(path);
	pPainter->restore();
   
}
/*! \fn void CShapeWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
*********************************************************************************************************
** \brief CShapeWidget::MoveHandleTo
** \details  �ƶ�ĳ��צ�ӵ��µ�λ�ã�����ͼԪ�Ĵ�С
** \param nHandle
** \param point
** \return void
** \author LiJin
** \date 2016��2��15��
** \note
********************************************************************************************************/
void CShapeWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
{
	QRectF rcLoc = m_rcPos;
	QPointF ptTemp = ptInput;

	switch (nHandle)
	{
	case POS_TOP_LEFT:
		//  �ƶ����Ͻǲ��ܳ����ұ߼��±�
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopLeft(ptTemp);

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
         if (m_Shape == ROUND)
         {
             if (rcLoc.width() > rcLoc.height())
             {
                 rcLoc.setBottom(rcLoc.top() + rcLoc.width());
             }
             else
             {
                 rcLoc.setRight(rcLoc.left() + rcLoc.height());
             }
         }

		break;
	case POS_TOP_CENTER:
		//  �ƶ��ϱ߲��ܳ����±�
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTop(ptTemp.y());

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            rcLoc.setRight(rcLoc.left() + rcLoc.height());
        }

		break;
	case POS_TOP_RIGHT:
		//  �ƶ����Ͻ� ���ܳ������½�
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopRight(ptTemp);

         //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
         if (m_Shape == ROUND)
         {
             if (rcLoc.width() > rcLoc.height())
             {
                 rcLoc.setBottom(rcLoc.top() + rcLoc.width());
             }
             else
             {
                 rcLoc.setRight(rcLoc.left() + rcLoc.height());
             }
         }

		break;
	case POS_RIGHT_CENTER:
		// �ƶ��ұ� ���ܳ��� ���
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setRight(ptTemp.x());

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            rcLoc.setBottom(rcLoc.top() + rcLoc.width());
        }
		break;
	case POS_BOTTOM_RIGHT:
		// �ƶ����½� ���ܳ������Ͻ�
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setBottomRight(ptTemp);

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            if (rcLoc.width() > rcLoc.height())
            {
                rcLoc.setBottom(rcLoc.top() + rcLoc.width());
            }
            else
            {
                rcLoc.setRight(rcLoc.left() + rcLoc.height());
            }
        }
		break;

	case POS_BOTTOM_CENTER:
		// �ƶ��±� ���ܳ����ϱ�
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottom(ptTemp.y());
        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            rcLoc.setRight(rcLoc.left() + rcLoc.height());
        }
        
		break;
	case POS_BOTTOM_LEFT:
		// �ƶ����½� ���ܳ������Ͻ�
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottomLeft(ptTemp);

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            if (rcLoc.width() > rcLoc.height())
            {
                rcLoc.setBottom(rcLoc.top() + rcLoc.width());
            }
            else
            {
                rcLoc.setRight(rcLoc.left() + rcLoc.height());
            }
        }
		break;
	case POS_LEFT_CENTER:
		// �ƶ���� ���ܳ����ұ�
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setLeft(ptTemp.x());

        //��Ϊ��׼Բ��ʱ  Ҫͬʱ���ƿ�Ⱥ͸߶���ͬ
        if (m_Shape == ROUND)
        {
            rcLoc.setBottom(rcLoc.top() + rcLoc.width());
        }
		break;
	}
	rcLoc = rcLoc.normalized();

	MoveTo(rcLoc);
}


void  CShapeWidget::GetMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	switch (m_Shape)
	{
    case CShapeWidget::RECTANGLE:
    {
        //path.addPath(shape());
        GetRectangleMovingPath(path);
        break;
    }
    case CShapeWidget::ELLIPSE:
    {
        //��Բ
        GetEllipseMovingPath(path);
        break;
    }
    case CShapeWidget::ROUNDRECT:
    {
        //Բ�Ǿ���
        GetMovingRoundRectPath(path);
        break;
    }
    case CShapeWidget::TRAPEZOID:
    {
        //����
        GetMovingTrapezoidPath(path);
        break;
    }
    case CShapeWidget::PARALLELOGRAM:
    {
        //ƽ���ı���
        GetMovingParallelogarmPath(path);
        break;
    }
    case CShapeWidget::DIAMOND:
    {
        //����
        GetMovingDiamondPath(path);
        break;
    }
	case CShapeWidget::TRAPEZOID_MIRROR:
		break;
	case CShapeWidget::PARALLELOGRAM_MIRROR:
		break;
    case CShapeWidget::ROUND:
    {
        //Բ��
        rcShape.setHeight(qMin(rcShape.width(), rcShape.height()));
        rcShape.setWidth(qMin(rcShape.width(), rcShape.height()));
        path.addEllipse(rcShape);
        break;
    }
    case CShapeWidget::TRIANGLE:
    {
        //������
        GetMovingTriglePath(path);
        break;
    }
	case CShapeWidget::CAMBER:
	{
		//����
		GetCamberMovingPath(path);
		break;
	}
	case CShapeWidget::CHORD:
	{
		//����
		GetChordMovingPath(path);
		break;
	}
	case CShapeWidget::PIE:
	{
		//����
		GetPieMovingPath(path);
		break;
	}
	case CShapeWidget::ERR_SHAPE:
		break;
	default:
		break;
	}
}

void CShapeWidget::GetPath(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();

	switch (m_Shape)
	{
    case CShapeWidget::RECTANGLE:
    {
        GetRectanglePath(path);
//         path.addPath(shape());
        break;
    }
	case CShapeWidget::ELLIPSE:
    {
        //��Բ
        GetEllipsePath(path);
        break;
    }
    case CShapeWidget::ROUNDRECT:
    {
        //Բ�Ǿ���
        GetRoundRectPath(path);
        break;
    }
    case CShapeWidget::TRAPEZOID:
    {
        //����
        GetTrapeZoidPath(path);
        break;
    }
    case CShapeWidget::PARALLELOGRAM:
    {
        //ƽ���ı���
        GetParallelogarm(path);
        break;
    }
    case CShapeWidget::DIAMOND:
    {
        //����
        GetDiamondPath(path);
        break;
    }
	case CShapeWidget::TRAPEZOID_MIRROR:
		break;
	case CShapeWidget::PARALLELOGRAM_MIRROR:
		break;
    case CShapeWidget::ROUND:
    {
        rcShape.setHeight(qMin(rcShape.width(), rcShape.height()));
        rcShape.setWidth(qMin(rcShape.width(), rcShape.height()));
        path.addEllipse(rcShape);
        break;
    }
    case CShapeWidget::TRIANGLE:
    {
        //������
        GetTrianglePath(path);
        break;
    }
	case CShapeWidget::CAMBER:
	{
		//����
		GetCamberPath(path);
		break;
	}
	case CShapeWidget::CHORD:
	{
		//����
		GetChordPath(path);
		break;
	}
	case CShapeWidget::PIE:
	{
		//����
		GetPiePath(path);
		break;
	}
	case CShapeWidget::ERR_SHAPE:
		break;
	default:
		break;
	}
}

/*! \fn QPointF  CShapeWidget::GetEllipsePath(QPainterPath& path)
*********************************************************************************************************
** \brief CShapeWidget::GetEllipsePath(QPainterPath& path)
** \details ��ȡ��Բpath
** \return QPointF
** \author LiJin
** \date 2016��3��15��
** \note
********************************************************************************************************/
void CShapeWidget::GetEllipsePath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.addEllipse(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addEllipse(shape().boundingRect());
    }
}

void CShapeWidget::GetEllipseMovingPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();

        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.addEllipse(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addEllipse(rcShape);
    }
}

//����
void CShapeWidget::GetCamberMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

		p1.moveTo(rcShape.topRight().x(), rcShape.center().y());

		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.topRight().x(), rcShape.center().y());

		path.arcTo(rcShape,m_nStartAngle,m_nEndAngle);
	}
}

void CShapeWidget::GetCamberPath(QPainterPath & path)
{
	QRectF rcShape = m_rcPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.moveTo(rcShape.topRight().x(), rcShape.center().y());
		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.topRight().x(),rcShape.center().y());
		path.arcTo(shape().boundingRect(), m_nStartAngle, m_nEndAngle);
	}
}

void CShapeWidget::GetChordMovingPath(QPainterPath & path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

		p1.moveTo(rcShape.topRight().x(), rcShape.center().y());
		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		p1.lineTo(rcShape.topRight().x(), rcShape.center().y());
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.topRight().x(), rcShape.center().y());
		path.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		path.lineTo(rcShape.topRight().x(), rcShape.center().y());
	}
}

void CShapeWidget::GetChordPath(QPainterPath & path)
{
	QRectF rcShape = m_rcPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.moveTo(rcShape.topRight().x(), rcShape.center().y());
		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		p1.lineTo(rcShape.topRight().x(), rcShape.center().y());
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.topRight().x(), rcShape.center().y());
		path.arcTo(shape().boundingRect(), m_nStartAngle, m_nEndAngle);
		path.lineTo(rcShape.topRight().x(), rcShape.center().y());
	}
}

void CShapeWidget::GetPieMovingPath(QPainterPath & path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

		p1.moveTo(rcShape.center());
		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		p1.lineTo(rcShape.center());
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.center());
		path.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		path.lineTo(rcShape.center());
	}
}

void CShapeWidget::GetPiePath(QPainterPath & path)
{
	QRectF rcShape = m_rcPos.normalized();

	QPainterPath p1;
	QRectF rcInput = boundingRect();

	if (m_dblRotateAngle != 0.0f)
	{
		QPointF ptCenter = GetRotationCenterPoint();
		QTransform tr_temp;
		tr_temp.reset();
		tr_temp.translate(ptCenter.x(), ptCenter.y());
		tr_temp = tr_temp.rotate(m_dblRotateAngle);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);
		tr_temp.translate(-ptCenter.x(), -ptCenter.y());

		p1.moveTo(rcShape.center());
		p1.arcTo(rcShape, m_nStartAngle, m_nEndAngle);
		p1.lineTo(rcShape.center());
		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.center());
		path.arcTo(shape().boundingRect(), m_nStartAngle, m_nEndAngle);
		path.lineTo(rcShape.center());
	}
}

void CShapeWidget::GetRoundRectPath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.addRoundRect(rcShape,20,20);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRoundRect(rcShape, 20, 20);
    }
}

void CShapeWidget::GetMovingRoundRectPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.addRoundRect(rcShape, 20, 20);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRoundRect(rcShape, 20, 20);
    }
}

//��ȡ����path
void CShapeWidget::GetRectanglePath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.addRect(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRect(rcInput);
    }
}

//��ȡ�����ƶ�path
void CShapeWidget::GetRectangleMovingPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.addRect(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRect(rcShape);
    }
}

//��ȡ����path
void CShapeWidget::GetDiamondPath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.moveTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
        p1.lineTo((rcShape.topLeft().x() + rcShape.topRight().x()) / 2,rcShape.topLeft().y());

        p1.lineTo(rcShape.topRight().x(), (rcShape.topRight().y() + rcShape.bottomRight().y()) / 2);

        p1.lineTo((rcShape.bottomLeft().x() + rcShape.bottomRight().x()) / 2, rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
        path.lineTo((rcShape.topLeft().x() + rcShape.topRight().x()) / 2, rcShape.topLeft().y());

        path.lineTo(rcShape.topRight().x(), (rcShape.topRight().y() + rcShape.bottomRight().y()) / 2);

        path.lineTo((rcShape.bottomLeft().x() + rcShape.bottomRight().x()) / 2, rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
    }
}
//��ȡ�ƶ��е�����path
void CShapeWidget::GetMovingDiamondPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.moveTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
        p1.lineTo((rcShape.topLeft().x() + rcShape.topRight().x()) / 2, rcShape.topLeft().y());

        p1.lineTo(rcShape.topRight().x(), (rcShape.topRight().y() + rcShape.bottomRight().y()) / 2);

        p1.lineTo((rcShape.bottomLeft().x() + rcShape.bottomRight().x()) / 2, rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
        path.lineTo((rcShape.topLeft().x() + rcShape.topRight().x()) / 2, rcShape.topLeft().y());

        path.lineTo(rcShape.topRight().x(), (rcShape.topRight().y() + rcShape.bottomRight().y()) / 2);

        path.lineTo((rcShape.bottomLeft().x() + rcShape.bottomRight().x()) / 2, rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x(), (rcShape.topLeft().y() + rcShape.bottomLeft().y()) / 2);
    }
}

//��ȡ������path
void CShapeWidget::GetTrianglePath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.moveTo(rcShape.center().x(), rcShape.topLeft().y());
        p1.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.center().x(), rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.center().x(), rcShape.topLeft().y());
        path.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.center().x(), rcShape.topLeft().y());
    }
}

//��ȡ�ƶ��е�������path
void CShapeWidget::GetMovingTriglePath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.moveTo(rcShape.center().x(), rcShape.topLeft().y());
        p1.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.center().x(), rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.center().x(), rcShape.topLeft().y());
        path.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.center().x(), rcShape.topLeft().y());
    }
}

//��ȡ����path
void CShapeWidget::GetTrapeZoidPath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        p1.lineTo(rcShape.topRight().x() - rcShape.width() / 8.0, rcShape.topRight().y());

        p1.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        path.lineTo(rcShape.topRight().x() - rcShape.width() / 8.0, rcShape.topRight().y());

        path.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
    }
}
//��ȡ�ƶ��е�����path
void CShapeWidget::GetMovingTrapezoidPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        p1.lineTo(rcShape.topRight().x() - rcShape.width() / 8.0, rcShape.topRight().y());

        p1.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        path.lineTo(rcShape.topRight().x() - rcShape.width() / 8.0, rcShape.topRight().y());

        path.lineTo(rcShape.bottomRight().x(), rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
    }
}

//��ȡƽ���ı���path
void CShapeWidget::GetParallelogarm(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        p1.lineTo(rcShape.topRight().x(), rcShape.topRight().y());

        p1.lineTo(rcShape.bottomRight().x() - rcShape.width() / 8.0, rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        path.lineTo(rcShape.topRight().x(), rcShape.topRight().y());

        path.lineTo(rcShape.bottomRight().x() - rcShape.width() / 8.0, rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
    }
}
//��ȡ�ƶ��е�ƽ���ı���path
void CShapeWidget::GetMovingParallelogarmPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

        p1.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        p1.lineTo(rcShape.topRight().x(), rcShape.topRight().y());

        p1.lineTo(rcShape.bottomRight().x() - rcShape.width() / 8.0, rcShape.bottomRight().y());

        p1.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        p1.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());

        path = tr_temp.map(p1);
    }
    else
    {
        path.moveTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
        path.lineTo(rcShape.topRight().x(), rcShape.topRight().y());

        path.lineTo(rcShape.bottomRight().x() - rcShape.width() / 8.0, rcShape.bottomRight().y());

        path.lineTo(rcShape.bottomLeft().x(), rcShape.bottomLeft().y());

        path.lineTo(rcShape.topLeft().x() + rcShape.width() / 8.0, rcShape.topLeft().y());
    }
}


/*! \fn QPointF  CShapeWidget::GetRoTationCenterPoint()const
********************************************************************************************************* 
** \brief CShapeWidget::GetRoTationCenterPoint 
** \details ��ת�����ĵ�
** \return QPointF 
** \author LiJin 
** \date 2016��3��15�� 
** \note 
********************************************************************************************************/
QPointF  CShapeWidget::GetRotationCenterPoint()const
{
	QPointF ptCenter;

	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//���Ͻ�
		ptCenter = m_rcPos.topLeft();
	}
	else if (m_eRotateCenter == ROTATE_TOPMID)
	{
		//����
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.topLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//����
		ptCenter = m_rcPos.topRight();
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//����
		ptCenter = QPointF(m_rcPos.topLeft().x(), m_rcPos.center().y());;
	}
	else if (m_eRotateCenter == ROTATE_MIDCENTER)
	{
		//����
		ptCenter = m_rcPos.center();
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//����
		ptCenter = QPointF(m_rcPos.topRight().x(), m_rcPos.center().y());
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//���½�
		ptCenter = m_rcPos.bottomLeft();
	}
	else if (m_eRotateCenter == ROTATE_BOTTOMMID)
	{
		//����
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.bottomLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//����
		ptCenter = QPointF(m_rcPos.bottomRight());
	}

	return ptCenter;
}

/** @}*/

