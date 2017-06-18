#ifndef PREDICT_WEATHER_TABLE_H
#define PREDICT_WEATHER_TABLE_H

#include "predict_weather_model.h"

#include <QTableView>

class CPredictWeatherModel;

class CPredictWeatherTable : public QTableView
{
	Q_OBJECT

public:

	CPredictWeatherTable(QWidget* parent, QVector<CPPPointInfo > &vecWeaItem);

	~CPredictWeatherTable();

	public slots:

	//void DoubleClicked(const QModelIndex& index);
	//void ShowMouseRightButton(const QPoint& point);

signals:
	//void SendSelectedRows(QSet<int32s> &set, int32s nType, void *pModel);

public:

	CPredictWeatherModel*  m_pPrdtWeaModel;

};
#endif  //PREDICT_WEATHER_TABLE_H