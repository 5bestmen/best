#include <QStylePainter>
#include <QStyleOptionToolButton>
#include "dcolorbutton.h"

DColorButton::DColorButton(QWidget *parent) :
    QToolButton(parent)
{
    m_color = QColor(0,0,0);
}

void DColorButton::paintEvent(QPaintEvent *e)
{
    QToolButton::paintEvent(e);
    QPainter painter(this);
    painter.setPen(m_color);
    painter.setBrush(m_color);
    painter.drawRect(6,20,16,3);
}

void DColorButton::setColor(const QColor &color)
{
    m_color = color;
    update();
}
