#ifndef DELEGATE_H
#define DELEGATE_H

#include <QStyledItemDelegate>
#include <QMetaType>  
#include <QPointF>  
#include <QVector>  

class StarRating
{
public:
	enum EditMode { Editable, ReadOnly };

	StarRating(int starCount = 1, int maxStarCount = 5);

	void paint(QPainter *painter, const QRect &rect,
		const QPalette &palette, EditMode mode) const;
	QSize sizeHint() const;
	int starCount() const
	{ 
		return m_nStarCount;
	}
	int maxStarCount() const 
	{
		return m_nMaxStarCount;
	}
	void setStarCount(int starCount) 
	{
		m_nStarCount = starCount; 
	}
	void setMaxStarCount(int maxStarCount)
	{
		m_nMaxStarCount = maxStarCount;
	}

private:
	QPolygonF starPolygon;
	QPolygonF diamondPolygon;
	int m_nStarCount;
	int m_nMaxStarCount;
};

Q_DECLARE_METATYPE(StarRating);

class CStarDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	CStarDelegate(QObject *parent=0);
	~CStarDelegate();
public:
	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
	
};

#endif // DELEGATE_H
