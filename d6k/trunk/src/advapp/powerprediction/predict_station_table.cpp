#include "predict_station_table.h"

#include <QStandardItem>


CPredictStationTable::CPredictStationTable(QWidget* parent, QVector<CPPPointInfo >& vecStnItem)
	:QTableView(parent)
{
	
	m_pPrdtStnModel = new CPredictStationModel(vecStnItem, this);

	

	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(doubleClicked(const QModelIndex &), this, SLOT(DoubleClicked(const QModelIndex &)));
	
}



CPredictStationTable::~CPredictStationTable()
{


}
