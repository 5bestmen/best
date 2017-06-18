#include <QPainter>
#include <QVariant>
#include <QtGui>
#include <math.h>
#include <QPalette>
#include "delegate.h"
#include "alarmdataconfig.h"

const int PaintingScaleFactor = 20;

StarRating::StarRating(int starCount, int maxStarCount)
{
	m_nStarCount = starCount;
	m_nMaxStarCount = maxStarCount;

	starPolygon << QPointF(1.0, 0.5);
	for (int i = 1; i < 5; ++i)
		starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
		0.5 + 0.5 * sin(0.8 * i * 3.14));

	diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)
		<< QPointF(0.6, 0.5) << QPointF(0.5, 0.6)
		<< QPointF(0.4, 0.5);
}

QSize StarRating::sizeHint() const
{
	return PaintingScaleFactor * QSize(m_nMaxStarCount, 1);
}

void StarRating::paint(QPainter *painter, const QRect &rect,
	const QPalette &palette, EditMode mode) const
{
	painter->save();

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::NoPen);

	if (mode == Editable) 
	{
		painter->setBrush(palette.highlight());
	}
	else 
	{
		painter->setBrush(palette.background());
	}

	int yOffset = (rect.height() - PaintingScaleFactor) / 2;
	painter->translate(rect.x(), rect.y() + yOffset);
	//»­±Ê×ø±ê  
	painter->scale(PaintingScaleFactor, PaintingScaleFactor);

	for (int i = 0; i < m_nMaxStarCount; ++i) 
	{
		if (i < m_nStarCount) 
		{
			painter->drawPolygon(starPolygon, Qt::WindingFill);
		}
		else if (mode == Editable) 
		{
			painter->drawPolygon(diamondPolygon, Qt::WindingFill);
		}
		painter->translate(1.0, 0.0);
	}

	painter->restore();
}


CStarDelegate::CStarDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

CStarDelegate::~CStarDelegate()
{

}

void CStarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 0)
	{
		StarRating starRating = qvariant_cast<StarRating>(index.data());

		if (option.state & QStyle::State_Selected)
		{
			painter->fillRect(option.rect, option.palette.highlight());
		}
		if (index.data().toInt() == alarm_confim)
		{
			starRating.paint(painter, option.rect, QPalette(Qt::green)/*option.palette*/,
				StarRating::ReadOnly);
		}
		else if (index.data().toInt() == alarm_unconfim)
		{
			starRating.paint(painter, option.rect, QPalette(Qt::red)/*option.palette*/,
				StarRating::ReadOnly);
		}		
	}
	else 
	{
		QStyledItemDelegate::paint(painter, option, index);
	}
}
