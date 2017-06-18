/*! @file background.cpp
<PRE>
********************************************************************************
模块名       :
文件名       : background.cpp
文件实现功能 : 背景
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  背景
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "background.h"


CBackground::CBackground()
{
	m_clrBackground = Qt::white;
	m_nSnapGridGap = 20;
	m_bShowGrid = true;
	m_bShowCoord = true;
//	m_rcBackground.setRect(0, 0, 1600, 800);
}

CBackground::~CBackground()
{
	m_arrSnapPoints.clear();
}

void CBackground::SetSize(QSize sizeGraph)
{
	m_rcBackground.setRect(0, 0, sizeGraph.width(), sizeGraph.height());
}

QRectF &CBackground::GetBackgroundRect() 
{
	return m_rcBackground;
}

void CBackground::DrawBackground(QPainter *pPainter, const QRectF &rect)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;
	if (rect.isNull() || rect.isEmpty())
		return; 
  
// 	QRectF fillrect = sceneRect.normalized().toRect();
 
	pPainter->save();
	QBrush br = pPainter->brush();
	br.setColor(m_clrBackground);
  	br.setStyle(Qt::SolidPattern);
// 	br.setTexture( (QPixmap("../product-ifix.jpg")) );
	pPainter->fillRect(rect, br);
	pPainter->drawRect(rect);
	pPainter->restore();
	
	if (m_bShowGrid)
	{
		m_arrGridLines.clear();
		
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int i = 0;

		i = 1;
		for (;;)
		{
			x1 = rect.left() + m_nSnapGridGap*i;
			y1 = rect.top();

			x2 = x1;
			y2 = rect.bottom();

			if (x1 >= rect.right())
			{
				break;
			}
			m_arrGridLines.push_back(QLine(x1,y1,x2,y2));
			i++;
		}
		i = 1;
		for (;;)
		{
			x1 = rect.left() ;
			y1 = rect.top() + m_nSnapGridGap*i;

			x2 = rect.right();
			y2 = y1;

			if (y1 >= rect.bottom())
			{
				break;
			}
			m_arrGridLines.push_back(QLine(x1, y1, x2, y2));
			i++;
		}

		if (m_arrGridLines.empty() == false)
		{
			QColor clGrid = Qt::lightGray;
			clGrid.setAlpha(50);
			pPainter->save();
			pPainter->setPen(clGrid);
			pPainter->drawLines(m_arrGridLines.data(), m_arrGridLines.size());
			pPainter->restore();
		}

	}
	else if (m_bShowSnapGridGap)
	{
		m_arrSnapPoints.clear();

		int tempx = rect.left();
		int tempy = rect.top();

		tempx = (tempx + (m_nSnapGridGap / 2)) / m_nSnapGridGap;
		tempx = tempx*m_nSnapGridGap;
		tempy = (tempy + (m_nSnapGridGap / 2)) / m_nSnapGridGap;
		tempy = tempy*m_nSnapGridGap;

		//创建point数组，批量绘制
		for (int x = (tempx + m_nSnapGridGap); x < rect.right(); x += m_nSnapGridGap)
		{
			for (int y = (tempy + m_nSnapGridGap); y < rect.bottom(); y += m_nSnapGridGap)
			{
				m_arrSnapPoints.push_back(QPoint(x, y));
			}
		}

		pPainter->save();
	//	pPainter->setPen(Qt::black);
	 
		pPainter->setPen(Qt::darkGray);
		pPainter->drawPoints(m_arrSnapPoints.data(), m_arrSnapPoints.size());
		//painter->drawRect(tempx, tempy, testrect.width(), testrect.height());
		pPainter->restore();
	}
	if (m_bShowCoord)
	{
		QLine lnHorz(rect.left(), rect.height() / 2, rect.right(), rect.height() / 2);
		QLine lnVert(rect.width() / 2, rect.top(), rect.width() / 2, rect.bottom());

		pPainter->save();
		pPainter->setPen(Qt::darkGray);
		pPainter->drawLine(lnHorz);
		pPainter->drawLine(lnVert);

		pPainter->restore();
	}
}

/** @}*/
