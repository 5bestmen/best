#pragma once

#include <QGraphicsItem>

class CGroupMoveLine : public QGraphicsItem
{
public:
	CGroupMoveLine();
	~CGroupMoveLine();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget);

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;

	void InsetNewPoint(const QPointF &pfPoint);

private:
	QRectF m_rect;

	//µã¼¯ºÏ
	QList<QPointF> m_lstPfPoints;
};
