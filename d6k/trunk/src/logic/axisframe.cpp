/*! @file
<PRE>
********************************************************************************
模块名       :  自定义坐标轴
文件名       :  axisframe.cpp
文件实现功能 :  自定义横坐标和纵坐标
作者         :  wangwei
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   自定义坐标轴
*  @author  wangewei
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#include <QDebug>
#include <QPainter>
#include <QWidget>
#include "axisframe.h"

CAxisFrame::CAxisFrame(int nType, QWidget *parent):
    QFrame(parent)
{
    m_nCoordType = nType;
    this->setContentsMargins(0,0,0,0);
}

CAxisFrame::~CAxisFrame()
{

}

/*********************************************************************************************************
** \brief CAxisFrame::DrawHorizontalAixs
** \details 画横轴
** \param nPainter
** \return void
** \author wangwei
** \date 2016年8月31日
* * \note
********************************************************************************************************/
void CAxisFrame::DrawHorizontalAixs(QPainter &nPainter)
{
    QWidget* pParent = qobject_cast<QWidget*>(this->parent());

    Q_ASSERT(pParent);

    if (pParent == nullptr)
    {
        return;
    }

    int nWidth = pParent->width();

    this->setFixedSize(nWidth,20);

    nPainter.drawLine(20,19,nWidth,20);

    int nNums = 0;
    for (int i=20; i<nWidth; i+=15)
    {

        if (nNums%10 == 0)
        {
            nPainter.drawText(i-3,10,QString::number(nNums));
        }

        if (nNums%5 == 0)
        {
            nPainter.drawLine(i,19,i,11);
        }
        else
        {
            nPainter.drawLine(i,19,i,14);
        }
        nNums++;
    }
}

/*********************************************************************************************************
** \brief CAxisFrame::DrawVorizontalAixs
** \details 画纵轴
** \param nPainter
** \return void
** \author wangwei
** \date 2016年8月31日
* * \note
********************************************************************************************************/
void CAxisFrame::DrawVorizontalAixs(QPainter &nPainter)
{
    QWidget* pParent = qobject_cast<QWidget*>(this->parent());

    Q_ASSERT(pParent);

    if (pParent == nullptr)
    {
        return;
    }

    int nHeight = pParent->height();

    this->setFixedSize(20, nHeight);

    nPainter.drawLine(19, 20, 19, nHeight);

    int nNums = 0;
    for (int i = 20; i < nHeight; i += 15)
    {

        if (nNums % 10 == 0 && nNums != 0)
        {
            nPainter.drawText(2, i+4, QString::number(nNums));
        }

        if (nNums % 5 == 0 && nNums % 10 != 0)
        {
            nPainter.drawLine(19, i, 11, i);
        }
        else
        {
            nPainter.drawLine(19, i, 14, i);
        }
        nNums++;
    }
}

/*********************************************************************************************************
** \brief CAxisFrame::paintEvent
** \details 重写paintevent
** \param event
** \return void
** \author wangwei
** \date 2016年8月31日
* * \note
********************************************************************************************************/
void CAxisFrame::paintEvent(QPaintEvent *event)
{
    //划线
    QPainter painter(this);
    painter.setPen(QPen(QColor(133,133,133), 1, Qt::SolidLine));

    if (m_nCoordType == 1)
    {
        //横轴
        DrawHorizontalAixs(painter);
    }
    else if (m_nCoordType == 2)
    {
        //纵轴
        DrawVorizontalAixs(painter);
    }
    else
    {
        //donothing
    }
    QFrame::paintEvent(event);
}
