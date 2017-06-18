#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>
#include <QSet>
#include <QUndoStack>

class CTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum ITEM{NORMALITEM,MODIFIEDITEM};

	enum ROW{NORMALROW, DELEROW, MODIFIEDROW, INSERTROW};

public:
	CTableModel(QObject *parent, QStringList headerList);
	~CTableModel();

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

	void sort (int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	bool InsertRowsInModel(int position, int rows, QSet<int> &set);

	void emitDataChanged(const QModelIndex &index);

	QModelIndex createDataIndex(int row, int col, void *data = 0)
	{
		return QAbstractTableModel::createIndex(row, col, data);
	}

	QUndoStack *undoStackTest();

	void SaveDataModel(QString tableName);

	void AddRowModel(QVector<QString> &rowList, int rowType);

	void GetColumInfo(QMap<QString, QString> &desc2RealField);

	void RemoveRowModel(QVector<QString> &rowList, int rowType);
signals:
	//void sendData(void *sourceData, int type, int serial);
public:
	QStringList m_lstHorizontalHeader;
	QStringList m_lstVerticalHeader;
	QList< QVector<QString> > m_arrRow;
	QList< QVector<int> > m_lstItemStatus;

	QUndoStack *m_pStack;

	QList<int> m_lstRowStatus;

	QMap<QString, QString> m_desc2RealField;
};

#endif // TABLEMODEL_H
