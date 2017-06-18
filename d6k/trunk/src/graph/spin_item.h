#ifndef SPIN_ITEM_H
#define SPIN_ITEM_H

#include "graph_global.h"

//#include <string>
#include <memory>
 
 
class CBaseWidget;
class QPainter;
 
/*! \class CSpinItem  spin_item.h  "spin_item.h"
*  \brief 旋转用的小旋钮.
*/ 
class CSpinItem  
{
public:
 	CSpinItem() = delete;
	CSpinItem(CBaseWidget *pWidget);

	virtual ~CSpinItem();
public:
	void DrawSpin(QPainter *pPainter);  
private:
	CBaseWidget *m_pWidget;
};


#endif // SPIN_ITEM_H
