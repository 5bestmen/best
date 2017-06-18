
/*! @file chartview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  chartview.h
�ļ�ʵ�ֹ��� :  ����ͼ����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ����ͼ����
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
