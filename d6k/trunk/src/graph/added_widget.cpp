#include "added_widget.h"
#include "widget_factory.h"
#include  "graph_module.h"
#include "curveplot_wgt.h"
#include "colour_define.h"


#include <QPainter>
#include <QGraphicsProxyWidget>

CAddedWidget::CAddedWidget()
{
}


CAddedWidget::CAddedWidget(QRectF rcPos, ADD_TYPE shape): CBaseWidget(rcPos)
{
	m_nObjType = 0;
	m_Shape = shape;

	m_rcPos = rcPos;
	m_rcMovingPos = rcPos;

	//默认为5
	m_nInterval = 5;

	m_pPlotWgt = nullptr;

	//m_pProxyWgt = new QGraphicsProxyWidget(this);


	//
	//setFlag(QGraphicsItem::ItemIsMovable, true);
// 	setFlag(QGraphicsItem::ItemIsSelectable, true);
// 	setFlag(QGraphicsItem::ItemIsFocusable, true);


	m_eRotateCenter = ROTATE_MIDCENTER;

	switch (shape)
	{
	case CAddedWidget::NORMAL_CURE_PLOT:
	{
		//一般曲线图
		m_szName = "CurePlot";
		m_nObjType = DRAW_ADDR_CURE_PLOT;
		m_rcPos.setSize(QSizeF(400, 400));
		m_rcMovingPos.setSize(QSizeF(400, 400));



// 		CCurePlotWgt *plotWgt = new CCurePlotWgt;
// 
// 		AddWidget(plotWgt->GetChartView());

		m_pPlotWgt = new CCurvePlotWgt(this);

		
		break;
	}

	default:
		break;
	}
}

// void CAddedWidget::AddWidget(QWidget * pWgt)
// {
// 	Q_ASSERT(pWgt);
// 	if (pWgt == nullptr)
// 	{
// 		return;
// 	}
// 
// 	m_pProxyWgt->setWidget(pWgt);
// }


void CAddedWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
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

	if (static_cast<int>(m_dblRotateAngle) % 90 != 0)
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

	//
	//m_pProxyWgt->resize(m_rcPos.size().width()-10,m_rcPos.size().height()-10);
	//m_pProxyWgt->setPos(m_rcPos.topLeft().x()+5,m_rcPos.topLeft().y()+5);

	m_pPlotWgt->resize(m_rcPos.size().width() - m_nInterval*2, m_rcPos.size().height() - m_nInterval * 2);
	m_pPlotWgt->setPos(m_rcPos.topLeft().x() + m_nInterval, m_rcPos.topLeft().y() + m_nInterval);
// 	m_pPlotWgt->UpdateChart();
}

QRectF CAddedWidget::boundingRect() const
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

QPointF CAddedWidget::GetRotationCenterPoint() const
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

void CAddedWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
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

void CAddedWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected /*= false*/)
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

	if (GetRotateAngle() != 0.0f )
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}

	pPainter->drawPath(path);
	pPainter->restore();
}
 
void CAddedWidget::GetPath(QPainterPath& path)
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

void CAddedWidget::GetMovingPath(QPainterPath& path)
{
	Q_UNUSED(path);
}



CAddedWidget::~CAddedWidget()
{
	//m_pProxyWgt->deleteLater();
}
