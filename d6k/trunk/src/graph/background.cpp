/*! @file background.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  background.cpp
�ļ�ʵ�ֹ��� :  ����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "background.h"
#include <QApplication>


CBackground::CBackground()
{
	m_clrBackground = QColor(255, 255, 255);
	m_nSnapGridGap = 20;
	m_bShowGrid = true;
	m_bShowCoord = true;
	m_nGradiendMode = 0;
	m_nBrushStyle = 1;
	m_pfImage = QPoint(0, 0);
	m_bTiling = false;
	m_bStretch = false;
	m_szImage = QSize(0,0);

	//	m_rcBackground.setRect(0, 0, 1600, 800);
}

CBackground::~CBackground()
{
	m_arrSnapPoints.clear();
}

void CBackground::SetSize(QSize &sizeGraph)
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
	// 	br.setTexture( (QPixmap("../product-ifix.jpg")) );
	///////////////


	QBrush br = pPainter->brush();
	br.setColor(m_clrBackground);
	br.setStyle((Qt::BrushStyle)m_nBrushStyle);


	//////////////
	if (m_nGradiendMode != 0)
	{
		//����
		QPainterPath tPath;
		tPath.addRect(rect);

		QLinearGradient myGradient;
		if (m_nGradiendMode == LEFTTORIGHT)
		{
			myGradient.setStart(rect.topLeft());
			myGradient.setFinalStop(rect.topRight());
		}
		else if (m_nGradiendMode == RIGHTTOLEFT)
		{
			myGradient.setStart(rect.topRight());
			myGradient.setFinalStop(rect.topLeft());

		}
		else if (m_nGradiendMode == TOPTOBOTTOM)
		{
			myGradient.setStart(rect.topLeft());
			myGradient.setFinalStop(rect.bottomLeft());

		}
		else if (m_nGradiendMode == BOTTOMTOTOP)
		{
			myGradient.setStart(rect.bottomLeft());
			myGradient.setFinalStop(rect.topLeft());

		}

		myGradient.setColorAt(0.0, m_clrBackground);
		myGradient.setColorAt(0.9, m_cloEndColor);

		//pPainter->setBrush(myGradient);
		pPainter->fillPath(tPath, QBrush(myGradient));
	}
	else
	{
		pPainter->fillRect(rect, br);
	}
	///////////////

	pPainter->drawRect(rect);
	pPainter->restore();

	pPainter->save();
	//����ͼƬ
	QPixmap tImage(qApp->applicationDirPath() + m_strBackgroundImage);

	//m_szImage = tImage.size();


	if (!tImage.isNull())
	{
		if (m_szImage.width() == 0 || m_szImage.height() == 0)
		{
			//ͼƬ��ʵ�ʳߴ�
			m_szImage = tImage.size();
		}
		else
		{
			tImage = tImage.scaled(m_szImage);
		}

		if (m_bStretch)
		{
			//����
			pPainter->drawPixmap(rect.topLeft() + m_pfImage, tImage.scaled(rect.size().toSize()));
			//m_szImage = rect.size().toSize();

		}
		else if (m_bTiling)
		{
			//ƽ��
			pPainter->drawTiledPixmap(rect, tImage);
			//m_szImage = rect.size().toSize();
		}
		else if (tImage.size().width() > rect.size().width() || tImage.size().height() > rect.size().height())
		{
			pPainter->drawPixmap(rect.topLeft() + m_pfImage, tImage.scaled(rect.size().toSize()));
			//m_szImage = rect.size().toSize();
		}
		else
		{
			pPainter->drawPixmap(rect.topLeft() + m_pfImage, tImage);
			//m_szImage = tImage.size();

		}
	}

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
			m_arrGridLines.push_back(QLine(x1, y1, x2, y2));
			i++;
		}
		i = 1;
		for (;;)
		{
			x1 = rect.left();
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
			pPainter->drawLines(m_arrGridLines.data(), (int)m_arrGridLines.size());
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

		//����point���飬��������
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
		pPainter->drawPoints(m_arrSnapPoints.data(), (int)m_arrSnapPoints.size());
		//painter->drawRect(tempx, tempy, testrect.width(), testrect.height());
		pPainter->restore();
	}
	if (m_bShowCoord)
	{
		QLine lnHorz(rect.left(), rect.y() + rect.height() / 2, rect.right(), rect.y() + rect.height() / 2);
		QLine lnVert(rect.x() + rect.width() / 2, rect.top(), rect.x() + rect.width() / 2, rect.bottom());

		pPainter->save();
		pPainter->setPen(Qt::darkGray);
		pPainter->drawLine(lnHorz);
		pPainter->drawLine(lnVert);

		pPainter->restore();
	}
}

/** @}*/
