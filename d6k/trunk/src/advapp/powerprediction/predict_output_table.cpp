#include "predict_output_table.h"

#include <QStandardItem>


CPredictOutputTable::CPredictOutputTable(QWidget* parent, QVector<CPPPointInfo >& vecWeaItem)
	:QTableView(parent)
{

	m_pPrdtOutputModel = new CPredictOutputModel(vecWeaItem, this);



	setContextMenuPolicy(Qt::CustomContextMenu);

	//connect(this, SIGNAL(doubleClicked(const QModelIndex &), this, SLOT(DoubleClicked(const QModelIndex &)));

}



CPredictOutputTable::~CPredictOutputTable()
{


}
