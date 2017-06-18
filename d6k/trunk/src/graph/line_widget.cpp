/*! @file line_widget.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  line_widget.cpp
文件实现功能 :  图层
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   图层
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/

#include "line_widget.h"
#include "widget_factory.h"
#include "colour_define.h"

#include <QPainter>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QRadialGradient>

CLineWidget::CLineWidget()
{
	m_nObjType = WIDGET_LINE;
}

CLineWidget::~CLineWidget()
{

}

CLineWidget::CLineWidget(QRectF rcPos, CLineWidget::LINE shape) :CBaseWidget(rcPos)
{
	m_nObjType = WIDGET_LINE;
    //m_nObjType = DRAW_TOOLS_LINE;

    switch (shape)
    {
    case CLineWidget::BEE_LINE:
    {
        //直线
		m_szName = "Line";
        m_nObjType = DRAW_TOOLS_LINE;
        break;
    }
    case CLineWidget::POLY_LINE:
    {
        //折线
		m_szName = "Polyline";
        m_nObjType = DRAW_TOOLS_POLYLINE;
        break;
    }
	case CLineWidget::POLYGONO:
	{
		//多边形
		m_szName = "Polygon";
		m_nObjType = DRAW_TOOLS_POLYGON;
		break;
	}
	case CLineWidget::PIPE:
	{
		//管道
		m_szName = "Pipe";
		m_nObjType = DRAW_TOOLS_PIPE;
		//线条变粗
		GetPenInfo()->SetWidth(20);
		//GetPenInfo()->SetColor(QColor(0, 0, 0));
		GetBrushInfo()->SetColor(QColor(0, 0, 0));
		GetBrushInfo()->SetEndColor(QColor(255,255,255));
// 		GetBrushInfo()->SetColor(QColor(0,0,0));
// 		GetBrushInfo()->SetEndColor(QColor(255,0,255));
		break;
	}
    default:
        break;
    }

	//GetBrushInfo()->SetColor(QColor(255,255,255,0));
}

void CLineWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;


    QPainterPath path;
    GetPath(path);



    pPainter->save();
    QPen pen;
	m_PenInfo.ToQPen(pen);
// 
	if (m_nObjType == DRAW_TOOLS_PIPE)
	{
		//管道
		pPainter->setRenderHint(QPainter::Antialiasing);
		pen.setCapStyle(Qt::RoundCap);
		pen.setJoinStyle(Qt::RoundJoin);
		CBrushInfo* pBrush = GetBrushInfo();

		FillPiPePath(pPainter, *pBrush);
	}
	else
	{
		pPainter->setPen(pen);
		pPainter->setRenderHint(QPainter::Antialiasing);

		pPainter->drawPath(path);
	}

    

	if (m_nObjType == DRAW_TOOLS_POLYGON)
	{
		CBrushInfo* pBrush = GetBrushInfo();
		FillPath(pPainter, path, *pBrush);
	}


    pPainter->restore();

	pPainter->save();
	if (m_szTxt.length())
	{
		//pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
		QTextOption txtOpt;
		SetFontLayout(txtOpt);

		//txtOpt.
		pen.setColor(m_FontInfo.GetColor());
		pPainter->setPen(pen);
		pPainter->setFont(m_FontInfo.GetFont());
		pPainter->setRenderHint(QPainter::TextAntialiasing);

		if (m_dblRotateAngle != 0.0f)
		{
			QPointF ptCenter = GetRotationCenterPoint();

			pPainter->translate(ptCenter);
			pPainter->rotate(m_dblRotateAngle);
			pPainter->translate(-ptCenter.x(), -ptCenter.y());
		}

		pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
#if 0
		QString szVal = QString("Z=%1").arg(this->zValue());
		QRectF rcTxt = m_rcPos;
		rcTxt.setY(m_rcPos.y() + 16);
		pPainter->drawText(rcTxt, szVal, txtOpt);
#endif
	}
	pPainter->restore();

    DrawSelectedTracker(pPainter);

}

void CLineWidget::FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	auto pBrush = info.CreateQBrush();

	pPainter->save();

	if (info.GetGradiendMode() != 0)
	{
		//渐变
		QLinearGradient myGradient;
		if (info.GetGradiendMode() == CBrushInfo::LEFTTORIGHT)
		{
			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().topRight());
		}
		else if (info.GetGradiendMode() == CBrushInfo::RIGHTTOLEFT)
		{
			myGradient.setStart(GetPosition().topRight());
			myGradient.setFinalStop(GetPosition().topLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::TOPTOBOTTOM)
		{
			myGradient.setStart(GetPosition().topLeft());
			myGradient.setFinalStop(GetPosition().bottomLeft());

		}
		else if (info.GetGradiendMode() == CBrushInfo::BOTTOMTOTOP)
		{
			myGradient.setStart(GetPosition().bottomLeft());
			myGradient.setFinalStop(GetPosition().topLeft());

		}

		myGradient.setColorAt(0.0, info.GetColor());
		myGradient.setColorAt(0.9, info.GetEndColor());

		//pPainter->setBrush(myGradient);
		pPainter->fillPath(p, QBrush(myGradient));
	}
	else
	{
		pPainter->fillPath(p, *pBrush);
	}


	pPainter->restore();
}


void CLineWidget::FillPiPePath(QPainter *pPainter, CBrushInfo &info)
{
	Q_ASSERT(pPainter);
	if (pPainter == nullptr)
		return;

	auto pBrush = info.CreateQBrush();

	pPainter->save();


	for (int i = m_arrRelativePoints.count()-1; i >0; i--)
	{

		QRectF tref;

		QPointF pfStart(m_arrRelativePoints.at(i-1).x()*GetPosition().width() + GetPosition().topLeft().x(),
			m_arrRelativePoints.at(i - 1).y()*GetPosition().height() + GetPosition().topLeft().y());

		QPointF pfEnd(m_arrRelativePoints.at(i).x()*GetPosition().width() + GetPosition().topLeft().x(),
			m_arrRelativePoints.at(i).y()*GetPosition().height() + GetPosition().topLeft().y());

		tref.setX(pfStart.x());
		tref.setY(pfStart.y() - GetPenInfo()->GetWidth() / 2.0);
		tref.setWidth(sqrt(pow(pfEnd.x() - pfStart.x(), 2) + pow(pfEnd.y() - pfStart.y(), 2)));
		tref.setHeight(GetPenInfo()->GetWidth());

		double angel = CalculateRoutateAngel(pfStart, pfEnd);

		pPainter->translate(pfStart.x(), pfStart.y());

		pPainter->rotate(angel);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);

		pPainter->translate(-pfStart.x(), -pfStart.y());

		//渐变
		QLinearGradient myGradient;

		if (m_arrRelativePoints.count() != i && i!= 1)
		{
			QRadialGradient myRadio(pfStart, GetPenInfo()->GetWidth()/2.0,pfStart);
			//每个节点处画个圆  begin
			QRectF tfellp;
			tfellp.setX(pfStart.x() - GetPenInfo()->GetWidth() / 2.0);
			tfellp.setY(pfStart.y() - GetPenInfo()->GetWidth() / 2.0);
			tfellp.setWidth(GetPenInfo()->GetWidth());
			tfellp.setHeight(GetPenInfo()->GetWidth());

// 			myRadio.setStart(tfellp.topLeft());
// 			myRadio.setFinalStop(tfellp.bottomLeft());


			myRadio.setColorAt(0.0, info.GetEndColor());
			//myRadio.setColorAt(0.5, info.GetEndColor());
			myRadio.setColorAt(1, info.GetColor());

			QPainterPath tPath;
			tPath.addEllipse(tfellp);
			//pPainter->setBrush(myGradient);
			pPainter->fillPath(tPath, QBrush(myRadio));
		}

		myGradient.setStart(tref.topLeft());
		myGradient.setFinalStop(tref.bottomLeft());


		myGradient.setColorAt(0.0, info.GetColor());
		myGradient.setColorAt(0.5, info.GetEndColor());
		myGradient.setColorAt(1, info.GetColor());

		//pPainter->setBrush(myGradient);
		pPainter->fillRect(tref, QBrush(myGradient));



		pPainter->translate(pfStart.x(), pfStart.y());

		pPainter->rotate(-angel);
		//	QRectF rcRet = tr_temp.mapRect(rcInput);

		pPainter->translate(-pfStart.x(), -pfStart.y());


	}



	pPainter->restore();
}

void CLineWidget::SetFontLayout(QTextOption &txtOpt)
{
	if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_CENTER)
	{
		//中心
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_TOP)
	{
		//上
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_BOTTOM)
	{
		//下
		txtOpt.setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_LEFT)
	{
		//左
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::FONT_RIGHT)
	{
		//右
		txtOpt.setAlignment(Qt::AlignTop | Qt::AlignRight);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_LEFT)
	{
		//中  - 左
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	}
	else if (m_FontInfo.GetFontLayout() == CFontInfo::MID_RIGHT)
	{
		//中  - 右
		txtOpt.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	}
}

bool CLineWidget::SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter)
{
	Q_ASSERT(pXmlWriter);
	if (pXmlWriter == nullptr)
		return false;

	pXmlWriter->writeStartElement("Widget");
	pXmlWriter->writeAttribute("Type", QString::number(m_nObjType));
	//暂时没有用
	//pXmlWriter->writeAttribute("ClassType", QString::number(m_nClassType)); 
	pXmlWriter->writeAttribute("ID", QString::number(m_nID));
	//zvalue
	pXmlWriter->writeAttribute("ZValue", QString::number(this->zValue()));

	pXmlWriter->writeAttribute("Name", m_szName.c_str());
	pXmlWriter->writeAttribute("WidgetName", QString::fromLocal8Bit(m_szWidgetName.c_str()));
	pXmlWriter->writeAttribute("Pos.x", QString::number(m_rcPos.x()));
	pXmlWriter->writeAttribute("Pos.y", QString::number(m_rcPos.y()));
	//大小
	pXmlWriter->writeAttribute("Width", QString::number(m_rcPos.width()));
	pXmlWriter->writeAttribute("Height", QString::number(m_rcPos.height()));
	pXmlWriter->writeAttribute("RotateCenter", QString::number(m_eRotateCenter));
	//旋转角度
	pXmlWriter->writeAttribute("Rotate", QString::number(m_dblRotateAngle));

	if (m_nObjType == DRAW_TOOLS_POLYLINE || m_nObjType ==DRAW_TOOLS_POLYGON || m_nObjType == DRAW_TOOLS_PIPE)
	{
		//折线和多边形都要记录点信息
		for each (QPointF var in m_arrPonits)
		{
			//折线 需要记录点的信息
			pXmlWriter->writeStartElement("LinePoint");
			QString strPoint = QString::number(var.x()) + "," + QString::number(var.y());

			pXmlWriter->writeCharacters(strPoint);
			pXmlWriter->writeEndElement();

		}

	}
	//静态属性
	SaveBaseXml(pXmlWriter);
	//动态属性
	SaveDynamicXml(pXmlWriter);

	pXmlWriter->writeEndElement();


	return true;
}

//绘制管道
void CLineWidget::DrawPipe(QPainter * pPainter)
{
	//管道
	if (m_arrRelativePoints.count() > 0)
	{
		if (m_dblRotateAngle != 0.0f)
		{
// 			QPointF ptCenter = GetRotationCenterPoint();
// 			QTransform tr_temp;
// 			tr_temp.reset();
// 			tr_temp.translate(ptCenter.x(), ptCenter.y());
// 			tr_temp = tr_temp.rotate(m_dblRotateAngle);
// 			//	QRectF rcRet = tr_temp.mapRect(rcInput);
// 			tr_temp.translate(-ptCenter.x(), -ptCenter.y());
// 
// 			p1.moveTo(m_arrRelativePoints.at(0) + GetPosition().topLeft());
// 
// 			//折线
// 			for each (QPointF pfClick in m_arrRelativePoints)
// 			{
// 				p1.lineTo(pfClick + GetPosition().topLeft());
// 			}
// 
// 			path = tr_temp.map(p1);

		}
		else
		{
			//path.moveTo(m_arrRelativePoints.at(0) + GetPosition().topLeft());


			//管道
// 			for each (QPointF pfClick in m_arrRelativePoints)
// 			{
// 				path.lineTo(pfClick + GetPosition().topLeft());
// 			}

			QPointF pfStart(m_arrRelativePoints.at(0) + GetPosition().topLeft());
			QPointF pfEnd(m_arrRelativePoints.at(0) + GetPosition().topLeft());

			QRadialGradient radialGradient(pfStart,GetPenInfo()->GetWidth(),pfEnd);
			radialGradient.setColorAt(0,GetBrushInfo()->GetColor());
			radialGradient.setColorAt(1,GetBrushInfo()->GetEndColor());

			QPen tPen(QBrush(radialGradient),GetPenInfo()->GetWidth());

			pPainter->setPen(tPen);

			pPainter->drawLine(QLine(pfStart.toPoint(),pfEnd.toPoint()));

			for (int i = 0; i < m_arrRelativePoints.count()-1; i++)
			{
				QPointF tStartPf = m_arrRelativePoints[i] + GetPosition().topLeft();
				QPointF tEndPf = m_arrRelativePoints[i+1] + GetPosition().topLeft();

				QRadialGradient radialGradient2(tStartPf, GetPenInfo()->GetWidth(), tEndPf);
				radialGradient2.setColorAt(0, GetBrushInfo()->GetColor());
				radialGradient2.setColorAt(1, GetBrushInfo()->GetEndColor());

				QPen tPen(QBrush(radialGradient2), GetPenInfo()->GetWidth());
				pPainter->setPen(tPen);

				pPainter->drawLine(QLine(tStartPf.toPoint(), tEndPf.toPoint()));

			}
		}

	}
}

double CLineWidget::CalculateRoutateAngel(QPointF pfStart, QPointF pfEnd)
{
	double angle = 0;

	if (pfEnd.x() >= pfStart.x() && pfEnd.y() >= pfStart.y())
	{
		//区间1
		qDebug() << sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) + (pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y()));
		qDebug() << (pfEnd.y() - pfStart.y());
		qDebug() << 1.0*(pfEnd.y() - pfStart.y()) / (sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) + (pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y())));

		angle = asin(1.0*(pfEnd.y()-pfStart.y())/(sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) + 
			(pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y())))) * 180 / 3.141592;
	}
	else if (pfEnd.x() <= pfStart.x() && pfEnd.y() >= pfStart.y())
	{
		//区间2
		angle = 180 - asin(1.0*(pfEnd.y() - pfStart.y()) / (sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) + 
			(pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y())))) * 180 / 3.141592;;
	}
	else if (pfEnd.x() <= pfStart.x() && pfEnd.y() <= pfStart.y())
	{
		//区间3
		angle = 180 + asin(1.0*(pfStart.y() - pfEnd.y()) / (sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) +
			(pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y())))) * 180 / 3.141592;;
	}
	else if (pfEnd.x() >= pfStart.x() && pfEnd.y() <= pfStart.y())
	{
		//区间4
		angle = 360 - asin(1.0*(pfStart.y() - pfEnd.y()) / (sqrt((pfEnd.x() - pfStart.x())*(pfEnd.x() - pfStart.x()) +
			(pfEnd.y() - pfStart.y())*(pfEnd.y() - pfStart.y())))) * 180 / 3.141592;;
	}

	return angle;
}

QRectF CLineWidget::boundingRect() const
{
	QRectF rcBounding = m_rcPos.normalized();
// 	double dPenWidth = m_PenInfo.GetWidth() / 2;
// 
// 	if (dPenWidth > 0.0f)
// 	{ // 扩大半个线宽
// 		rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
// 	}
	return rcBounding;
}

/** @}*/

// QPainterPath CLineWidget::shape() const
// {
//     QPainterPath p1, p2;
//     QRectF rcInput = boundingRect();
// 
//     if (m_dblRotateAngle != 0.0f)
//     {
//         QPointF ptCenter = GetRotationCenterPoint();
// 
//         QTransform tr_temp;
//         tr_temp.reset();
//         tr_temp.translate(ptCenter.x(), ptCenter.y());
//         tr_temp = tr_temp.rotate(m_dblRotateAngle);
//         //	QRectF rcRet = tr_temp.mapRect(rcInput);
//         tr_temp.translate(-ptCenter.x(), -ptCenter.y());
// 
//         QRectF rcShape = m_rcPos.normalized();
//         // 
//         p1.moveTo(rcShape.topLeft());
//         p1.lineTo(rcShape.right(), rcShape.bottom());
// 
//         p2 = tr_temp.map(p1);
//     }
//     else
//     {
//         QRectF rcShape = m_rcPos.normalized();
//         // 
//         p1.moveTo(rcShape.topLeft());
//         p1.lineTo(rcShape.right(), rcShape.bottom());
// 
//         p2.addRect(rcInput);
//     }
//     return p2;
// }

void CLineWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
{
    Q_ASSERT(pPainter);
    if (pPainter == nullptr)
        return;

    

    pPainter->save();

    if (bSingleSelected)
    {
        QPen pen(XGraphics::LightBlue);
        pen.setWidthF(m_PenInfo.GetWidth());
        pen.setStyle(Qt::DashLine);

        pPainter->setPen(pen);
    }
    else
    {// 多选的移动(或者组合图元的子图元)，原来是啥样，还是啥样
        QPen pen(Qt::darkGray);
        pen.setWidth(1);
        pen.setStyle(Qt::SolidLine);
        pPainter->setPen(pen);
    }

    QColor clBrush(XGraphics::LightBlue);
    clBrush.setAlpha(240);
    QBrush brBack;
    brBack.setColor(clBrush);
    brBack.setStyle(Qt::SolidPattern);

    //旋转坐标轴
    QPointF tCenter = GetRotationCenterPoint();

    if (GetRotateAngle() > -0.0001 && GetRotateAngle() < 0.0001)
    {
    }
    else
    {
//         tTramsform.translate(tCenter.x(), tCenter.y());
//         tTramsform.rotate(GetRotateAngle());
//         tTramsform.translate(-tCenter.x(), -tCenter.y());

        pPainter->translate(GetMovingRotationCenterPoint());
        pPainter->rotate(GetRotateAngle());
        pPainter->translate(-GetMovingRotationCenterPoint());
    }    

    
    pPainter->setRenderHint(QPainter::Antialiasing);

    pPainter->fillRect(m_rcMovingPos, brBack);

    pPainter->setPen(Qt::black);

	if (m_nObjType == DRAW_TOOLS_LINE)
	{
		pPainter->drawLine(m_rcMovingPos.x(), m_rcMovingPos.center().y(), m_rcMovingPos.right(), m_rcMovingPos.center().y());

	}
	if (m_nObjType == DRAW_TOOLS_POLYLINE)
	{
		QPainterPath path;
		GetMovingPath(path);
		pPainter->drawPath(path);
	}
    

    pPainter->restore();

//     pPainter->save();
//     QPainterPath path;
//     GetMovingPath(path);
// 
//     pPainter->drawPath(path);
//     pPainter->restore();

}

void  CLineWidget::GetMovingPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

	if (m_nObjType == DRAW_TOOLS_LINE)
	{
		if (m_dblRotateAngle != 0.0f)
		{
			QTransform tr_temp;
			tr_temp.reset();
			tr_temp.translate(GetMovingRotationCenterPoint().x(), GetMovingRotationCenterPoint().y());
			tr_temp = tr_temp.rotate(m_dblRotateAngle);
			//	QRectF rcRet = tr_temp.mapRect(rcInput);
			tr_temp.translate(-GetMovingRotationCenterPoint().x(), -GetMovingRotationCenterPoint().y());

			p1.moveTo(m_rcMovingPos.x(), m_rcMovingPos.center().y());
			p1.lineTo(m_rcMovingPos.right(), m_rcMovingPos.center().y());


			path = tr_temp.map(p1);
		}
		else
		{
			path.moveTo(QPointF(rcShape.topLeft().x(),rcShape.center().y()));
			path.lineTo(QPointF(rcShape.topRight().x(), rcShape.center().y()));
		}
	}
	else if (m_nObjType == DRAW_TOOLS_POLYLINE)
	{
		//折线
		if (m_arrRelativePoints.count() > 0)
		{
			if (m_dblRotateAngle != 0.0f)
			{
// 				QTransform tr_temp;
// 				tr_temp.reset();
// 				tr_temp.translate(m_rcMovingPos.center().x(), m_rcMovingPos.center().y());
// 				tr_temp = tr_temp.rotate(m_dblRotateAngle);
// 				//	QRectF rcRet = tr_temp.mapRect(rcInput);
// 				tr_temp.translate(-m_rcMovingPos.center().x(), -m_rcMovingPos.center().y());
// 
// 				QPointF pfStart;
// 				pfStart.setX(m_arrRelativePoints.at(0).x()*m_rcMovingPos.width() + m_rcMovingPos.topLeft().x());
// 				pfStart.setY(m_arrRelativePoints.at(0).y()*m_rcMovingPos.height() + m_rcMovingPos.topLeft().y());
// 
// 				p1.moveTo(pfStart);
// 
// 				//折线
// 				for each (QPointF pfClick in m_arrRelativePoints)
// 				{
// 					QPointF tPf;
// 					tPf.setX(pfClick.x()*m_rcMovingPos.width() + m_rcMovingPos.topLeft().x());
// 
// 					tPf.setY(pfClick.y()*m_rcMovingPos.height() + m_rcMovingPos.topLeft().y());
// 
// 					p1.lineTo(tPf);
// 				}
// 
// 				path = tr_temp.map(p1);

			}
			else
			{
				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*rcShape.width() + rcShape.topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*rcShape.height() + rcShape.topLeft().y());

				path.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*rcShape.width() + rcShape.topLeft().x());

					tPf.setY(pfClick.y()*rcShape.height() + rcShape.topLeft().y());

					path.lineTo(tPf);
				}
			}

		}

	}
	else if (m_nObjType == DRAW_TOOLS_POLYGON)
	{
		if (m_arrRelativePoints.count() > 0)
		{
			//多边形
			path.moveTo(rcShape.topLeft() + m_arrRelativePoints.at(0));
			//多边形
			for each (QPointF pfClick in m_arrRelativePoints)
			{
				path.lineTo(pfClick + rcShape.topLeft());
			}

			path.lineTo(rcShape.topLeft() + m_arrRelativePoints.at(0));
		}

	}

}

void CLineWidget::GetPath(QPainterPath& path)
{
     QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_nObjType == DRAW_TOOLS_LINE)
    {
        //直线
        //直线
		if (m_dblRotateAngle != 0.0f)
		{
			QPointF ptCenter = GetRotationCenterPoint();
			QTransform tr_temp;
			tr_temp.reset();
			tr_temp.translate(ptCenter.x(), ptCenter.y());
			tr_temp = tr_temp.rotate(m_dblRotateAngle);
			//	QRectF rcRet = tr_temp.mapRect(rcInput);
			tr_temp.translate(-ptCenter.x(), -ptCenter.y());

			p1.moveTo(QPointF(rcShape.topLeft().x(), rcShape.center().y()));
			p1.lineTo(QPointF(rcShape.topRight().x(), rcShape.center().y()));

			path = tr_temp.map(p1);
		}
		else
		{
			path.moveTo(QPointF(rcShape.topLeft().x(), rcShape.center().y()));
			path.lineTo(QPointF(rcShape.topRight().x(), rcShape.center().y()));
		}
    }
    else if (m_nObjType == DRAW_TOOLS_POLYLINE)
    {
		//折线
		if (m_arrRelativePoints.count() > 0)
		{
			if (m_dblRotateAngle != 0.0f)
			{
				QPointF ptCenter = GetRotationCenterPoint();
				QTransform tr_temp;
				tr_temp.reset();
				tr_temp.translate(ptCenter.x(), ptCenter.y());
				tr_temp = tr_temp.rotate(m_dblRotateAngle);
				//	QRectF rcRet = tr_temp.mapRect(rcInput);
				tr_temp.translate(-ptCenter.x(), -ptCenter.y());

				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				p1.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					p1.lineTo(tPf);
				}

				path = tr_temp.map(p1);

			}
			else
			{
				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				path.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					path.lineTo(tPf);
				}
			}

		}
    }
	else if (m_nObjType == DRAW_TOOLS_POLYGON)
	{
		//多边形 
		if (m_arrRelativePoints.count() > 0)
		{
			if (m_dblRotateAngle != 0.0f)
			{
				QPointF ptCenter = GetRotationCenterPoint();
				QTransform tr_temp;
				tr_temp.reset();
				tr_temp.translate(ptCenter.x(), ptCenter.y());
				tr_temp = tr_temp.rotate(m_dblRotateAngle);
				//	QRectF rcRet = tr_temp.mapRect(rcInput);
				tr_temp.translate(-ptCenter.x(), -ptCenter.y());

				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				p1.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					p1.lineTo(tPf);
				}

				p1.lineTo(pfStart);

				path = tr_temp.map(p1);

			}
			else
			{
				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				path.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					path.lineTo(tPf);
				}

				path.lineTo(pfStart);
			}

		}
	}
	else if (m_nObjType == DRAW_TOOLS_PIPE)
	{
		//管道
		if (m_arrRelativePoints.count() > 0)
		{
			if (m_dblRotateAngle != 0.0f)
			{
				QPointF ptCenter = GetRotationCenterPoint();
				QTransform tr_temp;
				tr_temp.reset();
				tr_temp.translate(ptCenter.x(), ptCenter.y());
				tr_temp = tr_temp.rotate(m_dblRotateAngle);
				//	QRectF rcRet = tr_temp.mapRect(rcInput);
				tr_temp.translate(-ptCenter.x(), -ptCenter.y());

				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				p1.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					p1.lineTo(tPf);
				}

				path = tr_temp.map(p1);

			}
			else
			{
				QPointF pfStart;
				pfStart.setX(m_arrRelativePoints.at(0).x()*GetPosition().width() + GetPosition().topLeft().x());
				pfStart.setY(m_arrRelativePoints.at(0).y()*GetPosition().height() + GetPosition().topLeft().y());

				path.moveTo(pfStart);

				//折线
				for each (QPointF pfClick in m_arrRelativePoints)
				{
					QPointF tPf;
					tPf.setX(pfClick.x()*GetPosition().width() + GetPosition().topLeft().x());

					tPf.setY(pfClick.y()*GetPosition().height() + GetPosition().topLeft().y());

					path.lineTo(tPf);
				}
			}

		}
	}
}

//往集合中插入点
void CLineWidget::InsertNewPoint(const QPointF &pfClick)
{
	m_arrPonits.append(pfClick);
	//更加新点确定rect 四个值
	double dLeft = m_arrPonits.first().x();
	double dTop = m_arrPonits.first().y();
	double dRight = m_arrPonits.first().x();
	double dBottom = m_arrPonits.first().y();

	for each (QPointF tPonit in m_arrPonits)
	{
		if (dLeft > tPonit.x())
		{
			//最左边
			dLeft = tPonit.x();
		}

		if (dTop > tPonit.y())
		{
			//最上面
			dTop = tPonit.y();
		}

		if (dRight < tPonit.x())
		{
			//最右边
			dRight = tPonit.x();
		}

		if (dBottom < tPonit.y())
		{
			//最下面
			dBottom = tPonit.y();
		}
	}

	MoveTo(QRectF(QPointF(dLeft,dTop),QPointF(dRight,dBottom)));
}

void CLineWidget::UpdateAllPoints()
{
	m_arrRelativePoints.clear();
	for each (QPointF tfPoint in m_arrPonits)
	{
		double dx = (tfPoint.x() - this->GetPosition().topLeft().x())/ this->GetPosition().width();
		qDebug() << this->GetPosition().topLeft().x();
		qDebug() << this->GetPosition().width();
		double dy = (tfPoint.y() - this->GetPosition().topLeft().y()) / this->GetPosition().height();
		//相对比例
		m_arrRelativePoints.append(QPointF(dx,dy));
	}
}
