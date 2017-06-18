#ifndef SYSTEMOUTPUTMODEL_H
#define SYSTEMOUTPUTMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QList>

class CSystemOutputModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CSystemOutputModel(QObject *parent);
	~CSystemOutputModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	void addRow(QVector<QString> rowVec);

	void clearAll();

	bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool setData(const QModelIndex &index, const QVariant &value, int role /* = Qt::EditRole */);

	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	QList< QVector<QString> > arr_row_list;
};

#endif // SYSTEMOUTPUTMODEL_H
