#ifndef PREDICT_STATION_TABLE_H
#define PREDICT_STATION_TABLE_H

#include "predict_station_model.h"

#include <QTableView>

class CPredictStationModel;

class CPredictStationTable : public QTableView
{
	Q_OBJECT

public:

	CPredictStationTable(QWidget* parent, QVector<CPPPointInfo > &vecStnItem);

	~CPredictStationTable();

public slots:

	//void DoubleClicked(const QModelIndex& index);
	//void ShowMouseRightButton(const QPoint& point);

signals:
	//void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

public:

	CPredictStationModel*  m_pPrdtStnModel;

};
#endif  //PREDICT_STATION_TABLE_H