#ifndef PREDICT_WEATHER_MODEL_H
#define PREDICT_WEATHER_MODEL_H

#include "predict_datainfo.h"
#include "datatypes.h"
#include "predict_custom_delegate.h"
#include "predict_weather_table.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QObject>


class IMainModuleInterface;


class CPredictWeatherModel : public QAbstractTableModel
{
	Q_OBJECT

public:

	enum WeatherColumn
	{
		ID,
		Name,
		Type,
		SelectPoint,
	};

public:

	CPredictWeatherModel(QVector<CPPPointInfo > &vecWeaItems, QObject* parent = 0);

	~CPredictWeatherModel();

	//ˮƽ��ͷ
	void SetHorizontalHeaderList(QStringList strListHorHead);

	//��ֱ��ͷ
	void SetVerticalHeaderList(QStringList strListVerHead);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool ModifyContentData(const QModelIndex& index, const QVariant& value, int& role);

public:

	void SetColumnCount(int columns);

	void SetRowCount(int rows);

private:

	QVariant ShowData(int nRow, int nCol) const;

private:

	//ˮƽ��ͷ
	QStringList m_strListHorHead;

	CWeatherData* m_WeaData;

	QVector<CPPPointInfo > &m_vecWeaItems;

	//ˮƽ��ͷmap
	QMap<int32s, QString> m_mapWeaStr;

};

#endif  //PREDICT_WEATHER_MODEL_H

