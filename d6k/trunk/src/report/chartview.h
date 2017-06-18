
/*! @file chartview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  chartview.h
文件实现功能 :  曲线图定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   曲线图定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/
#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QLabel>
#include <QtWidgets/QGraphicsScene>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include "configinfo.h"

QT_CHARTS_USE_NAMESPACE

class CChartView : public QLabel
{
	Q_OBJECT

public:
	CChartView(CCtrlData* pData,QWidget *parent = Q_NULLPTR);
	~CChartView();
	void InitView();
private:
	CCtrlData* m_CtrlData;
};

#endif // CHARTVIEW_H
