#include "predict_inverter_table.h"

#include <QStandardItem>


CPredictInverterTable::CPredictInverterTable(QWidget* parent, QVector<CPPPointInfo >& vecInvterItem)
	:QTableView(parent)
{

	m_pPrdtInvterModel = new CPredictInverterModel(vecInvterItem, this);



	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(doubleClicked(const QModelIndex &), this, SLOT(DoubleClicked(const QModelIndex &)));

}



CPredictInverterTable::~CPredictInverterTable()
{


}



