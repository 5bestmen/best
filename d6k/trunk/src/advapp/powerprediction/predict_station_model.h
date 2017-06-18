#ifndef PREDICT_STATION_MODEL_H
#define PREDICT_STATION_MODEL_H

#include "predict_datainfo.h"
#include "datatypes.h"
#include "predict_custom_delegate.h"
#include "predict_station_table.h"

#include <QAbstractTableModel>
#include <QVector>
#include <QObject>


class IMainModuleInterface;


class CPredictStationModel : public QAbstractTableModel
{
	Q_OBJECT

public:

	enum StationColumn
	{
		ID, 
		Name, 
		Type,
		SelectPoint,
	};

public:

	CPredictStationModel(QVector<CPPPointInfo > &vecStnItems, QObject* parent = 0);

	~CPredictStationModel();

	//水平表头
	void SetHorizontalHeaderList(QStringList strListHorHead);

	//垂直表头
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

	//水平表头
	QStringList m_strListHorHead;

	CPlantInfo* m_StnData;

	QVector<CPPPointInfo > &m_vecStnItems;

	//水平表头map
	QMap<int32s, QString> m_mapIntStr;

};

#endif  //PREDICT_STATION_MODEL_H
