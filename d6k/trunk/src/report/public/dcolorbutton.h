#ifndef DCOLORBUTTON_H
#define DCOLORBUTTON_H

#include <QToolButton>
#include "dbasicwidget_global.h"

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QColor)
QT_END_NAMESPACE

class  DColorButton : public QToolButton
{
    Q_OBJECT
public:
    explicit DColorButton(QWidget *parent = 0);
    QColor color() const {return m_color;}

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
    void setColor(const QColor &color);

private:
    QColor m_color;
};

#endif // DCOLORBUTTON_H
