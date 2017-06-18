/*! @file shape_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : shape_widget.cpp
文件实现功能 : 基本形状图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  基本形状图元
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "shape_widget.h" 
#include "colour_define.h"
#include "graphics_layer.h"

#include <QPainter>

CShapeWidget::CShapeWidget() :CBaseWidget()
{

}

CShapeWidget::~CShapeWidget()
{

}

CShapeWidget::CShapeWidget(QRectF rcPos, CShapeWidget::SHAPE shape) :CBaseWidget(rcPos)
{
	m_nObjType = WIDGET_RECT;
	m_Shape = shape;

	switch (shape)
	{
	case CShapeWidget::RECTANGLE:
		m_szName = "RECT";
		m_szTxt = QStringLiteral("我是矩形");
		break;
	case CShapeWidget::ELLIPSE:
		break;
	case CShapeWidget::ROUNDRECT:
		break;
	case CShapeWidget::TRAPEZOID:
		break;
	case CShapeWidget::PARALLELOGRAM:
		break;
	case CShapeWidget::DIAMOND:
		break;
	case CShapeWidget::TRAPEZOID_MIRROR:
		break;
	case CShapeWidget::PARALLELOGRAM_MIRROR:
		break;
	case CShapeWidget::ROUND:
		m_szName = "ROUND";
		m_szTxt = QStringLiteral("我是圆形");
		break;
	case CShapeWidget::TRIANGLE:
		break;
	case CShapeWidget::ERR_SHAPE:
		break;
	default:
		break;
	}
//	setToolTip(QStringLiteral("我是矩形，哈哈..."));
}

void CShapeWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	Q_UNUSED(widget);
 
//	pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	pPainter->save();
	
	QPen pen(Qt::darkGray);
	pen.setWidthF(m_PenInfo.GetWidth());
	pen.setStyle(Qt::SolidLine);
	pPainter->setPen(pen);

	QColor clBrush(XGraphics::LightBlue);
	clBrush.setAlpha(255);
	QBrush brBack;
	brBack.setColor(clBrush);
	brBack.setStyle(Qt::SolidPattern);
	if (m_Shape == RECTANGLE)
	{
	}
	else
	{
		clBrush.setRgb(XGraphics::YellowGreen);
		clBrush.setAlpha(255);
		brBack.setColor(clBrush);
	}
	pPainter->fillRect(m_rcPos, brBack);
//	pPainter->drawRect(m_rcPos);
	QPainterPath path;
	GetPath(path);
	if (m_Shape == RECTANGLE)
	{
		
	}
	else
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}
	pPainter->drawPath(path);
	pPainter->restore();

	m_szTxt = QString::fromLocal8Bit(m_szWidgetName.c_str());
	if (m_szTxt.length())
	{
		//pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		QTextOption txtOpt;
		//txtOpt.
		pPainter->setRenderHint(QPainter::TextAntialiasing);
		pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
#if DEBUG
		QString szVal = QString("Z=%1").arg(this->zValue());
		QRectF rcTxt = m_rcPos;
		rcTxt.setY(m_rcPos.y() + 16);
		pPainter->drawText(rcTxt, szVal, txtOpt);
#endif
	}

	if (m_bSelected)
	{// 单选状态
		Q_ASSERT(m_pGraphicsLayer);
		if (m_pGraphicsLayer && m_pGraphicsLayer->IsCurrentLayer())
		{
			auto nRet = m_pGraphicsLayer->IsSingleSelection();
			if (nRet == CGraphicsLayer::SINGLE_SELECTED)
			{// 单选
				DrawSingleSelectedTracker(pPainter);
			}
			else if (nRet == CGraphicsLayer::MULTI_SELECTED)
			{// 多选
				DrawMultiSelectedTracker(pPainter,m_pGraphicsLayer->IsFirstSelectedWidget(this));
			}			
		}
	}
}

QRectF CShapeWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();	
	double dPenWidth = m_PenInfo.GetWidth()/2;

	if (dPenWidth > 0.0f)
	{
		// 扩大半个线宽
		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
	}

	return rcBounding;
}


void CShapeWidget::DrawSingleSelectedTracker(QPainter *pPainter)
{ 
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QRectF rcFrame = this->GetPosition();  //m_rcPos.normalized();
//	rcFrame.adjust(-1, -1, 1, 1);
	// 框
	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);

	// 爪
	unsigned int nHandles = GetHandleCount();

	QPointF ptSpinStart = GetSpinHandlePos(); // 靠近矩形框（图元）的起始点
	QPointF ptSpinEnd(ptSpinStart);
	ptSpinEnd.setY(ptSpinStart.y() - 15);
	QLine lnSpin(ptSpinStart.toPoint(), ptSpinEnd.toPoint());
	
	pPainter->save();
	pPainter->setPen(pen);
	// 画外部矩形框
	pPainter->drawRect(rcFrame);

	pPainter->drawLine(lnSpin);
	unsigned int i = 0;
	QRectF rcHandler;

	if (false) // 旋转
	{

	}
	else
	{
		//QBrush br(Qt::green);
		for (i = 1; i <= nHandles; i++)
		{
			rcHandler = GetHandleRect(i);
			pPainter->setBrush(Qt::white);
			pPainter->setPen(Qt::darkGray);
			pPainter->drawRect(rcHandler);
		}
	}

	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);
	pen.setColor(Qt::darkGray);

	pPainter->setRenderHint(QPainter::Antialiasing);
	pPainter->setPen(pen);
	QRectF rcSpin(ptSpinEnd.x() - 7, ptSpinEnd.y() - 14,14, 14);
	pPainter->drawArc(rcSpin, -60*16, -300 * 16);
	// 缩小3个像素
	rcSpin.adjust(3, 3, -3, -3);

// 	pen.setWidth(1);
// 	pen.setColor(Qt::white);
// 	pPainter->setPen(pen);
	pPainter->drawArc(rcSpin, -60 * 16, -300 * 16);


	double fAngle = 30;
	double fRadius = 7;

	double ptCenterX = ptSpinEnd.x();
	double ptCenterY = ptSpinEnd.y() - 7;

	double ptx = ptCenterX + fRadius * sin(fAngle* 3.14 / 180);
	double pty = ptCenterY + fRadius * cos(fAngle* 3.14 / 180);

	QPointF ptStart(ptx, pty);

	fRadius = 4;
	ptx = ptCenterX + fRadius * sin(fAngle* 3.14 / 180);
	pty = ptCenterY + fRadius * cos(fAngle* 3.14 / 180);
	QPointF ptEnd(ptx, pty);

	pPainter->drawLine(ptStart, ptEnd);

	QPointF ptArraw;
	ptStart.setX(ptCenterX + 7 + 2);
	ptStart.setY(ptCenterY);

	ptEnd = ptStart;
	ptEnd.setX(ptStart.x() - 4 - 3);

	ptArraw = ptStart;
	ptArraw.setY(ptStart.y() + 3);
	ptArraw.setX(ptEnd.x() + (ptStart.x() - ptEnd.x()) / 2);

	pPainter->drawLine(ptStart, ptArraw);

	pPainter->drawLine(ptEnd, ptArraw);

//	pPainter->drawLine(ptStart, ptEnd);
	QPointF ptStart1(ptStart);
	ptStart1.setX(ptStart.x() - 2);
	pPainter->drawLine(ptStart, ptStart1);

	QPointF ptEnd1(ptEnd);
	ptEnd1.setX(ptEnd.x() + 2);
	pPainter->drawLine(ptEnd, ptEnd1);

// 	QBrush brTmp;
// 	brTmp.setColor(Qt::red);
	//pPainter->drawPoint(ptTmp);

	pPainter->restore();
}


void CShapeWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QRectF rcFrame = this->GetPosition();  //m_rcPos.normalized();
	//	rcFrame.adjust(-1, -1, 1, 1);
	// 框
	QPen pen(XGraphics::DarkBlue);
	if (bFirstSelected)
	{
		pen.setWidth(3);
	}
	else
	{
		pen.setWidth(2);
	}
	pen.setStyle(Qt::SolidLine);

	pPainter->save();
	pPainter->setPen(pen);
	// 画外部矩形框
	pPainter->drawRect(rcFrame);

	pPainter->restore();
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
 	pPainter->fillRect(m_rcMovingPos, brBack);
	//	pPainter->drawRect(m_rcPos);
	QPainterPath path;
	GetMovingPath(path);
	if (m_Shape == RECTANGLE)
	{

	}
	else
	{
		pPainter->setRenderHint(QPainter::Antialiasing);
	}
	pPainter->drawPath(path);
	pPainter->restore();
}

void CShapeWidget::MoveHandleTo(int nHandle, const QPointF &point)
{
	QPointF ptTopLeft,ptBottomRight;

	ptTopLeft.setX(m_rcPos.x());
	ptTopLeft.setY(m_rcPos.y());
	
	__super::MoveHandleTo(nHandle, point);

	ptBottomRight.setX(m_rcPos.width());
	ptBottomRight.setY(m_rcPos.height());

	//m_rcPos.moveTo(ptTopLeft - ptBottomRight);
}


void  CShapeWidget::GetMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	switch (m_Shape)
	{
	case CShapeWidget::RECTANGLE:
		path.addRect(rcShape);
		break;
	case CShapeWidget::ELLIPSE:
		break;
	case CShapeWidget::ROUNDRECT:
		break;
	case CShapeWidget::TRAPEZOID:
		break;
	case CShapeWidget::PARALLELOGRAM:
		break;
	case CShapeWidget::DIAMOND:
		break;
	case CShapeWidget::TRAPEZOID_MIRROR:
		break;
	case CShapeWidget::PARALLELOGRAM_MIRROR:
		break;
	case CShapeWidget::ROUND:
		rcShape.setHeight(qMin(rcShape.width(), rcShape.height()));
		rcShape.setWidth(qMin(rcShape.width(), rcShape.height()));
		path.addEllipse(rcShape);
		break;
	case CShapeWidget::TRIANGLE:
		break;
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
		path.addRect(rcShape);
		break;
	case CShapeWidget::ELLIPSE:
		break;
	case CShapeWidget::ROUNDRECT:
		break;
	case CShapeWidget::TRAPEZOID:
		break;
	case CShapeWidget::PARALLELOGRAM:
		break;
	case CShapeWidget::DIAMOND:
		break;
	case CShapeWidget::TRAPEZOID_MIRROR:
		break;
	case CShapeWidget::PARALLELOGRAM_MIRROR:
		break;
	case CShapeWidget::ROUND:
		rcShape.setHeight(qMin(rcShape.width(), rcShape.height()));
		rcShape.setWidth(qMin(rcShape.width(), rcShape.height()));
		path.addEllipse(rcShape);
		break;
	case CShapeWidget::TRIANGLE:
		break;
	case CShapeWidget::ERR_SHAPE:
		break;
	default:
		break;
	}

}

/** @}*/

