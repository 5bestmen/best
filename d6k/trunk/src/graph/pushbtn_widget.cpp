#include "pushbtn_widget.h"
#include "widget_factory.h"
#include "graph_module.h"
#include "colour_define.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>

CPushBtnWidget::CPushBtnWidget()
	: CBaseWidget()
{
}

CPushBtnWidget::CPushBtnWidget(QRectF rcPos, BTN_TYPE shape):CBaseWidget(rcPos)
{
	m_nObjType = 0;
	m_Shape = shape;

	m_eRotateCenter = ROTATE_MIDCENTER;

	switch (shape)
	{
	case CPushBtnWidget::NORMAL:
	{
		//一般按钮
		m_szName = "Normal";
		m_nObjType = DRAW_TOOLS_BTN_NORMAL;

		GetPenInfo()->SetStyle(0);
		GetBrushInfo()->SetColor(QColor(240,240,240,200));
		m_rcPos.setSize(QSizeF(140,90));
		m_rcMovingPos.setSize(QSizeF(140,90));

		break;
	}
	case CPushBtnWidget::BTN_3D1:
	{
		//3D1
		m_szName = "3D1";
		GetBrushInfo()->SetColor(QColor(240, 240, 240, 200));

		GetBrushInfo()->SetEndColor(QColor(0,0,0,200));
		//top->bottom
		GetBrushInfo()->SetGradiendMode(3);

		m_rcPos.setSize(QSizeF(140, 90));
		m_rcMovingPos.setSize(QSizeF(140, 90));

		m_nObjType = DRAW_TOOLS_BTN_3D1;
		break;
	}
	case CPushBtnWidget::BTN_3D2:
	{
		//3D2
		m_szName = "3D2";
		m_nObjType = DRAW_TOOLS_BTN_3D2;

		GetPenInfo()->SetStyle(0);
		GetBrushInfo()->SetColor(QColor(240, 240, 240, 200));
		GetBrushInfo()->SetEndColor(QColor(0, 0, 0, 200));
		//top->bottom
		GetBrushInfo()->SetGradiendMode(3);

		m_rcPos.setSize(QSizeF(140, 90));
		m_rcMovingPos.setSize(QSizeF(140, 90));

		break;
	}
	case CPushBtnWidget::BTN_3D3:
	{
		//3D3
		m_szName = "3D3";
		m_nObjType = DRAW_TOOLS_BTN_3D3;

		GetBrushInfo()->SetColor(QColor(240, 240, 240, 200));

		GetBrushInfo()->SetEndColor(QColor(0, 0, 0, 200));
		//top->bottom
		GetBrushInfo()->SetGradiendMode(3);

		GetPenInfo()->SetWidth(8);

		m_rcPos.setSize(QSizeF(140, 90));
		m_rcMovingPos.setSize(QSizeF(140, 90));

		break;
	}
	case CPushBtnWidget::BTN_3D4:
	{
		//3D4
		m_szName = "3D4";
		m_nObjType = DRAW_TOOLS_BTN_3D4;

		GetBrushInfo()->SetColor(QColor(240, 240, 240, 200));

		GetBrushInfo()->SetEndColor(QColor(0, 0, 0, 200));
		//top->bottom
		GetBrushInfo()->SetGradiendMode(3);

		GetPenInfo()->SetWidth(8);

		m_rcPos.setSize(QSizeF(140, 140));
		m_rcMovingPos.setSize(QSizeF(140, 140));
		break;
	}
	case CPushBtnWidget::EMGRCY_STOP_A:
	{
		//急停A
		m_szName = "Emgrcy_Stop_A";
		m_nObjType = DRAW_TOOLS_BTN_EMGRCY_STOP_A;
		break;
	}

	case CPushBtnWidget::EMGRCY_STOP_B:
	{
		//急停B
		m_szName = "Emgrcy_stop_B";
		m_nObjType = DRAW_TOOLS_BTN_EMGRCY_STOP_B;
		break;
	}
	case CPushBtnWidget::EMGRCY_STOP_C:
	{
		//急停C
		m_szName = "Emgrcy_stop_C";
		m_nObjType = DRAW_TOOLS_BTN_EMGRCY_STOP_C;
		break;
	}
	case CPushBtnWidget::YELLOW_LIGHT:
	{
		//黄灯
		m_szName = "Yellow_Light";
		m_nObjType = DRAW_TOOLS_BTN_YELLOW_LIGHT;
		break;
	}
	case CPushBtnWidget::BLUE_LIGHT:
	{
		//蓝灯
		m_szName = "Blue_Light";
		m_nObjType = DRAW_TOOLS_BTN_BLUE_LIGHT;
		break;
	}
	case CPushBtnWidget::GREEN_LIGHT:
	{
		//绿灯
		m_szName = "Green_Light";
		m_nObjType = DRAW_TOOLS_BTN_GREEN_LIGHT;
		break;
	}
	case CPushBtnWidget::RED_NIGHT:
	{
		//红灯
		m_szName = "Red_Night";
		m_nObjType = DRAW_TOOLS_BTN_RED_NIGHT;
		break;
	}
	case CPushBtnWidget::YELLOW_LED:
	{
		//黄色LED
		m_szName = "Yellow_LED";
		m_nObjType = DRAW_TOOLS_BTN_YELLOW_LED;
		break;
	}
	case CPushBtnWidget::BLUE_LED:
	{
		//蓝色LED
		m_szName = "Blue_LED";
		m_nObjType = DRAW_TOOLS_BTN_BLUE_LED;
		break;
	}
	case CPushBtnWidget::GREEN_LED:
	{
		//绿色LED
		m_szName = "Green_LED";
		m_nObjType = DRAW_TOOLS_BTN_GREEN_LED;
		break;
	}
	case CPushBtnWidget::RED_LED:
	{
		//蓝色LED
		m_szName = "Red_LED";
		m_nObjType = DRAW_TOOLS_BTN_RED_LED;
		break;
	}

	default:
		break;
	}
}

CPushBtnWidget::~CPushBtnWidget()
{
}


void CPushBtnWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	pPainter->save();

	if (static_cast<int>(m_dblRotateAngle) != 0)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	if (static_cast<int>(m_dblRotateAngle) % 90 != 0 || CPushBtnWidget::NORMAL != m_Shape)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}


	QPen pen;
	m_PenInfo.ToQPen(pen);
	pPainter->setPen(pen);

	QPointF ptCenter = GetRotationCenterPoint();

	QPainterPath path;
	GetPath(path);

	//填充背景颜色

	CBrushInfo* pBrush = GetBrushInfo();
	pBrush->GetColor().dark(0);
	FillPath(pPainter, path, *pBrush);

	

	pPainter->drawPath(path);

	if (m_Shape == CPushBtnWidget::NORMAL)
	{
		//基本按钮
		pPainter->save();

		QPen normalPen(QColor(227,227,227,180));
		pPainter->setPen(normalPen);

		pPainter->drawPath(path);
		//
		normalPen.setColor(QColor(105, 105, 105, 180));
		normalPen.setWidth(2);
		pPainter->setPen(normalPen);

		QPainterPath addPath;
		if (m_bPressFlag)
		{
			GetNormalRressAddPath(addPath);
		}
		else
		{
			GetNormalAddPath(addPath);
		}
		
		pPainter->drawPath(addPath);

		pPainter->restore();
	}
	else if (m_Shape == CPushBtnWidget::BTN_3D3)
	{
		pPainter->save();

		pPainter->setRenderHint(QPainter::Antialiasing,false);
		QPen d3Pen(QColor(255,255,255));
		d3Pen.setWidth(1);
		pPainter->setPen(d3Pen);
		QPainterPath tPath;
		Get3D3Path(tPath);
		pPainter->drawPath(tPath);
		pPainter->restore();
	}
	else if (m_Shape == CPushBtnWidget::BTN_3D4)
	{
		pPainter->save();
		QPen d3Pen(QColor(255, 255, 255));
		d3Pen.setWidth(1);
		pPainter->setPen(d3Pen);
		QPainterPath tPath;
		Get3D4Path(tPath);
		pPainter->drawPath(tPath);
		pPainter->restore();
	}

	if (m_szTxt.length())
	{
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

		pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
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
	//配置状态
	tImage = GetModuleApi()->GetGraphicPath() + GetImageInfo()->GetBackgroundImage();
#else
	tImage = GetImageInfo()->GetBackgroundImage();
#endif // GRAPH_LIB
	//GetImageInfo()->SetImageSize(tImage.size());

	if (!tImage.isNull())
	{
		if (GetImageInfo()->GetImageSize().width() == 0 || GetImageInfo()->GetImageSize().height() == 0)
		{
			//图片的实际尺寸
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
			//拉伸
			pPainter->drawPixmap(m_rcPos.topLeft() + GetImageInfo()->GetImagePosition(), tImage.scaled(m_rcPos.size().toSize()));
			//m_szImage = rect.size().toSize();

		}
		else if (GetImageInfo()->GetbTiling())
		{
			pPainter->translate(boundingRect().center().x(), boundingRect().center().y());
			pPainter->rotate(GetRotateAngle());
			pPainter->translate(-boundingRect().center().x(), -boundingRect().center().y());
			//平铺
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
	// 画选择框
	DrawSelectedTracker(pPainter);
}


QRectF CPushBtnWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();
	// 	double dPenWidth = m_PenInfo.GetWidth() / 2;
	// 
	// 	if (dPenWidth > 0.0f)
	// 	{ // 扩大半个线宽
	// 		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
	// 	}
	return rcBounding;
}

QPointF CPushBtnWidget::GetRotationCenterPoint() const
{
	QPointF ptCenter;

	if (m_eRotateCenter == ROTATE_TOPLEFT)
	{
		//左上角
		ptCenter = m_rcPos.topLeft();
	}
	else if (m_eRotateCenter == ROTATE_TOPMID)
	{
		//中上
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.topLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_TOPRIGHT)
	{
		//右上
		ptCenter = m_rcPos.topRight();
	}
	else if (m_eRotateCenter == ROTATE_LEFTMID)
	{
		//左中
		ptCenter = QPointF(m_rcPos.topLeft().x(), m_rcPos.center().y());;
	}
	else if (m_eRotateCenter == ROTATE_MIDCENTER)
	{
		//中心
		ptCenter = m_rcPos.center();
	}
	else if (m_eRotateCenter == ROTATE_RIGHTMID)
	{
		//右中
		ptCenter = QPointF(m_rcPos.topRight().x(), m_rcPos.center().y());
	}
	else if (m_eRotateCenter == ROTATE_LEFTBOTTOM)
	{
		//左下角
		ptCenter = m_rcPos.bottomLeft();
	}
	else if (m_eRotateCenter == ROTATE_BOTTOMMID)
	{
		//下中
		ptCenter = QPointF(m_rcPos.center().x(), m_rcPos.bottomLeft().y());
	}
	else if (m_eRotateCenter == ROTATE_RIGHTBOTTOM)
	{
		//右下
		ptCenter = QPointF(m_rcPos.bottomRight());
	}

	return ptCenter;
}

/*! \fn void CShapeWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
*********************************************************************************************************
** \brief CShapeWidget::DrawMovingWidget
** \details 画移动中的图元
** \param pPainter
** \param bSingleSelected
** \return void
** \author LiJin
** \date 2016年2月11日
** \note 单选状态的外部选择框为淡兰色的虚框，如果是多选状态原来是啥样，还是啥样
********************************************************************************************************/
void CPushBtnWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
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
	{// 多选的移动(或者组合图元的子图元)，原来是啥样，还是啥样
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

	//旋转坐标轴
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

	if (GetRotateAngle() != 0.0f)
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	pPainter->drawPath(path);
	pPainter->restore();

}
/*! \fn void CShapeWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
*********************************************************************************************************
** \brief CShapeWidget::MoveHandleTo
** \details  移动某个爪子到新的位置，调整图元的大小
** \param nHandle
** \param point
** \return void
** \author LiJin
** \date 2016年2月15日
** \note
********************************************************************************************************/
void CPushBtnWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
{
	QRectF rcLoc = m_rcPos;
	QPointF ptTemp = ptInput;

	switch (nHandle)
	{
	case POS_TOP_LEFT:
		//  移动左上角不能超过右边及下边
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopLeft(ptTemp);

		break;
	case POS_TOP_CENTER:
		//  移动上边不能超过下边
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTop(ptTemp.y());

		break;
	case POS_TOP_RIGHT:
		//  移动右上角 不能超过左下角
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() > rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.bottom() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setTopRight(ptTemp);

		break;
	case POS_RIGHT_CENTER:
		// 移动右边 不能超过 左边
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setRight(ptTemp.x());

		break;
	case POS_BOTTOM_RIGHT:
		// 移动右下角 不能超过左上角
		if (ptTemp.x() < rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.left() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setBottomRight(ptTemp);

		break;

	case POS_BOTTOM_CENTER:
		// 移动下边 不能超过上边
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottom(ptTemp.y());

		break;
	case POS_BOTTOM_LEFT:
		// 移动左下角 不能超过右上角
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		if (ptTemp.y() < rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setY(rcLoc.top() + WIDGET_MULTI_SELECTED_MARGIN * 2);
		}
		rcLoc.setBottomLeft(ptTemp);
		break;
	case POS_LEFT_CENTER:
		// 移动左边 不能超过右边
		if (ptTemp.x() > rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2)
		{
			ptTemp.setX(rcLoc.right() - WIDGET_MULTI_SELECTED_MARGIN * 2);
		}

		rcLoc.setLeft(ptTemp.x());

		break;
	}
	rcLoc = rcLoc.normalized();

	MoveTo(rcLoc);
}


void  CPushBtnWidget::GetMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	switch (m_Shape)
	{
	case CPushBtnWidget::NORMAL:
	{
		//基本按钮
		GetNormalMovingPath(path);
		break;
	}
	case CPushBtnWidget::BTN_3D1:
	{
		//3d1
		Get3D1MovingPath(path);
		break;
	}
	case CPushBtnWidget::BTN_3D2:
	{
		//3d2
		Get3D2MovingPath(path);
		break;
	}
	case CPushBtnWidget::BTN_3D3:
	{
		//3d3
		Get3D3MovingPath(path);
		break;
	}
	case CPushBtnWidget::BTN_3D4:
	{
		//3d4
		Get3D4MovingPath(path);
		break;
	}
	default:
		break;
	}
}

void CPushBtnWidget::FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	auto pBrush = info.CreateQBrush();

	pPainter->save();

	if (info.GetGradiendMode() != 0)
	{
		//渐变
		QLinearGradient myGradient;
		if (info.GetGradiendMode() == CBrushInfo::LEFTTORIGHT)
		{

			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().topRight());
			
		}
		else if (info.GetGradiendMode() == CBrushInfo::RIGHTTOLEFT)
		{
			myGradient.setStart(GetPosition().topRight());
			myGradient.setFinalStop(GetPosition().topLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::TOPTOBOTTOM)
		{
			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().bottomLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::BOTTOMTOTOP)
		{
			myGradient.setStart(GetPosition().bottomLeft());
			myGradient.setFinalStop(GetPosition().topLeft());

		}

		if (m_bPressFlag)
		{

			if (m_Shape == BTN_3D1 || m_Shape == BTN_3D2 || m_Shape == BTN_3D3 || m_Shape == BTN_3D4)
			{
				myGradient.setColorAt(0.0, QColor(info.GetColor().red() - 20 < 0 ? 0 : info.GetColor().red() - 20,
					info.GetColor().green() - 20 < 0 ? 0 : info.GetColor().green() - 20,
					info.GetColor().blue() - 20 < 0 ? 0 : info.GetColor().blue() - 20, 180));

				myGradient.setColorAt(0.5, QColor(info.GetEndColor().red() - 20 < 0 ? 0 : info.GetEndColor().red() - 20,
					info.GetEndColor().green() - 20 < 0 ? 0 : info.GetEndColor().green() - 20,
					info.GetEndColor().blue() - 20 < 0 ? 0 : info.GetEndColor().blue() - 20, 180));

				myGradient.setColorAt(1, QColor(info.GetColor().red() - 20 < 0 ? 0 : info.GetColor().red() - 20,
					info.GetColor().green() - 20 < 0 ? 0 : info.GetColor().green() - 20,
					info.GetColor().blue() - 20 < 0 ? 0 : info.GetColor().blue() - 20, 180));

			}
			else
			{
				myGradient.setColorAt(0.0, QColor(info.GetColor().red() - 20 < 0 ? 0 : info.GetColor().red() - 20,
					info.GetColor().green() - 20 < 0 ? 0 : info.GetColor().green() - 20,
					info.GetColor().blue() - 20 < 0 ? 0 : info.GetColor().blue() - 20, 180));

				myGradient.setColorAt(0.9, QColor(info.GetEndColor().red() - 20 < 0 ? 0 : info.GetEndColor().red() - 20,
					info.GetEndColor().green() - 20 < 0 ? 0 : info.GetEndColor().green() - 20,
					info.GetEndColor().blue() - 20 < 0 ? 0 : info.GetEndColor().blue() - 20, 180));

			}

		}
		else
		{
			if (m_Shape == BTN_3D1 || m_Shape == BTN_3D2 || m_Shape == BTN_3D3 || m_Shape == BTN_3D4)
			{
				myGradient.setColorAt(0.0, info.GetColor());
				myGradient.setColorAt(0.5, info.GetEndColor());
				myGradient.setColorAt(1, info.GetColor());

			}
			else
			{
				myGradient.setColorAt(0.0, info.GetColor());
				myGradient.setColorAt(0.9, info.GetEndColor());

			}
		}



		//pPainter->setBrush(myGradient);

		pPainter->fillPath(p, QBrush(myGradient));
		
	}
	else
	{

		if (m_bPressFlag)
		{

			pPainter->fillPath(p, QBrush(QColor(pBrush->color().red() - 20 < 0 ? 0 : pBrush->color().red() - 20,
				pBrush->color().green() - 20 < 0 ? 0 : pBrush->color().green() - 20,
				pBrush->color().blue() - 20 < 0 ? 0 : pBrush->color().blue() - 20, 180)));
		}
		else
		{
			pPainter->fillPath(p, *pBrush);
		}
		
	}

	pPainter->restore();
}

void CPushBtnWidget::GetNormalPath(QPainterPath& path)
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


void CPushBtnWidget::GetNormalAddPath(QPainterPath& path)
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

		p1.moveTo(rcShape.bottomLeft());
		p1.lineTo(rcShape.bottomRight());
		p1.lineTo(rcShape.topRight());

		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.bottomLeft());
		path.lineTo(rcShape.bottomRight());
		path.lineTo(rcShape.topRight());
	}
	
}

void CPushBtnWidget::GetNormalRressAddPath(QPainterPath& path)
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

		p1.moveTo(rcShape.bottomLeft());
		p1.lineTo(rcShape.topLeft());
		p1.lineTo(rcShape.topRight());

		path = tr_temp.map(p1);
	}
	else
	{
		path.moveTo(rcShape.bottomLeft());
		path.lineTo(rcShape.topLeft());
		path.lineTo(rcShape.topRight());
	}
}

void CPushBtnWidget::GetNormalMovingPath(QPainterPath& path)
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

void CPushBtnWidget::Get3D1Path(QPainterPath& path)
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

		p1.addRoundRect(rcShape, 30, 30);
		path = tr_temp.map(p1);
	}
	else
	{
		path.addRoundRect(rcShape, 30, 30);
	}
}

void CPushBtnWidget::Get3D1MovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}

void CPushBtnWidget::Get3D2Path(QPainterPath& path)
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

void CPushBtnWidget::Get3D2MovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}

void CPushBtnWidget::Get3D3Path(QPainterPath & path)
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

		p1.addRoundRect(rcShape, 10, 10);
		path = tr_temp.map(p1);
	}
	else
	{
		path.addRoundRect(rcShape, 10, 10);
	}
}

void CPushBtnWidget::Get3D3MovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}

void CPushBtnWidget::Get3D4Path(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();
	rcShape.setHeight(qMin(rcShape.width(), rcShape.height()));
	rcShape.setWidth(qMin(rcShape.width(), rcShape.height()));
	path.addEllipse(rcShape);
}

void CPushBtnWidget::Get3D4MovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}

void CPushBtnWidget::GetPath(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();

	switch (m_Shape)
	{
	case CPushBtnWidget::NORMAL:
	{
		//基本按钮
		GetNormalPath(path);
		break;
	}
	case CPushBtnWidget::BTN_3D1:
	{
		//3d1
		Get3D1Path(path);
		break;
	}
	case CPushBtnWidget::BTN_3D2:
	{
		//3d2
		Get3D2Path(path);
		break;
	}
	case CPushBtnWidget::BTN_3D3:
	{
		//3d3
		Get3D3Path(path);
		break;
	}
	case CPushBtnWidget::BTN_3D4:
	{
		//3d4
		Get3D4Path(path);
		break;
	}

	default:
		break;
	}
}



void CPushBtnWidget::SetFontLayout(QTextOption &txtOpt)
{
	if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_CENTER)
	{
		//中心
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_TOP)
	{
		//上
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_BOTTOM)
	{
		//下
		txtOpt.setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_LEFT)
	{
		//左
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_RIGHT)
	{
		//右
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignRight);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_LEFT)
	{
		//中  - 左
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_RIGHT)
	{
		//中  - 右
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	}
}

