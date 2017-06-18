#include <QVBoxLayout>
#include "chartview.h"

CChartView::CChartView(CCtrlData* pData, QWidget *parent)
	: QLabel(parent)
{
	Q_ASSERT(pData);
	m_CtrlData = pData;
	InitView();
}

CChartView::~CChartView()
{

}

void CChartView::InitView()
{
	QChart *m_Chart = new QChart();
	m_Chart->setTitle(m_CtrlData->GetCtrlName());
	for (auto i:m_CtrlData->GetCtrlVecData())
	{
		QSplineSeries *series = new QSplineSeries;
		series->setName(i->m_szName);
		for (auto j:i->m_vecValue)
		{
			series->append(j.m_fYais,j.m_fXais);
		}
		m_Chart->addSeries(series);
	}
	m_Chart->createDefaultAxes();
	m_Chart->setAcceptHoverEvents(true);


	QChartView *chartView = new QChartView(m_Chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QVBoxLayout* pLay = new QVBoxLayout;
	pLay->addWidget(chartView);
	this->setLayout(pLay);
}