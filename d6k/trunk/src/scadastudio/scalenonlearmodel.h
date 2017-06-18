#ifndef SCALENONLEARMODEL_H
#define SCALENONLEARMODEL_H

#include <QAbstractTableModel>
#include "config_data.h"

class CQuoteItem;

class CScaleNonlearModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CScaleNonlearModel(QObject *parent, Config::CFesData *pFesData, CQuoteItem *pItem);
	~CScaleNonlearModel();
	
	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	void RefrushModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool InsertRowsInModel(int position, int rows, QSet<int> &set);

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

private:
	Config::CFesData *m_pFesData;

	QStringList horizontal_header_list;
};

#endif // SCALENONLEARMODEL_H
