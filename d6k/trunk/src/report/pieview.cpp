#include <QVBoxLayout>
#include "pieview.h"

CPieView::CPieView(CCtrlData* pData,QWidget *parent)
	: QLabel(parent)
{
	Q_ASSERT(pData);
	m_CtrlData = pData;
	
	InitView();
}

CPieView::~CPieView()
{

}

void CPieView::paintEvent(QPaintEvent *e)
{
	Q_UNUSED(e);
}


void CPieView::InitView()
{
	QPieSeries *series = new QPieSeries();

	QVector< CCtrlGernalValue* > m_Val = m_CtrlData->GetCtrlVecData();

	for (auto i:m_Val)
	{
		series->append(i->m_szName,i->GetXSumData());
	}

	series->setLabelsVisible(true);
		
	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->setTitle(m_CtrlData->GetCtrlName());

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);

	QVBoxLayout* pLay = new QVBoxLayout;
	pLay->addWidget(chartView);
	this->setLayout(pLay);
}