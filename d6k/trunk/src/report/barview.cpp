#include <QVBoxLayout>
#include "barview.h"

CBarView::CBarView(CCtrlData* pData,QWidget *parent)
	: QLabel(parent)
{
	Q_ASSERT(pData);
	m_CtrlData = pData;
	InitView();
}

CBarView::~CBarView()
{

}
void CBarView::InitView()
{
	QBarSeries *series = new QBarSeries();

	for (auto i:m_CtrlData->GetCtrlVecData())
	{
		QBarSet* pSet = new QBarSet(i->m_szName);
		for (auto j:i->m_vecValue)
		{
			*pSet << j.m_fXais;
		}
		series->append(pSet);
	}

	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(m_CtrlData->GetCtrlName());
	chart->setAnimationOptions(QChart::SeriesAnimations);

	QBarCategoryAxis *axis = new QBarCategoryAxis();
	//axis->append(categories);
	chart->createDefaultAxes();
	chart->setAxisX(axis, series);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QVBoxLayout* pLay = new QVBoxLayout;
	pLay->addWidget(chartView);
	this->setLayout(pLay);
}
