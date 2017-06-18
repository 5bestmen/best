#ifndef SPIN_ITEM_H
#define SPIN_ITEM_H

#include "graph_global.h"

//#include <string>
#include <memory>
#include <QGraphicsItem>
 
 
class QGraphicsItem; 
/*! \class CSpinItem  spin_item.h  "spin_item.h"
*  \brief ��ת�õ�С��ť.
*/ 
class CSpinItem : public QGraphicsItem
{
public:
	CSpinItem();
	CSpinItem(QRectF pos);

	virtual ~CSpinItem();
public:


public:
public:
	QRectF GetPosition() const;

	void  NormalizePosition();

protected:
	// ��ȡ�ⲿѡ���ĳߴ�
	QRectF GetHandleRect(int nHandle);

	void InitDefault();
public:
	 
	 
};


#endif // SPIN_ITEM_H
