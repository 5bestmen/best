#pragma once

#include <QChart>
#include <QList>
#include <QLineSeries>
#include <QLegendMarker>
#include <QChartView>
#include "base_widget.h"

using namespace QtCharts;

//曲线图
class CCurvePlotWgt : public QChart
{
	Q_OBJECT
public:
	CCurvePlotWgt(QGraphicsItem *parent = Q_NULLPTR);
	~CCurvePlotWgt();
	//增加曲线
	void AddSeries();
	//

	CBrushInfo* GetBrushInfo()
	{
		return &m_BrushInfo;
	}

	void UpdateChart();

public slots:
	void Slot_HandleMarkerClicked();

	//QChartView *GetChartView();
private:
	QList<QLineSeries *> m_series;

	//QChartView *m_chartView;
	//! 画刷信息
	CBrushInfo m_BrushInfo;

};
