#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QColorDialog>

#include "dcolorpanel.h"
#include <qdrawutil.h>
DColorPanel::DColorPanel(QWidget *parent) 
	:QMenu(parent)
{
    setMouseTracking(true);
    setBackgroundRole(QPalette::Background);

    m_nIndex= NULLWIDGET;
    m_nHover= NULLWIDGET;
    m_bSunken = false;

    qint32 nLine= LINENUM;
    quint32 color=c_RGB(0x00,0x00,0x00);

    if(nLine>5)		nLine=5;
    if(nLine<1)		nLine=1;
    m_nLine=nLine;
    setFixedSize(150,59+18*nLine);

    setWidgetRect();
    setColor(color);
    setDefaultColor(color);
}

void DColorPanel::setDefaultColor(quint32 color)
{
    m_defaultColor=color;

    if(isVisible())
        update();
}

void DColorPanel::setColor(quint32 color)
{
    if(color==0xffffffff)
    {
        m_nIndex = TOPWIDGET;
    }
    else
    {
        static const qint32 crSize = sizeof(RGB)/sizeof(RGB[0]);

        for(int i=0;i< crSize ;i++)
        {
            if(color==RGB[i].color)
            {
                m_nIndex=i;
                break;
            }
        }
    }

    m_color=color;
}


void DColorPanel::mouseMoveEvent ( QMouseEvent * e)
{
    quint32 hover = widgetPosition(e);
    if(m_nHover != hover)
    {
        m_nHover = hover;
        update();
    }

    QMenu::mouseMoveEvent(e);
}

void DColorPanel::mousePressEvent ( QMouseEvent * e)
{
    quint32 hover = widgetPosition(e);
    m_nHover = hover;
    m_bSunken = true;
    update();

    QMenu::mousePressEvent(e);
}

void DColorPanel::mouseReleaseEvent ( QMouseEvent * e)
{
    qint32 hover = widgetPosition(e);
    if(hover == NULLWIDGET)
        return;
    if(m_nHover != hover)
    {
        m_nHover = hover;
    }
    if(m_nIndex != hover)
    {
        m_nIndex = hover;
        m_color = RGB[m_nIndex].color;
        QColor color =QColor(r_RGB(m_color),g_RGB(m_color),b_RGB(m_color));
        emit colorChanged(color);
    }
    m_bSunken = false;
    update();

    QMenu::mouseReleaseEvent(e);
}

quint32 DColorPanel::widgetPosition(QMouseEvent *e)
{
    qint32 hover= NULLWIDGET;
    QPoint point = e->pos();

    if(rect().contains(point))
    {
        if( m_rectTop.contains(point))
        {
            hover=TOPWIDGET;
        }
        else if(m_rectBottom.contains(point))
        {
            hover=BOTTOMWIDGET;
        }
        else
        {
            qint32 x=point.x()-m_rectColor[0].left();
            qint32 y=point.y()-m_rectColor[0].top();

            if( x>=8*18||\
                x<0||\
                y>=m_nLine*18||\
                y<0)
            {
                hover=NULLWIDGET;
            }
            else
            {
                hover=y/18*8+x/18;
            }
        }
    }
    return hover;
}

void DColorPanel::paintEvent(QPaintEvent * )
{
    QPainter painter(this);
    painter.setBackground(QColor(255,255,255));
    painter.setPen(currentPalette().button().color());
    painter.setBrush(currentPalette().button());
    painter.drawRect(0,0,width(),height());

    drawTopWidget(&painter);
    drawMidWidget(&painter);
    drawBottomWidget(&painter);
    redrawWidget(&painter);
    setCurrentWidget(&painter);
    painter.end();
}

void DColorPanel::leaveEvent(QEvent *)
{
    update();
}

bool DColorPanel::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        if (m_rectBottom.contains(helpEvent->pos()))
        {
            QToolTip::showText(helpEvent->globalPos(), m_bottomName);
        }
        else if(false)
        {
            for(int i=0;i<m_nLine*8;i++)
            {
                if(m_rectColor[i].contains(helpEvent->pos()))
                {
                    QToolTip::showText(helpEvent->globalPos(), "reserve");
                    break;
                }
            }
        }
        else
        {
            QToolTip::hideText();
            event->ignore();
        }

        return true;
    }
    return QMenu::event(event);
}

QPalette DColorPanel::currentPalette()
{
    QPalette cPalette = palette();
    cPalette.setCurrentColorGroup(QPalette::Normal);
    return cPalette;
}

void DColorPanel::setWidgetRect()
{
    QRect rect( 0, 0, width(), height());

	//m_topName = tr("transparent");
    m_topName = tr("透明");
    m_rectTop.setLeft  ( rect.left()+3 );
    m_rectTop.setRight ( rect.right()-3);
    m_rectTop.setTop   ( rect.top()+3 );
    m_rectTop.setBottom( rect.top()+24) ;

   /* m_bottomName=tr("others...");*/
	 m_bottomName=tr("其他...");
    m_rectBottom.setLeft  ( rect.left() +3 );
    m_rectBottom.setRight ( rect.right()-3);
    m_rectBottom.setTop   ( rect.bottom()-24);
    m_rectBottom.setBottom( rect.bottom()-3);

    for(int j=0;j<m_nLine;j++)
    {
        for(int i=0;i<8;i++)
        {
            m_rectColor[j*8+i].setLeft(3+i*18);
            m_rectColor[j*8+i].setRight(3+(i+1)*18);
            m_rectColor[j*8+i].setTop(j*18+m_rectTop.bottom()+2);
            m_rectColor[j*8+i].setBottom(j*18+m_rectTop.bottom()+20);
        }
    }

}

void DColorPanel::drawTopWidget(QPainter *painter)
{

    painter->setPen(currentPalette().dark().color());
    painter->setBrush(currentPalette().button().color());
    painter->drawRect(m_rectTop.left()+3,\
        m_rectTop.top()+3,\
        m_rectTop.right()-m_rectTop.left()-6,\
        m_rectTop.bottom()-m_rectTop.top()-6);
    painter->setPen(currentPalette().text().color());
    painter->drawText(m_rectTop,\
        Qt::AlignCenter|Qt::AlignVCenter,\
        m_topName);
}

void DColorPanel::drawMidWidget(QPainter *painter)
{
    painter->setPen(currentPalette().dark().color());
    for(int i=0;i<8*m_nLine;i++)
    {
        quint32 color=RGB[i].color;
        painter->setBrush( QBrush(QColor(r_RGB(color),g_RGB(color),b_RGB(color))));
        painter->drawRect( m_rectColor[i].left()+3,m_rectColor[i].top()+3,12,12);
    }

    painter->setPen(currentPalette().dark().color());

    painter->drawLine(m_rectBottom.left(),\
        m_rectBottom.top()-4,\
        m_rectBottom.right()-m_rectBottom.left(),\
        m_rectBottom.top()-4);
}

void DColorPanel::drawBottomWidget(QPainter *painter)
{
    painter->setPen(currentPalette().base().color());
    painter->drawLine(m_rectBottom.left(),\
        m_rectBottom.top()-3,\
        m_rectBottom.right()-m_rectBottom.left(),\
        m_rectBottom.top()-3);

    painter->setPen(currentPalette().button().color());
    painter->setBrush(currentPalette().button().color());
    painter->drawRect(m_rectBottom);
    painter->setPen(currentPalette().text().color());
    painter->drawText(m_rectBottom,\
        Qt::AlignCenter|Qt::AlignVCenter,\
        m_bottomName);
}

void DColorPanel::redrawWidget(QPainter *painter)
{
    switch(m_nHover)
    {
    case TOPWIDGET:
    {
        qDrawShadePanel(painter,m_rectTop,currentPalette(),m_bSunken);
        break;
    }
    case BOTTOMWIDGET:
    {
        qDrawShadePanel(painter,m_rectBottom,currentPalette(),m_bSunken);
        break;
    }
    case NULLWIDGET:
    {
        break;
    }
    default:
    {
        qDrawShadePanel(painter,m_rectColor[m_nHover],currentPalette(),m_bSunken);
        painter->setPen(Qt::red);
        quint32 color=RGB[m_nHover].color;
        painter->setBrush( QBrush(QColor(r_RGB(color),g_RGB(color),b_RGB(color))));
        painter->drawRect(m_rectColor[m_nHover].left()+3,m_rectColor[m_nHover].top()+3,12,12);
    }
    }
}
void DColorPanel::setCurrentWidget(QPainter *painter)
{
    if(m_nIndex == NULLWIDGET)
        return;
    switch(m_nIndex)
    {
    case TOPWIDGET:
    {
        QPalette tPalette=currentPalette();
        QBrush nBrush=QBrush(Qt::Dense6Pattern);
        qDrawShadePanel(painter,m_rectTop,tPalette,true,1,&nBrush);
        drawTopWidget(painter);
                break;
    }
    case NULLWIDGET:
    case BOTTOMWIDGET:
    {
        break;
    }
    default:
    {
        QPalette tPalette=currentPalette();
        QBrush nBrush=QBrush(Qt::Dense6Pattern);
        qDrawShadePanel(painter,m_rectColor[m_nIndex],tPalette,true,1,&nBrush);
        painter->setPen(currentPalette().dark().color());
        quint32 color=RGB[m_nIndex].color;
        painter->setBrush( QBrush(QColor(r_RGB(color),g_RGB(color),b_RGB(color))));
        painter->drawRect(m_rectColor[m_nIndex].left()+3,m_rectColor[m_nIndex].top()+3,12,12);
    }
    }
}
