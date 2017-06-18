
/*! @file barview.h
<PRE>
********************************************************************************
模块名       :
文件名       :  barview.h
文件实现功能 :  柱状图定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   柱状图定义
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/
#ifndef BARVIEW_H
#define BARVIEW_H

#include <QLabel>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include "configinfo.h"

QT_CHARTS_USE_NAMESPACE

class CBarView : public QLabel
{
	Q_OBJECT

public:
	CBarView(CCtrlData* pData,QWidget *parent = Q_NULLPTR);
	~CBarView();

	void InitView();

private:
	CCtrlData* m_CtrlData;
	
};

#endif // BARVIEW_H
