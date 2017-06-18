/*! @file pieview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  pieview.h
文件实现功能 :  饼状图控件
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   饼状图控件
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/
#ifndef PIEVIEW_H
#define PIEVIEW_H
#include <QPaintEvent>
#include <QLabel>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include "configinfo.h"

QT_CHARTS_USE_NAMESPACE

class CPieView : public QLabel
{
	Q_OBJECT

public:
	CPieView(CCtrlData* pData,QWidget *parent = Q_NULLPTR);
	~CPieView();
protected:
	void paintEvent(QPaintEvent *e);
	void InitView();
	void SetPieValue();
private:
	CCtrlData* m_CtrlData;
};

#endif // PIEVIEW_H
