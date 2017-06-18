#include "curveplot_wgt.h"
#include <QLineSeries>
#include <QObject>
#include <math.h>
#include <QLegendMarker>
#include <QDebug>
#include <QChartView>

CCurvePlotWgt::CCurvePlotWgt(QGraphicsItem *parent):QChart(parent)
{
	//m_chartView = new QChartView(this,nullptr);


	AddSeries();
	AddSeries();
	AddSeries();
	AddSeries();

	for(QLegendMarker* marker :  this->legend()->markers()) 
	{
		QObject::connect(marker, SIGNAL(clicked()), this, SLOT(Slot_HandleMarkerClicked()));
	}

	UpdateChart();
}

CCurvePlotWgt::~CCurvePlotWgt()
{
	//m_chartView->deleteLater();
}

void CCurvePlotWgt::AddSeries()
{
	QLineSeries *series = new QLineSeries();
	m_series.append(series);

	series->setName(QString("line " + QString::number(m_series.count())));

	QList<QPointF> data;

	int offset = this->series().count();

	for (int i = 0; i < 360; i++) {
		qreal x = offset * 20 + i;
		data.append(QPointF(i, sin(2.0 * 3.141592 * x / 360.0)));
	}

	series->append(data);
	this->addSeries(series);

	if (m_series.count() == 1)
	{
		this->createDefaultAxes();
	}
}


void CCurvePlotWgt::UpdateChart()
{
	auto pBrush = m_BrushInfo.CreateQBrush();
 	setBackgroundBrush(*pBrush);
}


void CCurvePlotWgt::Slot_HandleMarkerClicked()
{
	QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
	Q_ASSERT(marker);

	switch (marker->type())
	{
	case QLegendMarker::LegendMarkerTypeXY:
	{
		marker->series()->setVisible(!marker->series()->isVisible());

		marker->setVisible(true);
		qreal alpha = 1.0;

		if (!marker->series()->isVisible()) {
			alpha = 0.5;
		}

		QColor color;
		QBrush brush = marker->labelBrush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setLabelBrush(brush);

		brush = marker->brush();
		color = brush.color();
		color.setAlphaF(alpha);
		brush.setColor(color);
		marker->setBrush(brush);

		QPen pen = marker->pen();
		color = pen.color();
		color.setAlphaF(alpha);
		pen.setColor(color);
		marker->setPen(pen);

		//![6]
		break;
	}
	default:
	{
		qDebug() << "Unknown marker type";
		break;
	}
	}
}

// QChartView * CCurePlotWgt::GetChartView()
// {
// 	return m_chartView;
// }
