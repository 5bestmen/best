
/*! @file barview.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  barview.h
�ļ�ʵ�ֹ��� :  ��״ͼ����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��״ͼ����
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
