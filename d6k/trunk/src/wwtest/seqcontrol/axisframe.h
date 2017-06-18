/*! @file axisframe.h
<PRE>
********************************************************************************
模块名       :  自定义坐标轴
文件名       :  axisframe.h
文件实现功能 :  
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
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

protected:
    virtual void paintEvent(QPaintEvent *event);
private:
    //1横轴   2纵轴
    int m_nCoordType;
};

#endif // CAXISFRAME_H
