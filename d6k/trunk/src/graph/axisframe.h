#ifndef CAXISFRAME_H
#define CAXISFRAME_H
#include <QFrame>

class CAxisFrame : public QFrame
{
public:
    CAxisFrame(int nType,QWidget *parent = 0);
    ~CAxisFrame();
    //划横轴
    void DrawHorizontalAixs(QPainter &nPainter);
    //划纵轴
    void DrawVorizontalAixs(QPainter &nPainter);

    //更新当前所在位置
    void UpdateCurrentHorizontal(int nXposition);
    //更新当前y所在位置
    void UpdateCurrentVorizontal(int nYposition);

protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    //1横轴   2纵轴
    int m_nCoordType;
    //当前x位置
    int m_nCurrentXPosition;
    //当前y位置
    int m_nCurrentYPosition;
};

#endif // CAXISFRAME_H
