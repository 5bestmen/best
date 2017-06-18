
#include "spin_item.h"
#include "base_widget.h"
#include <QPen>
#include <QPainter>
 
CSpinItem::CSpinItem(CBaseWidget *pWidget)
{
	m_pWidget = pWidget;
	Q_ASSERT(pWidget);
}


CSpinItem::~CSpinItem()
{

}
/*! \fn void CSpinItem::DrawSpin(QPainter *pPainter)
********************************************************************************************************* 
** \brief CSpinItem::DrawSpin 
** \details 绘旋转的小旋钮
** \param pPainter 
** \return void 
** \author LiJin 
** \date 2016年3月21日 
** \note 
********************************************************************************************************/
void CSpinItem::DrawSpin(QPainter *pPainter)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	Q_ASSERT(m_pWidget);
	if (m_pWidget == nullptr)
		return;

	pPainter->save();

 	QPointF ptCenter = m_pWidget->GetRotationCenterPoint();
	double dblRotateAngle = m_pWidget->GetRotateAngle();
	if (dblRotateAngle != 0.0f)
	{
 		pPainter->translate(ptCenter);
 		pPainter->rotate(dblRotateAngle);
 		pPainter->translate(-ptCenter);
	}
 
	QPen pen(Qt::darkGray);
	pen.setWidth(1);
	pen.setStyle(Qt::SolidLine);

	pPainter->setRenderHint(QPainter::Antialiasing);
	pPainter->setPen(pen);
	
	QPointF ptSpinStart = m_pWidget->GetSpinHandlePos(); // 靠近矩形框（图元）的起始点
	QPointF ptSpinEnd(ptSpinStart);
	ptSpinEnd.setY(ptSpinStart.y() - 15);
	QLine lnSpin(ptSpinStart.toPoint(), ptSpinEnd.toPoint());

	QRectF rcSpin(ptSpinEnd.x() - 7, ptSpinEnd.y() - 14, 14, 14);
 
	pPainter->drawArc(rcSpin, -60 * 16, -300 * 16);
	// 缩小3个像素
	rcSpin.adjust(3, 3, -3, -3);
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

	QPointF ptStart1(ptStart);
	ptStart1.setX(ptStart.x() - 2);
	pPainter->drawLine(ptStart, ptStart1);

	QPointF ptEnd1(ptEnd);
	ptEnd1.setX(ptEnd.x() + 2);
	pPainter->drawLine(ptEnd, ptEnd1);

	pPainter->restore();
}
