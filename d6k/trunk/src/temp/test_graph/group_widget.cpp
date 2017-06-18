/*! @file group_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : group_widget.cpp
文件实现功能 : 组合图元
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  组合图元
*  @author LiJin
*  @version 1.0
*  @date    2016.02.3
*******************************************************************************/
#include "group_widget.h" 
#include "colour_define.h"

#include <QPainter>

CGroupWidget::CGroupWidget() :CBaseWidget()
{
	m_nObjType = WIDGET_GROUP;
	m_szName = "GROUP";
	m_szTxt = QStringLiteral("我是组合图元");

	m_rcMovingPos = m_rcPos;
}

CGroupWidget::~CGroupWidget()
{
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		Q_ASSERT((*iter).WidgetPtr);
		if ((*iter).WidgetPtr)
		{
			delete (*iter).WidgetPtr;
			(*iter).WidgetPtr = nullptr;
		}
	}

	m_arrWidgets.clear();
}


void CGroupWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	Q_UNUSED(widget);
 
//	pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			if (it.WidgetPtr->isVisible())
			{
				it.WidgetPtr->paint(pPainter, option, widget);
			}
		}
	}
	QPen pen(Qt::darkGray);
	pen.setWidthF(m_PenInfo.GetWidth());
	pen.setStyle(Qt::SolidLine);

	pPainter->save();	
	m_szTxt = QString::fromLocal8Bit(m_szWidgetName.c_str());
	if (m_szTxt.length())
	{
		//pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		QTextOption txtOpt;
		//txtOpt.
		pPainter->setRenderHint(QPainter::TextAntialiasing);
		pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
	}
	pPainter->restore();

	if (m_bSelected)
	{
		DrawSingleSelectedTracker(pPainter);
	}
}

QRectF CGroupWidget::boundingRect() const
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


void CGroupWidget::DrawSingleSelectedTracker(QPainter *pPainter)
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
/*! \fn void CGroupWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
********************************************************************************************************* 
** \brief CGroupWidget::MoveHandleTo 
** \details  移动某个爪子到新的位置，调整图元的大小
** \param nHandle 
** \param point 
** \return void 
** \author LiJin
** \date 2016年2月15日 
** \note 
********************************************************************************************************/
void CGroupWidget::MoveHandleTo(int nHandle, const QPointF &ptInput)
{
	QRectF rcLoc = m_rcPos;
	QPointF ptTemp = ptInput;

	switch (nHandle)
	{
	case POS_TOP_LEFT:
		//  移动左上角不能超过右边及下边
		if (ptTemp.x() > rcLoc.right()- WIDGET_MULTI_SELECTED_MARGIN * 2)
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


void CGroupWidget::GetPath(QPainterPath& path)
{
	QRectF rcShape = m_rcPos.normalized();
	path.addRect(rcShape); 
}

void  CGroupWidget::GetMovingPath(QPainterPath& path)
{
	QRectF rcShape = m_rcMovingPos.normalized();

	path.addRect(rcShape);
} 
/*! \fn void CGroupWidget::AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat)
********************************************************************************************************* 
** \brief CGroupWidget::AddWidget 
** \details 添加一个图元到成组中
** \param pWidget 
** \param rcRelat 
** \return void 
** \author LiJin
** \date 2016年2月11日 
** \note 
********************************************************************************************************/
void CGroupWidget::AddWidget(CBaseWidget *pWidget, const QRectF & rcRelat)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;
	QRectF rcBound = m_rcPos;
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		if ((*iter).WidgetPtr && (*iter).WidgetPtr == pWidget)
		{// 有重复
			Q_ASSERT(false);
			return;
		}
	}
	if (m_arrWidgets.empty() == true)
	{
		m_rcPos = pWidget->GetPosition();
	}
	else
	{
		m_rcPos = m_rcPos.united(pWidget->GetPosition());
	}

	GRP_MEMBER member = { pWidget, rcRelat };
	m_arrWidgets.push_back(member);
	m_rcMovingPos = m_rcPos;
}
/*! \fn void CGroupWidget::MoveTo(const QRectF & rcPosition)
********************************************************************************************************* 
** \brief CGroupWidget::MoveTo 
** \details 确定图元的新位置
** \param rcPosition 
** \return void 
** \author LiJin
** \date 2016年2月11日 
** \note 
********************************************************************************************************/
void CGroupWidget::MoveTo(const QRectF & rcPosition)
{
	m_rcPos = rcPosition;
	m_rcMovingPos = m_rcPos;

	QRectF rect, scale;
	std::vector<GRP_MEMBER>::iterator iter = m_arrWidgets.begin();
	for (; iter != m_arrWidgets.end(); ++iter)
	{
		Q_ASSERT((*iter).WidgetPtr);
		if ((*iter).WidgetPtr)
		{
			scale = (iter)->RelativePos;

			rect.setX(m_rcPos.x() + scale.x()*m_rcPos.width());
			rect.setY(m_rcPos.y() + scale.y()*m_rcPos.height());
			rect.setWidth(scale.width()*m_rcPos.width());
			rect.setHeight(scale.height()*m_rcPos.height());

// 			rect.setX((int)(rect.x() * 100) / 100.f);
// 			rect.setY((int)(rect.y()* 100) / 100.f);
// 			rect.setWidth((int)(rect.width() * 100) / 100.f);
// 			rect.setHeight( (int)(rect.height() * 100) / 100.f);

			(*iter).WidgetPtr->MoveTo(rect);
		}
	}
}
void CGroupWidget::MoveDelta(const QPointF &delta)
{
//	qDebug() << delta;
	m_rcPos.translate(delta);
	m_rcMovingPos = m_rcPos;
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			it.WidgetPtr->MoveDelta(delta);		 
		}
	}
}

void CGroupWidget::MovingDelta(const QPointF &delta)
{
//	qDebug() << delta;
	m_rcMovingPos.translate(delta);
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			it.WidgetPtr->MovingDelta(delta);
		}
	}
}
/*! \fn void CGroupWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
********************************************************************************************************* 
** \brief CGroupWidget::DrawMovingWidget 
** \details 画移动中的组合图元 
** \param pPainter 
** \param bSingleSelected 
** \return void 
** \author LiJin
** \date 2016年2月11日 
** \note 单选该图元状态的外部选择框为淡兰色的虚框，如果是多选状态原来是啥样，还是啥样
********************************************************************************************************/
void CGroupWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;	
	for (auto it : m_arrWidgets)
	{
		Q_ASSERT(it.WidgetPtr != nullptr);
		if (it.WidgetPtr != nullptr)
		{
			//if (it.WidgetPtr->isVisible())
			{
				it.WidgetPtr->DrawMovingWidget(pPainter, false);
			}
		}
	}
	
	// 画外部选择框
	pPainter->save();
	if (bSingleSelected)
	{
		QPen pen(XGraphics::LightBlue);
		pen.setWidthF(m_PenInfo.GetWidth());
		pen.setStyle(Qt::DashLine);

		pPainter->setPen(pen);
	}
	else
	{// 多选的移动 ，原来是啥样，还是啥样
		QPen pen(Qt::darkGray);
		pen.setWidth(1);
		pen.setStyle(Qt::SolidLine);
		pPainter->setPen(pen);
	}
	QPainterPath path;
	GetMovingPath(path);
	pPainter->drawPath(path);
	pPainter->restore();
}
/*! \fn void CGroupWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
********************************************************************************************************* 
** \brief CGroupWidget::DrawMultiSelectedTracker 
** \details 多选状态下，该图元的外框
** \param pPainter 
** \param bFirstSelected 
** \return void 
** \author LiJin
** \date 2016年2月11日 
** \note 如果是第一个被选中的话，则外框加粗
********************************************************************************************************/
void CGroupWidget::DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	QRectF rcFrame = this->GetPosition();  
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

/** @}*/

