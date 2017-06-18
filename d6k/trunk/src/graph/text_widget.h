#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include "base_widget.h"

class CTextWidget : public CBaseWidget
{
public:
    CTextWidget();
    CTextWidget(QRectF rcPos);

    ~CTextWidget();

public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual QRectF boundingRect() const;

    virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected);

    virtual void GetPath(QPainterPath& path);

    void GetMovingPath(QPainterPath& path);

private:
    
};

#endif // TEXTWIDGET_H
