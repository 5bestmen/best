#include "predict_weather_table.h"

#include <QStandardItem>


CPredictWeatherTable::CPredictWeatherTable(QWidget* parent, QVector<CPPPointInfo >& vecWeaItem)
	:QTableView(parent)
{

	m_pPrdtWeaModel = new CPredictWeatherModel(vecWeaItem, this);



	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(doubleClicked(const QModelIndex &), this, SLOT(DoubleClicked(const QModelIndex &)));

}



CPredictWeatherTable::~CPredictWeatherTable()
{


}
