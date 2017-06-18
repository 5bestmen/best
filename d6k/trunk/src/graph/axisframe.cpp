

#include "axisframe.h"

#include <QDebug>
#include <QPainter>
#include <QWidget>

CAxisFrame::CAxisFrame(int nType, QWidget *parent):
    QFrame(parent)
{
    m_nCoordType = nType;
    //当前x所在位置
    m_nCurrentXPosition = 0;
    //当前y所在位置
    m_nCurrentYPosition = 0;
    this->setContentsMargins(0,0,0,0);
}

CAxisFrame::~CAxisFrame()
{

}

void CAxisFrame::DrawHorizontalAixs(QPainter &nPainter)
{

    int nWidth = qobject_cast<QWidget*>(this->parent())->width();

    this->setFixedSize(nWidth,15);

    //nPainter.drawLine(13,13,nWidth,13);

    int nNums = 0;

    for (int i=13; i<nWidth; i+=10)
    {

        if (nNums%4 == 0)
        {
            nPainter.drawText(i, 10, QString::number(nNums/4%10));
        }
        else if (nNums%2 == 0)
        {
            nPainter.drawLine(i, 2, i, 8);
        }
        else
        {
            nPainter.drawLine(i, 4, i, 6);
        }
        nNums++;
    }
}

void CAxisFrame::DrawVorizontalAixs(QPainter &nPainter)
{
    int nHeight = qobject_cast<QWidget*>(this->parent())->height();

    this->setFixedSize(15, nHeight);

    //nPainter.drawLine(13, 13, 13, nHeight);

    int nNums = 0;
    for (int i = 13; i < nHeight; i += 10)
    {

        if (nNums % 4 == 0)
        {
            if (nNums == 0)
            {
                nPainter.drawLine(2, i, 8, i);
            }
            else
            {
                nPainter.drawText(4, i + 5, QString::number(nNums / 4 % 10));
            }
            
        }
        else if (nNums % 2 == 0)
        {
            nPainter.drawLine(2, i, 8, i);
        }
        else
        {
            nPainter.drawLine(4, i, 6, i);
        }
        nNums++;
    }

}

void CAxisFrame::paintEvent(QPaintEvent *event)
{
    //划线
    QPainter painter(this);
    painter.setPen(QPen(QColor(133,133,133), 1, Qt::SolidLine));

    if (m_nCoordType == 1)
    {
        //横轴
        DrawHorizontalAixs(painter);
        if (m_nCurrentXPosition != 0)
        {
            painter.drawLine(m_nCurrentXPosition, 1, m_nCurrentXPosition, 12);
        }
        
    }
    else if (m_nCoordType == 2)
    {
        //纵轴
        DrawVorizontalAixs(painter);
        if (m_nCurrentYPosition != 0)
        {
            painter.drawLine(1, m_nCurrentYPosition, 12, m_nCurrentYPosition);
        }
    }
    else
    {
        //donothing
    }
    QFrame::paintEvent(event);
}

void CAxisFrame::UpdateCurrentHorizontal(int nXposition)
{
    //13为起始偏移量
    m_nCurrentXPosition = nXposition + 13;
    update();
}

void CAxisFrame::UpdateCurrentVorizontal(int nYposition)
{
    m_nCurrentYPosition = nYposition + 13;
    update();
}
