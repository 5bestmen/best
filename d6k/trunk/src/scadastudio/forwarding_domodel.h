#ifndef DOMODEL_H
#define DOMODEL_H

#include <QAbstractTableModel>
#include "scadastudiodefine.h"
#include "datatypes.h"

class QUndoStack;

namespace Config
{
	class CForwardingChannelData;
	class CFesData;
	class CForwardingDigitalOutputData;
}

class IMainModuleInterface;

class CForwardingDOModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, SourceTagName, OccNo, BlockNo, GroupNo, EntryNo, Address, FUN, INF, AssociatedDI, BlockingSignalTag,
		BlockingSignalCondition
	};

public:
	CForwardingDOModel(QObject *parent, IMainModuleInterface *pCore, Config::CForwardingChannelData *pChannelData, Config::CFesData *pFes);
	~CForwardingDOModel();

	std::vector <Config::CForwardingDigitalOutputData *> &GetDOs();

	void RefrushModel()
	{
		beginResetModel();
		endResetModel();
	}

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

public:
	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	//void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	QList<QVector<QString>> &GetDataList();

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

private:
	void InitPara();

private:
	QStringList horizontal_header_list;

	Config::CForwardingChannelData *m_pChannel;

	Config::CFesData *m_pFes;

	IMainModuleInterface *m_pCore;
};

#endif // DOMODEL_H
