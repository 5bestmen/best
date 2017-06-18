#ifndef PREDICT_OUTPUT_TABLE_H
#define PREDICT_OUTPUT_TABLE_H

#include "predict_output_model.h"

#include <QTableView>

class CPredictOutputModel;

class CPredictOutputTable : public QTableView
{
	Q_OBJECT

public:

	CPredictOutputTable(QWidget* parent, QVector<CPPPointInfo > &vecOutputItem);

	~CPredictOutputTable();

	public slots:

	//void DoubleClicked(const QModelIndex& index);
	//void ShowMouseRightButton(const QPoint& point);

signals:
	//void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

public:

	CPredictOutputModel*  m_pPrdtOutputModel;

};
#endif  //PREDICT_OUTPUT_TABLE_H