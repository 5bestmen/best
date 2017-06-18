
#include "text_widget.h"
#include "widget_factory.h"
#include "colour_define.h"
#include <QPainter>

CTextWidget::CTextWidget()
    : CBaseWidget()
{

}

CTextWidget::CTextWidget(QRectF rcPos) :CBaseWidget(rcPos)
{
    m_szTxt = QObject::tr("Text");
    m_nObjType = DRAW_TOOLS_TEXT;
}

CTextWidget::~CTextWidget()
{

}

void CTextWidget::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
//     Q_UNUSED(widget);
//     Q_UNUSED(option);
// 
//     Q_ASSERT(pPainter);
// 
//     if (pPainter == nullptr)
//     {
//         return;
//     }
//         
// 
//     pPainter->save();
// 
//     pPainter->setRenderHint(QPainter::Antialiasing);
// 
//     QPen pen;
//     m_PenInfo.ToQPen(pen);
//     pPainter->setPen(pen);
// 
//     QPointF ptCenter = GetRotationCenterPoint();
// 
// 
//     QColor clBrush(XGraphics::LightBlue);
//     clBrush.setAlpha(255);
//     QBrush brBack;
//     brBack.setColor(clBrush);
//     brBack.setStyle(Qt::SolidPattern);
// 
// 
//     QColor clBrush2(XGraphics::LightCoral);
//     clBrush2.setAlpha(255);
// 
//     CBrushInfo info;
//     info.SetStyle(Qt::SolidPattern);
//     info.SetColor(clBrush2);
// 
// 
//     clBrush.setRgb(XGraphics::YellowGreen);
//     clBrush.setAlpha(255);
//     brBack.setColor(clBrush);
//     
//     //	pPainter->fillRect(m_rcPos, brBack);
//     //	pPainter->drawRect(m_rcPos);
//     QPainterPath path;
//     GetPath(path);
// 
// 
//     //FillPath(pPainter, path, info);
//     
//     pPainter->drawPath(path);
// 
//     //m_szTxt = QString::fromLocal8Bit(m_szWidgetName.c_str());
//     if (m_szTxt.length())
//     {
//         //pPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
//         QTextOption txtOpt;
//         //txtOpt.
//         pPainter->setRenderHint(QPainter::TextAntialiasing);
//         pPainter->drawText(m_rcPos, m_szTxt, txtOpt);
// 
//     }
//     pPainter->restore();
//     // 画选择框
//     DrawSelectedTracker(pPainter);


    Q_UNUSED(widget);
    Q_UNUSED(option);

    Q_ASSERT(pPainter);
    if (pPainter == nullptr)
        return;


//     QPainterPath path;
//     GetPath(path);



    pPainter->save();
    QPen pen;
    pen.setWidth(1);
    pPainter->setPen(pen);
   // pPainter->setRenderHint(QPainter::Antialiasing);

    //pPainter->drawPath(path);




    if (m_szTxt.length())
    {
        QTextOption txtOpt;
        pPainter->setRenderHint(QPainter::TextAntialiasing);
        int nTextLength = m_szTxt.length();

        if (m_dblRotateAngle != 0.0f)
        {
            QPointF ptCenter = GetRotationCenterPoint();

            pPainter->translate(ptCenter);
            pPainter->rotate(m_dblRotateAngle);
            pPainter->translate(-ptCenter.x(), -ptCenter.y());
        }

        pPainter->drawText(QRectF(GetRotationCenterPoint().x() - nTextLength / 2 * 8, 
            GetRotationCenterPoint().y()-5, GetRotationCenterPoint().x() + nTextLength / 2 * 8, GetRotationCenterPoint().y()-5), m_szTxt, txtOpt);
    }

    pPainter->restore();
    

    DrawSelectedTracker(pPainter);
}

QRectF CTextWidget::boundingRect() const
{
    QRectF rcBounding = m_rcPos.normalized();
    double dPenWidth = m_PenInfo.GetWidth() / 2;

    if (dPenWidth > 0.0f)
    { // 扩大半个线宽
        rcBounding.adjust(0 - dPenWidth, 0 - dPenWidth, dPenWidth, dPenWidth);
    }
    return rcBounding;
}

void CTextWidget::DrawMovingWidget(QPainter *pPainter, bool bSingleSelected)
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
    if (GetRotateAngle() > -0.0001 && GetRotateAngle() < 0.0001)
    {
    }
    else
    {
        pPainter->setRenderHint(QPainter::Antialiasing);

        QPointF tCenter = GetRotationCenterPoint();

        pPainter->translate(m_rcMovingPos.center());
        pPainter->rotate(GetRotateAngle());
        pPainter->translate(-m_rcMovingPos.center());


    }


    pPainter->fillRect(m_rcMovingPos, brBack);


    pPainter->restore();

    pPainter->save();
    QPainterPath path;
    GetMovingPath(path);

    //     pPainter->fillPath(path, brBack);

    pPainter->setRenderHint(QPainter::Antialiasing);

    pPainter->drawPath(path);
    pPainter->restore();
}

void CTextWidget::GetPath(QPainterPath& path)
{
    QRectF rcShape = m_rcPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QPointF ptCenter = GetRotationCenterPoint();
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(ptCenter.x(), ptCenter.y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-ptCenter.x(), -ptCenter.y());

        p1.addRect(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRect(rcInput);
    }
}

void CTextWidget::GetMovingPath(QPainterPath& path)
{
    QRectF rcShape = m_rcMovingPos.normalized();

    QPainterPath p1;
    QRectF rcInput = boundingRect();

    if (m_dblRotateAngle != 0.0f)
    {
        QTransform tr_temp;
        tr_temp.reset();
        tr_temp.translate(m_rcMovingPos.center().x(), m_rcMovingPos.center().y());
        tr_temp = tr_temp.rotate(m_dblRotateAngle);
        //	QRectF rcRet = tr_temp.mapRect(rcInput);
        tr_temp.translate(-m_rcMovingPos.center().x(), -m_rcMovingPos.center().y());

        p1.addRect(rcShape);
        path = tr_temp.map(p1);
    }
    else
    {
        path.addRect(rcShape);
    }
}
