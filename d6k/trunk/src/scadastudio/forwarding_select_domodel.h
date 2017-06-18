#ifndef DOMODEL_H
#define DOMODEL_H

#include <QAbstractTableModel>
#include "scadastudiodefine.h"
#include "datatypes.h"

class QUndoStack;

namespace Config
{
	class CDevice;
}

class IMainModuleInterface;

class CForwardingDOSelectDataModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, InitQua, Address
	};

public:
	CForwardingDOSelectDataModel(QObject *parent, Config::CDevice *pDevice);
	~CForwardingDOSelectDataModel();

	void SetHorizontalHeaderList(QStringList horizontalHeaderList);

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void SetModalDatas(QList< QVector<QString> > &rowlist);

	void RefrushModel();

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

	QMap<int32s, CONFIG_PARA> &GetColumnInfo();

private:
	void InitPara();

private:
	QStringList horizontal_header_list;
	Config::CDevice *m_pDevice;
};

#endif // DOMODEL_H
