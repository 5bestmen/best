#include <QPainter>
#include "group_move_line.h"

CGroupMoveLine::CGroupMoveLine()
{
	m_rect.setRect(0, 0, 500, 500);
}

CGroupMoveLine::~CGroupMoveLine()
{
}

void CGroupMoveLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
	Q_UNUSED(item);
	Q_UNUSED(widget);

	painter->setRenderHint(QPainter::Antialiasing);

	for(int i=0; i<m_lstPfPoints.count(); i++)
	{
		painter->save();
		QPen tPen;
		tPen.setWidth(4);
		painter->setPen(tPen);
		painter->drawPoint(m_lstPfPoints.at(i));

		painter->restore();

		if (i > 0)
		{
			//»®Ïß
			painter->drawLine(m_lstPfPoints.at(i), m_lstPfPoints.at(i - 1));

		}
	}
}

QRectF CGroupMoveLine::boundingRect() const
{
	return m_rect;

}

QPainterPath CGroupMoveLine::shape() const
{
	QPainterPath path;
	path.addRect(m_rect);
	return path;
}

void CGroupMoveLine::InsetNewPoint(const QPointF &pfPoint)
{
	m_lstPfPoints.append(pfPoint);
	update();
}
