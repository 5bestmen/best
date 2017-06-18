#ifndef DCOLORPANEL_H
#define DCOLORPANEL_H

#include <QMenu>
#include "dbasicwidget_global.h"

#define TOPWIDGET       -2
#define BOTTOMWIDGET    -1
#define LINENUM         5

class  DColorPanel : public QMenu
{
    Q_OBJECT
public:
    explicit DColorPanel(QWidget *parent = 0);

    void setDefaultColor(quint32 color);
    void setColor(quint32 color);

protected:

    void mouseMoveEvent ( QMouseEvent * ) ;
    void mousePressEvent ( QMouseEvent * ) ;
    void mouseReleaseEvent ( QMouseEvent * );

    void paintEvent(QPaintEvent *);
    void leaveEvent(QEvent *);
    bool event(QEvent *event);

    QPalette currentPalette();

protected:
    void setWidgetRect();
    void drawTopWidget(QPainter *painter);
    void drawMidWidget(QPainter *painter);
    void drawBottomWidget(QPainter *painter);
    void redrawWidget(QPainter *painter);
    quint32 widgetPosition(QMouseEvent *);
    void setCurrentWidget(QPainter *painter);

signals:
    void colorChanged(QColor color);

 private:
    QRect m_rectTop;
    QRect m_rectBottom;

    QString m_topName;
    QString m_bottomName;

    QRect m_rectColor[64];

    quint32 m_defaultColor;
    quint32 m_color;

    qint32 m_nIndex;
    qint32 m_nHover;
    bool   m_bSunken;
    qint32 m_nLine;
};

#endif // DCOLORPANEL_H
