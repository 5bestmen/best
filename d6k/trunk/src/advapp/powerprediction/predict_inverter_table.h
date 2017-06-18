#ifndef   PREDICT_INVERTER_TABLE_H
#define   PREDICT_INVERTER_TABLE_H

#include "predict_inverter_model.h"

#include <QTableView>

class CPredictInverterModel;

class CPredictInverterTable : public QTableView
{
	Q_OBJECT

public:

	CPredictInverterTable(QWidget* parent, QVector<CPPPointInfo > &vecInvtersItem);
	~CPredictInverterTable();

	public slots:

	//void DoubleClicked(const QModelIndex& index);
	//void ShowMouseRightButton(const QPoint& point);

signals:
	//void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

public:

	CPredictInverterModel*  m_pPrdtInvterModel;

};
#endif  //PREDICT_INVERTER_TABLE_H